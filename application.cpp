/*
******************************************************************************
	application.cpp
	VCU1200_Base
	Erik Stafl
	1/23/2015

	Written for Tiva TM4C123BH6PGE
	Language: C++

	Copyright (c) 2013-2016 Stafl Systems, LLC.  All rights reserved.
******************************************************************************
*/

#include "application.h"
#include <stdio.h>

using namespace std;
// TODO: remove stdio after debug???

// Main Application Object
Application application;

// Reference to Board Object
extern VCU1200_Board board;

//Constructor function assigns CAN offsets on specified CAN bus for each referenced CAN object
Application::Application() : inverter_1(&board.can.can_1, 0x001), bms(&board.can.can_1, 0x300), faults(false)
{
    debug_counter = 0;
}

void Application::initialize()
{
	// Set CAN Termination Resistor Setting
	board.setCANTermination(CAN_1, CAN_1_TERMINATION);
	board.setCANTermination(CAN_2, CAN_2_TERMINATION);

    // Application CAN Reception
    initializeCANReceive();

    // Initialize Inputs for this Application
    initializeInputs();

    inverter_1.initializeCANReceive();
    bms.initializeCANReceive();

	changeState(STARTUP);

	max_inverter_power = 20; // KW To start
	max_inverter_torque = 10; //NM To start.

}

void Application::initializeFaults()
{
    // Set all CAN Faults to Immediate
    faults.setFaultTime(BMS_FAULT, 0);
    faults.setFaultTime(INVERTER_FR_FAULT, 0);
    faults.setFaultTime(INVERTER_FL_FAULT, 0);
    faults.setFaultTime(INVERTER_RR_FAULT, 0);
    faults.setFaultTime(INVERTER_RL_FAULT, 0);

    faults.setFaultTime(ACCELERATOR1_HIGH_FAULT, 500);
    faults.setFaultTime(ACCELERATOR1_LOW_FAULT,  500);
    faults.setFaultTime(ACCELERATOR2_HIGH_FAULT,  500);
    faults.setFaultTime(ACCELERATOR2_LOW_FAULT,  500);
    faults.setFaultTime(BSE1_HIGH_FAULT,  500);
    faults.setFaultTime(BSE1_LOW_FAULT,  500);

}

// Lets the CAN hardware know which addresses to be listening onA
void Application::initializeCANReceive()
{
    // Temporary Mailbox for Setup
    tCANMsgObject can_rx_message;

    // Setup Receive Messages on CAN Interface

    // Note: This is the main CAN mailbox for the application, NOT THE BOARD ITSELF
    // Board itself has some built-in CAN functions (I believe, need to verify 100%)
    // EXAMPLE Vehicle Settings Command from Display
    can_rx_message.ui32MsgID = 0x400;
    can_rx_message.ui32MsgIDMask = 0x7FF;
    can_rx_message.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    board.can.can_1.initializeReceiveMessage(this, &can_rx_message);
}

void Application::changeState(TestState new_state)
{

    // TODO: Evaluate what board level functionality will actually be needed to output (PWM, signals, Therms, etc.)
    // TODO: Obviously, figure out how those need to be integrated and coded for
    // TODO: Plan high level state machine for vehicle, driving our overall control of this function


    switch(new_state) {

    case STARTUP:
        // Default power-on state
        // Should ideally only be called once at the actual system boot
        // Thereafter, any 'soft shutdown' commands should enter the REST state
        // TODO: set flags for motor activity and enabling

        // TODO: 5v pots off

        //setOutput(POTENTIOMETER_5V, ON);


        break;

    case REST:
        // TODO: Ideally just a state for when we do not expect to use motor for a period of time
        // TODO: Safety- illuminated light to let us know that it won't be doing anything

        // TODO 5v pots on, safety checks active, indicators as desired
        // TODO

        //setOutput(POTENTIOMETER_5V, ON);
        //TODO: Clear all faults when vehicle is at rest.


        break;

    case ACTIVE:
        // Active state whenever motors are capable of delivering power!
        // TODO: Safety checks and redundancies for both testbench and production

        // TODO all sensors on, all safety checks at full chooch, warning sound initiated for a few seconds

       // setOutput(POTENTIOMETER_5V, ON);



        break;


    case MOTOR_FAULT:
        // If any controller throws a fault case, we enter this state to enter neutral
        // TODO:

        // kill all motor outputs (duh)
        //  VCU status --> LOW
        //  fault light? console outputs?

       // setOutput(POTENTIOMETER_5V, ON);

        break;

    case SYSTEM_FAULT:
        // VCU fault or other debug utilization
        // Non recoverable? e.g. force hard reset



       // setOutput(POTENTIOMETER_5V, OFF);
       break;

    default:
        // Should not enter here!

        break;

    }
}

void Application::processState() {

    switch(systemState) {

        case STARTUP:

        break;

        case REST:

        break;

        case ACTIVE:

        break;

        case MOTOR_FAULT:

        break;

        case SYSTEM_FAULT:

        break;

        default:
            // SHOULD NOT ENTER HERE!!!!
        break;
    }
}

void Application::manageConsole() {

    // temp buffer

}

bool Application::receiveCANMessage(CANPort can_port, tCANMsgObject* message, unsigned int mailbox)
{
    // True if Message has been Processed
    bool processed = false;

        // command messages from computer
        if (message->ui32MsgID == 0x400) {

                // TODO: initialize motor controller signal
                // TODO: toggle control option (speed v torque, limits, set speed or controlled)
                // TODO: initialize chargers (may be redundant)
            if (message->pui8MsgData[0] == 0x20) {
                // Motor control method
                switch (message->pui8MsgData[1]) {

                    case 0x00:
                        // disable output
                        inverter_1.enabled = false;
                        inverter_1.control_mode = ACX1Inverter::DISABLED; // unknown/disabled output mode
                        inverter_1.setDirection(ACX1Inverter::NEUTRAL);
                        inverter_1.setTorqueCommand(0.00);
                        inverter_1.setSpeedCommand(0.00);
                        inverter_1.sendCommandMessage();
                        break;

                    case 0x01:
                        // potentiometer torque control
                        inverter_1.enabled = true;
                        inverter_1.control_mode = ACX1Inverter::SPEED_SET; // torque ref, speed limited
                        inverter_1.controlMethodPoteniometer = true;
                        inverter_1.setSpeedCommand(0); // max speed under pot control
                        inverter_1.setTorqueCommand(100.0);
                        inverter_1.sendCommandMessage();
                        break;


                    case 0x02:
                        // speed controlled = 500 RPM
                        inverter_1.control_mode = ACX1Inverter::SPEED_SET;
                        inverter_1.setSpeedCommand(500);
                        inverter_1.setTorqueCommand(0x7FFF);
                        inverter_1.sendCommandMessage();
                        break;


                    case 0x04:
                        inverter_1.control_mode = ACX1Inverter::SPEED_SET;
                        inverter_1.setSpeedCommand(1000);
                        inverter_1.setTorqueCommand(0x7FFF);
                        inverter_1.sendCommandMessage();
                        break;


                    case 0x08:
                        inverter_1.control_mode = ACX1Inverter::SPEED_SET;
                        inverter_1.setSpeedCommand(1500);
                        inverter_1.setTorqueCommand(0x7FFF);
                        inverter_1.sendCommandMessage();
                        break;


                    case 0x10:
                        inverter_1.control_mode = ACX1Inverter::SPEED_SET;
                        inverter_1.setSpeedCommand(2000);
                        inverter_1.setTorqueCommand(0x7FFF);
                        inverter_1.sendCommandMessage();
                        break;


                    case 0x20:
                        inverter_1.control_mode = ACX1Inverter::SPEED_SET;
                        inverter_1.setSpeedCommand(2500);
                        inverter_1.setTorqueCommand(0x7FFF);
                        inverter_1.sendCommandMessage();
                        break;


                    case 0x40:
                        inverter_1.control_mode = ACX1Inverter::SPEED_SET;
                        inverter_1.setSpeedCommand(3000);
                        inverter_1.setTorqueCommand(0x7FFF);
                        inverter_1.sendCommandMessage();
                        break;



                }
            }



        }
//    // Vehicle Settings Command
//    if (message->ui32MsgID == 0x140)
//    {
//        // Cabin Heater Control
//        if (message->pui8MsgData[2] == 0x01)
//        {
//            // Turn off Cabin Heater
//            mmc_heater.disable();
//        }
//        else if (message->pui8MsgData[2] == 0x02)
//        {
//            // Turn on Cabin Heater
//            mmc_heater.enable();
//        }
//
//        // Drive Mode / Regen Settings - Only Allow Changes when Vehicle is Stopped
//        if (isVehicleStopped())
//        {
//            // Drive Mode Setting
//            if (message->pui8MsgData[1] == 0x01)
//            {
//                setDriveMode(NORMAL);
//            }
//            else if (message->pui8MsgData[1] == 0x02)
//            {
//                setDriveMode(EXTREME);
//            }
//            else if (message->pui8MsgData[1] == 0x03)
//            {
//                if (faults.isFaultActive())
//                {
//                    // Clear Faults
//                    faults.clearAll();
//
//                    // Restart Vehicle
//                    changeState(VEHICLE_STARTUP);
//                }
//
//                // Set Drive Mode to Valet
//                setDriveMode(VALET);
//            }
//
//            // Off Pedal Regen Setting
//            if (message->pui8MsgData[0] == 0x01)
//            {
//                setOffPedalRegen(REGEN_LOW);
//            }
//            else if (message->pui8MsgData[0] == 0x02)
//            {
//                setOffPedalRegen(REGEN_HIGH);
//            }
//        }
//
//        processed = true;
//    }
    processed = true;
    if (processed)
    {
        last_message_counter = 0;
        message_received = true;
    }

    return processed;
}

void Application::sendSimpleCAN(uint32_t messageID, uint8_t messageLength, uint64_t data)
{

    // Declare message container
    CANMessage simpleMessage(CANMessage::ID_STANDARD, CANMessage::DATA_FRAME, 8);

    // Set ID and length directly
    simpleMessage.setID(messageID);
    simpleMessage.setLength(messageLength);

    // Iterate to get data bytes from integral type input
    for (uint8_t i = 0; i < messageLength; i++) {
            simpleMessage.getData()[i] = data >> (i * 8);
    }

    // Send Message
    board.can.sendMessage(CAN_1, &simpleMessage);

}


void Application::sendCANData()
{
    /*
    // CAN Message Object for Transmission
    CANMessage msg(CANMessage::ID_STANDARD, CANMessage::DATA_FRAME, 8);

    uint16_t utemp;
    int16_t stemp;

    // Every 20ms - Step 1
    if (board.getStartupCounter() % 2 == 0)
    {
        // Vehicle Information 1 (ID 0x101)
        msg.setID(0x101);
        msg.setLength(8);

        // Vehicle Speed
        //TODO: Swap with APPS Data
        utemp = vehicle_speed * 10.0f;
        msg.getData()[7] = utemp >> 8;
        msg.getData()[6] = utemp;
        // J1772 Measured AC Voltage
        msg.getData()[5] = 0;
        // J1772 Maximum Current Draw
        msg.getData()[4] = 0;
        // Accelerator Position
        utemp = accelerator_command * 10.0f;
        msg.getData()[3] = utemp >> 8;
        msg.getData()[2] = utemp;
        // Regen Lever Position
        msg.getData()[1] = 0;
        msg.getData()[0] = 0;

        // Send Message
        board.can.sendMessage(CAN_1, &msg);
    }

    // Every 100ms - Step 1
    if (board.getStartupCounter() % 10 == 0)
    {
        //TODO: Setup COMs status to other ECU's, how does VCU report its coms with others? Did it loose coms ever?
        // Vehicle Status (ID 0x100)
        msg.setID(0x100);
        msg.setLength(6);

        // Vehicle State
        msg.getData()[5] = state;
        // Device Communication Status
        msg.getData()[4] = bms.isCommunicationGood();
        msg.getData()[4] |= mmc_heater.isCommunicationGood()        << 1;
        msg.getData()[4] |= charger.isCommunicationGood()           << 2;
        msg.getData()[4] |= dcdc_converter.isCommunicationGood()    << 3;
        msg.getData()[4] |= inverter_1.isCommunicationGood()        << 4;
        msg.getData()[4] |= inverter_2.isCommunicationGood()        << 5;
        msg.getData()[4] |= bms.getStatusFlag(BMS1000M::MONITOR_COMMUNICATION_GOOD) << 6;
        // Reserved
        msg.getData()[3] = 0;
        // Status Flags 1
        msg.getData()[2] = isVehicleStopped();
        msg.getData()[2] |= getInput(IGNITION_SW)   << 2;
        msg.getData()[2] |= torque_ready            << 3;
        msg.getData()[2] |= mmc_heater.isEnabled()  << 4;
        // Fault Flags 2
        msg.getData()[1] = faults.getFault(ACCELERATOR_HIGH_FAULT);
        msg.getData()[1] |= faults.getFault(ACCELERATOR_LOW_FAULT)      << 1;
        msg.getData()[1] |= faults.getFault(BMS_FAULT)                  << 2;
        msg.getData()[1] |= faults.getFault(INVERTER_1_FAULT)           << 3;
        msg.getData()[1] |= faults.getFault(INVERTER_2_FAULT)           << 4;
        // Fault Flags 1
        msg.getData()[0] = faults.getFault(INVERTER_1_PRECHARGE_FAULT);
        msg.getData()[0] |= faults.getFault(INVERTER_2_PRECHARGE_FAULT) << 1;
        msg.getData()[0] |= faults.getFault(AUX_PRECHARGE_FAULT)        << 2;
        msg.getData()[0] |= faults.getFault(CHARGER_PRECHARGE_FAULT)    << 3;
        msg.getData()[0] |= faults.getFault(INERTIA_SW_FAULT)           << 4;

        // Send Message
        board.can.sendMessage(CAN_1, &msg);

        // Vehicle Information 2 (ID 0x102)
        msg.setID(0x102);
        msg.setLength(8);

        // Reserved
        msg.getData()[7] = 0xFF;
        msg.getData()[6] = 0xFF;
        // Battery Power
        stemp = bms.getPackVoltage() * bms.getPackCurrent() / 50.0f;
        msg.getData()[5] = stemp >> 8;
        msg.getData()[4] = stemp;
        // Motor Temperature
        stemp = inverter_1.getMotorTemperature() * 10.0f;
        msg.getData()[3] = stemp >> 8;
        msg.getData()[2] = stemp;
        // Motor Speed
        stemp = inverter_1.getMotorSpeed();
        // Don't Allow Negative Numbers
        if (stemp < 0)
        {
            stemp = 0;
        }
        msg.getData()[1] = stemp >> 8;
        msg.getData()[0] = stemp;

        // Send Message
        board.can.sendMessage(CAN_1, &msg);

        // Vehicle Information 3 (ID 0x103)
        // TODO: Send Vehicle Information 3 (Range Information)

        // Vehicle Information 4 (ID 0x104)
        msg.setID(0x104);
        msg.setLength(8);

        // Reserved
        msg.getData()[7] = 0xFF;
        msg.getData()[6] = 0xFF;
        // Reserved
        msg.getData()[5] = 0xFF;
        msg.getData()[4] = 0xFF;
        // Motor Temperature
        stemp = inverter_2.getMaxTemperature() * 10.0f;
        msg.getData()[3] = stemp >> 8;
        msg.getData()[2] = stemp;
        // Motor Speed
        stemp = inverter_1.getMaxTemperature() * 10.0f;
        msg.getData()[1] = stemp >> 8;
        msg.getData()[0] = stemp;

        // Send Message
        board.can.sendMessage(CAN_1, &msg);

        // Vehicle Information 5 (ID 0x105)
        msg.setID(0x105);
        msg.setLength(8);

        // Reserved
        msg.getData()[7] = 0xFF;
        msg.getData()[6] = 0xFF;
        // Power Distribution Box Temperature
        stemp = pd_box_temp * 10.0f;
        msg.getData()[5] = stemp >> 8;
        msg.getData()[4] = stemp;
        // Main Radiator Temperature
        stemp = main_radiator_temp * 10.0f;
        msg.getData()[3] = stemp >> 8;
        msg.getData()[2] = stemp;
        // Cell Radiator Temperature
        stemp = cell_radiator_temp * 10.0f;
        msg.getData()[1] = stemp >> 8;
        msg.getData()[0] = stemp;

        // Send Message
        board.can.sendMessage(CAN_1, &msg);

        // Vehicle Information 6 (ID 0x106)
        msg.setID(0x106);
        msg.setLength(8);

        // Reserved
        msg.getData()[7] = 0xFF;
        msg.getData()[6] = 0xFF;
        msg.getData()[5] = 0xFF;
        msg.getData()[4] = 0xFF;
        msg.getData()[3] = 0xFF;
        msg.getData()[2] = 0xFF;
        // Drive Mode
        msg.getData()[1] = drive_mode;
        // Off Pedal Regen Setting
        msg.getData()[0] = regen_setting;

        // Send Message
        board.can.sendMessage(CAN_1, &msg);

        // VCU Board Status (ID 0x110)
        msg.setID(0x110);
        msg.setLength(8);

        // Board Temperature
        stemp = board.getAnalogValue(VCU1200_Board::BOARD_TEMP) * 10.0f;
        msg.getData()[7] = stemp >> 8;
        msg.getData()[6] = stemp;
        // Internal 12V Sense
        utemp = board.getAnalogValue(VCU1200_Board::SENSE_12V) * 1000.0f;
        msg.getData()[5] = utemp >> 8;
        msg.getData()[4] = utemp;
        // Internal 5V Sense
        utemp = board.getAnalogValue(VCU1200_Board::SENSE_5V) * 1000.0f;
        msg.getData()[3] = utemp >> 8;
        msg.getData()[2] = utemp;
        // Reserved
        msg.getData()[1] = 0;
        msg.getData()[0] = 0;

        // Send Message
        board.can.sendMessage(CAN_1, &msg);

        // Inverter 1 Information (ID 0x120)
        msg.setID(0x120);
        msg.setLength(8);
    }
    // Every 100ms - Step 2
    else if (board.getStartupCounter() % 10 == 1)
    {
        //TODO: DC Bus voltage that Inverters see! Super good for debuging
        // DC Bus Voltage
        utemp = inverter_1.getDCBusVoltage() * 10.0f;
        msg.getData()[7] = utemp >> 8;
        msg.getData()[6] = utemp;
        stemp = inverter_1.getMotorSpeed();
        msg.getData()[5] = stemp >> 8;
        msg.getData()[4] = stemp;
        stemp = inverter_1.getDeltaResolver() * 10.0f;
        msg.getData()[3] = stemp >> 8;
        msg.getData()[2] = stemp;
        stemp = inverter_1.getDCBusCurrent() * 10.0f;
        msg.getData()[1] = stemp >> 8;
        msg.getData()[0] = stemp;

        // Send Message
        board.can.sendMessage(CAN_1, &msg);

        // Inverter 2 Information (ID 0x121)
        msg.setID(0x121);
        msg.setLength(8);

        // DC Bus Voltage
        utemp = inverter_2.getDCBusVoltage() * 10.0f;
        msg.getData()[7] = utemp >> 8;
        msg.getData()[6] = utemp;
        stemp = inverter_2.getMotorSpeed();
        msg.getData()[5] = stemp >> 8;
        msg.getData()[4] = stemp;
        stemp = inverter_2.getDeltaResolver() * 10.0f;
        msg.getData()[3] = stemp >> 8;
        msg.getData()[2] = stemp;
        stemp = inverter_2.getDCBusCurrent() * 10.0f;
        msg.getData()[1] = stemp >> 8;
        msg.getData()[0] = stemp;

        // Send Message
        board.can.sendMessage(CAN_1, &msg);
    }*/
}

void Application::tick()
{
    setOutput(POTENTIOMETER_5V, ON);

    // Process the Main State Machine
    processState();

    // Check for System Faults
    checkFaults();

    // Manage BMS
    manageBMS();

    //Calculate Brake System Encoder
    calcBSE();

    //Calculate Steering System Encoder
    calcSSE();

    //Calculate Accelerator Position Sensor
    calcAPPS();

    //Calculate Torque Command and calculate Torque Split
    calculateTorqueCommand();

    // Process Inverters
    manageInverters();

    // Send CAN Data on Vehicle Bus
    sendCANData();

    //manageConsole();
    //setOutput(POTENTIOMETER_5V, ON);

    // TODO: Send inverter command data

}

void Application::manageInverters()
{
    //TODO: Add StateMachine Logic..

    inverter_1.setTorqueCommand(torque_command_FR);
    //torque_command_FL;        // Nm
    //torque_command_RR;        // Nm
    //torque_command_RL;        // Nm
}

void Application::manageBMS()
{
    /*
    if (state == STARTUP)
    {
        // Start Pre-charge Sequence
        bms.requestStateChange(BMS1000M::BMS_PRECHARGE);

        if (use_inverter_1 && !getOutput(PRECHARGE_1))
        {
            startPrecharge(PRECHARGE_INVERTER_1);
        }

        if (use_inverter_2 && !getOutput(PRECHARGE_2))
        {
            startPrecharge(PRECHARGE_INVERTER_2);
        }
    }
    */

    // Update Powered State
    //bms.setPowered(getOutput(BMS_POWER));

    // Perform Regular Processing
    bms.tick();
}

void Application::checkFaults()
{
    // BMS Master Fault
    if (bms.isFaultActive())
    {
        faults.setFault(BMS_FAULT, true);
    }
    else
    {
        faults.setFault(BMS_FAULT, false);
    }

    // BMS Master Communication Fault
    if (bms.getMessageReceived() > 200)
    {
        faults.setFault(BMS_COMMUNICATION_FAULT, true);
    }
    else
    {
        faults.setFault(BMS_COMMUNICATION_FAULT, false);
    }

    // Accelerator Faults & Brake Faults
    if (systemState == ACTIVE || systemState == STARTUP || systemState == SYSTEM_FAULT)
    {
        //APPS1
        if (getAnalogInput(APPS1) > ACCELERATOR1_HIGH_V_FAULT)
        {
            faults.setFault(ACCELERATOR1_HIGH_FAULT, true);
        }
        else if (getAnalogInput(APPS1) + APPS1_VOLTAGE_HYSTERESIS < ACCELERATOR1_HIGH_V_FAULT)
        {
            faults.setFault(ACCELERATOR1_HIGH_FAULT, false);
        }
        if (getAnalogInput(APPS1) < ACCELERATOR1_LOW_V_FAULT)
        {
            faults.setFault(ACCELERATOR1_LOW_FAULT, true);
        }
        else if (getAnalogInput(APPS1) - APPS1_VOLTAGE_HYSTERESIS > ACCELERATOR1_LOW_V_FAULT)
        {
            faults.setFault(ACCELERATOR1_LOW_FAULT, false);
        }

        //APPS2
        if (getAnalogInput(APPS2) > ACCELERATOR2_HIGH_V_FAULT)
        {
            faults.setFault(ACCELERATOR2_HIGH_FAULT, true);
        }
        else if (getAnalogInput(APPS2) + APPS2_VOLTAGE_HYSTERESIS < ACCELERATOR2_HIGH_V_FAULT)
        {
            faults.setFault(ACCELERATOR2_HIGH_FAULT, false);
        }
        if (getAnalogInput(APPS2) < ACCELERATOR2_LOW_V_FAULT)
        {
            faults.setFault(ACCELERATOR2_LOW_FAULT, true);
        }
        else if (getAnalogInput(APPS2) - APPS2_VOLTAGE_HYSTERESIS > ACCELERATOR2_LOW_V_FAULT)
        {
            faults.setFault(ACCELERATOR2_LOW_FAULT, false);
        }

        //BSE1
        if (getAnalogInput(BSE1) > BSE1_HIGH_V_FAULT)
        {
            faults.setFault(BSE1_HIGH_FAULT, true);
        }
        else if (getAnalogInput(BSE1) + BSE1_VOLTAGE_HYSTERESIS < BSE1_HIGH_V_FAULT)
        {
            faults.setFault(BSE1_HIGH_FAULT, false);
        }
        if (getAnalogInput(BSE1) < BSE1_LOW_V_FAULT)
        {
            faults.setFault(BSE1_LOW_FAULT, true);
        }
        else if (getAnalogInput(BSE1) - BSE1_VOLTAGE_HYSTERESIS > BSE1_LOW_V_FAULT)
        {
            faults.setFault(BSE1_LOW_FAULT, false);
        }
    }

    /*
    // Inverter Faults
    if (use_inverter_1 && inverter_1.isFaultActive())
    {
        faults.setFault(INVERTER_1_FAULT, true);
    }
    else
    {
        faults.setFault(INVERTER_1_FAULT, false);
    }

    if (use_inverter_2 && inverter_2.isFaultActive())
    {
        faults.setFault(INVERTER_2_FAULT, true);
    }
    else
    {
        faults.setFault(INVERTER_2_FAULT, false);
    }

    // Inertia Switch Fault

    if (getInput(INERTIA_SW))
    {
        faults.setFault(INERTIA_SW_FAULT, true);
    }
    else
    {
        faults.setFault(INERTIA_SW_FAULT, false);
    }
    */

    // Perform Fault Processing
    faults.tick();

    // Jump to Fault State after One Second
    if (systemState != SYSTEM_FAULT)
    {
        if (faults.isFaultActive())
        {
            // Jump to Fault State after 500 ms
            if (imminent_fault && fault_counter >= 50)
            {
                changeState(SYSTEM_FAULT);
            }
            else if (!imminent_fault)
            {
                imminent_fault = true;
            }

            // Increment Fault Counter
            fault_counter++;
        }
        else
        {
            imminent_fault = false;
            fault_counter = 0;
        }
    }
    else
    {
        imminent_fault = false;
        fault_counter = 0;
    }
}

void Application::calcBSE()
{

    //Calculate % value per APP, SET MIN MAX BASED ON Sensors seated in Pedal Rack
    //TODO: Set MIN MAX Values Based on Sensors seated in Pedal Rack
    float bse1_read = (getAnalogInput(BSE1) - BSE1_MIN_V) / (BSE1_MAX_V - BSE1_MIN_V);
    float bse2_read = (BSE2_MAX_V - getAnalogInput(BSE2)) / (BSE2_MAX_V - BSE2_MIN_V);

    float min = (((bse1_read) < (bse2_read)) ? (bse1_read) : (bse2_read));   //This probably wont work, but its an idea
    brake_pedal_command = (min*100);                                        //TODO: create a Min function (easy just google if you cant)
    //printf("%f \n",getAnalogInput(BSE1));                                 //DEBUG Raw Value APPS1
    //printf("%f \n",getAnalogInput(BSE2));                                 //DEBUG Raw Value APPS2
    float tolerance = bse1_read*0.1;                                       //Take 10% of value and see if the difference of the two values is greater.

    //TODO: Set the actual Fault using the fault function.
    if(( abs(bse1_read-bse2_read)) > tolerance )
    {
        //setOutput(LED_1, ON);
    }
    else
    {
        //setOutput(LED_1,OFF);
    }
}

void Application::calcAPPS()
{
    //TODO: Set this in the state machine when car is turned on..
    //setOutput(ANALOG_5V_1, ON);

    //Calculate % value per APP, SET MIN MAX BASED ON Sensors seated in Pedal Rack
    //TODO: Set MIN MAX Values Based on Sensors seated in Pedal Rack
    float apps1_read = (getAnalogInput(APPS1) - ACCELERATOR1_MIN_V) / (ACCELERATOR1_MAX_V - ACCELERATOR1_MIN_V);
    float apps2_read = (ACCELERATOR2_MAX_V - getAnalogInput(APPS2)) / (ACCELERATOR2_MAX_V - ACCELERATOR2_MIN_V);

    float min = (((apps1_read) < (apps2_read)) ? (apps1_read) : (apps2_read));   //This probably wont work, but its an idea
    accelerator_command = (min*100);                     //TODO: you may not want to type cast int... ALSO, create a Min function (easy just google if you cant)
    //printf("%f \n",getAnalogInput(APPS1));         //DEBUG Raw Value APPS1
    //printf("%f \n",getAnalogInput(APPS2));         //DEBUG Raw Value APPS2
    float tolerance = apps1_read*0.1;                        //Take 10% of value and see if the difference of the two values is greater.

    //TODO: Set the actual Fault using the fault function.
    if(( abs(apps1_read-apps2_read)) > tolerance )
    {
        //setOutput(LED_1, ON);
    }
    else
    {
        //setOutput(LED_1,OFF);
    }
}

//TODO: Once this is tested, add in regen logic like STAFL Systems, for now, just positive torque scaled to the accel %
void Application::calculateTorqueCommand()
{
    if (systemState == ACTIVE)
    {
        // Positive Torque
        system_torque_command = accelerator_command / 100.0f * max_inverter_torque;

        // Apply Power Limit to Inverter Command
        // FYI - Torque[NM] = (60*Power[KW])/(2*PI*RPM)
        // Power = (2*RPM*Torque[NM])/60
        // TODO: GET "getMotorSpeed()" function working ASAP to Apply a POWER LIMIT! REQUIRED!
        // Reference STAFL Function (setDriveMode) to set varying Max Powers for the inverters. For Comp I'd start with 70Kw. For Testing start way lower
        // SO the 0.0001047f Value = (2*3.14/60)*1000. The 1000 is because your in wats not KWats
        //if (torque_command > 0.0f && torque_command * getMotorSpeed() * 0.0001047f > max_inverter_power)
        //{
        //    torque_command = max_inverter_power / (getMotorSpeed() * 0.0001047f);
        //}
    }
    else
    {
        system_torque_command = 0.0f;
    }

    //TODO: Set the value "4" based on ACTIVE INVERTERS, this way if it changes sys torque is properly calculated
    torque_command_FR = system_torque_command/4;
    torque_command_FL = system_torque_command/4;
    torque_command_RR = system_torque_command/4;
    torque_command_RL = system_torque_command/4;

}

//TODO: Get this function to work with all the motor speeds. Only USE ONE though. And if its reveresed, add negative!
float Application::getMotorSpeed()
{
    /*
    if (use_inverter_1)
    {
        return inverter_1.getMotorSpeed();
    }
    else if (use_inverter_2)
    {
        return -inverter_2.getMotorSpeed();
    }
    else
    {
        // No Inverter is Configured
        return 0.0f;
    }
    */
    return 0.0f;

}

void Application::calcSSE()
{

}

void Application::FarenheitTemp()
{
        if(thermistor_counter >= 20) //will only read input from the thermistor after 20 ticks
        {
            //printf("TEST\n"); //used to seperate different outputs

            sensor_therm1 = getTemperatureFromResistance(getAnalogInput(THM_1), 100000.0f, THERMISTOR_B_VALUE); //pull temperature from (SENSOR_4 // Thermistor1)
            //printf("Thermistor 1 Celsius: %f \n", sensor_therm1); //output

            int tempConvert = static_cast<int>(sensor_therm1);        //convert from float to int //loses precision but were already a little over in our calculation
            tempConvert = ((tempConvert*9)/5)+32;                     //convertion to Farenheit
            //printf("Thermistor 1 Farenheit: %d \n", tempConvert); //output

            thermistor_counter = 0; //reset counter since we have hit 20 ticks
        }
        else
        {
            thermistor_counter++; //increase counter by one
        }
}

float Application::getTemperatureFromResistance(float resistance, float resistance_25c, float b_value)

{
    float temperature;

    // Calculate Temperature
    temperature = (25.0f + 273.15f) * b_value
            / logf(resistance_25c / resistance)
            / (b_value / logf(resistance_25c / resistance) - (25.0f + 273.15f));

    return temperature - 273.15f;
}

void Application::testFunction()
{
    /*
        test = getInput(BUTTON1);

        if (test) {
            setOutput(LED1, ON);frec
        }
        else {
            setOutput(LED1, OFF);
        }
     */
            /*
            if (!button_flag) {
                button_flag = true;
                if(LEDcount == 1) {
                    setOutput(LED1, ON);
                    ++LEDcount;
                }
                else if(LEDcount == 2) {
                    setOutput(LED2, ON);
                    ++LEDcount;
                }
                else if(LEDcount == 3) {
                    setOutput(LED3, ON);
                    ++LEDcount;
                }
                else if(LEDcount == 4) {
                    setOutput(LED4, ON);
                    ++LEDcount;
                }
                else {
                    setOutput(LED1, OFF);
                    setOutput(LED2, OFF);
                    setOutput(LED3, OFF);
                    setOutput(LED4, OFF);
                    LEDcount = 1;
                }
            }
        }
        else {
            button_flag = false;
        }
        */

        /*
        testInput(BUTTON2);
        testInput(BUTTON3);
        testInput(BUTTON4);
        testInput(BUTTON5);
        testInput(BUTTON6);
        */

        //printf("%i\n", getOutput(POTENTIOMETER_5V));
        //printf("%f\n", getAnalogInput(SENSOR_1));

    /*
        sendSimpleCAN(0x469, 4, 0x12345678);

        if (inverter_1.controlMethodPoteniometer) {
            float tempSpeed = getAnalogInput(SENSOR_1);
            tempSpeed = tempSpeed - 2.50;
            if (tempSpeed < 0) {
                inverter_1.setSpeedCommand(0);
            }
            else inverter_1.setSpeedCommand(tempSpeed * 10000);
        }
        inverter_1.tick();
    */
}
