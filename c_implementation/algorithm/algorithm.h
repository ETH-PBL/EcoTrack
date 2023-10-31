/**
 * @file algorithm.h
 * @brief Adaptive Sampling Algorithm for Microcontrollers based on TCP
 */

#ifndef _INC_ALGO_H
#define _INC_ALGO_H

#ifndef NULL
#define NULL 0
#endif

#define DECREASE (2)  /**< Decrease factor for k */
#define INCREASE (24) /**< Increase factor for k */
#define K_MIN 24      /**< Minimum value for k */

/**
 * @enum fsm
 * @brief Enumeration representing states in the Finite State Machine (FSM).
 */
enum fsm {
  D = 0, /**< Decrease state */
  Z = 1, /**< Zero state */
  I = 2, /**< Increase state */
};

/**
 * @struct algorithm_parameters
 * @brief Structure to hold the parameters (beta1, beta2 and gamma) of the
 * adaptive sampling algorithm.
 */
struct algorithm_parameters {
  float beta1;
  float beta2;
  float gamma;
};

/**
 * @struct algorithm_state
 * @brief Structure to hold the state of the adaptive sampling algorithm.
 */
struct algorithm_state {
  enum fsm fsm_state;                     /**< Finite State Machine State */
  struct algorithm_parameters parameters; /**< Parameters for the algorithm */
  int k;                  /**< Number of sensor readouts per day */
  float battery;          /**< Current battery level */
  float battery_capacity; /**< Battery capacity */
};

/**
 * @brief Initializes the algorithm state.
 * @param state Pointer to the algorithm state to be initialized.
 * @param battery_capacity Total battery capacity in Wh.
 * @param battery_level Initial battery level in interval [0,1].
 * @return 0 on success, -1 on failure.
 */
int fsm_init(struct algorithm_state *state, float battery_capacity,
             float battery_level);

/**
 * @brief Updates the finite state machine state based on the result of the
 * metric.
 * @param state Pointer to the algorithm state.
 * @param metric The result of the metric used for state transitions.
 */
static void update_fsm(struct algorithm_state *state, float metric);

/**
 * @brief Updates the value of k based on the finite state machine state.
 * @param state Pointer to the algorithm state.
 */
static void update_k(struct algorithm_state *state);

/**
 * @brief Updates the algorithm state based on the new battery level.
 * @param state Pointer to the algorithm state.
 * @param new_battery_level The new battery level.
 * @return The updated value of k (number of sensor readouts per day).
 */
int update_algorithm(struct algorithm_state *state, float new_battery_level);
#endif
