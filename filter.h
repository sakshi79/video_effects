#ifndef FILTER_H
#define FILTER_H

#include <opencv2/opencv.hpp>

// Image manipulation functions
cv::Mat toGrayscale(const cv::Mat& input);
int GreyScale(const cv::Mat& src, cv::Mat& dst);
int Sepia(const cv::Mat& src, cv::Mat& dst, float strength=0.85f);

int blur5x5_1( cv::Mat &src, cv::Mat &dst );
int blur5x5_2(cv::Mat &src, cv::Mat &dst);

int sobelX3x3(cv::Mat &src, cv::Mat &dst);
int sobelY3x3(cv::Mat &src, cv::Mat &dst);
int magnitude(cv::Mat &sx, cv::Mat &sy, cv::Mat &dst);

int blurQuantize(cv::Mat &src, cv::Mat &dst, int levels = 10);

int blurOutsideFaces(cv::Mat &src, cv::Mat &dst, std::vector<cv::Rect> &faces);
int emboss(cv::Mat &src, cv::Mat &dst);
int colorFace(cv::Mat &src, cv::Mat &dst, std::vector<cv::Rect> &faces);
int negative(cv::Mat &src, cv::Mat &dst);
int haloSparkles(cv::Mat &src, cv::Mat &dst, std::vector<cv::Rect> &faces);

#endif