// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#include "DLExtForce.h"
#include "DLExtKernelFactory.h"
#include "internal/DLExtForceImpl.h"

#include "openmm/OpenMMException.h"


using namespace DLExt;


bool Force::usesPeriodicBoundaryConditions() const
{
    return uses_periodic_bc;
}

bool Force::isPresentIn(const OpenMM::System& system)
{
    auto force_found = false;
    for (int i = 0; i < system.getNumForces(); ++i)
        if ((force_found = (&system.getForce(i) == this)))
            break;
    return force_found;
}

void Force::addTo(OpenMM::Context& context, OpenMM::System& system)
{
    if (&context.getSystem() != &system)
        throw OpenMM::OpenMMException(
            "The system is different from the one referred by the context"
        );

    auto& platform = context.getPlatform();
    if (!isSupported(platform))
        throw OpenMM::OpenMMException("Unsupported platform");

    // Check if this Force instance is already present in the system
    if (!isPresentIn(system))
        system.addForce(this);

    // Check if this Force instance has already created a ForceImpl within the ContextImpl
    auto& ctximpl = getContextImpl(context);
    auto& impls = ctximpl.getForceImpls();
    auto impl_found = false;
    for (const auto* impl : impls)
        if ((impl_found = (&impl->getOwner() == this)))
            break;
    if (!impl_found) {
        impls.push_back(createImpl());
        impls.back()->initialize(ctximpl);
    }
}

void Force::setCallbackIn(OpenMM::Context& context, Function<void, long long>& callback)
{
    auto& impl = reinterpret_cast<ForceImpl&>(getImplInContext(context));
    impl.setCallback(callback);
}

ContextView Force::view(OpenMM::Context& context)
{
    return ContextView(getContextImpl(context));
}

OpenMM::ForceImpl* Force::createImpl() const
{
    // OpenMM deletes each `OpenMM::ForceImpl*` instance within the destructor of
    // `OpenMM::ContextImpl`, so in theory, we shouldn't worry about memory leaks.
    return new ForceImpl(*this);
}
