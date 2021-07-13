#include <mex.h>
#include <lsd.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    std::string picture = mxArrayToString(prhs[0]);
    cv::Mat src = cv::imread(picture.c_str(), cv::IMREAD_COLOR);
    cv::Mat tmp, src_gray;
    cv::cvtColor(src, tmp, cv::COLOR_RGB2GRAY);
    tmp.convertTo(src_gray, CV_64FC1);

    image_double image = new_image_double(src_gray.cols, src_gray.rows);
    image->data = src_gray.ptr<double>(0);

    ntuple_list ntl = lsd(image);
    cv::Mat lsd = cv::Mat::zeros(image->ysize, image->xsize, CV_8UC1);
    cv::Point pt1, pt2;
    for (int j = 0; j != ntl->size ; ++j)
    {
        pt1.x = ntl->values[0 + j * ntl->dim];
        pt1.y = ntl->values[1 + j * ntl->dim];
        pt2.x = ntl->values[2 + j * ntl->dim];
        pt2.y = ntl->values[3 + j * ntl->dim];
        double width = ntl->values[4 + j * ntl->dim];
        cv::line(lsd, pt1, pt2, cv::Scalar(255), width, cv::LINE_AA);
    }
    free_ntuple_list(ntl);
    uchar *mat_array = lsd.ptr<uchar>(0);
    std::vector<int> pos;
    for (int i = 0; i != image->ysize * image->xsize; ++i)
    {
        if (mat_array[i] != 0)
        {
            pos.push_back(i + 1);
        }
    }

    plhs[0] = mxCreateDoubleMatrix(1, pos.size(), mxREAL);
    double *pos_mat = mxGetPr(plhs[0]);
    for (int i = 0; i != pos.size(); ++i)
    {
        pos_mat[i] = double(pos[i]);
    }
}