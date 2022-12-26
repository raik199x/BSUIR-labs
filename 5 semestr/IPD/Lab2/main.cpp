#include <iostream>
#include<iomanip>

#include "kernmod.hpp"
#include "pcitable.hpp"
#include "lpalette/lpalette.hpp"

/*!
 * \brief displays information about selected pci device
 * \param bus bus number
 * \param device device number
 * \param func function number
 * \param data encoded value of vendor id and device id
 */
void display(const u8 bus, const u8 device, const u8 func, u32 data)
{
	using namespace std;
	using namespace tlib;
	using namespace pci::ids;

	const u16 vendorID = data & 0xFFFF;
	data = data >> 16;
	const u16 deviceID = data & 0xFFFF;

	cout << LPalette::White() << setw(2) << setfill('0')
		 << u16(bus) << LPalette::Black(true) << ":"
		 << LPalette::White() << setw(2) << setfill('0')
		 << u16(device) << LPalette::Black(true) << "."
		 << LPalette::White() << setw(2) << setfill('0')
		 << u16(func) << LPalette::Black(true) << ":" << LPalette::Reset() << " "
		 << LPalette::Blue(true) << setw(4) << setfill('0') << vendorID << LPalette::Reset() << " "
		 << LPalette::Blue(true) << setw(4) << setfill('0') << deviceID << LPalette::Reset() << " "
		 << LPalette::RGB({245, 116, 0}) << findVendor(vendorID) << LPalette::Reset()
		 << ((findVendor(vendorID).length() < 20) ? "\t\t" : "") << "\t"
		 << LPalette::RGB({255, 165, 0}) << findDevice(vendorID, deviceID)
		 << LPalette::Reset() << endl;
}

/*!
 * \brief bus [0-255] slot [0-31] func [0-7]
 */
int main(void)
{
	using namespace std;
	using namespace tlib;
	cout << hex;

	bool first_match = true;

	for(u8 bus = 0; bus < 255; ++bus)
		for(u8 slot = 0; slot < 32; ++slot)
			for(u8 func = 0; func < 8; ++func)
			{
				const u32 data = kernmod(bus, slot, func);
				if (kernfail()) return PORT_FAILURE;
				if (first_match)
				{
					cout << LPalette::White() << "Bs"
						 << LPalette::Black(true) << ":"
						 << LPalette::White() << "Sl"
						 << LPalette::Black(true) << "."
						 << LPalette::White() << "Fn"
						 << LPalette::Black(true) << ":"
						 << LPalette::Reset() << " "
						 << LPalette::Blue(true) << "VnID"
						 << LPalette::Reset() << " "
						 << LPalette::Blue(true) << "DvID"
						 << LPalette::Reset() << " "
						 << LPalette::RGB({245, 116, 0}) << "Vendor"
						 << LPalette::Reset() << "\t\t\t\t"
						 << LPalette::RGB({255, 165, 0}) << "Device"
						 << LPalette::Reset() << endl << endl;
					first_match = false;
				}
				if(data != PORT_FAILURE)
					display(bus, slot, func, data);
			}
	cout << LPalette::Reset() << endl
		 << LPalette::White() << "Bs - bus"
		 << LPalette::Reset() << "    "
		 << LPalette::White() << "Sl - slot"
		 << LPalette::Reset() << "    "
		 << LPalette::White() << "Fn - function"
		 << LPalette::Reset() << "\t\t\t"
		 << LPalette::Blue(true) << "VnID - vendor ID"
		 << LPalette::Reset() << "\t"
		 << LPalette::Blue(true) << "DvID - device ID"
		 << LPalette::Reset() << endl;
	return 0;
}
