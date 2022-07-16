#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adt_image.h"
#include "stack_point.h"

struct image{
    unsigned int* matrix;
    unsigned int columns, lines;
};

Image* image_create_txt(char* arq_name){
    Image* img = malloc(sizeof(Image));

    if(img){
        FILE* arq;

        arq = fopen(arq_name, "r");

        unsigned int columns = 1, lines = 0;

        // If the archive doesn't open
        if(!arq){
            free(img);
            img = NULL;
            return img;
        }// if

        // Find the number of columns to create the matrix
        for(char c = fgetc(arq); c != '\n'; c = fgetc(arq)){
            if(c == ' ')
                ++columns;
        }// for


        // Back to the start of the archive
        fseek(arq, 0, SEEK_SET);

        // Find the number of lines to create the matrix
        for(char c = fgetc(arq); !feof(arq); c = fgetc(arq)){
            if(c == '\n')
                ++lines;
        }// for

        // Alloc the matrix to receive the image
        img->matrix = malloc(lines * columns * sizeof(unsigned int));
        // If isn't allocated correctly
        if(!img->matrix){
            free(img);
            img = NULL;
        // If is allocated
        }else{
            img->columns = columns;
            img->lines = lines;

            // Back to the start of the archive
            fseek(arq, 0, SEEK_SET);

            // Put the values of the image in the matrix
            for(int i = 0; i < img->lines; ++i){
                for(int j = 0; j < img->columns; ++j)
                    fscanf(arq, "%u", &img->matrix[(i * img->columns) + j]);
            }// for
        }// else if
        fclose(arq);
    }// if

    return img;
}// image_create

Image* image_create_imm(char* arq_name){
    Image* img = malloc(sizeof(Image));

    if(img){
        FILE* arq;

        arq = fopen(arq_name, "rb");

        unsigned int columns, lines;

        // If the archive doesn't open
        if(!arq){
            free(img);
            img = NULL;
            return img;
        }// if

        // Find the lines number of the image - the fist 4 bytes of the archive
        fread(&lines, sizeof(unsigned int), 1, arq);
        // Find the columns number of the image - the second 4 bytes of the archive
        fread(&columns, sizeof(unsigned int), 1, arq);

        // Alloc the matrix to receive the image
        img->matrix = malloc(lines * columns * sizeof(unsigned int));
        if(!img->matrix){
            free(img);
            img = NULL;
        }else{
            img->lines = lines;
            img->columns = columns;
            // Put the values of the image in the matrix
            fread(img->matrix, sizeof(unsigned int), (img->lines * img->columns), arq);
        }// else if
        fclose(arq);
    }// if

    return img;
}// image_create_imm

void image_free(Image* img){
    if(img){
        free(img->matrix);
        free(img);
    }// if

    return;
}// image_free

int image_segment(Image* img, Image** dest_img, int thr){
    if(img){
        *dest_img = malloc(sizeof(Image));
        if(*dest_img){
            // Line number
            (*dest_img)->lines = img->lines;
            // Columns number
            (*dest_img)->columns = img->columns;

            (*dest_img)->matrix = calloc(img->lines * img->columns, sizeof(unsigned int));
            if((*dest_img)->matrix){
                // Put the segment values of the image in the matrix
                for(int i = 0; i < img->lines * img->columns; ++i){
                    if(thr < img->matrix[i])
                        (*dest_img)->matrix[i] = 1;
                }// for
            }else{
                free(*dest_img);
                return OUT_OF_MEMORY;
            }// else if
        }else
            return OUT_OF_MEMORY;
    }else
        return STRUCT_NOT_ALLOC;

    return SUCCESS;
}// image_segment

int image_DLcomponents(Image* img, Image** dest_img){
    if(img){
        // Verify if "img" is segment
        int flag = 0;
        for(int i = 0; i < img->lines; ++i){
            for(int j = 0; j < img->columns; ++j){
                if(img->matrix[(i * img->columns) + j] > 1)
                    flag = 1;
            }// for
        }// for
        // If "img" isn't a segment image
        if(flag)
            return NOT_SEGMENT_IMAGE;

        // Counter to the number of object detected
        unsigned int label = 1;
        // Number of neighbors
        int num_neigh = 4;
        // Array to the neighbor points
        Point neigh[num_neigh];

        neigh[0].x = 0; neigh[0].y = -1;
        neigh[1].x = 0; neigh[1].y = 1;
        neigh[2].x = -1; neigh[2].y = 0;
        neigh[3].x = 1; neigh[3].y = 0;

        // Matrix to save the image with the objects detected
        unsigned int* dlc_matrix = calloc(img->lines * img->columns, sizeof(unsigned int));
        if(!dlc_matrix)
            return OUT_OF_MEMORY;

        // Stack of point to save the line(y) and column(x) that have a object
        Stack* st_object = stack_create();
        if(!st_object)
            return OUT_OF_MEMORY;

        // Go through the matrix
        for(int i = 0; i < img->lines; ++i){
            for(int j = 0; j < img->columns; ++j){
                // If have a object in img->matrix that isn't marked in dlc_matrix
                if(img->matrix[(i * img->columns) + j]
                   && !dlc_matrix[(i * img->columns) + j]){

                    // Point to save the first point of a object
                    Point po_object;
                    po_object.x = j;
                    po_object.y = i;

                    // Put the first point of a object in the stack
                    if(stack_push(st_object, po_object)){
                        free(dlc_matrix);
                        stack_free(st_object);
                        return OUT_OF_MEMORY;
                    }// if

                    // "While" to put the label value in the linked components
                    while(!stack_empty(st_object)){
                        Point po_aux, po_neigh;

                        // Get and remove the top point of the stack
                        stack_top(st_object, &po_aux);
                        stack_pop(st_object);

                        // Put label value in the removed top point of the stack
                        dlc_matrix[(po_aux.y * img->columns) + po_aux.x] = label;

                        // Find the neighbors of "po_aux" and put them in the stack
                        for(int i = 0; i < num_neigh; ++i){
                            if((po_aux.x + neigh[i].x) < img->columns && (po_aux.x + neigh[i].x) >= 0
                               && (po_aux.y + neigh[i].y) < img->lines && (po_aux.y + neigh[i].y) >= 0
                               && img->matrix[((po_aux.y + neigh[i].y) * img->columns) + (po_aux.x + neigh[i].x)]
                               && !dlc_matrix[((po_aux.y + neigh[i].y) * img->columns) + (po_aux.x + neigh[i].x)]){

                                po_neigh.x = po_aux.x + neigh[i].x;
                                po_neigh.y = po_aux.y + neigh[i].y;

                                if(stack_push(st_object, po_neigh)){
                                    free(dlc_matrix);
                                    stack_free(st_object);
                                    return OUT_OF_MEMORY;
                                }// if
                            }// if
                        }// for

                    }// while

                    // Update label
                    ++label;
                }// if
            }// for
        }// for

        stack_free(st_object);

        // Create the Image type
        *dest_img = malloc(sizeof(Image));
        if(*dest_img){
            // Line number
            (*dest_img)->lines = img->lines;
            // Columns number
            (*dest_img)->columns = img->columns;
            // Image values
            (*dest_img)->matrix = dlc_matrix;
        }else{
            free(dlc_matrix);
            return OUT_OF_MEMORY;
        }// else if
    }else
        return STRUCT_NOT_ALLOC;

    return SUCCESS;
}// image_DLcomponents

int image_labyrinth(Image* img, Image** dest_img){
    if(img){
        // Verify if "img" is segment
        int flag = 0;
        for(int i = 0; i < img->lines; ++i){
            for(int j = 0; j < img->columns; ++j){
                if(img->matrix[(i * img->columns) + j] > 1)
                    flag = 1;
            }// for
        }// for
        // If "img" isn't a segment image
        if(flag)
            return NOT_SEGMENT_IMAGE;

        // Find point where is the start of the lab
        int start_line, start_column = 0, flag_start = 0, flag_end = 0;
        for(int i = 0; i < img->lines; ++i){
            if(img->matrix[i * img->columns]){
                start_line = i;
                ++flag_start;
            }// if
            if(img->matrix[(i * img->columns) + (img->columns - 1)])
                ++flag_end;
        }// for
        // If have more then one or any start and/or end
        if(flag_start != 1 || flag_end != 1)
            return INVALID_IMAGE;

        int going_down = 0, going_up = 0, returning = 0, going_ahead = 1;
        unsigned int* lab_matrix = calloc(img->lines * img->columns, sizeof(unsigned int));
        if(!lab_matrix)
            return OUT_OF_MEMORY;

        // Stack of point to save the path that is going through
        Stack* st_path = stack_create();
        if(!st_path){
            free(lab_matrix);
            return OUT_OF_MEMORY;
        }// if

        // Stack of point to save the bifurcations
        Stack* st_bifurcation = stack_create();
        if(!st_bifurcation){
            free(lab_matrix);
            stack_free(st_path);
            return OUT_OF_MEMORY;
        }// if

        // Copy img->matrix to lab_matrix
        for(int i = 0; i < (img->lines * img->columns); ++i)
            lab_matrix[i] = img->matrix[i];

        // Make the path with "2"'s in lab_matrix
        Point po_cur;
        po_cur.x = start_column;
        po_cur.y = start_line;
        if(stack_push(st_path, po_cur)){
            stack_free(st_path);
            stack_free(st_bifurcation);
            free(lab_matrix);
            return OUT_OF_MEMORY;
        }// if
        while(po_cur.x != (img->columns - 1)){
            Point po_aux;
            stack_top(st_path, &po_aux);

            // Up point
            if((going_up || going_ahead || returning) && ((po_cur.y - 1 >= 0) && img->matrix[((po_cur.y - 1) * img->columns) + po_cur.x])){
                --po_cur.y;

                if(stack_push(st_bifurcation, po_cur)){
                    stack_free(st_path);
                    stack_free(st_bifurcation);
                    free(lab_matrix);
                    return OUT_OF_MEMORY;
                }// if

                ++po_cur.y;
            }// Up point

            // Down point
            if((going_down || going_ahead || returning) && ((po_cur.y + 1 < img->lines) && img->matrix[((po_cur.y + 1) * img->columns) + po_cur.x])){
                ++po_cur.y;

                if(stack_push(st_bifurcation, po_cur)){
                    stack_free(st_path);
                    stack_free(st_bifurcation);
                    free(lab_matrix);
                    return OUT_OF_MEMORY;
                }// if

                --po_cur.y;
            }// Down point

            // Forward point
            if((going_ahead || going_up || going_down) && ((po_cur.x + 1 < img->columns) && img->matrix[(po_cur.y * img->columns) + (po_cur.x + 1)])){
                ++po_cur.x;

                if(stack_push(st_bifurcation, po_cur)){
                    stack_free(st_path);
                    stack_free(st_bifurcation);
                    free(lab_matrix);
                    return OUT_OF_MEMORY;
                }// if

                --po_cur.x;
            }// Forward point

            // Previous point
            if((returning || going_up || going_down) && ((po_cur.x - 1 >= 0) && img->matrix[(po_cur.y * img->columns) + (po_cur.x - 1)])){
                --po_cur.x;

                if(stack_push(st_bifurcation, po_cur)){
                    stack_free(st_path);
                    stack_free(st_bifurcation);
                    free(lab_matrix);
                    return OUT_OF_MEMORY;
                }// if

                ++po_cur.x;
            }// Previous point

            // If there's a hole in the path
            if(stack_empty(st_bifurcation)){
                stack_free(st_path);
                stack_free(st_bifurcation);
                free(lab_matrix);
                return INVALID_IMAGE;

            // Get the next point of the path
            }else{
                // Get the new point of the path in "po_cur"
                stack_top(st_bifurcation, &po_cur);
                stack_pop(st_bifurcation);

                // "po_cur" can't be on the path if their isn't "1"
                while(!stack_empty(st_bifurcation) && lab_matrix[(po_cur.y * img->columns) + po_cur.x] != 1){
                    stack_top(st_bifurcation, &po_cur);
                    stack_pop(st_bifurcation);
                }// while

                // If "po_cur" isn't valid
                if(lab_matrix[(po_cur.y * img->columns) + po_cur.x] != 1){
                    stack_free(st_path);
                    stack_free(st_bifurcation);
                    free(lab_matrix);
                    return INVALID_IMAGE;
                }// if

                // The aux point needs to be neighbor of the next point of the path
                while((po_aux.x != po_cur.x - 1 || po_aux.y != po_cur.y) &&
                      (po_aux.x != po_cur.x + 1 || po_aux.y != po_cur.y) &&
                      (po_aux.y != po_cur.y - 1 || po_aux.x != po_cur.x) &&
                      (po_aux.y != po_cur.y + 1 || po_aux.x != po_cur.x)){

                    // Passed path receive "3"
                    lab_matrix[(po_aux.y * img->columns) + po_aux.x] = 3;

                    stack_pop(st_path);
                    stack_top(st_path, &po_aux);
                }// while

                // Define the direction of the path
                returning = 0;
                going_ahead = 0;
                going_down = 0;
                going_up = 0;
                if(po_aux.x == po_cur.x + 1)
                    returning = 1; // Left
                else if(po_aux.x == po_cur.x - 1)
                    going_ahead = 1; // Right
                else if(po_aux.y == po_cur.y + 1)
                    going_up = 1; // Up
                else
                    going_down = 1; // Down

                // Push the new point in the path
                if(stack_push(st_path, po_cur)){
                    stack_free(st_path);
                    stack_free(st_bifurcation);
                    free(lab_matrix);
                    return OUT_OF_MEMORY;
                }// if
                // Path receive "2"
                lab_matrix[(po_cur.y * img->columns) + po_cur.x] = 2;
            }// else if
        }// while

        stack_free(st_bifurcation);

        // Copy img->matrix to lab_matrix
        for(int i = 0; i < (img->lines * img->columns); ++i)
            lab_matrix[i] = img->matrix[i];

        // Put "2" just where is in "st_path"
        while(!stack_empty(st_path)){
            stack_top(st_path, &po_cur);
            stack_pop(st_path);

            lab_matrix[(po_cur.y * img->columns) + po_cur.x] = 2;
        }// while

        stack_free(st_path);

        // Create the Image type
        *dest_img = malloc(sizeof(Image));
        if(*dest_img){
            // Line number
            (*dest_img)->lines = img->lines;
            // Columns number
            (*dest_img)->columns = img->columns;
            // Image values
            (*dest_img)->matrix = lab_matrix;
        }else{
            free(lab_matrix);
            return OUT_OF_MEMORY;
        }// else if
    }else
        return STRUCT_NOT_ALLOC;

    return SUCCESS;
}// image_labyrinth

int image_write_bin(Image* img, char* file_name){
    if(img){
        FILE* file = fopen(file_name, "wb");
        if(!file)
            return OPEN_ARCHIVE_ERROR;

        // Put the line number in the firsts 4 bytes of the binary archive
        fwrite(&img->lines, sizeof(unsigned int), 1, file);
        // Put the columns number in the seconds 4 bytes of the binary archive
        fwrite(&img->columns, sizeof(unsigned int), 1, file);
        // Put the values of the image in the binary archive
        fwrite(img->matrix, sizeof(unsigned int), img->lines * img->columns, file);

        fclose(file);
    }else
        return STRUCT_NOT_ALLOC;

    return SUCCESS;
}// image_write_bin

void image_print(Image* img){
    if(img){
        for(int i = 0; i < img->lines; ++i){
            for(int j = 0; j < img->columns; ++j)
                printf("%4u ", img->matrix[(i * img->columns) + j]);
            printf("\n");
        }// for
    }// if

    return;
}// image_print
