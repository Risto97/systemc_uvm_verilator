#include <iostream>
#include <stdlib.h>

#include "obj_dir/Vwrap_ii_gen.h"
#include "top_env.hpp"
#include <cstdlib>
#include <verilated.h>
#include "systemc.h"


int sc_main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv);
  TOP_ENV<Vwrap_ii_gen> top_obj("aaaa", "mod1", 1000);

  top_obj.open_trace("top.vcd");
  top_obj.start_sim();


  top_obj.close_trace();
  exit(EXIT_SUCCESS);
}
