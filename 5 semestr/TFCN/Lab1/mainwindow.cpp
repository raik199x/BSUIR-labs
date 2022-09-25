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

#define TNT0 "/home/alexander/COMPORTS/tnt0"
#define TNT1 "/home/alexander/COMPORTS/tnt1"
#define TNT2 "/home/alexander/COMPORTS/tnt2"
#define TNT3 "/home/alexander/COMPORTS/tnt3"

void MainWindow::closeEvent(QCloseEvent *event){
    ::close(this->writer);
    ::close(this->reader);
    event->accept();
}

void MainWindow::Listening(){
    char accept_letter;
    while(true){
        int n = read(reader, &accept_letter, 1);
        if (this->reader == -1)
            break;
        else if(n == -1)
            continue;
        else if(accept_letter == '\0')
            continue;
        QString line; line.append(accept_letter);
        this->ui->outputBrowser->insertPlainText(line);

        line.clear();
        usleep(10000);
        this->ui->outputBrowser->verticalScrollBar()->setValue(this->ui->outputBrowser->verticalScrollBar()->maximum());
    }
    return;
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
    this->setWindowTitle("ComPortTalk");
    this->setFixedSize(QSize(404, 430));
    this->setStyleSheet("color: #00FFFF; background-color: #2F4F4F");
    ui->setupUi(this);

    this->portPair = FindFreePort();
    switch(portPair){
    case -1: QMessageBox::critical(this, "No Ports", "There is no available ports"); std::terminate(); break;
    case 0:  this->reader = open(TNT1, O_RDWR | O_NOCTTY | O_NDELAY); this->writer = open(TNT0, O_RDWR | O_NOCTTY | O_NDELAY);
             this->ui->statusWindow->setText("Transmitter tnt0, sent: 0\nReciever tnt1");
             this->setWindowTitle("ComPortTalk User One");
             break;
    case 1:  this->reader = open(TNT3, O_RDWR | O_NOCTTY | O_NDELAY);
             this->writer = open(TNT2, O_RDWR | O_NOCTTY | O_NDELAY);
             this->ui->statusWindow->setText("Transmitter tnt2, sent: 0\nReciever tnt3");
             this->setWindowTitle("ComPortTalk User Two");
             break;
    }


    fcntl(this->writer, F_SETFL, 0);

    this->Listener = new std::thread(&MainWindow::Listening, this);
    this->Listener->detach();
    this->ui->bits8->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CloseApp(){
    close();
}

void MainWindow::on_inputBrowser_textChanged(){
    if (this->inputLog.size() > this->ui->inputBrowser->toPlainText().size()){
        this->ui->inputBrowser->setText(this->inputLog);
        return;
    }
    if(this->ui->inputBrowser->toPlainText().size() - this->inputLog.size() > 40){
        QMessageBox::warning(this,"Message size", "could not paste buffer more that 40 symbols");
        this->ui->inputBrowser->setText(this->inputLog);
        return;
    }
    if (this->inputLog == this->ui->inputBrowser->toPlainText())
        return;
    // check letter encoding
    for (int i = this->inputLog.size(); i < this->ui->inputBrowser->toPlainText().size(); i++)
        if (this->ui->inputBrowser->toPlainText()[i] != '?' && this->ui->inputBrowser->toPlainText().toLatin1()[i] == '?'){
            QMessageBox::warning(this,"Letter encoding", "could not enter non latin symbols");
            this->ui->inputBrowser->setText(this->inputLog);
            return;
        }
    QString InputBlock = this->ui->inputBrowser->toPlainText();

    // QString -> char*
    for(int i = InputBlock.size()-this->inputLog.size(); i > 0; i--){
        char mess = InputBlock[InputBlock.size()-i].toLatin1();
        int n = write(writer, &mess, 1);
        if (n == -1){
            QMessageBox::critical(this,"Error", "No active users");
            return;
        }
        // changing num bytes
        this->bytesSend++;
        this->ui->statusWindow->setText("Transmitter tnt" + QString::number(this->portPair*2) + ", sent: " + QString::number(this->bytesSend) +"\nReciever tnt" + QString::number(this->portPair*2+1));
    }
    this->inputLog = this->ui->inputBrowser->toPlainText();
}


void MainWindow::on_inputBrowser_cursorPositionChanged(){
    this->ui->inputBrowser->moveCursor(QTextCursor::End);
}

void MainWindow::on_bits8_clicked()
{
     struct termios tty;
     tcgetattr(this->writer, &tty);
     tty.c_cflag &= ~CSIZE;
     tty.c_cflag |= CS8;
     tcsetattr(this->writer, TCSANOW ,&tty);
}


void MainWindow::on_bits7_clicked()
{
    struct termios tty;
    tcgetattr(this->writer, &tty);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS7;
    tcsetattr(this->writer, TCSANOW ,&tty);
}


void MainWindow::on_bits6_clicked()
{
    struct termios tty;
    tcgetattr(this->writer, &tty);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS6;
    tcsetattr(this->writer, TCSANOW ,&tty);
}


void MainWindow::on_bits5_clicked()
{
    struct termios tty;
    tcgetattr(this->writer, &tty);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS5;
    tcsetattr(this->writer, TCSANOW ,&tty);
}

