#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>
#include<communication.h>
#include<QMessageBox>
#include<QCloseEvent>
namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr,Communication *w = nullptr);
    ~RegisterWindow();

signals:
    void startRegist(QString,QString);
    void setEnabledWindow();

private slots:
    void on_regButton_clicked();
    void registerAcc();
    void registerDen();
    void nameExists();
    void closeEvent(QCloseEvent *event);

private:
    Ui::RegisterWindow *ui;
    Communication *connection;
};

#endif // REGISTERWINDOW_H
