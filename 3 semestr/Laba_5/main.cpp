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

class Mammal{
public:
    Mammal(){}
    Mammal(int age, string name_animal){
        this->is_alive = 1;
        this->name = name_animal;
        this->years_old = age;
    }
    ~Mammal(){}

    void output_name_age(){
        cout << "Name: " << this->name << endl << "Age: " << this->years_old << endl;
    }

protected:
    int years_old;
    bool is_alive;
    string name;
};

class Predator : public Mammal{
public:
    Predator(){}
    Predator(int age, string name_animal) : Mammal(age,name_animal){
    }
    ~Predator(){}

    void predator_info(){
        output_name_age();
    }
protected:
    int hunger;
};

class Herbivore : public Mammal{
public:
    Herbivore(){}
    Herbivore(int kids_born, int age, string animal_name) : Mammal(age,animal_name){
        this->kids_born = kids_born;
    }
    ~Herbivore(){}

    void herbivore_info(){
        output_name_age();
        cout << "Produce lives: " << this->kids_born << endl;
    }

protected:
    int kids_born;
};

class Bunny : public Herbivore{
public:
    Bunny(){}
    Bunny(int kids_born, int age, string animal_name) : Herbivore(kids_born,age,animal_name){
        this->speed = 0;
    }
    ~Bunny(){}

    void bunny_stat(){
        herbivore_info();
        cout << "bunny speed: " << this->speed << endl;
    }

protected:
    double speed;
};

class Wolf : public Predator{
public:
    Wolf(){}
    Wolf(int age, string name_animal) : Predator(age,name_animal){
        this->stamina = 0;
        this->anger = 0;
    }
    ~Wolf(){}

    void stat_wolf(){
        predator_info();
        cout << "wolf stamina: " << this->stamina << endl;
        cout << "wolf anger: " << this->anger << endl;
    }

protected:

    int stamina;
    int anger;
};

class Game : public Bunny,public Wolf{
public:
    Game(){}
    Game(int kids_born, string name_wolf, string name_bunny, int age_wolf,int age_bunny) : Bunny(kids_born,age_bunny,name_bunny), Wolf(age_wolf,name_wolf){
        this->anger = 10 / (age_wolf+1);
        this->stamina = 1000 / (age_wolf+1);
        this->speed = 15 / (age_bunny+1);
        if(this->speed == 0)
            this->speed++;
    }

    void increase_speed(){
        system("clear");
        int carrot = rand() % 3 + 1;
        cout << "chose whole to check" << endl << "1  2  3" << endl;
        int check = -1;
        check_num(check);
        if (check < 1 || check > 3){
            cout << "bunny choose the whole in his mind...." << endl << "speed increase failed!" << endl;
            enter_cont();
            return;
        } else if (check == carrot){
            cout << "Success!" << endl << "Speed increased (+0,2)" << endl;
            this->speed+=0.2;
            enter_cont();
        } else{
            cout << "failed!" << endl << "carrot was in the hole number - " << carrot;
            enter_cont();
            return;
        }
    }

    void info_output(){
        stat_wolf();
        cout << endl << endl;
        bunny_stat();
    }

    double Get_actions(){
        return this->speed;
    }

    void abuse_wolf(){
        cout << "Bunny decided to stress test wolf, but will be that a good idea?" << endl;
        cout << "Wolf stamina decreased" << endl;
        this->stamina-=15;
    }

    double end_day(){
        double actions = Get_actions();
        if (this->anger > speed){
            actions = -100;
        } else{
            this->stamina-=25;
            this->anger++;
        }
        return actions;
    }

    int meet_people(){
        system("clear");
        int carrot = rand() % 3 + 1;
        cout << "chose way to check" << endl << "1  2  3" << endl;
        int check = -1;
        check_num(check);
        if (check < 1 || check > 3){
            cout << "bunny choose way in his mind...." << endl << "Action denied" << endl;
            enter_cont();
            return 0;
        } else if (check == carrot){
            cout << "Success!" << endl << "Gained extra action" << endl;
            enter_cont();
            return 1;
        } else{
            cout << "failed!" << endl << "You lost your way and try to find the way back" << endl;
            enter_cont();
            return -1;
        }
    }

    int check_wolfe(){
        if (this->stamina <=0)
            return 100000;
        return 0;
    }

    ~Game(){}

private:
    int difficulti;
};


int main(){
   system("clear");
   srand(time(0));

   cout << "how we will name wolf?" << endl << "wolf name: ";
   string wolf_name;
   getline(cin,wolf_name);
   cout << "Enter his age (big age -> less stamina, big anger" << endl;
   cout << "Wolf Age: ";
   int age_wolf; check_num(age_wolf);

   cout << endl << endl;
   cin.ignore(1000,'\n');
   string bunny_name;
   cout << "name this bunny" << endl << "bunny name: ";
   getline(cin,bunny_name);
   cout << "Enter his age (big age -> less actions, less speed" << endl;
   cout << "bunny age: ";
   int age_bunny; check_num(age_bunny);
   int kids_born =1 + rand()%7; //int hunger = Difficulty;

   Game Start(kids_born, wolf_name, bunny_name, age_wolf, age_bunny);
   int actions = Start.Get_actions(); int option, luck;
   while(1){
       luck = 0;
       option = -1;
       system("clear");
       int win = Start.check_wolfe();
       if (win == 100000)
           actions = 100;
       if (actions == -100){
           option = -3377;
       } else if (actions == 100){
           option = -228;
       }
       else if (actions != 0){
       Start.info_output();
       cout << endl << endl << "Actions left: " << actions << endl << endl;
       cout << "1. Try to find carrot" << endl << "2. abuse wolf" << endl << "3. try to meet people" << endl << "4. End day..." << endl << "5. Let the wolf eat the bunny" << endl << ">";
       check_num(option);
       } else {
           option = 4;
           cout << "The little bunny is exhausted, the little bunny is..." << endl;
           cout << "Only fast legs will save him this night" << endl;
           enter_cont();
       }
       switch (option) {
       case 1: actions--; Start.increase_speed(); break;
       case 2: actions--; Start.abuse_wolf(); break;
       case 3: luck = Start.meet_people(); actions+=luck; break;
       case 4: actions = Start.end_day(); break;
       case -228: cout << "Bunny is safe now, congratulations!" << endl; return 0;
       case -3377: cout << "That was a good night... For the wolfes..." << endl << "Games lost" << endl; return 0;
       default: cout << "That was a good night... For the wolfes..." << endl; return 0;
       }
   }

   cout << "bye" << endl;
   return 0;
}
