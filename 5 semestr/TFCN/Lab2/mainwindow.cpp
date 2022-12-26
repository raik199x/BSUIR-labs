#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <fcntl.h>
#include <unistd.h>
#include <QMessageBox>
#include <QDebug>
#include <time.h>
#include <QScrollBar>
#include <QCloseEvent>
#include <sys/file.h>
#include <malloc.h>

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
        int n = read(reciever, &accept_letter, 1);
        if (this->reciever == -1) break;
        if(n == -1) continue;
        if (accept_letter != 'n') continue; // combination of starting packet
        QString line; char oldLetter = ESC;
        // getting packet
        read(reciever, &accept_letter, 1); // skip source
        read(reciever, &accept_letter, 1); // skip destination
        while(true){
            n = read(reciever, &accept_letter, 1);
            if (n == -1) break;
            if(oldLetter == ESC){
                oldLetter = accept_letter;
                continue;
            }
            if(oldLetter == '\0' && accept_letter == ESC){
                line.append('n');
                oldLetter = ESC;
                continue;
            }
            if(accept_letter == ESC) break;
            if (oldLetter == 'n'){
                oldLetter = accept_letter;
                continue;
            }
            line.append(oldLetter);
            oldLetter = accept_letter;
        }
        read(reciever, &accept_letter, 1); // get FCS
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
    case 1:  this->reciever = open(TNT3, O_RDWR | O_NOCTTY | O_NDELAY);
             this->transmitter = open(TNT2, O_RDWR | O_NOCTTY | O_NDELAY);
             this->setWindowTitle("ComPortTalk User Two");
             this->packet[2] = '2'; // source
             break;
    }


    fcntl(this->transmitter, F_SETFL, 0);

    this->Listener = new std::thread(&MainWindow::Listening, this);
    this->Listener->detach();
    this->ui->bits8->setChecked(true);
    // packet prep
    this->packet[0] = 'n'; // flag
    this->packet[1] = '0'; // destination
    this->packet[16] = '0'; // FCS
    CheckMessage(this->packet, 2); // clear data
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CloseApp(){
    close();
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
    if (CheckMessage(this->packet,3,mess) == true) return;
    // showing byte stuffing
    QString statusBarUpdate; statusBarUpdate.append("n");
    for(int i = 1; i < 17;i++){
        if (this->packet[i] == 'n'){
            statusBarUpdate.append("<font color=\"#9900FF\">\\0\\E</font>");
            continue;
        } else if (this->packet[i] == '\n'){
            statusBarUpdate.append("\\n");
            continue;
        }
        statusBarUpdate.append(this->packet[i]);
    }
    this->ui->statusWindow->append(statusBarUpdate);
    // sending packet
    char *prepPacket = (char*)malloc(2); int size = 1;
    prepPacket[0] = this->packet[0];
    // copying data seg
    for(int i = 1; i < 16; i++){
        // checking if it if flag
        if(this->packet[i] == 'n'){
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
    prepPacket[size-2] = ESC;
    prepPacket[size-1] = this->packet[16];             // FCS
    write(this->transmitter, prepPacket, size);       // sending packet
    CheckMessage(this->packet, 2);                   // clearing data packet
    free(prepPacket);
}


void MainWindow::on_inputBrowser_cursorPositionChanged(){
    this->ui->inputBrowser->moveCursor(QTextCursor::End);
}

void MainWindow::on_bits8_clicked()
{
     struct termios tty;
     tcgetattr(this->transmitter, &tty);
     tty.c_cflag &= ~CSIZE;
     tty.c_cflag |= CS8;
     tcsetattr(this->transmitter, TCSANOW ,&tty);
}


void MainWindow::on_bits7_clicked()
{
    struct termios tty;
    tcgetattr(this->transmitter, &tty);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS7;
    tcsetattr(this->transmitter, TCSANOW ,&tty);
}


void MainWindow::on_bits6_clicked()
{
    struct termios tty;
    tcgetattr(this->transmitter, &tty);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS6;
    tcsetattr(this->transmitter, TCSANOW ,&tty);
}


void MainWindow::on_bits5_clicked()
{
    struct termios tty;
    tcgetattr(this->transmitter, &tty);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS5;
    tcsetattr(this->transmitter, TCSANOW ,&tty);
}

