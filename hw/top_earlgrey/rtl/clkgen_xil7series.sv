// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

module clkgen_xil7series (
  input IO_CLK,
  input IO_RST_N,
  output clk_sys,
  output clk_48MHz,
  output rst_sys_n,
  output O_CLK,
  output O_CLK_div
);
  logic locked_pll;
  logic io_clk_buf;
  logic clk_50_buf;
  logic clk_50_unbuf;
  logic clk_fb_buf;
  logic clk_fb_unbuf;
  logic clk_48_buf;
  logic clk_48_unbuf;
  
  // input buffer
  IBUF io_clk_ibuf (
    .I (IO_CLK),
    .O (io_clk_buf)
  );
  
  /*PLLE2_ADV #(
    .BANDWIDTH            ("OPTIMIZED"),
    .COMPENSATION         ("ZHOLD"),
    .STARTUP_WAIT         ("FALSE"),
    .DIVCLK_DIVIDE        (1),
    .CLKFBOUT_MULT        (40),
    .CLKFBOUT_PHASE       (0.000),
    .CLKOUT0_DIVIDE       (100),
    .CLKOUT0_PHASE        (0.000),
    .CLKOUT0_DUTY_CYCLE   (0.500),
    .CLKOUT1_DIVIDE       (100),
    .CLKOUT1_PHASE        (0.000),
    .CLKOUT1_DUTY_CYCLE   (0.500),
    .CLKIN1_PERIOD        (50.000)
  ) pll (
    .CLKFBOUT            (clk_fb_unbuf),
    .CLKOUT0             (clk_50_unbuf),
    .CLKOUT1             (clk_48_unbuf),
    .CLKOUT2             (),
    .CLKOUT3             (),
    .CLKOUT4             (),
    .CLKOUT5             (),
     // Input clock control
    .CLKFBIN             (clk_fb_buf),
    .CLKIN1              (io_clk_buf),
    .CLKIN2              (1'b0),
     // Tied to always select the primary input clock
    .CLKINSEL            (1'b1),
    // Ports for dynamic reconfiguration
    .DADDR               (7'h0),
    .DCLK                (1'b0),
    .DEN                 (1'b0),
    .DI                  (16'h0),
    .DO                  (),
    .DRDY                (),
    .DWE                 (1'b0),
    // Other control and status signals
    .LOCKED              (locked_pll),
    .PWRDWN              (1'b0),
    // Do not reset PLL on external reset, otherwise ILA disconnects at a reset
    .RST                 (1'b0));

  // output buffering
  BUFG clk_fb_bufg (
    .I (clk_fb_unbuf),
    .O (clk_fb_buf)
  );

  BUFG clk_50_bufg (
    .I (clk_50_unbuf),
    .O (clk_50_buf)
  );

  BUFG clk_48_bufg (
    .I (clk_48_unbuf),
    .O (clk_48_buf)
  );
  
  

  // outputs
  // clock
  assign clk_sys = clk_50_buf; // TODO: choose 50 MHz clock as sysclock for now
  assign clk_48MHz = clk_48_buf;
  
  // outputs
  assign O_CLK = clk_sys;
  // reset
  assign rst_sys_n = locked_pll & IO_RST_N;*/
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////  
  logic clk_enable;
  logic clk_div_unbuf; // clock divider output
  logic clk_div_buf;
  logic clk_div2_unbuf;
  logic clk_div2_buf;
     
  always_ff @(posedge io_clk_buf or negedge IO_RST_N) 
  begin
      if (!IO_RST_N) 
          begin
              clk_div2_unbuf <= 0;
          end 
      else 
          begin
              clk_div2_unbuf <=~ clk_div2_unbuf;
          end 
  end
  
 always_ff @(posedge clk_div2_unbuf or negedge IO_RST_N) 
  begin
      if (!IO_RST_N) 
          begin
              clk_div_unbuf <= 0;
          end 
      else 
          begin
              clk_div_unbuf <=~ clk_div_unbuf;
          end
  end
  
  BUFG clk_bufg (
    .I (clk_div_unbuf),
    .O (clk_div_buf)
  );
  
  BUFG clk_2_bufg (
    .I (clk_div2_unbuf),
    .O (clk_div2_buf)
  );
 
  
  assign clk_sys = clk_div2_buf;
  assign clk_48MHz = clk_div2_buf;
  
  // outputs
  assign O_CLK = clk_sys;
  assign O_CLK_div = clk_div_buf;
  // reset
  assign rst_sys_n = IO_RST_N;
  
  
  
endmodule
