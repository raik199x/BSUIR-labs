#include "lpalette.hpp"

using namespace tlib;

LPalette::LPalette(const string palette)
{
	palette_ = palette;
}
LPalette::LPalette(const u8 value, const bool isActive)
{
	palette_ = isActive ? "" : "2";
	palette_ += ('0' + value);
}
LPalette::LPalette(const u8 value, const bool isBright, const bool isBackground)
{
	palette_ = isBright ? (isBackground ? "10" : "9") : (isBackground ? "4" : "3");
	palette_ += ('0' + value);
}
std::ostream& tlib::operator<<(std::ostream& stream, const LPalette& palette)
{
	return (stream << (palette.palette_.empty() ? "" : string("\033[" + palette.palette_ + "m")));
}

LPalette::RGB::RGB(const u8 red, const u8 green, const u8 blue)
{
	red_ = red;
	green_ = green;
	blue_ = blue;
}

LPalette LPalette::Reset(const bool reset)
{
	return LPalette(reset ? "0" : "");
}
LPalette LPalette::Bold(const bool isActive)
{
	return LPalette(1, isActive);
}
LPalette LPalette::Faint(const bool isActive)
{
	return LPalette(2, isActive);
}
LPalette LPalette::Dim(const bool isActive)
{
	return Faint(isActive);
}
LPalette LPalette::Italic(const bool isActive)
{
	return LPalette(3, isActive);
}
LPalette LPalette::Underline(const bool isActive)
{
	return LPalette(4, isActive);
}
LPalette LPalette::SlowBlink(const bool isActive)
{
	return LPalette(5, isActive);
}
LPalette LPalette::RapidBlink(const bool isActive)
{
	return LPalette(6, isActive);
}
LPalette LPalette::Invert(const bool isActive)
{
	return LPalette(7, isActive);
}
LPalette LPalette::Conseal(const bool isActive)
{
	return LPalette(8, isActive);
}
LPalette LPalette::Hide(const bool isActive)
{
	return Conseal(isActive);
}
LPalette LPalette::CrossOut(const bool isActive)
{
	return LPalette(9, isActive);
}
LPalette LPalette::Strike(const bool isActive)
{
	return CrossOut(isActive);
}

LPalette LPalette::Black(const bool isBright, const bool isBackground)
{
	return LPalette(0, isBright, isBackground);
}
LPalette LPalette::Red(const bool isBright, const bool isBackground)
{
	return LPalette(1, isBright, isBackground);
}
LPalette LPalette::Green(const bool isBright, const bool isBackground)
{
	return LPalette(2, isBright, isBackground);
}
LPalette LPalette::Yellow(const bool isBright, const bool isBackground)
{
	return LPalette(3, isBright, isBackground);
}
LPalette LPalette::Blue(const bool isBright, const bool isBackground)
{
	return LPalette(4, isBright, isBackground);
}
LPalette LPalette::Magenta(const bool isBright, const bool isBackground)
{
	return LPalette(5, isBright, isBackground);
}
LPalette LPalette::Cyan(const bool isBright, const bool isBackground)
{
	return LPalette(6, isBright, isBackground);
}
LPalette LPalette::White(const bool isBright, const bool isBackground)
{
	return LPalette(7, isBright, isBackground);
}
LPalette LPalette::Default(const bool isBright, const bool isBackground)
{
	return LPalette(9, isBright, isBackground);
}

LPalette LPalette::ColourTable(const u8 value, const bool isBackground)
{
	return LPalette(string(isBackground ? "4" : "3") + "8;5;" + std::to_string(value));
}

LPalette LPalette::RGB(const class RGB rgb, const bool isBackground)
{
	return LPalette(string(isBackground ? "4" : "3") + "8;2;" + std::to_string(rgb.red_) + ";" + std::to_string(rgb.green_) + ";" + std::to_string(rgb.blue_));
}
