#ifndef RABBITMQCLIENT_H
#define RABBITMQCLIENT_H

#include <QObject>
#include <QString>

class RabbitMQClient : public QObject {
    Q_OBJECT

public:
    RabbitMQClient(const QString& exchange, const QString& routingKey, const QString& hostname="localhost", const int port=5672, const QString& user="guest", const QString& pass="guest");
    int sendMessage(const QString& messageBody);
    int getLastErrorCode(){return errCode;}
    const QString getLastErrorMessage(){return QString(errBuf);}

private:
    int errCode;
    char errBuf[1024];

    QString exchange, routingKey, hostname, user, pass;
    int port;
};

#endif // RABBITMQCLIENT_H
