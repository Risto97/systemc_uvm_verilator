#include <iostream>
#include <stdlib.h>

#include <bitset>

#include "obj_dir/Vwrap_ii_gen.h"
#include "testbench.hpp"
#include <cstdlib>
#include <verilated.h>

int main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv);
  TESTBENCH<Vwrap_ii_gen> *tb = new TESTBENCH<Vwrap_ii_gen>(100);
  Vwrap_ii_gen *top = tb->m_core;

  int i = 1;
  int j = 0;
  std::bitset<8> din_data = 0;
  std::bitset<2> din_eot = 0;
  std::bitset<10> din = 0;

  tb->open_trace("top.vcd");

  tb->reset();
  tb->m_core->ii_s_ready = 1;
  while (!tb->done()) {
    tb->tick();

    din_data = std::bitset<8>(i);

    if (i == 5) {
      din_eot[0] = 1;
      if (j == 4) {
        din_eot[1] = 1;
        j = 0;
      } else {
        din_eot[1] = 0;
        j++;
      }
      i = 0;
    } else {
      din_eot[0] = 0;
    }

    din =
        std::bitset<10>((std::bitset<10>(din_data.to_ulong()) |
                         std::bitset<10>(din_eot.to_ulong() << din_data.size()))
                            .to_ulong());

    top->din_data = din.to_ulong();
    top->din_valid = 1;
    i++;
  }

  exit(EXIT_SUCCESS);
}
