#include "engine.hpp"

/*
*   C API Callbacks
*/
static void c_api_transmit_callback(void*, unsigned short);
static void c_api_versions_callback(struct ACI_INFO);
static void c_api_reads_callback(void);
static void c_api_writes_callback(void);
static void c_api_params_callback(void);

/**
*   Anonymous namespace for 
*   global variables, is the 
*   same as declaring static
*   variables.
*/
namespace  
{
    /**
    *   Pointer to the bus 
    *   port state. Used by 
    *   the *transmit* callback.
    *   Pointing at the Bus member
    *   variable.
    */
    int *_bus_port;   
    int _version_callback = 0;
    int _reads_callback = 0;
    int _writes_callback = 0;
    int _reads_update_val = 10;
    std::map<std::string, acc::DroneItem> _requsted_vars;
    std::map<std::string, acc::DroneItem> _requsted_cmds;
}


/*
*    _____             _            _____                     
*   | ____|_ __   __ _(_)_ __   ___|  ___|   _ _ __   ___ ___ 
*   |  _| | '_ \ / _` | | '_ \ / _ \ |_ | | | | '_ \ / __/ __|
*   | |___| | | | (_| | | | | |  __/  _|| |_| | | | | (__\__ \
*   |_____|_| |_|\__, |_|_| |_|\___|_|   \__,_|_| |_|\___|___/
*                |___/                                        
*
*/                            
template<class BUS> void 
acc::Engine<BUS>::start(int ep1, int ep2) {
    if (_aci_thread_run) 
        throw std::runtime_error("Engine is already started!");

    if (ep1 < 0 || ep2 < 0) 
        throw std::runtime_error("Ep1 and Ep2 must be positive integers!");
    
    if (_requsted_vars.empty() && _requsted_cmds.empty()) 
        throw std::runtime_error("Neither reads or writes are set!");

    try {
        _bus.open(); 
        _bus_port = &_bus._port_state;
        aciInit();

        // Callbacks
        aciSetSendDataCallback(&c_api_transmit_callback);              // Transmit
        aciInfoPacketReceivedCallback(&c_api_versions_callback);       // Version 
        aciSetVarListUpdateFinishedCallback(&c_api_reads_callback);    // Read
        aciSetCmdListUpdateFinishedCallback(&c_api_writes_callback);   // Write
        aciSetParamListUpdateFinishedCallback(&c_api_params_callback); // Params

        // Set engine and start thread.
        aciSetEngineRate(ep1, ep2);
        _launch_aci_thread();
        
        // Version
        aciCheckVerConf(); 
        while(!_version_callback) usleep(1000);
        
        // Read
        if (!_requsted_vars.empty()) {
            aciGetDeviceVariablesList(); 
            while(!_reads_callback) usleep(1000);
        }
        
        // Write
        if (!_requsted_cmds.empty()) {
            aciGetDeviceCommandsList(); 
            while(!_writes_callback) usleep(1000);
        }

    } catch (std::runtime_error e) {
        throw e;
    }
}

template<class BUS> void 
acc::Engine<BUS>::stop() {
    if (!_aci_thread_run) 
        return;

    _aci_thread_run = false;
    _aci_thread.join();
    _requsted_vars.clear();
    _requsted_cmds.clear();
    _version_callback = 0;
    _reads_callback   = 0;
    _writes_callback  = 0;
}

template<class BUS> void 
acc::Engine<BUS>::_launch_aci_thread() {
    _aci_thread_run = true;
    _aci_thread = std::thread(&acc::Engine<BUS>::_aci_thread_runner, this);
}
        
template<class BUS> void 
acc::Engine<BUS>::_aci_thread_runner() {
    int result = 0;
    unsigned char data = 0;
    while (_aci_thread_run) {
        result = ::read(_bus._port_state, &data, 1);
        while (result != -1) {
            aciReceiveHandler(data);
            result = ::read(_bus._port_state, &data, 1);
        }
        aciSynchronizeVars();
        aciEngine();
        usleep(10000);
    }
}

template<class BUS> void 
acc::Engine<BUS>::add_read(int pck, std::string read) {
    if (_aci_thread_run) throw std::runtime_error("Engine is running, you cannot add reads");
    std::map<std::string, DroneItem>::iterator it;
    it = _map_var.find(read);
    if (it == _map_var.end()) throw std::runtime_error("This entry read key not exist: " + read);
    if (!it->second.can_be_read()) throw std::runtime_error("This entry read cannot be read: " + read);
    _requsted_vars.insert(std::make_pair(read, it->second));
    std::map<std::string, DroneItem>::iterator it2;
    it2 = _requsted_vars.find(read);
    it2->second.pck = pck;
}

template<class BUS> void 
acc::Engine<BUS>::add_write(int pck, std::string write) {
    if (_aci_thread_run) throw std::runtime_error("Engine is running, you cannot add writes");
    std::map<std::string, DroneItem>::iterator it;
    it = _map_cmd.find(write);
    if (it == _map_cmd.end()) throw std::runtime_error("This entry write key not exist: " + write);
    if (!it->second.can_be_written()) throw std::runtime_error("This entry write cannot be written: " + write);
    _requsted_cmds.insert(std::make_pair(write, it->second));
    std::map<std::string, DroneItem>::iterator it2;
    it2 = _requsted_cmds.find(write);
    it2->second.pck = pck;
}

template<class BUS> int 
acc::Engine<BUS>::read(std::string key_read) {
    for (std::map<std::string, DroneItem>::iterator it=_requsted_vars.begin(); 
        it!=_requsted_vars.end(); ++it) 
    {
        if (it->first == key_read) {
            return *it->second.value_ptr();
        }
    }
    throw std::runtime_error("This entry read key not exist: " + key_read);
}  

template<class BUS> void 
acc::Engine<BUS>::write(std::string key_write, int value_write) {
    for (std::map<std::string, DroneItem>::iterator it=_requsted_cmds.begin(); 
        it!=_requsted_cmds.end(); ++it) 
    {
        if (it->first == key_write) {
            it->second.set_value(value_write);
            aciUpdateCmdPacket(it->second.pck);
            return;
        }
    }
    throw std::runtime_error("This entry write key not exist: " + key_write);
}


/*
*     ____      _ _ _                _                   ____ 
*    / ___|__ _| | | |__   __ _  ___| | _____           / ___|
*   | |   / _` | | | '_ \ / _` |/ __| |/ / __|  _____  | |    
*   | |__| (_| | | | |_) | (_| | (__|   <\__ \ |_____| | |___ 
*    \____\__,_|_|_|_.__/ \__,_|\___|_|\_\___/          \____|
*                                                                                                                                                                                                       
*/
static void 
c_api_transmit_callback(void* byte, unsigned short cnt)
{
    unsigned char *tbyte = (unsigned char *)byte;
    for (int i = 0; i < cnt; i++) {
        ::write(*_bus_port, &tbyte[i], 1);
    }
}

static void 
c_api_versions_callback(struct ACI_INFO aciInfo) {
    printf("******************** Versions *******************\n");
    printf("* Type\t\t\tDevice\t\tRemote\t*\n");
    printf("* Major version\t\t%d\t=\t\%d\t*\n", aciInfo.verMajor,ACI_VER_MAJOR);
    printf("* Minor version\t\t%d\t=\t\%d\t*\n", aciInfo.verMinor,ACI_VER_MINOR);
    printf("* MAX_DESC_LENGTH\t%d\t=\t\%d\t*\n", aciInfo.maxDescLength,MAX_DESC_LENGTH);
    printf("* MAX_NAME_LENGTH\t%d\t=\t\%d\t*\n", aciInfo.maxNameLength,MAX_NAME_LENGTH);
    printf("* MAX_UNIT_LENGTH\t%d\t=\t\%d\t*\n", aciInfo.maxUnitLength,MAX_UNIT_LENGTH);
    printf("* MAX_VAR_PACKETS\t%d\t=\t\%d\t*\n", aciInfo.maxVarPackets,MAX_VAR_PACKETS);
    printf("*************************************************\n");
    _version_callback = 1;
}

static void 
c_api_reads_callback() {
    std::vector<int> pkc_idx;
    for (std::map<std::string, acc::DroneItem>::iterator it=_requsted_vars.begin(); 
        it!=_requsted_vars.end(); ++it) 
    {
        pkc_idx.push_back(it->second.pck);
        aciAddContentToVarPacket(it->second.pck, it->second.num_id(), it->second.value_ptr()); 
    }
    std::sort(pkc_idx.begin(), pkc_idx.end());
    pkc_idx.erase(std::unique(pkc_idx.begin(), pkc_idx.end()), pkc_idx.end());
    for (auto &i : pkc_idx) {
        aciSetVarPacketTransmissionRate(i, _reads_update_val);
        aciVarPacketUpdateTransmissionRates();
        aciSendVariablePacketConfiguration(i);
    }
    _reads_callback = 1;
}

static void 
c_api_writes_callback() {
    std::vector<int> pkc_idx;
    for (std::map<std::string, acc::DroneItem>::iterator it=_requsted_cmds.begin(); 
        it!=_requsted_cmds.end(); ++it) 
    {
        pkc_idx.push_back(it->second.pck);
        aciAddContentToCmdPacket(it->second.pck, it->second.num_id(), it->second.value_ptr());
    }
    std::sort(pkc_idx.begin(), pkc_idx.end());
    pkc_idx.erase(std::unique(pkc_idx.begin(), pkc_idx.end()), pkc_idx.end());
    for (auto &i : pkc_idx) {
        aciSendCommandPacketConfiguration(i, 1);
        aciUpdateCmdPacket(i);
    }
    _writes_callback = 1;
}

static void 
c_api_params_callback() {}

/*
*    _____         _____                _ 
*   | ____|_  ____|_   _| __ ___  _ __ | |
*   |  _| \ \/ / __|| || '_ ` _ \| '_ \| |
*   | |___ >  < (__ | || | | | | | |_) | |
*   |_____/_/\_\___||_||_| |_| |_| .__/|_|
*                                |_|      
*   
*/
/**
*   Explicit template instantiation.
*   MUST be kept at the end of this file,
*   don't move!
*/
#include "explicits_templates.hpp"


//aciGetVarPacketRateFromDevice();
//aciSendParamLoad();




