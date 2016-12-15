#ifndef _ACI_COMM_COMMONS_HPP_
#define _ACI_COMM_COMMONS_HPP_
#ifdef __cplusplus

namespace acc
{
    enum class ACI_COMM_VAR
    {                               // UAV READ CLASSES IMPLEMENTATION
        UAV_status             = 0, // implemented
        flight_time            ,    // implemented
        battery_voltage        ,    // implemented
        HL_cpu_load            ,    // implemented
        HL_up_time             ,    // implemented
        motor_rpm_1            ,    // implemented, tested
        motor_rpm_2            ,    // implemented, tested
        motor_rpm_3            ,    // implemented, tested
        motor_rpm_4            ,    // implemented, tested
        motor_rpm_5            ,    // implemented, impossible to test
        motor_rpm_6            ,    // implemented, impossible to test
        GPS_latitude           ,    // implemented
        GPS_longitude          ,    // implemented
        GPS_height             ,    // implemented
        GPS_speed_x            ,    // implemented
        GPS_speed_y            ,    // implemented
        GPS_heading            ,    // implemented
        GPS_position_accuracy  ,    // implemented
        GPS_height_accuracy    ,    // implemented
        GPS_speed_accuracy     ,    // implemented
        GPS_sat_num            ,    // implemented
        GPS_status             ,    // implemented
        GPS_time_of_week       ,    // implemented
        GPS_week               ,    // implemented
        angvel_pitch           ,    // implemented
        angvel_roll            ,    // implemented
        angvel_yaw             ,    // implemented
        acc_x                  ,    // implemented
        acc_y                  ,    // implemented
        acc_z                  ,    // implemented
        Hx                     ,    // implemented
        Hy                     ,    // implemented
        Hz                     ,    // implemented
        angle_pitch            ,    // implemented, tested
        angle_roll             ,    // implemented, tested
        angle_yaw              ,    // implemented, tested
        fusion_latitude        ,    // implemented
        fusion_longitude       ,    // implemented
        fusion_dheight         ,    // implemented
        fusion_height          ,    // implemented
        fusion_speed_x         ,    // implemented
        fusion_speed_y         ,    // implemented
        RC_channel_0           ,    // implemented
        RC_channel_1           ,    // implemented
        RC_channel_2           ,    // implemented
        RC_channel_3           ,    // implemented
        RC_channel_4           ,    // implemented
        RC_channel_5           ,    // implemented
        RC_channel_6           ,    // implemented
        RC_channel_7                // implemented
    };

    enum class ACI_COMM_CMD
    {
        DIMC_motor_1                 = 0, // implemented, tested
        DIMC_motor_2                 ,    // implemented
        DIMC_motor_3                 ,    // implemented
        DIMC_motor_4                 ,    // implemented
        DIMC_motor_5                 ,    //
        DIMC_motor_6                 ,    //
        DMC_pitch                    ,    // implemented
        DMC_roll                     ,    // implemented
        DMC_yaw                      ,    // implemented
        DMC_thrust                   ,    // implemented
        CTRL_pitch                   ,    // implemented
        CTRL_roll                    ,    // implemented
        CTRL_yaw                     ,    // implemented
        CTRL_thrust                  ,    // implemented
        CTRL_ctrl                    ,    // implemented
        ctrl_mode                    ,    // implemented
        ctrl_enabled                 ,    // implemented
        disable_motor_onoff_by_stick      // implemented
    };

    typedef acc::ACI_COMM_VAR Var;
    typedef acc::ACI_COMM_CMD Cmd;

    enum class CTRL_MODE
    {
        DIMC = 0,
        DMC,
        CTRL,
        GPS,
        READ_ONLY
    };

    enum class UAV_Z : int {
      UPWARD = -1,
      DOWNWARD = 1
    };

    enum class CTRL_BIT {
      Pitch  = 0b000001,
      Roll   = 0b000010,
      YawDot = 0b000100,
      Thrust = 0b001000,
      Height = 0b010000,
      GPS    = 0b100000
    };
}

extern "C" {
    #include "asctecCommIntf.h"
}

#endif // __cplusplus
#endif // _ACI_COMM_COMMONS_HPP_
