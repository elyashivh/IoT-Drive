#pragma once

#include <stdint.h>     /* uint32_t */
#include <netinet/in.h> /* struct sockaddr_in */

#ifdef __cplusplus
extern "C"
{
#endif

    /* TCP & UDP */
    int SocketCreate(int socket_type, struct sockaddr_in *address, uint32_t port);
    void SocketClose(int socket_fd);
    void SocketBind(int server_fd, const struct sockaddr_in *address);
    void ConvertIP(int socket_fd, const char *ip, struct sockaddr_in *serv_addr);

    /* TCP  */
    /* server & client */
    void TCPSend(int socket_fd, const char *message, uint32_t size);
    int TCPReceive(int socket_fd, char *buffer, uint32_t size);
    /* server */
    void TCPListen(int socket_fd, uint32_t req_q_size, uint32_t port);
    int TCPAccept(int socket_fd, struct sockaddr_in *address, uint32_t port);
    /* client */
    void TCPConnect(int socket_fd, const struct sockaddr_in *serv_addr);

    /* UDP  */
    /* server & client */
    void UDPSendTo(int socket_fd, const char *message, uint32_t size,
                   const struct sockaddr_in *addr);
    void UDPReceiveFrom(int socket_fd, char *buffer, uint32_t size, int mode,
                        const struct sockaddr_in *addr);
    void UDPBroadcast(int socket_fd, struct sockaddr_in *broadcast_addr);

#ifdef __cplusplus
} /* extern "C" */
#endif
