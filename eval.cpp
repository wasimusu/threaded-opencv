#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iomanip>
#include <iostream>
#include <stdio.h>

using namespace std;

bool image_captured(cv::Mat image) {
    // Make provision to receive raw image? Or read from hard disk?
    // Check if the image is not blank.
    double sum = cv::sum(image)[0];
    if (sum == 0) { return false; }

    // Now check if there is enough texture in the image for feature extractors to work
    // Find edges and check if there are enough of them.
    return true;
}

bool has_enough_features(cv::Mat image) {
    cv::Mat gray, grad_x, grad_y, grad;

    cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY, 1);
    cv::GaussianBlur(gray, gray, cv::Size(3, 3), 1.0, 1.0, 0);

    cv::Sobel(gray, grad_x, CV_16S, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(grad_x, grad_x);

    cv::Sobel(gray, grad_y, CV_16S, 0, 1, 3, 1, 0, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(grad_y, grad_y);

    cv::addWeighted(grad_x, 0.5, grad_y, 0.5, 0, grad);
    cv::threshold(grad, grad, 10, 255, 0);


    // Split the image into grids and sum each of them and divide by their size
    int colStep = 2;
    int rowStep = 2;
    int roiWidth = grad.cols/rowStep;
    int roiHeight = grad.rows/colStep;

    std::vector<int> hist;
    for(int r = 0; r < rowStep; r++){
        for(int c = 0; c < colStep; c++){
            cv::Mat roi(grad,  cv::Rect(c*roiWidth, r*roiHeight, roiWidth, roiHeight));
            int sum = cv::sum(roi)[0]/(roiHeight*roiWidth*255);
            hist.push_back(sum);
        }
    }

    for(int i=0; i<hist.size(); i++){
        std::cout<<hist[i]<<"\t";
    }
    std::cout<<std::endl;

    return true;
}

int main() {
    std::string fname = "RGB_0.JPG";
    cv::Mat image = cv::imread(fname);

    bool state = has_enough_features(image);
    std::cout << "The image is good to go : " << state << std::endl;
}