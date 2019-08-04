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
 *      sslclose.c - sslClose() function for MySSL library
 *  PROJECT
 *      MySSL library
 *  FUNCTIONS
 *      global:
 *          void sslClose(SSL *ssl, int sock, SSL_CTX *ctx, bool do_shutdown);
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
#include <unistd.h>


////////////////////////////////////////////////////////////////////////////////
// GLOBAL functions
////////////////////////////////////////////////////////////////////////////////


/*!
 *  NAME
 *      sslClose - close an OpenSSL session
 *  SYNOPSIS
 *      void sslClose(
 *          SSL     *ssl,           // OpenSSL SSL structure
 *          int     sock,           // socket used by the session
 *          SSL_CTX *ctx,           // OpenSSL context used by the session
 *          bool    do_shutdown);   // flag to eventually force the execution of SSL_shutdown()
 *  DESCRIPTION
 *      sslClose() close an active OpenSSL session. Arguments are: ssl (OpenSSL SSL structure), sock (socket used by the session),
 *      ctx (OpenSSL context used by the session), do_shutdown (flag to eventually force the execution of SSL_shutdown()).
 *  RETURN VALUE
 *      None.
 */

void sslClose(
    SSL     *ssl,                   // OpenSSL SSL structure
    int     sock,                   // socket used by the session
    SSL_CTX *ctx,                   // OpenSSL context used by the session
    bool    do_shutdown)            // flag to eventually force the execution of SSL_shutdown()
{
    // frees the SSL structure (and executes shutdown) if allocated
    if (ssl) {
        // eventually executes shutdown
        if (do_shutdown)
            sslFunc(SSL_shutdown, ssl);

        // free the allocated SSL structure
        SSL_free(ssl);
    }

    // frees the socket in use
    close(sock);

    // frees context if allocated
    if (ctx)
        SSL_CTX_free(ctx);
}
