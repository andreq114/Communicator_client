#include "communication.h"

Communication::Communication()
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()),Qt::ConnectionType::DirectConnection);
}

void Communication::Login(){
    socket->setSocketOption(QAbstractSocket::LowDelayOption,1);
    socket->connectToHost("localhost", 50000);

}

void Communication::Logout(){
    socket->disconnectFromHost();
}

bool Communication::checkConnection(){
    socket->waitForConnected(1000);
    return (socket->state() == QTcpSocket::ConnectedState);
}

void Communication::connected(QByteArray name,QByteArray password)
{
    qDebug() << "Connected!";
    qDebug() << name+password;
    QString temp =          QLatin1Char(1) +
            QLatin1String("C_LOGIN") +
            QLatin1Char(2) +
            name +
            QLatin1Char(3) +
            password +
            QLatin1Char(30) + "\r\n";

    QByteArray data = temp.toUtf8();
    socket->write(data);


}

void Communication::startReg(QString log,QString pass){
    qDebug() << "REJESTRACJA!";
    qDebug() << log+pass;
    if(checkConnection()){
        QString temp =          QLatin1Char(1) +
                QLatin1String("C_REG") +
                QLatin1Char(2) +
                log +
                QLatin1Char(3) +
                pass +
                QLatin1Char(30) + "\r\n";

        QByteArray data = temp.toUtf8();
        socket->write(data);
    }
    else{
        QMessageBox::information(this,"Error","Brak polaczenia z serwerem");
    }
}


void Communication::disconnected()
{
    qDebug() << "Disconnected!";
}

void Communication::bytesWritten(qint64 bytes)
{
    qDebug() << "We wrote: " << bytes;
}

void Communication::sendMessage(QByteArray targetID,QByteArray message){
    QString temp =          QLatin1Char(1) +
            QLatin1String("C_SEND") +
            QLatin1Char(2) +
            targetID +
            QLatin1Char(3) +
            message +
            QLatin1Char(30) + "\r\n";
    QByteArray data = temp.toUtf8();
    socket -> write(data);
    int id = targetID.toInt();
    qDebug() << id;
}

void Communication::checkChanges(){
    QString temp =          QLatin1Char(1) +
            QLatin1String("C_ULIST") +
            QLatin1Char(2) +
            QLatin1Char(30) + "\r\n";

    QByteArray data = temp.toUtf8();
    qDebug() << "CHECK CHANGES";
    socket -> write(data);
    socket -> waitForBytesWritten();
    socket -> flush();

}

void Communication::takeMessages(QByteArray index){
    QString temp =          QLatin1Char(1) +
            QLatin1String("C_GET_MSG") +
            QLatin1Char(2) +
            index +
            QLatin1Char(30) + "\r\n";
    QByteArray data = temp.toUtf8();

    qDebug() << "TAKE MESSAGES WYSLANE";
    socket ->write(data);
    socket -> waitForBytesWritten();
    socket -> flush();
}


void Communication::readyRead()
{
    QByteArray resived_data;
    resived_data = socket->readAll();

    qDebug() << "URUCHOMIONY readyRead ODEBRANE";
    qDebug() << resived_data;


    if(resived_data[0] == char(1) && resived_data[resived_data.length()-3]==char(30)){

        QString temp = resived_data.mid(1,resived_data.indexOf(char(2))-1);

        if(temp.compare("S_GET_MSG")==0){
            if(!resived_data.contains("ACT")){
                QString ID = resived_data.mid(resived_data.indexOf(char(2))+1,resived_data.indexOf(char(3))-resived_data.indexOf(char(2))-1);
                QString WIADOMOSC = resived_data.mid(resived_data.indexOf(char(3))+1,resived_data.indexOf(char(4))-resived_data.indexOf(char(3))-1);
                QString CZAS = resived_data.mid(resived_data.indexOf(char(4))+1,resived_data.indexOf(char(30))-resived_data.indexOf(char(4))-1);
                qDebug() << ID;
                qDebug() << WIADOMOSC;
                qDebug() << CZAS;
                idList.append(ID);
                messageList.append(WIADOMOSC);
                dateList.append(CZAS);
                qDebug() << "WYSYLAM LISTE";
                emit messagesList(idList,messageList,dateList);
            }
            else{
                qDebug() << "KASUJE LISTE";
                idList.clear();
                messageList.clear();
                dateList.clear();



            }

        }else if(temp.compare("S_ULIST")==0){
            QString TEXT = resived_data.mid(resived_data.indexOf(char(2))+1,resived_data.indexOf(char(30))-resived_data.indexOf(char(2))-2);
            QStringList nick; //lista użyutkowników
            QStringList tempID; //lista id użytkowników
            nick.clear();
            tempID.clear();
            qDebug() << "------------------------------------------------------------------";

            qDebug() << TEXT;
            if(TEXT.contains(3)){
                while (TEXT.indexOf(char(4))!=-1){
                    nick.append(TEXT.mid(0,TEXT.indexOf(char(3))));
                    TEXT.remove(0,TEXT.indexOf(char(3))+1);
                    tempID.append(TEXT.mid(0,TEXT.indexOf(char(4))));
                    TEXT.remove(0,TEXT.indexOf(char(4))+1);
                }
                nick.append(TEXT.mid(0,TEXT.indexOf(char(3))));
                TEXT.remove(0,TEXT.indexOf(char(3))+1);
                tempID.append(TEXT);

            }
            qDebug() << tempID;
            emit updateLogin(nick,tempID);

        }else if(temp.compare("S_LOGIN") ==0){
            QString TEXT = resived_data.mid(resived_data.indexOf(char(2))+1,resived_data.indexOf(char(30))-resived_data.indexOf(char(2))-1);
            if (TEXT.compare("ACT") ==0){
                qDebug() << "ZALOGOWANO";
                emit loginAccepted();
            }else{
                qDebug() << "BLAD: " + TEXT;
                emit loginRejected();
            }

        }else if(temp.compare("S_SEND_MSG") ==0){
            QString TEXT = resived_data.mid(resived_data.indexOf(char(2))+1,resived_data.indexOf(char(30))-resived_data.indexOf(char(2))-1);
            if (TEXT.compare("ACT") ==0){
                qDebug() << "Wysłano";
            }else{
                qDebug() << "Blad przy wysylaniu ";
            }
        }else if(temp.compare("S_REG") ==0){
            QString TEXT = resived_data.mid(resived_data.indexOf(char(2))+1,resived_data.indexOf(char(30))-resived_data.indexOf(char(2))-1);
            qDebug() << "KOD:";
            qDebug() << TEXT;
            if (TEXT.compare("ACT") ==0){
                qDebug() << "ZAREJESTROWANO";
                emit registerAccepted();
            }else if(TEXT.compare(" NEQ")){
                qDebug() << "istnieje taki nick";
                emit usernameExists();
            }else{
                qDebug() << "BLAD: " + TEXT;
                emit registerDenied();
            }

        }
    }
}
