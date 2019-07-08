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

using namespace cv;
using namespace std;

void someOp(Mat& image, Mat& output){
    // Not all operations are suitable for split and merge technique.
    // Like those that change the size of output or number of channels or other things that require global context like normalization.
    // multiply.
    // resize and rotate will require smart merging.

    rotate(output, output, 1);
    image.convertTo(output, CV_32FC3, 10);
    cv::multiply(output, output, output);
    output.convertTo(output, CV_16UC3, 0.10);
}

void parallel(Mat& input, Mat& output) {
    int rows = input.rows;
    int cols = input.cols;

    Rect r1 = Rect(0, 0, cols / 2, rows / 2);
    Rect r2 = Rect(cols / 2, 0, cols / 2, rows / 2);
    Rect r3 = Rect(0, rows / 2, cols / 2, rows / 2);
    Rect r4 = Rect(cols / 2, rows / 2, cols / 2, rows / 2);
    vector<Rect> rois{r1, r2, r3, r4};

    // Split the tasks into several threads
    vector<std::thread> threads;

    // Split into rois
    Mat im0 = input(rois[0]);
    Mat im1 = input(rois[1]);
    Mat im2 = input(rois[2]);
    Mat im3 = input(rois[3]);
    vector<Mat> inputs = {im0, im1, im2, im3};
    Mat o1, o2, o3, o4;
    vector<Mat> outputs = {o1, o2, o3, o4};

    for (int i = 0; i < rois.size(); i++) {
        threads.push_back(std::thread(someOp, std::ref(inputs[i]), std::ref(inputs[i])));
    }

    // Wait for all the threads to finish their task
    // Copying back to the output is the only overhead of merge and split technique.
    // Smartly waiting for threads to complete their jobs.
    for (int i = 0; i < threads.size(); i++) {
        threads[i].join();
        inputs[i].copyTo(input(rois[3-i])); // Merge it smartly to allow rotation and resizing as well.
    }
}

void serial(Mat& image, Mat& output){
    someOp(image, output);
}

int main(int argc, char **argv) {
    string fname = "WPImage.tif";
    Mat image = imread(fname);
    Mat parallelIm;
    Mat serialIm;

    int N = 5; // Multiple of 5
    for (int i = 0; i < N; i++) {
        // The number of cores is the optimal number of threads
        // It takes 1 ms to create and destroy 10 threads

        auto start = std::chrono::system_clock::now();
        parallel(image, image);
        auto stop = std::chrono::system_clock::now();
        std::cout << "Parallel took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
                  << " ms\n";

        auto start1 = std::chrono::system_clock::now();
        serial(image, serialIm);
        auto stop1 = std::chrono::system_clock::now();
        std::cout << "Serial took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1).count()
                  << " ms\n";

    }

    cv::namedWindow("serial");
    cv::namedWindow("Parallel");
//    imshow("serial", serialIm);
    imwrite("Parallel.tif", image);
//    imshow("parallel", serialIm);
//    cout<<"Serial Image : "<<serialIm.rows<<"\t"<<serialIm.cols<<endl;
//    waitKey(1000);
//    waitKey(1000);
    return 0;
}