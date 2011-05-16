//*****************************************************************************
//
// idle_task.h - Prototypes for the SafeRTOS idle task.
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

#ifndef __IDLE_TASK_H__
#define __IDLE_TASK_H__

//*****************************************************************************
//
// Prototypes for the idle task.
//
//*****************************************************************************
extern unsigned long g_pulIdleTaskStack[128];
extern void TaskCreated(void);
extern void SafeRTOSTaskDeleteHook(xTaskHandle xTaskToDelete);
extern void SafeRTOSIdleHook(void);

#endif // __IDLE_TASK_H__
