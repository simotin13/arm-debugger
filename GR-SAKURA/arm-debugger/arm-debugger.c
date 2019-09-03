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
	ret = read_reg(REG_TYPE_DP, REG_R_IDCODE, &idcode);

	#if 1
	// write CTRL
	reg = 0x50000000;
	ret = write_reg(REG_TYPE_DP, REG_RW_CTRL_STAT, reg);
	#endif

	reg = 0;
	ret = read_reg(REG_TYPE_DP, REG_RW_CTRL_STAT, &reg);

	// read CTRL
	#if 0	// ▼ clear errors
	reg = 0;
	ret = read_reg(REG_TYPE_DP, REG_RW_CTRL_STAT, &reg);

	reg = 0x1E;
	ret = write_reg(REG_TYPE_DP, REG_W_ABORT, reg);

	// read CTRL
	reg = 0;
	ret = read_reg(REG_TYPE_DP, REG_RW_CTRL_STAT, &reg);
	#endif	// ▲ clear errors

	// Select AP-Bank
	reg = 0x000000F0;
	ret = write_reg(REG_TYPE_DP, REG_RW_SELECT, reg);

	reg = 0;
	ret = read_reg(REG_TYPE_DP, REG_RW_CTRL_STAT, &reg);
	
	reg = 0xFFFFFFFF;
	ret = read_reg(REG_TYPE_AP, REG_AP_BANK_F_IDR, &reg);

	while(1);
}
