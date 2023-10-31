# Adaptive Sampling Algorithm

## Overview

***Energy-Aware Adaptive Sampling for Self-Sustainability in Resource-Constrained IoT Devices***

[ENSSys 2023](*Not yet published*, https://doi.org/10.1145/3628353.3628545) / [arXiv:xxxx.xxxxx](https://arxiv.org/abs/xxxx.xxxxx) (preprint)

> <div align="justify">In the ever-growing Internet of Things (IoT) landscape, smart power management algorithms combined with energy harvesting solutions are crucial to obtain self-sustainability. This paper presents an energy-aware adaptive sampling rate algorithm designed for embedded deployment in resource-constrained, battery-powered IoT devices. The algorithm, based on a finite state machine (FSM) and inspired by Transmission Control Protocol (TCP) Reno’s additive increase and multiplicative decrease, maximizes sensor sampling rates, ensuring power self-sustainability without risking battery depletion. ... . The results, validated on data from three different European cities, show that the proposed algorithm enables self-sustainability while maximizing sampled locations per day. In experiments conducted with a 3000 mAh battery capacity, the algorithm consistently maintained a minimum of 24 localizations per day and achieved peaks of up to 3000.</div>

Please cite as:

```bibtex
@inproceedings{giordano23energyaware,
author = {Giordano, Marco and Cortesi, Silvano and Mekikis, Prodromos-Vasileios and Crabolu, Michele and Bellusci, Giovanni and Magno, Michele},
title = {Energy-Aware Adaptive Sampling for Self-Sustainability in Resource-Constrained IoT Devices},
year = {2023},
isbn = {979-8-4007-0438-3/23/11},
publisher = {Association for Computing Machinery},
address = {New York, NY, USA},
url = {https://doi.org/10.1145/3628353.3628545},
doi = {10.1145/3628353.3628545},
booktitle = {Proceedings of the 11th International Workshop on Energy Harvesting and Energy-Neutral Sensing Systems},
numpages = {7},
keywords = {sensor network, embedded systems, low-power, sustainability,energy harvesting, adaptive sampling rate, GNSS, LTE, tracking, bluetooth low energy},
location = {Istanbul, Turkiye},
series = {ENSsys '23}
}
```

This repository contains the source code and implementation of the adaptive sampling algorithm designed for microcontrollers. The algorithm is inspired by concepts from TCP, incorporating additive increase and multiplicative decrease strategies to dynamically adjust the number of sensor readouts per day (k) based on the current battery level.

## Folder Structure

- **algorithm-opensource**
  - **embedded_c_implementation** (Folder for Zephyr RTOS implementation for nRF52833 DK)
  - **c_implementation** (Folder for C implementation of the algorithm plus test files)
    - **algorithm** (Folder containing algorithm.c and .h)
      - algorithm.c
      - algorithm.h
    - **testfiles** (Folder containing sample testinput.csv and testoutput_groundtruth.csv)
      - testinput.csv
      - testoutput_groundtruth.csv
    - test.c (Test application as described later)
    - Makefile (Makefile for the test, including `make test` to build the test application and `make clean` to clean it)

## Algorithm Description

The core algorithm is implemented in the `algorithm.c` and `algorithm.h` files. It employs a finite state machine (FSM) with states for decrease (D), zero (Z), and increase (I). The algorithm dynamically adjusts the sampling frequency (k) based on the battery level, using additive increase for the increase state and multiplicative decrease for the decrease state.

A minimal sample on how to use/initialize the algorithm is given here:

```c
#include "algorithm.h"

int main() {
    // Initialize algorithm state
    struct algorithm_state state;

    // Set algorithm parameters
    state.parameters.beta1 = -0.203;
    state.parameters.beta2 = 0.468;
    state.parameters.gamma = 0.67;

    // 11.4 is capacity of the battery in Wh, 1 is initial battery level (in interval [0.1])
    if (fsm_init(&state, 11.4, 1) != 0) {
        // Initialization failed
        // Handle the error accordingly
        return 1;
    }

    // Now the algorithm is initialized and ready to use

    // Example usage of update_algorithm
    float new_battery_level = 10.0;
    int k = update_algorithm(&state, new_battery_level);

    // Print the result
    printf("Battery Level: %.2f, Updated k: %d\n", new_battery_level, k);

    return 0;
}
```

## Test Application

The `test.c` file serves as a test application for the algorithm. It reads input data from a CSV file (`testinput.csv`), updates the algorithm state for each input, and outputs the corresponding k values to an output CSV file (`testoutput_groundtruth.csv`). The test application is designed to run on Linux.

## Usage Instructions

1. **Compile the Test Application**

   Run the following command in the `c_implementation` folder:

   ```bash
   make test
   ```

2. **Run the Test Application**

   Execute the compiled test application with the following command from within the `c_implementation` folder:

   ```bash
   ./test -i <input_filepath> [-o <output_filepath>]
   ```

   - `<input_filepath>`: Path to the input CSV file containing battery level data.
   - `<output_filepath>` (optional): Path to the output CSV file for storing results. If not provided, results will be printed to stdout.

3. **Example Usage**

   ```bash
   ./test -i testfiles/testinput.csv -o testfiles/testoutput.csv
   ```

   This command reads battery level data from `testinput.csv`, updates the algorithm, and outputs the results to `testoutput.csv`.

## Note

- The test application is designed to run on Linux due to its usage of getopt and file operations specific to the Linux environment.

Feel free to explore and adapt the algorithm for your specific use case!