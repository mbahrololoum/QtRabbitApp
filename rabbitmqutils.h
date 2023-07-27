/* vim:set ft=c ts=2 sw=2 sts=2 et cindent: */
#ifndef librabbitmq_examples_utils_h
#define librabbitmq_examples_utils_h

/*
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MIT
 *
 * Portions created by Alan Antonuk are Copyright (c) 2012-2013
 * Alan Antonuk. All Rights Reserved.
 *
 * Portions created by VMware are Copyright (c) 2007-2012 VMware, Inc.
 * All Rights Reserved.
 *
 * Portions created by Tony Garnock-Jones are Copyright (c) 2009-2010
 * VMware, Inc. and Tony Garnock-Jones. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * ***** END LICENSE BLOCK *****
 */
#ifdef __cplusplus
extern "C" {
#endif

#define RBMQ_CODE_SUCCESS 0
#define RBMQ_CODE_ERROR_CREATING_SOCKET 1
#define RBMQ_CODE_ERROR_OPENING_SOCKET 2
#define RBMQ_CODE_ERROR_LOGGING_IN 3
#define RBMQ_CODE_ERROR_OPENING_CHANNEL 4
#define RBMQ_CODE_ERROR_DECLARING_QUEUE 5
#define RBMQ_CODE_ERROR_COPYING_QUEUE 6
#define RBMQ_CODE_ERROR_BINDING_QUEUE 7
#define RBMQ_CODE_ERROR_CONSUMING_MESSAGE 8
#define RBMQ_CODE_ERROR_CLOSING_CHANNEL 9
#define RBMQ_CODE_ERROR_CLOSING_CONNECTION 10
#define RBMQ_CODE_ERROR_ENDING_CONNECTION 11
#define RBMQ_CODE_ERROR_SENDING_MESSAGE 12

#define RBMQ_MSG_ERROR_CREATING_SOCKET "Error creating TCP socket"
#define RBMQ_MSG_ERROR_OPENING_SOCKET "Error opening TCP socket"

int check_for_error(char* outBuf, int x, char const *context);
int check_for_amqp_error(char* outBuf, amqp_rpc_reply_t x, char const *context);

void amqp_dump(char* outBuf, void const *buffer, size_t len);

uint64_t now_microseconds(void);
void microsleep(int usec);

#ifdef __cplusplus
}
#endif

#endif
