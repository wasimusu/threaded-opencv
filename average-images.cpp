#include <opencv2/highgui.hpp>
#include <iomanip>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <opencv2/core/ocl.hpp>

#include <iostream>
#include <string>
#include <thread>
#include <stdio.h>
#include <vector>
#include <queue>

using namespace cv;
using namespace std;

int main() {
    int height = 2832;
    int width = 4240;
    Mat image;
    string camera_type = "RGB";

    // Read two whitepanel images and average them
    std::string fname1 = camera_type + "/" + camera_type + "_" + "whitepanel0.tif";
    std::string fname2 = camera_type + "/" + camera_type + "_" + "whitepanel1.tif";
    cv::Mat image1 = cv::imread(fname1);
    cv::Mat image2 = cv::imread(fname2);
    if (image1.empty()) {
        std::cout << "Could not open or find inputWPImage: " << fname1 << std::endl;
    }
    if (image2.empty()) {
        std::cout << "Could not open or find inputWPImage: " << fname2 << std::endl;
    }
    cv::addWeighted(image1, 0.5, image2, 0.5, 0, image);

    // Resize image
    int c1 = cv::INTER_LINEAR;
    int c2 = cv::INTER_CUBIC;

    Mat linear, cubic, diff;
    cv::Mat refImage = cv::imread("whitepanel_resized.tif");

    cv::resize(image, linear, cv::Size(width, height), 0, 0, cv::INTER_LINEAR);
    std::cout << "Image size : " << image.rows << " , " << image.cols << std::endl;
    cv::resize(image, cubic, cv::Size(width, height), 0, 0,  cv::INTER_CUBIC);

    std::cout<<"Image types : "<<refImage.type() <<"\t"<<linear.type()<<"\t"<<cubic.type()<<endl;
    cv::absdiff(cubic, refImage, diff);

    std::cout << "Image size : "  <<"\t"<<linear.rows << " , " << linear.cols << std::endl;
    double r = cv::sum(diff)[0];
    double g = cv::sum(diff)[1];
    double b = cv::sum(diff)[2];
    diff.convertTo(diff, CV_8UC3, 80);
    std::cout << "Diff : " << diff.type() <<"\t"<< r << "\t" << g << "\t" << b << endl;


    // Write image
    std::string wp_fname = camera_type + "/" + camera_type + "_" + std::to_string(c2) + ".tif";
    cv::imwrite(wp_fname, cubic);

    return 0;
}