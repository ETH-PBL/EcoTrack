# Linux/Unix
TARGET:=testapp
PY_TEST:=test.py
CC=gcc
RM=rm -rf
MKDIR=mkdir -p
DIFF=diff
PS=/

CFLAGS=-Wall -Wextra -Wpedantic -g -Og -Ic_implementation
DEPFLAGS=-MMD -MP -MF $(BUILD_DIR)$(PS)$*.d

BUILD_DIR=build
SOURCES=c_implementation$(PS)algorithm.c test.c

TESTINPUT=testfiles$(PS)testinput.csv
TESTOUTPUT=testfiles$(PS)testoutput.csv
TESTGROUNDTRUTH=testfiles$(PS)testgroundtruth.csv

OBJECTS := $(SOURCES:%.c=$(BUILD_DIR)$(PS)%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

# Summary of callable targets
.PHONY: all test clean

# Default target, will be executed on `make`
all: $(TARGET)

# Test executable
test: all
	.$(PS)$(PY_TEST) -i $(TESTINPUT) -o $(TESTGROUNDTRUTH)
	$(BUILD_DIR)$(PS)$(TARGET) -i $(TESTINPUT) -o $(TESTOUTPUT)
	@$(DIFF) $(TESTOUTPUT) $(TESTGROUNDTRUTH); [ $$? -eq 0 ]
	@echo "Ground Truth from Python algorithm matches with generated output from C!"
	@$(RM) $(TESTOUTPUT) $(TESTGROUNDTRUTH)

# Cleaning build dir
clean:
	- $(RM) $(BUILD_DIR)

# Link sources to target
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $(BUILD_DIR)$(PS)$@

# Compile sources
$(BUILD_DIR)$(PS)%.o: %.c
	$(MKDIR) $(dir $@)
	$(CC) -o $@ $(CFLAGS) $(DEPFLAGS) $< -c


# Keep dependencies around, make them an explicit target:
$(DEPENDENCIES):

# Keep object files and output files:
.PRECIOUS: $(BUILD_DIR)$(PS)%.o

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),run)
include $(DEPENDENCIES)
endif
endif
