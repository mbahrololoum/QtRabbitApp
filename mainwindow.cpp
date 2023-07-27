#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>

#include "rabbitmqserver.h"
#include "rabbitmqclient.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->server = NULL;
    this->client = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
    if(server)
    {
        delete server;
    }
    if(client)
    {
        delete client;
    }
}

void MainWindow::onStartListening()
{
    qDebug() << "Start listening...";

    server = new RabbitMQServer(
                          ui->leServerExchange->text(),
                          ui->leServerBindingKey->text(),
                          ui->leServerQueue->text(),
                          ui->leServerHost->text(),
                          ui->leServerPort->text().toInt(),
                          ui->leServerUser->text(),
                          ui->leServerPass->text()
                    );

    connect(server, SIGNAL(onError(int, const QString&)), this, SLOT(onServerError(int, const QString&)));
    connect(server, SIGNAL(onQueueBinded()), this, SLOT(onServerStarted()));
    connect(server, SIGNAL(onMessageReceived(amqp_envelope_t*)), this, SLOT(onServerMessageReceived(amqp_envelope_t*)));
    connect(server, SIGNAL(onConnectionTerminated()), this, SLOT(onServerTerminated()));

    ui->teServerLog->setText("Starting server...");
    ui->btnStartListening->setDisabled(true);
    server->start();
}

void MainWindow::addServerLog(const QString& msg)
{
    ui->teServerLog->moveCursor(QTextCursor::End);
    ui->teServerLog->textCursor().insertText("\n"+msg);
    ui->teServerLog->moveCursor(QTextCursor::End);
}

void MainWindow::onServerError(int errCode, const QString& errMsg)
{
    addServerLog(QString("Server error (code %1):").arg(errCode));
    addServerLog(errMsg);
    ui->btnStartListening->setDisabled(false);
}

void MainWindow::onServerStarted()
{
    addServerLog("Server started successfully");
}

void MainWindow::onServerMessageReceived(amqp_envelope_t* envelope)
{
  //  addServerLog("New message received...");

    //qDebug() << "MASOUD " << (char *)envelope->message.body.bytes;

    char buf[10240];
    QString msg = "";

//    sprintf(buf, "Delivery: %u, exchange: %.*s, routingkey: %.*s\n",
//                   (unsigned) envelope->delivery_tag,
//                   (int) envelope->exchange.len, (char *) envelope->exchange.bytes,
//                   (int) envelope->routing_key.len, (char *) envelope->routing_key.bytes);

//    msg += buf;

//    if (envelope->message.properties._flags & AMQP_BASIC_CONTENT_TYPE_FLAG) {
//        sprintf(buf, "Content-type: %.*s\n",
//             (int) envelope->message.properties.content_type.len,
//             (char *) envelope->message.properties.content_type.bytes);
//        msg += buf;
//    }
//    msg += "----\n";

//    addServerLog(msg);
//    addServerLog(server->getEnvelopeContent(envelope));

//    server->destroyEnvelope(envelope);

    addServerLog((char *)envelope->message.body.bytes);
}

void MainWindow::onServerTerminated()
{
    addServerLog("Server stopped successfully");
    ui->btnStartListening->setDisabled(false);
}

void MainWindow::onSendMessage()
{
    qDebug() << "Sending message...";

    ui->btnSendMessage->setDisabled(true);
    client = new RabbitMQClient(ui->leClientExchange->text(), ui->leClientRoutingKey->text(), ui->leClientHost->text(), ui->leClientPort->text().toInt(), ui->leClientUser->text(), ui->leClientPass->text());

    QMessageBox msgBox;
    if(!client->sendMessage(ui->leClientMessage->text()))
    {
        msgBox.setText("Message sent successfully.");
    }
    else
    {
        msgBox.setText("Could not send message: "+client->getLastErrorMessage());
    }
    msgBox.exec();
    ui->btnSendMessage->setDisabled(false);
}
