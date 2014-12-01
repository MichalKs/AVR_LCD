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

#include <avr/io.h>

//void alarmSignalInit(void) {
//
//  DDRD |= (1<<3); // output pin
//  PORTD |= (1<<3); //
//
//}
//
//void setAlarm(uint8_t val) {
//
//  if (val) {
//    PORTD |= (1<<3);
//  } else {
//    PORTD &= ~(1<<3);
//  }
//
//
//}

//void initContactSwitch(void) {
//
//  DDRD &= ~(1<<2); // input pin
//  PORTD |= (1<<2); // pull up resistor
//
//}
//
//uint8_t checkContactSwitch(void) {
//
//  return !(PIND & (1<<2));
//}

uint8_t alarm;

/**
 * @brief Main function
 * @return
 */
int main(void) {

//  initContactSwitch();
//  alarmSignalInit();

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

	while (1) {

//	  alarm = checkContactSwitch();
//	  if (alarm) {
//	    setAlarm(1);
//	  } else {
//	    setAlarm(0);
//	  }

	  TIMER_SoftTimersUpdate(); // run timers
	}
}

/**
 * @brief Callback function called on every soft timer overflow
 */
void softTimerCallback(void) {

  LED_Toggle(LED1);
  LED_Toggle(LED0);

  if (!alarm)
    println("Door open");
  else {
    println("Door closed");
  }

}
