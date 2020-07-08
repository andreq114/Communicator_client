#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <chatwindow.h>
#include <QString>
#include "communication.h"
#include <windows.h>
#include<QObject>
#include<QKeyEvent>
#include "registerwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Communication *w;
signals:

    void sendNickname(QString);



private slots:

    void on_loginButton_clicked();
    void loginAcc();
    void loginRej();
    void enableWindow();

    void on_passwordEdit_returnPressed();

    void on_registerButton_clicked();

private:
    Ui::MainWindow *ui;
    ChatWindow *chatWindow;
    RegisterWindow *reg;

};

#endif // MAINWINDOW_H
