#include <iostream>
#include "net.hpp"

using namespace net;

struct Robot {
    double x;
    double y;
    double angle;
};




// int main() {
//     int sock = create_socket((char*)"udp");
//     sockaddr_in addr{};
//     settings_client_udp(sock, &addr, 8080);
//     bind_socket_udp(sock, &addr); //бинд сокета к порту только у клиента 

//     while(true){
//         int result;
//         receive_udp_int(sock, &result, &addr);
//         std::cout << result << '\n';
//     }
// }




int main() {
    Robot robot;
    
    int sock = create_socket((char*)"tcp"); // Создание TCP сокета
    sockaddr_in addr{}; // Создание структуры для хранения адреса сервера
    settings_client(sock, &addr, 8080); // Настройка адреса сервера (IP и порт)
    connect_to_server(sock, &addr); // Подключение к серверу

    // int result;
    // receive(sock, &result);
    receive_struct(sock, &robot);
    std::cout << robot.x << ' ' << robot.y << ' ' << robot.angle << '\n';
}
