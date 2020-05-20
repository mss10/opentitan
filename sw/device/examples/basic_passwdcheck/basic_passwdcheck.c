// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

/*
    This file is part of the ChipWhisperer Example Targets
    Copyright (C) 2012-2015 NewAE Technology Inc.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "sw/device/examples/demos.h"
#include "sw/device/lib/arch/device.h"
#include "sw/device/lib/base/log.h"
#include "sw/device/lib/dif/dif_gpio.h"
#include "sw/device/lib/pinmux.h"
#include "sw/device/lib/runtime/hart.h"
#include "sw/device/lib/spi_device.h"
#include "sw/device/lib/testing/test_status.h"
#include "sw/device/lib/uart.h"

#define IDLE        0
#define KEY         1
#define PLAIN       2

#define BUFLEN      64


#define START_CMD   0x07 // \a
#define END_CMD     0x0A // \n

static dif_gpio_t gpio;

void trigger_high(dif_gpio_t*);
void trigger_low(dif_gpio_t*);
void led_ok(dif_gpio_t*);
void led_error(dif_gpio_t*);

void delay_2_ms(){
  usleep(2 * 1000);
}


volatile int i_c = 0;

void my_puts(char *c)
{
  //do {
    uart_send_str(c);
 // } while (*++c);
}
//uart_rcv_char

void my_read(char *buf, int len)
{
  i_c = 0;
  char c;

  // wait for start command
  do{
      uart_rcv_char(&c);
  }while(c != START_CMD); // wait for start command
  // received a start command, start reading to buffer
  //my_puts("Received start\n");
  delay_2_ms();
  delay_2_ms();
  while(buf[i_c] != END_CMD) {
    uart_rcv_char(&buf[i_c]);
    i_c++;
    if(i_c >= BUFLEN) {i_c = 0;}
  }
  if (buf[i_c] == END_CMD) {
        buf[i_c] = '\0';
  }
  i_c = 0;

  return;
}

void clear_buf(char *buf)
{
  for(int i = 0; i < BUFLEN; i++) {
      buf[i] = 0;
  }
}

int main(int argc, char **argv) {
  
  uart_init(kUartBaudrate);
  base_set_stdout(uart_stdout);

  pinmux_init();
  //spid_init();

  dif_gpio_config_t gpio_config = {.base_addr =
                                       mmio_region_from_addr(0x40010000)};
  dif_gpio_init(&gpio_config, &gpio);
  // Enable GPIO: 0-7 and 16 is input; 8-15 is output.
  dif_gpio_output_mode_all_set(&gpio, 0x0ff00);
  // Add DATE and TIME because I keep fooling myself with old versions
  //LOG_INFO("Basic Passwd Check!");
  //LOG_INFO("Built at: " __DATE__ ", " __TIME__);

  // End the test here for DV simulation, since the rest of the code is more
  // appropriate for an FPGA demonstration.
  /*if (kDeviceType == kDeviceSimDV) {
    test_status_set(kTestStatusPassed);
  }*/

  //demo_gpio_startup(&gpio);

  // Now have UART <-> Buttons/LEDs demo
  // all LEDs off
  dif_gpio_all_write(&gpio, 0xff00);
  /*LOG_INFO("Try out the switches on the board");
  LOG_INFO("or type anything into the console window.");
  LOG_INFO("The LEDs show the ASCII code of the last character.");
  usleep(10 * 1000);  // 10 ms*/
  //spid_send("SPI!", 4);
  usleep(500 * 1000);  // 100 ms*/  

  char passwd[32];
  char correct_passwd[] = "h0px3";

  clear_buf(passwd);

  //uint32_t gpio_state = 0;
    while(true){

        my_puts("*****Safe-o-matic 3000 Booting...\n");
        //Print some fancy-sounding stuff so that attackers
        //will get scared and leave us alone
        my_puts("Aligning bits........[DONE]\n");
        delay_2_ms();
        my_puts("Checking Cesium RNG..[DONE]\n");
        delay_2_ms();
        my_puts("Masquerading flash...[DONE]\n");
        delay_2_ms();
        my_puts("Decrypting database..[DONE]\n");
        delay_2_ms();
        my_puts("\n\n");

        //Give them one last warning
        my_puts("WARNING: UNAUTHORIZED ACCESS WILL BE PUNISHED\n");

        trigger_low(&gpio);

        //Get password
        my_puts("Please enter password to continue: ");
        my_read(passwd, 32);

        uint8_t passbad = 0;

        trigger_high(&gpio);

        for(uint8_t i = 0; i < sizeof(correct_passwd); i++){
            if (correct_passwd[i] != passwd[i]){
                passbad = 1;
                break;
            }
        }

        if (passbad){
            //Stop them fancy timing attacks
            delay_2_ms();
            delay_2_ms();
            my_puts("PASSWORD FAIL\n");
            led_error(&gpio);
        } else {
            my_puts("Access granted, Welcome!\n");
            led_ok(&gpio);
        }
        
        delay_2_ms();
        delay_2_ms();
        //my_puts(passwd);

        //All done;
        while(true);
  }
}

void trigger_high(dif_gpio_t* gpio_ref){
  dif_gpio_all_write(gpio_ref, 0x8000);
}
void trigger_low(dif_gpio_t* gpio_ref){
  dif_gpio_all_write(gpio_ref, 0x0000);
}

void led_ok(dif_gpio_t* gpio_ref){
  dif_gpio_all_write(gpio_ref, 0x3300);
}

void led_error(dif_gpio_t* gpio_ref){
  dif_gpio_all_write(gpio_ref, 0xff00);
}

