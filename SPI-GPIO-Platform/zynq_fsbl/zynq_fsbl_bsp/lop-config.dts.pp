# 0 "C:\\Users\\Juergen\\Documents\\Projects\\FPGA\\Arty-Z7-A\\ArtyZ7-SPI-GPIO-SW\\SPI-GPIO-Platform\\zynq_fsbl\\zynq_fsbl_bsp\\lop-config.dts"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "C:\\Users\\Juergen\\Documents\\Projects\\FPGA\\Arty-Z7-A\\ArtyZ7-SPI-GPIO-SW\\SPI-GPIO-Platform\\zynq_fsbl\\zynq_fsbl_bsp\\lop-config.dts"

/dts-v1/;
/ {
        compatible = "system-device-tree-v1,lop";
        lops {
                lop_0 {
                        compatible = "system-device-tree-v1,lop,load";
                        load = "assists/baremetal_validate_comp_xlnx.py";
                };

                lop_1 {
                    compatible = "system-device-tree-v1,lop,assist-v1";
                    node = "/";
                    outdir = "C:/Users/Juergen/Documents/Projects/FPGA/Arty-Z7-A/ArtyZ7-SPI-GPIO-SW/SPI-GPIO-Platform/zynq_fsbl/zynq_fsbl_bsp";
                    id = "module,baremetal_validate_comp_xlnx";
                    options = "ps7_cortexa9_0 C:/Xilinx/Vitis/2024.2/data/embeddedsw/lib/sw_services/xilffs_v5_3/src C:/Users/Juergen/Documents/Projects/FPGA/Arty-Z7-A/ArtyZ7-SPI-GPIO-SW/_ide/.wsdata/.repo.yaml";
                };

                lop_2 {
                    compatible = "system-device-tree-v1,lop,assist-v1";
                    node = "/";
                    outdir = "C:/Users/Juergen/Documents/Projects/FPGA/Arty-Z7-A/ArtyZ7-SPI-GPIO-SW/SPI-GPIO-Platform/zynq_fsbl/zynq_fsbl_bsp";
                    id = "module,baremetal_validate_comp_xlnx";
                    options = "ps7_cortexa9_0 C:/Xilinx/Vitis/2024.2/data/embeddedsw/lib/sw_services/xilrsa_v1_8/src C:/Users/Juergen/Documents/Projects/FPGA/Arty-Z7-A/ArtyZ7-SPI-GPIO-SW/_ide/.wsdata/.repo.yaml";
                };

        };
    };
