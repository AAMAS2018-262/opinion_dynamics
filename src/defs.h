#pragma once

#include <iostream>

// helper macro

#define OD_NOP do { } while (0)

// printing macros

#ifdef NDEBUG
#define Debug(x) OD_NOP
#else
#define Debug(x) do { std::cout << x << std::endl; } while (0)
#endif

#if defined(NVERBOSE) && defined(NDEBUG)
#define Print(x) OD_NOP
#else
#define Print(x) do { std::cout << x << std::endl; } while (0)
#endif

#define Error(x) do { std::cerr << "Error: " << x << std::endl;\
                      std::exit(EXIT_FAILURE); } while (0)

// assert macro

#ifdef NDEBUG
# undef NDEBUG
# include <cassert>
# define NDEBUG
# define debug_assert(...) OD_NOP
#else
# include <cassert>
# define debug_assert(...) assert(__VA_ARGS__)
#endif
