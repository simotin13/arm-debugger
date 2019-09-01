#include <stdio.h>
#include "iodefine.h"
#include "cmt0.h"

static CMT0_CALLBACK cmt0_callback = 0;

#pragma interrupt (ctm0_int_handler(vect=28))
static void ctm0_int_handler(void)
{
	CMT.CMSTR0.BIT.STR0 = 0x00;	// タイマストップ
	if (cmt0_callback != NULL)
	{
		cmt0_callback();
	}
	return;
}

void cmt0_init(CMT0_CALLBACK cmt0_cb)
{
	cmt0_callback = cmt0_cb;
	SYSTEM.PRCR.WORD = 0xA502;	// プロテクト解除
    	MSTP(CMT0) = 0;
	SYSTEM.PRCR.WORD = 0xA500;	// プロテクト設定
	
	CMT0.CMCR.BIT.CKS = 0x00;	// クロックソース PCLK/8
	CMT0.CMCR.BIT.CMIE = 0x01;	// 割り込み許可
	IPR(CMT0,CMI0) = 6; 
	IEN(CMT0,CMI0) = 1;
	CMT0.CMCOR = 599;		// コンペアマッチ 100ns
}

void cmt0_start(void)
{
	CMT.CMSTR0.BIT.STR0 = 0x01;	// タイマスタート
}

void cmt0_clear_count(void)
{
	CMT0.CMCNT = 0;
}