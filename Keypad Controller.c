/******************************************************************************/
/*                                                                            */
/* PmodKYPD.c -- Demo for the use of the Pmod Keypad IP core                  */
/*                                                                            */
/******************************************************************************/
/* Author:   Mikel Skreen                                                     */
/* Copyright 2016, Digilent Inc.                                              */
/******************************************************************************/
/* File Description:                                                          */
/*                                                                            */
/* This demo continuously captures keypad data and prints a message to an     */
/* attached serial terminal whenever a positive edge is detected on any of    */
/* the sixteen keys. In order to receive messages, a serial terminal          */
/* application on your PC should be connected to the appropriate COM port for */
/* the micro-USB cable connection to your board's USBUART port. The terminal  */
/* should be configured with 8-bit data, no parity bit, 1 stop bit, and the   */
/* the appropriate Baud rate for your application. If you are using a Zynq    */
/* board, use a baud rate of 115200, if you are using a MicroBlaze system,    */
/* use the Baud rate specified in the AXI UARTLITE IP, typically 115200 or    */
/* 9600 Baud.                                                                 */
/*                                                                            */
/******************************************************************************/
/* Revision History:                                                          */
/*                                                                            */
/*    06/08/2016(MikelS):   Created                                           */
/*    08/17/2017(artvvb):   Validated for Vivado 2015.4                       */
/*    08/30/2017(artvvb):   Validated for Vivado 2016.4                       */
/*                          Added Multiple keypress error detection           */
/*    01/27/2018(atangzwj): Validated for Vivado 2017.4                       */
/*                                                                            */
/******************************************************************************/

#include "PmodKYPD.h"
#include "sleep.h"
#include "xil_cache.h"
#include "xparameters.h"

void DemoInitialize();
void DemoRun();
void DemoCleanup();
void DisableCaches();
void EnableCaches();
void DemoSleep(u32 millis);

PmodKYPD myDevice;

int main(void) {
   DemoInitialize();
   DemoRun();
   DemoCleanup();
   return 0;
}

// keytable is determined as follows (indices shown in Keypad position below)
// 12 13 14 15
// 8  9  10 11
// 4  5  6  7
// 0  1  2  3
#define DEFAULT_KEYTABLE "0FED789C456B123A"

void DemoInitialize() {
   EnableCaches();
   KYPD_begin(&myDevice, XPAR_PMODKYPD_0_AXI_LITE_GPIO_BASEADDR);
   KYPD_loadKeyTable(&myDevice, (u8*) DEFAULT_KEYTABLE);
}

void DemoRun() {
	u16 keystate;
	   XStatus status, last_status = KYPD_NO_KEY;
	   u8 key, last_key = 'x';
	   // Initial value of last_key cannot be contained in loaded KEYTABLE string

	   Xil_Out32(myDevice.GPIO_addr, 0xF);

   int buffer[4];
   xil_printf("Attack\r\n");
   while (1) {

	   for (int i = 0; i<4; i++)
	   {


      // Capture state of each key
      keystate = KYPD_getKeyStates(&myDevice);

      // Determine which single key is pressed, if any
      status = KYPD_getKeyPressed(&myDevice, keystate, &key);

      // Print key detect if a new key is pressed or if status has changed
      if (status == KYPD_SINGLE_KEY
                  && (status != last_status || key != last_key)) {
          	  int button = 0;
          	   button = (int) key - 55;
          	  if (i == 3 && button == 10)
          	  {
          		  if (buffer[0] == 2 && buffer[1] == 3 && buffer[2] == 6)
				  {
          			xil_printf("Hadoken\r\n");
				  }
          		  else if (buffer[0] == 6 && buffer[1] == 2 && buffer[2] == 3)
				  {
          			xil_printf("Shoryuken\r\n");
				  }
          		  else
          		  {
          			xil_printf("Punch\r\n");
          		  }
          		for (int j = 0; j<4; j++)
          		    {
          		       buffer[i] = 0;
          		    }
          		i = -1;
          	  }
          	  else if (i == 3 && button == 11)
			  {
          		if (buffer[0] == 2 && buffer[1] == 1 && buffer[2] == 4)
				{
          			 xil_printf("Tatsumaki Senpukyaku\r\n");
				}
          		else
          		{
          			xil_printf("Kick\r\n");
          		}
          		for (int j = 0; j<4; j++)
          		    {
          		    	buffer[i] = 0;
          		    }
          		i = -1;
			  }
          	  else
          	  {
          	  if ((int) key <= 57)
          	  {
          		  button = (int) key - 48;
          	  buffer[i] = button;
          	  }
          	  else
          	      	  {

          	      		  button = (int) key - 55;
          	      		  if (button == 10)
          	      		  {
          	      			xil_printf("Punch\r\n");
          	      		  }
          	      		  else if (button == 11)
          	      		  {
          	      			xil_printf("Kick\r\n");
          	      		  }
          	      		  else
          	      		  {
          	      			xil_printf("Not a valid input\r\n");
          	      		  }
          	      		for (int j = 0; j<4; j++)
          	      		{
          	      			buffer[i] = 0;
          	      		}
          	      		i = -1;
          	      	  }

               last_key = key;
          	  }} else if (status == KYPD_MULTI_KEY && status != last_status)
          	  {
               xil_printf("Error: Multiple keys pressed\r\n");
          	  }
            last_status = status;

            usleep(1000);
         }
	   }
	   }




void DemoCleanup() {
   DisableCaches();
}

void EnableCaches() {
#ifdef __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_ICACHE
   Xil_ICacheEnable();
#endif
#ifdef XPAR_MICROBLAZE_USE_DCACHE
   Xil_DCacheEnable();
#endif
#endif
}

void DisableCaches() {
#ifdef __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_DCACHE
   Xil_DCacheDisable();
#endif
#ifdef XPAR_MICROBLAZE_USE_ICACHE
   Xil_ICacheDisable();
#endif
#endif
}
