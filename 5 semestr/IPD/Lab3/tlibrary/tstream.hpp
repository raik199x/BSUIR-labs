#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace tlib
{
	using std::string;
	typedef std::stringstream sstream;

	using std::to_string;

	using std::cout;
	using std::cin;
	using std::clog;
	using std::cerr;
	using std::wcout;
	using std::wcin;
	using std::wclog;
	using std::wcerr;
	static constexpr const char* const endl = "\n";
	using std::getline;

	using std::hex;
	using std::dec;
	using std::oct;

	using std::fixed;
	using std::scientific;
	using std::hexfloat;
	using std::defaultfloat;

	using std::setw;
	using std::setfill;
	using std::boolalpha;

	using std::stoi;
	using std::stol;
	using std::stoul;
	using std::stoll;
	using std::stoull;
	using std::stof;
	using std::stod;
	using std::stold;

	typedef std::u8string s8;
	typedef std::u16string s16;
	typedef std::u32string s32;
	typedef std::wstring swide;
}
