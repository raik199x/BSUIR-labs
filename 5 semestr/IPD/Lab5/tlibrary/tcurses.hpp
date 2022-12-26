#pragma once

/*!
 * \title TCurses
 * \version 1.0
 * \author TimuriAlvarez
 */

#include <curses.h>
#include <string>
#include <sstream>
#include <iostream>

#include <tlibrary.hpp>

using namespace tcarbon;

class TCurses
{
public:
	using tposition = struct { i32 x, y; };
	using Location = enum
	{
		Begin,
		Current,
		End
	};
	using Text = enum
	{
		Normal,         //Normal display (no highlight)
		Bright,         //Best highlighting mode of the terminal
		Underline,      //Underlining
		Reverse,        //Reverse video
		Blinking,       //Blinking
		Dim,            //Half bright
		Bold,           //Extra bright or bold
		Protected,      //Protected mode
		Invisible,      //Invisible or blank mode
		Alternative,    //Alternate character set

		Italic          //Italiic ncurses extension
	};
	using Colour = enum
	{
		Black = COLOR_BLACK,
		Red = COLOR_RED,
		Green = COLOR_GREEN,
		Yellow = COLOR_YELLOW,
		Blue = COLOR_BLUE,
		Magenta = COLOR_MAGENTA,
		Cyan = COLOR_CYAN,
		White = COLOR_WHITE,
	};

	TCurses(void);
	TCurses(const i32 start_x, const i32 start_y,
			const i32 width, const i32 height);
	~TCurses(void);

	static auto cBreakable(const bool isBreakableByCtrlC = true) -> i32;
	static auto rawInput(const bool isRawInput = true) -> i32;
	static auto echo(const bool isEcho = true) -> i32;
	static auto hasColours(void) -> bool;

	auto box(const i32 horizontal = 0, const i32 vertical = 0) const -> i32;
	auto box(const i32 left, const i32 right, const i32 top, const i32 bottom,
			 const i32 top_left, const i32 top_right, const i32 bottom_left, const i32 bottom_right) const -> i32;

	auto refresh(void) const -> i32;
	auto clear(void) const -> i32;
	auto update(void) const -> i32;

	auto keypad(const bool isKeyPad = true) const -> i32;
	auto nodelay(const bool isNoDelay = true) const -> i32;
	auto getchar(void) const -> i32;
	auto getchar(const i32 x, const i32 y) const -> i32;
	auto move(const i32 x, const i32 y) const -> i32;

	auto print(const char* const message) const -> void;
	auto print(const i32 x, const i32 y, const char* const message) const -> void;

	auto position(const Location location = Current) const -> tposition;
	auto attribute(const Text attribute, const bool on = true) const -> i32;

	template<typename T> auto print(const std::basic_string<T>& message) const -> void;
	template<typename T> auto print(const std::basic_stringstream<T>& message) const -> void;
	template<typename T> auto print(const i32 x, const i32 y, const std::basic_string<T>& message) const -> void;
	template<typename T> auto print(const i32 x, const i32 y, const std::basic_stringstream<T>& message) const -> void;

private:
	void* window_;
};

template<typename T> auto TCurses::print(const std::basic_string<T>& message) const -> void
{
	return print(message.c_str());
}
template<typename T> auto TCurses::print(const std::basic_stringstream<T>& message) const -> void
{
	return print(message.str());
}
template<typename T> auto TCurses::print(const i32 x, const i32 y, const std::basic_string<T>& message) const -> void
{
	return print(x, y, message.c_str());
}
template<typename T> auto TCurses::print(const i32 x, const i32 y, const std::basic_stringstream<T>& message) const -> void
{
	return print(x, y, message.str());
}
