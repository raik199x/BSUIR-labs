#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include <filesystem>
#include <ncurses.h>
#include <thread>
#include <fstream>

bool __Bluetooth__ = false;

void AppendLogs(WINDOW *log, std::string logs[], std::string newLine){
	// replacing log message
	for(int i = 0; i < 5; i++)
		logs[i] = logs[i+1];
	logs[5] = newLine;
    wclear(log);
    box(log,0,0);
    mvwprintw(log,0,0,"%s","Log");
	// placing in window
	for(int i = 0; i < 6; i++)
        if(logs[i] == "") continue;
        else mvwprintw(log,i+1,1,"%s", logs[i].c_str());
	wrefresh(log);
}

std::string GettingInput(WINDOW *input){
    std::string result; char letter = '1'; unsigned int pos = 1;
    while(1){
        letter = wgetch(input);
        if(letter == '\n') break;
        else if (letter == KEY_BACKSPACE || letter == 127){
            if (pos < 2) continue;
            mvwprintw(input,1,--pos,"%c",' ');
            result.resize(result.size()-1);
            continue;
        } else if (pos == 101) continue;
        result+=letter;
        mvwprintw(input,1,pos++,"%c",letter);
    }
    wclear(input);
    box(input,0,0);
    mvwprintw(input,0,0,"%s","Input");
    wrefresh(input);
    return result;
}

/* 
Checks if bluetooth is active and changes flag 
*/
void Monitoring(WINDOW *monitoring){
    // bluetoothctl show | grep Powered | cut -d " " -f 2
    std::string connectedDevices[6];
    while(1){
        sleep(3);
        for(int i = 0; i < 5; i++) connectedDevices[i] = "";
        // checking if bluetooth is on
        FILE* pipeFile = popen("bluetoothctl show | grep Powered | cut -d \" \" -f 2", "r");
        char c = fgetc(pipeFile);
        __Bluetooth__ = c == 'y';
	    pclose(pipeFile);
        // clearing window
        wclear(monitoring);
        box(monitoring,0,0);
        mvwprintw(monitoring,0,0,"Monitoring");

        if (__Bluetooth__ == false) {
            mvwprintw(monitoring,1,1,"Bluetooth is off");
            wrefresh(monitoring);
            continue;
        }

        // finding available devices
        system("bluetoothctl paired-devices | cut -d \" \" -f 2 > devices.txt");
        std::ifstream getAddresses("devices.txt"); std::string lines;
        while(getline(getAddresses,lines)){
            if(lines == "Device") continue;
            // check if device is connected
            std::string command = "bluetoothctl info " + lines + " | grep Connected | cut -d \" \" -f 2";
            pipeFile = popen(command.c_str(),"r");
            char c = fgetc(pipeFile);
            pclose(pipeFile);
            if(c == 'n') continue;

            // device connected. obtaining name
            command = "bluetoothctl info " + lines + " | grep Name | cut -d \" \" -f 2";
            pipeFile = popen(command.c_str(),"r"); 
            command.clear(); // we will use command for storing device name
            while(1){
                c = fgetc(pipeFile);
                if(feof(pipeFile)) break;
                if (c == '\n') continue;
                command += c;
            }
            for(int i = 0; i < 5; i++)
                if(connectedDevices[i] == ""){
                    connectedDevices[i] = command;
                    break;
                }
            pclose(pipeFile);
        }
        for(int i = 0; i < 5; i++){
            if(connectedDevices[i] == "") continue;
            mvwprintw(monitoring,i+1,1,"%s",connectedDevices[i].c_str());
        }
        wrefresh(monitoring);
    }
}

// inits connection via bluetoothctl
std::string innitConnection(std::string address){
    std::string command, response; const char *convert;
    command = "bluetoothctl connect " + address;
    convert = command.c_str();
    FILE* pipeFile = popen(convert, "r");
	while (1){
		char c = fgetc(pipeFile);
		if (feof(pipeFile)) break;
        response+=c;
	}
    pclose(pipeFile);
    if(response.find("Connection successful") < 1000)
        return "Connected!";
    else 
        return "Not connected!";
}

std::string deleteConnection(std::string address){
    std::string command, response; const char *convert;
    command = "bluetoothctl disconnect " + address + " | grep disconnected";
    convert = command.c_str();
    FILE* pipeFile = popen(convert, "r");
	char c = fgetc(pipeFile);
    pclose(pipeFile);
    if(c == 'S')
        return "Disconnected";
    else 
        return "Error";
}

// inits file send via gnome bluetooth send
std::string sendFile(std::string address, std::string pathToFile){
    std::string command, response; const char *convert;

    // processing file path so linux can use it
    std::string processing;
    for(int i = 0; i < pathToFile.size();i++){
        if(pathToFile[i] == ' '){
            processing+="\\ ";
            continue;
        }
        processing += pathToFile[i];
    }
    if(!std::filesystem::exists(pathToFile)) return "File not found";
    pathToFile = '"' + pathToFile + '"';


    command = "bluetooth-sendto --device=" + address + " " + pathToFile;
    convert = command.c_str();
    system(convert);
    return "Conversation ended";
}

/*
    Thread that outputs scanned devices
*/
void OutputScan(WINDOW *scan){
    system("bluetoothctl scan on 1> /dev/null 2> /dev/null &");
    unsigned int line = 0;
    while(1){
        sleep(3);
        if(!std::filesystem::exists("scanned.txt")) continue;

        wclear(scan);
        box(scan,0,0);
        mvwprintw(scan,0,0,"Scan");

        std::ifstream reader("scanned.txt");
        std::string lines;
        // skipping used lines
        for(int i = 0; i < line; i++) getline(reader,lines);
        for(int i = 0; i < 6; i++){
            if(!getline(reader,lines)){
                line = 0;
                break;
            }
            mvwprintw(scan,1+i,1,"%s",lines.c_str());
            line++;
        }
        if(!getline(reader,lines)) line = 0;
        wrefresh(scan);
    }
}

int main(){
    initscr();
    curs_set(0);
    noecho();

    int menuX = 13, menuY= 8;
    WINDOW *menu = newwin(menuY,menuX,0,0);
    box(menu,0,0);
	mvwprintw(menu,0,0,"Menu");
    wrefresh(menu);

    WINDOW *monitoring = newwin(8,20,0,menuX);
    box(monitoring,0,0);
	mvwprintw(monitoring,0,0,"Monitoring");
    wrefresh(monitoring);

    WINDOW *scan = newwin(8,50,0,menuX+20);
    box(scan,0,0);
	mvwprintw(scan,0,0,"Scan");
    wrefresh(scan);

    WINDOW *log = newwin(8,20,0,menuX+70); std::string logs[6];
    box(log,0,0);
	mvwprintw(log,0,0,"Log");
    wrefresh(log);

    WINDOW *input = newwin(3,menuX+90,8,0);
    box(input,0,0);
	mvwprintw(input,0,0,"Input");
    wrefresh(input);

    std::thread bluetoothParameters(Monitoring,monitoring);
    bluetoothParameters.detach();
    std::thread bluetoothScanner(OutputScan,scan);
    bluetoothScanner.detach();
    sleep(3); // wait until all is initialized
    keypad(menu, true); // allows to use arrows
	int highlight = 0; // highlight current option
    std::string choices[5] = { "Scan", "Connect", "Disconnect", "Send file", "Exit"};
    bool cycleActive = true;
    while(cycleActive){
        for(int i = 0; i < 5; i++){
			if (i == highlight)
				wattron(menu, A_REVERSE);
			mvwprintw(menu,i+1,1,"%s", choices[i].c_str());
			wattroff(menu, A_REVERSE);
		}
        int choice = wgetch(menu);
        switch (choice){
			case KEY_UP: highlight == 0 ? highlight = 4 : highlight--;					break;
			case KEY_DOWN: highlight == 4 ? highlight = 0 : highlight++;				break;
			case 10:{	// enter pressed
                if (highlight == 0 && __Bluetooth__)
                    system("bluetoothctl devices | cut -d \" \" -f 2- > scanned.txt");
                else if (highlight == 1 && __Bluetooth__){
                    AppendLogs(log,logs,"Enter baddr");
                    AppendLogs(log,logs,innitConnection(GettingInput(input)));
                } else if(highlight == 2 && __Bluetooth__){
                    AppendLogs(log,logs,"Enter baddr");
                    AppendLogs(log,logs,deleteConnection(GettingInput(input)));
                } else if(highlight == 3 && __Bluetooth__){
                    AppendLogs(log,logs,"Enter path to file");
                    std::string path = GettingInput(input);
                    AppendLogs(log,logs,"Enter baddr");
                    AppendLogs(log,logs,sendFile(GettingInput(input), path));
                }
                else if (highlight == 4) cycleActive = false;
                break;
            }
			default:                                                                    break;
		}
    }

    endwin();
    if(std::filesystem::exists("devices.txt")) remove("devices.txt");
    if(std::filesystem::exists("scanned.txt")) remove("scanned.txt");
    system("kill $(ps | grep bluetoothctl | cut -d ' ' -f 3)");
    return 0;
}
