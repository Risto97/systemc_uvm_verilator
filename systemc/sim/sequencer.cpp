#include "sequencer.hpp"
#include <tlm_utils/tlm_quantumkeeper.h>

using namespace sc_core;
using namespace sc_dt;
using namespace tlm;

SC_HAS_PROCESS(sequencer);

sequencer::sequencer(sc_module_name name) :
  sc_module(name),
  isoc("isoc")
{
	SC_THREAD(test);
}

void sequencer::test()
{
	tlm_generic_payload pl;
	sc_time loct;
	tlm_utils::tlm_quantumkeeper qk;

	for (int i = 0; i != 10; ++i)
    {
      sc_uint<8> val = i+1;
      pl.set_address(0);
      pl.set_command(TLM_WRITE_COMMAND);
      pl.set_data_length(1);
      pl.set_data_ptr((unsigned char*)&val);

      isoc->b_transport(pl, loct);
      qk.set_and_sync(loct);
      // msg(pl);

      loct += sc_time(5, SC_NS);
    }
}
