#include <iostream>
#include <string>
#include <cstdio>

#include "asciify.hpp"

WINDOW *init_ncurses()
{
    // create a full screen ncurses window
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    int row, col;
    getmaxyx(stdscr, row, col);

    WINDOW *win = newwin(row, col, 0, 0);
    box(win, 0, 0);
    wrefresh(win);

    return win;
}


int main(int argc, char *argv[])
{

    // user input on what the program should do
    cout << "What would you like to do?" << endl;
    cout << "1. Render an ASCII image from ./images in terminal" << endl;
    cout << "2. Render live webcam in ASCII in the terminal" << endl;
    cout << "Please enter your choice (1 or 2): ";

    int choice;
    cin >> choice;

    // check if the choice is valid
    if (choice != 1 && choice != 2) {
        cout << "Error: Invalid choice." << endl;
        return 0;
    }

    cout << endl;

    // colored or not?
    cout << "Would you like it to be colored?" << endl;
    cout << "1. Yes" << endl;
    cout << "2. No" << endl;
    cout << "Please enter your choice (1 or 2): ";

    int color_choice;
    cin >> color_choice;
    
    // check if the choice is valid
    if (color_choice != 1 && color_choice != 2) {
        cout << "Error: Invalid choice." << endl;
        return 0;
    }

    cout << endl;

    // -----------------------------------------------------------

    if (choice == 1)
    {
        // read the image name from the user
        string img_name;
        cout << "Please enter the name of the image: ";
        cin >> img_name;

        // check if the image name is valid or not
        bool valid_img = check_img_name(img_name);
        if (valid_img) {
            cout << "Received Image: " << img_name << endl;
        } else {
            cout << "Error: Failed to check_image_name." << endl;
            return 0;
        }

        // read the image
        Mat image;
        string img_path = "./images/" + img_name;
        if (color_choice == 1)
            image = imread(img_path, IMREAD_COLOR);
        else if (color_choice == 2)
            image = imread(img_path, IMREAD_GRAYSCALE);

        if (image.empty()) {
            cout << "Error: Failed to read image" << endl;
            return 0;
        }

        WINDOW *win = init_ncurses();
        render_image(win, image, color_choice);
    }
    else if (choice == 2)
    {
        WINDOW *win = init_ncurses();
        render_video(win, color_choice);
    }

    // -----------------------------------------------------------

    return 0;
}