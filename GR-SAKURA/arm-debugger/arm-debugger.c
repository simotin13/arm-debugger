#include <stdint.h>
#include "iodefine.h"
#include "cmt0.h"

static uint32_t test = 5;
static uint32_t mod = 0;
void main(void)
{
	uint32_t idcode;
	mod = test % 2;

	// クロック発生回路初期化
	init_clock();
	
	// SWD機能初期化
	init_swd();
	
	// SWD選択
	select_swd();

	// reaad idcode
	read_dp_reg(0x00, &idcode);
	while(1);
}
