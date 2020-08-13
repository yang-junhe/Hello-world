#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>

#define u32 long

#define SERVER_IP	"192.168.1.133"
#define SERVER_PORT	((uint16_t)7007)
#define BUFF_SIZE	(1024 * 6)


int main(int argc, char *argv[])
{
    int conn_sock;
    char test_str[BUFF_SIZE]	= "tcp echo test";
    struct sockaddr_in	server_addr;

    conn_sock	= socket(AF_INET, SOCK_STREAM, 0);
    if (conn_sock < 0) {
        perror("socket(2) error");
        goto create_err;
    }

    (void)memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family		= AF_INET;
    server_addr.sin_port		= htons(SERVER_PORT);
    if (argc != 3) {
        server_addr.sin_addr.s_addr	= inet_addr(SERVER_IP);
    } else {
        server_addr.sin_addr.s_addr	= inet_addr(argv[1]);
        snprintf(test_str, BUFF_SIZE, "%s", argv[2]);
    }

    if (connect(conn_sock,
                (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect(2) error");
        goto err;
    }

    if (write(conn_sock, test_str, strlen(test_str)) < 0) {
        perror("send data error");
        goto err;
    }
    (void)memset(test_str, 0, BUFF_SIZE);
    if (read(conn_sock, test_str, BUFF_SIZE) < 0) {
        perror("receive data error");
        goto err;
    }
    printf("%s\n", test_str);
    fflush(stdout);

    return EXIT_SUCCESS;

 err:
    close(conn_sock);
 create_err:
    fprintf(stderr, "client error");
    return EXIT_FAILURE;
}
