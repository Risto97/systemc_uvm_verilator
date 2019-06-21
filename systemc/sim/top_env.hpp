#ifndef TESTBENCH_H
#define TESTBENCH_H

#include "systemc.h"
#include "verilated_vcd_sc.h"
#include <stdint.h>
#include <stdio.h>
#include <verilated.h>
#include <cstdlib>

template <class MODULE> class TOP_ENV : sc_core::sc_module {
public:
  MODULE top_v;
  VerilatedVcdSc *m_trace;
  sc_core::sc_time T;
  sc_core::sc_time Tsim;

  /* Signal Declarations
   */

  sc_clock clk;
  sc_signal<bool> rst;

  sc_signal<bool> din_ready;
  sc_signal<bool> din_valid;
  sc_signal<uint32_t> din_data;

  sc_signal<bool> ii_s_valid;
  sc_signal<bool> ii_s_ready;
  sc_signal<uint32_t> ii_s_data;

  /// ***************************************//////////

  /* clang-format off */

  SC_HAS_PROCESS(TOP_ENV);
  TOP_ENV(sc_module_name nm_top, const char *nm_mod, unsigned long Ncycle)
    : sc_module(nm_top),
      top_v(nm_mod),
      T(10, SC_NS),
      Tsim(T * Ncycle),
      clk("clk", T),
      rst("rst"),
      din_ready("din_ready"),
      din_valid("din_valid"),
      din_data("din_data"),
      ii_s_valid("ii_s_valid"),
      ii_s_ready("ii_s_ready"),
      ii_s_data("ii_s_data")
  {
    din_valid = 1;
    ii_s_ready = 1;
    din_data = 5;

    top_v.clk(clk);
    top_v.rst(rst);
    top_v.din_ready(din_ready);
    top_v.din_valid(din_valid);
    top_v.din_data(din_data);
    top_v.ii_s_valid(ii_s_valid);
    top_v.ii_s_ready(ii_s_ready);
    top_v.ii_s_data(ii_s_data);
    m_trace = new VerilatedVcdSc;


    Verilated::traceEverOn(true);
  }
  /* clang-format on */

  ~TOP_ENV() { close_trace(); }

  virtual void reset(void) {
    rst = 1;
    sc_start(10 * T);
    rst = 0;
  }

  virtual void start_sim(void) {
    reset();
    sc_start(Tsim);
  }

  virtual void open_trace(const char *vcdname) {
    top_v.trace(m_trace, 10);
    m_trace->open(vcdname);
  }

  virtual void close_trace(void) {
    m_trace->close();
    m_trace = NULL;
  }

  virtual bool done(void) { return (Verilated::gotFinish()); }
};

#endif
