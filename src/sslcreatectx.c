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
 *      sslcreatectx.c - sslCreateCtx() function for MySSL library
 *  PROJECT
 *      MySSL library
 *  FUNCTIONS
 *      global:
 *          SSL_CTX* sslCreateCtx(int type, int *error);
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
 *      sslCreateCtx - create an OpenSSL context
 *  SYNOPSIS
 *      SSL_CTX* sslCreateCtx(
 *          int type,               // context type: SSL_SERVER/SSL_CLIENT
 *          int *error);            // error flag
 *  DESCRIPTION
 *      sslCreateCtx() create a new OpenSSL context. createSSLContex() execute the necessary initial actions to use the OpenSSL library and open/verify the
 *      certificate files.
 *  RETURN VALUE
 *      Upon successful completion, sslCreateCtx() shall return a valid OpenSSL context-descriptor.
 *      Otherwise, NULL shall be returned and an error flag is set to indicate the error.
 */

SSL_CTX* sslCreateCtx(
    int type,                       // context type: SSL_SERVER/SSL_CLIENT
    int *error)                     // error flag
{
    // Load encryption & hashing algorithms for the SSL program
    SSL_library_init();

    // Load the error strings for SSL & CRYPTO APIs
    SSL_load_error_strings();

    // Create a SSL_METHOD structure (choose a SSL/TLS protocol version)
    const SSL_METHOD *meth = SSLv23_method();

    // create a SSL_CTX structure
    *error = 0;     // default: no error
    SSL_CTX *my_ctx;
    if ((my_ctx = SSL_CTX_new(meth)) == NULL) {
        // error: set OpenSSL error flag and return context (==NULL)
        *error = 0;
        return my_ctx;
    }

    // test mode (server/client)
    if (type == SSL_SERVER) {
        // SERVER: load the server certificate into the SSL_CTX structure
        if (SSL_CTX_use_certificate_file(my_ctx, RSA_SERVER_CERT, SSL_FILETYPE_PEM) != 1) {
            // error: set error flag and return context
            *error = -1;
            return my_ctx;
        }

        // load the private-key corresponding to the server certificate
        if (SSL_CTX_use_PrivateKey_file(my_ctx, RSA_SERVER_KEY, SSL_FILETYPE_PEM) != 1) {
            // error: set error flag and return context
            *error = -1;
            return my_ctx;
        }

        // check if the server certificate and private-key matches
        if (SSL_CTX_check_private_key(my_ctx) != 1) {
            // error: set error flag and return context
            *error = -1;
            return my_ctx;
        }
    }
    else {
        // CLIENT: load the RSA CA certificate into the SSL_CTX structure. This will allow this client to verify the server's certificate
        if (SSL_CTX_load_verify_locations(my_ctx, RSA_CLIENT_CA_CERT, NULL) != 1) {
            // error: set error flag and return context
            *error = -1;
            return my_ctx;
        }

        // set flag in context to require peer (server) certificate verification
        SSL_CTX_set_verify(my_ctx, SSL_VERIFY_PEER, NULL);
        SSL_CTX_set_verify_depth(my_ctx, 1);
    }

    // unset error flag and return a valid OpenSSL context-descriptor
    *error = 0;
    return my_ctx;
}
