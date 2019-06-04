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

void split_merge(Mat input, Mat output){
    int rows = input.rows;
    int cols = input.cols;

    Rect r1 = Rect(0, 0, cols/2, rows/2);
    Rect r2 = Rect(cols/2, 0, cols/2, rows/2);
    Rect r3 = Rect(0, rows/2, cols/2, rows/2);
    Rect r4 = Rect(cols/2, rows/2, cols/2, rows/2);

    thread t1(GaussianBlur, input(r1), output(r1), Size (5,5), 1.0, 1.0, 0);
    thread t2(GaussianBlur, input(r2), output(r2), Size (5,5), 1.0, 1.0, 0);
    thread t3(GaussianBlur, input(r3), output(r3), Size (5,5), 1.0, 1.0, 0);
    thread t4(GaussianBlur, input(r4), output(r4), Size (5,5), 1.0, 1.0, 0);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

int main( int argc, char** argv )
{
    string names[] = {"rj.png", "rj2.png", "rj3.png", "rj4.png", "rj5.png"};
    string gnames[] = {"grj.png", "grj2.png", "grj3.png", "grj4.png", "grj5.png"};

    Mat image1, image2, image3, image4, image5;
    image1 = imread(names[0], IMREAD_COLOR);

    // Creating placeholders for grayscaled images
    Mat gray1 = cv::Mat::zeros(image1.rows, image1.cols, CV_8UC3);
    cout<<"Blurred image channels : "<< gray1.channels()<<endl;

    cout << "Filename : "<< names[0] << endl;
    int N = 5; // Multiple of 5
    for(int i=0; i<N; i++){
        // The number of cores is the optimal number of threads
        // It takes 1 ms to create and destroy 10 threads

        auto start1 = std::chrono::system_clock::now();
        GaussianBlur(image1, gray1, Size (5,5), 1.0, 1.0, 0);
        auto stop1 = std::chrono::system_clock::now();
        std::cout<<"Serial took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1).count() << " ms\n";

        auto start = std::chrono::system_clock::now();
        split_merge(image1, gray1);
        auto stop = std::chrono::system_clock::now();
        std::cout<<"Parallel took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " ms\n";
    }
    return 0;

    // Serial is faster than parallel
}