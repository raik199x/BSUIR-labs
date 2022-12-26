#include <chrono>
#include <thread>
#include <array>

#include "tlibrary/lpalette.hpp"

/* The goal of the application:
 * List HDD/SSD Drives with attributes:
 * - Model
 * - Manufacturer
 * - Serial Number
 * - Firmware Revision
 * - Memory Information
 * - Interface type
 * - List of supported modes
 * 
 * In short, the output must be simillar to these two ones:
 * $ sudo hdparm -I /dev/sda
 * $ sudo sg_sat_identify /dev/sda
 */

/* The information was taken from the bunch of sources:
 * [1] https://wiki.osdev.org/ATA_PIO_Mode
 * [2] https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ata/ns-ata-_identify_device_data
 * [3] http://www.racelinecentral.com/HarddriveSpeed.html#Programmed%20I/O%20(PIO)%20Modes
 * [4] https://wiki.osdev.org/AHCI
 * [5] https://en.wikipedia.org/wiki/Parallel_ATA#Features_introduced_with_each_ATA_revision
 * and a book:
 * [0] ATA-Interface-Reference-Manual.pdf (c)
 */

#include "port/io.hpp"
#include "port/ata.hpp"

using namespace tlib;
using namespace tcarbon;
using namespace port;

/*!
 * \brief From source [1]: The disk that was selected last (by the BIOS, during boot) is supposed to maintain control of the electrical values on each IDE bus. If there is no disk connected to the bus at all, then the electrical values on the bus will all go "high" (to +5 volts).
 * A computer will read this as an 0xFF byte -- this is a condition called a "floating" bus. Measuring "float" is a shortcut for detecting that drives do not exist. Reading a non-0xFF value is not completely definitive.
 * \return true if the bus is "floating", else - false
 */
bool isFloatingBus(const struct ata::IDEBus bus, const bool notify)
{
	return io::read8(bus.CommandBlock + ata::StatusReg, notify) == 0xFF;
}

static u16 buffer[256][2];
static u8 signature_low[2];
static u8 signature_high[2];

bool identify(const struct ata::IDEBus bus, const bool isMaster, const bool notify)
{
	using namespace ata;

	cout << "\t" << (isMaster ? "Master" : "Slave") << " Drive" << '\t' << "[";
	//From sources [0] and [1]:
	constexpr const u8 IDENTIFY = 0xEC;
	//To use the IDENTIFY command, select a target drive by sending 0xA0 for the master drive, or 0xB0 for the slave, to the "drive select" IO port. On the Primary bus, this would be port 0x1F6.
	io::write8(bus.CommandBlock + DriveOrHeadReg, isMaster ? 0xA0 : 0xB0, notify);
	//Then set the SectorCount, LBAlo, LBAmid, and LBAhi IO ports to 0 (port 0x1F2 to 0x1F5).
	io::write8(bus.CommandBlock + SectorCountReg, 0, notify);
	io::write8(bus.CommandBlock + SectorNumberReg, 0, notify);
	io::write8(bus.CommandBlock + CylinderLowReg, 0, notify);
	io::write8(bus.CommandBlock + CylinderHighReg, 0, notify);
	//Then send the IDENTIFY command (0xEC) to the Command IO port (0x1F7).
	io::write8(bus.CommandBlock + CommandReg, IDENTIFY, notify);
	//Then read the Status port (0x1F7) again.
	u8 status = io::read8(bus.CommandBlock + StatusReg, notify);
	//If the value read is 0, the drive does not exist.
	if (status == 0)
	{
		cout << LPalette::Pure::Red()
			 << "doesn't exist"
			 << LPalette::Reset() << "]" << endl;
		return false;
	}
	//For any other value: poll the Status port (0x1F7) until bit 7 (BSY, value = 0x80) clears.
	constexpr const u64 LIMIT = 0xFFFFFFFFFFFFFFFF;
	u64 limit = 0u;
	while (true)
	{
		if (limit == LIMIT) 
		{
			cout << LPalette::Pure::Red()
				 << "busy too long: time is out"
				 << LPalette::Reset() << "]" << endl;
		return false;
		}
		if ((status & 0x80) == 0) break;
		status = io::read8(bus.CommandBlock + StatusReg, notify);
		++limit;
	}
	//Because of some ATAPI drives that do not follow spec, at this point you need to check the LBAmid and LBAhi ports (0x1F4 and 0x1F5) to see if they are non-zero.
	signature_low[isMaster] = io::read8(bus.CommandBlock + CylinderLowReg, notify);
	signature_high[isMaster] = io::read8(bus.CommandBlock + CylinderHighReg, notify);
	//If so, the drive is not ATA, and you should stop polling.
	if (signature_low[isMaster] && signature_high[isMaster])
	{
		cout << LPalette::Pure::Red()
			 << "exists, but not ATA"
			 << LPalette::Reset() << "]" << endl;
		return false;
	}
	//Otherwise, continue polling one of the Status ports until bit 3 (DRQ, value = 8) sets, or until bit 0 (ERR, value = 1) sets.
	limit = 0u;
	while (true)
	{
		if (limit == LIMIT) 
		{
			cout << LPalette::Pure::Red()
				 << "data request failure: the time is out"
				 << LPalette::Reset() << "]" << endl;
			return false;
		}
		if ((status & 8) == 0) break;
		if ((status & 1) == 0) break;
		status = io::read8(bus.CommandBlock + StatusReg, notify);
		++limit;
	}
	//At that point, if ERR is clear, the data is ready to read from the Data port (0x1F0).
	if (status & 1)
	{
		cout << LPalette::Pure::Red()
			 << "error detected: "
			 << LPalette::Reset() << "]" << endl;
		return false;
	}
	cout << LPalette::Pure::Green()
		 << "identified"
		 << LPalette::Reset() << "]" << endl;
	//Read 256 16-bit values, and store them.
	for (usize index = 0u; index < 256; ++index)
		buffer[index][isMaster] = io::read16(bus.CommandBlock + DataReg);
	return true;
}

string extractor(const bool isMaster, const usize index1, const usize index2)
{
	string result;
	for (usize index = index1; index <= index2; ++index)
	{
		u16 tmp = buffer[index][isMaster];
		char hight = tmp;
		tmp = tmp >> 8;
		char low = tmp;

		result += low;
		result += hight;
	}
	return result;
}

string get_interface(const bool isMaster)
{
	/* From source [1]: */
	/* get the "signature bytes" */
	const u8 cl = signature_low[isMaster];
	const u8 ch = signature_high[isMaster];

	/* differentiate ATA, ATAPI, SATA and SATAPI */
	if (cl == 0x14 && ch == 0xEB) return "PATAPI";
	if (cl == 0x69 && ch == 0x96) return "SATAPI";
	if (cl == 0x00 && ch == 0x00) return "PATA";
	if (cl == 0x3C && ch == 0xC3) return "SATA";
	cout << LPalette::Pure::Red();
	return "UNKNOWN";
}

/*!
 * \brief List of modes generator, e.g. get_mode("mode", 0b101101) -> "mode 0, mode 2, mode 3, mode 5"
 */
string get_mode(const char* const mode, u8 bits)
{
	if (bits == 0) return "NONE";

	string result;
	for (u8 i = 0; i < 8; ++i, bits = bits >> 1)
	{
		if ((bits & 0b1) == 0) continue;
		if (!result.empty()) result += ", ";
		result += mode;
		result += to_string((u32)(i));
	}
	return result;
}

/*!
 * \brief Pio bits signature for get_mode function
 */
u8 get_pio_signature(const u16 PIOCycleTime)
{
	//Constants from source [3]:
	static constexpr const u16 pio4 = 120;
	static constexpr const u16 pio3 = 180;
	static constexpr const u16 pio2 = 240;
	static constexpr const u16 pio1 = 383;
	static constexpr const u16 pio0 = 600;

	switch (PIOCycleTime)
	{
	case pio4:
		return 0b11111;
	case pio3:
		return 0b01111;
	case pio2:
		return 0b00111;
	case pio1:
		return 0b00011;
	case pio0:
		return 0b00001;
	default:
		return 0b00000;
	}
}

void parse(const bool isMaster, const bool rawHex)
{
	static constexpr const char* const ata_types_long[2] = {"Hard Disk Drive", "Solid State Drive"};
	static constexpr const char* const ata_types_short[2] = {"HDD", "SSD"};

	const bool isFXed = (buffer[0][isMaster] >> 6) & 0b1;
	const bool isRemovable = (buffer[0][isMaster] >> 7) & 0b1;
	const bool isSSD = (buffer[0][isMaster] >> 15) == 0;
	const string ata_serial = extractor(isMaster, 10, 19);
	const string ata_firmware = extractor(isMaster, 23, 26);
	const string ata_model = extractor(isMaster, 27, 46);
	const string ata_prod_id = extractor(isMaster, 170, 173);
	const string ata_media = extractor(isMaster, 176, 185);

	const bool isDMA = (buffer[49][isMaster] >> 8) & 0b1;
	const bool isLBA = (buffer[49][isMaster] >> 9) & 0b1;
	const bool isIOready = (buffer[49][isMaster] >> 10) & 0b1;
	const bool isIOreadyDisabled = (buffer[49][isMaster] >> 11) & 0b1;
	const string sDMA = get_mode("sdma", buffer[62][isMaster] & 0x00FF);
	const string mDMA = get_mode("mdma", buffer[63][isMaster] & 0x00FF);
	const bool isLBA48 = (buffer[80][isMaster] >> 5) & 0b1;
	const string uDMA = get_mode("udma", buffer[88][isMaster] & 0x00FF);

	/*!
	 * \brief From source [2]: Obtain information about Cycle Time in different modes
	 */
	const u64 MinimumMWXferCycleTime = buffer[65][isMaster];
	const u64 RecommendedMWXferCycleTime = buffer[66][isMaster];
	const u16 MinimumPIOCycleTime = buffer[67][isMaster];
	const u16 MinimumPIOCycleTimeIOReady = buffer[68][isMaster];

	/*!
	 * \brief Using source [3]: Get user-defined PIO signature, simillar to sdma-, mdma-, udma-signatures.
	 */
	const u8 pio_signature = get_pio_signature(MinimumPIOCycleTime);
	const string uPIO = pio_signature == 0 ? string("UNDEFINED: non-standard cycle time = ") + to_string(MinimumPIOCycleTime) : get_mode("pio", pio_signature);

	/*!
	 * \brief From source [1]: Obtain ata interface type
	 */
	const string ata_interface = get_interface(isMaster);

	const u64 ata_naa = buffer[108][isMaster] / 0x1000;
	const u64 ata_oui = (buffer[108][isMaster] - ata_naa * 0x1000) * 0x10000 / 0x10 + buffer[109][isMaster] / 0x10;
	const u64 ata_uid = (buffer[109][isMaster] & 0xF) * 0x100000000 + buffer[110][isMaster] * 0x10000 + buffer[111][isMaster];

	const u16 config_max_cylinders = buffer[1][isMaster];
	const u16 config_max_heads = buffer[3][isMaster];
	const u16 config_max_sectors_per_track = buffer[6][isMaster];
	const u16 config_current_cylinders = buffer[54][isMaster];
	const u16 config_current_heads = buffer[55][isMaster];
	const u16 config_current_sectors_per_track = buffer[56][isMaster];

	const u32 config_current_addressable = (buffer[58][isMaster] << 16) + buffer[57][isMaster];
	const u32 config_total_addressable = (buffer[61][isMaster] << 16) + buffer[60][isMaster];
	const u32 config_total_addressable_48 = (buffer[101][isMaster] << 16) + buffer[100][isMaster];

	/*!
	 * \brief From source [2]: PhysicalLogicalSectorSize. Good explanation here:
	 * https://www.quora.com/What-are-the-differences-between-physical-sector-size-vs-logical-sector-size
	 */
	const bool LogicalSectorLongerThan256Words = (buffer[106][isMaster] >> 3) & 0b1;
	const u16 config_logical_size = 512;
	const u16 config_physical_size = (LogicalSectorLongerThan256Words ? 4096 : 512);

	const f64 config_size_MiB = (f64)(isLBA48 ? config_total_addressable_48 : config_total_addressable) * config_physical_size / (1048576);
	const f64 config_size_MB = (f64)(isLBA48 ? config_total_addressable_48 : config_total_addressable) * config_physical_size / (1000000);

	/*!
	 * \brief From source [2]
	 */
	const bool NonZeroOffsets = (buffer[78][isMaster] >> 14) & 0b1;

	cout << "General info" << ":\t\t"
		 << LPalette::LThemes::Manjaro()
		 << (isFXed ? "Fixed" : "Not fixed") << " " << ata_interface << " Device with "
		 << (isRemovable ? "" : "non-") << "removable media" << LPalette::Reset() << endl
		 << "Drive Model" << ":\t\t"
		 << LPalette::LThemes::Manjaro()
		 << ata_model << LPalette::Reset() << endl
		 << "Serial Number" << ":\t\t"
		 << LPalette::LThemes::Manjaro()
		 << ata_serial << LPalette::Reset() << endl
		 << "Firmware Revision" << ":\t"
		 << LPalette::LThemes::Manjaro()
		 << ata_firmware << LPalette::Reset() << endl
		 << "Media Manufacturer" << ":\t" << endl
		 << "Media Type" << ":\t\t"
		 << LPalette::LThemes::Manjaro()
		 << ata_types_long[isSSD] << " (" << ata_types_short[isSSD] << ")"
		 << LPalette::Reset() << endl
		 << "Additional Product ID" << ":\t"
		 << LPalette::LThemes::Manjaro()
		 << ata_prod_id << LPalette::Reset() << endl
		 << "Media Serial Number" << ":\t"
		 << LPalette::LThemes::Manjaro()
		 << ata_media << LPalette::Reset() << endl

		 << "\t" << "Capabilities" << endl
		 << dec
		 << "LBA" << ":\t\t"
		 << LPalette::LThemes::Orange()
		 << isLBA << LPalette::Reset() << endl
		 << "LBA48" << ":\t\t"
		 << LPalette::LThemes::Orange()
		 << isLBA48 << LPalette::Reset() << endl
		 << "IOReady" << LPalette::Reset() << ":\t"
		 << LPalette::LThemes::Orange()
		 << isIOready << LPalette::Reset() << "\t" << "("
		 << (isIOreadyDisabled ? LPalette::Pure::Red() : LPalette::Pure::Green())
		 << (isIOreadyDisabled ? "disabled" : "enabled")
		 << LPalette::Reset() << ")" << endl
		 << "DMA" << ":\t\t"
		 << LPalette::LThemes::Orange()
		 << isDMA << LPalette::Reset() << endl
		 << "PIO Cycle Time" << ":\t"
		 << "no flow control = "
		 << LPalette::LThemes::Orange()
		 << MinimumPIOCycleTime << " ns" << LPalette::Reset()
		 << "\t" << "IOReady flow control = "
		 << LPalette::LThemes::Orange()
		 << MinimumPIOCycleTimeIOReady << " ns"
		 << LPalette::Reset() << endl
		 << "DMA Cycle Time" << ":\t"
		 << "minimum = "
		 << LPalette::LThemes::Orange()
		 << MinimumMWXferCycleTime << " ns" << LPalette::Reset() << "\t\t"
		 << "recommended = "
		 << LPalette::LThemes::Orange()
		 << RecommendedMWXferCycleTime << " ns" << LPalette::Reset() << endl
		 << "PIO" << ":\t\t"
		 << LPalette::LThemes::Orange()
		 << uPIO << LPalette::Reset() << endl
		 << "Single DMA" << ":\t"
		 << LPalette::LThemes::Orange()
		 << sDMA << LPalette::Reset() << endl
		 << "Multiple DMA" << ":\t"
		 << LPalette::LThemes::Orange()
		 << mDMA << LPalette::Reset() << endl
		 << "Ultra DMA" << ":\t"
		 << LPalette::LThemes::Orange()
		 << uDMA << LPalette::Reset() << endl

		 << "\t" << "Configuration" << endl
		 << "Logical" << "\t\t"
		 << LPalette::LThemes::SkyFall() << "max"
		 << LPalette::Reset() << "\t"
		 << LPalette::LThemes::SkyFall() << "current"
		 << LPalette::Reset() << endl
		 << "cylinders" << "\t"
		 << LPalette::LThemes::SkyFall() << config_max_cylinders
		 << LPalette::Reset() << "\t"
		 << LPalette::LThemes::SkyFall() << config_current_cylinders
		 << LPalette::Reset() << endl
		 << "heads" << "\t\t"
		 << LPalette::LThemes::SkyFall() << config_max_heads << LPalette::Reset() << "\t"
		 << LPalette::LThemes::SkyFall() << config_current_heads << LPalette::Reset() << endl
		 << "sectors/track" << "\t"
		 << LPalette::LThemes::SkyFall() << config_max_sectors_per_track
		 << LPalette::Reset() << "\t"
		 << LPalette::LThemes::SkyFall() << config_current_sectors_per_track
		 << LPalette::Reset() << endl
		 << "CHS current addressable sectors" << ":\t"
		 << LPalette::LThemes::SkyFall() << config_current_addressable
		 << LPalette::Reset() << endl
		 << "LBA    user addressable sectors" << ":\t"
		 << LPalette::LThemes::SkyFall() << config_total_addressable
		 << LPalette::Reset() << endl
		 << "LBA48  user addressable sectors" << ":\t"
		 << LPalette::LThemes::SkyFall() << config_total_addressable_48
		 << LPalette::Reset() << endl
		 << "Logical  Sector size (B)" << ":\t\t"
		 << LPalette::LThemes::SkyFall() << config_logical_size
		 << LPalette::Reset() << endl
		 << "Physical Sector size (B)" << ":\t\t"
		 << LPalette::LThemes::SkyFall() << config_physical_size
		 << LPalette::Reset() << endl
		 << "Logical Sector non-zero offset" << ":\t\t"
		 << LPalette::LThemes::SkyFall() << NonZeroOffsets
		 << LPalette::Reset() << endl
		 << "device size in MiB (1024 B)" << ":\t\t"
		 << LPalette::LThemes::SkyFall() << (u64)config_size_MiB
		 << LPalette::Reset() << endl
		 << "device size in MB  (1000 B)" << ":\t\t"
		 << LPalette::LThemes::SkyFall() << (u64)config_size_MB
		 << LPalette::Reset() << endl
		 << "device size in GiB (1024 MiB)" << ":\t\t"
		 << LPalette::LThemes::SkyFall() << (u64)(config_size_MiB / 1024)
		 << LPalette::Reset() << endl
		 << "device size in GB  (1000 MB) " << ":\t\t"
		 << LPalette::Pure::Cyan() << (u64)(config_size_MB / 1000)
		 << LPalette::Reset() << endl
		 << hex

		 << "\t" << "Logical Unit World Wide Name (WWN) Device Identification" << endl
		 << "NAA" << ":\t\t"
		 << LPalette::LThemes::Layan() << ata_naa
		 << LPalette::Reset() << "\t\t" << "(Network Address Authority)" << endl
		 << "IEEE OUI" << ":\t"
		 << LPalette::LThemes::Layan() << setw(6) << setfill('0')  << ata_oui
		 << LPalette::Reset() << "\t\t" << "(Organizationally Unique Identifier)" << endl
		 << "Unique ID" << ":\t" << LPalette::LThemes::Layan()
		 << setw(9) << setfill('0') << ata_uid << LPalette::Reset() << endl
		 << "Full WWN ID" << ":\t" << LPalette::LThemes::Layan() << ata_naa
		 << setw(6) << setfill('0') << ata_oui << setw(9) << setfill('0') << ata_uid
		 << LPalette::Reset() << endl;

	if (!rawHex) { cout << endl; return; }
	cout << "\t" << "Raw Buffer Output" << endl
		 << LPalette::LThemes::Manjaro() << "Index"
		 << LPalette::Reset() << "\t"
		 << LPalette::LThemes::SkyFall() << "Buffer Content (hex)"
		 << LPalette::Reset() << "\t\t\t\t"
		 << LPalette::Pure::Cyan() << "Content Representation"
		 << LPalette::Reset() << endl;
	for (u64 index = 0; index < 256; index += 8)
	{
		cout << " " << LPalette::LThemes::Manjaro() << dec << setw(3) << setfill('0') << index
			 << LPalette::Reset() << hex << "\t";
		for (u8 offset = 0; offset < 8; ++offset)
			cout << LPalette::LThemes::SkyFall()
				 << setw(4) << setfill('0')
				 << buffer[index + offset][isMaster]
				 << LPalette::Reset()
				 << (offset == 7 ? "\t\t" : " ");
		for (u8 offset = 0; offset < 8; ++offset)
		{
			char low = buffer[index + offset][isMaster] & 0x00FF;
			char high = buffer[index + offset][isMaster] >> 8;
			if (low < ' ' || low > '~') low = '.';
			if (high < ' ' || high > '~') high = '.';
			cout << LPalette::Pure::Cyan() << high << low
				 << LPalette::Reset() << (offset == 7 ? "\n" : " ");
		}
	}
	cout << endl;
}

void foreachBus(const struct ata::IDEBus bus, const bool rawHex, const bool notify)
{
	const bool isFloating = isFloatingBus(bus, notify);
	cout << "\t\t" << LPalette::Pure::Yellow() << "The " << bus.name << " Bus" << LPalette::Reset()
		 << (string(bus.name) == string(ata::PrimaryBus.name) ? "\t" : "")
		 << "\t" << "(" << LPalette::Yellow() << "0x" << bus.CommandBlock << ", " << "0x" << bus.ControlBlock << LPalette::Reset() << ")"
		 << "\t" << "[" << (isFloating ? LPalette::Pure::Red() : LPalette::Pure::Green())
		 << (isFloating ? "" : "not ") << "floating" << LPalette::Reset() << "]" << endl;
	if (isFloating) return;

	if (identify(bus, true, notify)) parse(true, rawHex);
	if (identify(bus, false, notify)) parse(false, rawHex);
	cout << endl;
}

int main(void)
{
	cout << hex << boolalpha;
	cerr << hex << boolalpha;
	clog << hex << boolalpha;

	if (!io::getIOPerms())
	{
		cerr << LPalette::Pure::Red() << "Failed to grant required permissions." << LPalette::Reset() << endl;
		return -1;
	}

	/*!
	 * \brief Wait 1.5 seconds for the drive to finish last operation.
	 */
	std::this_thread::sleep_for(std::chrono::milliseconds(1500));

	std::array<ata::IDEBus, 4> buses = { ata::PrimaryBus, ata::SecondaryBus, ata::ExtraPrimaryBus, ata::ExtraSecondaryBus };
	for (auto bus : buses)
		foreachBus(bus, true, false);
	
	return 0;
}
