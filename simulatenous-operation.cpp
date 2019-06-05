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

void blur_write(string input_fname, string output_fname, Mat output) {
    // Choose one of the two operations below. GaussianBlur is slower than cvtColor
    GaussianBlur(imread(input_fname), output, Size(5, 5), 1.0, 1.0, 0);
//    cvtColor(imread(input_fname), output, COLOR_RGB2GRAY, 1);

//    imwrite(output_fname, output); // Optional. Disk IO is intensive and slow.
}

#define NUM_THREADS 8 // The number of cores is the optimal number of threads
#define NUM_IMAGES 8
int main(int argc, char **argv) {
    string names[] = {"rj.png", "rj2.png", "rj3.png", "rj4.png", "rj5.png", "rj6.png", "rj7.png", "rj8.png"};
    // Two distinct output names to seprate the outputs of serial and parallel systems
    string gray_names1[] = {"grj.png", "grj2.png", "grj3.png", "grj4.png", "grj5.png", "grj6.png", "grj7.png", "grj8.png"};
    string gray_names2[] = {"2grj.png", "2grj2.png", "2grj3.png", "2grj4.png", "2grj5.png", "2grj6.png", "2grj7.png", "2grj8.png"};

    Mat image = imread(names[0], IMREAD_COLOR);

    // Creating placeholders for grayscaled images
    Mat gray = cv::Mat::zeros(image.rows, image.cols, CV_8UC3);
    vector<Mat> outputs;
    for(int i = 0; i <NUM_IMAGES; i++)
        outputs.push_back(gray.clone());

    // It takes 1 ms to create and destroy 10 threads
    auto start = std::chrono::system_clock::now();
    for (auto i = 0; i < NUM_IMAGES / NUM_THREADS; ) {
        // Create threads
        vector<thread> threads;
        for (int j = 0; j < NUM_THREADS; j++) {
            threads.push_back(thread(blur_write, names[j + i], gray_names1[i + j], outputs[i+j]));
        }

        // Wait for threads to complete their task
        for (int i = 0; i < threads.size(); i++)
            threads[i].join();

        // Display logs
        if ((i % 1) == 0) { cout << "i " << i << endl; }
        i += NUM_THREADS;
    }
    auto stop = std::chrono::system_clock::now();
    std::cout << "Parallel took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
              << " ms\n";

    auto start1 = std::chrono::system_clock::now();
    for (auto i = 0; i < NUM_IMAGES; i++) {
        // The size of gray should be defined before you can pass it into a thread
        blur_write(names[i % 6], gray_names2[i % 6], gray);
    }
    auto stop1 = std::chrono::system_clock::now();
    std::cout << "Serial took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1).count()
              << " ms\n";

    return 0;

    // parallel is 4.34 times faster than serial for 8 threads on 8 core machine
    // For lighter operations executed in parallel, this value may decrease
    // For all the operations demoed here, the speedup is at least 4 times.
}