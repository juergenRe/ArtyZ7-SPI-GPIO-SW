#include "xgpio.h"

XGpio_Config XGpio_ConfigTable[] __attribute__ ((section (".drvcfg_sec"))) = {

	{
		"xlnx,axi-gpio-2.0", /* compatible */
		0x41200000, /* reg */
		0x1, /* xlnx,interrupt-present */
		0x1, /* xlnx,is-dual */
		0x401d, /* interrupts */
		0xf8f01000, /* interrupt-parent */
		0x4 /* xlnx,gpio-width */
	},
	{
		"xlnx,axi-gpio-2.0", /* compatible */
		0x41210000, /* reg */
		0x0, /* xlnx,interrupt-present */
		0x0, /* xlnx,is-dual */
		0xffff, /* interrupts */
		0xffff, /* interrupt-parent */
		0xe /* xlnx,gpio-width */
	},
	 {
		 NULL
	}
};