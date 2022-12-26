#pragma once

#include <string>

typedef std::int8_t i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;

typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

constexpr u32 PORT_FAILURE = 0xffffffff;

u32 kernmod(const u8 bus, const u8 device, const u8 func, const u8 pcireg = 0);
bool kernfail(void);
