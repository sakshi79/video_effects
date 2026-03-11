// Sakshi Bhatia

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <sys/time.h>
#include "filter.h"
#include "faceDetect/faceDetect.h"

using namespace cv;
using namespace std;

// returns a double which gives time in seconds
double getTime() {
  struct timeval cur;

  gettimeofday( &cur, NULL );
  return( cur.tv_sec + cur.tv_usec / 1000000.0 );
}

int main()
{
    // std::string video_path = cv::samples::findFile("../videoplayback.mp4");
    // //std::string video_path = cv::samples::findFile("../kitchen_video.mp4");
    // cv::VideoCapture cap(video_path);

    // if (!cap.isOpened())
    // {
    //     std::cout << "Could not open the video: " << video_path << std::endl;
    //     return 1;
    // }

    VideoCapture cap(0);


    // Defining required frame variables
    cv::Mat frame;
    cv::Mat out_frame;
    cv::Mat sobelX16;
    cv::Mat sobelY16;
    cv::Mat gray_frame;
    std::vector<cv::Rect> faces;

    bool gray=false;  // cv2 COLOR_BGR2GRAY grayframe
    bool sepia = false;
    bool blur = false;
    bool faceDetect = false;
    bool neg = false;
    bool embs = false;
    bool blurRest = false;
    bool cFace = false;
    bool halo = false;
    bool sobelX = false;
    bool sobelY = false;
    bool grad_mag = false;
    bool blurQ = false;

    std::cout << "Press 'q' to quit\n";
    std::cout << "Press 'g' to toggle grayscale. \n";
    std::cout << "Press 'p' to toggle sepia filter. \n";
    std::cout << "Press 'b' for 5x5 Gaussian blur filter. \n";
    std::cout << "Press 'x' for SobelX filter. \n";
    std::cout << "Press 'y' for SobelY filter. \n";
    std::cout << "Press 's' to save the current frame\n";


    // Creating Display Window
    cv::namedWindow("Video Streaming", cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO);
    cv::resizeWindow("Video Streaming", 1280, 720);

    while (true)
    {
        cap >> frame;   
        if(frame.empty())
            break;

        out_frame = frame.clone();

        char k = (char)cv::waitKey(5);
        if(k == 'q')
        {
            break;
        }
        if (k == 'g')
        {
            gray = !gray;
        }
        if(k=='p')
        {
            sepia = !sepia;
            //gray = false;
        }
        if(k=='b')
        {
            blur = !blur;
            // const int Ntimes = 10;
            // double startTime = getTime();
            // // execute the file on the original image a couple of times
            // for(int i=0; i<Ntimes; i++)
            // {
            //     blur5x5_1(frame, out_frame);
            // }
            // double endTime = getTime();
            // //compute time per image
            // double difference = (endTime - startTime) / Ntimes;
            // printf("Time per image (1): %.4lf seconds\n", difference);

            // // set up the timing for version 2
            // startTime = getTime();
            // for(int i=0;i<Ntimes;i++) {
            //     blur5x5_2(frame, out_frame);
            // }
            // endTime = getTime();
            // difference = (endTime - startTime) / Ntimes;
            // printf("Time per image (2): %.4lf seconds\n", difference );
        }
        if(k=='x')
        {
            sobelX=!sobelX;
        }
        if(k=='y')
        {
            sobelY=!sobelY;
        }
        if(k=='m')
        {
            grad_mag = !grad_mag;
        }
        if(k=='l')
        {
            blurQ = !blurQ;
        }
        if(k=='f')
        {
            faceDetect = !faceDetect;
        }
        if(k=='n')
        {
            neg = !neg;
        }
        if(k=='e')
        {
            embs = !embs; 
        }
        if(k=='r')
        {
            //blur rest image except face
            blurRest = !blurRest;
        }
        if(k=='c')
        {
            cFace = !cFace;
        }
        if(k=='w')
        {
            //sparkles halo
            halo = !halo;
        }

        if(k == 's')
        {
            cv::imwrite("current_frame.png", out_frame);
        }


        bool needFaces = faceDetect || blurRest || cFace || halo;
        if(needFaces)
        {
            faces.clear();
            cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);
            detectFaces(gray_frame, faces);
        }

        if(gray && out_frame.channels() == 3)
        {
            out_frame = toGrayscale(out_frame);
        }        
        if(sepia)
        {
            Sepia(out_frame, out_frame);
        }
        if(blur)
        {
            blur5x5_2(out_frame, out_frame);

        }
        if(sobelX)
        {
            sobelX3x3(out_frame, sobelX16);
            cv::convertScaleAbs(sobelX16, out_frame);
        }
        if(sobelY)
        {
            sobelY3x3(out_frame, sobelY16);
            cv::convertScaleAbs(sobelY16, out_frame);
        }
        if(grad_mag)
        {
            sobelX3x3(out_frame, sobelX16);
            sobelY3x3(out_frame, sobelY16);
            magnitude(sobelX16, sobelY16, out_frame);
        }
        if(blurQ)
        {
            blurQuantize(out_frame, out_frame, 10);
        }
        if(faceDetect)
        {
            // draw boxes
            drawBoxes(out_frame, faces);
            // cv::imshow("Video Streaming", out_frame);
        }
        if(neg)
        {
            negative(out_frame, out_frame);
            // cv::imshow("Video Streaming", out_frame);
           
        }
        if(embs)
        {
            emboss(out_frame, out_frame);
            // cv::imshow("Video Streaming", out_frame);
        }
        if(blurRest)
        {
            blurOutsideFaces(out_frame, out_frame, faces);
            // cv::imshow("Video Streaming", out_frame);
            
        }
        if(cFace)
        {
            colorFace(out_frame, out_frame, faces);
            // cv::imshow("Video Streaming", out_frame);
        }
        if(halo)
        {
            haloSparkles(out_frame, out_frame, faces);
            // cv::imshow("Video Streaming", out_frame);
        }
        cv::imshow("Video Streaming", out_frame);
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
