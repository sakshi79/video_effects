// Sakshi Bhatia

#include "filter.h"
#include <algorithm>
#include <cmath>
#include <algorithm>

cv::Mat toGrayscale(const cv::Mat& input)
{
    //helper function to convert a frame to grayscale
    cv::Mat gray;
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    return gray;
}

int GreyScale(const cv::Mat& src, cv::Mat& dst)
{
    if(src.empty() || src.type() != CV_8UC3)
    {
        return -1;
    }

    dst.create(src.rows, src.cols, src.type());

    for(int y=0; y<src.rows; y++)
    {
        const cv::Vec3b* srcRow = src.ptr<cv::Vec3b>(y);
        cv::Vec3b* dstRow = dst.ptr<cv::Vec3b>(y);

        for(int x=0; x<src.cols; x++)
        {
            const cv::Vec3b& p = srcRow[x];
            float gray = 0.6 *(255.0f - p[2]) + 0.2*p[1] + 0.2*p[0];
            uint8_t g = static_cast<uint8_t>(std::min(255.0f, std::max(0.0f, gray)));

            dstRow[x] = cv::Vec3b(g,g,g);
        }
    }
    return 0;
}


int Sepia(const cv::Mat& src, cv::Mat& dst, float strength)
{
    if(src.empty() || src.type() != CV_8UC3)
    {
        return -1;
    }

    dst.create(src.rows, src.cols, src.type());

    int rows = src.rows;
    int cols = src.cols;
    // computing frame center
    float cx = cols/2.0f;
    float cy = rows/2.0f;
    float maxDist = std::sqrt(cx*cx + cy*cy);
    

    for(int y=0; y<rows; y++)
    {
        const cv::Vec3b* srcRow = src.ptr<cv::Vec3b>(y);
        cv::Vec3b* dstRow = dst.ptr<cv::Vec3b>(y);

        for(int x=0; x<cols; x++)
        {

            float dx = x-cx;
            float dy = y-cy;
            float dist = std::sqrt(dx*dx + dy*dy);
            float vignette = 1.0f - strength * (dist/maxDist) * (dist/maxDist);
            vignette = std::max(0.0f, vignette);

            // BGR order in OpenCV
            float B = srcRow[x][0];
            float G = srcRow[x][1];
            float R = srcRow[x][2];

            float newR = 0.393f * R + 0.769f * G + 0.189f * B;
            float newG = 0.349f * R + 0.686f * G + 0.168f * B;
            float newB = 0.272f * R + 0.534f * G + 0.131f * B;
            newR *= vignette;
            newG *= vignette;
            newB *= vignette;

            dstRow[x][2] = static_cast<uint8_t>(std::min(255.0f, std::max(0.0f, newR)));
            dstRow[x][1] = static_cast<uint8_t>(std::min(255.0f, std::max(0.0f, newG)));
            dstRow[x][0] = static_cast<uint8_t>(std::min(255.0f, std::max(0.0f, newB)));
                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
        }
    }
    return 0;
}


int blur5x5_1( cv::Mat &src, cv::Mat &dst)
{
    if(src.empty() || src.type() != CV_8UC3)
    {
        return -1;
    }

    dst = src.clone();
    int kernel[5][5] = {
        {1, 2, 4, 2, 1},
        {2, 4, 8, 4, 2},
        {4, 8, 16, 8, 4},
        {2, 4, 8, 4, 2},
        {1, 2, 4, 2, 1}
    };

    const int kernelSum = 100;
    
    for(int y=2; y < src.rows-2; y++)
    {
        for(int x=2; x<src.cols-2; x++)
        {
            int sumB = 0, sumG = 0, sumR = 0;
            for(int ky=-2; ky<=2; ky++)
            {
                for(int kx=-2; kx<=2; kx++)
                {
                    cv::Vec3b pixel = src.at<cv::Vec3b>(y+ky, x+kx);
                    int weight = kernel[ky+2][kx+2];
                    sumB += weight*pixel[0];
                    sumG += weight*pixel[1];
                    sumR += weight*pixel[2];
                }
            }
            dst.at<cv::Vec3b>(y,x)[0] = static_cast<uchar>(sumB/kernelSum);
            dst.at<cv::Vec3b>(y,x)[1] = static_cast<uchar>(sumG/kernelSum);
            dst.at<cv::Vec3b>(y,x)[2] = static_cast<uchar>(sumR/kernelSum);
        }
    }
    return 0;
}


int blur5x5_2( cv::Mat &src, cv::Mat &dst)
{
    if(src.empty() || src.type() != CV_8UC3)
    {
        return -1;
    }

    dst = src.clone();
    cv::Mat temp = src.clone();

    int rows = src.rows;
    int cols = src.cols;

    // Horizontal Pass
    for(int y=2; y<rows-2; y++)
    {
        const cv::Vec3b* srcRow = src.ptr<cv::Vec3b>(y);
        cv::Vec3b* tmpRow = temp.ptr<cv::Vec3b>(y);

        for(int x=2; x<cols-2; x++)
        {
            for(int c=0; c<3; c++)
            {
                tmpRow[x][c] = (
                    srcRow[x-2][c] +
                    2* srcRow[x-1][c] +
                    4* srcRow[x][c] +
                    2* srcRow[x+1][c] +
                    srcRow[x+2][c]
                ) / 10;
            }
        }
    }

    // Vertical pass
    for(int y=2; y<rows-2; y++)
    {
        cv::Vec3b* dstRow = dst.ptr<cv::Vec3b>(y);
        const cv::Vec3b* row_m2 = temp.ptr<cv::Vec3b>(y - 2);
        const cv::Vec3b* row_m1 = temp.ptr<cv::Vec3b>(y - 1);
        const cv::Vec3b* row_0  = temp.ptr<cv::Vec3b>(y);
        const cv::Vec3b* row_p1 = temp.ptr<cv::Vec3b>(y + 1);
        const cv::Vec3b* row_p2 = temp.ptr<cv::Vec3b>(y + 2);

        for(int x=2; x<cols-2; x++)
        {
            for(int c=0; c<3; c++)
            {
                dstRow[x][c] = (
                    row_m2[x][c] +
                    2* row_m1[x][c] +
                    4* row_0[x][c] +
                    2* row_p1[x][c] +
                    row_p2[x][c]
                ) /10;
            }
        }
    }
    return 0;
}

int blurQuantize(cv::Mat &src, cv::Mat &dst, int levels)
{
    if (src.empty() || src.type() != CV_8UC3 || levels < 2)
        return -1;

    cv::Mat blurred;
    blur5x5_2(src, blurred);  

    dst = blurred.clone();  
    int rows = blurred.rows;
    int cols = blurred.cols;

    // compute bucket size
    float b = 255.0f / levels;
    for (int y = 0; y < rows; y++)
    {
        cv::Vec3b* row = dst.ptr<cv::Vec3b>(y);
        for (int x = 0; x < cols; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                // quantize
                int xt = static_cast<int>(row[x][c] / b);
                row[x][c] = static_cast<uchar>(xt * b);
            }
        }
    }

    return 0;
}




int sobelX3x3(cv::Mat &src, cv::Mat &dst)
{
    if (src.empty() || src.type() != CV_8UC3)
        return -1;

    dst = cv::Mat::zeros(src.size(), CV_16SC3); // signed short output

    int rows = src.rows;
    int cols = src.cols;

    for (int y = 1; y < rows - 1; y++)
    {
        const cv::Vec3b* row_m1 = src.ptr<cv::Vec3b>(y - 1);
        const cv::Vec3b* row_0  = src.ptr<cv::Vec3b>(y);
        const cv::Vec3b* row_p1 = src.ptr<cv::Vec3b>(y + 1);

        cv::Vec3s* dstRow = dst.ptr<cv::Vec3s>(y);

        for (int x = 1; x < cols - 1; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                int val =
                    row_m1[x-1][c] * -1 + row_m1[x+1][c] * 1 +
                    row_0[x-1][c]  * -2 + row_0[x+1][c]  * 2 +
                    row_p1[x-1][c] * -1 + row_p1[x+1][c] * 1;

                dstRow[x][c] = static_cast<short>(val);
            }
        }
    }
    return 0;
}


int sobelY3x3(cv::Mat &src, cv::Mat &dst)
{
    if (src.empty() || src.type() != CV_8UC3)
        return -1;

    dst = cv::Mat::zeros(src.size(), CV_16SC3); // signed short output

    int rows = src.rows;
    int cols = src.cols;

    for (int y = 1; y < rows - 1; y++)
    {
        const cv::Vec3b* row_m1 = src.ptr<cv::Vec3b>(y - 1);
        const cv::Vec3b* row_0  = src.ptr<cv::Vec3b>(y);
        const cv::Vec3b* row_p1 = src.ptr<cv::Vec3b>(y + 1);

        cv::Vec3s* dstRow = dst.ptr<cv::Vec3s>(y);

        for (int x = 1; x < cols - 1; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                int val =
                    row_m1[x-1][c] * -1 + row_m1[x][c] * -2 + row_m1[x+1][c] * -1 +
                    row_p1[x-1][c] * 1  + row_p1[x][c] *  2 + row_p1[x+1][c] * 1;

                dstRow[x][c] = static_cast<short>(val);
            }
        }
    }

    return 0;
}

int magnitude(cv::Mat &sx, cv::Mat &sy, cv::Mat &dst)
{
    if (sx.empty() || sy.empty() || sx.type() != CV_16SC3 || sy.type() != CV_16SC3)
        return -1;
    if (sx.size() != sy.size())
        return -1;

    dst = cv::Mat::zeros(sx.size(), CV_8UC3);
    int rows = sx.rows;
    int cols = sx.cols;

    for (int y = 0; y < rows; y++)
    {
        const cv::Vec3s* sxRow = sx.ptr<cv::Vec3s>(y);
        const cv::Vec3s* syRow = sy.ptr<cv::Vec3s>(y);
        cv::Vec3b* dstRow = dst.ptr<cv::Vec3b>(y);

        for (int x = 0; x < cols; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                int val = static_cast<int>( std::sqrt( sxRow[x][c] * sxRow[x][c] +
                                                      syRow[x][c] * syRow[x][c] ) );
                // clamp to 0-255
                dstRow[x][c] = static_cast<uchar>( std::min(255, val) );
            }
        }
    }
    return 0;
}


int blurOutsideFaces(cv::Mat &src, cv::Mat &dst, std::vector<cv::Rect> &faces)
{
    if(src.empty() || src.type() != CV_8UC3)
        return -1;

        // Create a blured image
        cv::Mat blurred;
        blur5x5_2(src, blurred);
        dst = blurred.clone();

        for(const auto &face: faces)
        {
            // Copy original pixels back in face, leaving rest image blurred
            cv::Rect roi = face &cv::Rect(0, 0, src.cols, src.rows);
            src(roi).copyTo(dst(roi));
        }
        return 0;

}


int emboss(cv::Mat &src, cv::Mat &dst)
{
    cv::Mat sx, sy;
    sobelX3x3(src, sx);
    sobelY3x3(src, sy);
    dst.create(src.size(), CV_8UC3);

    const float lx=0.7071f;
    const float ly=0.7071f;

    for(int y=0; y<src.rows; y++)
    {
        for(int x=0; x<src.cols; x++)
        {
            //Using at since ptr is slower here 
            cv::Vec3s gx = sx.at<cv::Vec3s>(y,x);
            cv::Vec3s gy = sy.at<cv::Vec3s>(y,x);
            cv::Vec3b &out = dst.at<cv::Vec3b>(y,x);

            for(int c=0; c<3; c++)
            {
                int val = gx[c] * lx + gy[c] *ly;
                val += 128;
                out[c] = cv::saturate_cast<uchar>(val);
            }
        }
    }
    return 0;
}



int grayscale3(cv::Mat &src, cv::Mat &dst)
{
    if(src.empty() || src.type() != CV_8UC3)
        return -1;
    dst.create(src.size(), CV_8UC3);

    for(int y=0; y<src.rows; y++)
    {
        const cv::Vec3b* srcRow = src.ptr<cv::Vec3b>(y);
        cv::Vec3b* dstRow = dst.ptr<cv::Vec3b>(y);

        for(int x = 0; x < src.cols; x++)
        {
            uchar g = static_cast<uchar>(
                0.299f * srcRow[x][2] +
                0.587f * srcRow[x][1] +
                0.114f * srcRow[x][0]
            );
            dstRow[x] = cv::Vec3b(g, g, g);
        }
    }
    return 0;
}

int colorFace(cv::Mat &src, cv::Mat &dst, std::vector<cv::Rect> &faces)
{
    if(src.empty() || src.type() != CV_8UC3)
        return -1;

    // Step 1: make whole image greyscale
    grayscale3(src, dst);

    // Step 2: copy color faces back
    for(const auto &face : faces)
    {
        int pad = 10; // helps include hair/edges

        cv::Rect expanded(
            face.x - pad,
            face.y - pad,
            face.width + 2*pad,
            face.height + 2*pad
        );

        cv::Rect roi = expanded & cv::Rect(0, 0, src.cols, src.rows);
        src(roi).copyTo(dst(roi));
    }

    return 0;
}

int negative(cv::Mat &src, cv::Mat &dst)
{
    if (src.empty() || src.type() != CV_8UC3)
        return -1;

    dst = src.clone();

    for (int y = 0; y < src.rows; y++)
    {
        cv::Vec3b *row = dst.ptr<cv::Vec3b>(y);
        for (int x = 0; x < src.cols; x++)
        {
            row[x][0] = 255 - row[x][0];
            row[x][1] = 255 - row[x][1];
            row[x][2] = 255 - row[x][2];
        }
    }
    return 0;
}

int haloSparkles(cv::Mat &src, cv::Mat &dst, std::vector<cv::Rect> &faces)
{
    if(src.empty() || src.type() != CV_8UC3)
        return -1;

    dst = src.clone();

    static int frameCount = 0;
    frameCount++;

    for(const auto &face : faces)
    {
        // Halo parameters
        cv::Point center(
            face.x + face.width / 2,
            face.y - face.height / 6
        );

        int radius = face.width / 2;
        int sparkleCount = 20;

        // Draw sparkles on a semicircle
        for(int i = 0; i < sparkleCount; i++)
        {
            float theta =
                CV_PI * (float(i) / sparkleCount) +
                0.1f * std::sin(0.1f * frameCount + i);

            int x = int(center.x + radius * std::cos(theta));
            int y = int(center.y - radius * std::sin(theta));

            if(x < 0 || x >= src.cols || y < 0 || y >= src.rows)
                continue;

            // Sparkle appearance
            cv::Scalar color(255, 255, 200); // warm sparkle
            int size = 2 + (i % 3);

            cv::circle(dst, cv::Point(x, y), size, color, -1);
        }
    }

    return 0;
}






