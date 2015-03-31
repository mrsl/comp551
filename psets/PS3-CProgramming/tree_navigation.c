#include "roneos.h"
#include "ronelib.h"

// Resources -rprintf, Game->robotstuff / joystickrobot, MRMTreeConverge

// Behavior Reference
// Beh struct - tv, rv, active
// motorSetBeh(&behOutput) - Move robot based on beh struct
// behMoveForward
// behMoveFromNbr
// behMoveToNbr
// behIRObstacleAvoid_ExcludeRobots
// behBumpAvoid
// behSubsume - decide output beh based on whether the high priority beh is active


// System Reference
// roneID - system variable

// IR Communications
// irCommsSetXmitPower

// Neighbors Reference
// neighborsGetMutex / neighborsPutMutex

// NbrData - Send data during each neighbor round
// nbrDataCreate(NbrDataPtr, string name, size, value)
// nbrDataSet(NbrDataPtr, value)
// value = nbrDataGet(NbrDataPtr)

// NbrList - Array of Neighbor data
// nbrListCreate(nbrListPtr)
// nbrListClear(nbrListPtr)
// nbrListPrint(nbrListPtr, string name)
// nbrListGetSize(nbrListPtr)
// nbrListGetNbr(nbrListPtr, index)
// nbrListFindSource(nbrListPtr, broadcastMsgPtr) - find the nbr that is the source of the broadcast
// nbrListPrintHops(nbrListPtr, broadcastMsgPtr, string name) - print nbrs / nbr hop count

// Broadcast
// BroadcastMessage struct
// broadcastMsgCreate(broadcastMsgPtr, MAX_HOPS) - create broadcast message
// broadcastMsgIsSource - check if this robot is the source of the broadcast message
// broadcastMsgUpdateLeaderElection - Select leader by changing broadcast message to the robot with the lowest id and hop count
// broadcastMsgUpdate - Update broadcast message; Allows for multiple sources but no leader election
// broadcastMsgUpdateNbrData -  Update local neighbor data with the incoming data from the sender of broadcast message

// TestRobotStuff.c
// teamCount - each joystick controls a group of robots
// nbrDataTeam - which team the robot is on, otherwise TEAM_NONE
// nbrDataMode[TEAM_COUNT] - which mode each team is currently using?
// TEAM_NONE - # of teams
// nbrListFindNbrsWithDataEqual - Find nbrs with the same data (nbrDataTeam)

#define MAX_HOPS 10
#define MOTION_TV 85

void led_hop_count(int hop_count, boolean root)
{
	ledsClear(LED_ALL);
	switch(hop_count)
	{
		case 1:
		{
			ledsSetPattern(LED_ALL, LED_PATTERN_CIRCLE, LED_BRIGHTNESS_MED, LED_RATE_MED);
			ledsClear(LED_RED);
			ledsClear(LED_GREEN);
		}
		break;
		case 2:
		{
			ledsSetPattern(LED_ALL, LED_PATTERN_CIRCLE, LED_BRIGHTNESS_MED, LED_RATE_MED);
			ledsClear(LED_RED);
			ledsClear(LED_BLUE);
		}
		break;
		case 3:
		{
			ledsSetPattern(LED_ALL, LED_PATTERN_CIRCLE, LED_BRIGHTNESS_MED, LED_RATE_MED);
			ledsClear(LED_RED);
		}
		break;
		case 4:
		{
			ledsSetPattern(LED_ALL, LED_PATTERN_CIRCLE, LED_BRIGHTNESS_MED, LED_RATE_MED);
			ledsClear(LED_GREEN);
			ledsClear(LED_BLUE);
		}
		break;
		case 5:
		{
			ledsSetPattern(LED_ALL, LED_PATTERN_CIRCLE, LED_BRIGHTNESS_MED, LED_RATE_MED);
			ledsClear(LED_GREEN);
		}
		break;
		case 6:
		{
			ledsSetPattern(LED_ALL, LED_PATTERN_CIRCLE, LED_BRIGHTNESS_MED, LED_RATE_MED);
			ledsClear(LED_BLUE);
		}
		break;
		default:
		{
			ledsSetPattern(LED_ALL, LED_PATTERN_BLINK, LED_BRIGHTNESS_MED, LED_RATE_MED);
		}
		break;
	}

	if(root)
	{
		cprintf("root node: %d\n", roneID);
		ledsSetPattern(LED_ALL, LED_PATTERN_CIRCLE, LED_BRIGHTNESS_MED, LED_RATE_MED);
	}
}

// Created Self Stabilizing Tree
void self_stabilizing_tree(void* parameters)
{
	uint32 lastWakeTime = osTaskGetTickCount();
	uint32 neighborRound;
	boolean printNbrs;
	NbrList nbrList;
	BroadcastMessage broadcastMsg;
	Beh behOutput;
	boolean nav = FALSE;

	// Setup Commands
	neighborsInit(300);
	radioCommandSetSubnet(2);
	irCommsSetXmitPower(IR_COMMS_POWER_MAX * 0.5);

	broadcastMsgCreate(&broadcastMsg, MAX_HOPS);

	while(TRUE)
	{
		/* Initialize the output behavior to inactive */
		behOutput = behInactive;

		neighborsGetMutex();
		printNbrs = neighborsNewRoundCheck(&neighborRound);

		nbrListCreate(&nbrList);
		if (printNbrs)
		{
			// Print Neighbor IDs
			nbrListPrint(&nbrList, "nbrs");
			broadcastMsgUpdateLeaderElection(&broadcastMsg, &nbrList);

			// Set LED pattern based on neighbor hop count
			led_hop_count(broadcastMsgGetHops(&broadcastMsg), broadcastMsgIsSource(&broadcastMsg));
			nbrListPrintHops(&nbrList, &broadcastMsg, "hop_count");
		}

		// Navigate to the root node by traveling along the tree when the blue button is pressed
		uint8 buttonBlue = buttonsGet(BUTTON_BLUE);
		if(buttonBlue)
		{
			nav = TRUE;
		}

		if(nav)
		{
			NbrList parentNbrList;
			nbrListCreate(&parentNbrList);
			nbrListGetParents(&parentNbrList, &nbrList, &broadcastMsg);

			uint8 rootID = broadcastMsgGetSourceID(&broadcastMsg);
			uint8 size = nbrListGetSize(&parentNbrList);
			uint8 index = 0;
			uint8 i;
			for(i = 0; i < size; ++i)
			{
				if(parentNbrList.nbrs[i]->ID == rootID)
				{
					index = i;
				}
			}

			if(size > 0)
			{
				behMoveToNbr(&behOutput, parentNbrList.nbrs[index] , MOTION_TV);
				if(index != 0)
				{
					nav = FALSE;
				}
			}
		}
		neighborsPutMutex();

		motorSetBeh(&behOutput);
		osTaskDelayUntil(&lastWakeTime, BEHAVIOR_TASK_PERIOD);
	}
}

int main (void)
{
	systemInit();
	systemPrintStartup();

	behaviorSystemInit(self_stabilizing_tree, 4096);

	osTaskStartScheduler();
	//Program should never get here
	return 0;
}
