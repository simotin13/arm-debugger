#include <stdint.h>
#include "iodefine.h"
#include "cmt0.h"

void main(void)
{
	uint32_t idcode;

	// �N���b�N������H������
	init_clock();
	
	// SWD�@�\������
	init_swd();
	
	// SWD�I��
	select_swd();
	
	// IDCODE �ǎ�
	idcode = readIDCode();

	while(1);
}
