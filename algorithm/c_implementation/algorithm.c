#include "algorithm.h"
#include <float.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

#define MAX(a,b) (((a)>(b))?(a):(b))

/**
 * @brief Updates the finite state machine state based on the result of the
 * metric.
 * @param state Pointer to the algorithm state.
 * @param metric The result of the metric used for state transitions.
 */
static void update_fsm(struct algorithm_state *state, float metric) {
  switch (state->fsm_state) {
  case Z:
    if (metric < state->parameters.beta1) {
      state->fsm_state = D;
    } else if (metric > state->parameters.beta2) {
      state->fsm_state = I;
    }
    break;
  case D:
    if (metric > state->parameters.beta1) {
      state->fsm_state = Z;
    }
    break;
  case I:
    if (metric < state->parameters.beta2) {
      state->fsm_state = Z;
    }
    break;
  default:
    break;
  }
}

/**
 * @brief Updates the value of k based on the finite state machine state.
 * @param state Pointer to the algorithm state.
 */
static void update_k(struct algorithm_state *state) {
  switch (state->fsm_state) {
  case D:
    state->k = (state->k + (DECREASE - 1)) / DECREASE; // proper rounding
    state->k = MAX(state->k, K_MIN);
    break;
  case I:
    state->k = state->k + INCREASE;
    break;
  default:
    break;
  }
}

int fsm_init(struct algorithm_state *state, float battery_capacity,
             float battery_level) {
  if (state == NULL) {
    return -1;
  }

  state->fsm_state = Z;
  state->k = K_MIN;
  state->battery = battery_level;
  state->battery_capacity = battery_capacity;
  return 0;
}

int update_algorithm(struct algorithm_state *state, float new_battery_level) {
  // calculate metric
  float metric =
      (new_battery_level > state->parameters.gamma)
          ? FLT_MAX
          : state->battery_capacity * (new_battery_level - state->battery) -
                (1 / new_battery_level - 1);
  state->battery = new_battery_level;
  update_fsm(state, metric);
  update_k(state);
  return state->k;
}
