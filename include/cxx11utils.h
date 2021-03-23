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

#define GET_MEMBER(object, member_ptr) ((object).*(member_ptr))
#define GET_PTR_MEMBER(ptr, member_ptr) ((*ptr).*(member_ptr))


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

template <typename R, typename... Ts>
using Function = std::function<R(Ts...)>;

//
//  Functions
//
template <typename T>
constexpr void maybe_unused(T&&) { }

// `make_unique` for C++11
template <typename T, typename ...Args>
UPtr<T> make_unique(Args&& ...args)
{
    return UPtr<T>(new T(std::forward<Args>(args)...));
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


}  // namespace cxx11utils


#endif  // CXX11_UTILS_H_
