#include <iostream>
#include <string>
#include <cstdio>


#include "asciify.hpp"

#define ASCII_CHARS ".,:;+*?&%S#@"

// ----------------------------------------------------------------------------

// HELPER FUNCTIONS

void imgtoAscii(WINDOW* win,cv::Mat &image, int disp_height, int disp_width, int start_x, int start_y)
{
    for (int i = 0; i < disp_height; ++i)
    {
        for (int j = 0; j < disp_width; ++j)
        {
            int intensity = image.at<uchar>(i, j);
            int index = intensity / 25;
            mvwprintw(win, start_y + i, start_x + j, "%c", ASCII_CHARS[index]);
        }
    }
    return;
}

void imgtoColorAscii(WINDOW* win, cv::Mat &image, int disp_height, int disp_width, int start_x, int start_y)
{
    for (int i = 0; i < disp_height; ++i) 
    {
        for (int j = 0; j < disp_width; ++j)
        {
            cv::Vec3b color = image.at<cv::Vec3b>(i, j);
            int r = color[2] / 51;
            int g = color[1] / 51;
            int b = color[0] / 51;
            int color_pair_index = 36 * r + 6 * g + b + 1; // +1 because color pair 0 is reserved

            int intensity = (color[0] + color[1] + color[2]) / 3;
            int index = intensity / 25;

            wattron(win, COLOR_PAIR(color_pair_index));
            mvwprintw(win, start_y + i, start_x + j, "%c", ASCII_CHARS[index]);
            wattroff(win, COLOR_PAIR(color_pair_index));
        }
    }
}

// ----------------------------------------------------------------------------

// HEADER FUNCTIONS

WINDOW *initNcurses()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // initialize color
    start_color();
    if (!has_colors() || !can_change_color() || COLOR_PAIRS < 217 || COLORS < 216)
    {
        endwin();
        std::cout << "Error: Terminal does not support color." << std::endl;
        return NULL;
    }

    // Initialize 216 color pairs
    for (int r = 0; r < 6; ++r)
    {
        for (int g = 0; g < 6; ++g)
        {
            for (int b = 0; b < 6; ++b)
            {
                // Create a color and initialize a color pair for it
                int color_index = 36 * r + 6 * g + b + 1; // +1 because color 0 is reserved
                init_color(color_index, r * 1000 / 5, g * 1000 / 5, b * 1000 / 5);
                init_pair(color_index, color_index, COLOR_BLACK);
            }
        }
    }
    wbkgd(stdscr, COLOR_PAIR(1));

    // draw the border
    box(stdscr, 0, 0);
    wrefresh(stdscr);
    getch();

    // display the welcome message on the std screen
    std::string msg1 = "Welcome to Asciify!";
    std::string msg2 = "Press any key to continue...";
    mvwprintw(stdscr, (LINES / 2), (COLS - msg1.length()) / 2, msg1.c_str());
    mvwprintw(stdscr, (LINES / 2) + 1, (COLS - msg2.length()) / 2, msg2.c_str());
    wrefresh(stdscr);
    getch();

    return stdscr;
}

bool checkImage(std::string img_name)
{
    if (img_name.empty())
    {
        std::cout << "Error: Image name not entered." << std::endl;
        return false;
    }

    // check if the image name has a valid extension
    std::string img_ext = img_name.substr(img_name.find_last_of(".") + 1);
    if (img_ext != "jpg" && img_ext != "jpeg" && img_ext != "png")
    {
        std::cout << "Error: Invalid image extension." << std::endl;
        return false;
    }

    // check if the image exists
    std::string img_dir = "./images/";
    const std::string img_path = img_dir + img_name;
    FILE *file = fopen(img_path.c_str(), "r");
    if (file == NULL)
    {
        std::cout << "Error: Image does not exist." << std::endl;
        return false;
    }
    fclose(file);

    return true;
}

cv::Mat readImage(std::string img_name, int color_choice)
{
    // read the image
    cv::Mat image;
    std::string img_dir = "./images/";
    const std::string img_path = img_dir + img_name;
    if (color_choice == 1)
        image = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
    else if (color_choice == 2)
        image = cv::imread(img_path, cv::IMREAD_COLOR);

    // check if the image is empty
    if (image.empty())
    {
        std::cout << "Could not read the image: " << img_path << std::endl;
        return cv::Mat();
    }

    return image;
}

void renderImage(WINDOW *win, cv::Mat &image, int color_choice)
{
    wclear(win);
    box(win, 0, 0);

    // get the dimension of the current screen
    int t_height, t_width;
    getmaxyx(win, t_height, t_width);
    if (t_height <= 0 || t_width <= 0) {
        std::cerr << "Invalid terminal size: " << t_height << "x" << t_width << std::endl;
        return;
    }

    // get the size of the image
    int img_height = image.rows;
    int img_width = image.cols;

    // calculate the aspect ratio of the displayed ascii image
    float disp_height, disp_width, aspect_ratio;
    disp_height = (float)(img_height);
    disp_width = (float)(img_width * 1.75);
    aspect_ratio = disp_height / disp_width;

    // size it down to fit the terminal height and width
    if (disp_width > t_width) {
        disp_width = t_width - 2;
        disp_height = disp_width * aspect_ratio;
    } 
    else if (disp_height > t_height)
    {
        disp_height = t_height - 2;
        disp_width = disp_height / aspect_ratio;
    } 
    else 
    {
        disp_height = img_height -2;
        disp_width = img_width -2;
    }

    // // resize the image by creating a copy of the image
    cv::Mat image_copy = image.clone();
    cv::resize(image_copy, image_copy, cv::Size(disp_width, disp_height));

    // find the cursor position
    int start_y = (t_height - disp_height) / 2;
    int start_x = (t_width - disp_width) / 2;

    // render the image
    if (color_choice == 1)
        imgtoAscii(win, image_copy, disp_height, disp_width, start_x, start_y);
    else if (color_choice == 2)
        imgtoColorAscii(win, image_copy, disp_height, disp_width, start_x, start_y);

    // refresh the window
    wrefresh(win);
    return;
}

void renderWebcam(WINDOW *win, int color_choice, int fps)
{
    // Open the default webcam
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        std::cout << "Error: Could not open webcam." << std::endl;
        return;
    }

    // Read and display each frame
    cv::Mat frame;
    while (true)
    {
        // Read the next frame
        if (!cap.read(frame)) {
            break;
        }

        // Display the frame
        renderImage(win, frame, color_choice);

        // Wait for 1000/FPS ms
        napms(1000 / fps);
    }
    cap.release();
}

// ----------------------------------------------------------------------------