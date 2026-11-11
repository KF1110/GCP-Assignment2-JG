#ifndef _SDL3_PRINT_HPP_
#define _SDL3_PRINT_HPP_

// std::print and std::println, on a compiler that has not got them yet.
//
// C++23 added <print>, and it is how a line of output should be written:
//
//   std::println("camera: at ({}, {}, {})", x, y, z);
//
// rather than the stream of << that the same line needs otherwise.  Each {} is
// a hole for the next argument, and the argument's own type decides how it is
// printed - no manipulators, nothing to set and then set back.  A {} can say
// more if it has to: {:.2f} for two decimal places, {:>8} to pad to a width of
// eight.  What it will not do is read a format string that does not match the
// arguments: that is checked when the program is compiled, which is the part
// printf never managed.
//
// This is C++20, so <print> is not there.  What is here instead is a shim onto
// {fmt} - deps/fmt-12.2.0 - which is the library std::print was standardised
// from, and is still where its implementations come from.  So the two spell
// the same things the same way, and a program written against this one needs
// no changes when the compiler catches up.  That is what the test below is
// for: where the real <print> exists, it is used and nothing here is defined.
//
// One honest caveat for the curious: the standard says a program may not add
// declarations to namespace std, and this does.  It is how a shim of this kind
// is always done, and it is safe here because the name being added is exactly
// the one the standard will later define, with the same meaning - but it is
// worth knowing that it is a liberty rather than a technique.
//
// fmt is used header-only, so there is nothing to build or link: including
// this file is the whole of it.  A program that printed a great deal might
// compile {fmt} once into a library of its own instead.

#include <version> // __cpp_lib_print, where the library says what it has

#if defined(__cpp_lib_print)

#include <print> // the real thing; nothing below is needed

#else

#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY 1
#endif

#include <fmt/format.h> // fmt::print, fmt::println, fmt::format_string
#include <fmt/std.h>    // {} for standard types, such as std::thread::id
#include <cstdio>       // FILE, stdout, stderr
#include <utility>      // std::forward

namespace std
{

// The four the standard has, with the standard's own signatures.  The format
// string is a type that can only be built from a compile-time constant, which
// is what lets the checking happen then rather than at run time.
template <class... Args>
void print(fmt::format_string<Args...> f, Args&&... args)
{
  fmt::print(f, std::forward<Args>(args)...);
}

template <class... Args>
void print(FILE* stream, fmt::format_string<Args...> f, Args&&... args)
{
  fmt::print(stream, f, std::forward<Args>(args)...);
}

template <class... Args>
void println(fmt::format_string<Args...> f, Args&&... args)
{
  fmt::println(f, std::forward<Args>(args)...);
}

template <class... Args>
void println(FILE* stream, fmt::format_string<Args...> f, Args&&... args)
{
  fmt::println(stream, f, std::forward<Args>(args)...);
}

// ...and the one that prints nothing but the newline
inline void println() { fmt::print("\n"); }

inline void println(FILE* stream) { fmt::print(stream, "\n"); }

} // namespace std

#endif // __cpp_lib_print

#endif // _SDL3_PRINT_HPP_
