#ifndef DTI_DRIVER_HPP
#define DTI_DRIVER_HPP

#include "systemc.h"
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <sstream>

// template <class data_T> struct dti_vif {
//   bool dti_valid;
//   bool dti_ready;
//   data_T dti_data;
// };

class dti_drv :
  public sc_core::sc_module
{
public:
  // dti_vif<sc_bv<32> > intf;
  dti_drv(sc_core::sc_module_name);
	tlm_utils::simple_target_socket<dti_drv> soc;

protected:
	sc_dt::sc_uint<8> val;

	typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
	void b_transport(pl_t&, sc_core::sc_time&);
	void msg(const pl_t&);

};

#endif
