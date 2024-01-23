# Asciify

This is a program for generating ascii art in the terminal. Not only does this generate ascii art for an image, but it also supports live video from a webcam. This program is written in C++ and uses the OpenCV library for image processing and ncurses for terminal graphics.

Some cool features of this program include:
- Ability to generate ascii art from an image of various types (jpg, png, etc.)
- Live ascii art from a webcam
- Option for colored ascii art
- Resolution control through auto resizing based on terminal size


## Installation

To run this program you will need to have OpenCV installed. You can install OpenCV by following the instructions [here](https://docs.opencv.org/4.x/d0/d3d/tutorial_general_install.html). You will also need to have ncurses installed. 

To compile the program, run the following command in the root directory of the project:
```
make
```

To run the program, run the following command in the root directory of the project:
```
./bin/asciify
```

</br>

> **Note:** You need to add all the images you want to use to the `images` directory. The program will not work if you do not have any images in this directory. For customization, you can edit the defined variables in the `src/asciify.cpp` file. There are several factors that play into correct rendering of image and video that are terminal specific to your liking. This includes the terminal theme, color support, terminal font, rendering speed etc. Make sure to use a dark theme.
