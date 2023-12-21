#include <iostream>
#include <string>
#include <cstdio>
#include <thread>

#include "asciify.hpp"


int main(int argc, char *argv[])
{
    std::cout << "\n";

    std::cout << "What would you like to do?\n"
              << "1. Render an ASCII image from ./images in terminal\n"
              << "2. Render live webcam in ASCII in the terminal\n"
              << "Please enter your choice (1 or 2): ";

    int choice;
    std::cin >> choice;
    if (choice != 1 && choice != 2) {
        std::cout << "Error: Invalid choice." << std::endl;
        return 0;
    }

    std::cout << "\n";

    std::cout << "Select one of the following: \n"
              << "1. Grayscale\n"
              << "2. Colored\n"
              << "Please enter your choice (1 or 2): ";

    int color_choice;
    std::cin >> color_choice;
    if (color_choice != 1 && color_choice != 2) {
        std::cout << "Error: Invalid choice." << std::endl;
        return 0;
    }

    std::cout << "\n";

    if (choice == 1)
    {
        // read the image name from the user
        std::string img_name;
        std::cout << "Please enter the name of the image: ";
        std::cin >> img_name;

        // validate image and type
        bool valid_img = checkImage(img_name);
        if (valid_img) {
            std::cout << "Received Image: " << img_name << std::endl;
        } else {
            std::cout << "Error: rrrr." << std::endl;
            return 0;
        }

        // read the image
        cv::Mat image;
        image = readImage(img_name, color_choice);
        if (image.empty()) {
            std::cout << "Error: Failed to read image" << std::endl;
            return 0;
        }

        // read the refresh rate from the user
        int refresh_rate;
        std::cout << "Please enter the refresh rate in seconds: ";
        std::cin >> refresh_rate;

        // initalize ncurses
        WINDOW *win = initNcurses();
        if (win == NULL) {
            std::cout << "Error: Failed to initialize ncurses" << std::endl;
            return 0;
        }
        
        // render the image
        while (true) {
            renderImage(win, image, color_choice);
            std::this_thread::sleep_for(std::chrono::seconds(refresh_rate));
        }
        delwin(win);
    }
    else if (choice == 2)
    {
        // initialize ncurses
        WINDOW *win = initNcurses();
        if (win == NULL) {
            std::cout << "Error: Failed to initialize ncurses" << std::endl;
            return 0;
        }

        // render the video
        renderVideo(win, color_choice);
        delwin(win);
    }


    return 0;
}