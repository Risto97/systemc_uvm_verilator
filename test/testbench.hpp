#ifndef testbench_H
#define testbench_H

#include "verilated_vcd_c.h"
#include <iostream>
#include <stdint.h>
#include <stdio.h>

template <class MODULE> class TESTBENCH {
public:
  unsigned long m_tick_count;
  unsigned long m_tick_end;
  MODULE *m_core;
  VerilatedVcdC *m_trace;

  TESTBENCH(unsigned long tick_end) {
    m_core = new MODULE;
    m_tick_count = 0l;
    m_tick_end = tick_end;
    Verilated::traceEverOn(true);
  }

  virtual ~TESTBENCH(void) {
    close_trace();
    delete m_core;
    m_core = NULL;
  }

  virtual void open_trace(const char *vcdname) {
    if (!m_trace) {
      m_trace = new VerilatedVcdC;
      m_core->trace(m_trace, 99);
      m_trace->open(vcdname);
    }
  }

  virtual void close_trace(void) {
    if (m_trace) {
      m_trace->close();
      m_trace = NULL;
    }
  }

  virtual void reset(void) {
    m_core->rst = 1;
    this->tick();
    m_core->eval();
    this->tick();
    m_core->eval();
    m_core->rst = 0;
  }

  virtual void tick(void) {
    m_tick_count++;

    m_core->clk = 0;
    m_core->eval();

    if (m_trace)
      m_trace->dump(10 * m_tick_count - 9);

    m_core->clk = 1;
    m_core->eval();

    if (m_trace)
      m_trace->dump(10 * m_tick_count);

    m_core->clk = 0;
    m_core->eval();
    if (m_trace) {
      m_trace->dump(10 * m_tick_count + 5);
      m_trace->flush();
    }
    if (m_tick_count == m_tick_end)
      Verilated::gotFinish(true);
  }

  virtual bool done(void) { return (Verilated::gotFinish()); }
};

#endif
