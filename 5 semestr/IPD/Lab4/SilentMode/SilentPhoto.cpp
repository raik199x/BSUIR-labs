#include <opencv4/opencv2/opencv.hpp>
#include <unistd.h>

using namespace cv;

int main(){
    if (fork() != 0){
        return 0;
    }
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        return -1;
    }
    // image capture
    Mat img;
    cap >> img;
    imwrite("me.png",img);
    cap.release();
    return 0;
}