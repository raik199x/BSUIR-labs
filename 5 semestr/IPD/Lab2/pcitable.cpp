#include <map>
#include <fstream>
#include <sstream>

#include "pcitable.hpp"
#include "lpalette/lpalette.hpp"

using std::fstream;
using namespace tlib;
using namespace pci::ids;

typedef struct
{
	u64 position;
	string vendor;
} pci_map_entity;

/*!
 * \brief pci_map map for boosting up parsing  process: known vendors will be saved in the map as well as their position in pci.ids file to be used again lately
 */
std::map<u64, pci_map_entity> pci_map = std::map<u64, pci_map_entity>();

/*!
 * \brief validates a character to be a hex lowercase digit
 * \param c character to be validated
 * \return true if c is a valid character, else- returns false
 */
bool validChar(const char c)
{
	if (c >= '0' && c <= '9') return true;
	if (c >= 'a' && c <= 'f') return true;
	return false;
}

/*!
 * \brief validates bunch of characters to be all valid
 * \return true if all characters are valid, else - false
 */
bool valid(const char c0, const char c1, const char c2, const char c3)
{
	return validChar(c0) && validChar(c1)
			&& validChar(c2) && validChar(c3);
}

/*!
 * \brief convert character to hex number
 * \param c character to be converted
 * \return hex value decoded from character
 */
u8 ctou8(const char c)
{
	return (c >= '0' && c <= '9') ? (c - '0') : (c - 'a' + 10);
}

/*!
 * \brief convert 4 characters to a single hex number
 * \return hex value decoded from characters
 */
u16 ltou16(const char c0, const char c1, const char c2, const char c3)
{
	u16 retvalue = 0;
	retvalue += ctou8(c0);
	retvalue *= 16;
	retvalue += ctou8(c1);
	retvalue *= 16;
	retvalue += ctou8(c2);
	retvalue *= 16;
	retvalue += ctou8(c3);
	return retvalue;
}

/*!
 * \brief finds vendor name; uses pci_map to save found vendor names and locations in pci.ids file for all next attempts to find these vendors
 * \param venid PCI vendor ID
 * \return vendor name from pci_map if already known, else parses cpi.ids file and returns vendor name if found, else unknown vendor message
 */
string pci::ids::findVendor(const u16 venid)
{
	if (pci_map.contains(venid)) return pci_map.at(venid).vendor;

	fstream pci_ids;
	pci_ids.open("pci.ids");
	if (!pci_ids.is_open())
	{
		std::cout << LPalette::Red();
		return "pci.ids file is unaccessable";
	}
	u64 pos;
	std::stringstream sstream;
	sstream << "Unknown vendor " << std::hex << venid << " (pci.ids failure)";
	string result = sstream.str();

	while (!pci_ids.eof())
	{
		string line;
		std::getline(pci_ids, line);
		pos = pci_ids.tellg();
		if (line.empty()) continue;
		if (!valid(line[0], line[1], line[2], line[3])) continue;
		if(ltou16(line[0], line[1], line[2], line[3]) != venid) continue;
		line = line.erase(0, 6);
		pci_map.insert(std::pair(venid, pci_map_entity({ pos, line })));
		result = line;
		break;
	}
	pci_ids.close();
	if (result == sstream.str())
		std::cout << LPalette::Red();
	return result;
}

/*!
 * \brief finds device name; uses pci_map to locate vendor in the pci.ids file, then parses all vendor's devices
 * \param venid PCI vendor ID
 * \param devid PCI device ID
 * \return device name from parced pci.ids file if was found, else unknown device message
 */
string pci::ids::findDevice(const u16 venid, const u16 devid)
{
	findVendor(venid);
	if (!pci_map.contains(venid))
	{
		std::cout << LPalette::Red();
		return "Unknown device vendor";
	}
	fstream pci_ids;
	pci_ids.open("pci.ids");
	if (!pci_ids.is_open())
	{
		std::cout << LPalette::Red();
		return "pci.ids file is unaccessable";
	}
	std::stringstream sstream;
	sstream << "Unknown device " << std::hex << devid << " (pci.ids failure)";
	string result = sstream.str();
	pci_ids.seekg(pci_map.at(venid).position);
	while (!pci_ids.eof())
	{
		string line;
		std::getline(pci_ids, line);
		if (line.empty()) continue;
		if (validChar(line[0])) break;
		if (!valid(line[1], line[2], line[3], line[4])) continue;
		if (ltou16(line[1], line[2], line[3], line[4]) != devid) continue;
		result = line.erase(0, 7);
		break;
	}
	pci_ids.close();
	if (result == sstream.str())
		std::cout << LPalette::Red();
	return result;
}
