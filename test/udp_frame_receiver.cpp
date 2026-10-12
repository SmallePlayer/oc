#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include "net.hpp"
#include "cv_net.hpp"

using namespace net;

double calculate_fps(int64& prev_tick) {
    int64 now_tick = cv::getTickCount();
    double dt = (now_tick - prev_tick) / cv::getTickFrequency();
    prev_tick = now_tick;
    return (dt > 0.0) ? 1.0 / dt : 0.0;
}

int main() {
    int sock = create_socket((char*)"udp");
    sockaddr_in addr{};
    settings_server_udp(sock, &addr, 8080);     // INADDR_ANY:8080
    bind_socket_udp(sock, &addr);

    sockaddr_in from{};
    std::vector<uchar> jpeg;

    double fps = 0.0;
    int64 prev = cv::getTickCount();

    std::cout << "waiting for frames on udp 8080...\n";
    while (true) {
        size_t bytes = receive_frame(sock, jpeg, &from);
        if (bytes == 0) continue;

        cv::Mat frame;
        decompress_frame(jpeg, frame);
        if (!frame.empty()) {
            std::cout << "received " << bytes << " bytes in " << (bytes / 1024.0) << " KiB" << '\n';
            cv::imshow("udp frames", frame);
            if (cv::waitKey(1) == 27) break;
        }
    }
    return 0;
}
