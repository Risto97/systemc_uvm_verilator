#ifndef DTI_MONITOR_HPP_
#define DTI_MONITOR_HPP_

#include <systemc>
#include <tlm.h>
#include <uvm>

#include "dti_vif.hpp"
#include "dti_packet.hpp"

class dti_monitor : public uvm::uvm_monitor
{
 public:
  uvm::uvm_analysis_port<dti_packet> item_collected_port;

  dti_vif* vif;
  bool checks_enable;
  bool coverage_enable;

  dti_monitor(uvm::uvm_component_name name)
  : uvm_monitor(name),
    item_collected_port("item_collected_port"),
    vif(0),
    checks_enable(false),
    coverage_enable(false)
  {}

  UVM_COMPONENT_UTILS(dti_monitor);

  void build_phase(uvm::uvm_phase& phase)
  {
    std::cout << sc_core::sc_time_stamp() << ": build_phase " << name() << std::endl;

    uvm::uvm_monitor::build_phase(phase);

    uvm::uvm_config_db<bool>::get(this, "*", "checks_enable", checks_enable);
    uvm::uvm_config_db<bool>::get(this, "*", "coverage_enable", coverage_enable);
  }

  void connect_phase(uvm::uvm_phase &phase) {
    if (!uvm_config_db<dti_vif*>::get(this, "*", "vif", vif))
      UVM_FATAL(this->name(), "Virtual interface not defined! Simulation aborted!");
  }

  void run_phase( uvm::uvm_phase& phase )
  {
    dti_packet p;

    while (true) // monitor forever
    {
      sc_core::wait( vif->dti_data.default_event() ); // wait for input changes
      p.data = vif->dti_data.read();
      std::cout << sc_core::sc_time_stamp() << ": " << name() << " received " << p.data << std::endl;
      item_collected_port.write(p);

      if(checks_enable) { std::cout << "no checks yet" << std::endl; }
      if(coverage_enable) { std::cout << "no checks yet" << std::endl; }
    }
  }
};


#endif
