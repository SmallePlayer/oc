#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include "net.hpp"
#include "cv_net.hpp"

using namespace net;

int main() {
    int sock = create_socket("udp");
    sockaddr_in addr{};
    set_client_addr(&addr, 8080);     // получатель: 127.0.0.1:8080

    cv::VideoCapture cap = setup_camera(0, 1920, 1080, 30);

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
