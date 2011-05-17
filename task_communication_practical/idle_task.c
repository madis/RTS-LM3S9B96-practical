//*****************************************************************************
//
// idle_task.c - The SafeRTOS idle task.
//
// Copyright (c) 2009-2010 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 5961 of the DK-LM3S9B96 Firmware Package.
//
//*****************************************************************************

#include "SafeRTOS/SafeRTOS_API.h"
#include "display_task.h"
#include "idle_task.h"

//*****************************************************************************
//
// The stack for the idle task.
//
//*****************************************************************************
unsigned long g_pulIdleTaskStack[128];

//*****************************************************************************
//
// The number of tasks that are running.
//
//*****************************************************************************
static unsigned long g_ulTasks;

//*****************************************************************************
//
// The number of tasks that existed the last time the display was updated (used
// to detect when the display should be updated again).
//
//*****************************************************************************
static unsigned long g_ulPreviousTasks;

//*****************************************************************************
//
// The number of seconds that the application has been running.  This is
// initialized to -1 in order to get the initial display updated as soon as
// possible.
//
//*****************************************************************************
static unsigned long g_ulSeconds = 0xffffffff;

//*****************************************************************************
//
// A buffer to contain the string versions of the information displayed at the
// bottom of the display.
//
//*****************************************************************************
static char g_pcTimeString[12];
static char g_pcTaskString[4];
//*****************************************************************************
//
// This function is called by the application whenever it creates a task.
//
//*****************************************************************************
void
TaskCreated(void)
{
    //
    // Increment the count of running tasks.
    //
    g_ulTasks++;
}

//*****************************************************************************
//
// This hook is called by SafeRTOS when a task is deleted.
//
//*****************************************************************************
void
SafeRTOSTaskDeleteHook(xTaskHandle xTaskToDelete)
{
    //
    // Decrement the count of running tasks.
    //
    g_ulTasks--;
}


//*****************************************************************************
//
// Displays a value in a human-readable format.  This does not need to deal
// with leading/trailing spaces sicne the values displayed are monotonically
// increasing.
//
//*****************************************************************************
static void
DisplayValue(char *pcBuffer, unsigned long ulValue, unsigned long ulX,
             unsigned long ulY)
{
    //
    // See if the value is less than 10.
    //
    if(ulValue < 10)
    {
        //
        // Display the value using only a single digit.
        //
        pcBuffer[0] = '0' + ulValue;
        pcBuffer[1] = '\0';
        DisplayString(ulX + 15, ulY, pcBuffer);
    }

    //
    // Otherwise, see if the value is less than 100.
    //
    else if(ulValue < 100)
    {
        //
        // Display the value using two digits.
        //
        pcBuffer[0] = '0' + (ulValue / 10);
        pcBuffer[1] = '0' + (ulValue % 10);
        pcBuffer[2] = '\0';
        DisplayString(ulX + 12, ulY, pcBuffer);
    }

    //
    // Otherwise, see if the value is less than 1,000.
    //
    else if(ulValue < 1000)
    {
        //
        // Display the value using three digits.
        //
        pcBuffer[0] = '0' + (ulValue / 100);
        pcBuffer[1] = '0' + ((ulValue / 10) % 10);
        pcBuffer[2] = '0' + (ulValue % 10);
        pcBuffer[3] = '\0';
        DisplayString(ulX + 9, ulY, pcBuffer);
    }

    //
    // Otherwise, see if the value is less than 10,000.
    //
    else if(ulValue < 10000)
    {
        //
        // Display the value using four digits.
        //
        pcBuffer[0] = '0' + (ulValue / 1000);
        pcBuffer[1] = '0' + ((ulValue / 100) % 10);
        pcBuffer[2] = '0' + ((ulValue / 10) % 10);
        pcBuffer[3] = '0' + (ulValue % 10);
        pcBuffer[4] = '\0';
        DisplayString(ulX + 6, ulY, pcBuffer);
    }

    //
    // Otherwise, see if the value is less than 100,000.
    //
    else if(ulValue < 100000)
    {
        //
        // Display the value using five digits.
        //
        pcBuffer[0] = '0' + (ulValue / 10000);
        pcBuffer[1] = '0' + ((ulValue / 1000) % 10);
        pcBuffer[2] = '0' + ((ulValue / 100) % 10);
        pcBuffer[3] = '0' + ((ulValue / 10) % 10);
        pcBuffer[4] = '0' + (ulValue % 10);
        pcBuffer[5] = '\0';
        DisplayString(ulX + 3, ulY, pcBuffer);
    }

    //
    // Otherwise, the value is between 100,000 and 999,999.
    //
    else
    {
        //
        // Display the value using six digits.
        //
        pcBuffer[0] = '0' + ((ulValue / 100000) % 10);
        pcBuffer[1] = '0' + ((ulValue / 10000) % 10);
        pcBuffer[2] = '0' + ((ulValue / 1000) % 10);
        pcBuffer[3] = '0' + ((ulValue / 100) % 10);
        pcBuffer[4] = '0' + ((ulValue / 10) % 10);
        pcBuffer[5] = '0' + (ulValue % 10);
        pcBuffer[6] = '\0';
        DisplayString(ulX, ulY, pcBuffer);
    }
}

//*****************************************************************************
//
// This hook is called by the SafeRTOS idle task when no other tasks are
// runnable.
//
//*****************************************************************************
void
SafeRTOSIdleHook(void)
{
    unsigned long ulTemp;

    //
    // See if this is the first time that the idle task has been called.
    //
    if(g_ulSeconds == 0xffffffff)
    {
        //
        // Draw the boxes for the statistics that are displayed.
        //
        DisplayMove(0, 240 - 20);
        DisplayDraw(319, 240 - 20);
        DisplayDraw(319, 239);
        DisplayDraw(0, 239);
        DisplayDraw(0, 240 - 20);
        DisplayMove(64, 240 - 20);
        DisplayDraw(64, 239);
        DisplayMove(110, 240 - 20);
        DisplayDraw(110, 239);
        DisplayMove(215, 240 - 20);
        DisplayDraw(215, 239);
        DisplayMove(267, 240 - 20);
        DisplayDraw(267, 239);

        //
        // Place the statistics titles in the boxes.
        //
        DisplayString(14, 240 - 18, "Uptime");
        DisplayString(72, 240 - 18, "Tasks");
        DisplayString(133, 240 - 18, "IP Address");
        DisplayString(235, 240 - 18, "TX");
        DisplayString(287, 240 - 18, "RX");
    }

    //
    // Get the number of seconds that the application has been running.
    //
    ulTemp = xTaskGetTickCount() / (1000 / portTICK_RATE_MS);

    //
    // See if the number of seconds has changed.
    //
    if(ulTemp != g_ulSeconds)
    {
        //
        // Update the local copy of the run time.
        //
        g_ulSeconds = ulTemp;

        //
        // Convert the number of seconds into a text string.
        //
        g_pcTimeString[0] = '0' + ((ulTemp / 36000) % 10);
        g_pcTimeString[1] = '0' + ((ulTemp / 3600) % 10);
        g_pcTimeString[2] = ':';
        g_pcTimeString[3] = '0' + ((ulTemp / 600) % 6);
        g_pcTimeString[4] = '0' + ((ulTemp / 60) % 10);
        g_pcTimeString[5] = ':';
        g_pcTimeString[6] = '0' + ((ulTemp / 10) % 6);
        g_pcTimeString[7] = '0' + (ulTemp % 10);
        g_pcTimeString[8] = '\0';

        //
        // Have the display task write this string onto the display.
        //
        DisplayString(8, 240 - 10, g_pcTimeString);
    }

    //
    // See if the number of tasks has changed.
    //
    if(g_ulTasks != g_ulPreviousTasks)
    {
        //
        // Update the local copy of the number of tasks.
        //
        ulTemp = g_ulPreviousTasks = g_ulTasks;

        //
        // Convert the number of tasks into a text string and display it.
        //
        if(ulTemp < 10)
        {
            g_pcTaskString[0] = ' ';
            g_pcTaskString[1] = '0' + (ulTemp % 10);
            g_pcTaskString[2] = ' ';
            g_pcTaskString[3] = '\0';
            DisplayString(78, 240 - 10, g_pcTaskString);
        }
        else
        {
            g_pcTaskString[0] = '0' + ((ulTemp / 10) % 10);
            g_pcTaskString[1] = '0' + (ulTemp % 10);
            g_pcTaskString[2] = '\0';
            DisplayString(81, 240 - 10, g_pcTaskString);
        }
    }

}
