
#include "dti_driver.hpp"

using namespace sc_core;
using namespace tlm;
using namespace std;
using namespace sc_dt;

dti_drv::dti_drv(sc_module_name name) :
  sc_module(name),
  soc("soc")
{
  soc.register_b_transport(this, &dti_drv::b_transport);
}

void dti_drv::b_transport(pl_t& pl, sc_time& offset)
{
	tlm_command    cmd  = pl.get_command();
	uint64         addr = pl.get_address();
	unsigned char* data = pl.get_data_ptr();

	if (addr < 0x10)
    {
      switch(cmd)
        {
        case TLM_WRITE_COMMAND:
          {
            val = *((sc_uint<8>*)pl.get_data_ptr());
            pl.set_response_status( TLM_OK_RESPONSE );
            msg(pl);
            break;
          }
        case TLM_READ_COMMAND:
          {
            *data = (unsigned char) val;
            pl.set_response_status( TLM_OK_RESPONSE );
            msg(pl);
            break;
          }
        default:
          pl.set_response_status( TLM_COMMAND_ERROR_RESPONSE );
          SC_REPORT_ERROR("GPIO", "TLM bad command");
        }
    }
	else
		SC_REPORT_WARNING("GPIO", "TLM wrong address");
	offset += sc_time(3, SC_NS);
}


void dti_drv::msg(const pl_t& pl)
{
  stringstream ss;
	ss << hex << pl.get_address();
	sc_uint<8> val = *((sc_uint<8>*)pl.get_data_ptr());
	string cmd  = pl.get_command() == TLM_READ_COMMAND ? "read  " : "write ";

	string msg = cmd + "val: " + to_string((int)val) + " adr: " + ss.str();
	msg += " @ " + sc_time_stamp().to_string();

	SC_REPORT_INFO("GPIO", msg.c_str());
}
