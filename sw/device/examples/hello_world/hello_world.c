// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include "sw/device/examples/demos.h"
#include "sw/device/lib/arch/device.h"
#include "sw/device/lib/base/log.h"
#include "sw/device/lib/dif/dif_gpio.h"
#include "sw/device/lib/pinmux.h"
#include "sw/device/lib/runtime/hart.h"
#include "sw/device/lib/spi_device.h"
#include "sw/device/lib/testing/test_status.h"
#include "sw/device/lib/uart.h"

static dif_gpio_t gpio;

void trigger_CW_high(dif_gpio_t* gpio_ref);
void trigger_CW_low(dif_gpio_t* gpio_ref);

char arr[64];

volatile int i_c = 0;

int main(int argc, char **argv) {
  
  for (i_c = 0; i_c < 64; i_c++)
  {
     arr[i_c] = 0;      
  }
  i_c=0;
  uart_init(kUartBaudrate);
  base_set_stdout(uart_stdout);

  pinmux_init();
  spid_init();

  dif_gpio_config_t gpio_config = {.base_addr =
                                       mmio_region_from_addr(0x40010000)};
  dif_gpio_init(&gpio_config, &gpio);
  // Enable GPIO: 0-7 and 16 is input; 8-15 is output.
  dif_gpio_output_mode_all_set(&gpio, 0x0ff00);
  // Add DATE and TIME because I keep fooling myself with old versions
  LOG_INFO("Hello World!");
  LOG_INFO("Built at: " __DATE__ ", " __TIME__);

  // End the test here for DV simulation, since the rest of the code is more
  // appropriate for an FPGA demonstration.
  if (kDeviceType == kDeviceSimDV) {
    test_status_set(kTestStatusPassed);
  }

  demo_gpio_startup(&gpio);

  // Now have UART <-> Buttons/LEDs demo
  // all LEDs off
  dif_gpio_all_write(&gpio, 0xff00);
  /*LOG_INFO("Try out the switches on the board");
  LOG_INFO("or type anything into the console window.");
  LOG_INFO("The LEDs show the ASCII code of the last character.");
  usleep(10 * 1000);  // 10 ms*/
  spid_send("SPI!", 4);

  //uint32_t gpio_state = 0;
  while (true) {
    //dif_gpio_all_write(&gpio, 0xff00);
    usleep(1000 * 1000);  // 1000 ms
    dif_gpio_all_write(&gpio, 0x0000);
    trigger_CW_high(&gpio);
    //gpio_state = demo_gpio_to_log_echo(&gpio, gpio_state);
    //demo_spi_to_log_echo();
    //demo_uart_to_uart_and_gpio_echo(&gpio);
    //usleep(1000 * 1000);  // 1000 ms
    /*volatile int b = 0xAFFA;
    for (volatile int i = 0; i < 1000; i++)
      b *= 11;*/
    //for(volatile int i = 0; i < 10; i++);
     
    usleep(50 * 1000);  // 50 ms
    //uart_send_str("rCBBD4A2B34F2571758FF6A797E09859D");   
    trigger_CW_low(&gpio);
    usleep(200 * 1000);  // 200 ms

    do{
	 //for (i_c = 0; i_c < 64; i_c++)
	 //{
	    uart_rcv_char(&arr[i_c]);
	    //uart_send_str(arr[i_c]);
	 //}
	if(arr[i_c] == 61){ // =
    		dif_gpio_all_write(&gpio, 0xff00);
		usleep(2000 * 1000);  // 1000 ms
        }
	if(arr[i_c] == 60){ // <
	    	dif_gpio_all_write(&gpio, 0x4400);
		usleep(2000 * 1000);  // 1000 ms
	}
	if(arr[i_c] == 63){ // ?
	    	dif_gpio_all_write(&gpio, 0x3300);
		usleep(2000 * 1000);  // 1000 ms
	}
	 i_c++;
         if(i_c == 64){ i_c = 0;}
    }while(arr[i_c] != 0x0A);
    //LOG_INFO("Hello World!");
    
    for (i_c = 0; i_c < 64; i_c++)
    {
        arr[i_c] = 0;
    }
    i_c = 0;
  }
}

void trigger_CW_high(dif_gpio_t* gpio_ref){
  dif_gpio_all_write(&gpio, 0x8000);
}
void trigger_CW_low(dif_gpio_t* gpio_ref){
  dif_gpio_all_write(&gpio, 0x0000);
}
