// SYSTEM MATHS CONSTANTS (Don't modify)
#define ON 1
#define OFF 0

// PROJECT CONSTANTS
#define RAMPING_STEPS 1
// #define STARTUP_SPEED 100
#define START_RAMPING_COUNT 100
#define STEP_RATE_MAX 75
// #define STEP_RATE_MIN 2000
#define HOMING_SPEED 100
#define TILT_SLOW_STEP_RATE 1200
#define TILT_FAST_STEP_RATE 500
#define PAN_SLOW_STEP_RATE 400
#define PAN_FAST_STEP_RATE 105
#define LINE_VOLTAGE 5.00
#define X_MAXCOUNT 16000
#define X_MINCOUNT -16000
#define TILT_SENSOR_IGNORE 325
#define NUM_BATT_READINGS 10
#define NUM_TEMP_READINGS 10
#define NIGHT_TRIP_TIME_FROM_STARTUP 240
// #define BT_TICKMAX 3600
#define ONE_SECOND 20
#define NBR_ZONES 4
#define MAX_NBR_VERTICES 10  //This is the maximum number of vertices per zone/map
#define MAX_NBR_MAP_PTS 40   // This is the maximum overall number of vertices
#define MAX_NBR_PERIMETER_PTS 100
#define MAX_NBR_PTS 40
#define STEPS_PER_RAD 2052 //36 * 57 (36 steps per degree and 57 radians per degree?)
#define TILT_SEP  10//Initially used directly as steps of tilt between rungs.  But treat it as m for Cartesian version.
#define MID_PT_SEPARATION 20 // Get the approximate number of mid points to insert between vertices as (x1 - x0)/MID_PT_SEPARATION
#define LASER_HT  5//Height of laser in metres.  Used for polar:cartesian conversions.

// Default values to put in EEPROM (written by LoadDefs.c)
#define DEF_USER_LASER_POWER 100
#define DEF_MAX_LASER_POWER 120
#define DEF_LASER_2_TEMP_TRIP 50
#define DEF_LASER_2_BATT_TRIP 102
#define DEF_LASER_2_OPERATE_FLAG 0
#define DEF_MAP_TOTAL_PTS 0
#define DEF_SPEED 50
#define DEF_ACTIVE_MAP_ZONES 1
#define DEF_OP_MODE 0 
#define DEF_FIRST_TIME_ON 1
#define DEF_LASER_ID 0

#define DEF_ACCEL_TRIP_POINT 600
#define DEF_RESET_SECONDS 3600

#define DEF_USER_LIGHT_TRIP_LEVEL 10
#define DEF_FACTORY_LIGHT_TRIP_LEVEL 10
#define DEF_LIGHT_TRIGGER_OPERATION 1
#define DEF_OPERATION_MODE 0

#define WHO_AM_I_MPU6050 0x75
// MPU pins.  The BASCOM code used a variable and set these pins to 0xD0 and 0xD1 for board version 6.12.  
// These values, 0xD2 and 0xD3, are for version 6.13  Ref sub Initgyro() in BASCOM code.
// These are not physical pins (and not MCU addresses) but addresses in the relevant I2C devices (MPU and laser power controller)
#define MPU6050_W_PIN 0xD2 //These are 8 bit addesses which already include the r/w bit in each.
#define MPU6050_R_PIN 0xD3
// MCP4725 is DAC to control laser power.  Comms is via I2C.
// #define LASER_W_PIN 0xD2 //This value is made up.  Needs to be found.
#define MCP4725 (0x60 << 1) // I2C address of MCP4725.  This is the 7 bit address (0x60) left shifted to get the write address.

#define BAUD 9600 //57600  //Baud rate for HC-05
#define MYUBRR F_CPU/16/BAUD-1 // The formula for calculating the UBRR value is given in the AVR datasheet:
#define DEBUG_MSG_LENGTH 100
