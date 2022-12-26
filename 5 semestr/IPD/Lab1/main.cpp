#include <iostream>
#include <thread>
#include <cstdio>

#include <unistd.h>
#include <termios.h>
#include <sys/types.h>

#include "battery.hpp"
#include "lpalette/lpalette.hpp"

void CheckRoot(void){
	if(getuid()){
		std::cerr << "Please, run as root" << std::endl;
		std::terminate();
	}
}

int CheckSystemCommandSuccess(){
	/*
	Function to check success of function system() by checking creating of file '1'

	return:	0 - all good
			-1 - fail
	*/
	FILE *checker;
	if ((checker = fopen("1", "r"))){
		fclose(checker);
		system("rm 1");
		return 0;
	}
	else
		return -1;
}

int installDaemon(){
	/*
	Installs custom implemented service to watch the battery

	returns:	-1 if function fail
				0 if function success
	*/
	system("clear");
	//checking if daemon files exist
	FILE *checker;
	if ((checker = fopen("daemon/batteryUpTime.sh","r"))){
		fclose(checker);
		if((checker = fopen("daemon/IPD_batteryDaemon.service", "r")))
			fclose(checker);
		else{
			std::cout << tlib::LPalette::Red() << "Error: cannot find daemon service" << tlib::LPalette::Reset() << std::endl;
			sleep(2);
			return -1;
		}
	} else {
		std::cout << tlib::LPalette::Red() << "Error: cannot find daemon script" << tlib::LPalette::Reset() << std::endl;
		sleep(2);
		return -1;
	}

	std::cout << "Install deamon log:\n";
	system("mkdir /opt/IPD_battery && touch 1");
	sleep(1);
	if (CheckSystemCommandSuccess() == -1){
		std::cout << tlib::LPalette::Red() << "Error: cannot create directory" << tlib::LPalette::Reset() << std::endl;
		sleep(2);
		return -1;
	}

	system("cp daemon/batteryUpTime.sh /opt/IPD_battery/ && cp daemon/IPD_batteryDaemon.service /etc/systemd/system/ && touch 1");
	sleep(1);
	if(CheckSystemCommandSuccess() == -1){
		std::cout << tlib::LPalette::Red() << "Error: failed copy of files" << tlib::LPalette::Reset() << std::endl;
		sleep(2);
		return -1;
	}

	system("systemctl daemon-reload && systemctl enable IPD_batteryDaemon.service && systemctl start IPD_batteryDaemon.service && touch 1");
	sleep(1);
	if(CheckSystemCommandSuccess() == -1){
		std::cout << tlib::LPalette::Red() << "Error: cannot use systemctl functions" << tlib::LPalette::Reset() << std::endl;
		sleep(2);
		return -1;
	}
	return 0;
}

int uninstallDaemon(){
	/*
	Deletes custom implemented service from the system

	return:	0 - daemon deleted
			-1 - error
	*/
	system("clear");
	std::cout << "Uninstall log:\n";
	system("systemctl disable IPD_batteryDaemon.service && systemctl stop IPD_batteryDaemon.service && touch 1");
	sleep(1);
	if(CheckSystemCommandSuccess() == -1){
		std::cout << tlib::LPalette::Red() << "Error: cannot use systemctl functions" << tlib::LPalette::Reset() << std::endl;
		sleep(2);
		return -1;
	}

	system("rm -rf  /opt/IPD_battery/ && rm /etc/systemd/system/IPD_batteryDaemon.service && touch 1");
	sleep(1);
	if(CheckSystemCommandSuccess() == -1){
		std::cout << tlib::LPalette::Red() << "Error: cannot delete service files" << tlib::LPalette::Reset() << std::endl;
		sleep(2);
		return -1;
	}

	system("systemctl daemon-reload");
	sleep(1);
	return 0;
}

void mainThread(char *key){
	/*
	Thread that works with all user input

	char *key: letter from main thread that always changes
	return: nothing
	*/
	while(true){
		system("clear");
		Battery::Run();
		std::cout << tlib::LPalette::Yellow() << "i/u" << tlib::LPalette::Reset() << " -- install/uninstall daemon service" << std::endl
				  << tlib::LPalette::Yellow() << " s " << tlib::LPalette::Reset() << " -- activate sleep" << std::endl
				  << tlib::LPalette::Yellow() << " h " << tlib::LPalette::Reset() << " -- activate hibernate" << std::endl
				  << tlib::LPalette::Yellow() << " q " << tlib::LPalette::Reset() << " -- quit program" << std::endl;
		sleep(1);
		switch((*key)){
			case 'i': installDaemon(); break;
			case 'u': uninstallDaemon(); break;
			case 'h': Battery::Hibernate(); break;
			case 's': Battery::Sleep(); break;
			case 'q': return;
			default: continue;
		}
		(*key) = '~';
	}
	return;
}

int main(void){
	CheckRoot();

	char key = '~';
	std::thread mT(mainThread, &key);

	//no enter input set up
	struct termios old_tio, new_tio;
	// get the terminal settings for stdin
	tcgetattr(STDIN_FILENO,&old_tio);
	new_tio=old_tio;
	new_tio.c_lflag &=(~ICANON & ~ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

	while(true){
		key = getc(stdin);
		if(key == 'q')
			break;
	}

	mT.join();
	tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
	return 0;
}
