#ifndef DTI_DRIVER_HPP_
#define DTI_DRIVER_HPP_

#include <systemc>
#include <tlm.h>
#include <uvm>

#include "dti_vif.hpp"
#include "dti_packet.hpp"

enum Mode_T {producer = 0, consumer = 1};

template <class REQ>
class dti_driver : public uvm::uvm_driver<REQ>
{
public:
  dti_vif* vif;
  Mode_T mode;

  dti_driver( uvm::uvm_component_name name) :
    uvm::uvm_driver<REQ>(name)
  {}

  UVM_COMPONENT_PARAM_UTILS(dti_driver<REQ>);

  void build_phase(uvm::uvm_phase& phase){
    std::cout << sc_core::sc_time_stamp() << ": build_phase " << this->name() << std::endl;

    uvm_driver<REQ>::build_phase(phase);

    if(!uvm::uvm_config_db<Mode_T>::get(this, "*", "mode", mode))
      UVM_FATAL(this->name(), "Mode not defined! Simulation aborted!");
  }

  void connect_phase(uvm::uvm_phase &phase) {
    if (!uvm_config_db<dti_vif*>::get(this, "*", "vif", vif))
      UVM_FATAL(this->name(), "Virtual interface not defined! Simulation aborted!");
  }

  void main_phase(uvm::uvm_phase& phase)
  {
    std::cout << sc_core::sc_time_stamp() << ": " << this->name() << " " << phase.get_name() << "..." << std::endl;

    REQ req, rsp;

    while(true) // execute all sequences
      {
        this->seq_item_port->get_next_item(req);

        if(mode == producer)
          drive_transfer_producer(req);
        else if(mode == consumer)
          drive_transfer_consumer(req);

        rsp.set_id_info(req);
        this->seq_item_port->item_done();
        this->seq_item_port->put_response(rsp);
      }
  }

  void drive_transfer_producer(const REQ& p)
  {
    std::cout << sc_core::sc_time_stamp() << ": " << this->name() << " sending value " << p.data << std::endl;
    vif->dti_data.write(p.data);
    vif->dti_valid = 1;
    do{
      sc_core::wait(vif->clk->posedge_event());
    }
    while(vif->dti_ready == 0);
  }

  void drive_transfer_consumer(const REQ& p)
  {
    std::cout << sc_core::sc_time_stamp() << ": " << this->name() << " sending value " << p.data << std::endl;
    vif->dti_ready = 1;
    sc_core::wait(vif->clk->posedge_event());
  }
};

#endif
