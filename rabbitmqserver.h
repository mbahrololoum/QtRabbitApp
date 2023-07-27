#ifndef RABBITMQSERVER_H
#define RABBITMQSERVER_H

#include <QString>
#include <QThread>

#include <amqp.h>

class RabbitMQServer : public QThread {
    Q_OBJECT

public:
    RabbitMQServer(const QString& exchange, const QString& bindingKey, const QString& queue, const QString& hostname="localhost", const int port=5672, const QString& user="guest", const QString& pass="guest");
    void run();
    int getLastErrorCode(){return errCode;}
    const QString getLastErrorMessage(){return QString(errBuf);}
    const QString getEnvelopeContent(amqp_envelope_t* envelope);
    void destroyEnvelope(amqp_envelope_t* envelope);

signals:
    void onError(int errCode, const QString& errMsg);
    void onChannelOpened();
    void onConnectionEstablished();
    void onQueueDeclared();
    void onQueueBinded();
    void onMessageReceived(amqp_envelope_t* envelope);
    void onChannelClosed();
    void onConnectionClosed();
    void onConnectionTerminated();

private:
    int errCode;
    char errBuf[1024];

    QString exchange, bindingKey, queue, hostname, user, pass;
    int port;
};

#endif // RABBITMQSERVER_H
