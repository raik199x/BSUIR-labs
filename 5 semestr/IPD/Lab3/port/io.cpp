#include <sys/io.h>

#include "io.hpp"
#include "../tlibrary/tstream.hpp"

using namespace tlib;
using namespace tcarbon;

bool port::io::getIOPerms(void)
{
	return iopl(3) == 0;
}

u8 port::io::read8(const u16 port, const bool notify)
{
	const u8 value = inb(port);
	if (notify) clog << "0x" << (u64)(port) << " : " << "0x" << (u64)(value) << endl;
	return value;
}
void port::io::write8(const u16 port, const u8 value, const bool notify)
{
	if (notify) clog << "0x" << (u64)(value) << " -> " << "0x" << (u64)(port) << endl;
	return outb(value, port);
}
u16 port::io::read16(const u16 port, const bool notify)
{
	const u16 value = inw(port);
	if (notify) clog << "0x" << (u64)(port) << " : " << "0x" << (u64)(value) << endl;
	return value;
}
void port::io::write16(const u16  port, const u16 value, const bool notify)
{
	if (notify) clog << "0x" << (u64)(value) << " -> " << "0x" << (u64)(port) << endl;
	return outw(value, port);
}
u32 port::io::read32(const u16 port, const bool notify)
{
	const u32 value = inl(port);
	if (notify) clog << "0x" << (u64)(port) << " : " << "0x" << (u64)(value) << endl;
	return value;
}
void port::io::write32(const u16 port, const u32 value, const bool notify)
{
	if (notify) clog << "0x" << (u64)(value) << " -> " << "0x" << (u64)(port) << endl;
	return outl(value, port);
}
