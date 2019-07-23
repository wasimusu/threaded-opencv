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
        for(int i = range.start; i < range.end; i++){
            // Not all operations are suitable for split and merge technique.
            // Like those that change the size of output or number of channels or other things that require global context like normalization.
            // multiply.
            // resize and rotate will require smart merging.

            // Divide image in diff number of parts and process simultaneously
            cv::Mat in(outputImage, cv::Rect(0, (outputImage.rows/diff)*i, outputImage.cols, outputImage.rows/diff));
            cv::Mat out(outputImage, cv::Rect(0, (outputImage.rows/diff)*i, outputImage.cols, outputImage.rows/diff));

            std::cout<<"Thread "<<i <<std::endl;
            out.convertTo(out, CV_8UC3, 0.10);
            cv::GaussianBlur(in, out, cv::Size(175, 175), 1.0, 1.0, 0); // Fine
            cv::rotate(out, out, 1); // Fine
            std::cout<<"Join "<<i<<std::endl;
        }
    }
};

int main(){
    std::string fname = "RGB_ref.jpg";
    cv::Mat image = cv::imread(fname);
    cv::Mat out = image.clone();

    // create 8 threads and use TBB
    cv::parallel_for_(cv::Range(0, 8), ParallelOpenCV(image, out, 8));
    cv::imwrite("Out.jpg", out);
}