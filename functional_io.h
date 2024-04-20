/*
******************************************************************************
	Functional IO Mapping
	Lithos Energy Weld Center - Primary VCU
	Erik Stafl
	3/22/2017

	Written for Tiva TM4C123BH6PGE
	Language: C++

	Copyright (c) 2013-2017 Stafl Systems, LLC.  All rights reserved.
******************************************************************************
*/

#ifndef VCU1200_BASE_1_FUNCTIONAL_IO_H_
#define VCU1200_BASE_1_FUNCTIONAL_IO_H_

#include "Devices/vcu1200_board.h"

// Signal States
enum SignalState
{
	OFF = 0,
	ON
};

// Signal Polarity
enum SignalPolarity
{
	ACTIVE_HIGH = 0,
	ACTIVE_LOW
};

// Functional Digital Inputs
enum InputSignal
{
	// Primary VCU Input Signals
	PC_STATUS = 0,
	SDC_ACTIVE,
	ALL_OK,
	IMD_OK,
	BMS_OK,
	BSPD_OK,
	NUM_INPUT_SIGNALS
};

// Functional Analog Inputs
enum AnalogSignal
{
	BSE1 = 0,
	BSE2,
	APPS1,
	APPS2,
	SSE,
	THM_1,
	THM_2,
	THM_3,
	THM_4,
    NUM_ANALOG_SIGNALS
};

// Functional Outputs
enum OutputSignal
{
	// Primary VCU Output Signals
	VCU_OK = 0,
	COOL_FAN_MOTOR,
	COOL_FAN_ACCUM,
	DASH_STATUS,
	POTENTIOMETER_5V,
	NUM_OUTPUT_SIGNALS
};

// Return the Raw Voltage of the Analog Input
float getAnalogInput(AnalogSignal input);

// Return the Current State of the Functional Input
SignalState getInput(InputSignal input);

// Return the Current State of the Functional Output
SignalState getOutput(OutputSignal output);

// Setup Pull-up / Pull-down Resistors on Inputs
void initializeInputs();

// Return the Corresponding Digital Input Pin for the Given Input Signal
VCU1200_Board::Input mapInput(InputSignal input);

// Return the Corresponding Output Pin for the Given Output Signal
VCU1200_Board::Output mapOutput(OutputSignal output);

// Set Logical Signal Polarity of an Input Pin (Active High or Active Low)
void setInputPolarity(InputSignal input, SignalPolarity polarity);

// Handles Turning On or Off Outputs
void setOutput(OutputSignal output, SignalState out_state);

#endif /* VCU1200_BASE_1_FUNCTIONAL_IO_H_ */
