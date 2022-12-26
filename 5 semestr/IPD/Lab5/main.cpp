/*!
 * \title Lab5 - USB info obtainer
 * \version 1.0
 * \author TimuriAlvarez
 */

#include <map>
#include <list>
#include <array>
#include <thread>
#include <fstream>
#include <filesystem>

#include <tcurses.hpp>
#include <tstream.hpp>

using namespace tlib;
constexpr const i64 delay = 100;
constexpr const i32 winsize_x = 25 + 2;
constexpr const i32 winsize_y = 10 + 2;
constexpr const i32 max_logs = 3 * winsize_y - 2;

static inline auto syspp(const char* const command) -> i32
{
	return ::system(command);
}
static inline auto syspp(const string command) -> i32
{
	return syspp(command.c_str());
}
static inline auto syspp(const sstream command) -> i32
{
	return syspp(command.str());
}

auto first_two_words(const string str) -> string
{
	sstream tmpstream;
	string tmpstr1, tmpstr2;
	tmpstream << str;
	tmpstream >> tmpstr1;
	tmpstream >> tmpstr2;
	if (!isalnum(tmpstr2.back()))
		tmpstr2 = tmpstr2.substr(0, tmpstr2.size() - 1);
	return tmpstr1 + " " + tmpstr2;
}

struct
{
	std::atomic<bool> terminate = false;
	std::mutex mutex;
	sstream info_new, info_old, logs_new, logs_old, blck_new, blck_old;
} shared;

auto usbInfo(void) -> void
{
	while (!shared.terminate)
	{
		syspp("lsusb | grep -vi 'Linux Foundation' > ./tmpinfo.txt");
		std::this_thread::sleep_for(std::chrono::nanoseconds(delay));
		std::ifstream file;
		file.open("./tmpinfo.txt");
		if (!file.is_open()) continue;
		while(shared.mutex.try_lock() != true) {};
		auto tmpstream = sstream() << file.rdbuf();
		file.close();
		string tmpstr;
		bool is_cleaned = false;
		while (true)
		{
			getline(tmpstream, tmpstr);
			if (tmpstr.empty())
				break;
			if (!is_cleaned)
			{
				shared.info_new = sstream();
				is_cleaned = true;
			}
			shared.info_new << tmpstr.substr(33, tmpstr.size()) << newl;
		}
		shared.mutex.unlock();
		std::this_thread::sleep_for(std::chrono::nanoseconds(delay));
	}
	syspp("rm -f ./tmpinfo.txt");
}
auto usbBlck(void) -> void
{
	while (!shared.terminate)
	{
		syspp("lsblk -l | grep -i '^[sh]d[a-z] ' > ./tmpblck.txt");
		std::this_thread::sleep_for(std::chrono::nanoseconds(delay));
		std::ifstream file;
		file.open("./tmpblck.txt");
		if (!file.is_open()) continue;
		while(shared.mutex.try_lock() != true) {};
		auto tmpstream = sstream() << file.rdbuf();
		file.close();
		string tmpstr;
		bool is_cleaned = false;
		while (true)
		{
			getline(tmpstream, tmpstr);
			if (tmpstr.empty())
				break;
			if (!is_cleaned)
			{
				shared.blck_new = sstream();
				is_cleaned = true;
			}
			shared.blck_new << tmpstr.substr(0, tmpstr.find(' ')) << newl;
		}
		shared.mutex.unlock();
		std::this_thread::sleep_for(std::chrono::nanoseconds(delay));
	}
	syspp("rm -f ./tmpblck.txt");
}

auto is_removable(const string sdN) -> bool
{
	const string path = "/sys/block/" + sdN +  "/removable";
	if (!std::filesystem::exists(path)) return false;
	std::ifstream file;
	file.open(path);
	if (!file.is_open()) return false;
	u16 tmpval = 0u;
	file >> tmpval;
	file.close();
	return tmpval == 1u;
}
auto model(const string sdN) -> string
{
	string tmpstr = "Unknown";
	const string path = "/sys/block/" + sdN +  "/device/model";
	if (!std::filesystem::exists(path)) return tmpstr;
	std::ifstream file;
	file.open(path);
	if (!file.is_open()) return tmpstr;
	getline(file,  tmpstr);
	file.close();
	if (tmpstr.empty()) tmpstr = "Unknown";
	return tmpstr;
}

auto main(void) -> i32
{
	auto map = std::map<string, string>();
	auto screen = TCurses();
	TCurses::cBreakable(true);
	auto boxes = std::array{ TCurses(0 * winsize_x, 0 * winsize_y, 1 * winsize_x, 1 * winsize_y),
							 TCurses(1 * winsize_x, 0 * winsize_y, 2 * winsize_x, 1 * winsize_y),
							 TCurses(0 * winsize_x, 1 * winsize_y, 3 * winsize_x, 2 * winsize_y),
							 TCurses(3 * winsize_x, 0 * winsize_y, 2 * winsize_x, 3 * winsize_y) };
	auto titles = std::array<string, 4>{ "Menu", "Block Devices", "USB Devices", "Application Logs" };
	for (auto& win : boxes)
	{
		static u8 index = 0u;
		win.box();
		win.print(1, 0, " " + titles.at(index++) + " ");
		win.update();
	}
	auto	menu = TCurses(0 * winsize_x + 1, 0 * winsize_y + 1, 1 * winsize_x - 2, 1 * winsize_y - 2),
			blck = TCurses(1 * winsize_x + 1, 0 * winsize_y + 1, 2 * winsize_x - 2, 1 * winsize_y - 2),
			info = TCurses(0 * winsize_x + 1, 1 * winsize_y + 1, 3 * winsize_x - 2, 2 * winsize_y - 2),
			logs = TCurses(3 * winsize_x + 1, 0 * winsize_y + 1, 2 * winsize_x - 2, max_logs);

	shared.logs_new << "Application started." << newl;
	auto threads = std::array{ std::thread(usbInfo), std::thread(usbBlck) };

	menu.nodelay(); blck.nodelay();
	menu.echo(false); blck.echo(false);
	menu.keypad(true); blck.keypad(true);
	auto choices = std::array{ "Clear logs", "Unmount Device", "Quit" };
	u32 highlight = 0u;
	while (true)
	{
		static bool first_skip1 = true, first_skip2 = true, terminate = false, redraw = true, umount = false, force_refresh = false;
		std::this_thread::sleep_for(std::chrono::nanoseconds(delay));
		u32 index = 0;
		switch (menu.getchar())
		{
		case 0403: //KEY_DOWN
			if (highlight != 0)
			{
				--highlight;
				redraw = true;
			}
			break;
		case 0402: //KEY_UP
			if (highlight + 1 != choices.size())
			{
				++highlight;
				redraw = true;
			}
			break;
		case '\n': //KEY_ENTER
			switch (highlight)
			{
			case 0:
				shared.logs_new = sstream() << "Logs were cleared." << newl;
				break;
			case 1:
				umount = true;
				break;
			case 2:
				terminate = true;
				break;
			default:
				break;
			}
			break;
		case 'q': case 'Q':
			terminate = true;
		default:
			break;
		}
		if (redraw)
		{
			menu.clear();
			for (auto& choice : choices)
			{
				if (index == highlight)
					menu.attribute(TCurses::Reverse, true);
				menu.print(0, index, sstream() << index + 1 << ". " << choice);
				menu.attribute(TCurses::Reverse, false);
				++index;
			}
			menu.update();
			redraw = false;
		}
		if (terminate) break;
		if (umount)
		{
			while (shared.mutex.try_lock() != true) {}
			sstream memstream = sstream() << shared.blck_new.str();
			shared.mutex.unlock();
			u32 u_highlight = 0u, u_size = 0u;
			bool u_terminate = false, u_redraw = true, u_umount = false;
			while (!u_terminate)
			{
				std::this_thread::sleep_for(std::chrono::nanoseconds(delay));
				if (u_redraw)
				{
					string tmpstr;
					u32 u_index = 0u;
					blck.clear();
					sstream tmpstream = sstream() << memstream.str();
					while (true)
					{
						getline(tmpstream, tmpstr);
						if (tmpstr.empty()) break;
						if (!is_removable(tmpstr)) continue;
						if (u_index == u_highlight)
							blck.attribute(TCurses::Reverse, true);
						blck.print(0, u_index, sstream() << tmpstr << " " << model(tmpstr));
						++u_index;
						blck.attribute(TCurses::Reverse, false);
						u_size = std::max(u_size, u_index);
					}
					if (u_index == u_highlight)
						blck.attribute(TCurses::Reverse, true);
					blck.print(0, u_index, "Quit.");
					blck.move(0, u_index + 1);
					blck.attribute(TCurses::Reverse, false);
					blck.update();
					u_redraw = false;
					force_refresh = true;
				}
				switch (blck.getchar())
				{
				case 0403: //KEY_DOWN
					if (u_highlight != 0)
					{
						--u_highlight;
						u_redraw = true;
					}
					break;
				case 0402: //KEY_UP
					if (u_highlight != u_size)
					{
						++u_highlight;
						u_redraw = true;
					}
					break;
				case '\n':
					u_umount = true;
					break;
				case 'q': case 'Q':
					u_terminate = true;
					break;
				default:
					break;
				}
				if (u_umount)
				{
					if (u_highlight == u_size) break;
					string tmpstr, sdN;
					sstream tmpstream = sstream() << memstream.str();
					u32 u_index = 0u;
					while (true)
					{
						getline(tmpstream, tmpstr);
						if (tmpstr.empty()) break;
						if (!is_removable(tmpstr)) continue;
						if (u_index == u_highlight) break;
						++u_index;
					}
					if (tmpstr.empty()) break;
					syspp("lsblk -l | grep -i '^" + tmpstr + "[^ ]' > ./tmplist.txt");
					std::ifstream file;
					file.open("./tmplist.txt");
					sstream list;
					if (file.is_open())
					{
						list = sstream() << file.rdbuf();
						file.close();
					}
					syspp("rm -f ./tmplist.txt");
					shared.logs_new << "Unmounting " << tmpstr << " (";
					string u_tmpstr;
					bool is_first = true;
					std::list<string> sdNs;
					while (true)
					{
						getline(list, u_tmpstr);
						if (u_tmpstr.empty()) break;
						if (!is_first) shared.logs_new << ", ";
						is_first = false;
						sdNs.push_back(u_tmpstr);
						shared.logs_new << sdNs.back().substr(0, u_tmpstr.find(' '));
					}
					shared.logs_new << ")..." << newl;
					bool umount_failed = false;
					for (auto& sdNn : sdNs)
					{
						shared.logs_new << sdNn.substr(0, sdNn.find(' ')) << " ";
						string mount_point = sdNn.substr(31, sdNn.size() - 31);
						if (mount_point.empty())
						{
							shared.logs_new << "not mounted" << newl;
							continue;
						}
						shared.logs_new << "'" << mount_point << "': ";
						syspp("sudo umount -R '" + mount_point + "' 2> ./tmpumnt.txt");
						std::ifstream file;
						file.open("./tmpumnt.txt");
						if (file.is_open())
						{
							getline(file, u_tmpstr);
							if (u_tmpstr.empty())
								shared.logs_new << "success" << newl;
							else
							{
								shared.logs_new << "failed" << newl;
								umount_failed = true;
							}
							file.close();
						}
						else
						{
							shared.logs_new << "failed" << newl;
							umount_failed = true;
						}
						syspp("rm -f ./tmpumnt.txt");
					}
					shared.logs_new << (umount_failed ? "Failed to unmount" : "Successfully unmounted")
									<< " " << model(tmpstr) << newl;
					u_terminate = true;
				}
			}
			umount = false;
		}
		if (shared.mutex.try_lock() == true)
		{
			if (shared.info_new.str() != shared.info_old.str())
			{
				info.clear();
				u16 index = 0u;
				string tmpstr;
				while (true)
				{
					getline(shared.info_new, tmpstr);
					if (tmpstr.empty()) break;
					info.print(sstream() << ++index << ". " << tmpstr << newl);
					if (first_skip1) continue;
					if (shared.info_old.str().find(tmpstr) != string::npos) continue;
					shared.logs_new << "USB device added: " << first_two_words(tmpstr) << "..." << newl;
				}
				while (!first_skip1)
				{
					getline(shared.info_old, tmpstr);
					if (tmpstr.empty()) break;
					if (shared.info_new.str().find(tmpstr) != string::npos) continue;
					shared.logs_new << "USB device removed: " << first_two_words(tmpstr) << "..." << newl;
				}
				first_skip1 = false;
				info.update();
				shared.info_old = sstream() << shared.info_new.str();
			}
			if (force_refresh || shared.blck_new.str() != shared.blck_old.str())
			{
				force_refresh = false;
				blck.clear();
				string tmpstr;
				while(true)
				{
					getline(shared.blck_new, tmpstr);
					if (tmpstr.empty()) break;
					blck.print(sstream() << tmpstr << " " << (is_removable(tmpstr) ? " is " : "non-") << "removable " << model(tmpstr) << newl);
					if (first_skip2) continue;
					if (shared.blck_old.str().find(tmpstr) == string::npos)
						shared.logs_new << "Block device added: " << tmpstr << "." << newl;
				}
				while(!first_skip2)
				{
					getline(shared.blck_old, tmpstr);
					if (tmpstr.empty()) break;
					if (shared.blck_new.str().find(tmpstr) == string::npos)
						shared.logs_new << "Block device removed: " << tmpstr << "." << newl;
				}
				first_skip2 = false;
				blck.update();
				shared.blck_old = sstream() << shared.blck_new.str();
			}
			shared.mutex.unlock();
		}
		if (shared.logs_new.str() != shared.logs_old.str())
		{
			logs.clear();
			sstream tmpstream = sstream() << shared.logs_new.str();
			string tmpstr;
			u32 counter = 0;
			while (true)
			{
				getline(tmpstream, tmpstr);
				if (tmpstr.empty()) break;
				++counter;
			}
			if (counter > max_logs)
			{
				tmpstream = sstream();
				for (u32 i = 0; i < (counter - max_logs); ++i)
					getline(shared.logs_new, tmpstr);
				while (true)
				{
					getline(shared.logs_new, tmpstr);
					if (tmpstr.empty()) break;
					tmpstream << tmpstr << newl;
				}
				shared.logs_new = sstream() << tmpstream.str();
			}
			logs.print(shared.logs_new);
			logs.update();
			shared.logs_old = sstream() << shared.logs_new.str();
		}
	}
	shared.terminate = true;
	for (auto& thread : threads)
		thread.join();
	return 0;
}
