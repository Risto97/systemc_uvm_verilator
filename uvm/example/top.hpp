#ifndef TOP_HPP_
#define TOP_HPP_

#include "verilated_vcd_sc.h"
#include <systemc>
#include <uvm>

#include "dti_vif.hpp"
#include "packet.hpp"
#include "dti_agent.hpp"
#include "dti_sequence.hpp"

template <class MODULE> class top : public uvm::uvm_env {
public:
  MODULE dut;
  VerilatedVcdSc *m_trace;

  dti_agent* din_agent;
  dti_agent* dout_agent;
  // scoreboard* scoreboard0;

  sc_core::sc_time T;
  sc_core::sc_time Tsim;

  sc_clock clk;
  sc_signal<bool> rst;

  dti_vif *din_vif;
  dti_vif *dout_vif;

  UVM_COMPONENT_UTILS(top);

  /* clang-format off */
  top(uvm::uvm_component_name name, const char *dut_name, unsigned long Ncycle, const char *vcdname) :
    uvm::uvm_env(name),
    dut(dut_name),
    T(10, SC_NS),
    Tsim(T * Ncycle),
    clk("clk", T),
    rst("rst")
  {
    /* clang-format on */

    m_trace = new VerilatedVcdSc;
    Verilated::traceEverOn(true);
    open_trace(vcdname);

    dut.clk(clk);
    dut.rst(rst);

    din_vif = new dti_vif(&clk, &rst);
    dout_vif = new dti_vif(&clk, &rst);

    dut.din_ready(din_vif->dti_ready);
    dut.din_valid(din_vif->dti_valid);
    dut.din_data(din_vif->dti_data);
    dut.dout_valid(dout_vif->dti_valid);
    dut.dout_ready(dout_vif->dti_ready);
    dut.dout_data(dout_vif->dti_data);
  }

  void build_phase(uvm::uvm_phase& phase)
  {
    std::cout << sc_core::sc_time_stamp() << ": build_phase " << name() << std::endl;

    uvm::uvm_env::build_phase(phase);
    // uvm_config_db_options::turn_on_tracing();

    din_agent = dti_agent::type_id::create("din_agent", this);
    assert(din_agent);

    dout_agent = dti_agent::type_id::create("dout_agent", this);
    assert(dout_agent);

    // scoreboard0 = scoreboard::type_id::create("scoreboard0", this);
    // assert(scoreboard0);

    uvm::uvm_config_db<int>::set(this, "din_agent", "is_active", uvm::UVM_ACTIVE);
    uvm::uvm_config_db<Mode_T>::set(this, "din_agent.*", "mode", producer);

    uvm::uvm_config_db<int>::set(this, "dout_agent", "is_active", uvm::UVM_ACTIVE);
    uvm::uvm_config_db<Mode_T>::set(this, "dout_agent.*", "mode", consumer);

    uvm::uvm_config_db<dti_vif *>::set(0, "top.din_agent.*", "vif", din_vif);
    uvm::uvm_config_db<dti_vif *>::set(0, "top.dout_agent.*", "vif", dout_vif);

    uvm::uvm_config_db<uvm_object_wrapper*>
      ::set(this,"din_agent.sequencer.run_phase","default_sequence",
            dti_sequence<dti_packet>::type_id::get());

    uvm::uvm_config_db<uvm_object_wrapper*>
      ::set(this,"dout_agent.sequencer.run_phase","default_sequence",
            dti_sequence<dti_packet>::type_id::get());
  }


  void reset_phase(uvm::uvm_phase &phase) {
    phase.raise_objection(this);
    reset();
    phase.drop_objection(this);
  }

  void final_phase(uvm::uvm_phase &phase) {
    std::cout << sc_time_stamp() << ": " << get_full_name()
              << " phase: " << phase.get_name() << std::endl;
    close_trace();
  }

  virtual void main_phase(uvm::uvm_phase &phase) {
    phase.raise_objection(this);
    sc_core::wait(Tsim); // 1ms
    phase.drop_objection(this);
  }

  virtual void reset(void) {
    rst = 1;
    sc_core::wait(2 * T);
    rst = 0;
  }

  virtual void open_trace(const char *vcdname) {
    dut.trace(m_trace, 10);
    m_trace->open(vcdname);
  }

  virtual void close_trace(void) {
    m_trace->close();
    m_trace = NULL;
  }
};

#endif /* TOP_HPP_ */