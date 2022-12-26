#pragma once

/*!
 * \title TLibrary
 * \version 1.0
 * \author TimuriAlvarez
 */

#include <cstdint>
#include <string>

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
	typedef std::int8_t			i8;			// – 8 bit integer
	typedef std::int16_t		i16;		// – 16 bit integer
	typedef std::int32_t		i32;		// – 32 bit integer
	typedef std::int64_t		i64;		// – 64 bit integer
	typedef signed __int128		i128;		// – 128 bit integer

	typedef std::uint8_t		u8;			// – 8 bit integer
	typedef std::uint16_t		u16;		// – 16 bit integer
	typedef std::uint32_t		u32;		// – 32 bit integer
	typedef std::uint64_t		u64;		// – 64 bit integer
	typedef unsigned __int128	u128;		// – 128 bit integer

	/*!
	 * \brief Floating point data types
	 */
	typedef float				f32;		// – 32 bit float
	typedef double				f64;		// – 64 bit float
	typedef __float128			f128;		// – 128 bit float

	/*!
	 * \brief Character data types
	 */
	typedef char8_t				c8;			// – 8 bit character
	typedef char16_t			c16;		// – 16 bit character
	typedef char32_t			c32;		// – 32 bit character
	typedef wchar_t				cwide;		// – wide character

	/*!
	 * \brief Not basic but std types
	 */
	typedef std::size_t			usize;		// - 64 bit integer
	typedef std::intptr_t		iptr;		// - integer representation of pointers
	typedef std::uintptr_t		uptr;		// - integer representation of pointers
	typedef std::ptrdiff_t		dptr;		// - integer representation of pointers
}
