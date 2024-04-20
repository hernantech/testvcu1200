/*
******************************************************************************
	fault_conditions.h
	VCU1200_Corvette
	Erik Stafl
	1/23/2015

	Written for Tiva TM4C123BH6PGE
	Language: C++

	Copyright (c) 2013-2016 Stafl Systems, LLC.  All rights reserved.
******************************************************************************
*/
#ifndef FAULT_CONDITIONS_H_
#define FAULT_CONDITIONS_H_

// Fault/Warning Conditions
enum Condition
{
	PACK_VOLTAGE_HIGH = 0,
	PACK_VOLTAGE_LOW,
	CELL_VOLTAGE_HIGH,
	CELL_VOLTAGE_LOW,
	HIGH_PACK_DISCHARGE_CURRENT,
	HIGH_PACK_CHARGE_CURRENT,
	HIGH_TEMPERATURE,
	LOW_TEMPERATURE,
	PRECHARGE_FAULT,
	BMS_FAULT,                  // Any BMS Fault
	BMS_COMMUNICATION_FAULT,    // Loss of BMS COMS
	DISCHARGE_ENVELOPE_FAULT,
	CHARGE_ENVELOPE_FAULT,
	ISOLATION_LEAKAGE_FAULT,
	ACCELERATOR1_HIGH_FAULT,
	ACCELERATOR1_LOW_FAULT,
    ACCELERATOR2_HIGH_FAULT,
    ACCELERATOR2_LOW_FAULT,
    BSE1_HIGH_FAULT,
    BSE1_LOW_FAULT,
    INVERTER_FR_FAULT,
    INVERTER_FL_FAULT,
    INVERTER_RR_FAULT,
    INVERTER_RL_FAULT,
	MAX_FAULT_NUMBER
};

#endif /* FAULT_CONDITIONS_H_ */
