#include <QtTest>
#include <iostream>
#include <fstream>
#include "../Butcher-circus-randomizer/sec_func.h"

using namespace std;

class Tests : public QObject
{
    Q_OBJECT

public:
    Tests();
    ~Tests();

private slots:
    void Test1TinketNameParse();
    void Test2TrinketRandomization();

};

Tests::Tests(){

}

Tests::~Tests(){

}

void Tests::Test1TinketNameParse(){
    QVERIFY(ParsingTrinket("Leper[rage_mask]",1) == "rage_mask");
    QVERIFY(ParsingTrinket("Vestal[basher]",1) == "basher");
    QVERIFY(ParsingTrinket("Leper[rage_mask]",2) == "Leper");
    QVERIFY(ParsingTrinket("Vestal[basher]",2) == "Vestal");
    QVERIFY(ParsingTrinket("Vestal",1) == "Vestal");
    QVERIFY(ParsingTrinket("Vestal",2) == "Vestal");
}

void Tests::Test2TrinketRandomization(){
    string heroes[4] = { "arbalest", "grave_robber", "leper", "occultist" }; int lvl = 2;
    QFile trinketList(":/trinkets/trinkets/list.txt");
    if (!trinketList.open(QIODevice::ReadOnly)){
        QCOMPARE("FILE NOT FOUND","1");
    }
    string stopLine = "--3--";vector<string> possibleTrinkets;
    QTextStream in(&trinketList);
    // obtaining possible trinkets for team
    while(true){
        QString line = in.readLine();
        string lineFromFile = line.toStdString();
        if(lineFromFile == stopLine)
            break;
        if(lineFromFile.find("--") < 100)  // skipping level delimeter
            continue;
        if(lineFromFile.find("[") < 100){ // means there somewhere symbol '['
            for(int i = 0; i < 4; i++)
                if(ParsingTrinket(lineFromFile,1) == heroes[i]){
                    possibleTrinkets.push_back(lineFromFile);
                    break;
                }
        } else
            possibleTrinkets.push_back(lineFromFile);
    }
    trinketList.close();
    for(int i = 0; i < 1000; i++){
        QString *trinkets = GetTrinkets(lvl,heroes);
        for(int j = 0; j < 8; j++){
            bool isPossible = false;
            for(unsigned int c = 0; c < possibleTrinkets.size(); c++)
                if(ParsingTrinket(possibleTrinkets[c],2) == trinkets[j].toStdString()){
                    isPossible = true;
                    break;
                }
            QVERIFY(isPossible == true);
        }
        delete [] trinkets;
    }
}

QTEST_APPLESS_MAIN(Tests)

#include "tst_tests.moc"
