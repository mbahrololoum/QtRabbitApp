#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>
#include <amqp_tcp_socket.h>
#include <amqp.h>
#include <amqp_framing.h>

#include "rabbitmqutils.h"
#include "rabbitmqclient.h"

RabbitMQClient::RabbitMQClient(const QString& exchange, const QString& routingKey, const QString& hostname, const int port, const QString& user, const QString& pass)
{
    this->exchange = exchange;
    this->routingKey = routingKey;
    this->hostname = hostname;
    this->port = port;
    this->user = user;
    this->pass = pass;
}

int RabbitMQClient::sendMessage(const QString& messageBody)
{
    amqp_connection_state_t conn = amqp_new_connection();
    amqp_socket_t *socket = amqp_tcp_socket_new(conn);
    if (!socket)
    {
        sprintf(errBuf, RBMQ_MSG_ERROR_CREATING_SOCKET);
        return errCode = RBMQ_CODE_ERROR_CREATING_SOCKET;
    }

    if (amqp_socket_open(socket, hostname.toStdString().c_str(), port))
    {
        sprintf(errBuf, RBMQ_MSG_ERROR_OPENING_SOCKET);
        return errCode = RBMQ_CODE_ERROR_OPENING_SOCKET;
    }

    if(check_for_amqp_error(errBuf, amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, user.toStdString().c_str(), pass.toStdString().c_str()), "Logging in"))
    {
        return errCode = RBMQ_CODE_ERROR_LOGGING_IN;
    }

    amqp_channel_open(conn, 1);

    if(check_for_amqp_error(errBuf, amqp_get_rpc_reply(conn), "Opening channel"))
    {
        return errCode = RBMQ_CODE_ERROR_OPENING_CHANNEL;
    }

    amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.content_type = amqp_cstring_bytes("text/plain");
    props.delivery_mode = 2; /* persistent delivery mode */
    if(check_for_error(errBuf, amqp_basic_publish(conn,
                                    1,
                                    amqp_cstring_bytes(exchange.toStdString().c_str()),
                                    amqp_cstring_bytes(routingKey.toStdString().c_str()),
                                    0,
                                    0,
                                    &props,
                                    amqp_cstring_bytes(messageBody.toStdString().c_str())),
                 "Publishing"))
    {
        return errCode = RBMQ_CODE_ERROR_SENDING_MESSAGE;
    }

    if(check_for_amqp_error(errBuf, amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel"))
    {
        return errCode = RBMQ_CODE_ERROR_CLOSING_CHANNEL;
    }

    if(check_for_amqp_error(errBuf, amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection"))
    {
        return errCode = RBMQ_CODE_ERROR_CLOSING_CONNECTION;
    }

    if(check_for_error(errBuf, amqp_destroy_connection(conn), "Ending connection"))
    {
        return errCode = RBMQ_CODE_ERROR_ENDING_CONNECTION;
    }

    return errCode = RBMQ_CODE_SUCCESS;
}
