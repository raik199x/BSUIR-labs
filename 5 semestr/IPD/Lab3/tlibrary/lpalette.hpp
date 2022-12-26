#pragma once

/*!
 * \title TLinux
 * \subtitle LPalette
 * \version 1.0
 * \author TimuriAlvarez
 */

#include <iostream>

#include "tlibrary.hpp"
#include "tstream.hpp"

/*!
 * \namespace tlib
 * \brief Common namespace for TLibLibrary
 */
namespace tlib
{
	class LPalette;

	std::ostream& operator<<(std::ostream& stream, const LPalette& palette);
}

/*!
 * \class LPalette
 * \brief LPalette class for making your console output look like a masterpiece
 */
class tlib::LPalette
{
private:
	LPalette(const string palette);
	LPalette(const tcarbon::u8 value, const bool isActive);
	LPalette(const tcarbon::u8 value, const bool isBright, const bool isBackground);

public:
	/*!
	 * \class LRGB
	 * \brief The RGB class for LPalette Custom RGB Colours
	 */
	class LRGB
	{
		friend LPalette;
	public:
		LRGB(const tcarbon::u8 red = 0u, const tcarbon::u8 green = 0u, const tcarbon::u8 blue = 0u);
		LRGB(const tcarbon::u32 hex);

	private:
		tcarbon::u8 leftover_, red_, green_, blue_;
	};

	/*!
	 * \group LPalette Styles
	 */
	static LPalette Reset(const bool reset = true);
	static LPalette Bold(const bool isActive = true);
	static LPalette Faint(const bool isActive = true), Dim(const bool isActive = true);
	static LPalette Italic(const bool isActive = true);
	static LPalette Underline(const bool isActive = true);
	static LPalette SlowBlink(const bool isActive = true);
	static LPalette RapidBlink(const bool isActive = true);
	static LPalette Invert(const bool isActive = true);
	static LPalette Conceal(const bool isActive = true), Hide(const bool isActive = true);
	static LPalette CrossOut(const bool isActive = true), Strike(const bool isActive = true);

	/*!
	 * \group LPalette Colours
	 */
	static LPalette Black(const bool isBright = false, const bool isBackground = false);
	static LPalette Red(const bool isBright = false, const bool isBackground = false);
	static LPalette Green(const bool isBright = false, const bool isBackground = false);
	static LPalette Yellow(const bool isBright = false, const bool isBackground = false);
	static LPalette Blue(const bool isBright = false, const bool isBackground = false);
	static LPalette Magenta(const bool isBright = false, const bool isBackground = false);
	static LPalette Cyan(const bool isBright = false, const bool isBackground = false);
	static LPalette White(const bool isBright = false, const bool isBackground = false);
	static LPalette Default(const bool isBright = false, const bool isBackground = false);

	/*!
	 * \group LPalette Custom Colours Constructors
	 */
	static LPalette ColourTable(const tcarbon::u8 value, const bool isBackground = false);
	static LPalette RGB(const LRGB rgb = LRGB(), const bool isBackground = false);
	static LPalette HEX(const tcarbon::u32 hex = 0u, const bool isBackground = false);

	/*!
	 * \group Operator for writing LPalette objects into output streams
	 */
	friend std::ostream& operator<<(std::ostream& stream, const LPalette& palette);

	/*!
	 * \class LThemes
	 * \brief Different colours from different themes
	 */
	class LThemes;
	class Pure;

private:
	string palette_;
};

class tlib::LPalette::LThemes
{
public:
	static LPalette Manjaro(const bool isBackground = false);
	static LPalette Layan(const bool isBackground = false);
	static LPalette Dorian(const bool isBackground = false);
	static LPalette Dracula(const bool isBackground = false);
	static LPalette Ubuntu(const bool isBackground = false);
	static LPalette SkyFall(const bool isBackground = false);

	static LPalette Orange(const bool isBackground = false);
};

class tlib::LPalette::Pure
{
public:
	static LPalette Red(const bool isBackground = false);
	static LPalette Green(const bool isBackground = false);
	static LPalette Blue(const bool isBackground = false);

	static LPalette Yellow(const bool isBackground = false);
	static LPalette Magenta(const bool isBackground = false);
	static LPalette Cyan(const bool isBackground = false);

	static LPalette Black(const bool isBackground = false);
	static LPalette White(const bool isBackground = false);
};
