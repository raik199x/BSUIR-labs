#ifndef SEC_FUNC_H
#define SEC_FUNC_H
#include <iostream>
#include <QApplication>
#include <QFile>

using namespace std;

string ParsingTrinket(string line, int mode);
QString* GetTrinkets(int lvl, string usedFighters[4]);

#endif // SEC_FUNC_H
