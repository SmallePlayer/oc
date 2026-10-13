#pragma once

#include <iostream>
#include <cstring>
#include <cstddef>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <chrono>
#include <thread>

namespace net {

// creation / options

int create_socket(const char* protocol);
void set_reuse_addr(int sockfd);

// address setup (IPv4)

void set_server_addr(struct sockaddr_in* addr, int port);
void set_client_addr(struct sockaddr_in* addr, int port, const char* ip = nullptr);

// tcp server / client

int bind_socket(int sockfd, struct sockaddr_in* addr);
void listen_socket(int sockfd);
int accept_connection(int sockfd, struct sockaddr_in* client_addr);
int connect_to_server(int sockfd, struct sockaddr_in* server_addr);

// stream data (tcp / connected udp)

void send_int(int sockfd, int value);
void send_str(int sockfd, const char* message);
void send_bytes(int sockfd, const void* data, size_t size);

void recv_int(int sockfd, int* result);
void recv_str(int sockfd, char* buffer, size_t buffer_size);
void recv_bytes(int sockfd, void* buffer, size_t buffer_size);

template <typename T>
void send_struct(int sockfd, const T* data) {
    const char* buf = reinterpret_cast<const char*>(data);
    size_t total = 0;
    while (total < sizeof(T)) {
        ssize_t n = write(sockfd, buf + total, sizeof(T) - total);
        if (n <= 0) return;
        total += n;
    }
}

template <typename T>
void recv_struct(int sockfd, T* data) {
    char* buf = reinterpret_cast<char*>(data);
    size_t total = 0;
    while (total < sizeof(T)) {
        ssize_t n = read(sockfd, buf + total, sizeof(T) - total);
        if (n == 0) return;      // peer closed the connection
        if (n < 0)  return;      // error (could check errno, retry on EINTR)
        total += n;
    }
}

// datagram data (unconnected udp)

void send_to_int(int sockfd, struct sockaddr_in* addr, int value);
void send_to_str(int sockfd, struct sockaddr_in* addr, const char* message);
ssize_t send_to_bytes(int sockfd, struct sockaddr_in* addr, const void* data, size_t size);

void recv_from_int(int sockfd, struct sockaddr_in* addr, int* result);
void recv_from_str(int sockfd, struct sockaddr_in* addr, char* buffer, size_t buffer_size);
ssize_t recv_from_bytes(int sockfd, struct sockaddr_in* addr, void* buffer, size_t buffer_size);

// delay

void delay_seconds(int sec);
void delay_milliseconds(int ms);
void delay_microseconds(int usec);

}
