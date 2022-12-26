#include "crcfunctions.h"


QString ToBinary(QString symbols){
    QString result;

    for(unsigned int i = 0; i < symbols.size(); i++){
        char symbol = symbols[i].toLatin1();
        std::bitset<8>x(symbol);
        for(int j = x.size()-1; j != -1; j--)
            x[j] == false ? result+="0" : result+="1";
    }

    return result;
}


/*
 *  Since we only convert FCS from bits, we need to know only 1 byte
 */
char FromBinary(QString bits){
    int x = 1, res = 0;
    for(int i = bits.size()-1; i != -1; i--){
        res = res + (bits[i].unicode()-48) * x;
        x*=2;
    }
    char result = res;
    return result;
}


/* CRC8      modes:
 *
 * 1 - Calculates CRC for sending
 *
 * 2 - Calculates CRC for check (also requres FCS from packet)
 *
 * return FSC if mode 1, y\n
 */
char CRCcalc(QString bits, int mode, char FCS){
    // first of all we need to clear 0 bits at the start (if there any)
    for(int i = 0; bits[i] != '1';)
        bits.remove(0,1);
    if(mode == 1)
        bits+="00000000";
    else if (mode == 2){
        QString symbol; symbol += QChar::fromLatin1(FCS);
        bits += ToBinary(symbol);
    }
    QString polinomial = "100000111";  // since we use CRC8
    while(polinomial.size() <= bits.size()){
        for(int i = 0; i < polinomial.size(); i++)
            bits[i] == polinomial[i] ? bits[i] = '0' : bits[i] = '1'; // XOR
        for(int i = 0; bits[i] != '1';){
            bits.remove(0,1);
            if(bits.size() == 0)
                break;
        }
    }
    if(mode == 2 && bits.size() == 0)
        return 'y';
    else if (mode == 2)
        return 'n';

    return FromBinary(bits);
}


QString FromBitsQStringToASCIIQString(QString bits){
    QString SubBits, result;
    // From bits to ascii symbols
    for(int i = 0; i < bits.size(); i++){
        SubBits+=bits[i];
        if((i+1) % 8 == 0){
            result+= FromBinary(SubBits);
            SubBits.clear();
        }
    }
    return result;
}
