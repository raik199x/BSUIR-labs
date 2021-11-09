#include <iostream>
#include <random>
#include <string>
#include <iomanip>

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

class Cats{
public:
    Cats(){}
    Cats(string cat_name){
        this->cat_name = cat_name;
    }
    virtual ~Cats(){}

    void virtual show_cats() = 0;
    void virtual say_meow() = 0;
    void virtual set_name(string name) = 0;
    void virtual inc_days() = 0;
    void virtual dec_hunger() = 0;
    void virtual inc_hunger() = 0;
    void virtual dec_lazy() = 0;
    void operator=(string name){
        this->cat_name = name;
    }
    string cat_name_return(){
        return this->cat_name;
    }
protected:
    string cat_name;
    int lazyness_level;
    int hunger;
    int day_on_street;
};

class WildCats : public Cats{
public:
    WildCats(){
        this->day_on_street = 0;
    }
    WildCats(string cat_name){
        this->cat_name = cat_name;
        this->day_on_street = 0;
    }
    virtual ~WildCats(){}

    void say_meow() override{
        cout << "RRRAAAAAAAAAAAW" << endl;
    }
    void show_cats() override{
        cout << "days on street: " << this->day_on_street << endl;
    }
    virtual void operator=(WildCats &other){
        this->day_on_street = other.day_on_street;
    }
    void inc_days() override{
        this->day_on_street++;
    }
protected:
    //int day_on_street;
private:
    void set_name(string name) override{
        this->cat_name = name;
    }
    virtual void dec_hunger() override{}
    virtual void inc_hunger() override{}
    virtual void dec_lazy() override{}
};

class HomeCats : public Cats{
public:
    HomeCats(){}
    HomeCats(int lazy){
        this->lazyness_level = lazy;
    }
    HomeCats(string cat_name, int lazyness) : Cats(cat_name){
        this->lazyness_level = lazyness;
    }
    ~HomeCats(){}
    void say_meow() override{
        cout << "meowwww" << endl;
    }
    void show_cats() override{
        cout << this->cat_name << endl;
    }
    void operator=(HomeCats &other){
        this->lazyness_level = other.lazyness_level;
    }
    void set_name(string name) override{
        this->cat_name = name;
    }
    virtual void dec_hunger() override{}
    virtual void inc_hunger() override{}
    virtual void dec_lazy() override{}
protected:
    //int lazyness_level;
private:
    void inc_days() override{}
};

class MyCats : public HomeCats{
public:
    MyCats(){}
    virtual ~MyCats(){}
    MyCats(string cat_name,int lazy) : HomeCats(cat_name,lazy){
        this->hunger = 1;
    }

    void show_cats(){
        HomeCats::show_cats();
        cout << "hunger: " << this->hunger << endl << "Lazyness: " << this->lazyness_level << endl << endl;
    }
    void operator=(MyCats &other){
        this->hunger = other.hunger;
    }
    void virtual inc_hunger(){
        this->hunger++;
    }
    void virtual dec_hunger(){
        this->hunger--;
        if (this->hunger < 0)
            this->hunger = 0;
        this->lazyness_level++;
    }
    void virtual dec_lazy(){
        this->lazyness_level--;
        if(lazyness_level<0)
            this->lazyness_level = 0;
    }
    void set_name(string name) {
        HomeCats::set_name(name);
    }
    void operator=(string name){
        this->cat_name = name;
    }
private:
    //int hunger;
};

void increase_WildCats(Cats ***data, int amount){
    Cats **temp = new Cats * [amount+1];
    for(int i = 0;i < amount+1;i++)
        temp[i] = new WildCats[1];
    for(int i = 0; i < amount;i++)
        temp[i][0] = (*data)[i][0];
    delete [] (*data);
    (*data) = temp;
}

void increase_MyHomeCats(Cats ***data, int amount){
    Cats **temp = new Cats * [amount+1];
    for(int i = 0;i < amount+1;i++)
        temp[i] = new MyCats[1];
    for(int i = 0; i < amount;i++)
        temp[i][0] = (*data)[i][0];
    delete [] (*data);
    (*data) = temp;
}

void increase_HomeCats(Cats ***data, int amount){
    Cats **temp = new Cats * [amount+1];
    for(int i = 0;i < amount+1;i++)
        temp[i] = new HomeCats[1];
    for(int i = 0; i < amount;i++)
        temp[i][0] = (*data)[i][0];
    delete [] (*data);
    (*data) = temp;
}

void delete_WildCat(Cats ***data,int& amount, int deleter){
    Cats **temp = new Cats * [amount-1];
    for(int i = 0;i < amount;i++)
        temp[i] = new WildCats[1];
    for(int i = 0, beta = 0; i < amount;i++, beta++){
        if(i == deleter){
            beta--;
            continue;
        }
        temp[beta][0] = (*data)[i][0];
    }
    //delete [] (*data);
    amount--;
    (*data) = temp;
}

void delete_MyHomeCat(Cats ***data,int& amount, int deleter){
    Cats **temp = new Cats * [amount-1];
    for(int i = 0;i < amount;i++)
        temp[i] = new MyCats[1];
    for(int i = 0, beta = 0; i < amount;i++, beta++){
        if(i == deleter){
            beta--;
            continue;
        }
        temp[beta][0] = (*data)[i][0];
    }
    //delete [] (*data);
    amount--;
    (*data) = temp;
}

int go_outside(int cats,Cats **data){
    system("clear");
    if (cats == 0){
        cout << "You cant find anyone, so you decided to go back" << endl << endl;
        enter_cont();
        return -1;
    }
    cin.ignore(100000,'\n');
    for (int i = 0; i < cats;i++){
        cout << "Cat num '" << i+1 << "'" << endl;
        data[i][0].show_cats();
    }
    cout << "which one we try to catch?" << endl;
    int option; check_num(option);
    if (option > cats || option < 1){
        cout << "you found a beautifull butterfly and tryed to catch it..." << endl << "with cat..." << "but none of you succeed and you both went for your other affairs" << endl;
        enter_cont();
        return -1;
    }
    int remember_choose = option-1;
    int test = rand()%2 + 1;
    cout << "try to guess which way cat will run" << endl << "1) left" << endl << "2) right" << endl;
    check_num(option);
    if (test == option){
        cout << "succeed!!" << endl;
        enter_cont();
        return remember_choose;
    }
    cout << "you guessed wrong and cat runs away from you =(" << endl;
    enter_cont();
    return -1;
}

int main(){
    system("clear");

    cout << "You just owned a cat shelter, guess it's time to save some cats! =)" << endl << endl;
    int hope = -1; string names;
    Cats ***cat; int Home_cats = 0, Wild_cats = 0, My_home_cats = 0;
    cat = new Cats ** [3];    // 0 - Wild cats 1 - HomeCats 2 - MyHomeCats
    cat[0] = new Cats * [1];
    cat[1] = new Cats * [1];
    cat[2] = new Cats * [1];

    bool stop_game = false;
    while(stop_game == false){
        system("clear");
        srand(time(0));
        if (Wild_cats != 0){
            for (int i = 0; i < Wild_cats;i++)
                cat[0][i][0].inc_days();
        }
        int chanse = rand()%3+1;
        if (chanse == 3){
            increase_WildCats(&cat[0],Wild_cats);
            Wild_cats++;
        }
        if (My_home_cats == 0)
            cout << "At that moment you dont have any cats, try to go outside =)" << endl;
        else{
            cout << "your cats:" << endl;
            for(int i = 0; i < My_home_cats;i++){
                if (chanse == 2)
                    cat[2][i][0].inc_hunger();
                cat[2][i][0].show_cats();
            }
        }
        int option; cout << "1) Go outside" << endl << "2) Try find people" << endl << "3) play with cat" << endl << "4) feed cats" << endl << "5) remember happy cats " << endl << "6) end carieer" << endl;
        check_num(option);
        switch (option) {
            case 1: hope = go_outside(Wild_cats, cat[0]);
                    if(hope == -1) {break;}
                    system("clear"); cout << "name your new cat: "; cin >> names;
                    increase_MyHomeCats(&cat[2], My_home_cats);
                    delete_WildCat(&cat[0],Wild_cats,hope);
                    cat[2][My_home_cats][0] = names;
                    My_home_cats++;
                    break;
            case 2: hope = rand()%2+1; if (My_home_cats == 0){cout << "guess there is no point" << endl; enter_cont(); break;}
                    cout << "whick number try to call?" << endl << "1) first" << endl << "2) second" << endl;
                    check_num(option);
                    if (hope == option){
                        cout << "you get lucky today! =)" << endl;
                        cout << "choose cat that we will give!" << endl << ">";
                        int cat_bye = -1;
                        do {
                            check_num(cat_bye);
                        } while(cat_bye <= 0 || cat_bye > My_home_cats);
                        cat_bye--;
                        increase_HomeCats(&cat[1],Home_cats);
                        cat[1][Home_cats][0].set_name(cat[2][0][0].cat_name_return());
                        Home_cats++;
                        delete_MyHomeCat(&cat[2],My_home_cats,cat_bye);
                        enter_cont();
                    } else {
                        cout << "no one answers =(" << endl;
                        enter_cont();
                    }
                    break;
            case 3: if (My_home_cats == 0){cout << "there is no one to play!" << endl; enter_cont(); break;}
                    cat[2][0][0].say_meow();
                    for(int i = 0; i < My_home_cats;i++){
                        cat[2][i][0].dec_lazy();
                    }
                    enter_cont();
                    break;
            case 4: if (My_home_cats != 0){
                        for(int i = 0; i < My_home_cats;i++)
                            cat[2][i][0].dec_hunger();
                    }
                    break;
            case 5: for (int i = 0; i < Home_cats;i++){cat[1][i][0].show_cats();}
                    enter_cont();
                    break;
            default:  stop_game = true; break;
        }


    }
    delete [] cat[0];
    delete [] cat[1];
    delete [] cat[2];
    delete [] cat;

   return 0;
}
