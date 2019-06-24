#include <systemc>
#include <uvm>
#include <verilated.h>
#include "Vwrap_ii_gen.h"

#include "top.hpp"

using namespace uvm;

int sc_main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv);

  uvm::uvm_config_db<int>::set(NULL, "*", "recording_detail", uvm::UVM_LOW);

  top<Vwrap_ii_gen> mytop("top", "v_top", 10, "top.vcd");

  uvm_default_printer->knobs.reference = 0;

  uvm::uvm_root::get()->print_topology();

  run_test();

  return 0;
}
