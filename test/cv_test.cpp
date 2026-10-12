#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>


double calculate_fps(int64& prev_tick) {
    int64 now_tick = cv::getTickCount();
    double dt = (now_tick - prev_tick) / cv::getTickFrequency();
    prev_tick = now_tick;
    return (dt > 0.0) ? 1.0 / dt : 0.0;
}

size_t compress_frame(const cv::Mat& frame, std::vector<uchar>& buffer, int quality = 80) {
    std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, quality}; // Параметры сжатия JPEG
    cv::imencode(".jpg", frame, buffer, params);
    return buffer.size();
}



const size_t CHUNK_SIZE = 1400;   // полезная часть пакета

int main() {
    cv::VideoCapture cap(0);            // 0 — первая камера
    if (!cap.isOpened()) {
        std::cerr << "Не удалось открыть камеру\n";
        return 1;
    }

    // Настройки для максимального FPS: MJPEG вместо несжатого формата
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);
    cap.set(cv::CAP_PROP_FPS, 30);

    cv::Mat frame;
    std::vector<uchar> buffer; // Буфер для сжатого кадра
    

    double fps = 0.0;
    int64 prev = cv::getTickCount();

    while (true) {
        cap >> frame;                   // берём кадр
        if (frame.empty()) break;

        compress_frame(frame, buffer, 80);

        fps = calculate_fps(prev);             // вычисляем FPS

        size_t frame_bytes = frame.total() * frame.elemSize(); // вес кадра в байтах
        double frame_mib = frame_bytes / (1024.0 * 1024.0);

        size_t jpeg_bytes = buffer.size();
        double jpeg_kib = jpeg_bytes / 1024.0;

        size_t chunk_count = (buffer.size() + CHUNK_SIZE - 1) / CHUNK_SIZE;

        for (size_t i = 0; i < chunk_count; ++i) {
            size_t offset = i * CHUNK_SIZE;
            size_t size   = std::min(CHUNK_SIZE, buffer.size() - offset);

            // buf.data() + offset, длина size — это один кусок
        }

        cv::putText(frame, cv::format("FPS: %.1f", fps),
                    {10, 30}, cv::FONT_HERSHEY_SIMPLEX, 1.0,
                    {0, 255, 0}, 2);    // рисуем FPS на кадре
        cv::putText(frame, cv::format("Size: %.2f MiB", frame_mib),
                    {10, 65}, cv::FONT_HERSHEY_SIMPLEX, 1.0,
                    {0, 255, 0}, 2);    // рисуем вес кадра
        cv::putText(frame, cv::format("JPEG: %.2f KiB", jpeg_kib),
                    {10, 100}, cv::FONT_HERSHEY_SIMPLEX, 1.0,
                    {0, 255, 0}, 2);    // рисуем вес сжатого кадра
        cv::putText(frame, cv::format("Chunks: %zu", chunk_count),
                    {10, 135}, cv::FONT_HERSHEY_SIMPLEX, 1.0,
                    {0, 255, 0}, 2);    // рисуем количество кусков
                    

        cv::imshow("camera", frame);    // показываем
        if (cv::waitKey(1) == 27) break; // выход по Esc
    }
    return 0;
}