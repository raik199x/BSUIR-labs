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

    // saving frame to get webcam resolution
    Mat frame;
    cap >> frame;
    // NAME , VIDEO CODEC , FPS , SIZE
    VideoWriter video("mov3.avi",cv::VideoWriter::fourcc('M','J','P','G'),30, frame.size());
    if(!video.isOpened()){
        return -1;
    }
    int i = 0;
    // video capture
    while (i != 360) {
        // capturing frame by frame
        cap >> frame;
        if (frame.empty()) break;
        video << frame;
        i++;
    }
    cap.release();
    video.release();
    return 0;
}