#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <cstddef>
#include "net.hpp"

constexpr size_t CHUNK_SIZE = 1400;

cv::VideoCapture setup_camera(int camera_index = 0, int width = 1920, int height = 1080, int fps = 30);
cv::Mat capture_frame(cv::VideoCapture& cap);
size_t compress_frame(const cv::Mat& frame, std::vector<uchar>& buffer, int quality = 80);
size_t decompress_frame(const std::vector<uchar>& buffer, cv::Mat& frame);
void send_frame(int sockfd, struct sockaddr_in* addr, std::vector<uchar>& buffer);
size_t receive_frame(int sockfd, std::vector<uchar>& buffer, struct sockaddr_in* from);
ssize_t receive_frame(int sockfd, struct sockaddr_in* addr, std::vector<uchar>& buffer);