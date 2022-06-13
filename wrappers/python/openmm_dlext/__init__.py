# flake8: noqa:F401

from .dlpack_extension import ContextView, DeviceType
from .dlpack_extension import Force as _Force
from .dlpack_extension import (
    IdsOrdering,
    atom_ids,
    forces,
    inverse_masses,
    positions,
    register_plugin,
    velocities,
)
from .dlpack_extension_swig import Force, _to_capsule


class Force(Force):
    def __init__(self):
        super(Force, self).__init__()
        self.__alt__ = _Force(_to_capsule(self))

    def is_present_in(self, system):
        return self.__alt__.is_present_in(_to_capsule(system))

    def add_to(self, context):
        system = context.getSystem()
        if not self.thisown and not self.is_present_in(system):
            raise RuntimeError("Force already added to another system")
        self.__alt__.add_to(_to_capsule(context), _to_capsule(system))
        self.thisown = False

    def set_callback_in(self, context, callback):
        self.__alt__.set_callback_in(_to_capsule(context), callback)

    def view(self, context):
        return self.__alt__.view(_to_capsule(context))


register_plugin()


dlpack_extension = None
dlpack_extension_swig = None
_dlpack_extension = None
_dlpack_extension_swig = None

del register_plugin
del dlpack_extension
del dlpack_extension_swig
del _dlpack_extension
del _dlpack_extension_swig
