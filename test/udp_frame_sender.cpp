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
    settings_client_udp(sock, &addr, 8080);     // получатель: 127.0.0.1:8080

    cv::VideoCapture cap = setup_camera(0, 1920, 1080, 30);

    double fps = 0.0;
    int64 prev = cv::getTickCount();

    while (true) {
        cv::Mat frame = capture_frame(cap);
        if (frame.empty()) break;

        std::vector<uchar> jpeg;
        compress_frame(frame, jpeg, 80);
        send_frame(sock, &addr, jpeg);

        std::cout << "sent " << jpeg.size() << " bytes in " << (jpeg.size() / 1024.0) << " KiB" << '\n';
    }
    return 0;
}
