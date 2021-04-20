from .dlpack_extension import (
    ContextView,
    DeviceType,
    IdsOrdering,
    atom_ids,
    forces,
    inverse_masses,
    positions,
    register_plugin,
    velocities
)

from .dlpack_extension_swig import (
    Force,
    _to_capsule
)

from .dlpack_extension import Force as _Force


class Force(Force):
    def __init__(self):
        super(Force, self).__init__()
        self.__alt__ = _Force(_to_capsule(self))

    def add_to(self, context):
        if self.thisown == 0:
            raise RuntimeError("Force already assigned to a System or Context")
        system = context.getSystem()
        self.__alt__.add_to(_to_capsule(context), _to_capsule(system))
        self.thisown = 0

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
