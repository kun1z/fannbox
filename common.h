#pragma once
//----------------------------------------------------------------------------------------------------------------------
#include <assert.h>
#include <limits.h>
static_assert(CHAR_BIT == 8, "This code requires [char] to be exactly 8 bits.");
static_assert(sizeof(int) >= 4, "This code requires [int] to be at least 32 bits.");
//----------------------------------------------------------------------------------------------------------------------
#include <stdint.h>
typedef unsigned int ui; typedef int si;
typedef unsigned char u8; typedef char s8;
typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
typedef uint64_t u64; typedef int64_t s64;
typedef float r32; typedef double r64;
typedef __uint128_t u128; typedef __int128_t s128;
//----------------------------------------------------------------------------------------------------------------------
#include <byteswap.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "doublefann.h"
#include "parallel_fann.h"
//----------------------------------------------------------------------------------------------------------------------
struct digit
{
    r64 pixel[28][28];
    ui digit;
};
struct digit * load_digits(ui * const, s8 const * const, s8 const * const);
u64 tick(void);
void print_digit(const r64[28][28]);
//----------------------------------------------------------------------------------------------------------------------