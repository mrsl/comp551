/*
 * led_tasks.c
 *
 *  Created on: February 22, 2015
 *
 *		This is a test of the LEDs, both the heartbeat and the RGB indicators.
 */

#include "roneos.h"
#include "ronelib.h"

void blink(void);
void buttonBlink(void);
void buttonColors(void* parameters);
void testLedBrightness(void);

/*
int main (void)
{
	systemInit();
	systemPrintStartup();
	buttonsInit();

	//blink();
	//buttonBlink();
	behaviorSystemInit(buttonColors, 4096);
	//testLedBrightness();

	osTaskStartScheduler();
	//Program should never get here
	return 0;
}
*/

//This is a basic Hello World test of the blinky Heart beat LED.
void blink (void){
	int c;
	for(c=0; c < 5; ++c)
	{
		blinky_led_flash(10000);
	}

}

// This tests the buttons. Any time one of the buttons is pressed the heart beat LED should flash
void buttonBlink (void){
	uint8 buttonRedOld = 0;
	uint8 buttonGreenOld = 0;
	uint8 buttonBlueOld = 0;
	uint8 buttonRed, buttonGreen, buttonBlue;
	while(1)
	{
		buttonRed = buttonsGet(BUTTON_RED);
		buttonGreen = buttonsGet(BUTTON_GREEN);
		buttonBlue = buttonsGet(BUTTON_BLUE);
		if ((buttonRed & !buttonRedOld) | (buttonGreen & !buttonGreenOld) | (buttonBlue & !buttonBlueOld)){
			blinky_led_flash(30000);
		}
		buttonRedOld = buttonRed;
		buttonGreenOld = buttonGreen;
		buttonBlueOld = buttonBlue;
	}
}

// Testing the colored LEDs.
void buttonColors(void* parameters)
{
	uint32 lastWakeTime = osTaskGetTickCount();
	uint8 buttonRedOld = 0;
	uint8 buttonGreenOld = 0;
	uint8 buttonBlueOld = 0;
	uint8 buttonRed, buttonGreen, buttonBlue;

	while(TRUE)
	{
		buttonRed = buttonsGet(BUTTON_RED);
		buttonGreen = buttonsGet(BUTTON_GREEN);
		buttonBlue = buttonsGet(BUTTON_BLUE);
		if (buttonRed & !buttonRedOld)
		{
			ledsSetPattern(LED_RED, LED_PATTERN_CIRCLE, LED_BRIGHTNESS_MED, LED_RATE_MED);
		}
		else if (buttonGreen & !buttonGreenOld)
		{
			ledsSetPattern(LED_GREEN, LED_PATTERN_CIRCLE, LED_BRIGHTNESS_MED, LED_RATE_MED);
		}
		else if (buttonBlue & !buttonBlueOld)
		{
			ledsSetPattern(LED_BLUE, LED_PATTERN_CIRCLE, LED_BRIGHTNESS_MED, LED_RATE_MED);
		}
		buttonRedOld = buttonRed;
		buttonGreenOld = buttonGreen;
		buttonBlueOld = buttonBlue;

		osTaskDelayUntil(&lastWakeTime, BEHAVIOR_TASK_PERIOD*10);
	}
}

// This was a way to see if the resistor values on the board were correct.
void testLedBrightness(void){
	ledsSetPattern(LED_ALL, LED_PATTERN_ON, LED_BRIGHTNESS_HIGH, LED_RATE_MED);
}


