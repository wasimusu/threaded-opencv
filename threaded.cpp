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

void blur_write(string input_fname, string output_fname, Mat output){
    GaussianBlur(imread(input_fname), output, Size (5,5), 1.0, 1.0, 0);
    imwrite(output_fname, output);
}

int main( int argc, char** argv )
{
    string names[] = {"rj.png", "rj2.png", "rj3.png", "rj4.png", "rj5.png"};
    string gnames[] = {"grj.png", "grj2.png", "grj3.png", "grj4.png", "grj5.png"};

    Mat image1, image2, image3, image4, image5;
    Mat gray1, gray2, gray3, gray4, gray5;
    gray1 = cv::Mat::zeros(image1.rows, image1.cols, CV_8UC3);

    image1 = imread(names[0], IMREAD_COLOR);

    // Creating placeholders for grayscaled images
    GaussianBlur(image1, gray1, Size (5,5), 1.0, 1.0, 0);
    cout<<"Blurred image channels : "<< gray1.channels()<<endl;

    cout << "Filename : "<< names[0] << endl;
    int N = 100; // Multiple of 5
    // It takes 1 ms to create and destroy 10 threads
    auto start = std::chrono::system_clock::now();
    for(auto i = 0; i<N/5; i++){
//        // The size of gray should be defined before you can pass it into a thread
        thread t1(blur_write, names[0], gnames[0], gray1);
        thread t2(blur_write, names[1], gnames[1], gray1);
        thread t3(blur_write, names[2], gnames[2], gray1);
        thread t4(blur_write, names[3], gnames[3], gray1);
        thread t5(blur_write, names[4], gnames[4], gray1);
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
    }
    auto stop = std::chrono::system_clock::now();
    std::cout<<"Parallel took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " ms\n";

    auto start1 = std::chrono::system_clock::now();
    for(auto i = 0; i<N; i++){
        // The size of gray should be defined before you can pass it into a thread
        blur_write(names[i%5], gnames[i%5], gray1);
    }
    auto stop1 = std::chrono::system_clock::now();
    std::cout<<"Serial took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1).count() << " ms\n";

    return 0;

}