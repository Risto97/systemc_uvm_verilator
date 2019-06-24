#ifndef DTI_AGENT_HPP_
#define DTI_AGENT_HPP_

#include <systemc>
#include <tlm.h>
#include <uvm>

#include "dti_packet.hpp"
#include "dti_driver.hpp"
#include "dti_sequencer.hpp"
#include "dti_monitor.hpp"


class dti_agent : public uvm::uvm_agent
{
public:
  dti_sequencer<dti_packet>* sequencer;
  dti_driver<dti_packet>* driver;
  dti_monitor* monitor;

  UVM_COMPONENT_UTILS(dti_agent);

  dti_agent(uvm::uvm_component_name name)
    : uvm_agent(name),
      driver(0),
      sequencer(0),
      monitor(0)
  {
    std::cout << sc_core::sc_time_stamp() << ": constructor " << name << std::endl;
  }
  void build_phase(uvm::uvm_phase& phase)
  {
    std::cout << sc_core::sc_time_stamp() << ": build_phase " << name() << std::endl;

    uvm::uvm_agent::build_phase(phase);

    if (get_is_active() == uvm::UVM_ACTIVE)
      {
        UVM_INFO(get_name(), "is set to UVM_ACTIVE", UVM_NONE);

        sequencer = dti_sequencer<dti_packet>::type_id::create("sequencer", this);
        assert(sequencer);

        driver = dti_driver<dti_packet>::type_id::create("driver", this);
        assert(driver);
      }
    else
      UVM_INFO(get_name(), "is set to UVM_PASSIVE", UVM_NONE);

    monitor = dti_monitor::type_id::create("monitor", this);
    assert(monitor);
  }

  void connect_phase(uvm::uvm_phase& phase)
  {
    std::cout << sc_core::sc_time_stamp() << ": connect_phase " << name() << std::endl;

    if (get_is_active() == uvm::UVM_ACTIVE)
      {
        driver->seq_item_port.connect(sequencer->seq_item_export);
      }
  }


};


#endif
