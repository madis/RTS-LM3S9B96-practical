#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "SafeRTOS/SafeRTOS_API.h"
#include "example_task.h"
#include "idle_task.h"
#include "priorities.h"

static unsigned long g_pulExampleTaskStack[128];

static void ExampleTask(void *pvParameters)
{
	while(1)
	{
		// This task does nothing
	}
}

unsigned long ExampleTaskInit(void)
{
	// Create ExampleTask
	if(xTaskCreate(ExampleTask, (signed portCHAR *)"ExampleTask",
		(signed portCHAR *)g_pulExampleTaskStack,
		sizeof(g_pulExampleTaskStack),
		NULL,
		PRIORITY_EXAMPLE_TASK,
		NULL) != pdPASS)
	{
		return(1);
	}
	TaskCreated();
	
	return(0);
}