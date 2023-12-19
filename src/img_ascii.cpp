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

#define CHAR_DISTORION 1.75

int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		cout << "Recieved Image: " << argv[1] << endl;
	}
	else
	{
		cout << "Error: No image argument entered" << endl;
		return 0;
	}

	//--------------------------------------------------------------------------

	// read the image
	Mat image;
	string img_dir = "../images/";

	image = imread(img_dir + string(argv[1]), IMREAD_GRAYSCALE);
	if (image.empty())
	{
		std::cout << "Could not read the image" << std::endl;
		return 1;
	}

	// find the scale factor and downsize the image to terminal size
	int height = image.size().height;
	int width = image.size().width;
	float scale = static_cast<float>(width) / height;

	int new_height = 100;
	int new_width = static_cast<int>(new_height * scale) * CHAR_DISTORION;

	Mat resized_image;
	cv::resize(image, resized_image, cv::Size(new_width, new_height));

	// cout << new_height << " " << new_width << endl;
	// // display the rezied image;
	// imshow("Display window", resized_image);
	// waitKey(0);

	//--------------------------------------------------------------------------

	// NCURSES and ASCII mapping
	string ascii_chars = ".,:;+*?%S#@";

	// Initialize ncurses
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	// Check if the terminal supports colors
	if (has_colors() == FALSE)
	{
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}

	// Start color
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	wbkgd(stdscr, COLOR_PAIR(1));

	// Create a new window
	WINDOW *win = newwin(new_height, new_width, 0, 0);
	wbkgd(win, COLOR_PAIR(1));

	//--------------------------------------------------------------------------

	// variables
	int ch;
	int term_height, term_width;

	const char *start_message = "Press 'r' to refresh, 'q' to quit. Start by pressing r";
	const char *resize_message = "The terminal size is too small. Please zoom out. Press 'r' to refresh";
	int sml = strlen(start_message);
	int rml = strlen(resize_message);

	// initial message
	ch = getch();
	getmaxyx(stdscr, term_height, term_width);
	mvwprintw(win, term_height / 2, (term_width / 2) - (sml / 2), start_message);
	wrefresh(win);

	// main loop
	for (;;)
	{
		ch = getch();

		if (ch == 'q')
		{
			break;
		}
		else if (ch == 'r')
		{
			wclear(win);
			getmaxyx(stdscr, term_height, term_width);

			if (term_height < new_height || term_width < new_width)
			{
				wclear(win);
				mvwprintw(win, term_height / 2, (term_width / 2) - (rml / 2), resize_message);
			}
			else
			{
				wclear(win);
				int start_y = (term_height - new_height) / 2;
				int start_x = (term_width - new_width) / 2;

				for (int i = 0; i < new_height; ++i)
				{
					for (int j = 0; j < new_width; ++j)
					{
						int intensity = resized_image.at<uchar>(i, j);
						int index = intensity / 25;
						mvwprintw(win, start_y + i, start_x + j, "%c", ascii_chars[index]);
					}
				}
			}
			wrefresh(win);
		}
		else if (ch == KEY_RESIZE)
		{
			wclear(win);
			resize_term(0, 0);
			wrefresh(win);
		}
	}

	// Wait for a key press before exiting
	getch();

	// Clean up NCurses
	endwin();

	//--------------------------------------------------------------------------

	return 0;
}