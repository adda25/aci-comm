#include "map_var_cmd.hpp"
/*
*    _____                        ____  _          __  __ 
*   |  ___|   _ _ __  _ __  _   _/ ___|| |_ _   _ / _|/ _|
*   | |_ | | | | '_ \| '_ \| | | \___ \| __| | | | |_| |_ 
*   |  _|| |_| | | | | | | | |_| |___) | |_| |_| |  _|  _|
*   |_|   \__,_|_| |_|_| |_|\__, |____/ \__|\__,_|_| |_|  
*                           |___/                         
*/
/**
*   Macros in order to insert
*   var and cmd to the map.
*/
#define INSERT_VAR(id, num_id) _map_var_cmd.insert(std::make_pair(id, acc::DroneItem(id,  num_id)))
#define INSERT_CMD(id, num_id) _map_var_cmd.insert(std::make_pair(id, acc::DroneItem(id,  num_id, DIP::WRITE)))

void 
acc::MapVarCmd::_alloc_map_var_cmd() {
    _alloc_map_var();
    _alloc_map_cmd();
}

/**
*   Variables.
*   INSERT NEW VARIABLES HERE.
*/
inline void 
acc::MapVarCmd::_alloc_map_var() {
    // 0x00xx: General system informations
    INSERT_VAR("UAV_status",            0x0001);
    INSERT_VAR("flight_time",           0x0002);
    INSERT_VAR("battery_voltage",       0x0003);
    INSERT_VAR("HL_cpu_load",           0x0004);
    INSERT_VAR("HL_up_time",            0x0005);
    // 0x01xx Sensor raw values
    INSERT_VAR("motor_rpm_1",           0x0100);
    INSERT_VAR("motor_rpm_2",           0x0101);
    INSERT_VAR("motor_rpm_3",           0x0102);
    INSERT_VAR("motor_rpm_4",           0x0103);
    INSERT_VAR("motor_rpm_5",           0x0104);
    INSERT_VAR("motor_rpm_6",           0x0105);
    INSERT_VAR("GPS_latitude",          0x0106);
    INSERT_VAR("GPS_longitude",         0x0107);
    INSERT_VAR("GPS_height",            0x0108);
    INSERT_VAR("GPS_speed_x",           0x0109);
    INSERT_VAR("GPS_speed_y",           0x010A);
    INSERT_VAR("GPS_heading",           0x010B);
    INSERT_VAR("GPS_position_accuracy", 0x010C);
    INSERT_VAR("GPS_height_accuracy",   0x010D);
    INSERT_VAR("GPS_speed_accuracy",    0x010E);
    INSERT_VAR("GPS_sat_num",           0x010F);
    INSERT_VAR("GPS_status",            0x0110);
    INSERT_VAR("GPS_time_of_week",      0x0111);
    INSERT_VAR("GPS_week",              0x0112);
    // 0x02xx Sensor calibrated values
    INSERT_VAR("angvel_pitch",          0x0200);
    INSERT_VAR("angvel_roll",           0x0201);
    INSERT_VAR("angvel_yaw",            0x0202);
    INSERT_VAR("acc_x ",                0x0203);
    INSERT_VAR("acc_y",                 0x0204);
    INSERT_VAR("acc_z ",                0x0205);
    INSERT_VAR("Hx",                    0x0206);
    INSERT_VAR("Hy",                    0x0207);
    INSERT_VAR("Hz",                    0x0208);
    // 0x03xx Data-fused values
    INSERT_VAR("angle_pitch",           0x0300);
    INSERT_VAR("angle_roll",            0x0301);
    INSERT_VAR("angle_yaw",             0x0302);
    INSERT_VAR("fusion_latitude",       0x0303);
    INSERT_VAR("fusion_longitude",      0x0304);
    INSERT_VAR("fusion_dheight",        0x0305);
    INSERT_VAR("fusion_height",         0x0306);
    INSERT_VAR("fusion_speed_x",        0x0307);
    INSERT_VAR("fusion_speed_y",        0x0308);
    // 0x06xx User command values
    INSERT_VAR("RC_channel_0",          0x0600);
    INSERT_VAR("RC_channel_1",          0x0601);
    INSERT_VAR("RC_channel_2",          0x0602);
    INSERT_VAR("RC_channel_3",          0x0603);
    INSERT_VAR("RC_channel_4",          0x0604);
    INSERT_VAR("RC_channel_5",          0x0605);
    INSERT_VAR("RC_channel_6",          0x0606);
    INSERT_VAR("RC_channel_7",          0x0607);
}

/**
*   Commands.
*   INSERT NEW COMMANDS HERE.
*/
inline void 
acc::MapVarCmd::_alloc_map_cmd() {
    INSERT_CMD("DIMC_motor_1",                 0x0500);
    INSERT_CMD("DIMC_motor_2",                 0x0501);
    INSERT_CMD("DIMC_motor_3",                 0x0502);
    INSERT_CMD("DIMC_motor_4",                 0x0503);
    INSERT_CMD("DIMC_motor_5",                 0x0504);
    INSERT_CMD("DIMC_motor_6",                 0x0505);
    INSERT_CMD("DMC_pitch",                    0x0506);
    INSERT_CMD("DMC_roll",                     0x0507);
    INSERT_CMD("DMC_yaw",                      0x0508);
    INSERT_CMD("DMC_thrust",                   0x0509);
    INSERT_CMD("CTRL_pitch",                   0x050A);
    INSERT_CMD("CTRL_roll",                    0x050B);
    INSERT_CMD("CTRL_yaw",                     0x050C);
    INSERT_CMD("CTRL_thrust",                  0x050D);
    INSERT_CMD("CTRL_ctrl",                    0x050E);
    INSERT_CMD("ctrl_mode",                    0x0600);
    INSERT_CMD("ctrl_enabled",                 0x0601);
    INSERT_CMD("disable_motor_onoff_by_stick", 0x0602);
}