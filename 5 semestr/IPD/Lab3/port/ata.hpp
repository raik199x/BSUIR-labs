#pragma once

#include "../tlibrary/tlibrary.hpp"
#include "../tlibrary/tstream.hpp"

namespace port::ata
{
	using namespace tlib;
	using namespace tcarbon;

	static constexpr const struct IDEBus
	{
		const char* const name;
		const u16 CommandBlock;
		const u16 ControlBlock;
	} PrimaryBus = { "Primary", 0x1F0, 0x3F0 },
	  SecondaryBus = { "Secondary", 0x170, 0x370 },
	  ExtraPrimaryBus = { "Extra Primary", 0x1E0, 0x3E0 },
	  ExtraSecondaryBus = { "Extra Secondary", 0x160, 0x360 };

	enum CommandBlockRegisters
	{
		DataReg = 0,					//DIOR DIOW
		ErrorReg = 1,					//DIOR
		FeatureReg = 1,					//DIOW
		SectorCountReg = 2,				//DIOR DIOW
		SectorNumberReg = 3,			//DIOR DIOW
		CylinderLowReg = 4,				//DIOR DIOW
		CylinderHighReg = 5,			//DIOR DIOW
		DriveOrHeadReg = 6,				//DIOR DIOW
		StatusReg = 7,					//DIOR
		CommandReg = 7					//DIOW
	};
	enum ControlBlockRegisters
	{
		//DataBusHighImpedanceRegs = 0,	//DIOR
		AlternateStatusReg = 6,			//DIOR
		DeviceControlReg = 6,			//DIOW
		DriveAddressReg = 7				//DIOR
	};
}
