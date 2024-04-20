/*
******************************************************************************
	application.h
	VCU1200_Base
	Erik Stafl
	1/23/2015

	Written for Tiva TM4C123BH6PGE
	Language: C++

	Copyright (c) 2013-2016 Stafl Systems, LLC.  All rights reserved.
******************************************************************************
*/
#ifndef APPLICATION_H_
#define APPLICATION_H_

//#include "Devices/vcu1200_board.h"
#include "CAN Devices/sme_acx1_inverter.h"
#include "CAN Devices/bms1000m.h"
#include "functional_io.h"
#include "Library/fault_manager.h"

enum TestState {
    STARTUP = 0,
    REST,
    ACTIVE,
    MOTOR_FAULT,
    SYSTEM_FAULT



};

class Application : public CANReceiver
{
public:
	// Default Constructor
	Application();

	// Initialize Application
	void initialize();

    // Peform Vehicle State Change
	void changeState(TestState new_state);

    // Check Faults Function
	void checkFaults();

    // Determine if Vehicle State Changes are Necessary
	void processState();

    // Initialize Faults
    void initializeFaults();

    // Send Vehicle Level CAN Data
    void sendCANData();

    // Send Simple can Messages
    void sendSimpleCAN(uint32_t messageID, uint8_t messageLength, uint64_t data);

    void initializeCANReceive();

    // Manage Display Updates? Not sure
    void manageConsole();

    // Process Received CAN Message
    bool receiveCANMessage(CANPort can_port, tCANMsgObject* message, unsigned int mailbox);

	// Perform Regular Processing
	void tick();

	// TODO: remove
	unsigned short debug_counter;

	// TODO: remove
	int LEDcount;

	// TODO: remove
	bool button_flag;



private:

	//calc F
    void FarenheitTemp();
    // FROM CORVETTE  //Get Temperature from Measured Thermistor Resistance
    float getTemperatureFromResistance(float resistance, float resistance_25c, float b_value);

    //TODO: DEFINE ALL OF THESE VARIABLES  WITH COMMENTS AND UNITS
    float sensor_therm1;

    int thermistor_counter;

    // The Current Vehicle Speed
    float vehicle_speed;            // mph

    //Calculate Steering System Encoder
    void calcSSE();

    float steering_command;             //%

    //Calculate Brake System Encoder %
    void calcBSE();

    float brake_pedal_command;          // %

    // Calculate Thorttle Position and check for 10% Fault
    void calcAPPS();

    // The Current Accelerator Position
    float accelerator_command;         // %

    // Perform Inverter Processing
    void manageInverters();

    // Calculate Torque Command
    void calculateTorqueCommand();

    // Return System Motor Speed
    float getMotorSpeed();

    // Total Torque Command
    float system_torque_command;    // Nm
    float torque_command_FR;        // Nm
    float torque_command_FL;        // Nm
    float torque_command_RR;        // Nm
    float torque_command_RL;        // Nm

    // Drive Mode Limits
    float max_inverter_power;       // kW
    float max_inverter_torque;      // Nm
    float off_pedal_regen_torque;   // Nm

    // BMS Object
    BMS1000M bms;

    // Manage BMS State Machine
    void manageBMS();

    //SME Inverter Objects
	ACX1Inverter inverter_1;

    // Fault Manager Object
    FaultManager faults;

    // True if a New Fault has been Detected
    bool imminent_fault;

    // How many ticks since Fault has been Active
    unsigned int fault_counter;

	TestState systemState;

	void testFunction();


};

#endif /* APPLICATION_H_ */
