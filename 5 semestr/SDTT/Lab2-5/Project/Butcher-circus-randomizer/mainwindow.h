#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::string fighters[18];
    QString hero[8];
    void Randoming(int numCommand);

private slots:
    void on_doRandom_clicked();

    void on_level1_valueChanged(int arg1);

    void on_level2_valueChanged(int arg1);

    void on_sameTeamLevel_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
