#include <fstream>
#include <filesystem>

#include "battery.hpp"
#include "lpalette/lpalette.hpp"

/*!
 * \name BatteryUtils
 * \brief Hidden realization of Battery namespace methods
 */
namespace BatteryUtils
{
	using namespace tlib;
	void CheckForBatterySupport(void);
	string FileReader(const string path);
	string ObtainInfo(const string file);
	long double Energy(const string path2energy1, const string path2energy2);
	void FileWriter(const string path, const string message);
	void SendInfo(const string file, const string message);
	void SecondAfterLastCharge(void);
}

/*!
 * \brief Checks for existing "/sys/class/power_supply/BAT0" directory
 */
void BatteryUtils::CheckForBatterySupport(void)
{
	if (std::filesystem::exists("/sys/class/power_supply/BAT0")) return;
	std::cerr << "Battery was not found on your device." << std::endl;
	std::terminate();
}

/*!
 * \brief Reads file from provided path
 * \param path: Path to the file to be read
 * \return string value of the file's content
 */
string BatteryUtils::FileReader(const string path)
{
	/*
	Reads data from file

	path: path to file
	return: string value containing
	*/
	std::ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		std::cerr << "Failed to open the file '" << path << "'";
		std::terminate();
	}
	std::stringstream stream;
	stream << file.rdbuf();
	file.close();
	string string = stream.str();
	if (string.back() == '\n') string.back() = '\0';
	return string;
}

/*!
 * \brief Obtains information from "/sys/class/power_supply/BAT0/" files
 * \param file: file name in '$BAT0' directry
 * \return same as BatteryUtils::FileReader
 */
string BatteryUtils::ObtainInfo(const string file)
{
	return FileReader("/sys/class/power_supply/BAT0/" + file);
}

/*!
 * \brief Battery Energy information obtainer
 * \return Battery Energy level as long double (in percents)
 */

/*!
 * \brief Battery Energy information obtainer
 * \param path2energy1: either "now" or "full"
 * \param path2energy2: either "full" or "full_design"
 * \return Battery charge level for "now" and "full", battery health level for "full" and "full_design" parameters
 * \warning Method behaviour with other input parameters combinations is undefined
 */
long double BatteryUtils::Energy(const string path2energy1, const string path2energy2)
{
	const long double now = std::strtold(ObtainInfo("energy_" + path2energy1).c_str(), nullptr),
			full = std::strtold(ObtainInfo("energy_" + path2energy2).c_str(), nullptr);
	return now / full * 100;
}

/*!
 * \brief Writes provided string into the file whith provided path
 * \param path: Path to the file to be written
 * \param message: Data to be written
 */
void BatteryUtils::FileWriter(const string path, const string message)
{
	std::ofstream file;
	file.open(path);
	if (!file.is_open())
	{
		std::cerr << "Failed to open the file '" << path << "'";
		std::terminate();
	}
	file << message;
	file.close();
}

/*!
 * \brief Obtains information to "/sys/power/" files
 * \param file: file name in '$POWER' directory
 * \param message: same as BatteryUtils::FileWriter
 */
void BatteryUtils::SendInfo(const string file, const string message)
{
	return FileWriter("/sys/power/" + file, message);
}

/*!
 * \brief Obtain time spend after last charge
 */
void BatteryUtils::SecondAfterLastCharge(void){
	if (!(std::filesystem::exists("/opt/IPD_battery/batteryUpTime.sh"))){
		std::cerr << LPalette::Yellow() << "Daemon service is not setted up" << LPalette::Reset() << std::endl;
		return;
	}
	std::cout << LPalette::White();
	if (!(std::filesystem::exists("/opt/IPD_battery/lastDisBattery.txt"))){
		std::cout << "0:00:00" << LPalette::Reset() << std::endl;
		return;
	}
	//obtaining saved time
	string last = FileReader("/opt/IPD_battery/lastDisBattery.txt");
	//obtaining current time
	system("echo $(date +%s) > /opt/IPD_battery/currentTime.txt");
	string current = FileReader("/opt/IPD_battery/currentTime.txt");
	system("rm /opt/IPD_battery/currentTime.txt");

	long long timeSpend = stoi(current) - stoi(last);

	int sec = 0, min = 0;
	//obtaining minutes
	while(timeSpend - 60 >= 0){
		min+=1;
		timeSpend-=60;
	}
	sec = timeSpend;
	timeSpend = 0;
	//obtaining hours
	while(min - 60 >= 0){
		min-=60;
		timeSpend+=1;
	}
	std::cout << timeSpend << ":";
	if (min < 10) std::cout << "0";
	std::cout << min << ":";
	if (sec < 10) std::cout << "0";
	std::cout << sec << LPalette::Reset() << std::endl;

	// output remain time

}

string Battery::GetSavingStatus(){
	// getting user
	string user;
	FILE* pipeFile = popen("who | cut -d ' ' -f 1 | head -n 1", "r");
	while (1){
		char c = fgetc(pipeFile);
		if (feof(pipeFile)) break;
		if (c == '\n') continue;
		user += c;
	}
	pclose(pipeFile);
	// getting user settings
		// getting path to user setting
	string path = "/home/" + user + "/.config/powerdevilrc";
	if(!std::filesystem::exists(path))
		return "No user settings found";
	// preparing command
	string command = "cat " + path + " | grep \"BatteryCriticalLevel\" | cut -d '=' -f 2";
	// reading data
	pipeFile = popen(command.c_str(), "r");
	string critStatus, lowStatus;
	while(1){
		char c;
		fscanf(pipeFile, "%c", &c);
		if (feof(pipeFile)) break;
		if (c == '\n') continue;
		critStatus += c;
	}
	pclose(pipeFile);
	// check
	if(critStatus == "") critStatus = "5";
	// prep command 2
	command = "cat " + path + " | grep \"BatteryLowLevel\" | cut -d '=' -f 2";
	// reading
	pipeFile = popen(command.c_str(), "r");
	while(1){
		char c;
		fscanf(pipeFile, "%c", &c);
		if (feof(pipeFile)) break;
		if (c == '\n') continue;
		lowStatus += c;
	}
	pclose(pipeFile);
	if (lowStatus == "") lowStatus = "15";
	string cap = BatteryUtils::FileReader("/sys/class/power_supply/BAT0/capacity");
	if (stoi(cap) < stoi(critStatus))
		return "Critical level mode";
	if (stoi(cap) < stoi(lowStatus))
		return "Low level mode";
	return "None";
}

/*!
 * \brief Main method for obtaining battery info
 */
void Battery::Run(void)
{
	using namespace std;
	using namespace BatteryUtils;

	CheckForBatterySupport();

	static const string name = ObtainInfo("model_name"), manufacturer = ObtainInfo("manufacturer");
	static const string technology = ObtainInfo("technology");
	static long double health = Energy("full", "full_design");

	cout << "Battery name:" << "\t\t" << LPalette::Yellow() << name << LPalette::Reset() << endl
		 << "Battery manufacturer:" << '\t' << LPalette::Yellow() << manufacturer << LPalette::Reset() << endl
		 << "Battary technology:" << '\t' << LPalette::Blue() << technology << LPalette::Reset() << endl
		 << "Battery charge status:" << '\t' << LPalette::Yellow(true) << ObtainInfo("status") << LPalette::Reset() << endl
		 << "Battery charge level:" << '\t' << LPalette::Blue(true) << Energy("now", "full") << "%" << LPalette::Reset() << endl
		 << "Bettery health level:" << '\t' << LPalette::Magenta() << health << "%" << LPalette::Reset() << endl
		 << "Power save mode status:" << '\t' << LPalette::Red() << GetSavingStatus() << LPalette::Reset() << endl
		 << "After last charging:" << '\t';
	system(" upower -i `upower -e | grep 'BAT'` | grep 'time to empty' ");
	SecondAfterLastCharge();
}

/*!
 * \brief Send machine to sleep state method
 */
void Battery::Sleep(void)
{
	BatteryUtils::SendInfo("mem_sleep", "deep");
	BatteryUtils::SendInfo("state", "mem");
}

/*!
 * \brief Send machine to hibernation state method
 */
void Battery::Hibernate(void)
{
	BatteryUtils::SendInfo("mem_sleep", "deep");
	BatteryUtils::SendInfo("state", "disk");
}
