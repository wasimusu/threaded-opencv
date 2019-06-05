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

void split_merge(Mat input, Mat output) {
    int rows = input.rows;
    int cols = input.cols;

    Rect r1 = Rect(0, 0, cols / 2, rows / 2);
    Rect r2 = Rect(cols / 2, 0, cols / 2, rows / 2);
    Rect r3 = Rect(0, rows / 2, cols / 2, rows / 2);
    Rect r4 = Rect(cols / 2, rows / 2, cols / 2, rows / 2);
    vector<Rect> rois{r1, r2, r3, r4};

    // Split the tasks into several threads
    vector<thread> threads;
    for (int i = 0; i < 4; i++)
        threads.push_back(thread(GaussianBlur, input(rois[i]), output(rois[i]), Size(5, 5), 1.0, 1.0, 0));

    // Wait for all the threads to finish their task
    for (int i = 0; i < threads.size(); i++)
        threads[i].join();
}

int main(int argc, char **argv) {
    string names[] = {"rj.png", "rj2.png", "rj3.png", "rj4.png", "rj5.png"};
    string gnames[] = {"grj.png", "grj2.png", "grj3.png", "grj4.png", "grj5.png"};

    Mat image1, image2, image3, image4, image5;
    image1 = imread(names[0], IMREAD_COLOR);

    // Creating placeholders for grayscaled images
    Mat gray1 = cv::Mat::zeros(image1.rows, image1.cols, CV_8UC3);
    cout << "Blurred image channels : " << gray1.channels() << endl;

    cout << "Filename : " << names[0] << endl;
    int N = 5; // Multiple of 5
    for (int i = 0; i < N; i++) {
        // The number of cores is the optimal number of threads
        // It takes 1 ms to create and destroy 10 threads

        auto start1 = std::chrono::system_clock::now();
        GaussianBlur(image1, gray1, Size(5, 5), 1.0, 1.0, 0);
        auto stop1 = std::chrono::system_clock::now();
        std::cout << "Serial took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1).count()
                  << " ms\n";

        auto start = std::chrono::system_clock::now();
        split_merge(image1, gray1);
        auto stop = std::chrono::system_clock::now();
        std::cout << "Parallel took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
                  << " ms\n";
    }
    return 0;

    // Serial is faster than parallel
}