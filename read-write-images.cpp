#include <opencv2/highgui.hpp>
#include <iomanip>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include <iostream>
#include <string>
#include <thread>
#include <stdio.h>
#include <vector>
#include <map>

using namespace cv;
using namespace std;

int main(int argc, char **argv) {

    string fname = "sample_image.tif";

    cv::Mat image;
    image = cv::Mat(cv::Size(800,800), CV_32FC3, cv::Scalar(100.0, 55500.5, 56000.9));
//    image.convertTo(image, CV_8UC3);
    auto i = image.at<Vec3f>(0,0);

    cout << image.type() <<"\t" << i.val[0] << '\t' << i.val[1] << '\t' << i.val[2] << endl;
    imwrite(fname, image);

    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Image", image);
    cv::waitKey(0);

    image = imread("RGB_1.tif");
    image.convertTo(image, CV_32FC3);
    i = image.at<Vec3f>(0,0);
    cout << image.type() <<"\t" << i.val[0] << '\t' << i.val[1] << '\t' << i.val[2] << endl;

    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Image", image);
    cv::waitKey(0);

    //    inputImage.setTo(cv::Scalar(10,20,30));
    passingFunction(fname, image);
    return 0;
    // Serial is faster than parallel
}