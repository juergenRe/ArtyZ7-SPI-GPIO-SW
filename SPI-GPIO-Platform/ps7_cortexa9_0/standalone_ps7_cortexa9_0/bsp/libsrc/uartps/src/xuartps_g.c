#include "xuartps.h"

XUartPs_Config XUartPs_ConfigTable[] __attribute__ ((section (".drvcfg_sec"))) = {

	{
		"xlnx,xuartps", /* compatible */
		0xe0000000, /* reg */
		0x5f5e100, /* xlnx,clock-freq */
		0x1, /* cts-override */
		0x17, /* clocks */
		0x401b, /* interrupts */
		0xf8f01000 /* interrupt-parent */
	},
	 {
		 NULL
	}
};