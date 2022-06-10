// SPDX-License-Identifier: MIT
// This file is part of `openmm-dlext`, see LICENSE.md

#ifndef CXX11_UTILS_H_
#define CXX11_UTILS_H_

#if !defined(DEFAULT_VISIBILITY)
#if defined(WIN32) || defined(_WIN32)
#define DEFAULT_VISIBILITY __declspec(dllexport)
#else
#define DEFAULT_VISIBILITY __attribute__((visibility("default")))
#endif
#endif

#include <functional>
#include <memory>
#include <type_traits>
#include <unordered_set>

namespace cxx11utils
{

//
//  Aliases
//
template <typename T>
using SPtr = std::shared_ptr<T>;

template <typename T>
using UPtr = std::unique_ptr<T>;

template <typename T>
using Set = std::unordered_set<T>;

template <typename T, typename... Args>
using Function = std::function<T(Args...)>;

template <typename T, typename Object, typename... Args>
using MemberFunction = T (Object::*)(Args...);

template <typename T, typename Object>
using Member = T Object::*;

//
//  Functions
//
template <typename T>
inline void maybe_unused(T&&)
{}

// `make_unique` for C++11
template <typename T, typename... Args>
UPtr<T> make_unique(Args&&... args)
{
    return UPtr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename Object, typename... Args>
T call_member(Object& object, MemberFunction<T, Object, Args...> member, Args&&... args)
{
    return (object.*member)(std::forward<Args>(args)...);
}

template <typename T, typename Object>
T get_member(Object& object, Member<T, Object> member)
{
    return object.*member;
}

template <typename D, typename B>
void assert_is_base_of()
{
    static_assert(std::is_base_of<D, B>::value, "Invalid class passed");
}

// TODO: Check that T can be converted into S
template <typename S, typename T>
bool contains(T& needle, Set<S>& haystack)
{
    return (haystack.find(needle) != haystack.end());
}

} // namespace cxx11utils

#endif // CXX11_UTILS_H_
