#include <stdint.h>
#include "iodefine.h"
#include "cmt0.h"

void main(void)
{
	volatile uint32_t idcode;
	volatile uint32_t reg;
	volatile int32_t ret = 0;

	// クロック発生回路初期化
	init_clock();
	
	// SWD機能初期化
	init_swd();
	
	// SWD選択
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
