/*!
 * \title TCurses
 * \version 1.0
 * \author TimuriAlvarez
 */

#include <list>
#include <ncurses.h>
#include <tcurses.hpp>

using win = WINDOW*;

TCurses::TCurses(void)
	: window_(initscr()) {}

TCurses::TCurses(const i32 start_x, const i32 start_y,
				 const i32 width, const i32 height)
	: window_(newwin(height, width, start_y, start_x)) {}

TCurses::~TCurses(void)
{
	endwin();
}

i32 TCurses::cBreakable(const bool isBreakableByCtrlC)
{
	return (isBreakableByCtrlC ? cbreak : nocbreak)();
}

i32 TCurses::rawInput(const bool isRawInput)
{
	return isRawInput ? ::raw() : ::noraw();
}

i32 TCurses::echo(const bool isEcho)
{
	return isEcho ? ::echo() : ::noecho();
}

bool TCurses::hasColours()
{
	return ::has_colors();
}

i32 TCurses::box(const i32 horizontal, const i32 vertical) const
{
	return ::box((win)window_, vertical, horizontal);
}

i32 TCurses::box(const i32 left, const i32 right, const i32 top, const i32 bottom,
				 const i32 top_left, const i32 top_right, const i32 bottom_left, const i32 bottom_right) const
{
	return ::wborder((win)window_, left, right, top, bottom, top_left, top_right, bottom_left, bottom_right);
}

i32 TCurses::refresh(void) const
{
	return ::wrefresh((win)window_);
}

i32 TCurses::clear(void) const
{
	return ::wclear((win)window_);
}

i32 TCurses::update(void) const
{
	return ::wrefresh((win)window_);
}

i32 TCurses::keypad(const bool isKeyPad) const
{
	return ::keypad((win)window_, isKeyPad);
}

i32 TCurses::nodelay(const bool isNoDelay) const
{
	return ::nodelay((win)window_, isNoDelay);
}

i32 TCurses::move(const i32 x, const i32 y) const
{
	return ::wmove((win)window_, y, x);
}

i32 TCurses::getchar(void) const
{
	return ::wgetch((win)window_);
}

i32 TCurses::getchar(const i32 x, const i32 y) const
{
	move(x,y);
	return getchar();
}

void TCurses::print(const char* const message) const
{
	wprintw((win)window_, "%s", message);
}

void TCurses::print(const i32 x, const i32 y, const char* const message) const
{
	mvwprintw((win)window_, y, x, "%s", message);
}

TCurses::tposition TCurses::position(const Location location) const
{
	tposition result;
	switch (location)
	{
	case Begin:
		getbegyx(stdscr, (result.y), (result.x));
		break;
	case Current:
		getyx(stdscr, (result.y), (result.x));
		break;
	case End:
		getmaxyx(stdscr, (result.y), (result.x));
		break;
	}
	return result;
}

i32 TCurses::attribute(const Text attribute, const bool on) const
{
	static constexpr const attr_t TextAttributes[11] = {
		A_NORMAL,        //Normal display (no highlight)
		A_STANDOUT,      //Best highlighting mode of the terminal
		A_UNDERLINE,     //Underlining
		A_REVERSE,       //Reverse video
		A_BLINK,         //Blinking
		A_DIM,           //Half bright
		A_BOLD,          //Extra bright or bold
		A_PROTECT,       //Protected mode
		A_INVIS,         //Invisible or blank mode
		A_ALTCHARSET,    //Alternate character set

		A_ITALIC,        //Italic ncurses extension
	};
	return on ? ::wattron((win)window_, TextAttributes[attribute]) : ::wattroff((win)window_, TextAttributes[attribute]);
}
