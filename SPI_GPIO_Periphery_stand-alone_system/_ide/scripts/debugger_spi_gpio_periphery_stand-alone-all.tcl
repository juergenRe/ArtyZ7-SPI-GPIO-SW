# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: C:\Users\Juergen\Documents\Projects\FPGA\Arty-Z7\ArtyZ7-SPI-GPIO-SW\SPI_GPIO_Periphery_stand-alone_system\_ide\scripts\debugger_spi_gpio_periphery_stand-alone-all.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source C:\Users\Juergen\Documents\Projects\FPGA\Arty-Z7\ArtyZ7-SPI-GPIO-SW\SPI_GPIO_Periphery_stand-alone_system\_ide\scripts\debugger_spi_gpio_periphery_stand-alone-all.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent Arty Z7 003017A5C50AA" && level==0 && jtag_device_ctx=="jsn-Arty Z7-003017A5C50AA-23727093-0"}
fpga -file C:/Users/Juergen/Documents/Projects/FPGA/Arty-Z7/ArtyZ7-SPI-GPIO-SW/SPI_GPIO_Periphery_stand-alone/_ide/bitstream/SPI-GPIO-Periphery.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw C:/Users/Juergen/Documents/Projects/FPGA/Arty-Z7/ArtyZ7-SPI-GPIO-SW/SPI_GPIO_PeripheryPfm/export/SPI_GPIO_PeripheryPfm/hw/SPI-GPIO-Periphery.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source C:/Users/Juergen/Documents/Projects/FPGA/Arty-Z7/ArtyZ7-SPI-GPIO-SW/SPI_GPIO_Periphery_stand-alone/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
dow C:/Users/Juergen/Documents/Projects/FPGA/Arty-Z7/ArtyZ7-SPI-GPIO-SW/SPI_GPIO_Periphery_stand-alone/Debug/SPI_GPIO_Periphery_stand-alone.elf
configparams force-mem-access 0
bpadd -addr &main