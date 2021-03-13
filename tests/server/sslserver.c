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
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <openssl/err.h>

int main(int argc, char *argv[])
{
    // test arguments
    if (argc != 2) {
        // args error
        printf("%s: wrong arguments counts\n", argv[0]);
        printf("usage: %s port [i.e.: %s 8888]\n", argv[0], argv[0]);
        return EXIT_FAILURE;
    }

    // create a socket
    int my_socket;
    if ((my_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        // socket() error
        fprintf(stderr, "%s: could not create socket (%s)\n", argv[0], strerror(errno));
        return EXIT_FAILURE;
    }

    // prepare the structure sockaddr_in for this server
    struct sockaddr_in server;              // (local) server socket info
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;            // set address family
    server.sin_addr.s_addr = INADDR_ANY;    // set server address for any interface
    server.sin_port = htons(atoi(argv[1])); // set server port number

    // assign an address to the created socket
    if (bind(my_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        // bind() error
        fprintf(stderr, "%s: bind failed (%s)", argv[0], strerror(errno));
        close(my_socket);
        return EXIT_FAILURE;
    }

    // start listening with a queue of max BACKLOG connections
    if (listen(my_socket, BACKLOG) < 0) {
        // listen() error
        fprintf(stderr, "%s: listen failed (%s)\n", argv[0], strerror(errno));
        close(my_socket);
        return EXIT_FAILURE;
    }

    // accept connection from an incoming client
    printf("waiting for incoming connections...\n");
    socklen_t socksize = sizeof(struct sockaddr_in);
    struct sockaddr_in client;          // (remote) client socket info
    int client_sock;
    if ((client_sock = accept(my_socket, (struct sockaddr *)&client, &socksize)) < 0) {
        // accept() error
        fprintf(stderr, "%s: accept failed (%s)\n", argv[0], strerror(errno));
        close(my_socket);
        return EXIT_FAILURE;
    }

    // closes the socket no longer in use
    close(my_socket);

    // preparing the SSL_accept(): create an OpenSSL context
    SSL_CTX *ctx;
    int error;
    if ((ctx = sslCreateCtx(SSL_SERVER, &error)) == NULL || error < 0) {
        // sslCreateCtx() error
        fprintf(stderr, "%s: OpenSSL error creating the context SSL_CTX\n", argv[0]);
        ERR_print_errors_fp(stderr);
        sslClose(NULL, my_socket, ctx, false);
        return EXIT_FAILURE;
    }

    // preparing the SSL_accept(): create the data structure corresponding to the OpenSSL context
    SSL *ssl;
    if ((ssl = SSL_new(ctx)) == NULL) {
        // errore SSL_new()
        fprintf(stderr, "%s: OpenSSL error creating the structure SSL\n", argv[0]);
        ERR_print_errors_fp(stderr);
        sslClose(ssl, client_sock, ctx, false);
        return EXIT_FAILURE;
    }

    // preparing the SSL_accept(): associate the SSL structure with the current socket
    if (SSL_set_fd(ssl, client_sock) == 0) {
        // SSL_set_fd() error
        fprintf(stderr, "%s: OpenSSL error on SSL_set_fd\n", argv[0]);
        ERR_print_errors_fp(stderr);
        sslClose(ssl, client_sock, ctx, false);
        return EXIT_FAILURE;
    }

    // accept an OpenSSL connection with SSL_accept()
    int rc;
    if ((rc = sslFunc(SSL_accept, ssl)) != 1) {
        // sslAccept() error
        fprintf(stderr, "%s: OpenSSL error on sslAccept (%d)\n", argv[0], SSL_get_error(ssl, rc));
        ERR_print_errors_fp(stderr);
        sslClose(ssl, client_sock, ctx, false);
        return EXIT_FAILURE;
    }

    // receiving message loop from the client
    char client_msg[MYBUFSIZE];
    int recv_size;
    while ((recv_size = sslRead(ssl, client_msg, MYBUFSIZE)) > 0 ) {
        // send answer to client
        printf("received message from sock %d: %s\n", client_sock, client_msg);
        char server_msg[MYBUFSIZE + 18];
        snprintf(server_msg, sizeof(server_msg), "you wrote to me: %s", client_msg);
        if (sslWrite(ssl, server_msg, strlen(server_msg)) < 0) {
            // sslWrite() error
            fprintf(stderr, "%s: send failed (%d)\n", argv[0], SSL_get_error(ssl, recv_size));
            ERR_print_errors_fp(stderr);
            sslClose(ssl, client_sock, ctx, true);
            return EXIT_FAILURE;
        }

        // clear buffer
        memset(client_msg, 0, MYBUFSIZE);
    }

    // loop terminated: test why
    if (recv_size < 0) {
        // sslRead() error
        fprintf(stderr, "%s: recv failed (%d)\n", argv[0], SSL_get_error(ssl, recv_size));
        ERR_print_errors_fp(stderr);
        sslClose(ssl, client_sock, ctx, true);
        return EXIT_FAILURE;
    }
    else if (recv_size == 0) {
        // Ok: client disconnected
        printf("%s: client disconnected\n", argv[0]);
    }

    // exit with Ok
    sslClose(ssl, client_sock, ctx, true);
    return EXIT_SUCCESS;
}
