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
 *      sslfunc.c - sslFunc() function for MySSL library
 *  PROJECT
 *      MySSL library
 *  FUNCTIONS
 *      global:
 *          int sslFunc(int (*pfunc)(SSL*), SSL *ssl);
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
 *      sslFunc - smart wrapper for sslConnect()/sslAccept()/sslShutdown()
 *  SYNOPSIS
 *      int sslFunc(
 *          int (*pfunc)(SSL*),     // pointer to the OpenSSL function to execute
 *          SSL *ssl)               // OpenSSL SSL structure
 *  DESCRIPTION
 *      sslFunc() is a smart wrapper for the sslConnect(), sslAccept() and sslShutdown() function of the OpenSSL library. This is a smart method to call the
 *      required OpenSSL function bypassing the architectural features of OpenSSL that includes read/write actions in every read/write/accept/connect/shutdown
 *      action. This function analize the required OpenSSL function result to repeat the action (after a timeout of SSL_RWTOUT ms) until the activity is
 *      terminated. if the number of repeated iterations is too high (>SSL_RWITER), the activity is terminated with an error (total timeout is SSL_RWTOUT * SSL_RWITER seconds).
 *  RETURN VALUE
 *      Upon successful completion, sslWrite() shall return the number of characters sent.
 *      Otherwise, -1 shall be returned and the error can be analyzed calling the OpenSSL library SSL_get_error() function with the return value to find out the
 *      reason.
 */

int sslFunc(
    int (*pfunc)(SSL*),             // pointer to the OpenSSL function to execute
    SSL *ssl)                       // OpenSSL SSL structure
{
    int result;

    // loop di esecuzione della funzione
    int count = 0;
    for (;;) {
        // test loop counter (tot.timeout = SSL_RWTOUT * SSL_RWITER)
        if (++count > SSL_RWITER) {
            // the total timeout is elapsed: break loop (e.g.: tot.timeout = 100 ms * 20 = 2 sec)
            break;
        }

        // executes the required function
        result = pfunc(ssl);
        if (result > 0) {
            // operation Ok: break loop
            break;
        }
        else {
            // operation NOK (result <= 0): start recovery procedure
            if (sslRecovery(ssl, result))
                continue; // more data to treat: continue in the loop

            // break loop
            break;
        }
    }

    // return the result of the required function or error
    return result;
}
