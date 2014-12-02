/**
 * @file: 	main.c
 * @brief:	LED test
 * @date: 	9 kwi 2014
 * @author: Michal Ksiezopolski
 *
 *
 * @verbatim
 * Copyright (c) 2014 Michal Ksiezopolski.
 * All rights reserved. This program and the
 * accompanying materials are made available
 * under the terms of the GNU Public License
 * v3.0 which accompanies this distribution,
 * and is available at
 * http://www.gnu.org/licenses/gpl.html
 * @endverbatim
 */

#include <stdio.h>
#include <string.h>

#include <led.h>
#include <timers.h>
#include <comm.h>
#include <hd44780.h>

#define SYSTICK_FREQ 1000 ///< Frequency of the SysTick set at 1kHz.
#define COMM_BAUD_RATE 115200UL ///< Baud rate for communication with PC

void softTimerCallback(void);

#define DEBUG

#ifdef DEBUG
#define print(str, args...) printf("MAIN--> "str"%s",##args,"\r")
#define println(str, args...) printf("MAIN--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif

int printfWrite(char c, FILE *stream) {
  COMM_Putc(c);
  return 0;
}

FILE mystdout = FDEV_SETUP_STREAM(printfWrite, NULL, _FDEV_SETUP_WRITE);


/**
 * @brief Main function
 * @return
 */
int main(void) {

  stdout = &mystdout;

  COMM_Init(COMM_BAUD_RATE);

  println("Starting program");

  TIMER_Init(SYSTICK_FREQ); // Initialize timer

  // Add a soft timer with callback running every 1000ms
  int8_t timerID = TIMER_AddSoftTimer(1000, softTimerCallback);
  TIMER_StartSoftTimer(timerID);

	LED_Init(LED0); // Add an LED
	LED_Init(LED1); // Add an LED
	LED_Init(LED2); // Add an LED
	LED_ChangeState(LED2, LED_ON);

	LCD_Init(); // Initialize the LCD

	while (1) {

	  TIMER_SoftTimersUpdate(); // run timers
	  LCD_Update(); // run LCD
	}
}

/**
 * @brief Callback function called on every soft timer overflow
 */
void softTimerCallback(void) {

  LED_Toggle(LED1);
  LED_Toggle(LED0);

  static uint8_t counter;

  switch(counter % 8) {
  case 0:
    LCD_Clear();
    LCD_Puts("Start...");
    break;
  case 1:
    LCD_Position(3,1);
    break;
  case 2:
    LCD_Putc('1');
    break;
  case 3:
    LCD_ShifDisplay(4,0);
    break;
  case 4:
    LCD_Position(6, 0);
    break;
  case 5:
    LCD_Putc('a');
    break;
  case 6:
    LCD_Clear();
    break;
  case 7:
    LCD_Puts("Finished test!!!");
    break;
  }

  counter++;

}
