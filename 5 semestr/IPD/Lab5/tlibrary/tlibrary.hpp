#pragma once

/*!
 * \title TLibrary
 * \version 1.0
 * \author TimuriAlvarez
 */

#include <string>
#include <cstdint>

#define bitsizeof(T_TYPE) (sizeof(T_TYPE) * 8)
#define tunused __attribute__((unused))
#define tused __attribute__((used))

/*!
 * \namespace tcarbon
 * \brief Carbon-Language-like data types
 */
namespace tcarbon
{
	/*!
	 * \brief Integer data types
	 */
	using i8 = std::int8_t;			// – 8 bit integer
	using i16 = std::int16_t;		// – 16 bit integer
	using i32 = std::int32_t;		// – 32 bit integer
	using i64 = std::int64_t;		// – 64 bit integer
	using i128 = signed __int128;	// – 128 bit integer

	using u8 = std::uint8_t;		// – 8 bit integer
	using u16 = std::uint16_t;		// – 16 bit integer
	using u32 = std::uint32_t;		// – 32 bit integer
	using u64 = std::uint64_t;		// – 64 bit integer
	using u128 = unsigned __int128;	// – 128 bit integer

	/*!
	 * \brief Floating point data types
	 */
	using f32 = float;				// – 32 bit float
	using f64 = double;				// – 64 bit float
	using f128 = __float128;		// – 128 bit float

	/*!
	 * \brief Character data types
	 */
	using c8 = char8_t;				// – 8 bit character
	using c16 = char16_t;			// – 16 bit character
	using c32 = char32_t;			// – 32 bit character
	using cwide = wchar_t;			// – wide character

	/*!
	 * \brief Not basic but std types
	 */
	using usize = std::size_t;		// - 64 bit integer
	using iptr = std::intptr_t;		// - integer representation of pointers
	using uptr = std::uintptr_t;	// - integer representation of pointers
	using dptr = std::ptrdiff_t;	// - integer representation of pointers
}
