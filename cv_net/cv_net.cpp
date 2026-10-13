#include "cv_net.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>

using namespace net;

cv::VideoCapture setup_camera(int camera_index, int width, int height, int fps) {
    cv::VideoCapture cap(camera_index);
    if (!cap.isOpened()) {
        std::cerr << "Не удалось открыть камеру\n";
        exit(1);
    }

    // Настройки для максимального FPS: MJPEG вместо несжатого формата
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    cap.set(cv::CAP_PROP_FPS, fps);

    return cap;
}

cv::Mat capture_frame(cv::VideoCapture& cap) {
    cv::Mat frame;
    cap >> frame;                   // берём кадр
    return frame;
}

size_t compress_frame(const cv::Mat& frame, std::vector<uchar>& buffer, int quality) {
    std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, quality}; // Параметры сжатия JPEG
    cv::imencode(".jpg", frame, buffer, params);
    return buffer.size();
}

size_t decompress_frame(const std::vector<uchar>& buffer, cv::Mat& frame) {
    frame = cv::imdecode(buffer, cv::IMREAD_COLOR);
    if (frame.empty()) return 0;
    return frame.total() * frame.elemSize();
}

void send_frame(int sockfd, struct sockaddr_in* addr, std::vector<uchar>& buffer) {
    size_t chunks = (buffer.size() + CHUNK_SIZE - 1) / CHUNK_SIZE;
    for (size_t i = 0; i < chunks; ++i) {
        size_t offset = i * CHUNK_SIZE;
        size_t size = std::min(CHUNK_SIZE, buffer.size() - offset);
        send_to_bytes(sockfd, addr, buffer.data() + offset, size);
    }
    send_to_bytes(sockfd, addr, nullptr, 0);   // 0 байт = конец кадра
}

size_t receive_frame(int sockfd, std::vector<uchar>& buffer, struct sockaddr_in* from) {
    buffer.clear();
    unsigned char chunk[CHUNK_SIZE];
    while (true) {
        ssize_t n = recv_from_bytes(sockfd, from, chunk, sizeof(chunk));
        if (n < 0) continue;
        if (n == 0) break;                      // 0 байт = конец кадра
        buffer.insert(buffer.end(), chunk, chunk + n);
    }
    return buffer.size();
}

