#include <filesystem>

#include "kernmod.hpp"
#include "lpalette/lpalette.hpp"

using namespace tlib;

constexpr size_t BUF_SIZE = 4;
constexpr const char* const TCFXM_DEVICE = "/dev/tcfx";
static bool kernelModuleFailure = false;

/*!
 * \brief sends specified parameters too the kernel module, may change kernel module failures flag, must  be followed by checking of kernfail() function call
 * \param bus bus number
 * \param slot slot number
 * \param func function number
 * \param pcireg pcci register number
 * \return content of the pci register if successful, else PORT_FAILURE
 */
u32 kernmod(const u8 bus, const u8 slot, u8 const func, u8 const pcireg)
{
	if (!std::filesystem::exists(TCFXM_DEVICE))
	{
		std::cerr << LPalette::Red()
				  << "Failed to reach the communication device:"
				  << LPalette::Reset() << "\t" << LPalette::Red()
				  << "the kernel module tCFX not installed/configured"
				  << LPalette::Reset() << std::endl;
		kernelModuleFailure = true;
		return PORT_FAILURE;
	}
	FILE* tcfx = nullptr;
	u16 counter = 0;
	while (tcfx == nullptr)
	{
		tcfx = fopen(TCFXM_DEVICE, "r+");
		if (counter++ != 0xFFFF) continue;
		std::cerr << LPalette::Red()
				  << "Failed to reach the tCFX communication device:"
				  << LPalette::Reset() << "\t" << LPalette::Red()
				  << "device is busy or not configured (multiple attempts have failed)"
				  << LPalette::Reset() << std::endl;
		kernelModuleFailure = true;
		return PORT_FAILURE;
	}
	u8 buffer[BUF_SIZE] = { bus, slot, func, pcireg };
	const size_t wn = fwrite(buffer, sizeof(u8), BUF_SIZE, tcfx);
	if (wn != BUF_SIZE)
	{
		std::cerr << LPalette::Red()
				  << "Failed to send a request to the tCFX communication device"
				  << LPalette::Reset() << std::endl;
		kernelModuleFailure = true;
		return PORT_FAILURE;
	}
	const size_t rn = fread(buffer, sizeof(u8), BUF_SIZE, tcfx);
	if (rn != BUF_SIZE)
	{
		std::cerr << LPalette::Red()
				  << "Failed to resive a response from the tCFX communication device"
				  << LPalette::Reset() << std::endl;
		kernelModuleFailure = true;
		return PORT_FAILURE;
	}
	fclose(tcfx);
	u32 retvalue = 0;
	retvalue += buffer[3];
	retvalue = retvalue << 8;
	retvalue += buffer[2];
	retvalue = retvalue << 8;
	retvalue += buffer[1];
	retvalue = retvalue << 8;
	retvalue += buffer[0];
	return retvalue;
}

/*!
 * \brief indicates kernel module critical failures
 * \return true if a critical failure has occured, else - false
 */
bool kernfail(void)
{
	return kernelModuleFailure;
}
