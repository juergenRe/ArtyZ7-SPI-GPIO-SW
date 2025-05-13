# 0 "C:\\Users\\Juergen\\Documents\\Projects\\FPGA\\Arty-Z7-A\\ArtyZ7-SPI-GPIO-SW\\SPI-GPIO-Platform\\hw\\sdt\\system-top.dts"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "C:\\Users\\Juergen\\Documents\\Projects\\FPGA\\Arty-Z7-A\\ArtyZ7-SPI-GPIO-SW\\SPI-GPIO-Platform\\hw\\sdt\\system-top.dts"
/dts-v1/;
# 1 "C:\\Users\\Juergen\\Documents\\Projects\\FPGA\\Arty-Z7-A\\ArtyZ7-SPI-GPIO-SW\\SPI-GPIO-Platform\\hw\\sdt\\zynq-7000.dtsi" 1
# 10 "C:\\Users\\Juergen\\Documents\\Projects\\FPGA\\Arty-Z7-A\\ArtyZ7-SPI-GPIO-SW\\SPI-GPIO-Platform\\hw\\sdt\\zynq-7000.dtsi"
/ {
 #address-cells = <1>;
 #size-cells = <1>;
 compatible = "xlnx,zynq-7000";
 model = "Xilinx Zynq";

 options {
  u-boot {
   compatible = "u-boot,config";
   bootscr-address = /bits/ 64 <0x3000000>;
  };
 };

 cpus_a9: cpus-a9@0 {
  #address-cells = <1>;
  #size-cells = <0>;

  ps7_cortexa9_0: cpu@0 {
   compatible = "arm,cortex-a9";
   device_type = "cpu";
   reg = <0>;
   clocks = <&clkc 3>;
   clock-latency = <1000>;
   cpu0-supply = <&regulator_vccpint>;
   operating-points = <
    666667 1000000
    333334 1000000
   >;
  };

  ps7_cortexa9_1: cpu@1 {
   compatible = "arm,cortex-a9";
   device_type = "cpu";
   reg = <1>;
   clocks = <&clkc 3>;
  };
 };

 fpga_full: fpga-region {
  compatible = "fpga-region";
  fpga-mgr = <&devcfg>;
  #address-cells = <1>;
  #size-cells = <1>;
  ranges;
 };

 pmu@f8891000 {
  compatible = "arm,cortex-a9-pmu";
  interrupts = <0 5 4>, <0 6 4>;
  interrupt-parent = <&intc>;
  reg = <0xf8891000 0x1000>,
    <0xf8893000 0x1000>;
 };

 regulator_vccpint: fixedregulator {
  compatible = "regulator-fixed";
  regulator-name = "VCCPINT";
  regulator-min-microvolt = <1000000>;
  regulator-max-microvolt = <1000000>;
  regulator-boot-on;
  regulator-always-on;
 };

 replicator {
  compatible = "arm,coresight-static-replicator";
  clocks = <&clkc 27>, <&clkc 46>, <&clkc 47>;
  clock-names = "apb_pclk", "dbg_trc", "dbg_apb";

  out-ports {
   #address-cells = <1>;
   #size-cells = <0>;


   port@0 {
    reg = <0>;
    replicator_out_port0: endpoint {
     remote-endpoint = <&tpiu_in_port>;
    };
   };
   port@1 {
    reg = <1>;
    replicator_out_port1: endpoint {
     remote-endpoint = <&etb_in_port>;
    };
   };
  };
  in-ports {

   port {
    replicator_in_port0: endpoint {
     remote-endpoint = <&funnel_out_port>;
    };
   };
  };
 };

 amba: axi {
  bootph-all;
  compatible = "simple-bus";
  #address-cells = <1>;
  #size-cells = <1>;
  interrupt-parent = <&intc>;
  ranges;

  adc: adc@f8007100 {
   compatible = "xlnx,zynq-xadc-1.00.a";
   reg = <0xf8007100 0x20>;
   interrupts = <0 7 4>;
   interrupt-parent = <&intc>;
   clocks = <&clkc 12>;
  };

  can0: can@e0008000 {
   compatible = "xlnx,zynq-can-1.0";
   status = "disabled";
   clocks = <&clkc 19>, <&clkc 36>;
   clock-names = "can_clk", "pclk";
   reg = <0xe0008000 0x1000>;
   interrupts = <0 28 4>;
   interrupt-parent = <&intc>;
   tx-fifo-depth = <0x40>;
   rx-fifo-depth = <0x40>;
  };

  can1: can@e0009000 {
   compatible = "xlnx,zynq-can-1.0";
   status = "disabled";
   clocks = <&clkc 20>, <&clkc 37>;
   clock-names = "can_clk", "pclk";
   reg = <0xe0009000 0x1000>;
   interrupts = <0 51 4>;
   interrupt-parent = <&intc>;
   tx-fifo-depth = <0x40>;
   rx-fifo-depth = <0x40>;
  };

  gpio0: gpio@e000a000 {
   compatible = "xlnx,zynq-gpio-1.0";
   #gpio-cells = <2>;
   clocks = <&clkc 42>;
   gpio-controller;
   interrupt-controller;
   #interrupt-cells = <2>;
   interrupt-parent = <&intc>;
   interrupts = <0 20 4>;
   reg = <0xe000a000 0x1000>;
  };

  i2c0: i2c@e0004000 {
   compatible = "cdns,i2c-r1p10";
   status = "disabled";
   clocks = <&clkc 38>;
   interrupt-parent = <&intc>;
   interrupts = <0 25 4>;
   clock-frequency = <400000>;
   reg = <0xe0004000 0x1000>;
   #address-cells = <1>;
   #size-cells = <0>;
  };

  i2c1: i2c@e0005000 {
   compatible = "cdns,i2c-r1p10";
   status = "disabled";
   clocks = <&clkc 39>;
   interrupt-parent = <&intc>;
   interrupts = <0 48 4>;
   clock-frequency = <400000>;
   reg = <0xe0005000 0x1000>;
   #address-cells = <1>;
   #size-cells = <0>;
  };

  intc: interrupt-controller@f8f01000 {
   compatible = "arm,cortex-a9-gic";
   #interrupt-cells = <3>;
   interrupt-controller;
   reg = <0xF8F01000 0x1000>,
         <0xF8F00100 0x100>;
  };

  L2: cache-controller@f8f02000 {
   compatible = "arm,pl310-cache";
   reg = <0xF8F02000 0x1000>;
   interrupts = <0 2 4>;
   arm,data-latency = <3 2 2>;
   arm,tag-latency = <2 2 2>;
   cache-unified;
   cache-level = <2>;
  };

  mc: memory-controller@f8006000 {
   compatible = "xlnx,zynq-ddrc-a05";
   reg = <0xf8006000 0x1000>;
  };

  ocm: sram@fffc0000 {
   compatible = "mmio-sram";
   reg = <0xfffc0000 0x10000>;
   #address-cells = <1>;
   #size-cells = <1>;
   ranges = <0 0xfffc0000 0x10000>;
   ocm-sram@0 {
    reg = <0x0 0x10000>;
   };
  };

  uart0: serial@e0000000 {
   compatible = "xlnx,xuartps", "cdns,uart-r1p8";
   status = "disabled";
   clocks = <&clkc 23>, <&clkc 40>;
   clock-names = "uart_clk", "pclk";
   reg = <0xE0000000 0x1000>;
   interrupts = <0 27 4>;
   interrupt-parent = <&intc>;
  };

  uart1: serial@e0001000 {
   compatible = "xlnx,xuartps", "cdns,uart-r1p8";
   status = "disabled";
   clocks = <&clkc 24>, <&clkc 41>;
   clock-names = "uart_clk", "pclk";
   reg = <0xE0001000 0x1000>;
   interrupts = <0 50 4>;
   interrupt-parent = <&intc>;
  };

  spi0: spi@e0006000 {
   compatible = "xlnx,zynq-spi-r1p6";
   reg = <0xe0006000 0x1000>;
   status = "disabled";
   interrupt-parent = <&intc>;
   interrupts = <0 26 4>;
   clocks = <&clkc 25>, <&clkc 34>;
   clock-names = "ref_clk", "pclk";
   #address-cells = <1>;
   #size-cells = <0>;
  };

  spi1: spi@e0007000 {
   compatible = "xlnx,zynq-spi-r1p6";
   reg = <0xe0007000 0x1000>;
   status = "disabled";
   interrupt-parent = <&intc>;
   interrupts = <0 49 4>;
   clocks = <&clkc 26>, <&clkc 35>;
   clock-names = "ref_clk", "pclk";
   #address-cells = <1>;
   #size-cells = <0>;
  };

  qspi: spi@e000d000 {
   compatible = "xlnx,zynq-qspi-1.0";
   reg = <0xe000d000 0x1000>;
   interrupt-parent = <&intc>;
   interrupts = <0 19 4>;
   clocks = <&clkc 10>, <&clkc 43>;
   clock-names = "ref_clk", "pclk";
   status = "disabled";
   #address-cells = <1>;
   #size-cells = <0>;
  };

  gem0: ethernet@e000b000 {
   compatible = "xlnx,zynq-gem", "cdns,gem";
   reg = <0xe000b000 0x1000>;
   status = "disabled";
   interrupts = <0 22 4>;
   interrupt-parent = <&intc>;
   clocks = <&clkc 30>, <&clkc 30>, <&clkc 13>;
   clock-names = "pclk", "hclk", "tx_clk";
   #address-cells = <1>;
   #size-cells = <0>;
  };

  gem1: ethernet@e000c000 {
   compatible = "xlnx,zynq-gem", "cdns,gem";
   reg = <0xe000c000 0x1000>;
   status = "disabled";
   interrupts = <0 45 4>;
   interrupt-parent = <&intc>;
   clocks = <&clkc 31>, <&clkc 31>, <&clkc 14>;
   clock-names = "pclk", "hclk", "tx_clk";
   #address-cells = <1>;
   #size-cells = <0>;
  };

  smcc: memory-controller@e000e000 {
   compatible = "arm,pl353-smc-r2p1", "arm,primecell";
   reg = <0xe000e000 0x0001000>;
   status = "disabled";
   clock-names = "memclk", "apb_pclk";
   clocks = <&clkc 11>, <&clkc 44>;
   ranges = <0x0 0x0 0xe1000000 0x1000000
      0x1 0x0 0xe2000000 0x2000000
      0x2 0x0 0xe4000000 0x2000000>;
   #address-cells = <2>;
   #size-cells = <1>;
   interrupt-parent = <&intc>;
   interrupts = <0 18 4>;
   nfc0: nand-controller@0,0 {
    compatible = "arm,pl353-nand-r2p1";
    reg = <0 0 0x1000000>;
    status = "disabled";
   };
   nor0: flash@1,0 {
    status = "disabled";
    compatible = "cfi-flash";
    reg = <1 0 0x2000000>;
   };
  };

  sdhci0: mmc@e0100000 {
   compatible = "arasan,sdhci-8.9a";
   status = "disabled";
   clock-names = "clk_xin", "clk_ahb";
   clocks = <&clkc 21>, <&clkc 32>;
   interrupt-parent = <&intc>;
   interrupts = <0 24 4>;
   reg = <0xe0100000 0x1000>;
  };

  sdhci1: mmc@e0101000 {
   compatible = "arasan,sdhci-8.9a";
   status = "disabled";
   clock-names = "clk_xin", "clk_ahb";
   clocks = <&clkc 22>, <&clkc 33>;
   interrupt-parent = <&intc>;
   interrupts = <0 47 4>;
   reg = <0xe0101000 0x1000>;
  };

  slcr: slcr@f8000000 {
   bootph-all;
   #address-cells = <1>;
   #size-cells = <1>;
   compatible = "xlnx,zynq-slcr", "syscon", "simple-mfd";
   reg = <0xF8000000 0x1000>;
   ranges;
   clkc: clkc@100 {
    bootph-all;
    #clock-cells = <1>;
    compatible = "xlnx,ps7-clkc";
    fclk-enable = <0xf>;
    clock-output-names = "armpll", "ddrpll", "iopll", "cpu_6or4x",
      "cpu_3or2x", "cpu_2x", "cpu_1x", "ddr2x", "ddr3x",
      "dci", "lqspi", "smc", "pcap", "gem0", "gem1",
      "fclk0", "fclk1", "fclk2", "fclk3", "can0", "can1",
      "sdio0", "sdio1", "uart0", "uart1", "spi0", "spi1",
      "dma", "usb0_aper", "usb1_aper", "gem0_aper",
      "gem1_aper", "sdio0_aper", "sdio1_aper",
      "spi0_aper", "spi1_aper", "can0_aper", "can1_aper",
      "i2c0_aper", "i2c1_aper", "uart0_aper", "uart1_aper",
      "gpio_aper", "lqspi_aper", "smc_aper", "swdt",
      "dbg_trc", "dbg_apb";
    reg = <0x100 0x100>;
   };

   rstc: rstc@200 {
    compatible = "xlnx,zynq-reset";
    reg = <0x200 0x48>;
    #reset-cells = <1>;
    syscon = <&slcr>;
   };

   pinctrl0: pinctrl@700 {
    compatible = "xlnx,pinctrl-zynq";
    reg = <0x700 0x200>;
    syscon = <&slcr>;
   };
  };

  dmac_s: dma-controller@f8003000 {
   compatible = "arm,pl330", "arm,primecell";
   reg = <0xf8003000 0x1000>;
   interrupt-parent = <&intc>;




   interrupts = <0 13 4>,
                <0 14 4>, <0 15 4>,
                <0 16 4>, <0 17 4>,
                <0 40 4>, <0 41 4>,
                <0 42 4>, <0 43 4>;
   #dma-cells = <1>;
   clocks = <&clkc 27>;
   clock-names = "apb_pclk";
  };

  devcfg: devcfg@f8007000 {
   compatible = "xlnx,zynq-devcfg-1.0";
   reg = <0xf8007000 0x100>;
   interrupt-parent = <&intc>;
   interrupts = <0 8 4>;
   clocks = <&clkc 12>, <&clkc 15>, <&clkc 16>, <&clkc 17>, <&clkc 18>;
   clock-names = "ref_clk", "fclk0", "fclk1", "fclk2", "fclk3";
   syscon = <&slcr>;
  };

  efuse: efuse@f800d000 {
   compatible = "xlnx,zynq-efuse";
   reg = <0xf800d000 0x20>;
  };

  global_timer: timer@f8f00200 {
   compatible = "arm,cortex-a9-global-timer";
   reg = <0xf8f00200 0x20>;
   interrupts = <1 11 0x301>;
   interrupt-parent = <&intc>;
   clocks = <&clkc 4>;
  };

  ttc0: timer@f8001000 {
   interrupt-parent = <&intc>;
   interrupts = <0 10 4>, <0 11 4>, <0 12 4>;
   compatible = "cdns,ttc";
   clocks = <&clkc 6>;
   reg = <0xF8001000 0x1000>;
  };

  ttc1: timer@f8002000 {
   interrupt-parent = <&intc>;
   interrupts = <0 37 4>, <0 38 4>, <0 39 4>;
   compatible = "cdns,ttc";
   clocks = <&clkc 6>;
   reg = <0xF8002000 0x1000>;
  };

  scutimer: timer@f8f00600 {
   bootph-all;
   interrupt-parent = <&intc>;
   interrupts = <1 13 0x301>;
   compatible = "arm,cortex-a9-twd-timer";
   reg = <0xf8f00600 0x20>;
   clocks = <&clkc 4>;
  };

  scuwdt: scuwatchdog@f8f00620 {
   interrupt-parent = <&intc>;
   interrupts = <1 14 4>;
   compatible = "xlnx,ps7-scuwdt-1.00.a";
   reg = <0xf8f00620 0xe0>;
  };

  usb0: usb@e0002000 {
   compatible = "xlnx,zynq-usb-2.20a", "chipidea,usb2";
   status = "disabled";
   clocks = <&clkc 28>;
   interrupt-parent = <&intc>;
   interrupts = <0 21 4>;
   reg = <0xe0002000 0x1000>;
   phy_type = "ulpi";
  };

  usb1: usb@e0003000 {
   compatible = "xlnx,zynq-usb-2.20a", "chipidea,usb2";
   status = "disabled";
   clocks = <&clkc 29>;
   interrupt-parent = <&intc>;
   interrupts = <0 44 4>;
   reg = <0xe0003000 0x1000>;
   phy_type = "ulpi";
  };

  watchdog0: watchdog@f8005000 {
   clocks = <&clkc 45>;
   compatible = "cdns,wdt-r1p2";
   interrupt-parent = <&intc>;
   interrupts = <0 9 1>;
   reg = <0xf8005000 0x1000>;
   timeout-sec = <10>;
  };

  coresight: coresight@f8800000 {
   compatible = "xlnx,ps7-coresight-comp-1.00.a";
   status = "disabled";
   reg = <0xf8800000 0x100000>;
  };

  etb@f8801000 {
   compatible = "arm,coresight-etb10", "arm,primecell";
   reg = <0xf8801000 0x1000>;
   clocks = <&clkc 27>, <&clkc 46>, <&clkc 47>;
   clock-names = "apb_pclk", "dbg_trc", "dbg_apb";
   in-ports {
    port {
     etb_in_port: endpoint {
      remote-endpoint = <&replicator_out_port1>;
     };
    };
   };
  };

  tpiu@f8803000 {
   compatible = "arm,coresight-tpiu", "arm,primecell";
   reg = <0xf8803000 0x1000>;
   clocks = <&clkc 27>, <&clkc 46>, <&clkc 47>;
   clock-names = "apb_pclk", "dbg_trc", "dbg_apb";
   in-ports {
    port {
     tpiu_in_port: endpoint {
      remote-endpoint = <&replicator_out_port0>;
     };
    };
   };
  };

  funnel@f8804000 {
   compatible = "arm,coresight-static-funnel", "arm,primecell";
   reg = <0xf8804000 0x1000>;
   clocks = <&clkc 27>, <&clkc 46>, <&clkc 47>;
   clock-names = "apb_pclk", "dbg_trc", "dbg_apb";


   out-ports {
    port {
     funnel_out_port: endpoint {
      remote-endpoint =
       <&replicator_in_port0>;
     };
    };
   };

   in-ports {
    #address-cells = <1>;
    #size-cells = <0>;


    port@0 {
     reg = <0>;
     funnel0_in_port0: endpoint {
      remote-endpoint = <&ptm0_out_port>;
     };
    };

    port@1 {
     reg = <1>;
     funnel0_in_port1: endpoint {
      remote-endpoint = <&ptm1_out_port>;
     };
    };

    port@2 {
     reg = <2>;
    };

   };
  };

  ptm@f889c000 {
   compatible = "arm,coresight-etm3x", "arm,primecell";
   reg = <0xf889c000 0x1000>;
   clocks = <&clkc 27>, <&clkc 46>, <&clkc 47>;
   clock-names = "apb_pclk", "dbg_trc", "dbg_apb";
   cpu = <&ps7_cortexa9_0>;
   out-ports {
    port {
     ptm0_out_port: endpoint {
      remote-endpoint = <&funnel0_in_port0>;
     };
    };
   };
  };

  ptm@f889d000 {
   compatible = "arm,coresight-etm3x", "arm,primecell";
   reg = <0xf889d000 0x1000>;
   clocks = <&clkc 27>, <&clkc 46>, <&clkc 47>;
   clock-names = "apb_pclk", "dbg_trc", "dbg_apb";
   cpu = <&ps7_cortexa9_1>;
   out-ports {
    port {
     ptm1_out_port: endpoint {
      remote-endpoint = <&funnel0_in_port1>;
     };
    };
   };
  };
 };
};
# 3 "C:\\Users\\Juergen\\Documents\\Projects\\FPGA\\Arty-Z7-A\\ArtyZ7-SPI-GPIO-SW\\SPI-GPIO-Platform\\hw\\sdt\\system-top.dts" 2
# 1 "C:\\Users\\Juergen\\Documents\\Projects\\FPGA\\Arty-Z7-A\\ArtyZ7-SPI-GPIO-SW\\SPI-GPIO-Platform\\hw\\sdt\\pl.dtsi" 1
/ {
 amba_pl: amba_pl {
  ranges;
  compatible = "simple-bus";
  #address-cells = <1>;
  #size-cells = <1>;
  firmware-name = "Periphery_wrapper.bit.bin";
  clocking0: clocking0 {
   compatible = "xlnx,fclk";
   assigned-clocks = <&clkc 15>;
   assigned-clock-rates = <100000000>;
   #clock-cells = <0>;
   clock-output-names = "fabric_clk";
   clocks = <&clkc 15>;
  };
  axi_gpio_0: gpio@41200000 {
   #interrupt-cells = <2>;
   interrupts = < 0 29 4 >;
   xlnx,gpio-board-interface = "btns_4bits";
   compatible = "xlnx,axi-gpio-2.0" , "xlnx,xps-gpio-1.00.a";
   xlnx,all-outputs = <0>;
   #gpio-cells = <2>;
   xlnx,gpio-width = <4>;
   interrupt-parent = <&intc>;
   xlnx,rable = <0>;
   xlnx,dout-default = <0x0>;
   xlnx,is-dual = <1>;
   xlnx,ip-name = "axi_gpio";
   xlnx,tri-default-2 = <0xffffffff>;
   reg = <0x41200000 0x10000>;
   xlnx,all-inputs-2 = <0>;
   clocks = <&clkc 15>;
   xlnx,all-outputs-2 = <1>;
   gpio-controller;
   xlnx,interrupt-present = <1>;
   xlnx,gpio2-board-interface = "leds_4bits";
   xlnx,edk-iptype = "PERIPHERAL";
   xlnx,dout-default-2 = <0x0>;
   status = "okay";
   xlnx,gpio2-width = <4>;
   clock-names = "s_axi_aclk";
   xlnx,use-board-flow;
   interrupt-controller;
   interrupt-names = "ip2intc_irpt";
   xlnx,tri-default = <0xffffffff>;
   xlnx,name = "axi_gpio_0";
   xlnx,all-inputs = <1>;
  };
  axi_gpio_1: gpio@41210000 {
   xlnx,gpio-board-interface = "shield_dp0_dp13";
   compatible = "xlnx,axi-gpio-2.0" , "xlnx,xps-gpio-1.00.a";
   xlnx,all-outputs = <0>;
   #gpio-cells = <2>;
   xlnx,gpio-width = <14>;
   xlnx,rable = <0>;
   xlnx,dout-default = <0x0>;
   xlnx,is-dual = <0>;
   xlnx,ip-name = "axi_gpio";
   xlnx,tri-default-2 = <0xffffffff>;
   reg = <0x41210000 0x10000>;
   xlnx,all-inputs-2 = <0>;
   clocks = <&clkc 15>;
   xlnx,all-outputs-2 = <0>;
   gpio-controller;
   xlnx,interrupt-present = <0>;
   xlnx,gpio2-board-interface = "Custom";
   xlnx,edk-iptype = "PERIPHERAL";
   xlnx,dout-default-2 = <0x0>;
   status = "okay";
   xlnx,gpio2-width = <32>;
   clock-names = "s_axi_aclk";
   xlnx,use-board-flow;
   xlnx,tri-default = <0xffffffff>;
   xlnx,name = "axi_gpio_1";
   xlnx,all-inputs = <0>;
  };
 };
};
# 4 "C:\\Users\\Juergen\\Documents\\Projects\\FPGA\\Arty-Z7-A\\ArtyZ7-SPI-GPIO-SW\\SPI-GPIO-Platform\\hw\\sdt\\system-top.dts" 2
# 1 "C:\\Users\\Juergen\\Documents\\Projects\\FPGA\\Arty-Z7-A\\ArtyZ7-SPI-GPIO-SW\\SPI-GPIO-Platform\\hw\\sdt\\pcw.dtsi" 1
 &ps7_cortexa9_0 {
  xlnx,rable = <0>;
  xlnx,i-cache-size = <0x8000>;
  xlnx,d-cache-line-size = <20>;
  xlnx,i-cache-line-size = <20>;
  xlnx,cpu-1x-clk-freq-hz = <108333336>;
  xlnx,ip-name = "ps7_cortexa9";
  xlnx,d-cache-size = <0x8000>;
  xlnx,num-cores = <2>;
  xlnx,cpu-clk-freq-hz = <650000000>;
  bus-handle = <&amba>;
 };
 &ps7_cortexa9_1 {
  xlnx,rable = <0>;
  xlnx,i-cache-size = <0x8000>;
  xlnx,d-cache-line-size = <20>;
  xlnx,i-cache-line-size = <20>;
  xlnx,cpu-1x-clk-freq-hz = <108333336>;
  xlnx,ip-name = "ps7_cortexa9";
  xlnx,d-cache-size = <0x8000>;
  xlnx,cpu-clk-freq-hz = <650000000>;
  bus-handle = <&amba>;
 };
 &amba {
  ps7_pmu_0: ps7_pmu@f8891000 {
   xlnx,rable = <0>;
   compatible = "xlnx,ps7-pmu-1.00.a";
   status = "okay";
   xlnx,ip-name = "ps7_pmu";
   reg = <0xf8891000 0x1000 0xf8893000 0x1000>;
   xlnx,name = "ps7_pmu_0";
  };
  ps7_ddr_0: ps7_ddr@100000 {
   xlnx,rable = <0>;
   compatible = "xlnx,ps7-ddr-1.00.a";
   status = "okay";
   xlnx,ip-name = "ps7_ddr";
   reg = <0x00100000 0x1ff00000>;
   xlnx,name = "ps7_ddr_0";
  };
  ps7_ocmc_0: ps7_ocmc@f800c000 {
   xlnx,rable = <0>;
   compatible = "xlnx,ps7-ocmc-1.00.a";
   status = "okay";
   xlnx,ip-name = "ps7_ocmc";
   reg = <0xf800c000 0x1000>;
   xlnx,name = "ps7_ocmc_0";
  };
  ps7_gpv_0: ps7_gpv@f8900000 {
   xlnx,rable = <0>;
   compatible = "xlnx,ps7-gpv-1.00.a";
   status = "okay";
   xlnx,ip-name = "ps7_gpv";
   reg = <0xf8900000 0x100000>;
   xlnx,name = "ps7_gpv_0";
  };
  ps7_scuc_0: ps7_scuc@f8f00000 {
   xlnx,rable = <0>;
   compatible = "xlnx,ps7-scuc-1.00.a";
   status = "okay";
   xlnx,ip-name = "ps7_scuc";
   reg = <0xf8f00000 0xfd>;
   xlnx,name = "ps7_scuc_0";
  };
  ps7_iop_bus_config_0: ps7_iop_bus_config@e0200000 {
   xlnx,rable = <0>;
   compatible = "xlnx,ps7-iop-bus-config-1.00.a";
   status = "okay";
   xlnx,ip-name = "ps7_iop_bus_config";
   reg = <0xe0200000 0x1000>;
   xlnx,name = "ps7_iop_bus_config_0";
  };
  ps7_ram_0: ps7_ram@0 {
   xlnx,rable = <0>;
   compatible = "xlnx,ps7-ram-1.00.a";
   status = "okay";
   xlnx,ip-name = "ps7_ram";
   reg = <0x00000000 0x30000>;
   xlnx,name = "ps7_ram_0";
  };
  ps7_ram_1: ps7_ram@ffff0000 {
   xlnx,rable = <0>;
   compatible = "xlnx,ps7-ram-1.00.a";
   status = "okay";
   xlnx,ip-name = "ps7_ram";
   reg = <0xffff0000 0xfe00>;
   xlnx,name = "ps7_ram_1";
  };
  ps7_dma_ns: ps7_dma@f8004000 {
   xlnx,rable = <0>;
   compatible = "xlnx,ps7-dma-1.00.a";
   status = "okay";
   xlnx,ip-name = "ps7_dma";
   xlnx,is-secure;
   reg = <0xf8004000 0x1000>;
   xlnx,name = "ps7_dma_ns";
  };
  ps7_afi_0: ps7_afi@f8008000 {
   xlnx,rable = <0>;
   compatible = "xlnx,ps7-afi-1.00.a";
   status = "okay";
   xlnx,ip-name = "ps7_afi";
   reg = <0xf8008000 0x1000>;
   xlnx,name = "ps7_afi_0";
  };
  ps7_afi_1: ps7_afi@f8009000 {
   xlnx,rable = <0>;
   compatible = "xlnx,ps7-afi-1.00.a";
   status = "okay";
   xlnx,ip-name = "ps7_afi";
   reg = <0xf8009000 0x1000>;
   xlnx,name = "ps7_afi_1";
  };
  ps7_afi_2: ps7_afi@f800a000 {
   xlnx,rable = <0>;
   compatible = "xlnx,ps7-afi-1.00.a";
   status = "okay";
   xlnx,ip-name = "ps7_afi";
   reg = <0xf800a000 0x1000>;
   xlnx,name = "ps7_afi_2";
  };
  ps7_afi_3: ps7_afi@f800b000 {
   xlnx,rable = <0>;
   compatible = "xlnx,ps7-afi-1.00.a";
   status = "okay";
   xlnx,ip-name = "ps7_afi";
   reg = <0xf800b000 0x1000>;
   xlnx,name = "ps7_afi_3";
  };
  ps7_m_axi_gp0: ps7_m_axi_gp@40000000 {
   xlnx,rable = <0>;
   compatible = "xlnx,ps7-m-axi-gp-1.00.a";
   status = "okay";
   xlnx,ip-name = "ps7_m_axi_gp";
   xlnx,is-hierarchy;
   reg = <0x40000000 0x40000000>;
   xlnx,name = "ps7_m_axi_gp0";
  };
  ps7_qspi_linear_0: ps7_qspi_linear@fc000000 {
   reg = <0xfc000000 0x1000000>;
  };
 };
 &uart0 {
  xlnx,rable = <0>;
  status = "okay";
  xlnx,clock-freq = <100000000>;
  xlnx,has-modem = <0>;
  xlnx,ip-name = "ps7_uart";
  cts-override;
  port-number = <0>;
  xlnx,uart-clk-freq-hz = <100000000>;
  xlnx,name = "ps7_uart_0";
 };
 &qspi {
  num-cs = <1>;
  xlnx,qspi-clk-freq-hz = <200000000>;
  xlnx,rable = <0>;
  xlnx,bus-width = <2>;
  xlnx,ip-name = "ps7_qspi";
  spi-rx-bus-width = <4>;
  xlnx,connection-mode = <0>;
  spi-tx-bus-width = <4>;
  status = "okay";
  qspi-fbclk = <0>;
  xlnx,clock-freq = <200000000>;
  xlnx,fb-clk = <1>;
  xlnx,qspi-mode = <0>;
  xlnx,name = "ps7_qspi_0";
  xlnx,qspi-bus-width = <2>;
  is-dual = <0>;
 };
 &gem0 {
  xlnx,has-mdio = <1>;
  phy-mode = "rgmii-id";
  xlnx,enet-slcr-1000mbps-div0 = <8>;
  xlnx,enet-slcr-10mbps-div0 = <8>;
  enet-reset = <&gpio0 9 0>;
  xlnx,rable = <0>;
  xlnx,enet-slcr-1000mbps-div1 = <1>;
  xlnx,enet-slcr-10mbps-div1 = <50>;
  xlnx,ip-name = "ps7_ethernet";
  xlnx,eth-mode = <1>;
  xlnx,enet-reset = "MIO , 9";
  xlnx,enet-clk-freq-hz = <125000000>;
  xlnx,enet-slcr-100mbps-div0 = <8>;
  xlnx,ptp-enet-clock = <0x6750918>;
  local-mac-address = [ 00 0a 23 00 00 00 ];
  status = "okay";
  xlnx,enet-slcr-100mbps-div1 = <5>;
  xlnx,name = "ps7_ethernet_0";
 };
 &usb0 {
  xlnx,rable = <0>;
  xlnx,usb-reset = "MIO , 46";
  status = "okay";
  xlnx,ip-name = "ps7_usb";
  phy_type = "ulpi";
  usb-reset = <&gpio0 46 0>;
  xlnx,name = "ps7_usb_0";
 };
 &sdhci0 {
  xlnx,rable = <0>;
  xlnx,has-power = <0>;
  status = "okay";
  xlnx,ip-name = "ps7_sdio";
  xlnx,sdio-clk-freq-hz = <50000000>;
  xlnx,has-wp = <0>;
  xlnx,has-cd = <1>;
  xlnx,name = "ps7_sd_0";
 };
 &i2c0 {
  xlnx,rable = <0>;
  xlnx,has-interrupt = <1>;
  status = "okay";
  xlnx,clock-freq = <108333336>;
  xlnx,i2c-clk-freq-hz = <108333336>;
  xlnx,ip-name = "ps7_i2c";
  xlnx,name = "ps7_i2c_0";
 };
 &spi0 {
  xlnx,rable = <0>;
  status = "okay";
  xlnx,has-ss0 = <1>;
  xlnx,ip-name = "ps7_spi";
  xlnx,has-ss1 = <1>;
  num-cs = <3>;
  xlnx,spi-clk-freq-hz = <166666672>;
  xlnx,has-ss2 = <1>;
  xlnx,name = "ps7_spi_0";
 };
 &gpio0 {
  xlnx,rable = <0>;
  xlnx,mio-gpio-mask = <0x5600>;
  status = "okay";
  gpio-mask-high = <0>;
  xlnx,emio-gpio-width = <64>;
  xlnx,ip-name = "ps7_gpio";
  gpio-mask-low = <22016>;
  emio-gpio-width = <0x40>;
  xlnx,name = "ps7_gpio_0";
 };
 &mc {
  xlnx,rable = <0>;
  status = "okay";
  xlnx,ddr-clk-freq-hz = <525000000>;
  xlnx,ip-name = "ps7_ddrc";
  xlnx,has-ecc = <0>;
  xlnx,name = "ps7_ddrc_0";
 };
 &devcfg {
  xlnx,rable = <0>;
  status = "okay";
  xlnx,ip-name = "ps7_dev_cfg";
  xlnx,name = "ps7_dev_cfg_0";
 };
 &adc {
  xlnx,rable = <0>;
  status = "okay";
  xlnx,ip-name = "ps7_xadc";
  xlnx,name = "ps7_xadc_0";
 };
 &coresight {
  xlnx,rable = <0>;
  status = "okay";
  xlnx,ip-name = "ps7_coresight_comp";
  xlnx,name = "ps7_coresight_comp_0";
 };
 &global_timer {
  xlnx,rable = <0>;
  status = "okay";
  xlnx,ip-name = "ps7_globaltimer";
  xlnx,name = "ps7_globaltimer_0";
 };
 &L2 {
  xlnx,rable = <0>;
  status = "okay";
  xlnx,ip-name = "ps7_pl310";
  xlnx,name = "ps7_pl310_0";
 };
 &dmac_s {
  xlnx,rable = <0>;
  status = "okay";
  xlnx,ip-name = "ps7_dma";
  xlnx,is-secure;
  xlnx,name = "ps7_dma_s";
 };
 &intc {
  xlnx,rable = <0>;
  status = "okay";
  xlnx,irq-f2p-mode = "DIRECT";
  xlnx,ip-name = "ps7_intc_dist";
  xlnx,name = "ps7_intc_dist_0";
 };
 &scutimer {
  xlnx,rable = <0>;
  status = "okay";
  xlnx,ip-name = "ps7_scutimer";
  xlnx,name = "ps7_scutimer_0";
 };
 &scuwdt {
  xlnx,rable = <0>;
  status = "okay";
  xlnx,ip-name = "ps7_scuwdt";
  xlnx,name = "ps7_scuwdt_0";
 };
 &slcr {
  xlnx,rable = <0>;
  status = "okay";
  xlnx,ip-name = "ps7_slcr";
  xlnx,name = "ps7_slcr_0";
 };
 &clkc {
  fclk-enable = <0x1>;
  ps-clk-frequency = <50000000>;
 };
# 5 "C:\\Users\\Juergen\\Documents\\Projects\\FPGA\\Arty-Z7-A\\ArtyZ7-SPI-GPIO-SW\\SPI-GPIO-Platform\\hw\\sdt\\system-top.dts" 2
/ {
 board = "arty-z7-20";
 compatible = "xlnx,arty-z7-20";
 device_id = "7z020";
 slrcount = <1>;
 family = "Zynq";
 ps7_qspi_linear_0_memory: memory@fc000000 {
  compatible = "xlnx,ps7-qspi-linear-1.00.a-memory";
  xlnx,ip-name = "ps7_qspi_linear";
  device_type = "memory";
  memory_type = "linear_flash";
  reg = <0xfc000000 0x1000000>;
 };
 ps7_ddr_0_memory: memory@00100000 {
  compatible = "xlnx,ps7-ddr-1.00.a";
  xlnx,ip-name = "ps7_ddr";
  device_type = "memory";
  memory_type = "memory";
  reg = <0x00100000 0x1FF00000>;
 };
 ps7_ram_0_memory: memory@0 {
  compatible = "xlnx,ps7-ram-1.00.a";
  xlnx,ip-name = "ps7_ram";
  device_type = "memory";
  memory_type = "memory";
  reg = <0x0 0x30000>;
 };
 ps7_ram_1_memory: memory@ffff0000 {
  compatible = "xlnx,ps7-ram-1.00.a";
  xlnx,ip-name = "ps7_ram";
  device_type = "memory";
  memory_type = "memory";
  reg = <0xffff0000 0xfe00>;
 };
 chosen {
  stdout-path = "serial0:115200n8";
 };
 aliases {
  serial0 = &uart0;
  spi0 = &qspi;
  serial1 = &coresight;
  i2c0 = &i2c0;
  ethernet0 = &gem0;
 };
 cpus_a9: cpus-a9@0 {
  compatible = "cpus,cluster";
  address-map = <0xf0000000 &amba 0xf0000000 0x10000000>,
         <0x00100000 &ps7_ddr_0_memory 0x00100000 0x1FF00000>,
         <0x0 &ps7_ram_0_memory 0x0 0x30000>,
         <0xffff0000 &ps7_ram_1_memory 0xffff0000 0xfe00>,
         <0x41200000 &axi_gpio_0 0x41200000 0x10000>,
         <0x41210000 &axi_gpio_1 0x41210000 0x10000>,
         <0xf8008000 &ps7_afi_0 0xf8008000 0x1000>,
         <0xf8009000 &ps7_afi_1 0xf8009000 0x1000>,
         <0xf800a000 &ps7_afi_2 0xf800a000 0x1000>,
         <0xf800b000 &ps7_afi_3 0xf800b000 0x1000>,
         <0xf8800000 &coresight 0xf8800000 0x100000>,
         <0xf8006000 &mc 0xf8006000 0x1000>,
         <0xf8007000 &devcfg 0xf8007000 0x100>,
         <0xf8004000 &ps7_dma_ns 0xf8004000 0x1000>,
         <0xf8003000 &dmac_s 0xf8003000 0x1000>,
         <0xe000b000 &gem0 0xe000b000 0x1000>,
         <0xf8f00200 &global_timer 0xf8f00200 0x100>,
         <0xe000a000 &gpio0 0xe000a000 0x1000>,
         <0xf8900000 &ps7_gpv_0 0xf8900000 0x100000>,
         <0xe0004000 &i2c0 0xe0004000 0x1000>,
         <0xf8f01000 &intc 0xf8f01000 0x1000>,
         <0xe0200000 &ps7_iop_bus_config_0 0xe0200000 0x1000>,
         <0xf8f02000 &L2 0xf8f02000 0x1000>,
         <0xf800c000 &ps7_ocmc_0 0xf800c000 0x1000>,
         <0xf8891000 &ps7_pmu_0 0xf8891000 0x1000>,
         <0xe000d000 &qspi 0xe000d000 0x1000>,
         <0xfc000000 &ps7_qspi_linear_0_memory 0xfc000000 0x1000000>,
         <0xf8f00000 &ps7_scuc_0 0xf8f00000 0xfd>,
         <0xf8f00600 &scutimer 0xf8f00600 0x20>,
         <0xf8f00620 &scuwdt 0xf8f00620 0xe0>,
         <0xe0100000 &sdhci0 0xe0100000 0x1000>,
         <0xf8000000 &slcr 0xf8000000 0x1000>,
         <0xe0006000 &spi0 0xe0006000 0x1000>,
         <0xe0000000 &uart0 0xe0000000 0x1000>,
         <0xe0002000 &usb0 0xe0002000 0x1000>,
         <0xf8007100 &adc 0xf8007100 0x21>;
  #ranges-address-cells = <0x1>;
  #ranges-size-cells = <0x1>;
 };
};
