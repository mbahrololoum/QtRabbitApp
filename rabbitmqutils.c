/* vim:set ft=c ts=2 sw=2 sts=2 et cindent: */
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

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <amqp.h>
#include <amqp_framing.h>

#include "rabbitmqutils.h"

int check_for_error(char* outBuf, int x, char const *context)
{
    if (x < 0) {
        sprintf(outBuf, "%s: %s\n", context, amqp_error_string2(x));
        return 1;
    }
    return 0;
}

int check_for_amqp_error(char* outBuf, amqp_rpc_reply_t x, char const *context)
{
    switch (x.reply_type) {
    case AMQP_RESPONSE_NORMAL:
        return 0;

    case AMQP_RESPONSE_NONE:
        sprintf(outBuf, "%s: missing RPC reply type!\n", context);
        break;

    case AMQP_RESPONSE_LIBRARY_EXCEPTION:
        sprintf(outBuf, "%s: %s\n", context, amqp_error_string2(x.library_error));
        break;

    case AMQP_RESPONSE_SERVER_EXCEPTION:
        switch (x.reply.id) {
        case AMQP_CONNECTION_CLOSE_METHOD: {
            amqp_connection_close_t *m = (amqp_connection_close_t *) x.reply.decoded;
            sprintf(outBuf, "%s: server connection error %uh, message: %.*s\n",
                    context,
                    m->reply_code,
                    (int) m->reply_text.len, (char *) m->reply_text.bytes);
            break;
        }
        case AMQP_CHANNEL_CLOSE_METHOD: {
            amqp_channel_close_t *m = (amqp_channel_close_t *) x.reply.decoded;
            sprintf(outBuf, "%s: server channel error %uh, message: %.*s\n",
                    context,
                    m->reply_code,
                    (int) m->reply_text.len, (char *) m->reply_text.bytes);
            break;
        }
        default:
            sprintf(outBuf, "%s: unknown server error, method id 0x%08X\n", context, x.reply.id);
            break;
        }
        break;
    }

    return 1;
}

static void dump_row(char* outBuf, long count, int numinrow, int *chs)
{
    int i;
    char tempBuf[10240];

    strcpy(outBuf, "");

    sprintf(tempBuf, "%08lX:", count - numinrow);
    strcat(outBuf, tempBuf);

    if (numinrow > 0) {
        for (i = 0; i < numinrow; i++) {
            if (i == 8) {
                sprintf(tempBuf, " :");
                sprintf(tempBuf, "");
                strcat(outBuf, tempBuf);
            }
            sprintf(tempBuf, " %02X", chs[i]);
            strcat(outBuf, tempBuf);
        }
        for (i = numinrow; i < 16; i++) {
            if (i == 8) {
                sprintf(tempBuf, " :");
                strcat(outBuf, tempBuf);
            }
            sprintf(tempBuf, "   ");
            strcat(outBuf, tempBuf);
        }
        sprintf(tempBuf, "  ");
        strcat(outBuf, tempBuf);
        for (i = 0; i < numinrow; i++) {
            if (isprint(chs[i])) {
                sprintf(tempBuf, "%c", chs[i]);
            } else {
                sprintf(tempBuf, ".");
            }
            strcat(outBuf, tempBuf);
        }
    }
    sprintf(tempBuf, "\n");
    strcat(outBuf, tempBuf);
}

static int rows_eq(int *a, int *b)
{
    int i;

    for (i=0; i<16; i++)
        if (a[i] != b[i]) {
            return 0;
        }

    return 1;
}

void amqp_dump(char* outBuf, void const *buffer, size_t len)
{

    unsigned char *buf = (unsigned char *) buffer;
    char tempBuf[10240];
    long count = 0;
    int numinrow = 0;
    int chs[16];
    int oldchs[16] = {0};
    int showed_dots = 0;
    size_t i;

    strcpy(outBuf, "");

    for (i = 0; i < len; i++) {
        int ch = buf[i];

        if (numinrow == 16) {
            int j;

            if (rows_eq(oldchs, chs)) {
                if (!showed_dots) {
                    showed_dots = 1;
                    sprintf(tempBuf, " .. .. .. .. .. .. .. .. : .. .. .. .. .. .. .. ..\n");
                    strcat(outBuf, tempBuf);
                }
            } else {
                showed_dots = 0;
                dump_row(tempBuf, count, numinrow, chs);
                strcat(outBuf, tempBuf);
            }

            for (j=0; j<16; j++) {
                oldchs[j] = chs[j];
            }

            numinrow = 0;
        }

        count++;
        chs[numinrow++] = ch;
    }

    dump_row(tempBuf, count, numinrow, chs);
    strcat(outBuf, tempBuf);

    if (numinrow != 0) {
        sprintf(tempBuf, "%08lX:\n", count);
        strcat(outBuf, tempBuf);
    }
}
