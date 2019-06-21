export SYSTEMC ?= /tools/systemc/
export SYSTEMC_ARCH = linux64

TOP_MODULE = wrap_ii_gen
TB_FN = tb.cpp
VERILATOR_FLAGS = -cc -CFLAGS -fpic -LDFLAGS -Wno-fatal
TRACE_FLAGS = --trace -no-trace-params --trace-structs
SYSTEMC_INCL = -I/tools/systemc/include
SVLIB = -I/tools/git_repos/pygears/pygears/common/svlib -I/tools/git_repos/pygears/pygears/cookbook/svlib -I/tools/work/cascade_classifier/cascade_classifier/pygears_impl/design/gears/svlib

default: clean
	-verilator $(VERILATOR_FLAGS) $(SYSTEMC_INCL) -Ibuild/svlib $(SVLIB) -clk clk --top-module $(TOP_MODULE) $(TRACE_FLAGS) build/*.sv dti.sv --exe $(TB_FN)

run: default
	make -j -C obj_dir/ -f V$(TOP_MODULE).mk V$(TOP_MODULE)
	obj_dir/V$(TOP_MODULE)

wave: run
	gtkwave top.vcd

clean:
	-rm -rf obj_dir
	-rm top.vcd
