#pragma once

#include <string>

#include "kernmod.hpp"

namespace pci::ids
{
	using std::string;

	string findVendor(const u16 venid);
	string findDevice(const u16 venid, const u16 devid);
};

