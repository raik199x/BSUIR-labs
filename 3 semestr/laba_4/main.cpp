#include <iostream>
#include <cstring>
//модификаторы насследования
//
using namespace std;

void check_num(int& num){
    while(1){
        cin >> num;
        if(!num){
            cout << "Wrong data!" << endl;
            cin.clear();
            cin.ignore(100000,'\n');
        } else
            break;
    }
}

string new_lines(int &length){
    string future_stroka; int mode = -1;
    bool leave;


    cout << "1) Numbers" << endl << "2) Names" << endl << "3) Simple note" << endl << "4) Go back" << endl << "Option: ";
    while(1){
        check_num(mode);
        if (mode <= 0 || mode > 4){
            cout << endl << "invalid option!" << endl << "option: ";
            continue;
        }
        break;
    }

    if (mode == 4)
        return "none";

    cin.ignore(1000000,'\n');
    if (mode == 3){
        cout << endl << "line: ";
        getline(cin,future_stroka);
        length = future_stroka.length();
    } else if (mode == 1){
        leave = false;
        while(1){
            cout << endl << "line: ";
            getline(cin,future_stroka);
            leave = true;
            for (unsigned long i = 0; i < future_stroka.length();i++)
                if ((future_stroka[i] >= 'a' && future_stroka[i] <= 'z') || future_stroka[i] == ' ' || (future_stroka[i] >= 'A' && future_stroka[i] <= 'Z')){
                    cout << "invalid data detected: '" << future_stroka[i] << "' decide not to write it!" << endl;
                    leave = false;
                }
            if (leave == true)
                break;
        }
        length = future_stroka.length();
    } else{
        leave = false;
        while(1){
            cout << endl << "line: ";
            getline(cin,future_stroka);
            leave = true;
            for (unsigned long i = 0; i < future_stroka.length();i++)
                if (future_stroka[i] == '-' && future_stroka[i+1] == '-'){
                    cout << "invalid combination detected: '" << future_stroka[i] << future_stroka[i+1] << "' decide not to write it!" << endl;
                    leave = false;
                } else if ((future_stroka[i] >= '0' && future_stroka[i] <= '9') || (future_stroka[i] != ' ' && future_stroka[i] < 'a' && future_stroka[i] > 'z') || (future_stroka[i] != ' ' && future_stroka[i] < 'A' && future_stroka[i] > 'Z')){
                    cout << "invalid data detected: '" << future_stroka[i] << "' decide not to write it!" << endl;
                    leave = false;
                }
            if (leave == true)
                break;
        }
        length = future_stroka.length();
    }
    return future_stroka;
}

void enter_cont(){
    cin.ignore(100000,'\n');
    cout << "Press ENTER to continue";
    string temp;
    getline(cin,temp);
}

class Stroka{
public:
    Stroka(){
        this->strok = new char[1];
        this->strok[0] = '*';
    }
    Stroka(Stroka &other){
        this->strok = other.strok;
    }
    ~Stroka(){
        delete []this->strok;
    }

    char& operator [](unsigned int i){
        return strok[i];
    }

    ostream& operator <<(ostream& os){
        os << this->strok;
        return os;
    }

    char* output_stroka(){
        return this->strok;
    }

    void* operator new(size_t amount){
        void * data;
        data = new char[amount];
        return data;
    }

    void *operator new[](size_t amount){
        void * data;
        data = new char[amount];
        return data;
    }

    void operator delete(void *p){
        :: operator delete(p);
    }

    void operator delete[](void *p){
        :: operator delete(p);
    }

    void operator =(char * line){
        int size = strlen(line)+1;
        this->strok = new char[size];
        for (int i = 0; i < size;i++)
            this->strok[i] = line[i];
    }

    void operator =(Stroka &other){
        int size = strlen(other.strok)+1;
        this->strok = new char[size];
        for (int i = 0 ; i < size ; i++)
            this->strok[i] = other.strok[i];
    }

private:
    char *strok;
};

void event_increase(Stroka **data, int amount){
    Stroka *temp = new Stroka[amount+2];
    for (int i = 0;i < amount;i++)
        temp[i] = (*data)[i];
    delete [](*data);
    (*data) = temp;
}

void event_delete(Stroka**data,int& amount, int deleter){
    amount--;
    Stroka*temp = new Stroka[amount];
    for (int i = 0, beta = 0; i < amount+1;i++,beta++){
        if (i == deleter){
            beta--;
            continue;
        }
        temp[beta] = (*data)[i];
    }
    delete[](*data);
    (*data) = temp;
    return;
}

void show_strings(Stroka *data,int data_amount){
    system("clear");
    if (data_amount == 0){
        cout << "it's easy to start, but to continue is an art" << endl << endl << "Lines not found" << endl;
        return;
    }
    for (int i = 0; i < data_amount;i++){
        cout << i+1 << "." << data[i].output_stroka() << endl << endl;
    }
    return;
}

void modify_menu(Stroka **data,int& data_amount){
    while(1){
        //if we dont have anything
        if (data_amount == 0){
            cout << endl << "empty data!" << endl;
            enter_cont();
            return;
        }
        //choosing str
        show_strings((*data),data_amount);
        cout << "choose string or -1 to go back" << endl;
        int num; check_num(num); num--;
        //checking num
        if (num == -2)
            return;
        if (num < 0 && num > data_amount){
            cout << "invalid number!" << endl;
            system("sleep 2");
            continue;
        }
        //user-friendly
        system("clear");
        for (int i = 0 ; i < data_amount;i++){
            if (num == i){
                cout << "> " << (*data)[i].output_stroka() << endl << endl;
            } else
                cout << i+1 << "." << (*data)[i].output_stroka() << endl << endl;
        }
        cout << "1) delete line" << endl << "2) modify line" << endl << "3) go back" << endl << "option: ";
        int option; check_num(option);

        if (option == 3)
            continue;
        else if (option == 2){
            int length = -1;
            string temp = new_lines(length);
            if (temp == "none")
                    return;
            char *our; our = new char[temp.length()+1];
            for (unsigned long i = 0; i < temp.length();i++)
                our[i] = temp[i];
            our[temp.length()] = '\0';
            (*data)[num] = our;
            delete []our;
        } else if (option == 1)
            event_delete(data,data_amount,num);
          else
            cout << "unexpected error";
    }
    return;
}

void add_strings(Stroka** data,int& data_amount){
    system("clear"); int length = -1; char* line;
    string future_stroka = new_lines(length);

    if(future_stroka == "none")
        return;

    line = new char[length+1];
    for (int i = 0; i < length;i++)
        line[i] = future_stroka[i];
    line[length] = '\0';

    event_increase(data,data_amount);
    (*data)[data_amount] = line;
    delete []line;
    data_amount++;
    return;
}

int main(){
    Stroka* data; int data_amount = 0;
    data = new Stroka[1];
    while(1){
        system("clear");
        cout << "1. Show strings" << endl << "2. Add strings" << endl << "3. Modify menu" << endl << "4. close program" << endl;
        int option;
        check_num(option);
        switch (option) {
            case 1: show_strings(data,data_amount); enter_cont(); break;
            case 2: add_strings(&data,data_amount); break;
            case 3: modify_menu(&data,data_amount);break;
            default: delete []data; return 0;
        }
    }
}
