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

//using namespace cv;
//using namespace std;

void someOp(cv::Mat &image, cv::Mat &output) {
    // Not all operations are suitable for split and merge technique.
    // Like those that change the size of output or number of channels or other things that require global context like normalization.
    // multiply.
    // resize and rotate will require smart merging.

    cv::rotate(image, output, 1);
    image.convertTo(output, CV_32FC3, 10);
    output.convertTo(output, CV_16UC3, 0.10);
}

class ParallelizeOp : public cv::ParallelLoopBody {
private:
    cv::Mat input;
    cv::Mat output;
    const int num_threads = 4;
public:
    ParallelizeOp(cv::Mat &input, cv::Mat &output) :
            input(input), output(output) {}

    virtual void operator()(const cv::Range &range) const {
//    virtual void operator()(std::function<void(cv::Mat &, cv::Mat &)> func) const{
        int rows = input.rows;
        int cols = input.cols;
        std::cout<<"Input : "<<rows<<" , "<<cols<<"\n";

        cv::Rect r1 = cv::Rect(0, 0, cols / 2, rows / 2);
        cv::Rect r2 = cv::Rect(cols / 2, 0, cols / 2, rows / 2);
        cv::Rect r3 = cv::Rect(0, rows / 2, cols / 2, rows / 2);
        cv::Rect r4 = cv::Rect(cols / 2, rows / 2, cols / 2, rows / 2);
        std::vector <cv::Rect> rois{r1, r2, r3, r4};

//        for(int i = 0; i < num_threads; i++){
        for (int i = range.start; i < range.end; i++) {
            cv::Mat in(input, rois[i]);
            cv::Mat out(output, rois[3 - i]);
            someOp(std::ref(in), std::ref(out)); // Works perfectly well.
        }
    }
};

void parallelold(cv::Mat &input, cv::Mat &output) {
    int rows = input.rows;
    int cols = input.cols;

    cv::Rect r1 = cv::Rect(0, 0, cols / 2, rows / 2);
    cv::Rect r2 = cv::Rect(cols / 2, 0, cols / 2, rows / 2);
    cv::Rect r3 = cv::Rect(0, rows / 2, cols / 2, rows / 2);
    cv::Rect r4 = cv::Rect(cols / 2, rows / 2, cols / 2, rows / 2);
    std::vector <cv::Rect> rois{r1, r2, r3, r4};

    // Split into rois
    cv::Mat im0 = input(rois[0]);
    cv::Mat im1 = input(rois[1]);
    cv::Mat im2 = input(rois[2]);
    cv::Mat im3 = input(rois[3]);
    std::vector <cv::Mat> inputs = {im0, im1, im2, im3};
    cv::Mat o1, o2, o3, o4;
    std::vector <cv::Mat> outputs = {o1, o2, o3, o4};
    output = input.clone();

    // Split the tasks into several threads
    std::vector <std::thread> threads;
    for (int i = 0; i < rois.size(); i++) {
        threads.push_back(std::thread(someOp, std::ref(inputs[i]), std::ref(outputs[i])));
    }

    // Wait for all the threads to finish their task
    // Copying back to the output is the only overhead of merge and split technique.
    // Smartly waiting for threads to complete their jobs.
    for (int i = 0; i < threads.size(); i++) {
        threads[i].join();
        outputs[i].copyTo(output(rois[3 - i])); // Merge it smartly to allow rotation and resizing as well.
    }
}

void parallel(cv::Mat &input, cv::Mat &output) {
    // Parallelize without having to copy at the end. Copying can be huge overhead.

    int rows = input.rows;
    int cols = input.cols;

    cv::Rect r1 = cv::Rect(0, 0, cols / 2, rows / 2);
    cv::Rect r2 = cv::Rect(cols / 2, 0, cols / 2, rows / 2);
    cv::Rect r3 = cv::Rect(0, rows / 2, cols / 2, rows / 2);
    cv::Rect r4 = cv::Rect(cols / 2, rows / 2, cols / 2, rows / 2);
    std::vector <cv::Rect> rois{r1, r2, r3, r4};

    // Split the tasks into several threads
    std::vector <std::thread> threads;
    output = input.clone();
    for (int i = 0; i < rois.size(); i++) {
        cv::Mat in(input, rois[i]);
        cv::Mat out(output, rois[3 - 1]);
        threads.push_back(std::thread(someOp, std::ref(in), std::ref(out)));
    }

    // Wait for all the threads to finish their task
    for (int i = 0; i < threads.size(); i++) { threads[i].join(); }
    threads.clear();
}


void serial(cv::Mat &image, cv::Mat &output) {
    someOp(image, output);
}

int main(int argc, char **argv) {
    std::string fname = "WPImage.tif";
    cv::Mat image = cv::imread(fname);
    cv::Mat parallelIm;
    cv::Mat serialIm;
    cv::Mat parallelNew;
    cv::Mat parallelfor;

    int N = 5; // Multiple of 5
    for (int i = 0; i < N; i++) {
        // The number of cores is the optimal number of threads
        // It takes 1 ms to create and destroy 10 threads

        auto start2 = std::chrono::system_clock::now();
        parallelold(image, parallelIm);
        auto stop2 = std::chrono::system_clock::now();
        std::cout << "Parallel Old took "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(stop2 - start2).count()
                  << " ms\n";

        auto start = std::chrono::system_clock::now();
        parallel(image, parallelNew);
        auto stop = std::chrono::system_clock::now();
        std::cout << "Parallel took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
                  << " ms\n";

//        auto start3 = std::chrono::system_clock::now();
////        cv::parallel_for_(someOp, ParallelizeOp(image, parallelfor));
//        cv::parallel_for_(cv::Range(0, 4), ParallelizeOp(std::ref(image), std::ref(parallelfor)));
//        auto stop3 = std::chrono::system_clock::now();
//        std::cout << "parallel_for_ took "
//                  << std::chrono::duration_cast<std::chrono::milliseconds>(stop3 - start3).count()
//                  << " ms\n";

        auto start1 = std::chrono::system_clock::now();
        serial(image, serialIm);
        auto stop1 = std::chrono::system_clock::now();
        std::cout << "Serial took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1).count()
                  << " ms\n";

        std::cout << std::endl;
    }

    cv::imwrite("Parallel.tif", parallelNew);
    return 0;
}