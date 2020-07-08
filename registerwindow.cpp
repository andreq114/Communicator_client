#include "registerwindow.h"
#include "ui_registerwindow.h"

RegisterWindow::RegisterWindow(QWidget *parent,Communication *w) :
    QMainWindow(parent),
    ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);
    connect(this,SIGNAL(startRegist(QString,QString)),w,SLOT(startReg(QString,QString)));
    connect(w,SIGNAL(registerAccepted()),this,SLOT(registerAcc()));
    connect(w,SIGNAL(registerDenied()),this,SLOT(registerDen()));
    connect(this,SIGNAL(setEnabledWindow()),parent,SLOT(enableWindow()));
    connect(w,SIGNAL(usernameExists()),this,SLOT(nameExists()));
    connection = w;
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
    connection->Logout();
    emit setEnabledWindow();
    qDebug() << "NISZCZE REJESTRACJE";
}

void RegisterWindow::on_regButton_clicked()
{

    QString login = ui->loginReg->text();
    QString password = ui->passwordReg->text();
    connection->Login();
    emit startRegist(login,password);


}

void RegisterWindow::registerAcc(){
    QMessageBox::information(this,"Rejestracja","Rejestracja powiodła się");
    delete this;
    emit setEnabledWindow();
}

void RegisterWindow::registerDen(){
    QMessageBox::information(this,"Rejestracja","Rejestracja nie powiodła się");
}

void RegisterWindow::nameExists(){
    QMessageBox::information(this,"Rejestracja","Taki uzytkownik juz istnieje.");
}

void RegisterWindow::closeEvent (QCloseEvent *event)
{
        event->ignore();
        delete this;

}
