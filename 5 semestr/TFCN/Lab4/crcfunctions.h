#ifndef CRCFUNCTIONS_H
#define CRCFUNCTIONS_H
#include <QString>
#include <bitset>

QString ToBinary(QString symbols);
char FromBinary(QString bits);
char CRCcalc(QString bits, int mode, char FCS = '\0');
QString FromBitsQStringToASCIIQString(QString bits);

#endif // CRCFUNCTIONS_H
