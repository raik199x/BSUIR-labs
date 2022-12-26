#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "crcfunctions.h"

#include <fcntl.h>
#include <unistd.h>
#include <QMessageBox>
#include <QDebug>
#include <QScrollBar>
#include <QCloseEvent>
#include <sys/file.h>
#include <malloc.h>
#include <random>
#include <cmath>


#define TNT0 "/home/raik199x/COMPORTS/tnt0"
#define TNT1 "/home/raik199x/COMPORTS/tnt1"
#define TNT2 "/home/raik199x/COMPORTS/tnt2"
#define TNT3 "/home/raik199x/COMPORTS/tnt3"
#define ESC 27


void MainWindow::closeEvent(QCloseEvent *event){
    ::close(this->transmitter);
    ::close(this->reciever);
    event->accept();
}


void MainWindow::Listening(){
    char accept_letter;
    while(true){
        bool isThereByteStuffing = false;
        int n = read(reciever, &accept_letter, 1);
        if (this->reciever == -1 || accept_letter != 'n' || n == -1) continue;
        QString line; char oldLetter = ESC;
        // getting packet
        read(reciever, &accept_letter, 1); // skip source
        read(reciever, &accept_letter, 1); // skip destination
        while(true){
            n = read(reciever, &accept_letter, 1);
            if (n == -1) break;
            if(oldLetter == '\0' && accept_letter == ESC){ // byte stuff code
                line.append('n');
                oldLetter = ESC;
                isThereByteStuffing = true;
                continue;
            }
            if(accept_letter == ESC){   // end of packet
                if(oldLetter != ESC) line.append(oldLetter); // because of bytestuff, we can have old letter as ESC
                break;
            }
            if(oldLetter == ESC){
                oldLetter = accept_letter;
                continue;
            }
            line.append(oldLetter);
            oldLetter = accept_letter;
        }
        read(reciever, &accept_letter, 1); // get FCS
        char isJam[13];
        n = read(reciever,&isJam,13);
        if(n == 13) continue; // means collision

        // recovering
        if(isThereByteStuffing == false && CRCcalc(ToBinary(line),2,accept_letter) == 'n'){
            QString bits = ToBinary(line);
            for(int i = 0; i < bits.size();i++){
                bits[i] == '1' ? bits[i] = '0' : bits[i] = '1';
                // if we recovered mistake
                if(CRCcalc(bits,2,accept_letter) == 'y'){
                    line = FromBitsQStringToASCIIQString(bits);
                    break;
                }
                bits[i] == '1' ? bits[i] = '0' : bits[i] = '1';
            }
        }
        this->ui->outputBrowser->insertPlainText(line);
        line.clear();
        this->ui->outputBrowser->verticalScrollBar()->setValue(this->ui->outputBrowser->verticalScrollBar()->maximum());
    }
    return;
}
/*!
 *  Checks data packet
 *
 *  modes (int):
 *  1 - check if data packet full (true - full)
 *
 *  2 - clear packet (always true)
 *
 *  3 - put symbol (true - empty space left after placing symbol, false - no empty space left afret placing)
 *
 *  if invalid mode returns false
 */
bool CheckMessage(char *packet, int mode, char symbol = '\0'){
    if (mode == 1){
        for(int i = 3; i < 16; i++)
            if (packet[i] == '\0')
                return false;
        return true;
    } else if (mode == 2){
        for(int i = 3; i < 16; i++)
            packet[i] = '\0';
        return true;
    } else if (mode == 3){
        for(int i = 3; i < 16; i++)
            if (packet[i] == '\0'){
                packet[i] = symbol;
                return i == 15 ? false : true;
            }
    }
    return false;
}

int FindFreePort(){
    for(int i = 0; i < 2; i++){
        int testReciever;
        int testTransmitter;
        if( i == 0){
            testReciever = open(TNT1, O_RDWR | O_NOCTTY | O_NDELAY);
            testTransmitter = open(TNT2, O_RDWR | O_NOCTTY | O_NDELAY);
        } else{
            testReciever = open(TNT3, O_RDWR | O_NOCTTY | O_NDELAY);
            testTransmitter = open(TNT0, O_RDWR | O_NOCTTY | O_NDELAY);
        }

        if (testReciever == -1 || testTransmitter == -1){
            if (testReciever != -1) ::close(testReciever);
            if (testTransmitter != -1) ::close(testTransmitter);
            continue;
        }

        int check = write(testTransmitter, "\0", 1);
        if (check == -1){ ::close(testReciever); ::close(testTransmitter); continue;}
        usleep(10);
        char letter;
        check = read(testReciever, &letter,1);
        if (check != -1 && letter == '\0'){::close(testReciever); ::close(testTransmitter); return i; }
        else { ::close(testReciever); ::close(testTransmitter); continue; }
    }
    return -1;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QIcon image(":/images/me.png");
    this->setWindowIcon(image);
    this->setWindowTitle("ComPortTalk");
    this->setFixedSize(QSize(404, 430));
    this->setStyleSheet("color: #00FFFF; background-color: #2F4F4F");
    ui->setupUi(this);

    this->portPair = FindFreePort();
    switch(portPair){
    case -1: QMessageBox::critical(this, "No Ports", "There is no available ports"); std::terminate(); break;
    case 0:  this->reciever = open(TNT1, O_RDWR | O_NOCTTY | O_NDELAY); this->transmitter = open(TNT0, O_RDWR | O_NOCTTY | O_NDELAY);
             this->setWindowTitle("ComPortTalk User One");
             this->packet[2] = '0'; // source
             break;
    case 1:  this->reciever = open(TNT3, O_RDWR | O_NOCTTY | O_NDELAY); this->transmitter = open(TNT2, O_RDWR | O_NOCTTY | O_NDELAY);
             this->setWindowTitle("ComPortTalk User Two");
             this->packet[2] = '2'; // source
             break;
    }


    fcntl(this->transmitter, F_SETFL, 0);

    this->Listener = new std::thread(&MainWindow::Listening, this);
    this->Listener->detach();
    this->ui->bits8->setChecked(true);
    // packet prep
    this->packet[0] = 'n';     // flag
    this->packet[1] = '0';    // destination
    this->packet[16] = '\0'; // FCS
    CheckMessage(this->packet, 2); // clear data
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::CloseApp(){
    close();
}


void MainWindow::SendMessage(){
    // preparing packet
    bool isThereByteStuff = false; // for understanding do we need to calculate FCS
    char *prepPacket = (char*)malloc(2); int size = 1;
    prepPacket[0] = this->packet[0]; // setting flag
    // copying data seg
    for(int i = 1; i < 16; i++){
        // checking if it if flag
        if(this->packet[i] == 'n'){
            isThereByteStuff = true;
            prepPacket = (char*)realloc(prepPacket, size+2);
            size+=2;
            prepPacket[size-2] = '\0';
            prepPacket[size-1] = ESC;
            continue;
        }
        prepPacket = (char*)realloc(prepPacket, ++size);
        prepPacket[size-1] = this->packet[i];
    }
    prepPacket = (char*)realloc(prepPacket, (size+2));
    size+=2;
    prepPacket[size-2] = ESC; // flag of ending data seg
    // deciding FCS
    if (isThereByteStuff == false){
        // first convert to QString
        QString bytes;
        for(int i = 3; i < size-2; i++)
            bytes+=QChar::fromLatin1(prepPacket[i]);
        // getting FCS
        prepPacket[size-1] = CRCcalc(ToBinary(bytes),1);
    } else
        prepPacket[size-1] = this->packet[16];

    // deciding if there would be bad bit
        // Seed with a real random value, if available
    std::random_device r;
        // Choose a random mean between 0 and 1
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(0, 1);
    if(uniform_dist(e1) == 1 && isThereByteStuff == false){ // making bad bit
        QString dataSeg;
        // obtaining dataSeg
        for(int i = 3; i < 16; i++)
            dataSeg+=QChar::fromLatin1(prepPacket[i]);
        dataSeg = ToBinary(dataSeg);
        std::uniform_int_distribution<int> dist(0, dataSeg.size()-1); // test mode
        int numOfBadBit = dist(e1);
        dataSeg[numOfBadBit] == '1' ? dataSeg[numOfBadBit] = '0' : dataSeg[numOfBadBit] = '1';
        dataSeg = FromBitsQStringToASCIIQString(dataSeg);
        for(int i = 0; i < dataSeg.size(); i++)
            prepPacket[i+3] = dataSeg[i].toLatin1();
    }
    // sending packet + collision
    bool allowExit = false; int collision = 0;
    while(allowExit == false && collision < 16){
        char check;
        int n = read(this->reciever,&check,1);
        if(n != -1) continue;
        allowExit = true;
        write(this->transmitter, prepPacket, size);
        if(uniform_dist(e1) == 1){
            collision++;
            allowExit = false;
            write(this->transmitter, std::string(13,ESC).c_str(),13); // jam - signal
            unsigned int maxSleep;
            collision > 10 ?  maxSleep = pow(2,10) - 1 : maxSleep = pow(2,collision) - 1;
            std::uniform_int_distribution<int> dist(0,maxSleep);
            usleep(dist(e1));
        }
    }
    // showing packet in status window
    QString statusBarUpdate; statusBarUpdate.append("n");
    for(int i = 1; i < 16;i++){
        if (prepPacket[i] == ESC && isThereByteStuff == true){
            statusBarUpdate.append("<font color=\"#9900FF\">\\E</font>");
            continue;
        }
        if (prepPacket[i] == '\0' && isThereByteStuff == true){
            statusBarUpdate.append("<font color=\"#9900FF\">\\0</font>");
            continue;
        }
        if (prepPacket[i] == '\n'){
            statusBarUpdate.append("\\n");
            continue;
        }
        if (prepPacket[i ]== '<' || prepPacket[i] == '>'){
            statusBarUpdate.append("/B");
            continue;
        }
        statusBarUpdate.append(prepPacket[i]);
    }
    if(prepPacket[size-1] != '\0' && prepPacket[size-1] >= ' ' && prepPacket[size-1] <= '~'){ // try to ouput FCS symbol
        statusBarUpdate.append("<font color=\"#cdce17\">");
        statusBarUpdate.append(QChar::fromLatin1(prepPacket[size-1]));
        statusBarUpdate.append("</font>");
    } else if (prepPacket[size-1] != '\0'){ // if FCS symbol does not have ascii representation
        statusBarUpdate.append("<font color=\"#cdce17\">");
        QString bits; bits += prepPacket[size-1]; bits = ToBinary(bits);
        for(int i = 0; i < bits.size(); i++) statusBarUpdate.append(bits[i]);
        statusBarUpdate.append("</font>");
    }
    statusBarUpdate.append(":"); statusBarUpdate.append(QString(collision,'*')); // adding status of collision
    this->ui->statusWindow->append(statusBarUpdate);

    CheckMessage(this->packet, 2);                   // clearing data packet
    free(prepPacket);
}


void MainWindow::on_inputBrowser_textChanged(){
    // forbid deleting
    if (this->inputLog.size() > this->ui->inputBrowser->toPlainText().size()){
        this->ui->inputBrowser->setText(this->inputLog);
        return;
    }
    // check if text did not changed
    if (this->inputLog == this->ui->inputBrowser->toPlainText())
        return;
    // forbid pasting
    if(this->ui->inputBrowser->toPlainText().size()-1 != this->inputLog.size()){
        this->ui->inputBrowser->setText(this->inputLog);
        return;
    }
    // check letter encoding
    if (this->ui->inputBrowser->toPlainText()[this->ui->inputBrowser->toPlainText().size()-1] != '?' && this->ui->inputBrowser->toPlainText().toLatin1()[this->ui->inputBrowser->toPlainText().size()-1] == '?'){
            QMessageBox::warning(this,"Letter encoding", "could not enter non latin symbols");
            this->ui->inputBrowser->setText(this->inputLog);
            return;
    }
    QString InputBlock = this->ui->inputBrowser->toPlainText(); // getting text from transmitter block
    this->inputLog = this->ui->inputBrowser->toPlainText();    // remember new line
    // QString -> char
    char mess = InputBlock[InputBlock.size()-1].toLatin1();
    if (CheckMessage(this->packet,3,mess) == true) return;  // is packet ready yet?
    SendMessage();                                         // Block of message proccessing
}


void MainWindow::on_inputBrowser_cursorPositionChanged(){
    this->ui->inputBrowser->moveCursor(QTextCursor::End);
}

void MainWindow::on_bits8_clicked(){
     struct termios tty;
     tcgetattr(this->transmitter, &tty);
     tty.c_cflag &= ~CSIZE;
     tty.c_cflag |= CS8;
     tcsetattr(this->transmitter, TCSANOW ,&tty);
}


void MainWindow::on_bits7_clicked(){
    struct termios tty;
    tcgetattr(this->transmitter, &tty);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS7;
    tcsetattr(this->transmitter, TCSANOW ,&tty);
}


void MainWindow::on_bits6_clicked(){
    struct termios tty;
    tcgetattr(this->transmitter, &tty);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS6;
    tcsetattr(this->transmitter, TCSANOW ,&tty);
}


void MainWindow::on_bits5_clicked(){
    struct termios tty;
    tcgetattr(this->transmitter, &tty);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS5;
    tcsetattr(this->transmitter, TCSANOW ,&tty);
}

