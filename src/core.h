#pragma once
#include <raylib.h>
#include <windows.h>
//
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <thread>
#include <string>
#include <string_view>
#include <vector>
#include <type_traits>
#include <cassert>


// Builtins
void SETUP();
void UPDATE();
void RENDER();
void CLEAN();

#define  DRAWER()   BeginDrawing(); ClearBackground(win_clr);

# define  log   std::cerr << 
# define  Module namespace
# define foreach for

//  expression --> stringifyed type of expression
#define typeof(_expr)  decltype(_expr)
#define ttos(_expr)  type_name<decltype(_expr)>()
#define strType(_typen)   type_name<_typen>()


#define stringify(x) #x
#define merge(a, b) a##b

// type-safe Macro for converting anything to str
#define tostr(x) ([](auto&& val) -> std::string {             \
    using T = std::decay_t<decltype(val)>;                    \
    if constexpr (std::is_same_v<T, char>)                   \
        return std::string(1, val);                           \
    else                                                      \
        return std::to_string(val);                           \
})(x)


#define  is_null(_ch_or_int)   ((_ch_or_int) == 0)



// type aliases
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using int128 = __int128_t;
//
using uchar = unsigned char;
using zchar = unsigned char;
//
using uint8 = unsigned int8;
using uint16 = unsigned int16;
using uint = unsigned int;
using uint32 = unsigned int;  // =uint
using uint64 = unsigned long long;
using uint128 = __uint128_t;
//
using f16 = _Float16;
using f32 = float;
using f = float;
using f64 = double;
using f128 = __float128;
//
using std::string;
using ustring = std::basic_string<unsigned char>;
using strview = std::string_view;
using std::array;
template<typename T> using stdlist = std::initializer_list<T>;
//
using duration = std::chrono::milliseconds;
using timepoint = std::chrono::_V2::system_clock::time_point;



