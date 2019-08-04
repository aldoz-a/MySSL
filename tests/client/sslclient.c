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

#include "myssl.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <openssl/err.h>

int main(int argc, char *argv[])
{
    // test arguments
    if (argc != 3) {
        // args error
        printf("%s: wrong arguments counts\n", argv[0]);
        printf("usage: %s host port [i.e.: %s 127.0.0.1 8888]\n", argv[0], argv[0]);
        return EXIT_FAILURE;
    }

    // create a socket
    int my_socket;
    if ((my_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        // socket() error
        fprintf(stderr, "%s: could not create socket (%s)\n", argv[0], strerror(errno));
        return EXIT_FAILURE;
    }

    // prepare the structure sockaddr_in for the remote server
    struct sockaddr_in server;                      // (remote) server socket info
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;                    // set address family
    server.sin_addr.s_addr = inet_addr(argv[1]);    // set server address
    server.sin_port = htons(atoi(argv[2]));         // set port number of the server

    // connect to the remote server
    if (connect(my_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        // connect() error
        fprintf(stderr, "%s: connect failed (%s)\n", argv[0], strerror(errno));
        close(my_socket);
        return EXIT_FAILURE;
    }

    // preparing the SSL_connect(): creates an OpenSSL context
    SSL_CTX *ctx;
    int error;
    if ((ctx = sslCreateCtx(SSL_CLIENT, &error)) == NULL || error < 0) {
        // sslCreateCtx() error
        fprintf(stderr, "%s: OpenSSL error creating the context SSL_CTX\n", argv[0]);
        ERR_print_errors_fp(stderr);
        sslClose(NULL, my_socket, ctx, false);
        return EXIT_FAILURE;
    }

    // preparing the SSL_connect(): creates the data structure corresponding to the OpenSSL context
    SSL *ssl;
    if ((ssl = SSL_new(ctx)) == NULL) {
        // SSL_new() error
        fprintf(stderr, "%s: OpenSSL error creating the structure SSL\n", argv[0]);
        ERR_print_errors_fp(stderr);
        sslClose(ssl, my_socket, ctx, false);
        return EXIT_FAILURE;
    }

    // preparing the SSL_connect(): associate the SSL structure with the current socket
    if (SSL_set_fd(ssl, my_socket) == 0) {
        // SSL_set_fd() error
        fprintf(stderr, "%s: OpenSSL error on SSL_set_fd\n", argv[0]);
        ERR_print_errors_fp(stderr);
        sslClose(ssl, my_socket, ctx, false);
        return EXIT_FAILURE;
    }

    // start an OpenSSL connection using SSL_connect()
    int rc;
    if ((rc = sslFunc(SSL_connect, ssl)) != 1) {
        // sslConnect() error
        fprintf(stderr, "%s: OpenSSL error on SSL_connect (%d)\n", argv[0], SSL_get_error(ssl, rc));
        ERR_print_errors_fp(stderr);
        sslClose(ssl, my_socket, ctx, false);
        return EXIT_FAILURE;
    }

    // communication loop with the remote server
    for (;;) {
        // build a message for the remote server
        char my_msg[MYBUFSIZE];
        printf("write a message to the remote Server: ");
        scanf("%s", my_msg);

        // send message to the remote server
        if ((rc = sslWrite(ssl, my_msg, strlen(my_msg))) < 0) {
            // sslWrite() error
            fprintf(stderr, "%s: send failed (%d)\n", argv[0], SSL_get_error(ssl, rc));
            ERR_print_errors_fp(stderr);
            sslClose(ssl, my_socket, ctx, true);
            return EXIT_FAILURE;
        }

        // receive an answer from the remote server
        memset(my_msg, 0, MYBUFSIZE);
        if ((rc = sslRead(ssl, my_msg, MYBUFSIZE)) < 0) {
            // sslRead() error
            fprintf(stderr, "%s: recv failed (%d)\n", argv[0], SSL_get_error(ssl, rc));
            ERR_print_errors_fp(stderr);
            sslClose(ssl, my_socket, ctx, true);
            return EXIT_FAILURE;
        }

        // show the answer
        printf("server reply: %s\n", my_msg);
    }

    // exit with Ok
    sslClose(ssl, my_socket, ctx, true);
    return EXIT_SUCCESS;
}
