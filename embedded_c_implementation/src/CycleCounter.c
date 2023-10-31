/*
 * ==========================================================================================================
 * @file    CycleCounter.c
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

#include "CycleCounter.h"

void init_timer() {
#ifdef CONFIG_CPU_CORTEX_M7
  *DWT_LAR = 0xC5ACCE55;
#endif
  *SCB_DEMCR |= 0x01000000;
  *DWT_CYCCNT = 0; // reset the counter
  *DWT_CONTROL &= ~1;
}

void start_timer() {
  init_timer();
  *DWT_CONTROL |= 1; // enable the counter
}

void stop_timer() {
  *DWT_CONTROL &= ~1; // disable the counter
}

uint32_t get_cycles() { return *DWT_CYCCNT; }
