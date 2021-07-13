/**
 * @file lsd_opencv_example.cpp
 *
 * Test the LSD algorithm with OpenCV
 */
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <lsd.h>
#include <iostream>

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 2)
    {
        std::cout << "Usage: lsd_opencv_example imageName" << std::endl;
        return -1;
    }
    cv::Mat src = cv::imread(argv[1], cv::IMREAD_COLOR);
    cv::Mat tmp, src_gray;
    cv::cvtColor(src, tmp, cv::COLOR_RGB2GRAY);
    tmp.convertTo(src_gray, CV_64FC1);

    int cols  = src_gray.cols;
    int rows = src_gray.rows;

    image_double image = new_image_double(cols, rows);
    image->data = src_gray.ptr<double>(0);
    ntuple_list ntl = lsd(image);

    cv::Mat lsd = cv::Mat::zeros(rows, cols, CV_8UC1);
    cv::Point pt1, pt2;
    for (int j = 0; j != ntl->size ; ++j)
    {
        pt1.x = int(ntl->values[0 + j * ntl->dim]);
        pt1.y = int(ntl->values[1 + j * ntl->dim]);
        pt2.x = int(ntl->values[2 + j * ntl->dim]);
        pt2.y = int(ntl->values[3 + j * ntl->dim]);
        int width = int(ntl->values[4 + j * ntl->dim]);
        cv::line(lsd, pt1, pt2, cv::Scalar(255), width, cv::LINE_AA);
    }
    free_ntuple_list(ntl);

    cv::namedWindow("src", cv::WINDOW_AUTOSIZE);
    cv::imshow("src", src);
    cv::namedWindow("lsd", cv::WINDOW_AUTOSIZE);
    cv::imshow("lsd", lsd);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}