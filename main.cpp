#include <opencv2/highgui.hpp>
#include <map>
#include <exiv2/exiv2.hpp>
#include <iomanip>
#include <cassert>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <iterator>
#include <tiffio.h>
#include <sqlite3.h>
#include <string>
#include <cmath>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <typeinfo>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <string>
#include <thread>
#include <stdio.h>
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace cv;
using namespace std;

// Initialize a matrix filled with zeros
//    cv::Mat image = cv::Mat::zeros(window_height, window_width, CV_8UC3);


// Suppose there are twenty images.
// What is faster ?
// Doing 5 images parallely or doing serially.

void prints(int hours){
    cout<<"Doing nothing for "<<hours<<endl;
}

int main( int argc, char** argv )
{
//    char window_name[] = "Image";
//    auto imagename = "rj.png";
//
//    Mat image, grayscale, grayscale1, grayscale2, grayscale3, grayscale4, grayscale5, grayscale6;
//
//    image = imread(imagename, IMREAD_COLOR);
//    // Cloning means making a separate copy not just copying the reference address
//    Mat image1 = image.clone();
//    Mat image2 = image.clone();
//    Mat image3 = image.clone();
//    Mat image4 = image.clone();
//    Mat image5 = image.clone();
//
//    int rows = image.rows;
//    int cols = image.cols;
//    Mat gray = cv::Mat::zeros(rows, cols, CV_8UC3);
//
////    // x,y, width, height
////    Rect r1 = Rect(0, 0, cols, rows/2);
////    Rect r2 = Rect(0, rows/2, cols, rows/2);
//
//    cvtColor(image, grayscale1, COLOR_RGB2GRAY, 0);
//    cvtColor(image, grayscale2, COLOR_RGB2GRAY, 0);
//    cvtColor(image, grayscale3, COLOR_RGB2GRAY, 0);
//    cvtColor(image, grayscale4, COLOR_RGB2GRAY, 0);
//    cvtColor(image, grayscale5, COLOR_RGB2GRAY, 0);
//
//    GaussianBlur(image, grayscale, Size (5,5), 1.0, 1.0, 0);
//
//    int N = 10;
//    // It takes 1 ms to create and destroy 10 threads
//    auto start = std::chrono::system_clock::now();
//    for(auto i = 0; i<N; i++){
////        // The size of grayscale should be defined before you can pass it into a thread
//        thread t1(GaussianBlur, image, grayscale, Size (5,5), 1.0, 1.0, 0);
////        thread t2(GaussianBlur, image, grayscale, Size (5,5), 1.0, 1.0, 0);
////        thread t3(GaussianBlur, image3, grayscale3, Size (5,5), 1.0, 1.0, 0);
////        thread t4(GaussianBlur, image4, grayscale4, Size (5,5), 1.0, 1.0, 0);
////        thread t5(GaussianBlur, image5, grayscale5, Size (5,5), 1.0, 1.0, 0);
////
//        t1.join();
//////        t2.join();
//////        t3.join();
//////        t4.join();
//////        t5.join();
//    }
//    auto stop = std::chrono::system_clock::now();
//    std::cout<<"Parallel took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " ms\n";
//
//    auto start1 = std::chrono::system_clock::now();
//    for(auto i = 0; i<N; i++){
//        // The size of grayscale should be defined before you can pass it into a thread
////        cvtColor(image, grayscale, COLOR_RGB2GRAY);
//        GaussianBlur(image, grayscale, Size (5,5), 1.0, 1.0, 0);
//    }
//    auto stop1 = std::chrono::system_clock::now();
//    std::cout<<"Serial took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1).count() << " ms\n";
//    /// show it in a window during DELAY ms
//    //  cv::imshow(window_name, grayscale(r2));
//    //  cv::waitKey(0); // wait for a key press



    return 0;

}