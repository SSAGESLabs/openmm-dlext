from .dlpack_extension import (
    ContextView,
    DeviceType,
    IdsOrdering,
    atom_ids,
    forces,
    inverse_masses,
    positions,
    velocities
)

from .dlpack_extension_swig import Force

from .dlpack_extension import Force as _Force
from .dlpack_extension_swig import Convert_toCapsule as _to_capsule


class Force(Force):
    def __init__(self):
        super(Force, self).__init__()
        self.__alt__ = _Force(_to_capsule(self))

    def add_to(self, context, system):
        self.__alt__.add_to(_to_capsule(context), _to_capsule(system))

    def set_callback_in(self, context, callback):
        self.__alt__.set_callback_in(_to_capsule(context), callback)

    def view(self, context):
        return self.__alt__.view(_to_capsule(context))


dlpack_extension = None
dlpack_extension_swig = None
to_capsule = None
to_force = None
_dlpack_extension = None
_dlpack_extension_swig = None

del dlpack_extension
del dlpack_extension_swig
del to_capsule
del to_force
del _dlpack_extension
del _dlpack_extension_swig
