#include "net.hpp"
using namespace net;


int main() {
    int sock = create_socket((char*)"udp");
    sockaddr_in addr{};
    settings_server_udp(sock, &addr, 8080);
    int i = 0;
    while(i < 100){ 
        int num{i++};
        send_udp_int(sock, &addr, num);
        delay_seconds(1);
    }
}



// int main() {
//     int sock = create_socket((char*)"tcp");
//     sockaddr_in addr{};
//     settings_server(sock, &addr, 8080);
//     bind_socket(sock, &addr);
//     listen_socket(sock);
//     sockaddr_in client_addr{};
//     int client_sock = accept_connection(sock, &client_addr);

//     send(client_sock, 42);
// }
