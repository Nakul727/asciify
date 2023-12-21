#ifndef ASCIIFY_HPP
#define ASCIIFY_HPP

#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <ncurses.h>

WINDOW *initNcurses();
bool checkImage(std::string img_name);
cv::Mat readImage(std::string img_name, int color_choice);

void renderImage(WINDOW *win, cv::Mat& image, int color_choice);
void renderVideo(WINDOW *win, int color_choice);

#endif