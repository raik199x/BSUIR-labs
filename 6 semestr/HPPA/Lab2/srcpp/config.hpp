#pragma once

#include <cstdint>

// lscpu | grep cache -> pick the biggest * instances
// ex: L3 cache:                        8 MiB (2 instances)
// 8 * 1024 * 1024 * 2
constexpr static const std::size_t cache = 8 * 1024 * 1024 * 2;

// must be equal 1 or 2 ( 2 to insure cache will be full )
constexpr static const std::size_t m = 2;

/*!
 * \brief Size of the array for cache emulation.
 */
constexpr static const std::size_t Offset = m * cache;

/*!
 * \brief Maximum power of the cache associativity.
 */
constexpr static const std::size_t Nmax = 20;

/*!
 * \brief Size of the element in the array for cache emulation (in bytes).
 */
constexpr static const std::size_t element_size = 64;
