#include <stdint.h>
#include <iodefine.h>
#include "cmt0.h"

static tmr_waiting = 0;

static const unsigned char sw_select_packet[16] = {0,1,1,1, 1,0,0,1, 1,1,1,0, 0,1,1,1};

static void tick(void)
{
	// OFF �o��
	tmr_waiting = 1;
	cmt0_clear_count();
	PORT2.PODR.BIT.B2 = 0;

	// �^�C�}�N��
	cmt0_start();
	while(tmr_waiting != 0);	

	// ON �o��
	tmr_waiting = 1;
	cmt0_clear_count();
	PORT2.PODR.BIT.B2 = 1;

	// �^�C�}�N��
	cmt0_start();
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
	// SWD�p�|�[�g������
	init_sw_port();

	// CMT������
	cmt0_init(tick_tmr_event_handler);
}

void select_swd(void)
{
	volatile int i;

	// High �o�� 50��ȏ�
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

	// High �o�� 50��ȏ�
	PORT2.PODR.BIT.B1 = 1;
	for(i = 0; i < 52; i++)
	{
		tick();
	}
	PORT2.PODR.BIT.B2 = 1;
	for(i = 0; i < 500; i++);

	
	#if 1
	// 2�N���b�N�ƃR�}���h���M�̊Ԃ�DWO��LOW���o���Ă���K�v������
	// 8 clock�Ƃ̋L�ڂ�����̂�
	PORT2.PODR.BIT.B1 = 0;
	for(i = 0; i < 8; i++)
	{
		tick();
	}
	#endif

	PORT2.PODR.BIT.B2 = 1;
	for(i = 0; i < 500; i++);
}

int32_t read_dp_reg(uint8_t addr, uint32_t *pVal)
{
	volatile uint8_t reg;
	volatile uint32_t val;
	volatile uint32_t onBits = 0;
	volatile uint8_t ack[3] = {0};
	volatile uint8_t parity = 0;
	volatile uint8_t debug;
	volatile int i;

	// Start:High
	PORT2.PODR.BIT.B1 = 1;
	tick();
	
	// APnDP:DP(0:DP 1:AP) Low
	PORT2.PODR.BIT.B1 = 0;
	tick();

	// 0:Write 1:Read
	PORT2.PODR.BIT.B1 = 1;
	onBits = 1;
	tick();
	
	// A(3:2) LSB
	reg = (addr >> 2) & 0x01;
	onBits += reg; 
	PORT2.PODR.BIT.B1 = reg;
	tick();

	reg = (addr >> 3) & 0x01;
	onBits += reg; 
	PORT2.PODR.BIT.B1 = reg;
	tick();

	// parity ODD
	parity = 0;
	if (onBits % 2)
	{
		parity = 1;
	}
	
	debug = parity;
	PORT2.PODR.BIT.B1 = parity;
	tick();

	// stop 0
	PORT2.PODR.BIT.B1 = 0;
	tick();

	// Park
	// Not driven by the host must be read as 1 by the target because of the pull-up.
	PORT2.PODR.BIT.B1 = 1;
	PORT2.PCR.BIT.B1 = 1;	// enable pull up.
	PORT2.PDR.BIT.B1 = 0;	
	tick();

	// Check ACK
	tick();
	ack[0] = PORT2.PIDR.BIT.B1;
	tick();
	ack[1] = PORT2.PIDR.BIT.B1;
	tick();
	ack[2] = PORT2.PIDR.BIT.B1;
	if ((ack[0] != 1) || (ack[1] != 0)  && (ack[2] != 0))
	{
		// Not OK
		return -1;
	}
	
	PORT2.PODR.BIT.B2 = 1;
	for(i = 0; i < 500; i++);

	val = 0;
	onBits = 0;
	for(i = 0; i < 32; i++)
  	{
		// LSB first
		tick();
		reg = PORT2.PIDR.BIT.B1;
    		val |= (reg << i);
		if (reg & 0x01)
		{
			onBits += 1;
		}
	}

	// Read parity
	tick();
	reg = PORT2.PIDR.BIT.B1;

	// calc parity
	parity = 0;
	if (onBits % 2)
	{
		parity = 1;
	}

	if (reg != parity)
	{
		// data parity not match
		return -2;
	}

	*pVal = val;

	PORT2.PCR.BIT.B1 = 0;	// disable pull up.
	PORT2.PODR.BIT.B1 = 0;	
	PORT2.PDR.BIT.B1 = 1;	
	for(i = 0; i < 500; i++);

	// 4�N���b�N�ƃR�}���h���M�̊Ԃ�DWO��LOW���o���Ă���K�v������
	PORT2.PODR.BIT.B1 = 0;
	for(i = 0; i < 4; i++)
	{
		tick();
	}

	return 0;
}

int32_t write_dp_reg(uint8_t addr, uint32_t val)
{
	volatile uint8_t reg;
	volatile uint32_t onBits = 0;
	volatile uint8_t ack[3] = 0;
	volatile uint8_t parity = 0;
	volatile uint8_t debug = 0;
	int i;

	// Start:High
	PORT2.PODR.BIT.B1 = 1;
	tick();
	
	// APnDP:DP(0:DP 1:AP) Low
	PORT2.PODR.BIT.B1 = 0;
	tick();

	// 0:Write 1:Read
	PORT2.PODR.BIT.B1 = 0;
	tick();
	
	// A(3:2) LSB
	reg = (addr >> 2) & 0x01;
	onBits += reg;
	PORT2.PODR.BIT.B1 = reg;
	tick();

	reg = (addr >> 3) & 0x01;
	onBits += reg;
	PORT2.PODR.BIT.B1 = reg;
	tick();

	// parity even
	parity = 0;
	if (onBits % 2)
	{
		parity = 1;
	}
	PORT2.PODR.BIT.B1 = parity;
	tick();

	// stop 0
	PORT2.PODR.BIT.B1 = 0;
	tick();

	// Park
	// Not driven by the host must be read as 1 by the target because of the pull-up.
	PORT2.PODR.BIT.B1 = 1;
	PORT2.PCR.BIT.B1 = 1;	// enable pull up.
	PORT2.PDR.BIT.B1 = 0;	
	tick();

	// Check ACK
	tick();
	ack[0] = PORT2.PIDR.BIT.B1;
	tick();
	ack[1] = PORT2.PIDR.BIT.B1;
	tick();
	ack[2] = PORT2.PIDR.BIT.B1;
	if ((ack[0] != 1) || (ack[1] != 0)  && (ack[2] != 0))
	{
		// Not OK
		return -1;
	}
	
	PORT2.PODR.BIT.B2 = 1;
	for(i = 0; i < 500; i++);


	PORT2.PDR.BIT.B1 = 1;
	onBits = 0;
	for(i = 0; i < 32; i++)
  	{
		// LSB first
		reg = (val >> i) & 0x01;
		onBits += reg;
		PORT2.PIDR.BIT.B1 = reg;
		tick();
	}

	// calc parity
	parity = 0;
	if (onBits % 2)
	{
		parity = 1;
	}
	
	// send parity
	debug = parity;
	PORT2.PIDR.BIT.B1 = parity;
	tick();

	// 2�N���b�N�ƃR�}���h���M�̊Ԃ�DWO��LOW���o���Ă���K�v������
	PORT2.PODR.BIT.B1 = 0;
	for(i = 0; i < 2; i++)
	{
		tick();
	}

	return 0;
}