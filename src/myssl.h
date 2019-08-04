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

#ifndef MYSSL_H
#define MYSSL_H

/*!
 *  FILE
 *      myssl.h -  MySSL library public header file
 *  PROJECT
 *      MySSL library
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

#include <openssl/ssl.h>
#include <stdbool.h>

// tipi per sslCreateCtx()
#define SSL_SERVER  0
#define SSL_CLIENT  1

// altre define
#define BACKLOG     10      // numero connessioni per coda listen(): valore ragionevole
                            // per multi-connect (e non fa danni in single-connect)
#define MYBUFSIZE   1024    // size buffer per send/recv

// prototipi globali
SSL_CTX* sslCreateCtx(int type, int *error);
int      sslWrite(SSL *ssl, const void *buf, int num);
int      sslRead(SSL *ssl, void *buf, int num);
int      sslFunc(int (*pfunc)(SSL*), SSL *ssl);
void     sslClose(SSL *ssl, int sock, SSL_CTX *ctx, bool do_shutdown);

#endif /* MYSSL_H */
