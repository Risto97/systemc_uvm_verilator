#ifndef DTI_SEQUENCE_HPP_
#define DTI_SEQUENCE_HPP_

#include <systemc>
#include <tlm.h>
#include <uvm>

template <typename REQ = uvm::uvm_sequence_item, typename RSP = REQ>
class dti_sequence : public uvm::uvm_sequence<REQ,RSP>
{
 public:
  dti_sequence( const std::string& name ) : uvm::uvm_sequence<REQ,RSP>( name )
  {
    std::cout << sc_core::sc_time_stamp() << ": constructor " << name << std::endl;
  }

  UVM_OBJECT_PARAM_UTILS(dti_sequence<REQ,RSP>);

  void pre_body()
  {
    // raise objection if started as a root dti_sequence
    if(this->starting_phase != NULL)
      this->starting_phase->raise_objection(this);
  }

  void body()
  {
    REQ* req;
    RSP* rsp;

    UVM_INFO(this->get_name(), "Starting dti_sequence", uvm::UVM_MEDIUM);

    for(int i = 1; i < 10; i++)
    {
      req = new REQ();
      rsp = new RSP();

      req->data = i;
      std::cout << sc_core::sc_time_stamp() << ": " << this->get_full_name() << " start_item value " << i << std::endl;

      this->start_item(req);
      this->finish_item(req);
      this->get_response(rsp);
    }

    UVM_INFO(this->get_name(), "Finishing dti_sequence", uvm::UVM_MEDIUM);
  }

  void post_body()
  {
    // drop objection if started as a root dti_sequence
    if(this->starting_phase != NULL)
      this->starting_phase->drop_objection(this);
  }

};


#endif
