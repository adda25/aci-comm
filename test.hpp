#include "aci_comm_engine.hpp"
#include "aci_comm_uav.hpp"

void 
testcase() {
    using namespace acc;
    Engine<SerialBus>* ae;

    try {
        std::string port = "/dev/tty.usbserial-A504DRSI";
        //std::string port = "/dev/tty.usbserial-AL00XNUW";
        
        // Create an Engine with a SerialBus.
        // The init arguments are the SerialBus
        // args. *ae* is a pointer to the singleton
        // instance of Engine.
        ae = &Engine<SerialBus>::init(port, B57600);

        // Add variables to read, to packet 1.
        // ae->add_read(0, Var::motor_rpm_1, Var::motor_rpm_2, Var::motor_rpm_3, Var::motor_rpm_4); 
        ae->add_read(0, Var::acc_x, Var::acc_y, Var::acc_z); 
        
        // Add commands to write, packet 0.
        // ae->add_write(0, Cmd::DIMC_motor_1, 
        //                  Cmd::DIMC_motor_2, 
        //                  Cmd::DIMC_motor_3,
        //                  Cmd::DIMC_motor_4,
        //                  Cmd::ctrl_mode,
        //                  Cmd::ctrl_enabled,
        //                  Cmd::disable_motor_onoff_by_stick);


        // Start the engine.
        // Returns when the variables and
        // commands callbacks are executed.
        // Default engine params: 100, 10
        ae->start(1000, 100);

        // For each key_write, write the
        // specified value.
        // ae->write(Cmd::ctrl_mode,                    0);
        // ae->write(Cmd::ctrl_enabled,                 1);
        // ae->write(Cmd::disable_motor_onoff_by_stick, 1);

        // sleep(5);
        std::cout << "READY TO ACC" << std::endl;
        // ae->write(Cmd::DIMC_motor_1, 10);
        // std::cout << "ACC" << std::endl;
        sleep(2);
        
        int i = 0;
        while(i < 1000) {
            // auto yaw = uav.attitude.yaw.read();
            // auto pitch = uav.attitude.pitch.read();
            // auto roll = uav.attitude.roll.read();
            
            std::cout <<  ae->read(Var::acc_x) << "\t" << ae->read(Var::acc_y) << "\t" << ae->read(Var::acc_z) << std::endl;
            //ae->write(Cmd::DIMC_motor_1, 10);

            i++;
            usleep(10000);
        }
        std::cout << "STOP" << std::endl;
        // ae->write(Cmd::DIMC_motor_1, 0);
        //std::cout << "STOP" << std::endl;
        //sleep(5);

    } catch (std::runtime_error e) {
        std::cout << "Exception: " << e.what() << std::endl;
        ae->stop();
    } catch (...) {
        std::cout << "!!! Unexpected error !!!" << std::endl;
        ae->stop();
    }
}

void
testcase2() {
    using namespace acc;
    std::string port = "/dev/tty.usbserial-A504DRSI";

    UAV uav(port, B57600, CTRL_MODE::DIMC);
    try {
        // Enable
        //auto pitch = uav.angles.pitch;
        //pitch.enable_read(0).enable_write(0);

        //uav.angles.pitch.enable_write(0).enable_read(0);
        // uav.attitude.yaw.enable_read(0);
        // uav.attitude.pitch.enable_read(0);
        // uav.attitude.roll.enable_read(0);
        for (int i = 0; i < 4; i++) {
            uav.motors[i].enable_read(0);
            uav.motors[i].enable_write(0);
        }
        uav.start().control_enable(true);

        // Write
        //uav.angles.pitch.write(30.0);
        //uav.angles.roll.write(30.0);
        sleep(2);
        std::cout << "READY" << std::endl;
        // Read
        for (int i = 0; i < 4; i++) {
            uav.motors[i].write(1500);
            sleep(2);
            uav.motors[i].write(1075);
            sleep(2);
        }

    } catch (std::runtime_error e) {
        std::cout << "Exception: " << e.what() << std::endl;
        uav.stop();
    } catch (...) {
        std::cout << "!!! Unexpected error !!!" << std::endl;
        uav.stop();
    }
}

void
testcase3() {
    using namespace acc;
    std::string port = "/dev/tty.usbserial-A504DRSI";

    UAV uav(port, B57600, CTRL_MODE::DIMC);
    try {
        uav.motors.enable_read(0).enable_write(0);
        uav.start().control_enable(true);

        sleep(2);
        std::cout << "READY" << std::endl;
        uav.motors.write({1500, 1500, 1500, 1500});
        sleep(3);
        uav.motors.write(std::array<FLOATING_POINT_PRECISION, 4>{{1075, 1075, 1075, 1075}});
        
    } catch (std::runtime_error e) {
        std::cout << "Exception: " << e.what() << std::endl;
        uav.stop();
    } catch (...) {
        std::cout << "!!! Unexpected error !!!" << std::endl;
        uav.stop();
    }
}

void
testcase_read() {
    using namespace acc;
    using namespace std;
    // std::string port = "/dev/ttyUSB0";
    std::string port = "/dev/tty.usbserial-A504DRSI";

    UAV uav(port, B57600, CTRL_MODE::DIMC);
    // uav.orientation = UAV_Z::UPWARD;
    uav.orientation = UAV_Z::DOWNWARD;

    Logger lg(std::cout);
    lg.floating_point_digits = 2;

    try {
        // frame read test
        // uav.frame.enable_read_angles(0);
        // uav.frame.enable_read_angles_d(1);
        // uav.frame.enable_read_acc(1);
        
        // enable read info
        // uav.info.status.enable_read(1);
        // uav.info.flight_time.enable_read(1);
        // uav.info.battery_voltage.enable_read(1);
        // uav.info.cpu_load.enable_read(1);
        // uav.info.up_time.enable_read(1);

        // enable read RC channels
        uav.rc_ch[0].enable_read(1);
        uav.rc_ch[1].enable_read(1);
        uav.rc_ch[2].enable_read(1);
        uav.rc_ch[3].enable_read(1);
        uav.rc_ch[4].enable_read(1);
        uav.rc_ch[5].enable_read(1);
        uav.rc_ch[6].enable_read(1);
        uav.rc_ch[7].enable_read(1);

        uav.start();

        double freq = 60;

        lg.timer.reset_start_time(); // set timer in logger to 0
        // while(lg.timer.time() < 10.0) {
        while(true) {
            // print angles
            // lg.log(lg.time(), uav.frame.roll.read(), uav.frame.pitch.read(), uav.frame.yaw.read());
            // print angles dot
            // lg.log(lg.time(), uav.frame.roll_d.read(), uav.frame.pitch_d.read(), uav.frame.yaw_d.read());
            // print acc
            // lg.log(lg.timer.time(), uav.frame.x_dd.read(), uav.frame.y_dd.read(), uav.frame.z_dd.read());
            // print infos
            // lg.log(lg.time(), uav.info.status.read(), uav.info.flight_time.read(), uav.info.battery_voltage.read(), uav.info.cpu_load.read(), uav.info.up_time.read());
            // print RC channels
            lg.log(lg.timer.time(), uav.rc_ch[0].read(), uav.rc_ch[1].read(), uav.rc_ch[2].read(), uav.rc_ch[3].read(), uav.rc_ch[4].read(), uav.rc_ch[5].read(), uav.rc_ch[6].read(), uav.rc_ch[7].read());

            usleep(1E6/freq);
        }
        
    } catch (std::runtime_error e) {
        std::cout << "Exception: " << e.what() << std::endl;
        uav.stop();
    } catch (...) {
        std::cout << "!!! Unexpected error !!!" << std::endl;
        uav.stop();
    }
}

void
testcase_motors_start_stop() {
    using namespace acc;
    std::string port = "/dev/tty.usbserial-A504DRSI";

    UAV uav(port, B57600, CTRL_MODE::DIMC);
    try {
        uav.motors.enable_write(0);
        uav.start().control_enable(true);

        sleep(2);
        std::cout << "START" << std::endl;
        uav.motors.start();
        sleep(4);
        std::cout << "STOP" << std::endl;
        uav.motors.stop();
        
    } catch (std::runtime_error e) {
        std::cout << "Exception: " << e.what() << std::endl;
        uav.stop();
    } catch (...) {
        std::cout << "!!! Unexpected error !!!" << std::endl;
        uav.stop();
    }
}


