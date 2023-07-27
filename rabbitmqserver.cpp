#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>
#include <amqp_tcp_socket.h>
#include <amqp.h>
#include <amqp_framing.h>
#include <QDebug>

#include "rabbitmqutils.h"
#include "rabbitmqserver.h"

RabbitMQServer::RabbitMQServer(const QString& exchange, const QString& bindingKey, const QString& queue, const QString& hostname, const int port, const QString& user, const QString& pass)
{
    this->exchange = exchange;
    this->bindingKey = bindingKey;
    this->queue = queue;
    this->hostname = hostname;
    this->port = port;
    this->user = user;
    this->pass = pass;
}

void RabbitMQServer::run()
{
    amqp_bytes_t queuename;
    amqp_connection_state_t conn = amqp_new_connection();
    amqp_socket_t *socket = amqp_tcp_socket_new(conn);
    if (!socket)
    {
        sprintf(errBuf, RBMQ_MSG_ERROR_CREATING_SOCKET);
        emit onError(errCode = RBMQ_CODE_ERROR_CREATING_SOCKET, getLastErrorMessage());
        return;
    }

    if (amqp_socket_open(socket, hostname.toStdString().c_str(), port))
    {
        sprintf(errBuf, RBMQ_MSG_ERROR_OPENING_SOCKET);
        emit onError(errCode = RBMQ_CODE_ERROR_OPENING_SOCKET, getLastErrorMessage());
        return;
    }
    emit onConnectionEstablished();

    if(check_for_amqp_error(errBuf, amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, user.toStdString().c_str(), pass.toStdString().c_str()), "Logging in"))
    {
        emit onError(errCode = RBMQ_CODE_ERROR_LOGGING_IN, getLastErrorMessage());
        return;
    }

    amqp_channel_open(conn, 1);

    if(check_for_amqp_error(errBuf, amqp_get_rpc_reply(conn), "Opening channel"))
    {
        emit onError(errCode = RBMQ_CODE_ERROR_OPENING_CHANNEL, getLastErrorMessage());
        return;
    }
    emit onChannelOpened();

    amqp_queue_declare_ok_t *r = amqp_queue_declare(conn, 1, amqp_cstring_bytes(queue.toStdString().c_str())/*amqp_empty_bytes*/, 0, 1, 0, 0, amqp_empty_table);
    if(check_for_amqp_error(errBuf, amqp_get_rpc_reply(conn), "Declaring queue"))
    {
        emit onError(errCode = RBMQ_CODE_ERROR_DECLARING_QUEUE, getLastErrorMessage());
        return;
    }
    emit onQueueDeclared();

    queuename = amqp_bytes_malloc_dup(r->queue);
    if (queuename.bytes == NULL)
    {
        sprintf(errBuf, "Out of memory while copying queue name");
        emit onError(errCode = RBMQ_CODE_ERROR_COPYING_QUEUE, getLastErrorMessage());
        return;
    }

    amqp_queue_bind(conn, 1, queuename, amqp_cstring_bytes(exchange.toStdString().c_str()), amqp_cstring_bytes(bindingKey.toStdString().c_str()), amqp_empty_table);
    if(check_for_amqp_error(errBuf, amqp_get_rpc_reply(conn), "Binding queue"))
    {
        emit onError(errCode = RBMQ_CODE_ERROR_BINDING_QUEUE, getLastErrorMessage());
        return;
    }
    emit onQueueBinded();

    amqp_basic_consume(conn, 1, queuename, amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
    if(check_for_amqp_error(errBuf, amqp_get_rpc_reply(conn), "Consuming"))
    {
        emit onError(errCode = RBMQ_CODE_ERROR_CONSUMING_MESSAGE, getLastErrorMessage());
        return;
    }

    while(1)
    {
        amqp_rpc_reply_t res;
        amqp_envelope_t* envelope = new amqp_envelope_t;

        amqp_maybe_release_buffers(conn);

        res = amqp_consume_message(conn, envelope, NULL, 0);

        if (AMQP_RESPONSE_NORMAL != res.reply_type) {
            break;
        }

        emit onMessageReceived(envelope);
    }


    if(check_for_amqp_error(errBuf, amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel"))
    {
        emit onError(errCode = RBMQ_CODE_ERROR_CLOSING_CHANNEL, getLastErrorMessage());
        return;
    }
    emit onChannelClosed();

    if(check_for_amqp_error(errBuf, amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection"))
    {
        emit onError(errCode = RBMQ_CODE_ERROR_CLOSING_CONNECTION, getLastErrorMessage());
        return;
    }
    emit onConnectionClosed();

    if(check_for_error(errBuf, amqp_destroy_connection(conn), "Ending connection"))
    {
        emit onError(errCode = RBMQ_CODE_ERROR_ENDING_CONNECTION, getLastErrorMessage());
        return;
    }

    errCode = RBMQ_CODE_SUCCESS;
    emit onConnectionTerminated();
}

const QString RabbitMQServer::getEnvelopeContent(amqp_envelope_t* envelope)
{
    char outBuf[102400];
  //  amqp_dump(outBuf, envelope->message.body.bytes, envelope->message.body.len);
    amqp_dump(outBuf, envelope->message.body.bytes , envelope->message.body.len);

    qDebug() << "===== message.body.bytes "  <<  envelope->message.body.bytes ;
    qDebug() << "===== message.body.len "  <<  envelope->message.body.len ;
    qDebug() << "===== outBuf "  <<  outBuf ;

    return outBuf;
}

void RabbitMQServer::destroyEnvelope(amqp_envelope_t* envelope)
{
    amqp_destroy_envelope(envelope);
    delete envelope;
}
