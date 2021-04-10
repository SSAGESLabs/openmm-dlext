from .dlpack_extension_swig import Force as SwigForce
from .dlpack_extension_swig import Convert_toCapsule as to_capsule

from .dlpack_extension import (
    ContextView,
    DeviceType,
    DLType,
    Force,
    IdsOrdering,
    positions,
    velocities,
    forces,
    atom_ids,
    inverse_masses
)
