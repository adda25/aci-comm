#include "engine.hpp"
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
        ae->add_read(0, Var::motor_rpm_1, Var::motor_rpm_2, Var::motor_rpm_3, Var::motor_rpm_4); 
        
        // Add commands to write, packet 0.
        ae->add_write(0, Cmd::DIMC_motor_1,
                         Cmd::DIMC_motor_2,
                         Cmd::DIMC_motor_3,
                         Cmd::DIMC_motor_4,   
                         Cmd::ctrl_mode, 
                         Cmd::ctrl_enabled, 
                         Cmd::disable_motor_onoff_by_stick);


        // Start the engine.
        // Returns when the variables and
        // commands callbacks are executed.
        // Default engine params: 100, 10
        ae->start();

        // For each key_write, write the
        // specified value.
        ae->write(Cmd::ctrl_mode,                    0,
                  Cmd::ctrl_enabled,                 1,
                  Cmd::disable_motor_onoff_by_stick, 1);

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
    acc::UAV uav("/dev/tty.usbserial-A504DRSI", B57600);
    try {
        // Enable
        //uav.angles.pitch.enable_write(0);
        uav.angles.pitch.enable_read(0);
        uav.angles.yaw.enable_read(0);
        //uav.angles.roll.enable_write(2);
        uav.start();
    
        // Write
        //uav.angles.pitch.write(30.0);
        //uav.angles.roll.write(30.0);
    
        // Read
        int i = 0;
        while(i < 1000) {
            auto value = uav.angles.pitch.read();
            auto valueyaw = uav.angles.yaw.read(); 
            std::cout << value << " " << valueyaw << std::endl;
            i++;
            usleep(10000);
        }
        
    } catch (std::runtime_error e) {
        std::cout << "Exception: " << e.what() << std::endl;
        uav.stop();
    } catch (...) {
        std::cout << "!!! Unexpected error !!!" << std::endl;
        uav.stop();
    }
}


