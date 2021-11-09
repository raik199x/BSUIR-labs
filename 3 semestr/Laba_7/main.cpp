#include <iostream>
#include <cstring>
#include <random>

using namespace std;

void check_num(int& num){
    while(1){
        cin >> num;
        if(cin.fail() || num < 0){
            cout << "Wrong data!" << endl;
            cin.clear();
            cin.ignore(100000,'\n');
        } else
            break;
    }
}

void enter_cont(){
    cin.ignore(100000,'\n');
    cout << "Press ENTER to continue";
    string temp;
    getline(cin,temp);
}

template <typename T>
void sort(T* smth, int length){
    for(int i = 0 ; i < length-1;i++)
        for(int j = i+1; j < length;j++)
            if(smth[i] > smth[j]){
                T* lol; lol = new T[1];
                lol[0] = smth[i];
                smth[i] = smth[j];
                smth[j] = lol[0];
                delete []lol;
            }
}

template <typename T1,typename T2>
T1 sum(T1 first, T2 second){
    T1 ret = first + second;
    return ret;
}

template <typename T1,typename T2>
T1 dec(T1 first, T2 second){
    T1 ret = first - second;
    return ret;
}

template <typename T1,typename T2>
T1 mult(T1 first, T2 second){
    T1 ret = first * second;
    return ret;
}

template <typename T1,typename T2>
T1 divide(T1 first, T2 second){
    T1 ret = first / second;
    return ret;
}

template <typename T>
void decrease_memory(T** massiv, int deleter, int &length){
    T *temp = new T[length-1];
    for(int i = 0, beta = 0; i < length;i++,beta++){
        if(i == deleter){
            beta--;
            continue;
        }
        temp[beta] = (*massiv)[i];
    }
    delete [] (*massiv);
    (*massiv) = temp;
    length--;
}

template <typename T>
int sqrt_num(T num){
    T ret = sqrt(num);
    return ret;
}

template <typename T>
int pow_num(T num){
    T ret = num*num;
    return ret;
}

template <typename T>
void increase_memory(T** massiv, int &length){
    T *temp = new T[length+1];

    for(int i = 0; i < length; i++)
        temp[i] = (*massiv)[i];

    delete [] (*massiv);
    (*massiv) = temp;
    length++;
}

void secret_hotline(){
    system("clear");
    system("cat /home/alexander/qt-projects/Laba_7/hotline_miami");
    system("play -q /home/alexander/qt-projects/Laba_7/hotline.mp3");
    enter_cont();

    return;
}

void check_stability_stroka(char **stroka,int length){
    for(int i = 0; i < length; i++){
        while((*stroka)[i] <= 0)
            (*stroka)[i]+=127;
        while((*stroka)[i] > 127)
            (*stroka)[i]-=127;
    }
}

int main(){
    char *stroka = new char[1]; int length = 0, current_line = -1;
    int *num_int = new int[2]; double *num_double = new double[2]; int j;
    srand(time(0));
    string add;
    num_int[0] = 0; num_int[1] = 0; num_double[0] = 0; num_double[1] = 0;
    while(1){
        system("clear");
        if(current_line == 1)
            cout <<">Line: ";
        else
            cout << "1.Line: ";
        if (strcmp(stroka,"Elliot Anderson") == 0 && (j = rand()%6+1) == 2)
            cout << "Mr.Robot";
        else{
        for (int i = 0; i < length; i++){
            if(stroka[i] == ' '){
                cout << "' '";
                continue;
            } else if (stroka[i] <=32){
                cout << "?";
                continue;
            }
            cout << stroka[i];
        }
        }
        cout << endl << endl;
        if (current_line == 2)
            cout << ">Int: " << num_int[0] << " " << num_int[1] << endl << endl;
        else
            cout << "2.Int: " << num_int[0] << " " << num_int[1] << endl << endl;
        if (current_line == 3)
            cout << ">Double: " << num_double[0] << " " << num_double[1] << endl << endl;
        else
            cout << "3.double: " << num_double[0] << " " << num_double[1] << endl << endl;

        if(current_line == -1){
                cout << "8) secrets menu" << endl <<"9) to stop programm" << endl << "choose line: ";
                check_num(current_line);
                if (current_line == 9){
                    delete [] stroka;
                    delete [] num_int;
                    delete [] num_double;
                    return 0;
                }
                if (current_line == 8){
                    cout << "Marvel" << endl << "Ghoul SS rang" << endl << "Wrong number(2)" << endl << "Mr.Robot" << endl;
                    enter_cont();
                    current_line = -1;
                    continue;
                }
                if(current_line < 1 || current_line > 3){
                    cout << endl << "invalid line" << endl;
                    current_line = -1;
                }
            continue;
        }

        int option;
        if(current_line == 1){
            cout << "1) sort line" << endl << "2) add symbols"  << endl << "3) del symbol" << endl << "4) delete line" << endl << "5) choose another line" << endl << "6) inc"  << endl << "7) dec" << endl << "8) multyply" << endl << "9) divide "<< endl << "10) sqrt" << endl << "11) pow" << endl << ">";
            check_num(option);
            switch (option) {
            case 1: sort(stroka,length); break;
            case 2: cout << "to add: ";
                    cin.ignore(10000,'\n');
                    getline(cin,add);
                    for (unsigned long i = 0; i < add.length();i++){
                        increase_memory(&stroka,length);
                        stroka[length-1] = add[i];
                    }
                    if (strcmp(stroka,"Jacket") == 0)
                        secret_hotline();
                    break;
            case 3: cout << "which one (num): ";
                    check_num(option);
                    if (option > length){
                        cout << "invalid symbol!" << endl;
                        enter_cont();
                        break;
                    }
                    decrease_memory(&stroka,option,length);
                break;
            case 4: if(strcmp(stroka,"Iron Man") == 0)
                        system("play -q /home/alexander/qt-projects/Laba_7/sad.mp3 &");
                    for(int i = length; i > 0; i--)
                        decrease_memory(&stroka,i,length);
                    cout << "fully deleted" << endl;
                    enter_cont();
                    break;
            case 5: current_line = -1; break;
            case 6: //incr
                    cout << "enter shift: ";
                    check_num(option);
                    for(int i = 0; i < length;i++)
                        stroka[i] = sum(stroka[i],option);
                    check_stability_stroka(&stroka,length);
                    break;
            case 7: //decr
                cout << "enter shift: ";
                check_num(option);
                for(int i = 0; i < length;i++)
                    stroka[i] = dec(stroka[i],option);
                check_stability_stroka(&stroka,length);
                    break;
            case 8: //multi
                cout << "enter shift: ";
                check_num(option);
                for(int i = 0; i < length;i++)
                    stroka[i] = mult(stroka[i],option);
                check_stability_stroka(&stroka,length);
                    break;
            case 9: //divide
                cout << "enter shift: ";
                check_num(option);
                for(int i = 0; i < length;i++)
                    stroka[i] = divide(stroka[i],option);
                check_stability_stroka(&stroka,length);
                    break;
            case 10: //sqrt
                for(int i = 0; i < length;i++)
                    stroka[i] = sqrt_num(stroka[i]);
                check_stability_stroka(&stroka,length);
                     break;
            case 11: //pow
                for(int i = 0; i < length;i++)
                    stroka[i] = pow_num(stroka[i]);
                check_stability_stroka(&stroka,length);
                    break;
            default: break;
            }
        } else if (current_line == 2 || current_line == 3){
            cout << "1) sort" << endl << "11) write left number" << endl << "12) write right number" << endl << "2) sum" << endl <<  "3) decrease" << endl << "4) multiply" << endl << "5) divide" << endl << "6) choose another line" << endl << ">";
            check_num(option);
            if (current_line == 2)
            switch (option) {
            case 1: sort(num_int,2); break;
            case 11:  cout << "num: "; cin >> num_int[0]; break;
            case 12:  cout << "num: "; cin >> num_int[1]; break;
            case 2:  cout << sum(num_int[0],num_int[1]) << endl; enter_cont(); break;
            case 3:  cout << dec(num_int[0],num_int[1]) << endl;
                     if (num_int[0] == 1000 && num_int[1] == 7){
                         system("play -q /home/alexander/qt-projects/Laba_7/die.mp3 &");
                         for(int i = 993; i>0;i-=7){
                             system("sleep 0.05");
                             cout << i << "-7" << endl;
                         }
                     }
                     enter_cont(); break;
            case 4: cout << mult(num_int[0],num_int[1]) << endl; enter_cont(); break;
            case 5: cout << divide(num_int[0],num_int[1]) << endl; enter_cont(); break;
            case 6: current_line = -1; break;
            default: break;
            }
            if(current_line == 3)
                switch (option) {
                case 1: sort(num_double,2); break;
                case 11:  cout << "num: "; cin >> num_double[0]; break;
                case 12:  cout << "num: "; cin >> num_double[1]; break;
                case 2:  cout << sum(num_double[0],num_double[1]) << endl; enter_cont(); break;
                case 3: cout << dec(num_double[0],num_double[1]) << endl; enter_cont(); break;
                case 4: cout << mult(num_double[0],num_double[1]) << endl; enter_cont(); break;
                case 5: cout << divide(num_double[0],num_double[1]) << endl; enter_cont(); break;
                case 6: current_line = -1; break;
                default: break;
                }
        }
    }
}
