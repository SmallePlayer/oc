#include "net.hpp"


namespace net {

int create_socket(const char* protocol) {          // create a socket by protocol name
    int sockfd;
    if (strcmp(protocol, "tcp") == 0) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
    } else if (strcmp(protocol, "udp") == 0) {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    } else {
        std::cerr << "Invalid socket type: " << protocol << std::endl;
        return -1;
    }

    if (sockfd < 0) {
        std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
        return -1;
    }

    return sockfd;
}

int close_socket(int sockfd) {
    if (close(sockfd) < 0) {
        std::cerr << "Error closing socket: " << strerror(errno) << std::endl;
        return -1;
    }
    return 0;
}

void set_reuse_addr(int sockfd) {                  // allow quick rebind after restart
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Error setting socket options: " << strerror(errno) << std::endl;
    }
}

// address setup

void set_server_addr(struct sockaddr_in* addr, int port) {
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = INADDR_ANY;
    addr->sin_port = htons(port);
}

void set_client_addr(struct sockaddr_in* addr, int port, const char* ip) {
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    if (ip == nullptr || ip[0] == '\0') ip = "127.0.0.1";
    if (inet_pton(AF_INET, ip, &addr->sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported: " << ip << std::endl;
    }
}

// tcp server / client

int bind_socket(int sockfd, struct sockaddr_in* addr) {
    if (bind(sockfd, (struct sockaddr*)addr, sizeof(*addr)) < 0) {
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        return -1;
    }
    return 0;
}

void listen_socket(int sockfd) {
    if (listen(sockfd, 1) < 0) {
        std::cerr << "Listen failed: " << strerror(errno) << std::endl;
    }
}

int accept_connection(int sockfd, struct sockaddr_in* client_addr) {
    socklen_t addrlen = sizeof(*client_addr);
    int new_socket = accept(sockfd, (struct sockaddr*)client_addr, &addrlen);
    if (new_socket < 0) {
        std::cerr << "Accept failed: " << strerror(errno) << std::endl;
        return -1;
    }
    return new_socket;
}

int connect_to_server(int sockfd, struct sockaddr_in* server_addr) {
    if (connect(sockfd, (struct sockaddr*)server_addr, sizeof(*server_addr)) < 0) {
        std::cerr << "Connection failed: " << strerror(errno) << std::endl;
        return -1;
    }
    return 0;
}

// stream data (tcp / connected udp)

void send_int(int sockfd, int value) {
    write(sockfd, &value, sizeof(value));
}

void send_str(int sockfd, const char* message) {
    write(sockfd, message, strlen(message));
}

void send_bytes(int sockfd, const void* data, size_t size) {
    write(sockfd, data, size);
}

void recv_int(int sockfd, int* result) {
    read(sockfd, result, sizeof(*result));
}

void recv_str(int sockfd, char* buffer, size_t buffer_size) {
    read(sockfd, buffer, buffer_size);
}

void recv_bytes(int sockfd, void* buffer, size_t buffer_size) {
    read(sockfd, buffer, buffer_size);
}

// datagram data (unconnected udp)

void send_to_int(int sockfd, struct sockaddr_in* addr, int value) {
    sendto(sockfd, &value, sizeof(value), 0, (struct sockaddr*)addr, sizeof(*addr));
}

void send_to_str(int sockfd, struct sockaddr_in* addr, const char* message) {
    sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)addr, sizeof(*addr));
}

ssize_t send_to_bytes(int sockfd, struct sockaddr_in* addr, const void* data, size_t size) {
    return sendto(sockfd, data, size, 0, (struct sockaddr*)addr, sizeof(*addr));
}

void recv_from_int(int sockfd, struct sockaddr_in* addr, int* result) {
    socklen_t addrlen = sizeof(*addr);
    recvfrom(sockfd, result, sizeof(*result), 0, (struct sockaddr*)addr, &addrlen);
}

void recv_from_str(int sockfd, struct sockaddr_in* addr, char* buffer, size_t buffer_size) {
    socklen_t addrlen = sizeof(*addr);
    recvfrom(sockfd, buffer, buffer_size, 0, (struct sockaddr*)addr, &addrlen);
}

ssize_t recv_from_bytes(int sockfd, struct sockaddr_in* addr, void* buffer, size_t buffer_size) {
    socklen_t addrlen = sizeof(*addr);
    return recvfrom(sockfd, buffer, buffer_size, 0, (struct sockaddr*)addr, &addrlen);
}

// delay

void delay_seconds(int sec) {
    std::this_thread::sleep_for(std::chrono::seconds(sec));
}

void delay_milliseconds(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void delay_microseconds(int usec) {
    std::this_thread::sleep_for(std::chrono::microseconds(usec));
}

} // namespace net
