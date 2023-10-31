# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(algobenchmark)

target_sources(app PRIVATE
  src/main.c
  src/algorithm.c
  src/battery.c
  src/CycleCounter.c
)
