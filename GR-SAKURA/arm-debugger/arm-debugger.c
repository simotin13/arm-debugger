#include <stdint.h>
#include "iodefine.h"
#include "cmt0.h"

void main(void)
{
	// クロック発生回路初期化
	init_clock();
	
	// SWD機能初期化
	init_swd();

	while(1)
	{
	}
}
