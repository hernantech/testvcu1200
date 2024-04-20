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

// For CAN Addressing among this device category, adressing has two components
// (T/R)PDO_OFFSET_n is the ACX1 TPDO and RPDO offsets (0x180, 0x280, . . .)
// can_id_offset is each inverter's unique address (0x001, 0x002, . . . )
// Directly sending a CAN message will usually require both of these.
// TODO: Functions to update motor control parameters (Individual or ALL addressing flags)
// TODO: Functions to enforce timing sync across motors (Send all RPDO1 first, then RPDO2, etc.)
#include <stdio.h>
#include "sme_acx1_inverter.h"
#include <climits>




ACX1Inverter::ACX1Inverter(CANInterface* can_interface, unsigned int can_id_offset)
{
    reset();



    // Save Pointer to CAN Interface
    can         = can_interface;
    can_offset  = can_id_offset;

    debug_mcu_counter = 0;

}


// Section below includes all sample code from the RMS inverter.
// TODO: Complete reconstruction of similar functions for the SME ACX1 controllers.
//       Note: Not all functions needed. Many new functions will be needed.
void ACX1Inverter::disable()
{
    enabled = false;
    inverter_enabled = false;
    torque_command = 0.0f;

    // Send Command Message
    sendCommandMessage();
}

void ACX1Inverter::enable()
{
    enabled = true;
    vsm_state = VSM_STATE_1;
    inverter_lockout_clear = false;
}

float ACX1Inverter::getDCBusCurrent()
{
    return dc_bus_current;
}

float ACX1Inverter::getDCBusVoltage()
{
    return dc_bus_voltage;
}

float ACX1Inverter::getDeltaResolver()
{
    return delta_resolver;
}

ACX1Inverter::Direction ACX1Inverter::getDirection()
{
    return direction;
}

float ACX1Inverter::getMaxTemperature()
{
    float max_temp = module_a_temperature;

    // Find Highest Internal Temperature
    if (module_b_temperature > max_temp)
    {
        max_temp = module_b_temperature;
    }

    if (module_c_temperature > max_temp)
    {
        max_temp = module_c_temperature;
    }

    if (gate_drive_temperature > max_temp)
    {
        max_temp = gate_drive_temperature;
    }

    return max_temp;
}

float ACX1Inverter::getMotorSpeed()
{
    return motor_speed;
}

float ACX1Inverter::getMotorTemperature()
{
    return motor_temperature;
}

void ACX1Inverter::initializeCANReceive()
{
    // Temporary Mailbox for Setup
    tCANMsgObject can_rx_message;

    // Setup Receive Messages on CAN Interface

    /* All Inverter Feedback Messages
    can_rx_message.ui32MsgID = TPDO_OFFSET_1;   //Sets a "base" mailbox at TPDO_OFFSET_1
    can_rx_message.ui32MsgIDMask = 0x1FFFFFF0;  //Filter allows in all messages that: msgID = (ui32MsgID & ui32MsgIDMsk) + ( 0x[n...n] & !ui32MsgIDMask)
    can_rx_message.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER; // TODO: ??????? idk what this does
    can->initializeReceiveMessage(this, &can_rx_message);

    // MsgIDMask basically lets you set a range of values where zeroes are the variable part
    // and any F's are firm. The leading 1 is for 29-bit addresses but works equally for 11-bit as well.
    */

    can_rx_message.ui32MsgID = TPDO_OFFSET_1 + can_offset;
    can_rx_message.ui32MsgIDMask = 0x7FF;
    can_rx_message.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    can->initializeReceiveMessage(this, &can_rx_message);


    can_rx_message.ui32MsgID = TPDO_OFFSET_2 + can_offset;
    can_rx_message.ui32MsgIDMask = 0x7FF;
    can_rx_message.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    can->initializeReceiveMessage(this, &can_rx_message);

    can_rx_message.ui32MsgID = TPDO_OFFSET_3 + can_offset;
    can_rx_message.ui32MsgIDMask = 0x7FF;
    can_rx_message.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    can->initializeReceiveMessage(this, &can_rx_message);

    can_rx_message.ui32MsgID = TPDO_OFFSET_4 + can_offset;
    can_rx_message.ui32MsgIDMask = 0x7FF;
    can_rx_message.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    can->initializeReceiveMessage(this, &can_rx_message);

    can_rx_message.ui32MsgID = TPDO_OFFSET_5 + can_offset;
    can_rx_message.ui32MsgIDMask = 0x7FF;
    can_rx_message.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    can->initializeReceiveMessage(this, &can_rx_message);

    can_rx_message.ui32MsgID = TPDO_OFFSET_6 + can_offset;
    can_rx_message.ui32MsgIDMask = 0x7FF;
    can_rx_message.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    can->initializeReceiveMessage(this, &can_rx_message);


}

bool ACX1Inverter::isFaultActive()
{
    if (powered)
    {
        return fault_active;
    }
    else
    {
        return false;
    }
}

bool ACX1Inverter::receiveCANMessage(CANPort can_port, tCANMsgObject* message, unsigned int mailbox)
{


    // True if Message has been Processed
    bool processed = false;

    switch (message->ui32MsgID - can_offset)
    {

    // TODO: All cases for receiving Messages from the ACX1.

    case TPDO_OFFSET_1:
    // TPDO1 contains mostly high-level system status

        // TODO
        // tpdo1 contains master state

        inverter_mains_state = message->pui8MsgData[0]; // 0 ... 7; 7 = alarmed
        inverter_fault_code = message->pui8MsgData[1];
        dc_bus_voltage = (short)(message->pui8MsgData[2] | message->pui8MsgData[3] << 8) / 10.0f;
        dc_bus_current = (short)(message->pui8MsgData[4] | message->pui8MsgData[5] << 8) / 10.0f;
        dc_bus_current = (short)(message->pui8MsgData[6] | message->pui8MsgData[7] << 8) / 10.0f;

        break;


    case TPDO_OFFSET_2:
        // TODO inverter analog inputs::::: Not important right now
        break;


    case TPDO_OFFSET_3:
        // TODO analog inputs, driver status outputs
        inverter_fault_level = message->pui8MsgData[7] & 0xE0; // bits 5...7 of byte 7
        break;


    case TPDO_OFFSET_4:
        // motor feedback data
        motor_speed     = message->pui8MsgData[0] | message->pui8MsgData[1];
        feedback_torque = message->pui8MsgData[2] | message->pui8MsgData[3];
        inverter_motor_state = message->pui8MsgData[7];
        break;


    case TPDO_OFFSET_5:
        // TODO only used in two motor setups?
        break;


    case TPDO_OFFSET_6:
        // TODO literally no idea what this data is in here. need to find out later. probably not important now.
        break;




    default:
        break;
    }

    if (processed)
    {
        last_message_counter = 0;
        message_received = true;
    }

    return processed;
}

void ACX1Inverter::reset()
{
    enabled             = false;
    direction_counter   = 0;
    torque_command      = 0.0f;
    direction           = NEUTRAL;
    requested_direction = NEUTRAL;
    inverter_enabled    = false;
    fault_active        = false;
}

// TODO short --> uint8_t
void ACX1Inverter::sendCommandMessage(short opt)
{
    // CAN Message Object for Transmission
    CANMessage msg(CANMessage::ID_STANDARD, CANMessage::DATA_FRAME, 8);

    // Send Inverter Command Messages (0 for all, otherwise individual RPDO_n)

    msg.setLength(8);

    if (opt == 0x00 || opt == 0x01)
    {
        msg.setID(RPDO_OFFSET_1 + can_offset);
        enter_critical_section();
        msg.getData()[0] = request_mains_state;

        // TODO not currently using any digital or driver outputs
        // TODO Need to make specific case reset (Sw_reset bits) instance.
        msg.getData()[1] = 0x00;
        msg.getData()[2] = 0x00;
        msg.getData()[3] = 0x00;
        msg.getData()[4] = 0x00;
        msg.getData()[5] = 0x00;
        msg.getData()[6] = 0x00;
        msg.getData()[7] = 0x00;

        can->sendMessage(&msg);
        exit_critical_section();
    }

    if (opt == 0x00 || opt == 0x02)
    {

        short torque = torque_command;
        short speed  = speed_command;

        msg.setID(RPDO_OFFSET_2 + can_offset);
        enter_critical_section();
        msg.getData()[0] = speed;
        msg.getData()[1] = speed >> 8;
        msg.getData()[2] = torque;
        msg.getData()[3] = torque >> 8;

        // TODO set droop rate appropriately, leaving as 25% for now
        msg.getData()[4] = 0x40; // 0, 0% ... 255, 100% --> 64 = 25%

        // control_mode {0, 1, 2} = {disable, speed reference (torque limited), torque reference (speed limited)}

        // currently enables: Speed request mode, PWM output, and Reference speed request. Needs to be 0000 11nn_b for now. nn = 01 for speed, 10 for torque, 00 or 11 for no request
        msg.getData()[5] = 0x0D;

        msg.getData()[6] = 0x00;
        msg.getData()[7] = 0x00;

        can->sendMessage(&msg);

        exit_critical_section();
    }

    if (opt == 0x00 || opt == 0x03)
    {

        msg.setID(RPDO_OFFSET_3 + can_offset);
        enter_critical_section();


        // TODO Motor 2 controls, not used at all by us/
        msg.getData()[0] = 0x00;
        msg.getData()[1] = 0x00;
        msg.getData()[2] = 0x00;
        msg.getData()[3] = 0x00;
        msg.getData()[4] = 0x00;
        msg.getData()[5] = 0x00;
        msg.getData()[6] = 0x00;
        msg.getData()[7] = 0x00;

        can->sendMessage(&msg);

        exit_critical_section();
    }

    if (opt == 0x00 || opt == 0x03)
    {

        msg.setID(RPDO_OFFSET_3 + can_offset);
        enter_critical_section();


        // TODO Motor 2 controls, not used at all by us/
        msg.getData()[0] = 0x00;
        msg.getData()[1] = 0x00;
        msg.getData()[2] = 0x00;
        msg.getData()[3] = 0x00;
        msg.getData()[4] = 0x00;
        msg.getData()[5] = 0x00;
        msg.getData()[6] = 0x00;
        msg.getData()[7] = 0x00;

        can->sendMessage(&msg);

        exit_critical_section();
    }




}

void ACX1Inverter::setDirection(Direction dir)
{
    // Minimum Time in Each Direction is 10 ticks
    if (direction_counter < 10 || direction == dir)
        return;

    // Move Through Neutral Before Switching Directions
    if ((dir == FORWARD && direction == REVERSE) || dir == REVERSE && direction == FORWARD)
    {
        direction = NEUTRAL;
    }
    else
    {
        direction = dir;
    }

    requested_direction = dir;
    direction_counter = 0;

    if (direction == NEUTRAL)
    {
        torque_command = 0.0f;
        inverter_enabled = false;
    }
    else if (direction == FORWARD || direction == REVERSE)
    {
        inverter_enabled = true;
    }
}

void ACX1Inverter::setSpeedCommand(short spd_cmd)
{
    // speed in RPM
    speed_command = spd_cmd;
}
void ACX1Inverter::setTorqueCommand(float tq_cmd)
{
    // TODO take [-100, 100] % intended value --> short LOW to HIGH
    // TODO verify method

    torque_command = tq_cmd / 100 * SHRT_MAX;
}

void ACX1Inverter::tick()
{
    if (enabled)
    {
//        // Clear Inverter Lockout
//        if (!inverter_lockout_clear && vsm_state == VSM_STATE_4)
//        {
//            direction = FORWARD;
//            inverter_enabled = false;
//            sendCommandMessage();
//
//            // Inverter can now be Enabled
//            direction = NEUTRAL;
//            inverter_lockout_clear = true;
//        }

        // Manage Delayed Direction Changes
        if (direction != requested_direction && direction_counter >= 10)
            setDirection(requested_direction);

        if (tick_counter % 2 == 0)
        {
            sendCommandMessage(2);
            //printf("Command Message sent - - - Speed: %f , Torque: %f\n", speed_command, torque_command);
        }
    }

    // Increment Counters
    incrementCounters();
    direction_counter++;
    tick_counter++;
}

