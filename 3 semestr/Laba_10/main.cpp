#include <iostream>
#include "exceptions.h"

using namespace std;

void enter_cont(){
    cin.ignore(100000,'\n');
    cout << "Press ENTER to continue";
    string temp;
    getline(cin,temp);
}

int write_num(){
    int option;
    try{
        cin >> option;
        if (cin.fail())
            throw(Exceptions(1));
    }catch(Exceptions &ex){
        cin.clear();
        cin.ignore(10000,'\n');
        ex.what_happend();
        enter_cont();
        return -148148;
    }
    return option;
}

template <typename T1>
class TwiceList{
public:
    ~TwiceList(){
        delete head;
    }
private:
    class Node{
        friend class Iterator;
        friend class TwiceList<T1>;
    public:
        Node(T1 data){
            this->data = data;
        }
        Node(){}
        ~Node(){}
        void print_data(){
            if(!data)
                throw(Exceptions(11));
            cout << data << " ";
        }

        Node * right;
        Node * left;
        T1 data;
    };
public:
    class Iterator{
        friend class TwiceList<T1>;
    public:
        Iterator(){
            this->current_node = nullptr;
        }
        ~Iterator(){}

        Iterator(Node *node){
            this->current_node = node;
        }

        Iterator(const Iterator &wow){
            this->current_node = wow.current_node;
        }

        Iterator &operator=(Node &other){
            this->current_node = other;
            return *this;
        }

        Iterator &operator=(const Iterator &other){
            this->current_node = other.current_node;
            return *this;
        }

        bool operator==(const Iterator &other){
            return this->current_node == other.current_node;
        }

        bool operator!=(const Iterator &other){
            return !(this->current_node == other.current_node);
        }
        Iterator *operator--(){
            try{
            if(this->current_node == nullptr)
                throw(Exceptions(11));
            } catch(Exceptions &ex){
                ex.what_happend();
                enter_cont();
            }

            this->current_node->left;
            return *this;
        }

        Iterator operator++(){
            try{
            if(this->current_node == nullptr)
                throw(Exceptions(11));
            } catch (Exceptions &ex){
                ex.what_happend();
                enter_cont();
            }
            this->current_node = this->current_node->right;
            return *this;
        }
        Node *current_node;
    };

    TwiceList(){
        this->head = this->tail = new Node;
        this->head = nullptr;
        this->tail = nullptr;
    }

    void PushBack(T1 data){
       Node *some = new  Node(data);
       if(head == nullptr){
           this->head = some;
           this->tail = some;
           iter_head = Iterator(head);
           this->iter_tail = Iterator(tail);
       } else{
           this->tail->right = some;
           some->left = this->tail;
           this->tail = some;
           this->iter_tail = Iterator(tail);
       }
    }

    void PopBack(){
        try{
            if(head == nullptr)
                throw(Exceptions(11));
            else if (head == tail){
                delete head;
                head = nullptr;
                tail = nullptr;
                iter_head = nullptr;
                iter_tail = nullptr;
            } else {
                Node *future = tail->left;
                future->right = nullptr;
                delete tail;
                tail = future;
                this->iter_tail = Iterator(tail);
            }
        }catch(Exceptions &ex){
            ex.what_happend();
        }
    }

    Iterator chead() {return iter_head;}
    Iterator ctail() {return iter_tail;}

    int size(){
        int size = 1; Iterator probeg = Iterator(head);
        if(head == nullptr)
            return 0;
        else for (; probeg != tail; ++probeg){
            size++;
        }
        return size;
    }

    void find(T1 data_to_find){
        cout << endl;
        try{
        if (head == nullptr)
            throw(Exceptions(11));
        } catch(Exceptions &ex){
            ex.what_happend();
            return;
        }

        Iterator probeg = Iterator(head);
        cout << "Found iteams:" << endl;
        for (int i = 0 ; i < TwiceList::size();i++){
            if (probeg.current_node->data == data_to_find)
                cout << probeg.current_node->data << " on " << probeg.current_node << endl;
            ++probeg;
        }
        enter_cont();
    }

    void sort(){
        Iterator first = Iterator(head);
        Iterator run;
        for(int i = 0; i < TwiceList::size()-1; i++){
            run = first;
            ++run;
            for(int j = i+1; j < TwiceList::size();j++){
                if (first.current_node->data > run.current_node->data){
                    T1 new_data = first.current_node->data;
                    first.current_node->data = run.current_node->data;
                    run.current_node->data = new_data;
                }
                ++run;
            }
            ++first;
        }
    }

    int unique(){
        Iterator first = Iterator(head);
        Iterator run;
        int size = TwiceList::size();
        for(int i = 0; i < size-1; i++){
            run = first;
            ++run;
            for(int j = i+1; j < size;j++){
                if (first.current_node->data == run.current_node->data){
                    TwiceList::erase(j);
                    return -1;
                }
                ++run;
            }
            ++first;
        }
        return 0;
    }

    void add(int num){
        T1 data; int dline = TwiceList::size();
        if (num < 0 || num > dline){
            cout << "Invalid space" << endl;
            enter_cont();
            return;
        }
        try{
            system("clear");
            cout << "To insert: ";
            cin >> data;
            cout << endl;
            if(cin.fail())
                throw (Exceptions(1));
        } catch (Exceptions &ex){
            ex.what_happend();
            return;
        }
        Node *some = new  Node(data);
        if (head == nullptr){
            delete some;
            TwiceList::PushBack(data);
            return;
        }
        else if (num == 0){
            head->left = some;
            some->right = head;
            head = some;
            iter_head = some;
            some->left = nullptr;
            return;
        } else if (num == dline){
            TwiceList::PushBack(data);
            delete some;
            return;
        } else{
            Iterator run = Iterator(head);
            for(int i = 0 ; i < num;i++)
                ++run;
            run.current_node->left->right = some;
            some->left = run.current_node->left;
            some->right = run.current_node;
            run.current_node->left = some;
            return;
        }
    }

    void search_border(int gran1, int gran2, string type){
        cout << "Current container - " << type << endl;
        if (head == nullptr){
             cout << "Empty list" << endl;
            return;
        }
        Iterator iter = Iterator(head);
        for (int i = 0; i < TwiceList::size(); i++){
            if (iter.current_node->data >= gran1 && iter.current_node->data <= gran2)
                cout << "Found: " << iter.current_node->data << " - " << iter.current_node << endl;
            ++iter;

        }

    }

    void search_substr(string substr, string type){
        string temp;
        cout << "Current container - " << type << endl;
        if (head == nullptr){
             cout << "Empty list" << endl;
            return;
        }
        string current_list;
        Iterator iter = Iterator(head);
        for(int i = 0 ; i < TwiceList::size(); i++){
            if(sizeof(iter.current_node->data) == sizeof(char)){
                current_list.append(sizeof(char),iter.current_node->data);
                current_list+=" ";
            }
            else if(sizeof(iter.current_node->data) == sizeof(int)){
                current_list+=to_string(iter.current_node->data);
                current_list+=" ";
            } else if(sizeof(iter.current_node->data) == sizeof(double)){
                current_list+=to_string(iter.current_node->data);
                current_list+=" ";
            }
            ++iter;
        }
        if (current_list.find(substr) != 18446744073709551615)
            cout << current_list << " <-  was found here" << endl;
    }

    void erase(int num){
        int dline = TwiceList::size();
        try{
        if(head == nullptr)
            throw(Exceptions(11));
        if(num < 1 || num > dline)
            throw(Exceptions(12));
        } catch(Exceptions &ex){
            ex.what_happend();
            return;
        }
        if (num == dline || (dline == 1 && num == 1)){
            TwiceList::PopBack();
            return;
        } else if(num == 1){
            Node *stay = head->right;
            stay->left = nullptr;
            Node *to_delete = head;
            delete to_delete;
            iter_head = Iterator(stay);
            head = stay;
        } else if (num == dline){
            Node *stay = tail->left;
            stay->right = nullptr;
            Node *to_delete = tail;
            delete to_delete;
            iter_tail = Iterator(stay);
            tail = stay;
        } else{
            Node *to_delete = head;
            for(int i = 0; i<num-1;i++)
                to_delete = to_delete->right;
            to_delete->right->left = to_delete->left;
            to_delete->left->right = to_delete->right;
            delete to_delete;
        }
    }

private:
    Node *head;
    Node *tail;

    Iterator iter_head;
    Iterator iter_tail;
};

int main(){

    TwiceList <int> the_list;
    TwiceList <int>::Iterator list_iter;

    TwiceList <char> charovie;
    TwiceList <char>::Iterator charovie_iter;

    TwiceList <double> doublie;
    TwiceList <double>::Iterator doublie_iter;

    int option;
    bool unique = false, auto_sort = false, exit = false;

    while(true){
    system("clear");
    exit = false;
    cout << "Global menu:" << endl << "1)Container <int>" << endl << "2)Container <char>" << endl << "3)Container <double>" << endl << "4) Search with range" << endl << "5) Search substr" << endl << "6) enter line" << endl << "7) exit" << endl << ">";
    option = write_num();
    if (option < 1 || option >= 7){
        return 0;
    }

    if (option == 1)
    //for intovie
    while(true){
        if (exit == true)
            break;
        system("clear");
        if (auto_sort == true)
            the_list.sort();
        if(unique == true){
            while(the_list.unique() == -1){}
        }
        list_iter = the_list.chead();
        for (int i = 0; i < the_list.size();i++){
            cout << list_iter.current_node->data << " ";
            ++list_iter;
        }
        cout << endl << "1. Push back\n2. Pop back\n3. Search\n4. Delete iteam using num\n5. Add iteam using num\n6. Tasks\nn. Choose another container\n>";
        option = write_num();
        switch (option) {
        case 1: cout << "enter num: "; option = write_num(); the_list.PushBack(option); break;
        case 2: the_list.PopBack();break;
        case 3: cout << "What to find?\n>"; option = write_num(); the_list.find(option); break;
        case 4: {
            system("clear");
            list_iter = the_list.chead();
            int dlina = the_list.size();
            if (dlina == 0){
                cout << "Emptyness..." << endl;
                enter_cont();
                break;
            }
            for (int i = 0; i < dlina;i++){
                cout << i+1 << ">" << list_iter.current_node->data << endl;
                ++list_iter;
            }
            cout << endl << "Num to delete: ";
            cin >> option;
            the_list.erase(option);
            break;
        }
        case 5:{
            list_iter = the_list.chead();
            cout << "(0) ";
            for (int i = 0; i < the_list.size();i++){
                cout << list_iter.current_node->data << "(" << i+1 << ") ";
                ++list_iter;
            }
            cout << endl;
            cout << "enter num: "; option = write_num(); the_list.add(option); break;}
        case 6:{
            while(option != 3){
            system("clear");
            cout << "|---TASKS---|" << endl;
            cout << "|           |" << endl;
            if(auto_sort == false)
                cout << "|auto sort:-|" << endl;
            else
                cout << "|auto sort:+|" << endl;
            cout << "|           |" << endl;
            if (unique == false)
                cout << "|unique:-   |" << endl;
            else
                cout << "|unique:+   |" << endl;
            cout << "|           |" << endl;
            cout << "|-----------|" << endl << endl;
            cout << "1. Switch auto sort tumbler" << endl << "2. Switch unique tumbler" << endl << "3. Go back" << endl << ">";
            option = write_num();
            if (option < 1 || option > 3 || cin.fail()){
                cout << "Error occured!" << endl;
                        enter_cont();
            } else if (option == 1 && auto_sort == false)
                auto_sort = true;
            else if (option == 1)
                auto_sort = false;
            else if (option == 2 && unique == false){
                    unique = true;
                    cout << endl << "WARNING!" << endl << "If you continue, you should understand that all non-unique data will be DELETED" << endl << "Wish to continue?\n1. Yes\n2. No\n>";
                    option = write_num();
                    if (option < 1 || option > 2){
                        cout << "Invalid option, operation aborted" << endl;
                        unique = false;
                    } else if(option == 1)
                        unique = true;
                    else
                        unique = false;
            }
            else if (option == 2)
                unique = false;
            }
            break;
        }
        default: exit = true; break;
        }
    }
    else if (option == 2)
    //char
    while(true){
        if (exit == true)
            break;
        char temp;
        system("clear");
        if (auto_sort == true)
            charovie.sort();
        if(unique == true){
            while(charovie.unique() == -1){}
        }
        charovie_iter = charovie.chead();
        for (int i = 0; i < charovie.size();i++){
            cout << charovie_iter.current_node->data << " ";
            ++charovie_iter;
        }
        cout << endl << "1. Push back\n2. Pop back\n3. Search\n4. Delete iteam using num\n5. Add iteam using num\n6. Tasks\nn. Choose another container\n>";
        option = write_num();
        switch (option) {
            case 1: cout << "enter letter: "; cin >> temp; cin.ignore(10000,'\n'); charovie.PushBack(temp); break;
            case 2: charovie.PopBack();break;
        case 3: cout << "What to find?\n>"; cin >> temp; charovie.find(temp); break;
            case 4: {
                system("clear");
                charovie_iter = charovie.chead();
                int dlina = charovie.size();
                if (dlina == 0){
                    cout << "Emptyness..." << endl;
                    enter_cont();
                    break;
                }
                for (int i = 0; i < dlina;i++){
                    cout << i+1 << ">" << charovie_iter.current_node->data << endl;
                    ++charovie_iter;
                }
                cout << endl << "Num to delete: ";
                cin >> option;
                charovie.erase(option);
                break;
            }
            case 5:{
                charovie_iter = charovie.chead();
                cout << "(0) ";
                for (int i = 0; i < charovie.size();i++){
                    cout << charovie_iter.current_node->data << "(" << i+1 << ") ";
                    ++charovie_iter;
                }
                cout << endl;
                cout << "enter num: "; option = write_num(); charovie.add(option); break;}
            case 6:{
                while(option != 3){
                    system("clear");
                    cout << "|---TASKS---|" << endl;
                    cout << "|           |" << endl;
                    if(auto_sort == false)
                        cout << "|auto sort:-|" << endl;
                    else
                        cout << "|auto sort:+|" << endl;
                    cout << "|           |" << endl;
                    if (unique == false)
                        cout << "|unique:-   |" << endl;
                    else
                        cout << "|unique:+   |" << endl;
                    cout << "|           |" << endl;
                    cout << "|-----------|" << endl << endl;
                    cout << "1. Switch auto sort tumbler" << endl << "2. Switch unique tumbler" << endl << "3. Go back" << endl << ">";
                    option = write_num();
                    if (option < 1 || option > 3 || cin.fail()){
                        cout << "Error occured!" << endl;
                        enter_cont();
                    } else if (option == 1 && auto_sort == false)
                        auto_sort = true;
                    else if (option == 1)
                        auto_sort = false;
                    else if (option == 2 && unique == false){
                        unique = true;
                        cout << endl << "WARNING!" << endl << "If you continue, you should understand that all non-unique data will be DELETED" << endl << "Wish to continue?\n1. Yes\n2. No\n>";
                        option = write_num();
                        if (option < 1 || option > 2){
                            cout << "Invalid option, operation aborted" << endl;
                            unique = false;
                        } else if(option == 1)
                            unique = true;
                        else
                            unique = false;
                    }
                    else if (option == 2)
                        unique = false;
                }
                break;
            }
            default: exit = true; break;
        }
    }
    else if (option == 3)
    //double
    while(true){
        if (exit == true)
            break;
        double temp;
        system("clear");
        if (auto_sort == true)
            doublie.sort();
        if(unique == true){
            while(doublie.unique() == -1){}
        }
        doublie_iter = doublie.chead();
        for (int i = 0; i < doublie.size();i++){
            cout << doublie_iter.current_node->data << " ";
            ++doublie_iter;
        }
        cout << endl << "1. Push back\n2. Pop back\n3. Search\n4. Delete iteam using num\n5. Add iteam using num\n6. Tasks\nn. Choose another container\n>";
        option = write_num();
        switch (option) {
            case 1: cout << "enter num: "; cin >> temp; doublie.PushBack(temp); break;
            case 2: doublie.PopBack();break;
            case 3: cout << "What to find?\n>"; cin >> temp; doublie.find(temp); break;
            case 4: {
                system("clear");
                doublie_iter = doublie.chead();
                int dlina = doublie.size();
                if (dlina == 0){
                    cout << "Emptyness..." << endl;
                    enter_cont();
                    break;
                }
                for (int i = 0; i < dlina;i++){
                    cout << i+1 << ">" << doublie_iter.current_node->data << endl;
                    ++doublie_iter;
                }
                cout << endl << "Num to delete: ";
                cin >> option;
                doublie.erase(option);
                break;
            }
            case 5:{
                doublie_iter = doublie.chead();
                cout << "(0) ";
                for (int i = 0; i < doublie.size();i++){
                    cout << doublie_iter.current_node->data << "(" << i+1 << ") ";
                    ++doublie_iter;
                }
                cout << endl;
                cout << "enter num: "; option = write_num(); doublie.add(option); break;}
            case 6:{
                while(option != 3){
                    system("clear");
                    cout << "|---TASKS---|" << endl;
                    cout << "|           |" << endl;
                    if(auto_sort == false)
                        cout << "|auto sort:-|" << endl;
                    else
                        cout << "|auto sort:+|" << endl;
                    cout << "|           |" << endl;
                    if (unique == false)
                        cout << "|unique:-   |" << endl;
                    else
                        cout << "|unique:+   |" << endl;
                    cout << "|           |" << endl;
                    cout << "|-----------|" << endl << endl;
                    cout << "1. Switch auto sort tumbler" << endl << "2. Switch unique tumbler" << endl << "3. Go back" << endl << ">";
                    option = write_num();
                    if (option < 1 || option > 3 || cin.fail()){
                        cout << "Error occured!" << endl;
                        enter_cont();
                    } else if (option == 1 && auto_sort == false)
                        auto_sort = true;
                    else if (option == 1)
                        auto_sort = false;
                    else if (option == 2 && unique == false){
                        unique = true;
                        cout << endl << "WARNING!" << endl << "If you continue, you should understand that all non-unique data will be DELETED" << endl << "Wish to continue?\n1. Yes\n2. No\n>";
                        option = write_num();
                        if (option < 1 || option > 2){
                            cout << "Invalid option, operation aborted" << endl;
                            unique = false;
                        } else if(option == 1)
                            unique = true;
                        else
                            unique = false;
                    }
                    else if (option == 2)
                        unique = false;
                }
                break;
            }
        default: exit = true; break;
        }
    }
    else if (option == 4){
        int gran1 , gran2;
        cout << endl << "Enter first border: ";
        gran1 = write_num();
        cout << endl << "Enter second border: ";
        gran2 = write_num();
        cout << endl;
        the_list.search_border(gran1,gran2,"<int>");
        charovie.search_border(gran1,gran2,"<char>");
        doublie.search_border(gran1,gran2,"<double>");
        enter_cont();
    } else if (option == 5){
        string substr;
        cout << "Enter substr: ";
        cin >> substr;
        the_list.search_substr(substr,"<int>");
        charovie.search_substr(substr,"<char>");
        doublie.search_substr(substr,"<double>");
        enter_cont();
    } else if (option == 6){
        cin.ignore(1000,'\n');
        string to_add;
        cout << "enter data (seperate data with ' '): ";
        getline(cin,to_add); to_add+='\0';
        string substr;
        for (unsigned int i = 0 ; i < to_add.length(); i++){
            if (to_add[i] == ' ' || to_add[i] == '\0'){
                bool if_int = true, if_dobule = true; int z; double x;

                for(unsigned j = 0; j < substr.length();j++)
                    if (substr[j] != '.' && (substr[j] < '0' || substr[j] > '9')){
                        if_int = false;
                        if_dobule = false;
                    }

                if (if_dobule == true){
                if(substr.find('.') != 18446744073709551615){
                try{
                x = stof(substr);
                } catch(...){
                    if_dobule = false;
                }
                } else
                    if_dobule = false;
                }

                if(if_int == true){
                try{
                z = stoi(substr);
                } catch(...){
                    if_int = false;
                }
                }
                if (if_dobule == true)
                    doublie.PushBack(x);
                else if (if_int == true)
                    the_list.PushBack(z);
                else
                    for(unsigned int j = 0; j < substr.length(); j++)
                        charovie.PushBack(substr[j]);
                substr.clear();
                if(to_add[i] == '\0')
                    break;
                continue;
            }
            substr+= to_add[i];
        }
    }
    }
}
