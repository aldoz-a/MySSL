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

#ifndef MYSSL_PRIVATE_H
#define MYSSL_PRIVATE_H

/*!
 *  FILE
 *      myssl-private.h -  MySSL library private header file
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

// nomi file certificati
#define RSA_SERVER_CERT     "client.pem"
#define RSA_SERVER_KEY      "key.pem"
#define RSA_CLIENT_CA_CERT  "ca.pem"

// timeout e iterazioni per funzioni interne OpenSSL: sslRead()/sslwrite()/sslFunc
#define SSL_RWTOUT  100000  // timeout per funzioni interne (e.g.: 100000 us = 100 ms)
#define SSL_RWITER  20      // numero di iterazioni in RWSSL_TOUT
                            // (e.g.: tot.timeout = 100 ms * 20 = 2 sec

// prototipi globali
bool sslRecovery(SSL *ssl, int sslresult);

#endif /* MYSSL_PRIVATE_H */
