#include <stdint.h>
#include <iodefine.h>
#include "cmt0.h"

static tmr_waiting = 0;

static const unsigned char sw_select_packet[16] = {0,1,1,1, 1,0,0,1, 1,1,1,0, 0,1,1,1};

static void tick(void)
{
	// OFF 出力
	tmr_waiting = 1;
	CMT0.CMCNT = 0;
	PORT2.PODR.BIT.B2 = 0;

	// タイマ起動
	CMT.CMSTR0.BIT.STR0 = 0x01;
	while(tmr_waiting != 0);	

	// ON 出力
	tmr_waiting = 1;
	CMT0.CMCNT = 0;
	PORT2.PODR.BIT.B2 = 1;

	// タイマ起動
	CMT.CMSTR0.BIT.STR0 = 0x01;
	while(tmr_waiting != 0);

	return;
}

static void tick_tmr_event_handler()
{
	tmr_waiting = 0;
}

void init_sw_port(void)
{
	PORT2.PDR.BIT.B1 = 1;
	PORT2.PDR.BIT.B2 = 1;
	return;
}

void init_swd(void)
{
	// SWD用ポート初期化
	init_sw_port();

	// CMT初期化
	init_cmt0(tick_tmr_event_handler);
}
void select_swd(void)
{
	volatile int i;

	// High 出力 50回以上
	PORT2.PODR.BIT.B1 = 1;
	for(i = 0; i < 52; i++)
	{
		tick();
	}

	PORT2.PODR.BIT.B2 = 1;
	for(i = 0; i < 500; i++);
	
	for(i = 0; i < 16; i++)
	{
		PORT2.PODR.BIT.B1 = sw_select_packet[i];
		tick();
	}

	PORT2.PODR.BIT.B2 = 1;
	for(i = 0; i < 500; i++);

	// High 出力 50回以上
	PORT2.PODR.BIT.B1 = 1;
	for(i = 0; i < 52; i++)
	{
		tick();
	}
	PORT2.PODR.BIT.B2 = 1;
	for(i = 0; i < 500; i++);

	
	#if 1
	// CMSIS-DAP波形確認
	// 8クロックとコマンド送信の間でDWOはLOWを出している必要がある
	PORT2.PODR.BIT.B1 = 0;
	for(i = 0; i < 8; i++)
	{
		tick();
	}
	#endif

	PORT2.PODR.BIT.B2 = 1;
	for(i = 0; i < 500; i++);
}

static unsigned char ack[3] = 0;
static unsigned int rVal = 0;
static unsigned char parity = 0;
void readID(void)
{
	volatile unsigned char reg;
	int i;
	// Start:High
	PORT2.PODR.BIT.B1 = 1;
	tick();
	
	// APnDP:DP(0:DP 1:AP) Low
	PORT2.PODR.BIT.B1 = 0;
	tick();

	// 0:Write 1:Read
	PORT2.PODR.BIT.B1 = 1;
	tick();
	

	// A(3:2) 00
	PORT2.PODR.BIT.B1 = 0;
	tick();
	PORT2.PODR.BIT.B1 = 0;
	tick();

	// parity ODD
	PORT2.PODR.BIT.B1 = 1;
	tick();
	
	// stio 0
	PORT2.PODR.BIT.B1 = 0;
	tick();

	// Park
	// Not driven by the host must be read as 1 by the target because of the pull-up.
	PORT2.PODR.BIT.B1 = 1;
	PORT2.PCR.BIT.B1 = 1;	// enable pull up.
	PORT2.PDR.BIT.B1 = 0;	
	tick();

  	// =======================================================
	// Read ACK
	// =======================================================
	tick();
	ack[0] = PORT2.PIDR.BIT.B1;
	tick();
	ack[1] = PORT2.PIDR.BIT.B1;
	tick();
	ack[2] = PORT2.PIDR.BIT.B1;
	
	PORT2.PODR.BIT.B2 = 1;
	for(i = 0; i < 500; i++);
	rVal = 0;
	for(i = 0; i < 32; i++)
  	{
		tick();
		reg = PORT2.PIDR.BIT.B1;
    		rVal |= (reg << i);
	}
	// Read parity
	tick();
	parity = PORT2.PIDR.BIT.B1;
}
