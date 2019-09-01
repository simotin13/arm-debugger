#include <stdint.h>
#include "iodefine.h"
#include "cmt0.h"

void main(void)
{
	volatile uint32_t idcode;
	volatile uint32_t reg;
	volatile int32_t ret = 0;

	// �N���b�N������H������
	init_clock();
	
	// SWD�@�\������
	init_swd();
	
	// SWD�I��
	select_swd();

	// Read IDCODE
	reg = 0;
	ret = read_dp_reg(0, 0, &idcode);

	// write CTRL
	reg = 0x50000000;
	ret = write_dp_reg(0, 1, reg);

	// read CTRL
	reg = 0;
	ret = read_dp_reg(0, 1, &reg);

	while(1);
}
