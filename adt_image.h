#ifndef ADT_IMAGE_H_INCLUDED
#define ADT_IMAGE_H_INCLUDED

#include "return.h"

// Definition to struct image
typedef struct image Image;

// A function that put the values of a image on a matrix - for non-binary
Image* image_create_txt(char*);

// A function that put the values of a image on a matrix - for binary
Image* image_create_imm(char*);

// A function that free a struct image type
void image_free(Image*);

// A function that segment a binary image
int image_segment(Image*, Image**, int);

// A funciton that detect linked components in a image
int image_DLcomponents(Image*, Image**);

// A function that make the path of a labyrinth
int image_labyrinth(Image*, Image**);

// A function that write the image values in the file - binary
int image_write_bin(Image*, char*);

// A function that print the values of a image
void image_print(Image*);

#endif // ADT_IMAGE_H_INCLUDED