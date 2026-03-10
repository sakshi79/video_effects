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
   
    cv::Mat sepia_frame;
    cv::Mat sobelX16, sobelXvis;
    cv::Mat sobelY16, sobelYvis;
    cv::Mat magnitudeVis;
    cv::Mat blurQuantVis;
    cv::Mat faceDetectVis;
    cv::Mat embosVis;
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
    std::cout << "Press 'h' to toggle custom greyscale. \n";
    std::cout << "Press 'p' to toggle sepia filter. \n";
    std::cout << "Press 'b' for 5x5 Gaussian blur filter. \n";
    std::cout << "Press 'x' for SobelX filter. \n";
    std::cout << "Press 'y' for SobelY filter. \n";
    std::cout << "Press 's' to save the current frame\n";

    while (true)
    {
        cap >> frame;   
        if(frame.empty())
            break;

        //cv::resize(frame, frame, cv::Size(, 1000));
        out_frame = frame.clone();

        //cv::imshow("Video Display Window", out_frame);

        char k = (char)cv::waitKey(30);
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
            // sobelX3x3(out_frame, sobelX16);
            // cv::convertScaleAbs(sobelX16, sobelXvis);
            // cv::imshow("Sobel X", sobelXvis);
        }
        if(k=='y')
        {
            sobelY=!sobelY;
            // sobelY3x3(out_frame, sobelY16);
            // cv::convertScaleAbs(sobelY16, sobelYvis);
            // cv::imshow("Sobel Y", sobelYvis);
        }
        if(k=='m')
        {
            grad_mag = !grad_mag;
            // sobelX3x3(out_frame, sobelX16);
            // sobelY3x3(out_frame, sobelY16);
            // magnitude(sobelX16, sobelY16, magnitudeVis);
            // cv::imshow("Gradient Magnitude", magnitudeVis);
        }
        if(k=='l')
        {
            blurQ = !blurQ;
            // blurQuantize(out_frame, blurQuantVis, 10);
            // cv::imshow("Blur and Quantize", blurQuantVis);
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
            if(gray)
            {
                std::cout << "Saved frame as output_grayframe.png\n";
            }
            if(sepia)
            {
                Sepia(out_frame, sepia_frame);
            }
            else
            {
                std::cout << "Saved frame as output_frame.png\n";
            }
        }

        if(gray && out_frame.channels() == 3)
        {
            out_frame = toGrayscale(out_frame);
            cv::imshow("Video Streaming", out_frame);
        }        
        if(sepia)
        {
            Sepia(out_frame, out_frame);
            cv::imshow("Video Streaming", out_frame);
        }
        if(blur)
        {
            blur5x5_2(frame, out_frame);
            cv::imshow("Video Streaming", out_frame);

        }
        if(sobelX)
        {
            sobelX3x3(out_frame, sobelX16);
            cv::convertScaleAbs(sobelX16, out_frame);
            cv::imshow("Video Streaming", out_frame);
        }
        if(sobelY)
        {
            sobelX3x3(out_frame, sobelY16);
            cv::convertScaleAbs(sobelY16, out_frame);
            cv::imshow("Video Streaming", out_frame);
        }
        if(grad_mag)
        {
            sobelX3x3(out_frame, sobelX16);
            sobelY3x3(out_frame, sobelY16);
            magnitude(sobelX16, sobelY16, out_frame);
            cv::imshow("Video Streaming", out_frame);
        }
        if(blurQ)
        {
            blurQuantize(out_frame, out_frame, 10);
            cv::imshow("Video Streaming", out_frame);
        }
        if(faceDetect)
        {
            // convert to grayscale
            cv::cvtColor(out_frame, gray_frame, cv::COLOR_BGR2GRAY);
            // detect faces
            faces.clear();
            detectFaces(gray_frame, faces);

            // draw boxes
            drawBoxes(out_frame, faces);
            cv::imshow("Video Streaming", out_frame);
        }
        if(neg)
        {
            negative(out_frame, out_frame);
            cv::imshow("Video Streaming", out_frame);
           
        }
        if(embs)
        {
            emboss(out_frame, out_frame);
            cv::imshow("Video Streaming", out_frame);
        }
        if(blurRest)
        {
            cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);
            detectFaces(gray_frame, faces);
            blurOutsideFaces(frame, out_frame, faces);
            cv::imshow("Video Streaming", out_frame);
            
        }
        if(cFace)
        {
            cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);
            detectFaces(gray_frame, faces);
            colorFace(frame, out_frame, faces);
            cv::imshow("Video Streaming", out_frame);
        }
        if(halo)
        {
            // convert to grayscale
            cv::cvtColor(out_frame, gray_frame, cv::COLOR_BGR2GRAY);
            // detect faces
            faces.clear();
            detectFaces(gray_frame, faces);
            haloSparkles(frame, out_frame, faces);
            cv::imshow("Video Streaming", out_frame);
        }
        else
        {
            cv::namedWindow("Video Streaming", cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO);
            cv::resizeWindow("Output", 1280, 720);
            cv::imshow("Video Streaming", out_frame);
        }
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
