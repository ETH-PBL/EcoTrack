/*
 * ==========================================================================================================
 * @file    CycleCounter.h
 * @authors Silvano Cortesi
 * @copyright  "(c) 2020. ETH Zurich.
 * @date    05-May-2020
 *
 * Copyright   : (c) 2020.  Silvano Cortesi & Stephan Boner, ETH Zurich. All
 * rights reserved
 *
 * Description : functions to measure cycles.
 *
 * ============================================================================================================
 */

#ifndef CycleCounter_H
#define CycleCounter_H

#include <stdint.h>

#ifdef CONFIG_CPU_CORTEX_M7
static unsigned int *DWT_LAR = (unsigned int *)0xE0001FB0;
#endif

static unsigned int *DWT_CYCCNT =
    (unsigned int *)0xE0001004; // address of the register
static unsigned int *DWT_CONTROL =
    (unsigned int *)0xE0001000; // address of the register
static unsigned int *SCB_DEMCR =
    (unsigned int *)0xE000EDFC; // address of the register

/**
 * Function to compute and return the measured time
 */
void init_timer();

/**
 * Function to start the timer
 */
void start_timer();

/**
 * Function to stop the timer
 */
void stop_timer();

/**
 * Function to compute and return the measured time
 *
 * @return Passed cycles
 */
uint32_t get_cycles();

#endif /* CycleCounter_H */
