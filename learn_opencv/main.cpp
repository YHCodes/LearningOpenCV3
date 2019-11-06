//
//  main.cpp
//  learn_opencv
//
//  Created by lyh on 2019/10/25.
//  Copyright © 2019 viglyh. All rights reserved.
//


// https://www.jianshu.com/p/8b4a1c5cf44b  xcode 配置 Opencv

#include <iostream>
#include <opencv2/opencv.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
using namespace std;

// "g" means "global variable"
int g_slider_position = 0;    // keep the trackbar slider position state
int g_run = 1;     // which displays new frames as long it is different from zero.
int g_dontset = 0; // start out in single step mode
cv::VideoCapture g_cap;

void onTrackbarSlide(int pos, void *) {
    g_cap.set(cv::CAP_PROP_POS_FRAMES, pos);
    
    if (!g_dontset)
        g_run = 1;
    g_dontset = 0;
}

class Ch2Introduction {
public:
    // Example 2-1: Basic
    void example_2_1() {
        cv::Mat img = cv::imread(image_path_, -1);
        
        if (img.empty())
            return;
        
        cv::namedWindow("Example2-1", cv::WINDOW_AUTOSIZE);
        cv::imshow("Example2-1", img);
        cv::waitKey(0);
        cv::destroyWindow("Example2-1");
    }
    
    // Example 2-3
    void example_2_3() {
        cv::VideoCapture cap;
        cap.open(video_path_);
        cv::Mat frame;
        cv::namedWindow("Example2-3", cv::WINDOW_AUTOSIZE);
        for (;;) {
            // the video file is read frame by frame the capture object stream.
            cap >> frame;
            if (frame.empty())
                break;
            cv::imshow("Example2-3", frame);
            
            // wait 33 ms, if the user hits a key during that time, exit the read loop
            if (cv::waitKey(33) >= 0)
                break;
        }
    }
    
    // Example 2-4: trackbar slider
    void example_2_4() {
        std::string fpath = "images/test.avi";
        cv::namedWindow("Example2-4", cv::WINDOW_AUTOSIZE);
        g_cap.open(fpath);
        
        int frame_count  = (int) g_cap.get(cv::CAP_PROP_FRAME_COUNT);
        int frame_width  = (int) g_cap.get(cv::CAP_PROP_FRAME_WIDTH);
        int frame_height = (int) g_cap.get(cv::CAP_PROP_FRAME_HEIGHT);
        
        std::cout << "Video has " << frame_count << " frame of dimensions ("
        << frame_width << ", " << frame_height << ")." << std::endl;
        
        cv::createTrackbar("Position", "Example2-4", &g_slider_position, frame_count, onTrackbarSlide);
        
        cv::Mat frame;
        for (;;) {
            if (g_run != 0) {
                g_cap >> frame;
                if (frame.empty()) {
                    break;
                }
                int current_pos = (int) g_cap.get(cv::CAP_PROP_POS_FRAMES);
                g_dontset = 1;
                cv::setTrackbarPos("Position", "Example2-4", current_pos);
                cv::imshow("Example2-4", frame);
                g_run -= 1;
            }
            
            char c = (char) cv::waitKey(10);
            
            if (c == 's') { // single step
                g_run = 1; // A positive number indicates how many frames are displayed before stopping
                cout << "single step, run = " << g_run << endl;
            }
            
            if (c == 'r') { // run mode
                g_run = -1; // a negative number means the system runs in continuous video mode.
                cout << "run mode, run = " << g_run << endl;
            }
            
            if (c == 27) {
                break;
            }
        }
    }
    
    // Example 2-5 : a simple transformation
    void example_2_5() {
        const cv::Mat image = cv::imread(image_path_);
        
        cv::namedWindow("Example2-5 In", cv::WINDOW_AUTOSIZE);
        cv::namedWindow("Example2-5 Out", cv::WINDOW_AUTOSIZE);
        
        cv::imshow("Example2-5 In", image);
        
        cv::Mat out;
        
        cv::GaussianBlur(image, out, cv::Size(5, 5), 3, 3);
        cv::GaussianBlur(out, out, cv::Size(5, 5), 3, 3);
        cv::imshow("Example2-5 Out", out);
        
        cv::waitKey(0);
    }
    
    // Example 2-6: pyrDown()
    void example_2_6() {
        cv::Mat img_rgb, img_pyr, img_pyr2;
        
        img_rgb = cv::imread(image_path_);
        cv::pyrDown(img_rgb, img_pyr);
        cv::pyrDown(img_pyr, img_pyr2);
        
        cv::namedWindow("rgb", cv::WINDOW_AUTOSIZE);
        cv::namedWindow("pyr1", cv::WINDOW_AUTOSIZE);
        cv::namedWindow("pyr2", cv::WINDOW_AUTOSIZE);
        
        cv::imshow("rgb", img_rgb);
        cv::imshow("pyr1", img_pyr);
        cv::imshow("pyr2", img_pyr2);
        
        cv::waitKey(0);
        
    }
    
    // Example 2-7: Canny edge detector
    void example_2_7() {
        cv::Mat img_rgb, img_gry, img_cny;
        
        img_rgb = cv::imread(image_path_);
        cv::cvtColor(img_rgb, img_gry, cv::COLOR_BGR2GRAY);
        cv::Canny(img_gry, img_cny, 10, 100, 3, true);
        
        cv::namedWindow("Gray", cv::WINDOW_AUTOSIZE);
        cv::namedWindow("Canny", cv::WINDOW_AUTOSIZE);
        
        cv::imshow("Gray", img_gry);
        cv::imshow("Canny", img_cny);
        cv::waitKey(0);
    }
    
    // Example 2-9:
    void example_2_9() {
        cv::Mat img_rgb, img_pyr, img_pyr2;
        
        img_rgb = cv::imread(image_path_);
        cv::pyrDown(img_rgb, img_pyr);
        cv::pyrDown(img_pyr, img_pyr2);
        
        int x = 16, y = 32;
        
        cv::Vec3b intensity = img_rgb.at<cv::Vec3b>(y, x);
        
        uchar blue = intensity[0];
        uchar green = intensity[1];
        uchar red = intensity[2];
        
        cout << "At (x, y) = (" << x << "," << y << "): (blue, green, red) = (" <<
        (unsigned int) blue << ", " << (unsigned int) green << ", " << (unsigned int) red << ")" << endl;
        
        
        x /= 4; y /= 4;
        std::cout << "Pyramid2 pixel there is: (" <<
        (unsigned int) img_pyr2.at<cv::Vec3b>(y, x)[0] << "," <<
        (unsigned int) img_pyr2.at<cv::Vec3b>(y, x)[1] << "," <<
        (unsigned int) img_pyr2.at<cv::Vec3b>(y, x)[2] << ")" << std::endl;
        
    }
    
    // example 2-10: load videos from a camera
    void example_2_10() {
        cv::namedWindow("e", cv::WINDOW_AUTOSIZE);
        
        cv::VideoCapture cap;
        
        cap.open(0); // open the first camera
        
        if (!cap.isOpened()) {
            cerr << "Couldn't open capture." << endl;
        }
        
    }
    
    // example 2-11: write video
    void example_2_11() {
        cv::namedWindow("rgb", cv::WINDOW_AUTOSIZE);
        cv::namedWindow("log_polar", cv::WINDOW_AUTOSIZE);
        
        string dst_path = "images/out.avi";
        
        cv::VideoCapture cap(video_path_);
        
        double fps = cap.get(cv::CAP_PROP_FPS);
        cv::Size size(
                      (int) cap.get(cv::CAP_PROP_FRAME_WIDTH),
                      (int) cap.get(cv::CAP_PROP_FRAME_HEIGHT)
                      );
        
        cv::VideoWriter writer;
        writer.open(dst_path, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, size);
        
        cv::Mat logpolar_frame, bgr_frame;
        
        for (;;) {
            cap >> bgr_frame;
            if (bgr_frame.empty()) break;
            
            cv::imshow("rgb", bgr_frame);
            
            cv::logPolar(bgr_frame,
                         logpolar_frame,
                         cv::Point2f(bgr_frame.cols / 2, bgr_frame.rows / 2),
                         40,
                         cv::WARP_FILL_OUTLIERS);
            
            cv::imshow("Log_Polar", logpolar_frame);
            writer << logpolar_frame;
            char c = cv::waitKey(10);
            if (c == 27) break;
        }
        cap.release();
    }
private:
    string image_path_ = "images/fruits.jpg";
    string video_path_ = "images/test.avi";
    
};


int main(int argc, const char * argv[]) {
    Ch2Introduction ch2 = Ch2Introduction();
    ch2.example_2_1();
    return 0;
}

