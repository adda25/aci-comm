#ifndef _ACI_COMM_ENGINE_HPP_
#define _ACI_COMM_ENGINE_HPP_
#ifdef __cplusplus

#include <thread>
#include <atomic>
#include <iostream>
#include <tuple>
#include <map>
#include <vector>
#include <assert.h>

#ifndef _ACI_COMM_BUS_HPP_
    #include "bus.hpp"
#endif
#ifndef _ACI_COMM_PACKET_HPP_
    #include "packet.hpp"
#endif
#ifndef _SEMAPHORE_HPP_
    #include "semaphore.hpp"
#endif
#ifndef _ACI_COMM_COMMONS_HPP_
    #include "commons.hpp"
#endif
#ifndef _ACI_COMM_MAP_VAR_CMD_HPP_
    #include "map_var_cmd.hpp"
#endif

namespace acc 
{

    /*
    *    _____             _            
    *   | ____|_ __   __ _(_)_ __   ___ 
    *   |  _| | '_ \ / _` | | '_ \ / _ \
    *   | |___| | | | (_| | | | | |  __/
    *   |_____|_| |_|\__, |_|_| |_|\___|
    *                |___/              
    *   
    */
    /**
    *    Singleton class.
    *
    *    This class is NOT thread safe:
    *    you must use it from only ONE thread.
    */   
    template<class BUS>   
    class Engine
    {
    public:

        /**
        *   Singleton instace creator.
        *   Takes an arbitrary number
        *   of arguments, than wrap the args
        *   in a std::tuple, and call the private
        *   constructor with the BUS(argst).
        */
        template<class... BUS_ARGS>
        static Engine& init(BUS_ARGS... args_) {
            std::tuple<BUS_ARGS...> argst(args_...);
            static Engine<BUS> engine(std::move(BUS(argst)));
            return engine;
        }

    private:

        /**
        *   Private constructor.
        */
        Engine(BUS&& bus_) : 
            _bus(bus_), 
            _aci_thread_run(false),
            _aci_thread_sem(1) {
                auto ptr = &MapVarCmd::init();
                _map_var_cmd = ptr->get_map();
        };

        /**
        *   Private destructor.
        */
        ~Engine() { stop(); }

    public:

        /**
        *   Deleted copy constructor and
        *   copy operator.
        */
        Engine(Engine const&) = delete;
        void operator=(Engine const&) = delete;

        /**
        *   Returns the pointer to
        *   the BUS instance.
        */
        BUS* bus() { return &_bus; }

        void add_read(std::initializer_list<std::string> reads, int pck = 0);
        void add_read(int pck, std::string read);
        
        template<class... Args> void 
        add_read(int pck, std::string key_read, Args... args) {
            add_read(pck, key_read);
            add_read(pck, args...);
        }
  
        void add_write(int pck, std::string write);

        /**
        *   Start port setup,
        *   start aci_thread,
        *   load the packet workload.
        *
        *   Throws when:
        *   * bus cannot be opened
        *   * the port confs cannot be applied
        *   * the packet is not setted
        */
        void start(int ep1 = 100, int ep2 = 10);

        /**
        *   Join the aci_thread to the
        *   main thread.
        */
        void stop();

        int read(std::string key_read);

        template<class... Args> std::vector<int> 
        read(std::string key_read, Args... args) {
            std::vector<int> read_results;
            read_results.push_back(read(key_read));
            auto vec = read(args...);
            //read_results.push_back();
            return read_results;
            //read(key_read);
            //read(args...);
        }

        std::vector<int> 
        read(std::initializer_list<std::string> reads, bool pretty_print = false);  

        void write(std::string key_write, int value);

    private:
        BUS _bus;

        /**
        *   The thread where the 
        *   aci_callback lives.
        */
        std::thread _aci_thread;
        /**
        *   Flag used both as Engine state
        *   and as flag for thread loop exit.
        */
        std::atomic<bool> _aci_thread_run;
        /**
        *   Not used yet.
        */
        Semaphore _aci_thread_sem;
        /**
        *   The dictionary.
        */
        std::map<std::string, DroneItem> _map_var_cmd;

        void _launch_aci_thread();
        void _aci_thread_runner();

    }; // End Engine<BUS>

}; // End namespace

#endif // __cplusplus
#endif // _ACI_COMM_ENGINE_HPP_
