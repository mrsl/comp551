/*
 * led_tasks.c
 *
 *  Created on: February 22, 2015
 *
 *	This is a test of the LEDs, both the heartbeat and the RGB indicators.
 */

#include "roneos.h"
#include "ronelib.h"

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

/*
int main (void)
{
	systemInit();
	systemPrintStartup();
	behaviorSystemInit(buttonColors, 4096);
	osTaskStartScheduler();
	//Program should never get here
	return 0;
}
*/
