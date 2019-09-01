#include <stdint.h>
#include "iodefine.h"
#include "cmt0.h"
#include "sw_dp.h"

void main(void)
{
	uint32_t idcode;
	uint32_t reg;
	volatile int32_t ret = 0;

	// クロック発生回路初期化
	init_clock();
	
	// SWD機能初期化
	init_swd();
	
	// SWD選択
	select_swd();

	// Read IDCODE
	reg = 0;
	ret = read_dp_reg(REG_R_IDCODE, &idcode);

	// write CTRL
	reg = 0x50000000;
	ret = write_dp_reg(REG_RW_CTRL_STAT, reg);

	// read CTRL
	reg = 0;
	ret = read_dp_reg(REG_RW_CTRL_STAT, &reg);
	
	reg = 0x1E;
	ret = write_dp_reg(REG_W_ABORT, reg);

	// read CTRL
	reg = 0;
	ret = read_dp_reg(REG_RW_CTRL_STAT, &reg);

	while(1);
}
