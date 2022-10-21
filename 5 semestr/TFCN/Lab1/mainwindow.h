#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <thread>
#include "termios.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_inputBrowser_textChanged();

    void on_inputBrowser_cursorPositionChanged();

    void on_bits8_clicked();

    void on_bits7_clicked();

    void on_bits6_clicked();

    void on_bits5_clicked();

private:
    void closeEvent(QCloseEvent *event);
    std::thread *Listener;
    void Listening();
    void CloseApp();

    unsigned long long bytesSend = 0;
    int reader;
    int writer;
    QString inputLog;
    int portPair;
    Ui::MainWindow *ui;
signals:
};
#endif // MAINWINDOW_H