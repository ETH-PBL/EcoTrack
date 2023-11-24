from enum import Enum
import sys

DECREASE = 2  # Decrease factor for k
INCREASE = 24  # Increase factor for k
K_MIN = 24  # Minimum value for k


# class: FSM
# Enumeration representing states in the Finite State Machine (FSM).
class FSM(Enum):
    D = 0  # Decrease state
    Z = 1  # Zero state
    I = 2  # Increase state


# class Algorithm
# Class holding the algorithm state and
# the parameters (beta1, beta2 and gamma) of the adaptive sampling algorithm.
class Algorithm:
    def __init__(self, beta1, beta2, gamma, battery_capacity, battery_level):
        """Initializes the algorithm state.

        :param beta1: Parameter beta1 of the algorithm
        :param beta2: Parameter beta2 of the algorithm
        :param gamma: Parameter gamma of the algorithm
        :param battery_capacity: Total battery capacity in Wh
        :param battery_level: Initial battery level in interval [0,1]
        :returns: Algorithm object

        """
        # Finite State Machine State
        self.algorithm_state = FSM.Z

        # Parameters for the algorithm
        self.beta1 = beta1
        self.beta2 = beta2
        self.gamma = gamma

        # Number of sensor readouts per day
        self.k = K_MIN

        # Current battery level
        self.battery = battery_level

        # Battery capacity
        self.battery_capacity = battery_capacity

    def update_fsm(self, metric):
        """Updates the finite state machine state based on the result of the metric.

        :param metric: The result of the metric used for state transitions
        :returns: None

        """
        if self.algorithm_state == FSM.Z:
            if metric < self.beta1:
                self.algorithm_state = FSM.D
            elif metric > self.beta2:
                self.algorithm_state = FSM.I
        elif self.algorithm_state == FSM.D:
            if metric > self.beta1:
                self.algorithm_state = FSM.Z
        elif self.algorithm_state == FSM.I:
            if metric < self.beta2:
                self.algorithm_state = FSM.Z

    def update_k(self):
        """Updates the value of k based on the finite state machine state

        :returns: None

        """
        if self.algorithm_state == FSM.D:
            self.k = int((self.k + (DECREASE - 1)) / DECREASE) # proper rounding
            self.k = max(self.k, K_MIN)
        elif self.algorithm_state == FSM.I:
            self.k += INCREASE

    def update_algorithm(self, new_battery_level):
        """Updates the algorithm state based on the new battery level

        :param new_battery_level: The new battery level
        :returns: The updated value of k (number of sensor readouts per day)

        """
        metric = sys.float_info.max if new_battery_level > self.gamma else self.battery_capacity * (
            new_battery_level - self.battery) - (1 / new_battery_level - 1)
        self.battery = new_battery_level
        self.update_fsm(metric)
        self.update_k()
        return self.k
