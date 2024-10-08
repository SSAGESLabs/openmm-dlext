# SPDX-License-Identifier: MIT
# See LICENSE.md and CONTRIBUTORS.md at https://github.com/SSAGESLabs/openmm-dlext

# flake8: noqa:F401

import importlib.util
import pathlib

from .api_swig import Force, _to_capsule


def _load_api(pattern):
    path = next(pathlib.Path(__file__).parent.glob(pattern))
    spec = importlib.util.spec_from_file_location("_api", path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


try:
    _api = _load_api("_api.*")
except:
    _api = _load_api("_api_fallback.*")

ContextView = _api.ContextView
DeviceType = _api.DeviceType
IdsOrdering = _api.IdsOrdering
atom_ids = _api.atom_ids
forces = _api.forces
inverse_masses = _api.inverse_masses
positions = _api.positions
register_plugin = _api.register_plugin
velocities = _api.velocities


class Force(Force):
    def __init__(self):
        super(Force, self).__init__()
        self.__alt__ = _api.Force(_to_capsule(self))

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


# Clean up namespace
importlib = None
pathlib = None
api_swig = None

del _load_api
del register_plugin

del api_swig
del pathlib
del importlib
