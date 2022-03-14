#include <iostream>
#include<fstream>
#include <filesystem>

using namespace std;

void enter_cont(){
    cin.ignore(100000,'\n');
    cout << "Press ENTER to continue";
    string temp;
    getline(cin,temp);
}

class MyException{
public:
    MyException(int code){
        this->code = code;
    }
    ~MyException(){}
    void what_happed(){
        string temp;
        switch (code) {
            case 1: cout << "Cin failed, wrong type!" << endl; cout << "Enter to cont" << endl; getline(cin,temp); return;
            case 2: cout << "Student cant be younger than 17 years" << endl; break;
            case 3: cout << "We can teach people older than 60 yeas" << endl; break;
            case 11: cout << "Max score is 10.0" << endl; break;
            case 12: cout << "Score cant be loweer or equal 0" << endl; break;
            case 21: cout << "Invalid option" << endl; break;
            case 31: cout << "failed to open file" << endl; break;
            case 41: cout << "Forbidden symbol '~'" << endl; cout << "Enter to cont" << endl; getline(cin,temp); return;
            case 42: cout << "Forbidden numbers" << endl; cout << "Enter to cont" << endl; getline(cin,temp); return;
            case 43: cout << "Forbidden null line" << endl; cout << "Enter to cont" << endl; getline(cin,temp); return;
        }
        enter_cont();
    }
private:
    int code;
};

int write_num(){
    int num;
    while(true){
    cout << "enter num: ";
    try{
        cin >> num;
        cout << endl;
        if (cin.fail())
            throw(MyException(1));
    } catch(MyException &ex){
        cin.clear();
        cin.ignore(10000,'\n');
        ex.what_happed();
        continue;
    }
    return num;
    }
    return -200;
}

class Student{
friend ostream & operator<<(ostream&out,const Student list);
friend istream & operator >>(istream& in, Student list);
public:
~Student(){}
    Student(){
        this->name = "~";
        this->age = 0;
        this->average_score = 0;
    }

    Student(string name, int age, double score){
        this->name = name;
        this->age = age;
        this->average_score = score;
    }

    void set_info(string name, int age, double score){
        this->name = name;
        this->age = age;
        this->average_score = score;
    }

    Student(Student const &other){
        this->name = other.name;
        this->age = other.age;
        this->average_score = other.average_score;
    }

    void show_buffer(string path){
        system("clear");
        unsigned int i = path.length();
        fstream file;
        cout << "Name: " << this->name << endl << "Age: " << this->age << endl << "Score: " << this->average_score << endl;

        int option;
        cout << "1. put data somewhere" << endl << "2. take data" << endl << "3.go back" << endl << "> ";
        int data_amount = 0;
        try{
            cin >> option;
            if (cin.fail())
                throw(MyException(1));
            else if (option < 0 || option > 3)
                throw(MyException(21));
        } catch(MyException &ex){
            cin.clear();
            ex.what_happed();
            enter_cont();
            return;
        }
        cout << endl;
        if(option == 3)
            return;
        else if (this->name == "~" && option == 1){
            cout << "Buffer is curserd!" << endl;
            enter_cont();
            return;
        }
        else if (option == 2){

            if (path[i-5] == 't'){

                //output information and check how many items here
                file.open(path, fstream::in | fstream::out | fstream::app);
                string data;
                while (true) {
                    getline(file,data);
                    if (file.eof())
                            break;
                    cout << data_amount+1 << " >" << data << endl;
                    data_amount++;
                }
                //if ther is no iteam, just leave
                if (data_amount == 0){
                    cout << "No data!" << endl;
                    enter_cont();
                    return;
                }
                //else try to take data
                cout << "choose data send to buffer, ";
                option = write_num();
                    //if usere stupid
                    if (option < 0 || option > data_amount){
                        cout << "invalid argument" << endl;
                        enter_cont();
                        return;
                    }
                file.close(); file.open(path, fstream::in | fstream::out | fstream::app); //refresh file
                data.clear(); //clear line;
                //take needed data;
                while(option != 0){
                    getline(file,data);
                    option--;
                }
                //setting data
                string name; int years; double average_score; string taking;
                int separator = 0; int probeg = 0; //i need num for running through line and num for counting number of separators
                //parsing
                while(separator != 3){
                    if (data[probeg] == '~'){ //if i found separator i skip it
                        separator++;
                        probeg++;

                        if(separator == 2){  //preparing years
                            years = stoi(taking);
                            taking.clear();
                        } else if (separator == 3){ //prepearing score
                            average_score = stof(taking);
                            break;
                        }

                        continue;
                    }
                    if (separator == 0)
                        name.append(sizeof(char),data[probeg]);
                    else if (separator == 1 || separator == 2)
                        taking.append(sizeof(char),data[probeg]);
                    probeg++;
                } // end while
                this->name = name;
                this->age = years;
                this->average_score = average_score;
        } // end of taking data for txt
        else{
                file.open(path, fstream::binary | fstream::in | fstream::out | fstream::app);
                string name; int age; double average_score;
                //taking data
                while(true){
                    char letter;
                    file.read((char*)&letter,sizeof(letter));
                    if (file.eof())
                        break;
                    else if (letter == '~'){
                        file.read((char*)&age,sizeof(age));
                        file.read((char*)&average_score,sizeof(average_score));
                        data_amount++;
                        cout << data_amount << " >" << name << ":" << age << ":" << average_score << endl;
                        name.clear();
                    } else
                        name.append(sizeof(char),letter);
                } //end of taking data from file

                if (data_amount == 0){
                    cout << "No data!" << endl;
                    enter_cont();
                    return;
                }
                cout << "choose data send to buffer, ";
                option = write_num();
                    //if user stupid
                    if (option < 0 || option > data_amount){
                        cout << "invalid argument" << endl;
                        enter_cont();
                        return;
                    }
                file.close(); file.open(path, fstream::binary | fstream::in | fstream::out | fstream::app); //refresh file
                name.clear();
                while(true){
                    char letter;
                    file.read((char*)&letter,sizeof(letter));
                    if (file.eof())
                        break;
                    else if (letter == '~'){
                        file.read((char*)&age,sizeof(age));
                        file.read((char*)&average_score,sizeof(average_score));
                        option--;
                        if (option == 0){
                            this->name = name;
                            this->age = age;
                            this->average_score = average_score;
                            break;
                        }
                        name.clear();
                    } else
                        name.append(sizeof(char),letter);
                }  // end of seetting buffer


        } //end working with bin
    } //end of option 2
        else if (option == 1){
            if (path[i-5] == 't'){

                file.open(path, fstream::in | fstream::out | fstream::app);
                string data; int spaces = 1;
                cout << spaces << " >" << endl;
                spaces++;
                while (true) {
                    getline(file,data);
                    if (file.eof())
                            break;
                    cout << data << endl;
                    cout << spaces << " >" << endl;
                    spaces++; data_amount++;
                }
                spaces--;
                cout << "choose place, ";
                option = write_num();
                if (option < 1 || option > spaces){
                    cout << "No suck space!";
                    enter_cont();
                    return;
                }
                file.close(); file.open(path, fstream::in | fstream::out | fstream::app);
                option--; string *put = new string[data_amount];
                for(int i = 0; i < data_amount;i++){
                    getline(file,put[i]);
                }
                file.close(); file.open(path, fstream::in | fstream::out);
                for(int i = 0, beta = 0; i < data_amount+1;i++,beta++){
                    if(option == i){
                        beta--;
                        file << this->name << "~" << this->age << "~" << this->average_score << "~\n";
                        continue;
                    }
                    file << put[beta] << "\n";
                }
                delete [] put;
            } // end of insering txt
            else{
                file.open(path, fstream::binary | fstream::in | fstream::out | fstream::app);
                int spaces = 1;
                cout << spaces << ">" << endl;
                string name; int age; double average_score;
                while(true){
                    char letter;
                    file.read((char*)&letter,sizeof(letter));
                    if (file.eof())
                        break;
                    else if (letter == '~'){
                        file.read((char*)&age,sizeof(age));
                        file.read((char*)&average_score,sizeof(average_score));
                        data_amount++;
                        cout << name << ":" << age << ":" << average_score << endl;
                        spaces++;
                        cout << spaces << ">" << endl;
                        name.clear();
                    } else
                        name.append(sizeof(char),letter);
                } //end of taking data from file

                cout << "choose place to paste, ";
                option = write_num();
                if (option < 0 || option > spaces){
                    cout << "No such place" << endl;
                    enter_cont();
                    return;
                }
                option--;
                file.close(); file.open(path, fstream::binary | fstream::in | fstream::out | fstream::app);
                string *names = new string[data_amount];
                int *ages = new int [data_amount];
                double *scores = new double[data_amount];

                for(int i = 0; i < data_amount;i++){
                    char letter = '1';
                    while(letter != '~'){
                        file.read((char*)&letter,sizeof(letter));
                        names[i].append(sizeof(char),letter);
                    }
                    file.read((char*)&ages[i],sizeof(ages[i]));
                    file.read((char*)&scores[i],sizeof(scores[i]));
                }
                file.close(); file.open(path, fstream::binary | fstream::in | fstream::out);
                for(int i = 0, beta = 0; i < data_amount+1;i++,beta++){
                    if (i == option){
                        for(unsigned int z = 0; z < this->name.length(); z++)
                            file.write((char*)&this->name[z],sizeof(this->name[z]));
                        file.write("~",sizeof(char));
                        beta--;
                        file.write((char*)&this->age,sizeof(this->age));
                        file.write((char*)&this->average_score,sizeof(this->average_score));
                        continue;
                    }
                    for(unsigned int z = 0; z < names[beta].length(); z++)
                        file.write((char*)&names[beta][z],sizeof(names[beta][z]));
                    file.write((char*)&ages[beta],sizeof(ages[beta]));
                    file.write((char*)&scores[beta],sizeof(scores[beta]));
                }

                delete [] names; delete [] ages; delete [] scores;
            }
        } // end of option 1
        file.close();
} //end of function

void save(string path){
    unsigned int i = path.length();
    fstream file;
    if (path[i-5] == 't'){
        file.open(path, fstream::app | fstream::in);
        file << this->name << "~" << this->age << "~" << this->average_score << "~\n";
    }
     else{
        file.open(path, fstream::binary | fstream::app | fstream::in | fstream::out);
        for (unsigned long i = 0; i < this->name.length();i++)
           file.write((char*)&this->name[i],sizeof(this->name[i]));
        file.write("~",sizeof(char));
        file.write((char *)&age,sizeof(age));
        file.write((char*)&average_score,sizeof(average_score));
    }
    file.close();
}

void output(string path){
    system("clear");
    fstream file;
    unsigned int i = path.length();


    if (path[i-5] == 't'){
        file.open(path, fstream::app | fstream::out | fstream::in);
        try{
        if(!file.is_open())
            throw(MyException(31));
        }
        catch(MyException &ex){
            ex.what_happed();
            return;
        }
        string temp;
        while(true){
            getline(file,temp);
            if(file.eof())
                break;
            for(unsigned int i=0; i < temp.length();i++){
                if (temp[i] == '~'){
                    cout << "\n";
                    continue;
                }
                cout << temp[i];
            }
            cout << "\n";
        }
    } else {
        file.open(path,fstream::binary | fstream::out | fstream::app | fstream::in);
        bool stop = 0;
        while(!file.eof()){
            char letter = -1;
            while (true){
                file.read((char *)&letter,sizeof(letter));
                if (file.eof()){
                    stop = 1;
                    break;
                } else if (letter == '~')
                    break;
                cout << letter;
            }
            if (stop == 1)
                break;
            cout << "\n";
            int num;
            file.read((char*)&num,sizeof(num));
            cout << num << "\n";
            double num_2;
            file.read((char*)&num_2,sizeof(num_2));
            cout << num_2 << "\n\n";
        }
    }


    file.close();
}

void swap(Student &first, Student &second){
    Student lol = first;
    first = second;
    second = lol;
}

void sorting_system(string  path){
    system("clear");
    int option; fstream file;
    cout << "How to sort?" << endl <<"1. By name" << endl << "2. By age" << endl <<"3. By scoree" << endl << "4. Go back" << endl << ">";
    try {
        cin >> option;
        if (cin.fail())
            throw(MyException(1));
        if (option < 0 || option > 4)
            throw(MyException(21));
    }  catch (MyException &ex) {
        cin.clear();
        ex.what_happed();
        return;
    }

    if(option == 4)
        return;
    int data_amount = 0;
    unsigned int zxc = path.length(); Student *data;
    //see how many iteams we have
    if (path[zxc-5] == 't'){
        string temp; file.open(path, fstream::app | fstream::in);
        while(true){
           getline(file,temp);
           if(file.eof() || temp == "")
               break;
           data_amount++;
        }
    } else{
        int age; double score;
        file.open(path, fstream::binary | fstream::in);
        while(true){
            char letter = '1';
            file.read((char*)&letter,sizeof(char));
            if (file.eof())
                break;
            else if (letter == '~'){
                file.read((char*)&age,sizeof(age));
                file.read((char*)&score,sizeof(score));
                data_amount++;
            }
        }
    }
    file.close();
    if (data_amount == 0)
        return;
    //now, lets read that data
    data = new Student[data_amount];

    if (path[zxc-5] == 't'){
        file.open(path, fstream::app | fstream::in);
        string name,age,score, temp;
        for(int i = 0; i < data_amount; i++){
            name.clear(); age.clear(); score.clear();
            getline(file,temp);
            int what_to_do = 1;
            for(unsigned int j = 0; j < temp.length();j++){
                if (temp[j] == '~'){
                    what_to_do++;
                    continue;
                } else if (what_to_do == 1)
                    name.append(sizeof(char),temp[j]);
                else if (what_to_do == 2)
                    age.append(sizeof(char),temp[j]);
                else if (what_to_do == 3)
                    score.append(sizeof(char),temp[j]);
            }
            data[i].set_info(name,stoi(age),stof(score));
        }
    } else {
        file.open(path, fstream::app | fstream::in | fstream::binary);
        string name; int age; double score;
        for(int i = 0; i < data_amount; i++){
            name.clear();
            char letter;
            while(true){
                file.read((char*)&letter,sizeof(char));
                if(letter == '~')
                    break;
                name.append(sizeof(char),letter);
            }
            file.read((char*)&age,sizeof(age));
            file.read((char*)&score,sizeof(score));
            data[i].set_info(name,age,score);
        }
    }
    file.close();
    //moment of fucking sorting

        for(int i = 0; i < data_amount-1;i++)
            for(int j = i+1; j < data_amount;j++){
                if(option == 2 && data[i].age < data[j].age){
                    data->swap(data[i],data[j]);
                } else if (option == 3 && data[i].average_score < data[j].average_score){
                    data->swap(data[i],data[j]);
                } else if (option == 1 && data[i].name < data[j].name)
                    data->swap(data[i],data[j]);
            }
    //write data
    if(path[zxc-5] == 't'){
        file.open(path, fstream::out);
        for(int i = 0; i < data_amount;i++){
            file << data[i].name << "~" << data[i].age << "~" << data[i].average_score <<"~\n";
        }
    } else{
        file.open(path,fstream::out | fstream::binary);
        for(int i = 0; i < data_amount; i++){
            for(unsigned int j = 0; j < data[i].name.length();j++)
                file.write((char*)&data[i].name[j],sizeof(char));
            file.write("~",sizeof(char));
            file.write((char*)&data[i].age,sizeof(int));
            file.write((char*)&data[i].average_score,sizeof(double));
        }
    }

    file.close();
    delete [] data;
}

void output_reverse(string path){
    system("clear");
    unsigned int i = path.length();
    if (path[i-5] == 't'){
    fstream file(path, fstream::app | fstream::in | fstream::out); int number_of_data = 0;
    //implementation for txt
    while(true){
        string temp;
        getline(file,temp);
        if(file.eof())
            break;
        number_of_data++;
    }
    file.close();
    file.open(path, fstream::app | fstream::in | fstream::out);
    while(number_of_data != 0){
        string temp;
        file.seekg(0);
        for(int i = 0; i < number_of_data; i++)
           getline(file,temp);
        number_of_data--;
        for(unsigned int i=0; i < temp.length();i++){
            if (temp[i] == '~'){
                cout << "\n";
                continue;
            }
            cout << temp[i];
        }
        cout << "\n";
    }
    } else {
         fstream file(path, fstream::app | fstream::in | fstream::out | fstream::binary); int number_of_data = 0;
         bool stop = 0;
         while(!file.eof()){
             char letter = -1;
             while (true){
                 file.read((char *)&letter,sizeof(letter));
                 if (file.eof()){
                     stop = 1;
                     break;
                 } else if (letter == '~')
                     break;
             }
             if (stop == 1)
                 break;
             int num;
             file.read((char*)&num,sizeof(num));
             double num_2;
             file.read((char*)&num_2,sizeof(num_2));
             number_of_data++;
         }
         file.close();
         file.open(path, fstream::app | fstream::in | fstream::out | fstream::binary);
         stop = 0;
         while(number_of_data != 0){
             file.close();
             file.open(path, fstream::app | fstream::in | fstream::out | fstream::binary);
             string line;
             int num;
             double num_2;
             for(int i = 0; i < number_of_data;i++){
                 line.clear();
                 while(true){
                    char letter = -1;
                     file.read((char *)&letter,sizeof(letter));
                     if (file.eof()){
                         stop = 1;
                         break;
                     } else if (letter == '~')
                         break;
                     line.append(sizeof(char),letter);
                 }
                 line.append("\0");
                     if (stop == 1)
                         break;
                     file.read((char*)&num,sizeof(num));
                     file.read((char*)&num_2,sizeof(num_2));
                 }
             cout << line << "\n" << num << "\n" << num_2 << "\n\n";
             number_of_data--;
         }
    }
}

void duplicates(string path){
    unsigned int i = path.length();
    fstream file; int data_amount = 0;
    if (path[i-5] == 't'){
        file.open(path, fstream::app | fstream::in | fstream::out);
        string temp;
        while(true){
            getline(file,temp);
            if (file.eof())
                break;
            data_amount++;
        }
        if(data_amount == 0)
            return;
        file.close(); file.open(path, fstream::app | fstream::in | fstream::out);
        int real_num = 0;
        string *put = new string [data_amount];
        for(int z = 0; z < data_amount;z++){
            bool add = true;
            getline(file,temp);
            for(int i = 0; i < real_num;i++){
                if (temp == put[i]){
                    add = false;
                    break;
                }
            }
            if (add == true){
                put[real_num] = temp;
                real_num++;
            }
        }
        file.close(); file.open(path, fstream::out);
        for(int i = 0; i < real_num; i++)
            file << put[i] << "\n";

    } else{
        file.open(path, fstream::binary | fstream::app | fstream::in | fstream::out);
        string temp; int age; double score;
        while(true){
            char letter;
            file.read((char*)&letter,sizeof(letter));
            if (file.eof())
                break;
            if (letter == '~'){
                file.read((char*)&age,sizeof(age));
                file.read((char*)&score,sizeof(score));
                data_amount++;
            }
        }
        if(data_amount == 0)
            return;
        string *names = new string[data_amount]; int *ages = new int [data_amount]; double *scores = new double [data_amount];
        int real_num = 0;
        file.close();file.open(path, fstream::binary | fstream::app | fstream::in | fstream::out);
        for(int i = 0; i < data_amount; i++){
            bool add = true;
            temp.clear(); age = 0; score = 0;
            char letter;
            while(true){
                file.read((char*)&letter,sizeof(char));
                temp.append(sizeof(char),letter);
                if(letter == '~')
                    break;
            }
            file.read((char*)&age,sizeof(age));
            file.read((char*)&score,sizeof(score));
            for(int z = 0; z < real_num;z++){
                if (names[z] == temp && ages[z] == age && scores[z] == score){
                    add = false;
                    break;
                }
            }
            if(add == true){
                names[real_num] = temp;
                ages[real_num] = age;
                scores[real_num] = score;
                real_num++;
            }
        }
        file.close();file.open(path, fstream::out | fstream::binary);
        for(int i = 0; i < real_num;i++){
            for(unsigned int j = 0; j < names[i].length();j++)
                file.write((char*)&names[i][j],sizeof(char));
            file.write((char*)&ages[i],sizeof(int));
            file.write((char*)&scores[i],sizeof(double));
        }
        delete [] names; delete [] ages; delete [] scores;
    }
    file.close();
}

private:
    string name;
    int age;
    double average_score;
};

ostream & operator<<(ostream & out, const Student list){
    out << list.name << "\n" << list.age << "\n" << list.average_score << "\n\n";
    return out;
}

istream  & operator >> (istream & in, Student list){
    in >> list.name >> list.age >> list.average_score;
    return in;
}

Student make_student(){
    system("clear");
    string name;
    cin.ignore(10000,'\n');
    while(true){
        cout << "Name: ";
        try{
            getline(cin,name);
            for(unsigned int i = 0; i < name.length();i++){
                if (name[i] == '~')
                    throw(MyException(41));
                if (name[i] >= '0' && name[i] <= '9')
                    throw(MyException(42));
            }
            if (name == "" || name == "\n" || name == " ")
                throw(MyException(43));
        } catch(MyException &ex){
            ex.what_happed();
            name.clear();
            continue;
        }
        break;
    }
    int age;
    while(true){
    cout << endl << "Enter age: ";
    try{
        cin >> age;
        if (cin.fail())
            throw(MyException(1));
        if (age < 17)
            throw(MyException(2));
        if (age > 60)
            throw(MyException(3));
    } catch(MyException &ex){
        ex.what_happed();
        continue;
    }
    break;
    }
    double score;
    while(true){
    cout << endl << "Enter score: ";
    try{
        cin >> score;
        if (cin.fail())
            throw(MyException(1));
        if (score > 10)
            throw(MyException(11));
        if (score <= 0)
            throw(MyException(12));
    } catch(MyException &ex){
        ex.what_happed();
        continue;
    }
    break;
    }
    Student someone(name,age,score);
    return someone;
}

void recreate(string path){
    fstream file(path, fstream::out);
    file.close();
}

void dump(string path){
    unsigned int i = path.length();
    if (path[i - 1] == 't' && path[i-5] == 't')
        system("hexdump -C /home/alexander/1files_for_Laba_9/txt.txt");
    else if (path[i - 1] == 't' && path[i-5] != 't')
        system("hexdump -C /home/alexander/1files_for_Laba_9/bin.txt");
    else
        system("hexdump -C /home/alexander/1files_for_Laba_9/bin.bin");
    enter_cont();
}

void search(string path){
    system("clear");
    cout << "What to Find?" << endl << "1) name" << endl << "2) age" << endl << "3) score" << endl << "4) go back" << endl;
    string name; string age; string score; int option = write_num(); cout << endl;
    cin.ignore(100000,'\n');
    switch (option) {
    case 1: cout << "Name: ";getline(cin,name);  break;
    case 2: cout << "Age: ";getline(cin,age);   break;
    case 3: cout << "Score: ";getline(cin,score); break;
    default: cout << "invalid option, returning" << endl;  cout << "Enter to continue"; getline(cin,name); return;
    }

    fstream file;
    unsigned int i = path.length();
    if (path[i-5] == 't'){
        file.open(path, fstream::app | fstream::in | fstream::out);
        while(true){
            string line_from_file;
            getline(file,line_from_file);
            if (file.eof())
                break;
            bool to_print = 0;
            if (option == 1 && line_from_file.find(name) != 18446744073709551615)
                to_print = 1;
            else if (option == 2 && line_from_file.find(age) != 18446744073709551615)
                to_print = 1;
            else if (option == 3 && line_from_file.find(score) != 18446744073709551615)
                to_print = 1;

            if (to_print == 1){
                    for(unsigned int i=0; i < line_from_file.length();i++){
                        if (line_from_file[i] == '~'){
                            cout << "\n";
                            continue;
                        }
                        cout << line_from_file[i];
                    }
                    cout << "\n";
            }

        }
    } else{
        file.open(path, fstream::app | fstream::in | fstream::out | fstream::binary);
        string name_from_file; int num_from_file; double num_2_from_file;
        while(true){
            char letter = -1;
            bool extra_stop = 0;
            while (true){
                file.read((char *)&letter,sizeof(letter));
                if (letter == '~')
                    break;
                else if (file.eof()){
                    extra_stop = 1;
                    break;
                }
                name_from_file.append(sizeof(char),letter);
            }
            if (extra_stop == 1)
                break;
            file.read((char *)&num_from_file,sizeof(num_from_file));
            file.read((char*)&num_2_from_file,sizeof(num_2_from_file));
            bool to_print = 0;
            if (option == 1 && name_from_file.find(name) != 18446744073709551615)
                to_print = 1;
            else if (option == 2 && stoi(age) == num_from_file)
                to_print = 1;
            else if (option == 3 && stoi(score) == num_2_from_file)
                to_print = 1;

            if (to_print == 1){
                name_from_file.append("\0");
                cout << name_from_file << endl << num_from_file << endl << num_2_from_file << endl;
            }
            name_from_file.clear();
        }
    }

    cout << "Enter to continue";
    getline(cin,name);
    file.close();
}
void delete_info(string path, int to_delete = -1){
    system("clear");
    unsigned int i = path.length();
    fstream file; int data_amount = 0;

    if (path[i-5] == 't'){
        string line;
        file.open(path, fstream::app | fstream::in | fstream::out);
        while(true){
            getline(file,line);
            if(file.eof())
                break;
            data_amount++;
            cout << data_amount << ". " << line << endl;
        }
        if (data_amount == 0){
            cout << "No data!" << endl;
            enter_cont();
            return;
        }
        try{
            if (to_delete == -1){
                cout << "choose data to delete: ";
                cin >> to_delete;
            }
            if (cin.fail())
                throw(MyException(1));
            if (to_delete < 1 || to_delete > data_amount)
                throw(MyException(21));
        } catch(MyException &ex){
            ex.what_happed();
            return;
        }

        fstream temp("/home/alexander/1files_for_Laba_9/temp", fstream::in | fstream::out | fstream::app);
        file.close();
        file.open(path, fstream::app | fstream::in | fstream::out);
        for(int i = 0; i < data_amount;i++){
            getline(file,line);
            if (to_delete-1 == i)
                continue;
            temp << line << "\n";
        }
        file.close(); file.open(path, fstream::out); file.close(); file.open(path, fstream::out | fstream::in);
        temp.close(); temp.open("/home/alexander/1files_for_Laba_9/temp", fstream::in | fstream::out | fstream::app);
        while(true){
            getline(temp,line);
            if (temp.eof())
                break;
            file << line << "\n";
        } temp.close();
    } else {
        string line; int num; double num_2;
        file.open(path, fstream::binary | fstream::app | fstream::in | fstream::out);
        while(true){
            char letter = -1; bool exit = 0;
            while(letter != '~'){
                file.read((char*)&letter,sizeof(char));
                if (file.eof()){
                    exit = 1;
                    break;
                }
                line.append(sizeof(char),letter);
            }
            if (exit == 1)
                break;
            file.read((char *)&num,sizeof(num));
            file.read((char *)&num_2,sizeof(num_2));
            line.append("\0");
            cout << data_amount+1 << ". " << line << num << ":" << num_2 << endl;
            data_amount++;
            line.clear();
        }
        if (data_amount == 0){
            cout << "empty!" << endl;
            enter_cont();
            return;
        }
        try{
            if(to_delete == -1){
                cout << "choose data to delete: ";
                cin >> to_delete;
            }
            if (cin.fail())
                throw(MyException(1));
            if (to_delete < 1 || to_delete > data_amount)
                throw(MyException(21));
        } catch(MyException &ex){
            ex.what_happed();
            return;
        }
        fstream temp("/home/alexander/1files_for_Laba_9/temp", fstream::in | fstream::out | fstream::app | fstream::binary);
        file.close();
        file.open(path, fstream::app | fstream::in | fstream::out);

        for(int i = 0; i < data_amount;i++){
            char letter = -1; bool exit = 0;
            while(letter != '~'){
                file.read((char*)&letter,sizeof(char));
                if (file.eof()){
                    exit = 1;
                    break;
                }
                line.append(sizeof(char),letter);
            }
            if (exit == 1)
                break;
            file.read((char *)&num,sizeof(num));
            file.read((char *)&num_2,sizeof(num_2));

            if (to_delete == i+1){
                line.clear();
                continue;
            }

            for(unsigned int j = 0; j < line.length();j++)
                temp.write((char*)&line[j],sizeof(line[j]));
            temp.write((char*)&num,sizeof(num));
            temp.write((char *)&num_2,sizeof(num_2));
            line.clear();
        }

        file.close(); file.open(path, fstream::out | fstream::binary); file.close(); file.open(path, fstream::out | fstream::in | fstream::binary);
        temp.close(); temp.open("/home/alexander/1files_for_Laba_9/temp", fstream::in | fstream::out | fstream::app | fstream::binary);

        for(int i = 0; i < data_amount-1;i++){
            char letter = -1; bool exit = 0;
            while(letter != '~'){
                temp.read((char*)&letter,sizeof(char));
                if (file.eof()){
                    exit = 1;
                    break;
                }
                line.append(sizeof(char),letter);
            }
            if (exit == 1)
                break;
            temp.read((char *)&num,sizeof(num));
            temp.read((char *)&num_2,sizeof(num_2));


            for(unsigned int j = 0; j < line.length();j++)
                file.write((char*)&line[j],sizeof(line[j]));
            file.write((char*)&num,sizeof(num));
            file.write((char *)&num_2,sizeof(num_2));
            line.clear(); temp.close();
        }

    }
    file.close();
    remove("/home/alexander/1files_for_Laba_9/temp");
}

int main(){
    system("clear");
    int option; string path =" ";
    Student empty;
    Student charachter;
    if(!filesystem::exists("/home/alexander/1files_for_Laba_9"))
        filesystem::create_directory("/home/alexander/1files_for_Laba_9");
    while(1){
        system("clear");

        if (path == " "){
            cout << "choose file to work with" << endl << "1)txt.txt" << endl << "2)bin.txt" << endl << "3)bin.bin" << endl;
            while(1){
                option = write_num();
                if (option < 1 || option > 3){
                    cout << "Invalid number" << endl;
                    continue;
                } else if (option == 1)
                    path = "/home/alexander/1files_for_Laba_9/txt.txt";
                else if (option == 2)
                    path = "/home/alexander/1files_for_Laba_9/bin.txt";
                else
                    path = "/home/alexander/1files_for_Laba_9/bin.bin";
                break;
            }
            continue;
        }

        cout << "Current file: " << path << endl;
        cout << "1. Add student" << endl << "2. show students" << endl << "3. Recreate file" << endl << "4. choose another file" << endl << "5. reverse output" << endl << "6. Dump file" << endl << "7. Search" << endl << "8. Delete info" << endl << "9. Delete duplicates" << endl << "10. sort system" << endl << "11. show buffer" << endl;
        try{
            cin >> option;
            if (cin.fail())
                throw (MyException(1));
            if (option < 1)
                throw(MyException(21));
        } catch(MyException &ex){
            cin.clear();
            cin.ignore(10000,'\n');
            ex.what_happed();
            continue;
        }
        switch (option) {
        case 1: charachter = make_student(); charachter.save(path); break;
        case 2: empty.output(path); enter_cont(); break;
        case 3: recreate(path); break;
        case 4: path = " "; break;
        case 5: empty.output_reverse(path); enter_cont(); break;
        case 6: dump(path); break;
        case 7: search(path); break;
        case 8: delete_info(path); break;
        case 9: empty.duplicates(path); break;
        case 10:empty.sorting_system(path);
                break;
        case 11:empty.show_buffer(path);break;
        default: return 0;
        }
    }
}
//Сортировка по любому полю
