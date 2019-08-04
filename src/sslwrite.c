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
 *      sslwrite.c - sslWrite() function for MySSL library
 *  PROJECT
 *      MySSL library
 *  FUNCTIONS
 *      global:
 *          int sslWrite(SSL *ssl, const void *buf, int num);
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


////////////////////////////////////////////////////////////////////////////////
// GLOBAL functions
////////////////////////////////////////////////////////////////////////////////


/*!
 *  NAME
 *      sslWrite - write data to a SSL/TLS connection
 *  SYNOPSIS
 *      int sslWrite(
 *          SSL        *ssl,        // OpenSSL SSL structure
 *          const void *buf,        // buffer of data to write
 *          int        num);        // number of data to write
 *  DESCRIPTION
 *      sslWrite() writes num bytes from the buffer buf into the specified ssl connection. This is a smart method to call the OpenSSL library SSL_write()
 *      bypassing the architectural features of OpenSSL that includes read/write actions in every read/write/accept/connect/shutdown action. This function
 *      analize the SSL_write() result to repeat the action (after a timeout of SSL_RWTOUT ms) until the activity is terminated. if the number of repeated
 *      iterations is too high (>SSL_RWITER), the activity is terminated with an error (total timeout is SSL_RWTOUT * SSL_RWITER seconds).
 *  RETURN VALUE
 *      Upon successful completion, sslWrite() shall return the number of characters sent.
 *      Otherwise, -1 shall be returned and the error can be analyzed calling the OpenSSL library SSL_get_error() function with the return value to find out the
 *      reason.
 */

int sslWrite(
    SSL        *ssl,                // OpenSSL SSL structure
    const void *buf,                // buffer of data to write
    int        num)                 // number of data to write
{
    int sent;

    // write loop
    int count = 0;
    for (;;) {
        // test loop counter (tot.timeout = SSL_RWTOUT * SSL_RWITER)
        if (++count > SSL_RWITER) {
            // the total timeout is elapsed: break loop (e.g.: tot.timeout = 100 ms * 20 = 2 sec)
            break;
        }

        // execute operation
        sent = SSL_write(ssl, buf, num);
        if (sent > 0) {
            // operation Ok: break loop
            break;
        }
        else {
            // operation NOK (result <= 0): start recovery procedure
            if (sslRecovery(ssl, sent))
                continue; // more data to treat: continue in the loop

            // break loop
            break;
        }
    }

    // return the number of sent bytes or error
    return sent;
}
