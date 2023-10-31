/* main - Application main entry point */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "CycleCounter.h"
#include "algorithm.h"
#include "battery.h"
#include <zephyr/drivers/gpio.h>

/** A discharge curve specific to the power source. */
static const struct battery_level_point levels[] = {
    /* "Curve" here eyeballed from captured data for the [Adafruit
     * 3.7v 2000 mAh](https://www.adafruit.com/product/2011) LIPO
     * under full load that started with a charge of 3.96 V and
     * dropped about linearly to 3.58 V over 15 hours.  It then
     * dropped rapidly to 3.10 V over one hour, at which point it
     * stopped transmitting.
     *
     * Based on eyeball comparisons we'll say that 15/16 of life
     * goes between 3.95 and 3.55 V, and 1/16 goes between 3.55 V
     * and 3.1 V.
     */

    {10000, 3950},
    {625, 3550},
    {0, 3100},
};

LOG_MODULE_REGISTER(MAIN);

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void) {
  int ret;

  // Initialize all peripherals
  if (!device_is_ready(led.port)) {
    LOG_ERR("LED Dev not read!");
    return -1;
  }

  ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
  if (ret != 0) {
    LOG_ERR("Failed to configure LED (err %d)", ret);
    return ret;
  }

  struct algorithm_state state = {0};
  state.parameters.beta1 = -0.203;
  state.parameters.beta2 = 0.468;
  state.parameters.gamma = 0.67;

  fsm_init(&state, 11.4, 1);
  
  k_sleep(K_MSEC(1000));
  while (true) {
    init_timer();
    start_timer();
    int batt_mV = battery_sample();
    stop_timer();
    
    if (batt_mV < 0) {
      LOG_ERR("Failed to read battery voltage: %d", batt_mV);
      break;
    }

    int adc_cycles = get_cycles();
    batt_mV = batt_mV > 1263600000 ? 0 : batt_mV;
    init_timer();
    start_timer();
    unsigned int batt_pptt = battery_level_pptt(batt_mV, levels);
    float bat = batt_pptt / 100.0;
    stop_timer();

    int conversion = get_cycles();
    
    init_timer();
    //gpio_pin_set_dt(&led, 1);
    start_timer();
    int k = update_algorithm(&state, bat);
    stop_timer();
    int algorithm_cycles = get_cycles();
    //gpio_pin_set_dt(&led, 0);
    LOG_INF("%d mV (%d); %u pptt (%d); %d is k (%d)", batt_mV, adc_cycles, batt_pptt, conversion, k, algorithm_cycles);

    /* Burn battery so you can see that this works over time */
    k_sleep(K_MSEC(1000));
  }
}
