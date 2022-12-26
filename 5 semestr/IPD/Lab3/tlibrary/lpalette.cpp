#include "lpalette.hpp"

using namespace tlib;
using namespace tcarbon;

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

LPalette::LRGB::LRGB(const u8 red, const u8 green, const u8 blue)
{
	red_ = red;
	green_ = green;
	blue_ = blue;
	leftover_ = 0u;
}
LPalette::LRGB::LRGB(const u32 hex)
{
	u32 tmp = hex;
	blue_ = tmp & 0xFF;
	tmp = tmp >> 8;
	green_ = tmp & 0xFF;
	tmp = tmp >> 8;
	red_ = tmp & 0xFF;
	tmp = tmp >> 8;
	leftover_ = tmp & 0xFF;
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
LPalette LPalette::Conceal(const bool isActive)
{
	return LPalette(8, isActive);
}
LPalette LPalette::Hide(const bool isActive)
{
	return Conceal(isActive);
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

LPalette LPalette::RGB(const LRGB rgb, const bool isBackground)
{
	return LPalette(string(isBackground ? "4" : "3") + "8;2;" + std::to_string(rgb.red_) + ";" + std::to_string(rgb.green_) + ";" + std::to_string(rgb.blue_));
}
LPalette LPalette::HEX(const tcarbon::u32 hex, const bool isBackground)
{
	return RGB(LRGB(hex), isBackground);
}

LPalette LPalette::LThemes::Manjaro(const bool isBackground)
{
	return HEX(0x34be5b, isBackground);
}
LPalette LPalette::LThemes::Layan(const bool isBackground)
{
	return HEX(0x5657f5, isBackground);
}
LPalette LPalette::LThemes::Dorian(const bool isBackground)
{
	return HEX(0x31313a, isBackground);
}
LPalette LPalette::LThemes::Dracula(const bool isBackground)
{
	return HEX(0x282a36, isBackground);
}
LPalette LPalette::LThemes::Ubuntu(const bool isBackground)
{
	return HEX(0xdf4b18, isBackground);
}
LPalette LPalette::LThemes::SkyFall(const bool isBackground)
{
	return HEX(0x009593, isBackground);
}
LPalette LPalette::LThemes::Orange(const bool isBackground)
{
	return HEX(0xFF8000, isBackground);
}

LPalette LPalette::Pure::Red(const bool isBackground)
{
	return HEX(0xFF0000, isBackground);
}
LPalette LPalette::Pure::Green(const bool isBackground)
{
	return HEX(0x00FF00, isBackground);
}
LPalette LPalette::Pure::Blue(const bool isBackground)
{
	return HEX(0x0000FF, isBackground);
}

LPalette LPalette::Pure::Yellow(const bool isBackground)
{
	return HEX(0xFFFF00, isBackground);
}
LPalette LPalette::Pure::Magenta(const bool isBackground)
{
	return HEX(0xFF00FF, isBackground);
}
LPalette LPalette::Pure::Cyan(const bool isBackground)
{
	return HEX(0x00FFFF, isBackground);
}

LPalette LPalette::Pure::Black(const bool isBackground)
{
	return HEX(0x000000, isBackground);
}
LPalette LPalette::Pure::White(const bool isBackground)
{
	return HEX(0xFFFFFF, isBackground);
}
