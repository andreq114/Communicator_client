#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <iostream>
#include <string.h>
#include <ctime>
#include <windows.h>
#include <winbase.h>
#include <QString>
#include <time.h>


ChatWindow::ChatWindow(QWidget *parent,Communication *w) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Communicator");
    ui->logedUsers->clear();
    timer = new QTimer(this);
    connection = w;
    connect(this,SIGNAL(logoutUser()),parent,SLOT(enableWindow()));
    connect(connection,SIGNAL(updateLogin(QStringList,QStringList)),this,SLOT(updateList(QStringList,QStringList)));
    connect(connection,SIGNAL(messagesList(QStringList,QStringList,QStringList)),this,SLOT(displayMessages(QStringList,QStringList,QStringList)));
    connect(timer,SIGNAL(timeout()),this,SLOT(get_logedUsers_newMessages()));
    timer->start(1000);
    if (ui->logedUsers->currentIndex() == -1){
        ui->sendButton->setEnabled(false);
        ui->lineEdit -> setEnabled(false);
    }

}

void ChatWindow::closeEvent (QCloseEvent *event)
{
    event->ignore();
    delete this;
}

void ChatWindow::get_logedUsers_newMessages(){
    connection->checkChanges();
    if(!ui->logedUsers->currentText().isNull()){
        QString tempnick = ui->logedUsers->currentText();
        QString tempid = nick_users[tempnick];
        QByteArray id = tempid.toUtf8();
        qDebug() << "ID OD KTOREGO WIADOMOSCI POBIERAMY";
        qDebug() << id;
        connection->takeMessages(id);
    }
}

void ChatWindow::displayMessages(QStringList id,QStringList message,QStringList date){
    qDebug() << "ODBIERAM LISTE";
    QString nick;
    nameTemp = id;
    messageTemp = message;
    dateTemp = date;

    for(int i=0;i< id.length();i++){
        if(!messages.contains(date.at(i))){
            if(id_users.contains(id.at(i))){
                 nick = id_users[id.at(i)];
            }
            else{
                nick = nickname;
            }
            QString wiadomosc = nick + "            " + date.at(i) + "\n" + message.at(i) + "\n\n";
            ui->messageText->append(wiadomosc);
            messages.append(date.at(i));
        }

    }

}

void ChatWindow::cleanList(int a){
    qDebug() << " CZYSZCZE MESADŻESZ" << a;
    messages.clear();
}


ChatWindow::~ChatWindow()
{
    emit logoutUser();
    delete ui;
    qDebug() << "DESTRUKTOR";
}

void ChatWindow::setIndex(int a){
    indeks = a;
    ui->messageText->clear();

}

void ChatWindow::setNickname(QString nick){
    nickname = nick;
    ui->helloText->setText("Hello\n"+ nickname);
    qDebug() << "SETUJE NIKNEJMA";
}


void ChatWindow::on_sendButton_clicked()
{
    QString messageTemp = ui->lineEdit->text();
    QByteArray message = messageTemp.toUtf8();
    QString indeksTemp = nick_users[ui->logedUsers->currentText()];
    ui->lineEdit->clear();
    QByteArray indeks = indeksTemp.toUtf8();
    connection->sendMessage(indeks,message);

}

void ChatWindow::updateList(QStringList id, QStringList name){

    if(ui->logedUsers->count() != 0){
        ui->lineEdit->setEnabled(true);
        ui->sendButton->setEnabled(true);
    }
    else{
        ui->lineEdit->setEnabled(false);
        ui->sendButton->setEnabled(false);
    }



    if(usersTemp.length() <= name.length()){
        for(int i=0;i<name.size();i++){
            qDebug() << "WYKONUJE SIE DRUGA CZESC";
            if(!usersTemp.contains(name.at(i))){
                id_users.insert(id.at(i),name.at(i));
                nick_users.insert(name.at(i),id.at(i));
                ui->logedUsers->addItem(name.at(i));
            }
        }
    }else{
        for(int i=0;i<usersTemp.size();i++){
            qDebug() << "USUWAM";
            if(!name.contains(usersTemp.at(i))){
                int idTemp = ui->logedUsers->findText(usersTemp.at(i));
                ui->logedUsers->removeItem(idTemp);
                QString temporary = nick_users[usersTemp.at(i)];
                nick_users.remove(usersTemp.at(i));
                id_users.remove(temporary);

            }
        }
    }
    usersTemp = name;



}





void ChatWindow::on_logoutButton_clicked()
{
    emit logoutUser();
    delete this;



}


void ChatWindow::on_lineEdit_returnPressed()
{
    ui->sendButton->click();
}
