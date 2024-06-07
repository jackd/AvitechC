
#include "shared_Vars.h"
#include "pin_mappings.h"
#include "AppCmds.h"
#include <util/delay.h>
#include <stdio.h>
// uint8_t Command;
// uint16_t Instruction;

void DecodeCommsData() {
    switch (Command) {
        case 1: Cmd1(); break;
        case 2: Cmd2(); break;
        case 3: Cmd3(); break;
        case 4: Cmd4(); break;
        case 5: Cmd5(); break;
        case 6: Cmd6(); break;
        case 7: Cmd7(); break;
        // case 8: Cmd8(); break;  //Not used
        case 9: Cmd9(); break;
        case 10: Cmd10(); break;
        case 11: Cmd11(); break;
        case 12: Cmd12(); break;
        case 13: Cmd13(); break;
        case 14: Cmd14(); break;
        case 15: Cmd15(); break;
        case 16: Cmd16(); break;
        case 17: Cmd17(); break;
        case 18: Cmd18(); break;
        case 19: Cmd18(); break;
        case 20: Cmd20(); break;
        case 21: Cmd21(); break;
        // case 22: Cmd22(); break;
        // case 23: Cmd23(); break;
        // case 24: Cmd24(); break;
        // case 25: Cmd25(); break;
        // case 26: Cmd26(); break;
        // case 27: Cmd27(); break;
        // case 28: Cmd28(); break;
        // case 29: Cmd29(); break;
        case 30: Cmd30(); break;
        case 31: Cmd31(); break;
        case 32: Cmd32(); break;
        case 33: Cmd33(); break;
        case 34: Cmd34(); break;
        case 35: Cmd35(); break;
        case 36: Cmd36(); break;
        case 37: Cmd37(); break;
        case 38: Cmd38(); break;
    }
}

void Cmd1() {
    // Incoming value 0-100.. This value is a percentage of how many % the user wants laser dimmer than the max laser power
    eeprom_update_byte(&EramUserLaserPower, Instruction);
    UserLaserPower = Instruction;

    GetLaserTemperature();
    ThrottleLaser();
    Audio(1);
    _delay_ms(100);
}

void Cmd2() {
    uint8_t Mask;
    uint8_t A;

    // Process Pan Stop/Start Register
    Mask = 0b00000001;
    A = Mask & Instruction;
    if (A == 1) {
        PanEnableFlag = 1;
    } else {
        PanEnableFlag = 0;
    }

    // Process Pan Direction Register
    Mask = 0b00000010;
    A = Mask & Instruction;
    if (A == 2) {
        PanDirection = 1;
    } else {
        PanDirection = 0;
    }

    // Process Pan Speed Register
    Mask = 0b00000100;
    A = Mask & Instruction;
    if (A == 4) {
        PanSpeed = 1;
    } else {
        PanSpeed = 0;
    }
}

void Cmd3() {
    uint8_t Mask;
    uint8_t A;

    // Process Tilt Stop/Start Register
    Mask = 0b00000001;
    A = Mask & Instruction;
    if (A == 1) {
        TiltEnableFlag = 1;
    } else {
        TiltEnableFlag = 0;
    }

    // Process Tilt Direction Register
    Mask = 0b00000010;
    A = Mask & Instruction;
    if (A == 2) {
        TiltDirection = 1;
    } else {
        TiltDirection = 0;
    }

    // Process Tilt Speed Register
    Mask = 0b00000100;
    A = Mask & Instruction;
    if (A == 4) {
        TiltSpeed = 1;
    } else {
        TiltSpeed = 0;
    }
}

void Cmd4() {
    eeprom_update_byte(&EramMaxLaserPower, Instruction);
    MaxLaserPower = Instruction;
    GetLaserTemperature();
    ThrottleLaser();
    Audio(1);
}

void Cmd5() {
    eeprom_update_word(&EramLaserID, Instruction);
    LaserID = Instruction;
    Audio(1);
}

void Cmd6() {
    X = Instruction;
    ProcessCoordinates();
    SteppingStatus = 1;
    Audio(1);
    // StartTimer1();
    TCCR1B |= (1 << CS12) | (1 << CS10);
}

void Cmd7() {
    Y = Instruction;
    ProcessCoordinates();
    SteppingStatus = 1;
    Audio(1);
    // StartTimer1();
    TCCR1B |= (1 << CS12) | (1 << CS10);
}

void Cmd9() {
// **************** 20240522: These comments from the origingal BASCOM version.
//    The recived data on the bluetooth contains the map point number and the operation zone data.
//    The MSB 8 bits are the operation zone and the LSB is the map point number
//     1111  11111111
//      ^        ^------------ Map point number   eg point 1, point 40 etc
//    '  ^--------------------- Operation Zone     eg Zone 0 to 4
// **************** 20240522
    uint16_t Mask;
    uint16_t Result;
    uint16_t OperationZone;
    int MapPointNumber;

    Mask = 0;
    Result = 0;
    OperationZone = 0;
    MapPointNumber = 0;

    // StopTimer3();
    TCCR3B &= ~((1 << CS32) | (1 << CS30));

    // Get the Operation Zone data from the received data
    Mask = 0b111100000000; // Load the bit mask
    OperationZone = Instruction & Mask; // Filter out the operating zone from the data

    OperationZone <<= 4;

    Result = OperationZone | Y; // Encode the Operation zone into the Y data

    OperationZone >>= 12; // Bit shift the data to the correct format with a value of 0 to 4 Operating Zones

    // Decode the binary data
    switch (OperationZone) {
        case 1: OperationZone = 1; break;
        case 2: OperationZone = 2; break;
        case 4: OperationZone = 3; break;
        case 8: OperationZone = 4; break;
    }

    // Get the map point number from the received Instruction data
    Mask = 0b000011111111; // Load the bit mask
    MapPointNumber = Instruction & Mask; // Filter out the Map Point number from the data

    EramMapTotalPoints = MapPointNumber;
    MapTotalPoints = MapPointNumber; // Store to RAM variable

    uint16_t eepromAddress = (uint16_t)&EramPositions[MapTotalPoints].EramX;
    eeprom_update_word((uint16_t*)eepromAddress, X);
    eepromAddress = (uint16_t)&EramPositions[MapTotalPoints].EramY;
    eeprom_update_word((uint16_t*)eepromAddress, Y);

    Audio(1);

    // Check data that has been saved
    // Result = EramY;
    Result >>= 12; // Bit shift the data to the correct format with a value of 1 to 4 Operating Zones

    // Decode the binary data
    switch (Result) {
        case 1: uartPrint("<22:0001>\n"); break;
        case 2: uartPrint("<22:0002>\n"); break;
        case 4: uartPrint("<22:0003>\n"); break;
        case 8: uartPrint("<22:0004>\n"); break;
        default: uartPrint("<22:0099>\n"); break;
    }

    // StartTimer3();
    TCCR3B |= (1 << CS32) | (1 << CS30);
}

void Cmd10() { //Setup/Run mode selection. Delete all map points. Cold restart
    // uint8_t Mask;
    uint8_t A;

    A = Instruction;

    if (A == 0b00000000) {   //Run mode   <10:0>
        WarnLaserOnOnce = 1; //Enable laser warning when Run Mode button is pressed
        SetupModeFlag = 0;
        PORTE |= ~(1 << BUZZER); // Set BUZZER pin to HIGH
        // LoadPositionMap();  //20240409    Does something else need to be used to replace this?  LoadZoneMap is called from RunSweep.
        // LoadActiveMapZones();
    }

    if (A == 0b00000001) {   //Program Mode  <10:1>
        WarnLaserOnOnce = 1; //Enable laser warning when Program Mode button is pressed
        SetupModeFlag = 1;
        ProgrammingMode();   //Home machine ready for programming in points
    }

    if (A == 0b00000100) {  //Full cold restart of device <10:4>
        Audio(1);
        setupWatchdog();
        _delay_ms(1000);
    }

    if (A == 0b00001000) {  //Setup light sensor mode    <10:8>
        Audio(1);
        SetupModeFlag = 2;
        _delay_ms(1000);
    }
    // --Setup light sensor mode---
    if (A == 0b00010000) {                              //Store current value to default light trigger value    <10:16>
        if (SetupModeFlag == 2 && LightLevel < 100) {
            eeprom_update_byte(&EramFactoryLightTripLevel, LightLevel);
            FactoryLightTripLevel = LightLevel;
            Audio(1);
            _delay_ms(1);
        }
    }

    if (A == 0b00100000) {      //App telling the micro that the bluetooth is connected
        BT_ConnectedFlag = 1;
        SendDataFlag = 0;       //Output data back to application .1=Send data. 0=Don't send data used for testing only  . There just incase setup engineer forgets to turn the data dump off
        Audio(1);
        PrintConfigData();      //Send area data back to the app for user to see
    }

    if (A == 0b01000000) {      //App telling the micro that the bluetooth is disconnected
        BT_ConnectedFlag = 0;
        Audio(1);
    }
}

void Cmd11() {
    // Process the Send Diagnostic Data register
    if (Instruction == 0b00000001) {
        SendDataFlag ^= 1;  //Toggle vale of SendDataFlag
        SendSetupDataFlag = 0;
        Audio(1);
    }

    // Process the full reset flag on next restart
    if (Instruction == 0b00000010) {
        eeprom_update_byte(&EramFirstTimeOn, 255);
        Audio(1);
    }
}

void Cmd12() {
    // Store the accelerometer trip point
    eeprom_update_word(&EramAccelTripPoint, Instruction);
    AccelTripPoint = Instruction;
    Audio(1);
}

void Cmd13() {
    eeprom_update_byte(&EramOperationMode, Instruction);
    OperationMode = Instruction;
    Audio(1);
}

void Cmd14() {    // 20240522: Delete a map point.  It's always the last map point.  
    // Note that with C arrays being zero based, the point to be deleted is MapTotalPoints -1 
    EramPos PosType;
    uint16_t OpZone; // Operation zone number from the Y encoded data

    // Read the map location from EEPROM into Mytype
    uint16_t eepromAddress = (uint16_t)&EramPositions[MapTotalPoints - 1];
    eeprom_read_block(&PosType, (const void*)eepromAddress, sizeof(EramPos));
    // eeprom_read_block(&PosType, &EramPositions[MapTotalPoints - 1], sizeof(EramPos));
    // Get the Y map location and operation data
    OpZone = PosType.EramY;
    OpZone >>= 12; // Bit shift the data to the correct format with a value of 1 to 4 Operating Zones

    // Decode the binary data
    switch (OpZone) {
        case 1: OpZone = 1; break;
        case 2: OpZone = 2; break;
        case 4: OpZone = 3; break;
        case 8: OpZone = 4; break;
    }
    // uartPrint("<19:%X>", OpZone); // Tell App what point on what map was deleted
    printToBT(19, OpZone);
    MapTotalPoints -= 1;
    eeprom_update_byte(&EramMapTotalPoints, MapTotalPoints);
    Audio(1);
}

void Cmd15() {
    // Adjust the laser EramMinimumLaserPower
    // EramMinimumLaserPower = Instruction;
    // MinimumLaserPower = Instruction;
    // Audio(1);
}

void cmdSpeedZone(uint8_t i){
    // Adjust Zone i X & Y speed
    eeprom_update_byte(&EramSpeedZone[i], Instruction);
    SpeedZone[i] = Instruction;
    CalcSpeedZone();
    DSS_preload = CalcSpeed(); // Calculate speed from Y tilt value
    Audio(1);
}
void Cmd16() {
    cmdSpeedZone(0);
}

void Cmd17() {
    cmdSpeedZone(1);
}

void Cmd18() {
    cmdSpeedZone(2);
}

void Cmd19() {
    cmdSpeedZone(3);
}

void Cmd20() {
    cmdSpeedZone(4);
}

void Cmd21() {
    eeprom_update_byte(&EramActiveMapZones, Instruction);
    ActiveMapZones = Instruction;
    // LoadActiveMapZones(); //Does this need to be run?
    Audio(1);
}

void Cmd30() {
    eeprom_update_word(&EramResetSeconds, Instruction);
    ResetSeconds = Instruction;
    Audio(1);
}

void Cmd31() {
    eeprom_update_byte(&EramMapTotalPoints, 0);
    // MapTotalPoints = 0;
    Audio(4);
}

void Cmd32() {
    eeprom_update_byte(&EramLaser2OperateFlag, Instruction);
    Laser2OperateFlag = Instruction;
    Audio(1);
}

void Cmd33() {
    eeprom_update_byte(&EramLaser2BattTrip, Instruction);
    Laser2BattTrip = Instruction;
    Audio(1);
}

void Cmd34() {
    eeprom_update_byte(&EramLaser2TempTrip, Instruction);
    Laser2TempTrip = Instruction;
    Audio(1);
}

void Cmd35() {
    eeprom_update_byte(&EramUserLightTripLevel, Instruction);
    UserLightTripLevel = Instruction;
    Audio(1);
}

void Cmd36() {
    eeprom_update_byte(&EramLightTriggerOperation, Instruction);
    LightTriggerOperation = Instruction;
    Audio(1);
}

void Cmd37() {
    uint16_t HexResult;
    char Result[5];

    SetLaserVoltage(0);
    _delay_ms(1000);

    GetLightLevel();
    HexResult = LightLevel;
    sprintf(Result, "%X", HexResult);
    printToBT(26, HexResult);
    _delay_ms(50);
    // Clear serial input buffer
    Audio(1);
}

void Cmd38() {
    eeprom_update_byte(&EramFactoryLightTripLevel, Instruction);
    FactoryLightTripLevel = Instruction;
    Audio(1);
}