#include <iostream>
#include <string>
#include <cstdio>

#include "asciify.hpp"

// ----------------------------------------------------------------------------

#define IMG_DIR "./images/"
#define ASCII_CHARS  " .:-=+*#%@"

// ----------------------------------------------------------------------------

// Image functions

bool check_img_name(string img_name)
{
	if (img_name.empty()){
		cout << "Error: Image name not entered." << endl;
		return false;
 	}
 
	// check if the image name has a valid extension
	string img_ext = img_name.substr(img_name.find_last_of(".") + 1);
	if (img_ext != "jpg" && img_ext != "jpeg" && img_ext != "png"){
		cout << "Error: Invalid image extension." << endl;
		return false;
	}
	return true;
}



void render_image(WINDOW *win, Mat image, int color_choice)
{
	cout << "Entered ASCII Image Function correctly" << endl;
	return;
}


// ----------------------------------------------------------------------------

// Video functions

void render_video(WINDOW *win, int color_choice)
{
	cout << "Entered Video Function correctly" << endl;
	return;
}

// ----------------------------------------------------------------------------
