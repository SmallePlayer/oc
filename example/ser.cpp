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
//     settings_server_udp(sock, &addr, 8080);
//     int i = 0;
//     while(i < 100){ 
//         int num{i++};
//         send_udp_int(sock, &addr, num);
//         delay_seconds(1);
//     }
// }



int main() {
    Robot robot;
    robot.x = 1.0;
    robot.y = 2.0;
    robot.angle = 3.14;

    int sock = create_socket((char*)"tcp"); // Создание TCP сокета
    sockaddr_in addr{}; // Создание структуры для хранения адреса сервера
    settings_server(sock, &addr, 8080); // Настройка адреса сервера (IP и порт)
    bind_socket(sock, &addr); // Привязка сокета к адресу и порту
    listen_socket(sock); // Прослушивание входящих соединений
    sockaddr_in client_addr{}; // Создание структуры для хранения адреса клиента
    int client_sock = accept_connection(sock, &client_addr); // Принятие входящего соединения от клиента

    send_struct(client_sock, &robot);
    //send(client_sock, 42);
}
