# C Image Processor (PGM)

This project is a **simple image processor written in C** that works with **PGM (Portable GrayMap - P2)** image files.  
The program allows loading, manipulating, and saving grayscale images through an **interactive terminal menu**.

## Features

The system provides the following operations:

1. **Load image**
   - Loads an image in `.pgm` format (P2 ASCII).

2. **Save image**
   - Saves the current image in `.pgm` format.

3. **Fill image with stripes**
   - Generates an image with horizontal black and white stripes.

4. **Fill image with random values**
   - Creates an image with random pixel values (0–255).

5. **Threshold with user value**
   - Converts the image into a binary image using a threshold provided by the user.

6. **Threshold using average value**
   - Calculates the average pixel value of the image and uses it as the threshold.

7. **Threshold using most frequent value**
   - Finds the most frequent pixel value and uses it as the threshold.

8. **Mean filter**
   - Applies a **5×5 mean filter** to smooth the image.

9. **Exit program**

## Project Structure

.
├── main.c
└── README.md

## Supported Image Format

The program uses the **PGM ASCII format (P2)**.

Example `.pgm` file:

P2
4 4
255
0 50 100 150
50 100 150 200
100 150 200 250
150 200 250 255


## Compilation

To compile the project using **GCC**:

```bash
gcc main.c -o image_processor

Execution

After compiling:
./image_processor

On Windows:
image_processor.exe

Limitations

Maximum supported image size: 512 × 512 pixels

Only PGM P2 (ASCII) images are supported

Processing is limited to grayscale values (0–255)

Technologies Used

C programming language

Standard C libraries:

stdio.h

stdlib.h

time.h

ctype.h

Purpose

This project was developed for learning purposes, focusing on image manipulation using matrices in C, file handling, and basic image processing techniques.
