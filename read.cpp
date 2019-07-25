#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iomanip>
#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char **argv) {

    string fname = "RGB_whitepanel0.tif";

    cv::Mat image0 = cv::imread(fname, cv::IMREAD_GRAYSCALE); // grayscale images
    cv::Mat image1 = cv::imread(fname, cv::IMREAD_ANYDEPTH); // 16 - 32 bit images.
    cv::Mat image2 = cv::imread(fname, 3); // color images

    cout<<"Depths : "<<image0.depth() <<"\t"<<image1.depth() <<"\t"<<image2.depth() <<"\n";

    double min, max;

    minMaxLoc(image0, &min, &max);
    cout<<"Image 0 : "<<image0.channels()<<"\t"<<min<<"\t"<<max<<"\n";

    minMaxLoc(image1, &min, &max);
    cout<<"Image 1 : "<<image1.channels()<<"\t"<<min<<"\t"<<max<<"\n";

    minMaxLoc(image2, &min, &max);
    cout<<"Image 2 : "<<image2.channels()<<"\t"<<min<<"\t"<<max<<"\n";

    return 0;
}