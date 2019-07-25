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

void blur_write(string input_fname, string output_fname, UMat output) {
    // Compression factor for output image
    vector<int> compression_params(2);
    compression_params.push_back(int(cv::IMWRITE_JPEG_QUALITY));
    compression_params.push_back(10);

    // Choose one of the two operations below. GaussianBlur is slower than cvtColor
//    GaussianBlur(imread(input_fname), output, Size(5, 5), 1.0, 1.0, 0);
    GaussianBlur(imread(input_fname).getUMat(ACCESS_READ), output, Size(5, 5), 1.0, 1.0, 0);
//    cvtColor(imread(input_fname), output, COLOR_RGB2GRAY, 1);
//    resize(output, output,cv::Size(), 1, 1);
    rotate(output, output, 1);
//    imwrite(output_fname, output); // Optional. Disk IO is intensive and slow.
//    UMat result = cv::imread(output_fname);
//    cv::imwrite(output_fname,result,compression_params);
}

#define NUM_THREADS 8 // The number of cores is the optimal number of threads
#define NUM_IMAGES 8

int main(int argc, char **argv) {
    if (!cv::ocl::haveOpenCL()){
        cout << "OpenCL is not available..."<<endl;
    }

    string names[] = {"rj.png", "rj2.png", "rj3.png", "rj4.png", "rj5.png", "rj6.png", "rj7.png", "rj8.png"};
    // Two distinct output names to seprate the outputs of serial and parallel systems
    string gray_names1[] = {"grj.jpg", "grj2.jpg", "grj3.jpg", "grj4.jpg", "grj5.jpg", "grj6.jpg", "grj7.jpg",
                            "grj8.jpg"};
    string gray_names2[] = {"2grj.jpg", "2grj2.jpg", "2grj3.jpg", "2grj4.jpg", "2grj5.jpg", "2grj6.jpg", "2grj7.jpg",
                            "2grj8.jpg"};

    UMat image = imread(names[0], IMREAD_COLOR).getUMat(ACCESS_READ);

    // Creating placeholders for grayscaled images
    UMat gray = cv::UMat::zeros(image.rows, image.cols, CV_8UC3);
    vector<UMat> outputs;
    for (int i = 0; i < NUM_IMAGES; i++)
        outputs.push_back(gray.clone());

    // It takes 1 ms to create and destroy 10 threads
    auto start = std::chrono::system_clock::now();
    for (int j = 0; j < 1000; j++) {
        for (auto i = 0; i < NUM_IMAGES / NUM_THREADS;) {
            // Create threads
            vector<thread> threads;
            for (int j = 0; j < NUM_THREADS; j++) {
                threads.push_back(thread(blur_write, names[j + i], gray_names1[i + j], outputs[i + j]));
            }

            // Wait for threads to complete their task
            for (int i = 0; i < threads.size(); i++)
                threads[i].join();

            i += NUM_THREADS;
        }
    }
    auto stop = std::chrono::system_clock::now();
    std::cout << "Parallel took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
              << " ms\n";

    auto start1 = std::chrono::system_clock::now();
    for (int j = 0; j < 1000; j++) {
        for (auto i = 0; i < NUM_IMAGES; i++) {
            // The size of gray should be defined before you can pass it into a thread
            blur_write(names[i % 6], gray_names2[i % 6], gray);
        }
    }
    auto stop1 = std::chrono::system_clock::now();
    std::cout << "Serial took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1).count()
              << " ms\n";

    return 0;

    // parallel is 4.34 times faster than serial for 8 threads on 8 core machine
    // For lighter operations executed in parallel, this value may decrease
    // For all the operations demoed here, the speedup is at least 4 times.
}