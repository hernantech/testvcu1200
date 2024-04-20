/*
******************************************************************************
    SME Group ACX1 Motor Controller/Inverter
    Paul Schneider
    5/1/2018

    Modified from:
    Rinehart Motion Systems PM100 Inverter Class
    Erik Stafl
    10/23/2013

    Written for All Processors
    Language: Embedded C++

    Copyright (c) 2013 Stafl Systems, LLC.  All rights reserved.
******************************************************************************
*/

#ifndef ACX1INVERTER_H_
#define ACX1INVERTER_H_

#define TPDO_OFFSET_1 0x180
#define TPDO_OFFSET_2 0x280
#define TPDO_OFFSET_3 0x380
#define TPDO_OFFSET_4 0x480
#define TPDO_OFFSET_5 0x680
#define TPDO_OFFSET_6 0x6A0

#define RPDO_OFFSET_1 0x200
#define RPDO_OFFSET_2 0x300
#define RPDO_OFFSET_3 0x400
#define RPDO_OFFSET_4 0x500

#include "Library/critical.h"
#include "Library/can_interface.h"
#include "Library/can_receiver.h"
#include "Library/error.h"

class ACX1Inverter : public CANReceiver
{
public:
    // Inverter Directions
    enum MainState
    {
        UKNOWN = 0,
        STARTUP,
        MAINS_CHECK,
        MAINS_OK,
        PRECHARGING,
        PRECHARGE_OK,
        POWER_READY,
        ALARMED
    };

    enum Direction
    {
        NEUTRAL = 0,
        FORWARD,
        REVERSE
    };

    // Inverter State Machine State
    enum VSMState
    {
        VSM_STATE_1 = 1,
        VSM_STATE_2,
        VSM_STATE_3,
        VSM_STATE_4,
        VSM_STATE_5,
        VSM_STATE_6,
        VSM_STATE_7
    };

    enum ControlMode {
        SPEED_SET = 1,
        TORQUE_SET,
        DISABLED
    };
    // Default Constructor
    ACX1Inverter(CANInterface* can_interface, unsigned int can_id_offset);

    // Disable Inverter
    void disable();

    // Enable Inverter
    void enable();

    // Get DC Bus Current
    float getDCBusCurrent();

    // Get DC Bus Voltage at Inverter
    float getDCBusVoltage();

    // Get Delta Resolver Angle
    float getDeltaResolver();

    // Return the Current Direction of the Inverter
    Direction getDirection();

    // Return Maximum Temperature within Inverter
    float getMaxTemperature();

    // Get Motor Speed in RPM
    float getMotorSpeed();

    // Get Motor Temperature in Deg C
    float getMotorTemperature();

    // Initialize CAN Messages for Reception
    void initializeCANReceive();

    // Return True if Inverter Fault is Active
    bool isFaultActive();

    // Process Received CAN Message
    bool receiveCANMessage(CANPort can_port, tCANMsgObject* message, unsigned int mailbox);

    // Resets All Class Data
    void reset();

    // Send Command Packet to Inverter
    void sendCommandMessage(short opt = 0);

    // Set Inverter Direction
    void setDirection(Direction direction);

    // Set Speed Command
    void setSpeedCommand(short spd_cmd);
    // Set Torque Command
    void setTorqueCommand(float tq_cmd);

    // Perform Regular Processing
    void tick();

public:
    // CAN Interface for Message Transmission
    CANInterface* can;

    // CAN ID Offset of Message
    unsigned int can_offset;

    // TODO: remove
    unsigned short debug_mcu_counter;

    ControlMode control_mode;

    // TODO change or clean
    bool controlMethodPoteniometer;




    // Current Inverter Direction
    Direction direction;
    Direction requested_direction;

    // How many ticks we've been in the current direction
    unsigned int direction_counter;

    // Current VSM State
    //TODO: ??
    VSMState vsm_state;

    // True if Inverter is Powered Up
    bool enabled;

    // Inverter Command Enable Bit
    bool inverter_enabled;

    // TODO: ???
    bool inverter_lockout;
    bool inverter_lockout_clear;

    // TODO: ???
    bool enable_discharge;
    bool main_contactor;    // True if Closed
    bool precharge_relay;   // True if Closed

    float speed_command;    // RPM
    float torque_command;   // [-100, 100] % of maximum
    float feedback_torque;  // TODO: figure what this will be


    // Current Motor Speed in RPM
    float motor_speed;          // RPM
    float motor_angle;          // Degrees
    float electrical_output_f;  // Hz
    float delta_resolver;       // Degrees

    // Current Information
    float phase_a_current;  // Arms
    float phase_b_current;  // Arms
    float phase_c_current;  // Arms
    float dc_bus_current;   // A
    float battery_voltage;  // V

    // Voltage Information
    float dc_bus_voltage;   // V
    float output_voltage;   // Vpk
    //TODO: ???
    float ab_voltage;       // V
    float bc_voltage;       // V

    // Temperature Information
    float module_a_temperature;
    float module_b_temperature;
    float module_c_temperature;
    float gate_drive_temperature;
    float control_board_temperature;

    float motor_temperature;

    // Fault Information
    bool fault_active;



    uint16_t post_faults_low;
    uint16_t post_faults_high;
    uint16_t run_faults_low;
    uint16_t run_faults_high;

    uint8_t inverter_mains_state;
    uint8_t inverter_fault_code;
    uint8_t inverter_fault_level;
    uint8_t inverter_motor_state;

    uint8_t request_mains_state;
    uint8_t request_sw_reset;


    // Number of Ticks Since Program Start
    unsigned int tick_counter;
};

#endif /* RMSINVERTER_H_ */
