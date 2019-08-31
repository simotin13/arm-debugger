#include "iodefine.h"

void init_clock()
{
	volatile int i;

	SYSTEM.PRCR.WORD = 0xA501;	// �v���e�N�g����(�N���b�N������H)

	// ���C���N���b�N���U�J�n
	SYSTEM.MOSCCR.BYTE = 0x00;

	// ���C���N���b�N���U���艻�҂�
	for (i = 0; i < 100; i++);

	// PLLCR 192MHz (12MHz x 16)
	// STC: 01111 
	SYSTEM.PLLCR.WORD = 0x0F00;

	// PLL�N���b�N����J�n
	SYSTEM.PLLCR2.BYTE = 0x00;

	// PLL�N���b�N���艻�҂�
	for ( i = 0; i < 100; i++);

	// =========================================================
	// �N���b�N�\�[�X�ɂ���
	// CPU:�V�X�e���N���b�N
	// ���Ӌ@�\:PCLKB (�������Ӄo�X2)
	// =========================================================

	// �V�X�e���N���b�N�R���g���[�����W�X�^
	// -    b00-b03 �\��(0001�Œ�)
	// -    b04-b07 �\��(0001�Œ�)
	// PCKB b11-b08 ���Ӄ��W���[���N���b�NB (4����)
	// PCKA b15-b12 ���Ӄ��W���[���N���b�NB (4����)
	// BCK  b19-b16 �O���o�X�N���b�N (64����) ���g�p
	// -    b21-b20 �\��(0�Œ�)
	// ICK  b27-b24 �V�X�e���N���b�N (2����)
	// FCK  b31-b28 FlashIF �N���b�N (4����)
	// ����
	// PLL�N���b�N�̏ꍇ��PCKA,PCKB��2�����͐ݒ�s��
	SYSTEM.SCKCR.LONG = 0x21062211;

	// �V�X�e���N���b�N�R���g���[�����W�X�^2 SCKCR2
	// IEBCK b03 - b00 IEBUS�N���b�N 4����
	SYSTEM.SCKCR2.WORD = 0x22;

	// �V�X�e���N���b�N�R���g���[�����W�X�^3 SCKCR3
	// -     b07-b00 �\��(0�Œ�)
	// CKSEL b10-b08 PLL��H�I�� 100
	// -     b11-b31 �\��(0�Œ�)
	SYSTEM.SCKCR3.WORD = 0x000000400;

	SYSTEM.PRCR.WORD = 0xA500;  // �v���e�N�g�ݒ�
	return;
}
