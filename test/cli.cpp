#include <iostream>
#include "net.hpp"
using namespace net;


int main() {
    int sock = create_socket((char*)"udp");
    sockaddr_in addr{};
    settings_client_udp(sock, &addr, 8080);
    bind_socket_udp(sock, &addr); //бинд сокета к порту только у клиента 

    while(true){
        int result;
        receive_udp_int(sock, &result, &addr);
        std::cout << result << '\n';
    }
}








// int main() {
//     int sock = create_socket((char*)"tcp");
//     sockaddr_in addr{};
//     settings_client(sock, &addr, 8080);
//     connect_to_server(sock, &addr);

//     int result;
//     receive(sock, &result);
//     std::cout << result << '\n';
// }
