#ifndef SEQUENCER_HPP
#define SEQUENCER_HPP

#include <systemc>
#include <tlm>
#include <string>
#include <tlm_utils/simple_initiator_socket.h>

class sequencer :
  public sc_core::sc_module
{
public:
  sequencer(sc_core::sc_module_name);

	tlm_utils::simple_initiator_socket<sequencer> isoc;

protected:
	sc_dt::sc_uint<8> val;

	void test();

	typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
	void b_transport(pl_t&, sc_core::sc_time&);
};

#endif
