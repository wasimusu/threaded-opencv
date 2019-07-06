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

void someOp(Mat& image, Mat& output) {
    cv::resize(image, output, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR);
    cvtColor(image, output, COLOR_BGR2GRAY);
}

int main() {
    // Read two whitepanel images and average them
    std::string fname = "RGB/RGB_whitepanel0.tif";
    Mat image = imread(fname);
    Mat output;
    someOp(image, output);
    cout<<"Output : "<<output.rows<<endl;
    imwrite("passedimage.tif", output);
    return 0;
}