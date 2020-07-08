#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QtCore/QString>
#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QMainWindow>
#include<QMessageBox>



class Communication : public QMainWindow
{

    Q_OBJECT

public:
    Communication();
    bool checkConnection();

signals:
    void loginAccepted();
    void loginRejected();
    void updateLogin(QStringList id,QStringList name);
    void messagesList(QStringList id,QStringList message,QStringList date);
    void registerAccepted();
    void registerDenied();
    void usernameExists();

public slots:

    void connected(QByteArray name,QByteArray password);
    void disconnected();
    void startReg(QString log,QString pass);
    void bytesWritten(qint64 bytes);
    void readyRead();
    void sendMessage(QByteArray targetID,QByteArray message);
    void checkChanges();
    void takeMessages(QByteArray index);
    void Login();
    void Logout();
private:
    QTcpSocket *socket;
    QByteArray ready_data;
    QStringList idList;
    QStringList messageList;
    QStringList dateList;

};

#endif // COMMUNICATION_H
