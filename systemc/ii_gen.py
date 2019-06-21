from cascade_classifier.pygears_impl.design import ii_gen
from cascade_classifier.pygears_impl.scripts.svlib_utils import copy_svlib

from pygears.typing import Queue, Uint
from pygears.sim.modules import drv
from pygears.common import shred
from pygears.svgen import svgen
from pygears.conf.registry import bind

from string import Template
signal_spy_connect_t = Template("""
/*verilator tracing_on*/
${intf_name}_t ${intf_name}_data;
logic ${intf_name}_valid;
logic ${intf_name}_ready;
/*verilator tracing_off*/

assign ${intf_name}_data = ${conn_name}.data;
assign ${intf_name}_valid = ${conn_name}.valid;
assign ${intf_name}_ready = ${conn_name}.ready;
""")

din_t = Queue[Uint[8], 2]

ii_gen(din=drv(t=din_t, seq=[]), frame_size=(5, 5))

# bind('svgen/debug_intfs', [''])
bind('svgen/spy_connection_template', signal_spy_connect_t)
svgen('/ii_gen', outdir="build", wrapper=True)
copy_svlib()