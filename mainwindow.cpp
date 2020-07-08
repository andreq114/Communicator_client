#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QLineEdit"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Communicator");
    w = new Communication();
    connect(w,SIGNAL(loginAccepted()),this,SLOT(loginAcc()));
    connect(w,SIGNAL(loginRejected()),this,SLOT(loginRej()));

    ui->loginButton->setDefault(true);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::loginAcc(){
        qDebug() << "Dzialam";
        QString nick = ui->loginEdit->text();
        this->setEnabled(false);
        chatWindow = new ChatWindow(this,w);
        connect(this,SIGNAL(sendNickname(QString)),chatWindow,SLOT(setNickname(QString)));
        emit sendNickname(nick);
        chatWindow -> show();


}

void MainWindow::enableWindow(){
    this->setEnabled(true);
    ui->loginEdit->clear();
    ui->passwordEdit->clear();
    w->Logout();

}


void MainWindow::loginRej(){
    qDebug() << "Odrzucono";
    w->Logout();
    QMessageBox::information(this,"Error","Bledne dane");
}




void MainWindow::on_loginButton_clicked()
{
        QString nameTemp = ui->loginEdit->text();
        QByteArray name = nameTemp.toUtf8();
        QString passwordTemp = ui->passwordEdit->text();
        QByteArray password = passwordTemp.toUtf8();

        w->Login();
        if(w->checkConnection()){

            w->connected(name,password);
        }
        else{
            QMessageBox::information(this,"Error","Brak polaczenia z serwerem");
        }




}




void MainWindow::on_passwordEdit_returnPressed()
{
    ui->loginButton->click();
}

void MainWindow::on_registerButton_clicked()
{
    reg = new RegisterWindow(this,w);
    this->setEnabled(false);
    reg->show();
    reg->setEnabled(true);
}
