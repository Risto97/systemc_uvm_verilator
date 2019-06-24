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

#ifndef TOP_HPP_
#define TOP_HPP_

#include <systemc>
#include <uvm>
#include "verilated_vcd_sc.h"

#include "producer.hpp"
#include "consumer.hpp"
#include "packet.hpp"

template <class MODULE>
class top : public uvm::uvm_component
{
 public:
  MODULE top_v;
  VerilatedVcdSc *m_trace;

  sc_core::sc_time T;
  sc_core::sc_time Tsim;

  sc_clock clk;
  sc_signal<bool> rst;

  sc_signal<bool> din_ready;
  sc_signal<bool> din_valid;
  sc_signal<uint32_t> din_data;

  sc_signal<bool> dout_valid;
  sc_signal<bool> dout_ready;
  sc_signal<uint32_t> dout_data;

  producer<packet> p1;
  producer<packet> p2;
  tlm::tlm_fifo<packet> f;  // there is no uvm_tlm_fifo in UVM-SystemC

  consumer<packet> c;

  UVM_COMPONENT_UTILS(top);

  top( uvm::uvm_component_name name, const char *nm_mod, unsigned long Ncycle)
  : uvm::uvm_component(name),
    top_v(nm_mod),
    T(10, SC_NS),
    Tsim(T * Ncycle),
    clk("clk", T),
    rst("rst"),
    din_ready("din_ready"),
    din_valid("din_valid"),
    din_data("din_data"),
    dout_valid("dout_valid"),
    dout_ready("dout_ready"),
    dout_data("dout_data"),
    p1("producer1"),
    p2("producer2"),
    f("fifo"),
    c("consumer")
  {
    p1.out.connect(c.in);
    p2.out.connect(f); // f.blocking_put_export
    c.out.connect(f);  // f.get_export

    top_v.clk(clk);
    top_v.rst(rst);
    top_v.din_ready(din_ready);
    top_v.din_valid(din_valid);
    top_v.din_data(din_data);
    top_v.dout_valid(dout_valid);
    top_v.dout_ready(dout_ready);
    top_v.dout_data(dout_data);
  }

  virtual void run_phase( uvm::uvm_phase& phase )
  {
     phase.raise_objection(this);
     sc_core::wait(1.0, sc_core::SC_MS); // 1ms
     phase.drop_objection(this);
  }
};

#endif /* TOP_HPP_ */
