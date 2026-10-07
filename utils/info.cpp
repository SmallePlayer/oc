#include <string>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <iostream>

void print_interfaces() {
    struct ifaddrs* ifaddr = nullptr;
    if (getifaddrs(&ifaddr) == -1) {
        std::cerr << "getifaddrs: " << strerror(errno) << std::endl;
        return;
    }

    for (struct ifaddrs* ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr) continue;
        if (ifa->ifa_addr->sa_family != AF_INET) continue;

        struct sockaddr_in* addr = (struct sockaddr_in*)ifa->ifa_addr;
        char buf[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr->sin_addr, buf, sizeof(buf));

        std::cout << ifa->ifa_name << " -> " << buf << std::endl;
    }

    freeifaddrs(ifaddr);
}