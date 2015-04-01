/*
 * basic_motion.c
 *
 *  Created on: Mar 12, 2015
 *      Author: mrsl
 */

#include "roneos.h"
#include "ronelib.h"

void behaviorTask(void* parameters) {
	uint32 lastWakeTime = osTaskGetTickCount();

	Beh behOutput;
	while (TRUE)
	{
		/* Initialize the output behavior to inactive */
		behOutput = behInactive;

		behSetTv(&behOutput, 100);
		motorSetBeh(&behOutput);
		osTaskDelayUntil(&lastWakeTime, BEHAVIOR_TASK_PERIOD*10);
	}
}

/*
int main (void)
{
	systemInit();
	systemPrintStartup();

	behaviorSystemInit(behaviorTask, 4096);

	osTaskStartScheduler();
	//Program should never get here
	return 0;
}
*/
