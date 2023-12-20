#ifndef ASCIIFY_HPP
#define ASCIIFY_HPP

#include <iostream>
#include <string>
#include <cstdio>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <ncurses.h>

using namespace std;
using namespace cv;

bool check_img_name(string img_name);
void render_image(WINDOW *win, Mat image, int color_choice);
void render_video(WINDOW *win, int color_choice);

#endif