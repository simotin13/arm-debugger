#include <stdint.h>
#include "iodefine.h"
#include "cmt0.h"

static uint32_t test = 5;
static uint32_t mod = 0;
void main(void)
{
	uint32_t idcode;
	mod = test % 2;

	// �N���b�N������H������
	init_clock();
	
	// SWD�@�\������
	init_swd();
	
	// SWD�I��
	select_swd();

	// reaad idcode
	read_dp_reg(0x00, &idcode);
	while(1);
}
