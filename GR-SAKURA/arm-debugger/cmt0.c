#include <stdio.h>
#include "iodefine.h"
#include "cmt0.h"

static CMT0_CALLBACK cmt0_callback = 0;
void init_cmt0(CMT0_CALLBACK cmt0_cb)
{
	cmt0_callback = cmt0_cb;
	SYSTEM.PRCR.WORD = 0xA502;  // プロテクト解除
    	MSTP(CMT0) = 0;
	SYSTEM.PRCR.WORD = 0xA500;  // プロテクト設定
    
	// クロックソース PCLK/8
	CMT0.CMCR.BIT.CKS = 0x00;
	CMT0.CMCR.BIT.CMIE = 0x01;
	IPR(CMT0,CMI0) = 6; 
	IEN(CMT0,CMI0) = 1;
	CMT0.CMCOR = 599;
}

#pragma interrupt (ctm0_int_handler(vect=28))
void ctm0_int_handler(void)
{
	CMT.CMSTR0.BIT.STR0 = 0x00;	// タイマストップ
	if (cmt0_callback != NULL)
	{
		cmt0_callback();
	}
	return;
}
