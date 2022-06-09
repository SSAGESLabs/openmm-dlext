# OpenMM-dlext

The plugin is intended to provide a `OpenMM::Force` derived class that does no direct
computation, but instead uses [DLPack](https://github.com/dmlc/dlpack) wrappers around CPU
or GPU `OpenMM::Platform` data for an initialized instance of `OpenMM::Contex`, and a
callback interface to perform computations on such data outside the plugin.

## Usage

The wrappers around the simulation data are intended to be instantiated after the
`OpenMM::ContextImpl` for a particular simulation has been created.
