#include <opencv4/opencv2/opencv.hpp>
#include <ncurses.h>
#include <iostream>
#include <malloc.h>
#include <filesystem>
#include <fstream>
#include <list>

using namespace cv;
using namespace std;


void InitWindows(WINDOW *menu, WINDOW *log, WINDOW *input){
	// setting up menu
	box(menu,0,0);
	mvwprintw(menu,0,0,"Menu");
	// setting up log
	box(log,0,0);
	mvwprintw(log,0,0,"Log");
	// setting up input
	box(input,0,0);
	mvwprintw(input,0,0,"Input");
}


/*
	mode:
	1 -- return possible name for photo-file
	2 -- return possible name for video-file

	returns string with full path (like files/video1.avi)
*/
string GetFileName(int mode){
	string fileName = "files/";
	int num = 1; string name;
	mode == 1 ? name+="photo" : name+="video";
	name += to_string(num);
	mode == 1 ? name+=".png" : name+=".avi";
	while(filesystem::exists(fileName+name)){
		num++;
		mode == 1 ? name = "photo" : name = "video";
		name += to_string(num);
		mode == 1 ? name+=".png" : name+=".avi";
	}
	fileName += name;
	return fileName;
}


int MakePhoto(){
	VideoCapture cap(0);
	if (!cap.isOpened())	return -1;
	// image capture
	Mat img;
	cap >> img;
	imwrite(GetFileName(1),img);
	cap.release();
	return 0;
}

int MakeVideo(int amountFrames){
	if(amountFrames == -1)	return -1;
	VideoCapture cap(0);
	if (!cap.isOpened())	return -2;

	// saving frame to get webcam resolution
	Mat frame;
	cap >> frame;
	// NAME , VIDEO CODEC , FPS , SIZE
	VideoWriter video(GetFileName(2),cv::VideoWriter::fourcc('M','J','P','G'),30, frame.size());
	if(!video.isOpened())	return -1;
	int i = 0;
	// video capture
	while (i != amountFrames) {
		// capturing frame by frame
		cap >> frame;
		if (frame.empty())	break;
		video << frame;
		i++;
	}
	cap.release();
	video.release();
	return 0;
}

void AppendLogs(WINDOW *log, string logs[], string newLine){
	// replacing log message
	for(int i = 0; i < 3; i++)
		logs[i] = logs[i+1];
	logs[3] = newLine;
	while(logs[3].size() < 36)
	   logs[3] += " ";
	// placing in window
	for(int i = 0; i < 4; i++)
		mvwprintw(log,i+1,1,"%s", logs[i].c_str());
	wrefresh(log);
}

int WriteNumericData(WINDOW *input){
	string ClearInput = "                              ";
	wmove(input,1,1);
	wrefresh(input);
	int size = 0;
	string result;
	char* temp = (char*)malloc(2); temp[1] = '\0';
	while(true){
		temp[0] = getch();
		if (temp[0] == '\n')
			break;
		if (temp[0] == 'q'){
			mvwprintw(input,1,1,"%s", ClearInput.c_str());
			wrefresh(input);
			free(temp);
			return -1;
		}
		if((temp[0] == '\b' || temp[0] == 127) && size > 0){
			temp[0] = ' ';
			mvwprintw(input,1,1+(--size),"%s", temp);
			wmove(input,1,1+size);
			wrefresh(input);
			result.resize(result.size()-1);
			continue;
		}
		if (temp[0] < '0' || temp[0] > '9' || size >= 25)
			continue;
		mvwprintw(input,1,1+size,"%s", temp);
		wrefresh(input);
		result += temp[0];
		size++;
	}
	mvwprintw(input,1,1,"%s", ClearInput.c_str());
	free(temp);
	try{
		stoi(result);
	} catch (...){
		return -3;
	}
	return stoi(result);
}

/*
	Reads info about video0 and output in info window
*/
void OutputInfoAboutWebcam(){
	WINDOW *info = newwin(10,61,0,0);
	box(info,0,0);
	mvwprintw(info,0,1,"%s","Info");

	string webCam = "/sys/class/video4linux/video0"; int index = 0;
	list<string> allData;
	while(filesystem::exists(webCam)){
		webCam+="/device";
		std::ifstream files;
		string temp;
		// obtaining webcam name
		files.open(webCam+"/interface");
		getline(files,temp);
		allData.push_back("Camera name: " + temp);
		files.close();
		for (const auto & entry : filesystem::directory_iterator(webCam)){
			string path = entry.path();
			if(path == webCam+"/interface" || path == webCam + "/uevent")continue;
			files.open(path);
			getline(files,temp);
			files.close();
			if (temp == "")continue;
			// obtaining param name (from file name)
			string paramName;
			for(int i = path.size()-1;path[i] != '/';i--)
				paramName += path[i];
			reverse(paramName.begin(),paramName.end());
			paramName = paramName + ": " + temp;
			allData.push_back(paramName); // sending to all data list
		}
		allData.push_back(string(59,'-'));
		webCam = "/sys/class/video4linux/video" + to_string(++index);
	}
	mvprintw(10,0,"q to exit");
	refresh();
	// if linux does not have camera or does not support v4l
	if(allData.size() < 10)
		for(int i = 0; i < 10; i++)
			allData.push_back("Error while working with v4l");
	// output
	size_t posList = 0, choice = 1;
	keypad(info,true);
	while (choice != 'q'){
		for(int i = 0; i < 8; i++){
			auto listData = allData.begin();
			advance(listData,i+posList);
			mvwprintw(info,i+1,1,"%s",(*listData).c_str());
		}
		choice = wgetch(info);
		switch (choice){
			case KEY_UP: posList == 0 ? posList = posList : --posList;							break;
			case KEY_DOWN: posList+8 == allData.size()-1 ? posList = posList : ++posList;		break;
			case 113:																			break; // 'q'
			default:																			break;
		}
		for(int i = 1; i < 9; i++)
			for(int j = 1; j < 60; j++)
				mvwprintw(info,i,j,"%c",' ');
	}
	clear();
	refresh();
}

int main() {
	initscr();
	curs_set(0);
	// creating folder for images
	if(!filesystem::exists("files"))
		filesystem::create_directory("files");
	// checking size
	int yMax, xMax;
	getmaxyx(stdscr,yMax,xMax);
	if(yMax < 11 || xMax < 62){
		endwin();
		cout << "Make terminal bigger!" << endl;
		return -1;
	}
	noecho();
	WINDOW *menu = newwin(10,12,0,0);
	WINDOW *log = newwin(6,38,0,12);
	WINDOW *input = newwin(4,38,6,12);

	InitWindows(menu,log,input);
	// refreshing console
	refresh();
	wrefresh(menu);
	wrefresh(log);
	wrefresh(input);

	keypad(menu, true); // allows to use arrows
	int highlight = 0; // highlight current option
	bool cycle_active = true;
	string choices[4] = { "Make photo", "Make video", "Info      ", "Quit      "};  // menu buttons
	string logs[4];  // logs menu (26 symbol line)
	while(cycle_active){
		// output menu
		for(int i = 0; i < 4; i++){
			if (i == highlight)
				wattron(menu, A_REVERSE);
			mvwprintw(menu,i+1,1,"%s", choices[i].c_str());
			wattroff(menu, A_REVERSE);
		}
		// input mode
		int choice = wgetch(menu);
		switch (choice){
			case KEY_UP: highlight == 0 ? highlight = 3 : highlight--;					break;
			case KEY_DOWN: highlight == 3 ? highlight = 0 : highlight++;				break;
			case 10:{	// enter pressed
				if (highlight == 3) cycle_active = false;
				else if (highlight == 1){
					curs_set(1);
					AppendLogs(log,logs,"How many frames you want to capture?");
					int result = MakeVideo(WriteNumericData(input));
					switch (result){
						case  0: AppendLogs(log,logs,"Successfully captured video");	break;
						case -1: AppendLogs(log,logs,"Canceled");						break;
						case -2: AppendLogs(log,logs,"Could not open camera");			break;
						case -3: AppendLogs(log,logs,"Too big number");					break;
						default: AppendLogs(log,logs,"Error occurred");					break;
					}
					curs_set(0);
				}	else if (highlight == 0) MakePhoto() == 0 ? AppendLogs(log,logs,"Successfully captured photo") : AppendLogs(log,logs,"Could not open camera");
					else if (highlight == 2){
						OutputInfoAboutWebcam();
						// recovering windows
						wclear(menu); wclear(log); wclear(input);
						InitWindows(menu,log,input);
						wrefresh(menu); wrefresh(log); wrefresh(input);
						AppendLogs(log,logs,"Checked info window");
					}
				break;
			}
			default: break;
		}
	}
	endwin();
	return 0;
}
