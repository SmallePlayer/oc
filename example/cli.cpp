#include <iostream>
#include "net.hpp"

using namespace net;

struct Robot {
    double x;
    double y;
    double angle;
};




// int main() {
//     int sock = create_socket("udp");
//     sockaddr_in addr{};
//     set_client_addr(&addr, 8080);
//     bind_socket(sock, &addr); //бинд сокета к порту только у клиента

//     while(true){
//         int result;
//         recv_from_int(sock, &addr, &result);
//         std::cout << result << '\n';
//     }
// }




int main() {
    Robot robot;

    int sock = create_socket("tcp"); // Создание TCP сокета
    sockaddr_in addr{}; // Создание структуры для хранения адреса сервера
    set_client_addr(&addr, 8080); // Настройка адреса сервера (IP и порт)
    connect_to_server(sock, &addr); // Подключение к серверу

    recv_struct(sock, &robot);
    std::cout << robot.x << ' ' << robot.y << ' ' << robot.angle << '\n';
}
