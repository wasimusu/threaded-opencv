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

void op(Mat image){
    rotate(image, image, 2);
//    cv::resize(image, image, cv::Size(), 2, 2, INTER_CUBIC);
//    image.convertTo(image, CV_32FC3);
//    cv::normalize(image, image, 0, 65535, NORM_MINMAX); // Works only for NIR images.
//    image.convertTo(image, CV_16UC3);
}

void parallel(Mat input, Mat output) {
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
    vector<Mat> ims = {im0, im1, im2, im3};

    for (int i = 0; i < 4; i++)
        threads.push_back(thread(op, ims[i]));

    // Wait for all the threads to finish their task
    for (int i = 0; i < threads.size(); i++)
        threads[i].join();

    // Merge rois into single image
    input(rois[0]) = im0;
    input(rois[1]) = im1;
    input(rois[2]) = im2;
    input(rois[3]) = im3;
}

void serial(Mat image, Mat output){
    op(image);
}

int main(int argc, char **argv) {
    string fname = "WPImage.tif";
    Mat image = imread(fname);
    Mat parallelIm = image.clone();
    Mat serialIm = image.clone();

    int N = 5; // Multiple of 5
    for (int i = 0; i < N; i++) {
        // The number of cores is the optimal number of threads
        // It takes 1 ms to create and destroy 10 threads

        auto start = std::chrono::system_clock::now();
        parallel(image, parallelIm);
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
    imshow("serial", serialIm);
    imshow("parallel", parallelIm);
    waitKey(0);
    waitKey(1000);
    return 0;

    // Serial is faster than parallel
}