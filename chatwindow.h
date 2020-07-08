#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include<QString>
#include<communication.h>
#include<QTimer>
#include<QCloseEvent>

using namespace std;

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr,Communication *w = nullptr);
    ~ChatWindow();
    void setConnect(Communication *w);


signals:
    void sendRequest();

    void logoutUser();



private slots:
    void get_logedUsers_newMessages();

    void closeEvent(QCloseEvent *event);

    void on_sendButton_clicked();

    void on_logoutButton_clicked();

    void setIndex(int a);

    void updateList(QStringList id,QStringList name);

    void displayMessages(QStringList id,QStringList message,QStringList date);

    void setNickname(QString nick);

    void on_lineEdit_returnPressed();

    void cleanList(int a);

private:
    Ui::ChatWindow *ui;
    Communication *connection;
    int indeks = 0;
    QTimer *timer;
    QMap <QString,QString> id_users;            //kluczami są id
    QMap <QString,QString> nick_users;          //kluczami sa nicki
    QString nickname;
    QStringList messages;
    QStringList nameList ;


    QStringList nameTemp;
    QStringList messageTemp;
    QStringList dateTemp;

    QStringList usersTemp;


};

#endif // CHATWINDOW_H
