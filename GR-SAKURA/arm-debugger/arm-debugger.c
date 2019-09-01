#include <stdint.h>
#include "iodefine.h"
#include "cmt0.h"
#include "sw_dp.h"

void main(void)
{
	uint32_t idcode;
	uint32_t reg;
	volatile int32_t ret = 0;

	// �N���b�N������H������
	init_clock();
	
	// SWD�@�\������
	init_swd();
	
	// SWD�I��
	select_swd();

	// Read IDCODE
	reg = 0;
	ret = read_dp_reg(0x00, &idcode);

	// write CTRL
	reg = 0x50000000;
	ret = write_dp_reg(0x04, reg);

	// read CTRL
	reg = 0;
	ret = read_dp_reg(0x04, &reg);

	while(1);
}
