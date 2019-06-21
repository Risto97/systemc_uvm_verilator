import os
from pygears.conf.registry import registry, bind
import cascade_classifier.pygears_impl.design as design_dir

package_dir = os.path.dirname(design_dir.__file__)

svlib_dir = os.path.join(
    package_dir, 'gears', 'svlib')
registry('svgen/sv_paths').append(svlib_dir)

bind('svgen/debug_intfs', ['*'])