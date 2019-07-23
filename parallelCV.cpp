#include <opencv2/highgui.hpp>
#include <iomanip>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/core/ocl.hpp>

#include <iostream>
#include <string>
#include <thread>
#include <stdio.h>
#include <vector>
#include <queue>

//using namespace cv;
//using namespace std;

class ParallelOpenCV: public cv::ParallelLoopBody{
private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    int diff;
public:
    ParallelOpenCV(cv::Mat inputImage, cv::Mat &outputImage, int diff):
    inputImage(inputImage), outputImage(outputImage), diff(diff){}

    virtual void operator()(const cv::Range& range) const{
        int rows = inputImage.rows;
        int cols = inputImage.cols;

        cv::Rect r1 = cv::Rect(0, 0, cols / 2, rows / 2);
        cv::Rect r2 = cv::Rect(cols / 2, 0, cols / 2, rows / 2);
        cv::Rect r3 = cv::Rect(0, rows / 2, cols / 2, rows / 2);
        cv::Rect r4 = cv::Rect(cols / 2, rows / 2, cols / 2, rows / 2);
        std::vector <cv::Rect> rois{r1, r2, r3, r4};

        for(int i = range.start; i < range.end; i++){
            // Not all operations are suitable for split and merge technique.
            // Like those that change the size of output or number of channels or other things that require global context like normalization.
            // multiply.
            // resize and rotate will require smart merging.

            // Divide image in diff number of parts and process simultaneously
            int j = 3 - i;
            cv::Mat in(inputImage, rois[i]);
            cv::Mat out(outputImage, rois[3 - i]);

            cv::rotate(in, out, 1); // Fine
//            cv::namedWindow("ROI");
//            cv::imshow("ROI", outputImage);
//            cv::waitKey(100);
        }
    }
};

int main(){
    std::string fname = "quote.jpg";
    cv::Mat image = cv::imread(fname);
    cv::Mat out = image.clone();

    // create 8 threads and use TBB
    const int num_threads = 4;
    cv::parallel_for_(cv::Range(0, num_threads), ParallelOpenCV(image, out, num_threads));
    cv::imwrite("Out.jpg", out);
}