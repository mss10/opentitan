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


// Glitch function definition
#define GLITCH1

#define putch my_puts
#define uart_puts my_puts
//#define STR_IMPL_(x) #x      //argument to char
//#define putch(x) my_puts(STR_IMPL_(x))
#define getch uart_rcv_char



//#define START_CMD   0x07 // \a
#define END_CMD     0x0A // \n

static dif_gpio_t gpio;

void trigger_high(dif_gpio_t*);
void trigger_low(dif_gpio_t*);
void led_ok(dif_gpio_t*);
void led_error(dif_gpio_t*);
void all_off(dif_gpio_t*);
void led_1(dif_gpio_t *);

void glitch3(void);
void glitch2(void);
void glitch1(void);
void glitch_infinite(void);

void delay_2_ms(){
  usleep(2 * 1000);
}

void delay_200_ms(){
  usleep(200 * 1000);
}

/*char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}*/


volatile int i_c = 0;
//static size_t usb_chars_recved_total;

//////////////////////////////////////////////////
char* itoa(int val, int base){
	
	static char buf[32] = {0};
	
	int i = 30;
	
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
	
}

int strtoi(const char *s) {
  int sum = 0;
  char ch;
  char sign = *s;
  if (*s == '-' || *s == '+') s++;
  while ((ch = *s++) >= '0' && ch <= '9') {
    sum = sum * 10 - (ch - '0');
  }
  if (sign != '-') {
    sum = -sum;
  }
  return sum;
}
//////////////////////////////////////////////////

/*void putch(char c)
{
    int my_int = strtoi(&c);
    char* d_str = itoa(my_int, 10);
    uart_send_str(d_str);
}*/

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
  char rcv_char;
  while (uart_rcv_char(&rcv_char) != 0) {
    //uart_send_char(rcv_char);
    usleep(20 * 1000);
  }
  //uart_send_str("\r\n");
  //LOG_INFO("USB recieved %d characters.", usb_chars_recved_total);
  buf[0] = rcv_char;
  for (i_c = 1; i_c < 32; i_c++)
	{
	    //uart_rcv_char(&buf[i_c]);
      //delay_2_ms();
      if(uart_rcv_char(&rcv_char) == -1)
      {
        break;
      }
      else{
        buf[i_c] = rcv_char;
      }
	}
  if(buf[i_c-1] == END_CMD){
    buf[i_c-1] = '\0';
  }
  //char b[] = "";
  //char* int_ic = itoa(i_c, b);
  //my_puts(int_ic);
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
  usleep(50 * 1000);  // 50 ms*/  
  //char passwd[32] = "";
  //char correct_passwd[] = "h0px3";

  //clear_buf(passwd);

        //All done;
        //while(true);
        /*for(volatile int j=0; j < 1000000; j++)
        {
          if(j == 999999)
            {
              j = 0;
              all_off(&gpio); 
            }
        }*/

        
      uart_puts("*****Safe-o-matic 3000 Booting...\n");
      /* Uncomment this to get a HELLO message for debug */
      /*putch("h");
      putch("e");
      putch("l");
      putch("l");
      putch("o");
      putch("\n");*/

      while(true){
          #if defined(GLITCH1)
              glitch1();
          #elif defined(GLITCH2)
              glitch2();
          #elif defined(GLITCH3)
              glitch3();
          #elif defined(GLITCH_INF)
              glitch_infinite();
          #endif
      }
}

void trigger_high(dif_gpio_t* gpio_ref){
  uint32_t mask = 0xFF00;
  dif_gpio_masked_write(gpio_ref, mask, 0x8000);
}
void trigger_low(dif_gpio_t* gpio_ref){
    int32_t mask = 0xFF00;
  dif_gpio_masked_write(gpio_ref, mask, 0x1000);
}

void led_1(dif_gpio_t* gpio_ref){
  int32_t mask = 0xFF00;
  dif_gpio_masked_write(gpio_ref, mask, 0x1000);
}

void led_ok(dif_gpio_t* gpio_ref){
  dif_gpio_all_write(gpio_ref, 0x3300);
}

void led_error(dif_gpio_t* gpio_ref){
  uint32_t mask = 0xFF00;
  dif_gpio_masked_write(gpio_ref, mask, 0x4300);
  //dif_gpio_all_write(gpio_ref, 0xff00);
}

void all_off(dif_gpio_t* gpio_ref){
  dif_gpio_all_write(gpio_ref, 0x0000);
}

void glitch1(void)
{
    led_ok(&gpio);
    led_error(&gpio);
    //usleep(2000*1000);
    led_1(&gpio);
    //usleep(2000*1000);
    //Some fake variable
    volatile uint8_t a = 0;
    uart_puts("A\n");
    //uart_puts("1234");
    //External trigger logic
    trigger_high(&gpio);
    usleep(20*1000);
    trigger_low(&gpio);

    //Should be an infinite loop
    while(a != 2){
      ;
    }
    //uart_puts("1234");
    led_error(&gpio);
    led_error(&gpio);
    led_error(&gpio);
    led_error(&gpio);
    led_error(&gpio);
    led_error(&gpio);
    led_error(&gpio);
    led_error(&gpio);
    led_error(&gpio);

    uart_puts("1234");

    led_error(&gpio);
    led_error(&gpio);
    led_error(&gpio);
    led_error(&gpio);
    led_error(&gpio);
    led_error(&gpio);
    led_error(&gpio);
    led_error(&gpio);

    //Several loops in order to try and prevent restarting
    while(1){
    ;
    }
    while(1){
    ;
    }
    while(1){
    ;
    }
    while(1){
    ;
    }
    while(1){
    ;
    }
}


void glitch2(void)
{
    char c;

    putch("B");

    // c = getch();
    getch(&c);

    trigger_high(&gpio);
    trigger_low(&gpio);

    if (c != 'q'){
        putch("1");
    } else {
        putch("2");
    }
    putch("\n");
    putch("\n");
    putch("\n");
    putch("\n");
}


void glitch3(void)
{
    char inp[16];
    char c = 'A';
    unsigned char cnt = 0;
    uart_puts("Password:");

    while((c != '\n') & (cnt < 16)){
        //c = getch();
        getch(&c);
        inp[cnt] = c;
        cnt++;
    }

    char passwd[] = "touch";
    char passok = 1;

    trigger_high(&gpio);
    trigger_low(&gpio);

    //Simple test - doesn't check for too-long password!
    for(cnt = 0; cnt < 5; cnt++){
        if (inp[cnt] != passwd[cnt]){
            passok = 0;
        }
    }

    if (!passok){
        uart_puts("Denied\n");
    } else {
        uart_puts("Welcome\n");
    }
}

void glitch_infinite(void)
{
    /*char str[64];
    unsigned int k = 0;
    //Declared volatile to avoid optimizing away loop.
    //This also adds lots of SRAM access
    volatile uint16_t i, j;
    volatile uint32_t cnt;
    while(1){
        cnt = 0;
        trigger_high(&gpio);
        trigger_low(&gpio);
        for(i=0; i<200; i++){
            for(j=0; j<200; j++){
                cnt++;
            }
        }
        sprintf(str, "%lu %d %d %d\n", cnt, i, j, k++);
        uart_puts(str);
    }*/
}