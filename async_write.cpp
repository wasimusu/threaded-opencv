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
#include <queue>

#include <unistd.h>
#include <mutex>

using namespace cv;
using namespace std;

mutex mu;

queue<cv::Mat> images;
queue<std::string> filenames;
bool STREAMING = true;
int sleeptime = 50000;
int wrote = 0;

void writefiles(){
    // A dedicated process / thread to just write files.
    // Now there won't be any contention for resouces (disk IO) between threads.
    // Probably saves some CPU resources.

    // Invoke the function for once, and be STREAMING with it.
    // Change STREAMING to kill the function.
    while(1){
//        cout << "STREAMING : \t" <<STREAMING << "\tImage in buffer : " <<images.size() << endl;
        // The queues may not always have files but the we're not STREAMING.
        if(images.size() == 0){
            // Sleep for one second.
//            cout <<"Sleeping : "<<sleeptime<<"\tImages : "<<images.size()<<"\n";
            usleep(sleeptime);
//            cout<<"Woke up\n";
            // If it wakes up before there are files in the queue there will be segmentation fault.
        }

        try {
            if (images.size() == 0){throw "Hey man. Error!. Sorry for that. Please sleep in the meanwhile.";}

            // Pop, store values and write file
            std::string fname = filenames.front();
            cv::Mat image = images.front();
            filenames.pop();
            images.pop();
            wrote += 1;   // a shared variable in all of the threads.

            cv::imwrite(fname, image);

            // debugging purpose
//            cout <<"Wrote : "<<fname<<"\t"<<wrote<<endl;
        }
        catch (...){
//            cout <<"Threw error : yay!"<<endl;
//            cout <<"Error : " << e;
        }

        if((STREAMING == false) && (images.size() == 0)) {
            cout << "STREAMING : \t" <<STREAMING << "\tImage in buffer : " <<images.size() << endl;
            break;
        }

    }
    return;
}

void blur_write(string input_fname, string output_fname, Mat output) {
    // Compression factor for output image
    vector<int> compression_params(2);
    compression_params.push_back(int(cv::IMWRITE_JPEG_QUALITY));
    compression_params.push_back(10);

    // Choose one of the two operations below. GaussianBlur is slower than cvtColor
    GaussianBlur(imread(input_fname), output, Size(5, 5), 1.0, 1.0, 0);
//    cvtColor(imread(input_fname), output, COLOR_RGB2GRAY, 1);
//    resize(output, output,cv::Size(), 1, 1);
    rotate(output, output, 1);

    // Push files for async writing.
    images.push(output);
    filenames.push(output_fname);
//    cout << "Pushed : "<<output_fname<<"\t"<<filenames.size()<<endl;

//    cv::imwrite(output_fname, output, compression_params);
}

#define NUM_THREADS 8 // The number of cores is the optimal number of threads
#define NUM_IMAGES 8
int main(int argc, char **argv) {

    // Generate input filenames, output filenames
    vector<std::string> inputNames, serialNames, parallelNames;
    for(int i=0; i<8; i++){
        inputNames.push_back("RGB_"+std::to_string(i)+".tif");
        parallelNames.push_back("Parallel_RGB_"+std::to_string(i)+".tif");
        serialNames.push_back("Serial_RGB_"+std::to_string(i)+".tif");
    }

    Mat image = imread(inputNames[0], IMREAD_COLOR);

    // Creating placeholders for grayscaled images
    Mat gray = cv::Mat::zeros(image.rows, image.cols, CV_8UC3);
    vector<Mat> outputs;
    for(int i = 0; i <NUM_IMAGES; i++)
        outputs.push_back(gray.clone());

    std::thread t1(writefiles);
    std::thread t3(writefiles);
    std::thread t5(writefiles);
    std::thread t7(writefiles);

    // It takes 1 ms to create and destroy 10 threads
    // We need to stop processing files if there are too many files waiting to be written to the disk.
    auto start = std::chrono::system_clock::now();
    for(int c =0; c < 50; c++){
        cout << c << endl;
        for (auto i = 0; i < NUM_IMAGES / NUM_THREADS; ) {
            // Create threads
            vector<thread> threads;
            for (int j = 0; j < NUM_THREADS; j++) {
                threads.push_back(thread(blur_write, inputNames[j + i], parallelNames[i + j], outputs[i+j]));
            }

            // Wait for threads to complete their task
            for (int i = 0; i < threads.size(); i++)
                threads[i].join();

            i += NUM_THREADS;
        }
    }
    STREAMING = false;
    t1.join();
    t3.join();
    t5.join();
    t7.join();

    auto stop = std::chrono::system_clock::now();
    std::cout << "Parallel took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
              << " ms\n";

//    std::thread t2(writefiles);
////    std::thread t4(writefiles);
//    STREAMING = true;
//    auto start1 = std::chrono::system_clock::now();
//    for(int c =0; c < 25; c++){
//        cout << c << endl;
//        for (auto i = 0; i < NUM_IMAGES; i++) {
//        // The size of gray should be defined before you can pass it into a thread
//        blur_write(inputNames[i % 8], serialNames[i % 8], gray);
//        }
//    }
//
//    STREAMING = false;
//    t2.join();
////    t4.join();
//
//    auto stop1 = std::chrono::system_clock::now();
//    std::cout << "Serial took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1).count()
//              << " ms\n";

    return 0;

    // parallel is 4.34 times faster than serial for 8 threads on 8 core machine
    // For lighter operations executed in parallel, this value may decrease
    // For all the operations demoed here, the speedup is at least 4 times.
}