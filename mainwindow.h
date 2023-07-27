#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <amqp.h>
#include "rabbitmqclient.h"
#include "rabbitmqserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onStartListening();
    void onSendMessage();
    void onServerError(int errCode, const QString& errMsg);
    void onServerMessageReceived(amqp_envelope_t* envelope);
    void onServerStarted();
    void onServerTerminated();

private:
    Ui::MainWindow *ui;

    RabbitMQServer* server;
    RabbitMQClient* client;

    void addServerLog(const QString& msg);
};

#endif // MAINWINDOW_H
