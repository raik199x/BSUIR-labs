#include <iostream>

#include "exceptions.h"
#include "branch.h"
#include "leaves.h"
#include "plant.h"
#include "tree.h"

using namespace std;
using namespace Nature;

namespace checker_with_zero{
int enter_num(){
    int num;
    try{
        cin >> num;
        if(cin.fail())
            throw(Exceptions(1));
    } catch(Exceptions &ex){
        ex.what_happend();
        return -228;
    }
    return num;
}
}

namespace checker_without_zero{
int enter_num(){
    int num;
    try{
        cin >> num;
        if(cin.fail())
            throw(Exceptions(1));
        if(num == 0)
            throw(Exceptions(2));
    } catch(Exceptions &ex){
        ex.what_happend();
        return -228;
    }
    return num;
}
}

int main(){
    cout << "Being a god could be really incridiable:\nMaking oceans, mountaints, forests.... forests\nWhy dont we create a new forest?\n1)yes\nn)no\n>";
    int option = checker_without_zero::enter_num();
    if(option != 1){
        cout << "Being a god is very powerful, probably i should go sleep\n";
        return 0;
    }
    system("clear");
    cout << "Okay, first of all forests consist with trees\nWhat types of tress you want to be here? Use your imagination: ";
    string line[2]; cin >> line[0];
    cout << "\nNow, what height of tree you want? >";
    while(true){
        option = checker_with_zero::enter_num();
        if (option == -228)
            cout << "What are you doing?" << endl;
        else if (option < 0)
            cout << "Is your trees will grove IN eath?" << endl;
        else if (option <= 3)
            cout << "Comon, you a GOD, make it bigger" << endl;
        else
            break;
    }
    cout << "Last, but not least, which color will have crown of a tree? >";
    cin >> line[1];
    system("clear");
    Tree my_tree(line[0],option,line[1]);
    cout << "Beautiful! But tree look kinda lonely, maybe we should put it in leaves?\n";
    cout << "Write color of leaves > ";
    cin >> line[0];
    cout << endl << "And which draw will be on it? > ";
    cin >> line[1];
    Nature2::Leaves my_leave(line[0],line[1]);
    system("clear");
    cout << "This is the last one, branches, which size will they have?\n>";
    while(true){
        option = checker_without_zero::enter_num();
        if (option < 0)
            cout << "Comon, i know you tired, but lets finish this!" << endl;
        else
            break;
    }
    Branch my_branch(option);
    my_branch = my_leave;
    bool temp;
    cout << "Branches usually have fruits on it, will be your fruit eateble?\n1) yes\n2)no\n>";
    while(true){
        option = checker_without_zero::enter_num();
        if(option == 1)
            temp = true;
        else if (option == 2)
            temp = false;
        else{
            cout << "You know, i'm also tired.." << endl;
            continue;
        }
        break;
    }
    cout << "And finally, name of fruit: ";
    cin >> line[0];
    my_branch.create_fruite(temp,line[0]);
    system("clear");
    cout << "Thats is end, now you can go into your forest, see you later" << endl << "Enter to cont";
    cin.ignore(1000 ,'\n');
    getline(cin,line[0]);
    system("clear");

    while(true){
        system("clear");
        cout << "1) Look at trees\n2) Look at branches\n3) Push Branch\nn) Leave forest" << endl;
        option = checker_with_zero::enter_num();
        switch (option) {
        case 1: my_tree.Look(); break;
        case 2: my_branch.Look(); break;
        case 3: my_branch.try_push(); break;
        default: return 0;
        }
        cout << "Enter cont";
        cin.ignore(10000 ,'\n');
        getline(cin, line[0]);
    }
}
// +1 пространство имен
