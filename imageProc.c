#include <stdio.h>
#include <stdlib.h>

// Structure to hold grayscale pixel data
typedef struct {
    unsigned char value;
} PixelGray;

// Function to read a PGM image into a 2D array
PixelGray** readPGM(const char* filename, int* width, int* height) {
    FILE *file = fopen(filename, "rb"); //open file reading
    if (!file) {
        printf("Error opening file.\n");
        exit(1);
    }

    char fileFormat[2];
    fscanf(file, "%s", fileFormat); //get file format
    if (fileFormat[0] != 'P' || fileFormat[1] != '5') {
        printf("Not a valid file format.\n");
        exit(1);
    }

    fscanf(file, "%d %d", width, height); //get file dimensions
    int max_val;
    fscanf(file, "%d", &max_val); //get max val
    fgetc(file);//skip whitespace

    PixelGray** matrix = (PixelGray**) malloc((*height) * sizeof(PixelGray*)); //allocate memomry
    for (int i = 0; i< *height; i++) {
        matrix[i] = (PixelGray*) malloc((*width) * sizeof(PixelGray)); //allocate memory for row
        fread(matrix[i], sizeof(PixelGray), *width, file); //fill row with file data
    }

    fclose(file);
    return matrix;
}

// Function to write a 2D matrix as a PGM image
void writePGM(const char* filename, PixelGray** matrix, int* width, int* height) {
    FILE *file = fopen(filename, "wb"); //open file writing
    if (!file) {
        printf("Error opening file\n");
        exit(1);
    }

    fprintf(file, "P5\n%d %d\n255\n", *width, *height);//write header info
    for (int i = 0; i< *height; i++) {
        for (int j = 0; j< *width; j++) {
            fwrite(&(matrix[i][j]), sizeof(PixelGray), 1, file);//write matrix pixel to PGM file
        }
    }

    fclose(file);
}

// Function to threshold the image matrix
PixelGray** threshold(PixelGray** matrix, int* width, int* height) {
    PixelGray** newMatrix = (PixelGray**) malloc((*height) * sizeof(PixelGray*)); //allocate memory
    for (int i = 0; i< *height; i++) {
        newMatrix[i] = (PixelGray*) malloc((*width) * sizeof(PixelGray)); //allocate memory for row
        for (int j = 0; j < *width; j++) {
            if (matrix[i][j].value > 80) { //set new values
                newMatrix[i][j].value = 255;
            } else {
                newMatrix[i][j].value = 0;
            }
        }
    }

    return newMatrix;
}

// Function to rotate the image matrix
PixelGray** rotate(PixelGray** matrix, int* width, int* height) {
    //swap dimensions
    int newWidth = *height;
    int newHeight = *width;
    PixelGray** newMatrix = (PixelGray**) malloc(newHeight * sizeof(PixelGray*));//allocate memory
    for (int i = 0; i< newHeight; i++) {
        newMatrix[i] = (PixelGray*) malloc(newWidth * sizeof(PixelGray)); //allocate memory for row
        for (int j = 0; j< newWidth; j++) {
            newMatrix[i][j].value = matrix[j][i].value; //set new value for rotation
        }
    }

    //update original dimensions
    *width = newWidth;
    *height = newHeight;

    return newMatrix;
}
int main() {
    int width, height; // variable to hold width and height. Use reference in other functions
    PixelGray** image_original = readPGM("lenna.pgm", &width, &height);
// Now you have the grayscale image data in the 'image_original' 2D array
// Access pixel data using image[row][col].value
// For example, to access the pixel at row=2, col=3:
// unsigned char pixel_value = image[2][3].value;
// Create a new 2D array 'image_thresh' to store the threshold image
    PixelGray** image_thresh = threshold(image_original, &width, &height);
//write the image data as "threshold.pgm"
    writePGM("threshold.pgm", image_thresh, &width, &height);
// Create a new 2D array 'image_rotate' to store the rotated image
    PixelGray** image_rotate = rotate(image_original, &width, &height);
//write the image data as "rotate.pgm"
    writePGM("rotate.pgm", image_rotate, &width, &height);
    image_rotate = rotate(image_rotate, &width, &height);
//write the image data as "rotate_again.pgm"
    writePGM("rotate_again.pgm", image_rotate, &width, &height);
// Free the allocated memory when you're done
    for (int i = 0; i < height; ++i) {
        free(image_original[i]);
        free(image_thresh[i]);
        free(image_rotate[i]);
    }
    free(image_original);
    free(image_thresh);
    free(image_rotate);
    return 0;
}
