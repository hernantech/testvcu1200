/*
******************************************************************************
	Functional IO Mapping Implementation
	eHarley Project
	Erik Stafl
	1/23/2015

	Written for Tiva TM4C123BH6PGE
	Language: C++

	Copyright (c) 2013-2016 Stafl Systems, LLC.  All rights reserved.
******************************************************************************
*/

#include "functional_io.h"

// Reference to Main Board Object in vcu.cpp
extern VCU1200_Board board;

// Input Polarity Settings
SignalPolarity input_polarities[NUM_INPUT_SIGNALS];

// Local Storage of Input Signal States (for Secondary VCU Inputs)
SignalState input_states[NUM_INPUT_SIGNALS];

// Local Storage of Output Signal States (for Secondary VCU Outputs)
SignalState output_states[NUM_OUTPUT_SIGNALS];

SignalState getInput(InputSignal input)
{
    /*
	switch (input)
	{
	case BUTTON1:
		//return (board.getAnalogValue(VCU1200_Board::THERMISTOR_1) > 10000.0f ? ON : OFF);
	    return (SignalState)(!board.getInput(mapInput(input)));

    case BUTTON2:
        return (SignalState)(!board.getInput(mapInput(input)));

    case BUTTON3:
        return (SignalState)(!board.getInput(mapInput(input)));

    case BUTTON4:
        return (SignalState)(!board.getInput(mapInput(input)));

    case BUTTON5:
        return (SignalState)(!board.getInput(mapInput(input)));

    case BUTTON6:
        return (SignalState)(!board.getInput(mapInput(input)));
	}
    */

	// Continue to Normal Digital Inputs

	if (input_polarities[input] == ACTIVE_LOW)
	{
		// Invert Raw Pin State
		return (SignalState)(!board.getInput(mapInput(input)));
	}
	else
	{
		// Use Raw Pin State
		return (SignalState)(board.getInput(mapInput(input)));
	}
}

// Return the Raw Voltage of the Analog Input
float getAnalogInput(AnalogSignal input)
{
	switch (input)
	{
	case BSE1:
	    return board.getAnalogValue(VCU1200_Board::ANALOG_INPUT_2);

	case BSE2:
	    return board.getAnalogValue(VCU1200_Board::ANALOG_INPUT_5);

	case APPS1:
	    return board.getAnalogValue(VCU1200_Board::ANALOG_INPUT_3);

	case APPS2:
	    return board.getAnalogValue(VCU1200_Board::ANALOG_INPUT_4);

	case SSE:
	    return board.getAnalogValue(VCU1200_Board::ANALOG_INPUT_1);

	case THM_1:
	    return board.getAnalogValue(VCU1200_Board::THERMISTOR_1);

	case THM_2:
	    return board.getAnalogValue(VCU1200_Board::THERMISTOR_2);

	case THM_3:
	    return board.getAnalogValue(VCU1200_Board::THERMISTOR_3);

	case THM_4:
	    return board.getAnalogValue(VCU1200_Board::THERMISTOR_4);

	default:
		// Program should never get here
		error("Unimplemented Input in getAnalogInput() call", "functional_io.cpp", __LINE__);
		return 0.0f;
	}
}

SignalState getOutput(OutputSignal output)
{
	// Get State of Corresponding Output Pin on Board
	return (SignalState)board.getOutput(mapOutput(output));
}

void initializeInputs()
{
    board.setPinResistor(mapInput(PC_STATUS),   VCU1200_Board::PULL_DOWN);
    setInputPolarity(PC_STATUS,                 ACTIVE_HIGH);

    board.setPinResistor(mapInput(SDC_ACTIVE),  VCU1200_Board::PULL_DOWN);
    setInputPolarity(SDC_ACTIVE,                ACTIVE_HIGH);

    board.setPinResistor(mapInput(ALL_OK),      VCU1200_Board::PULL_DOWN);
    setInputPolarity(ALL_OK,                    ACTIVE_HIGH);

    board.setPinResistor(mapInput(IMD_OK),      VCU1200_Board::PULL_DOWN);
    setInputPolarity(IMD_OK,                    ACTIVE_HIGH);

    board.setPinResistor(mapInput(BMS_OK),      VCU1200_Board::PULL_DOWN);
    setInputPolarity(BMS_OK,                    ACTIVE_HIGH);

    board.setPinResistor(mapInput(BSPD_OK),     VCU1200_Board::PULL_DOWN);
    setInputPolarity(BSPD_OK,                   ACTIVE_HIGH);
	// Set Pins to to Pull Up Resistor, but still Active High
    //board.setPinResistor(mapInput(BUTTON1),    VCU1200_Board::PULL_UP);
    //setInputPolarity(BUTTON1,                       ACTIVE_HIGH);
//	board.setPinResistor(mapInput(E_STOP), 			VCU1200_Board::PULL_UP);
//	setInputPolarity(E_STOP, 						ACTIVE_HIGH);
//	board.setPinResistor(mapInput(X_POS_LIMIT), 	VCU1200_Board::PULL_UP);
//	setInputPolarity(X_POS_LIMIT, 					ACTIVE_HIGH);
//	board.setPinResistor(mapInput(X_NEG_LIMIT), 	VCU1200_Board::PULL_UP);
//	setInputPolarity(X_NEG_LIMIT, 					ACTIVE_HIGH);
//	board.setPinResistor(mapInput(Y_POS_LIMIT), 	VCU1200_Board::PULL_UP);
//	setInputPolarity(Y_POS_LIMIT, 					ACTIVE_HIGH);
//	board.setPinResistor(mapInput(Y_NEG_LIMIT), 	VCU1200_Board::PULL_UP);
//	setInputPolarity(Y_NEG_LIMIT, 					ACTIVE_HIGH);
//
//	// Set Pins to Active Low (Pull Up Resistor)
//	board.setPinResistor(mapInput(WELDER_GOOD), 	VCU1200_Board::PULL_UP);
//	setInputPolarity(WELDER_GOOD,					ACTIVE_LOW);
//	board.setPinResistor(mapInput(WELDER_NG), 		VCU1200_Board::PULL_UP);
//	setInputPolarity(WELDER_NG,						ACTIVE_LOW);
//	board.setPinResistor(mapInput(WELDER_END), 		VCU1200_Board::PULL_UP);
//	setInputPolarity(WELDER_END,					ACTIVE_LOW);
//	board.setPinResistor(mapInput(WELDER_CAUTION),	VCU1200_Board::PULL_UP);
//	setInputPolarity(WELDER_CAUTION,				ACTIVE_LOW);
//	board.setPinResistor(mapInput(WELDER_COUNT_UP),	VCU1200_Board::PULL_UP);
//	setInputPolarity(WELDER_COUNT_UP,				ACTIVE_LOW);
//	board.setPinResistor(mapInput(WELDER_READY),	VCU1200_Board::PULL_UP);
//	setInputPolarity(WELDER_READY,					ACTIVE_LOW);
//	board.setPinResistor(mapInput(START_BUTTONS),	VCU1200_Board::PULL_UP);
//	setInputPolarity(START_BUTTONS,					ACTIVE_LOW);
}

VCU1200_Board::Input mapInput(InputSignal input)
{
	switch (input)
	{
	case PC_STATUS:
		return VCU1200_Board::DIGITAL_INPUT_12;

	case SDC_ACTIVE:
	    return VCU1200_Board::DIGITAL_INPUT_11;

	case ALL_OK:
	    return VCU1200_Board::DIGITAL_INPUT_10;

	case IMD_OK:
	    return VCU1200_Board::DIGITAL_INPUT_9;

	case BMS_OK:
	    return VCU1200_Board::DIGITAL_INPUT_8;

	case BSPD_OK:
	    return VCU1200_Board::DIGITAL_INPUT_7;

	default:
		// Program should never get here
		error("Unimplemented Input Signal in mapInput() call", "functional_io.cpp", __LINE__);
		return VCU1200_Board::DIGITAL_INPUT_1;
	}
}

VCU1200_Board::Output mapOutput(OutputSignal output)
{
	switch (output)
	{
	case VCU_OK:
		return VCU1200_Board::HIGH_SIDE_DRIVER_1;

	case COOL_FAN_MOTOR:
	    return VCU1200_Board::RELAY_DRIVER_1;

	case COOL_FAN_ACCUM:
	    return VCU1200_Board::RELAY_DRIVER_2;

	case DASH_STATUS:
	    return VCU1200_Board::HIGH_SIDE_DRIVER_2;

	case POTENTIOMETER_5V:
	    return VCU1200_Board::ANALOG_5V_OUTPUT_2;

	default:
		// Program should never get here
		error("Unimplemented Output Signal in mapOutput() call", "functional_io.cpp", __LINE__);
		return VCU1200_Board::RELAY_DRIVER_1;
	}
}

void setInputPolarity(InputSignal input, SignalPolarity polarity)
{
	input_polarities[input] = polarity;
}

void setOutput(OutputSignal output, SignalState out_state)
{
	// Set Corresponding Pin on Board
	board.setOutput(mapOutput(output), (PinState)out_state);
}

