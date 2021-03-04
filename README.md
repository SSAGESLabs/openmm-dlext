OpenMM-dlext
------------

The plugin is intended to provide a `OpenMM::Force` derived class that does no direct
computation, but instead uses [DLPack](https://github.com/dmlc/dlpack) wrappers around CPU
or GPU `OpenMM::Platform` data for an initialized instance of `OpenMM::Contex`, and a
callback interface to perform computations on such data outside the plugin.

## Usage

The wrappers around the simulation data are intended to be instantiated after the
`OpenMM::ContextImpl` for a particular simulation has been created, and can be created by
passing the initialized simulation context to the constructor of the provided
`SimulationView` class. Once they have been created, the `DLExtForce` can also be added to
the set of forces of the appropriate `OpenMM::System`.

Given that forces are instantiated after `OpenMM::ContextImpl` creation, the original
`OpenMM::Context`, needs to be reintialized before perfoming any computation that is
intended to make use of the plugin.
