#include <stdint.h>
#include "iodefine.h"
#include "cmt0.h"

void main(void)
{
	uint32_t idcode;

	// クロック発生回路初期化
	init_clock();
	
	// SWD機能初期化
	init_swd();
	
	// SWD選択
	select_swd();
	
	// IDCODE 読取
	idcode = readIDCode();

	while(1);
}
