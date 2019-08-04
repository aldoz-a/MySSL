/*
 * Copyright © 2019 Aldo Abate <aldo.abate99@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/*!
 *  FILE
 *      myssl-utils.c - utilities functions for MySSL library
 *  PROJECT
 *      MySSL library
 *  FUNCTIONS
 *      global:
 *          bool sslRecovery(SSL *ssl, int sslresult);
 *      local:
 *          int sslSelectRd(SSL *ssl);
 *          int sslSelectWr(SSL *ssl);
 *  DESCRIPTION
 *      The MySSL library is a simple interface to OpenSSL library to permits user-friendly writing of Servers and
 *      Clients using OpenSSL.
 *  NOTES
 *      - The free software library "OpenSSL" is distributed under a "dual licensed" system: under the OpenSSL License
 *        and the SSLeay License. The OpenSSL License is Apache License 1.0 and SSLeay License bears some similarity to
 *        a 4-clause BSD License. Both licenses apply.
 *      - The library "OpenSSL" reference version is 1.0.2g
 *  AUTHOR
 *      Aldo Abate
 *  OPERATING SYSTEM
 *      Linux (kernel 2.6 and above - glibc 2.3.2 and above)
 *  COMPILER
 *      GNU GCC (ver. 4.4 and above)
 *  RELEASE
 *      0.1.0 (August 2019)
 */

#include "myssl.h"
#include "myssl-private.h"

// local prototypes
static int sslSelectRd(SSL *ssl);
static int sslSelectWr(SSL *ssl);


////////////////////////////////////////////////////////////////////////////////
// GLOBAL functions
////////////////////////////////////////////////////////////////////////////////


/*!
 *  NAME
 *      sslRecovery - execute recovery on a failed ssl operation
 *  SYNOPSIS
 *      bool sslRecovery(
 *          SSL *ssl,               // OpenSSL SSL structure
 *          int sslresult);         // ssl result to recover
 *  DESCRIPTION
 *      sslRecovery() execute a recovery action on a failed ssl operation.
 *  RETURN VALUE
 *      Upon successful completion, sslRecovery() shall return true.
 *      Otherwise, false shall be returned.
 */

bool sslRecovery(
    SSL *ssl,                       // OpenSSL SSL structure
    int sslresult)                  // ssl result to recover
{
    bool result = false;    // default result

    // test ssl error
    switch (SSL_get_error(ssl, sslresult)) {
    case SSL_ERROR_WANT_READ:
        // no data available right now: wait (using select()) a few seconds in case new data arrives
        if (sslSelectRd(ssl) > 0)
            result = true;  // more data to read

        break;

    case SSL_ERROR_WANT_WRITE:
        // socket not writable right now: wait (using select()) a few seconds and try again
        if (sslSelectWr(ssl) > 0)
            result = true;  // can write more data now

        break;

    case SSL_ERROR_ZERO_RETURN:
        // error: peer disconnected
        break;

    default:
        break;
    }

    // return result of the recovery action
    return result;
}


////////////////////////////////////////////////////////////////////////////////
// LOCAL functions
////////////////////////////////////////////////////////////////////////////////


/*!
 *  NAME
 *      sslSelectRd - execute a select() on a ssl file descriptor
 *  SYNOPSIS
 *      int sslSelectRd(
 *          SSL *ssl);              // OpenSSL SSL structure
 *  DESCRIPTION
 *      sslSelectRd() execute a select() on a ssl file descriptor corresponding to the read stream.
 *  RETURN VALUE
 *      sslSelectRd return the result of a select() operation on the ssl file descriptor.
 */

static int sslSelectRd(
    SSL *ssl)                       // OpenSSL SSL structure
{
    // get file descriptor associated to ssl socket
    int sock = SSL_get_rfd(ssl);

    // set file descriptor set associated to socket fd <sock>
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(sock, &fds);

    // set timeout
    struct timeval timeout;
    timeout.tv_sec  = 0;
    timeout.tv_usec = SSL_RWTOUT;   // timeout

    // return the result of a select() operation on the ssl file descriptor
    return select(sock + 1, &fds, NULL, NULL, &timeout);
}


/*!
 *  NAME
 *      sslSelectWr - execute a select() on a ssl file descriptor
 *  SYNOPSIS
 *      int sslSelectWr(
 *          SSL *ssl);              // OpenSSL SSL structure
 *  DESCRIPTION
 *      sslSelectWr() execute a select() on a ssl file descriptor corresponding to the write stream.
 *  RETURN VALUE
 *      sslSelectWr return the result of a select() operation on the ssl file descriptor.
 */

static int sslSelectWr(
    SSL *ssl)                       // OpenSSL SSL structure
{
    // get file descriptor associated to ssl socket
    int sock = SSL_get_wfd(ssl);

    // set file descriptor set associated to socket fd <sock>
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(sock, &fds);

    // set timeout
    struct timeval timeout;
    timeout.tv_sec  = 0;
    timeout.tv_usec = SSL_RWTOUT;   // timeout

    // return the result of a select() operation on the ssl file descriptor
    return select(sock + 1, NULL, &fds, NULL, &timeout);
}
