#pragma once

#include "../tlibrary/tlibrary.hpp"

namespace port::io
{
	using namespace tcarbon;
	bool getIOPerms(void);

	u8 read8(const u16 port, const bool notify = false);
	void write8(const u16 port, const u8 value = 0u, const bool notify = false);
	u16 read16(const u16 port, const bool notify = false);
	void write16(const u16 port, const u16 value = 0u, const bool notify = false);
	u32 read32(const u16 port, const bool notify = false);
	void write32(const u16 port, const u32 value = 0u, const bool notify = false);
}
