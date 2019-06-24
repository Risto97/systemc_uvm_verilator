//----------------------------------------------------------------------
//   Copyright 2014 NXP B.V.
//   Copyright 2007-2010 Mentor Graphics Corporation
//   Copyright 2007-2010 Cadence Design Systems, Inc.
//   Copyright 2010-2011 Synopsys, Inc.
//   All Rights Reserved Worldwide
//
//   Licensed under the Apache License, Version 2.0 (the
//   "License"); you may not use this file except in
//   compliance with the License.  You may obtain a copy of
//   the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in
//   writing, software distributed under the License is
//   distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//   CONDITIONS OF ANY KIND, either express or implied.  See
//   the License for the specific language governing
//   permissions and limitations under the License.
//----------------------------------------------------------------------

#include <systemc>
#include <uvm>
#include <verilated.h>
#include "Vwrap_ii_gen.h"
// #include "top_env.hpp"

#include "top.hpp"

using namespace uvm;

int sc_main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv);
  // TOP_ENV<Vwrap_ii_gen> top_obj("aaaa", "mod1", 10);

  uvm::uvm_config_db<int>::set(NULL, "top.producer1", "num_packets", 2);
  uvm::uvm_config_db<int>::set(NULL, "top.producer2", "num_packets", 4);
  uvm::uvm_config_db<int>::set(NULL, "*", "recording_detail", uvm::UVM_LOW);

  top<Vwrap_ii_gen> mytop("top", "v_top", 100);
  // top_obj.open_trace("top.vcd");

  uvm_default_printer->knobs.reference = 0;

  uvm::uvm_root::get()->print_topology();

  run_test();
  // top_obj.start_sim();

  // top_obj.close_trace();

  return 0;
}
