#include "sec_func.h"
#include "Random.h"


/*
 * mode == 1 -- trinket_name[leper] -> leper
 * mode == 2 -- trinket_name[leper] -> trinket_name
 */
string ParsingTrinket(string line, int mode){
    string result;
    if (line.find("[") > 100)
        return line;
    if(mode == 1)
        for(unsigned int i = line.find("[")+1; i < line.size()-1; i++) // obtaining hero name
            result += line[i];
    else if (mode == 2)
        for(unsigned int i = 0; i < line.find("["); i++) // obtaining hero name
            result += line[i];
    return result;
}


QString* GetTrinkets(int lvl, string usedFighters[4]){
    vector<string> possibleTrinkets;
    QFile trinketList(":/trinkets/trinkets/list.txt");
    if (!trinketList.open(QIODevice::ReadOnly)){
        qDebug() << "ERROR";
        std::terminate();
    }
    QTextStream in(&trinketList);
    string stopLine;
    // guessing stop line
    if (lvl >= 69)
        stopLine = "-----";
    else{
        stopLine = "--" + to_string(lvl+1);
        lvl + 1 < 10 ? stopLine+="--" : stopLine+="-";
    }
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
                if(ParsingTrinket(lineFromFile,1) == usedFighters[i]){
                    possibleTrinkets.push_back(lineFromFile);
                    break;
                }
        } else
            possibleTrinkets.push_back(lineFromFile);
    }
    QString *trinkets = new QString[8];
    // randoming
    while(true){
        bool escape = true;
        for(int i = 0; i < 8; i++)
            if(trinkets[i] == ""){
                escape = false;
                break;
            }
        if(escape == true) return trinkets;
        int num = possibleTrinkets.size()-1;
        num = Random::get(0,num);
        if(possibleTrinkets[num] == ParsingTrinket(possibleTrinkets[num],1)){
            for(int i = 0; i < 8; i++)
                if(trinkets[i] == "" && (((i+1) % 2 == 0 && possibleTrinkets[num] != trinkets[i-1].toStdString()) || (i+1) % 2 != 0)){
                    trinkets[i] = QString::fromStdString(possibleTrinkets[num]);
                    break;
                }
        } else{
            for(int i = 0; i < 4; i++)
                if(ParsingTrinket(possibleTrinkets[num],1) == usedFighters[i]){
                    if(trinkets[i*2] != "" && trinkets[i*2+1] != "")
                        break;
                    trinkets[i*2] == "" ? trinkets[i*2] = QString::fromStdString(ParsingTrinket(possibleTrinkets[num],2)) : trinkets[i*2+1] = QString::fromStdString(ParsingTrinket(possibleTrinkets[num],2));
                    break;
                }
        }
        possibleTrinkets.erase(possibleTrinkets.cbegin()+num);
    }
    return trinkets;
}
