#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

using namespace std;

void enter_to_cont(){
    cout << "Press enter to cont" << endl;
    string temp;
    getline(cin,temp);
}

int write_num(){
    int number;
    try{
        cin >> number;
        if (cin.fail())
            throw(1);
    } catch(int a){
        if (a == 1){
            cin.clear();
            cin.ignore(10000,'\n');
            enter_to_cont();
            return -229;
        }
    }
    return number;
}

string write_name(){
    system("clear");
    string name;
    cout << endl << "Write file name: ";
    cin >> name;
    return name;
}

void count_letters(string name_of_file){
    system("clear");
    cin.ignore(1000,'\n');
    if (name_of_file == "None"){
        cout << "You didnt write the name of file!\n";
        enter_to_cont();
        return;
    }

    char letter;
    cout << "Enter letter to count" << endl;
    cin >> letter;

    vector<char> lines; fstream file;
    cout << "Filename: " << name_of_file << endl << "Letter to find: " << letter << endl << "Status: ";
    system("sleep 0.5");
    if(filesystem::exists(name_of_file) == false){
           cout << "Failed to open\nAborted\n";
           cin.ignore(10000,'\n');
           enter_to_cont();
           return;
    }
    file.open(name_of_file, fstream::app | fstream::in);
    cout << "Opened successfull\n";
    system("sleep 1");
    int line_number = 1;
    while(true){
        int count = 0;
        string new_line;
        getline(file,new_line);
        if(file.eof())
            break;
        for(unsigned int i = 0; i < new_line.length();i++)
            lines.push_back(new_line[i]);
        lines.push_back('\0');
        for(vector<char>::iterator iter = lines.begin(); iter != lines.end();iter++)
            if(letter == *iter)
                count++;
        cout << line_number << ": " << count << endl;
        line_number++;
        lines.clear();
    }
    cin.ignore(1000,'\n');
    cout << endl;
    enter_to_cont();
    file.close();
}

void add_lines(string name_of_file){
    cin.ignore(10000,'\n');
    if (name_of_file == "None"){
        cout << "You didnt write the name of file!\n";
        enter_to_cont();
        return;
    }
    if(filesystem::exists(name_of_file) == false){
           cout << "File wasnt founded on disk\nOperation aborted\n";
           cin.ignore(10000,'\n');
           enter_to_cont();
           return;
    }
    fstream file(name_of_file, fstream::app | fstream::in | fstream::ate);
    cout << endl << "Enter line to add\n>";
    string temp; getline(cin,temp);
    file << temp << '\n';
    file.close();
}

void show_data(string name_of_file){
    cin.ignore(10000,'\n');
    if (name_of_file == "None"){
        cout << "You didnt write the name of file!\n";
        enter_to_cont();
        return;
    }
    if(filesystem::exists(name_of_file) == false){
           cout << "File wasnt founded on disk\nOperation aborted\n";
           cin.ignore(10000,'\n');
           enter_to_cont();
           return;
    }
        fstream file(name_of_file, fstream::app | fstream::out | fstream::in);
        while(true){
            string temp;
            getline(file,temp);
            if(file.eof())
                break;
            cout << temp << endl;
        }
        enter_to_cont();
        file.close();
}

int main(){
    string name_of_file = "None";
    while(true){
        system("clear");
        cout << "Current file: " << name_of_file << endl << endl;
        cout << "1) Write name of file\n2)Count letters\n3)Add line in file\n4)Show data in fie\n5)Exit\n>";
        int option = write_num();
        switch (option) {
        case 1: name_of_file = write_name(); break;
        case 2: count_letters(name_of_file); break;
        case 3: add_lines(name_of_file); break;
        case 4: show_data(name_of_file); break;
        case -229: break;
        default: return 0;
        }
    }

    return 0;
}
