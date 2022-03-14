#include <iostream>

using namespace std;

void enter_cont(){
    cin.ignore(100000,'\n');
    cout << "Press ENTER to continue";
    string temp;
    getline(cin,temp);
}

class MyException{
public:
    MyException(int code_ex){
        this->code_ex = code_ex;
    }
    void what_happend(){
        switch (this->code_ex) {
        case -1: cout << "cin Fail!\nProbably type Int get type Char\n"; break;
        case -2: cout << "Invalid Option!" << endl; break;
        case 0: cout << "0 exception" << endl; break;
        case -3: cout << "Invalid argumens of massiv!" << endl; break;
        case -99: cout << "Unexpected error, please contact with developer" << endl; break;
        default: cout << "Unexpected?" << endl; break;
        }
        what_code();
    }
    void what_code(){
        cout << "Exception code: " << code_ex << endl;
    }
private:
    int code_ex;
};

void my_unexpected(){
    throw MyException(-99);
}

void my_terminated(){
    cout << "The terminated function was implemented" << endl;
    exit(1);
}

class Massiv{
public:
    friend ostream &operator<<(ostream &out, Massiv &other);
    Massiv(){
        for(int i = 0; i < 10;i++){
            this->data1[i] = 0;
            this->data2[i] = 0;
        }
    }
    int operator[](const int i){
        return data1[i];
    }
    void set_num(const int index,const int num){
        this->data1[index] = num;
    }
    void clear_array(){
        for(int i = 0; i < 10;i++)
            this->data1[i] = 0;
    }
    Massiv operator +(const int num){
        for(int i = 0;i < 10;i++)
            this->data1[i]+=num;
        return (*this);
    }
    Massiv operator -(const int num){
        for(int i = 0;i < 10;i++)
            this->data1[i]-=num;
        return (*this);
    }
    Massiv operator *(const int num){
        for(int i = 0;i < 10;i++)
            this->data1[i]*=num;
        return (*this);
    }
    Massiv operator /(const int num){
        for(int i = 0;i < 10;i++)
            this->data1[i]/=num;
        return (*this);
    }

    void set_num_2(const int index,const int num){
        this->data2[index] = num;
    }

    void matr_plus_matr(){
        for(int i = 0; i < 10;i++)
            this->data1[i]+=this->data2[i];
    }

    void matr_minus_matr(){
        for(int i = 0; i < 10; i++)
            this->data1[i]-=this->data2[i];
    }

    void matr_mul_matr(){
        for(int i = 0; i < 10;i++)
            this->data1[i]*=this->data2[i];
    }

    void matr_dec_matr(){
        try{
            for (int i = 0; i < 10;i++){
                if (this->data2[i] == 0)
                    throw(MyException(0));
            }
        } catch (MyException &ex) {
                ex.what_happend();
                enter_cont();
                return;
          }
        for(int i =0; i < 10;i++)
            this->data1[i]/=this->data2[i];
    }

    void show_transp(){
        for(int i = 0; i < 10; i++)
            cout << data1[i] << " " << data2[i] << endl;
    }

    virtual ~Massiv(){}
protected:
    int data1[10];
    int data2[10];
};

ostream & operator <<(ostream & out, Massiv &other){
    for(int i = 0;i < 10;i++)
        out << other.data1[i] << " ";
    out << endl;
    for (int i = 0; i < 10;i++)
        out << other.data2[i] << " ";
    return out;
}

int main(){
    set_terminate(my_terminated);
    set_unexpected(my_unexpected);
    Massiv a;
    while(1){
        int num = -1;
        system("clear");
        cout << a << endl;
        int option;
        cout << "1) set num" << endl << "2)+" << endl << "3)-" << endl << "4)*" << endl << "5)/" << endl << "6)clear array" << endl << "7) exit" << endl << "8) throw code" << endl << "9) show transp" << endl << ">";

        try{
            cin >> option;
            if(cin.fail())
                throw MyException(-1);
            else if (option > 9 || option <= 0)
                throw MyException(-2);
        } catch(MyException &ex){
            ex.what_happend();
            cin.clear();
            enter_cont();
            continue;
        }
        int mode;
        if (option >= 1 && option <= 6){
            try{
                cout << "which one?" << endl << "1) first matrix" << endl << "2) second matrix" << endl;
                cin >> mode;
                if (cin.fail())
                    throw MyException(-1);
                if (mode < 1 || mode > 2)
                    throw MyException(-3);
            } catch(MyException &ex){
                ex.what_happend();
                cin.clear();
                enter_cont();
                break;
            }
        }
        switch (option) {
        case 1: try{
                    cout << "enter index: ";
                    cin >> option;
                    cout << endl << "enter num: ";
                    cin >> num;
                    cout << endl;
                    if (cin.fail())
                        throw MyException(-1);
                    if (option < 1 || option > 10)
                        throw MyException(-3);
                } catch(MyException &ex){
                    ex.what_happend();
                    cin.clear();
                    enter_cont();
                    break;
                }
                if (mode == 1)
                    a.set_num(option-1,num);
                else
                    a.set_num_2(option-1,num);
                break;
        case 2: if (mode == 1){
                try{
                    cout << "enter num: ";
                    cin >> option;
                    cout << endl;
                    if (cin.fail())
                        throw MyException(-1);
                } catch(MyException &ex){
                    ex.what_happend();
                    cin.clear();
                    enter_cont();
                    break;
                }
                a = a + option;
            } else {
                a.matr_plus_matr();
            }
                break;
        case 3: if (mode == 1){
            try{
            cout << "enter num: ";
            cin >> option;
            cout << endl;
                    if (cin.fail())
                        throw MyException(-1);
                } catch(MyException &ex){
                    ex.what_happend();
                    cin.clear();
                    enter_cont();
                    break;
                }
                a = a - option;
        }else {
         a.matr_minus_matr();
        }
                break;
        case 4: if (mode == 1){
            try{
            cout << "enter num: ";
            cin >> option;
            cout << endl;
                    if (cin.fail())
                        throw MyException(-1);
                } catch(MyException &ex){
                    ex.what_happend();
                    cin.clear();
                    enter_cont();
                    break;
                }
                a = a * option;
            } else {
                a.matr_mul_matr();
            }
                break;
        case 5: if(mode == 1){
            try{
            cout << "enter num: ";
            cin >> option;
            cout << endl;
                    if (cin.fail())
                            throw MyException(-1);
                    else if (option == 0)
                            throw MyException(0);

                } catch(MyException &ex){
                    ex.what_happend();
                    cin.clear();
                    enter_cont();
                    break;
                }

                a = a / option;
            } else{
             a.matr_dec_matr();
            }
                break;
        case 6: a.clear_array(); break;
        case 8: cin >> option;
                throw (option);
                break;
        case 9: a.show_transp();
                enter_cont();
                break;
        default: return 0;
        }
    }
}
