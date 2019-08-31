#include "iodefine.h"

void init_clock()
{
	volatile int i;

	SYSTEM.PRCR.WORD = 0xA501;	// プロテクト解除(クロック発生回路)

	// メインクロック発振開始
	SYSTEM.MOSCCR.BYTE = 0x00;

	// メインクロック発振安定化待ち
	for (i = 0; i < 100; i++);

	// PLLCR 192MHz (12MHz x 16)
	// STC: 01111 
	SYSTEM.PLLCR.WORD = 0x0F00;

	// PLLクロック動作開始
	SYSTEM.PLLCR2.BYTE = 0x00;

	// PLLクロック安定化待ち
	for ( i = 0; i < 100; i++);

	// =========================================================
	// クロックソースについて
	// CPU:システムクロック
	// 周辺機能:PCLKB (内部周辺バス2)
	// =========================================================

	// システムクロックコントロールレジスタ
	// -    b00-b03 予約(0001固定)
	// -    b04-b07 予約(0001固定)
	// PCKB b11-b08 周辺モジュールクロックB (4分周)
	// PCKA b15-b12 周辺モジュールクロックB (4分周)
	// BCK  b19-b16 外部バスクロック (64分周) 未使用
	// -    b21-b20 予約(0固定)
	// ICK  b27-b24 システムクロック (2分周)
	// FCK  b31-b28 FlashIF クロック (4分周)
	// 注意
	// PLLクロックの場合はPCKA,PCKBは2分周は設定不可
	SYSTEM.SCKCR.LONG = 0x21062211;

	// システムクロックコントロールレジスタ2 SCKCR2
	// IEBCK b03 - b00 IEBUSクロック 4分周
	SYSTEM.SCKCR2.WORD = 0x22;

	// システムクロックコントロールレジスタ3 SCKCR3
	// -     b07-b00 予約(0固定)
	// CKSEL b10-b08 PLL回路選択 100
	// -     b11-b31 予約(0固定)
	SYSTEM.SCKCR3.WORD = 0x000000400;

	SYSTEM.PRCR.WORD = 0xA500;  // プロテクト設定
	return;
}
