#include <iostream>
#include <string>
#include <cstdio>

#include "asciify.hpp"

#define ASCII_CHARS ".,:;+*?&%S#@"

// ----------------------------------------------------------------------------

// HELPER FUNCTIONS

// ----------------------------------------------------------------------------
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
    if (!has_colors())
    {
        endwin();
        std::cout << "Error: Terminal does not support color." << std::endl;
        return NULL;
    }
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
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
    {
        image = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
    }
    else if (color_choice == 2)
    {
        image = cv::imread(img_path, cv::IMREAD_COLOR);
    }

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

    // ------------------------------------------------------------------------

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

    // display the terminal size and image size and aspect ratio
    std::string msg1 = "Original Image size: " + std::to_string(img_height) + "x" + std::to_string(img_width);
    std::string msg2 = "Terminal size: " + std::to_string(t_height) + "x" + std::to_string(t_width);
    mvwprintw(win, 1, 1, msg1.c_str());
    mvwprintw(win, 2, 1, msg2.c_str());

    // ------------------------------------------------------------------------

    // calculate the aspect ratio of the displayed ascii image
    float disp_height, disp_width, aspect_ratio;
    disp_height = (float)(img_height);
    disp_width = (float)(img_width * 1.75);
    aspect_ratio = disp_height / disp_width;

    // display the display size and aspect ratio
    std::string msg3 = "Display size: " + std::to_string(disp_height) + "x" + std::to_string(disp_width);
    std::string msg4 = "Aspect Ratio: " + std::to_string(aspect_ratio);
    mvwprintw(win, 3, 1, msg3.c_str());
    mvwprintw(win, 4, 1, msg4.c_str());

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


    // display the new display size
    std::string msg5 = "New Display size: " + std::to_string(disp_height) + "x" + std::to_string(disp_width);
    mvwprintw(win, 5, 1, msg5.c_str());

    // ------------------------------------------------------------------------

    // // resize the image by creating a copy of the image
    cv::Mat image_copy = image.clone();
    cv::resize(image_copy, image_copy, cv::Size(disp_width, disp_height));

    // find the cursor position
    int start_y = (t_height - disp_height) / 2;
    int start_x = (t_width - disp_width) / 2;

    // render the image
    if (color_choice == 1)
    {
        for (int i = 0; i < disp_height; ++i)
        {
            for (int j = 0; j < disp_width; ++j)
            {
                int intensity = image_copy.at<uchar>(i, j);
                int index = intensity / 25;
                mvwprintw(win, start_y + i, start_x + j, "%c", ASCII_CHARS[index]);
            }
        }
    }
    // else if (color_choice == 2){
    //     for (int i = 0; i < disp_height; ++i)
    //     {
    //         for (int j = 0; j < disp_width; ++j)
    //         {
    //             cv::Vec3b intensity = image_copy.at<cv::Vec3b>(i, j);
    //             int index = (intensity[0] + intensity[1] + intensity[2]) / 3 / 25;
    //             mvwprintw(win, start_y + i, start_x + j, "%c", ASCII_CHARS[index]);
    //         }
    //     }
    // }

    // ------------------------------------------------------------------------

    wrefresh(win);
    return;
}

void renderVideo(WINDOW *win, const int color_choice)
{
    return;
}

// ----------------------------------------------------------------------------