#pragma once

/*!
 * \title TLinux
 * \subtitle LPalette
 * \version 1.0
 * \author TimuriAlvarez
 */

#include <iostream>

/*!
 * \namespace tlib
 * \brief Common namespase for TLibLibrary
 */
namespace tlib
{
	using std::string;
	typedef unsigned char uchar;

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
	LPalette(const uchar value, const bool isActive);
	LPalette(const uchar value, const bool isBright, const bool isBackground);

public:
	class RGB;

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
	static LPalette Conseal(const bool isActive = true), Hide(const bool isActive = true);
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
	static LPalette ColourTable(const uchar value, const bool isBackground = false);
	static LPalette RGB(const RGB rgb, const bool isBackground = false);

	/*!
	 * \group Operator for writing LPalette objects into output streams
	 */
	friend std::ostream& operator<<(std::ostream& stream, const LPalette& palette);

private:
	string palette_;
};

/*!
 * \class RGB
 * \brief The RGB class for LPalette Custom RGB Colours
 */
class tlib::LPalette::RGB
{
	friend LPalette;
public:
	RGB(const uchar red, const uchar green, const uchar blue);

private:
	uchar red_, green_, blue_;
};
