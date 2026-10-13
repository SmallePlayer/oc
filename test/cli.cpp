#include <iostream>
#include "net.hpp"
using namespace net;


int main() {
    int sock = create_socket("udp");
    sockaddr_in addr{};
    set_client_addr(&addr, 8080);
    bind_socket(sock, &addr); //бинд сокета к порту только у клиента

    while(true){
        int result;
        recv_from_int(sock, &addr, &result);
        std::cout << result << '\n';
    }
}








// int main() {
//     int sock = create_socket("tcp");
//     sockaddr_in addr{};
//     set_client_addr(&addr, 8080);
//     connect_to_server(sock, &addr);

//     int result;
//     recv_int(sock, &result);
//     std::cout << result << '\n';
// }
