/*
 * SPIControl.c
 *
 *  Routines for accessing the SPI port in test environment
 */
#include "xparameters.h"

#include "SPIControl.h"

#define SpiPs_RecvByte(BaseAddress) \
		(u8)XSpiPs_In32((BaseAddress) + XSPIPS_RXD_OFFSET)

#define SpiPs_SendByte(BaseAddress, Data) \
		XSpiPs_Out32((BaseAddress) + XSPIPS_TXD_OFFSET, (Data))

XSpiPs SpiCtrl; 				

XSpiPs *getSPIInstance(void) {
    return &SpiCtrl;
}

// configure SPI
// Manual SS
// Auto Start
// prescaler = 128
// CLK_POL = 0 (Clk is zero when no transmission runs)
// CLK_PH = 1 (no inactivation of SS during back to back transfers). Driving at rising edge, sampling at falling edge
int SpiInitMaster(UINTPTR BaseAddress){
	int Status;
    XSpiPs_Config *SpiConfig = XSpiPs_LookupConfig(BaseAddress);

	if (NULL == SpiConfig) {
		return XST_FAILURE;
	}

	Status = XSpiPs_CfgInitialize(&SpiCtrl, SpiConfig, SpiConfig->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to check hardware build.
	 */
	Status = XSpiPs_SelfTest(&SpiCtrl);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	xil_printf("%s self test succeeded\r\n", __func__);

	XSpiPs_Disable(&SpiCtrl);

    // deselect all slaves using 0x0F. Selecting is done by passing a value of 0..2 (typ.: 0)
	Status = XSpiPs_SetSlaveSelect(&SpiCtrl, SPI_DESELECT_ALL);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = XSpiPs_SetOptions(&SpiCtrl, XSPIPS_MASTER_OPTION | XSPIPS_CLK_PHASE_1_OPTION | XSPIPS_MANUAL_START_OPTION | XSPIPS_FORCE_SSELECT_OPTION);
	if (Status != XST_SUCCESS) {
		xil_printf("%s XSpiPs_SetOptions fail\n", __func__);
		return XST_FAILURE;
	}

	Status = XSpiPs_SetClkPrescaler(&SpiCtrl, XSPIPS_CLK_PRESCALE_256);
	if (Status != XST_SUCCESS) {
		xil_printf("%s XSpiPs_SetClkPrescaler fail\n", __func__);
		return XST_FAILURE;
	}
	//XSpiPs_Enable(&SpiCtrl);
	xil_printf("spi 0 config finished\n");

	return XST_SUCCESS;
}

// cs = 0xF deselects all slaves
void SpiSetSlaveSelect(u8 cs){
    XSpiPs_SetSlaveSelect(&SpiCtrl, cs);
}

u32 SpiReadWrite(u8 *pTx, u8 *pRx, u32 ByteCount){
	return XSpiPs_Transfer(&SpiCtrl, pTx, pRx, ByteCount);
}




