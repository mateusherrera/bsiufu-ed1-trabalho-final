#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "adt_image.h"

// A function to check if the extensions is correct
int checkExt(char*, int);

// A function to check if a file exist and if it will be overwrite or not
int checkFile(char*);

// A function to open a imagem
void openImg(char*);

// A function to convert a txt image to a imm image
void convert_txt_to_imm(char*, char*);

// A function to segment a image
void segmentImg(int, char*, char*);

// A function to detect the linked components of a image
void ccImg(char*, char*);

// A function to make the path of a labyrinth
void labyrinthImg(char*, char*);

int main(int argc, char** argv){
    // if there's no arguments
    if(argc == 1)
        printf("\nSem Argumentos\n");

    // the argument is -open
    else if(!strcmp(argv[1], "-open")){
        // if the number of arguments isn't valid
        if(argc != 3)
            printf("\nNúmero de argumentos inválido\n");

        // check if the extension of the file is correct
        else if(checkExt(argv[2], strlen(argv[2])))
            printf("\nExtensão inválida.\n");

        // if is all correct
        else
            openImg(argv[2]);

    // the argument is -convert
    }else if(!strcmp(argv[1], "-convert")){
        // if the number of arguments isn't valid
        if(argc != 4)
            printf("\nNúmero de argumentos inválido\n");

        // check if the extension of the first file is correct
        else if(checkExt(argv[2], strlen(argv[2])))
            printf("\nExtensão inválida\n");

        // check if the extension of the second file is correct
        else if(checkExt(argv[3], strlen(argv[3])))
            printf("\nExtensão inválida\n");

        // convert the image to imm
        else if(argv[2][strlen(argv[2]) - 3] == 't' && argv[3][strlen(argv[3]) - 3] == 'i')
            convert_txt_to_imm(argv[2], argv[3]);

        // if the extension isn't correct
        else
            printf("\nA conversão deve ser feita de um txt(primeiro arquivo) para um imm(segundo arquivo)\n");
        // else if end

    // the argument is -segment
    }else if(!strcmp(argv[1], "-segment")){
        // if the number of arguments isn't valid
        if(argc != 5){
            printf("\nNúmero de argumentos inválido\n");
            printf("\nAté logo!\n");
            return 1;
        }// if

        // if the size of thr is correct
        if(strlen(argv[2]) > 3)
            printf("\nValor inválido para thr\n");

        // if thr is numeric
        else{
            int val = 0;
            for(int i = 0; argv[2][i] != '\0'; ++i){

                // if the char isn't numeric val receive 1
                if(!isdigit(argv[2][i]))
                    val = 1;
            }// for

            if(val){
                printf("\nO argumento thr não é um número\n");
                return 1;
            }// if
        }// else

        int thr = atoi(argv[2]);
        // check if thr is correct
        if(thr > 255 || thr < 0)
            printf("\nValor de thr inválido\n");

        // check if the extension of the first file is correct
        else if(checkExt(argv[3], strlen(argv[3])))
            printf("\nExtensão inválida\n");

        // check if the extension of the second file is correct
        else if(checkExt(argv[4], strlen(argv[4])))
            printf("\nExtensão inválida\n");

        // segment the image
        else if(argv[3][strlen(argv[3]) - 3] == 'i' && argv[4][strlen(argv[4]) - 3] == 'i')
            segmentImg(thr, argv[3], argv[4]);

        // if the extension isn't correct
        else
            printf("\nA conversão deve ser feita de um imm(primeiro arquivo) para um imm(segundo arquivo)\n");
        // else if end

    // the argument is -cc
    }else if(!strcmp(argv[1], "-cc")){
        // if the number of arguments isn't valid
        if(argc != 4)
            printf("\nNúmero de argumentos inválido\n");

        // check if the extension of the first file is correct
        else if(checkExt(argv[2], strlen(argv[2])))
            printf("\nExtensão inválida\n");// Add the neighbor point to the stack (and receive the label before)

        // check if the extension of the second file is correct
        else if(checkExt(argv[3], strlen(argv[3])))
            printf("\nExtensão inválida\n");

        // make the cc of the image
        else if(argv[2][strlen(argv[2]) - 3] == 'i' && argv[3][strlen(argv[3]) - 3] == 'i')
            ccImg(argv[2], argv[3]);

        // if the extension isn't correct
        else
            printf("\nA conversão deve ser feita de um imm(primeiro arquivo) para um imm(segundo arquivo)\n");
        // else if end

    // the argument is -lab
    }else if(!strcmp(argv[1], "-lab")){
        // if the number of arguments isn't valid
        if(argc != 4)
            printf("\nNúmero de argumentos inválido\n");

        // check if the extension of the first file is correct
        else if(checkExt(argv[2], strlen(argv[2])))
            printf("\nExtensão inválida\n");

        // check if the extension of the second file is correct
        else if(checkExt(argv[3], strlen(argv[3])))
            printf("\nExtensão inválida\n");

        // make the path of a labyrinth
        else if(argv[2][strlen(argv[2]) - 3] == 'i' && argv[3][strlen(argv[3]) - 3] == 'i')
            labyrinthImg(argv[2], argv[3]);

        // if the extension isn't correct
        else
            printf("\nA conversão deve ser feita de um imm(primeiro arquivo) para um imm(segundo arquivo)\n");
        // else if end

    // the argument isn't valid
    }else
        printf("\nArgumento inválido\n");
    // else if end

    printf("\nAté logo!\n");
    return 0;
}// main

int checkExt(char* str, int size_arc){
    // if the archive isn't valid
    if(size_arc < 5 || str[size_arc - 4] != '.')
        return 1; // return true to error

    char format[4] = {str[size_arc - 3],
                      str[size_arc - 2],
                      str[size_arc - 1]};

    if(strcmp(format, "txt") && strcmp(format, "imm"))
        return 1; // return true to error

    return 0; // return false to error
}// checkExt

int checkFile(char* str){
    FILE* arq_test;

    if(str[strlen(str) - 1] == 't')
        arq_test = fopen(str, "r");
    else
        arq_test = fopen(str, "rb");

    if(arq_test){
        char c;
        printf("\nArquivo \"%s\" já existe. Deseja sobrescrevê-lo?[S/N] ", str);
        scanf("%c", &c);
        getchar();

        while(c != 'S' && c != 's' && c != 'N' && c != 'n'){
            printf("\nOpção Inválida!!\n");
            scanf("%c", &c);
            getchar();
        }// while

        if(c == 'N' || c == 'n')
            return 1;// "no" answer
    }// if

    return 0; // "yes" answer
}// checkFile

void openImg(char* arq){
    Image* img;

    if(arq[strlen(arq) - 1] == 't')
        img = image_create_txt(arq);
    else
        img = image_create_imm(arq);

    // Error in the image_create
    if(!img)
        printf("\nErro ao criar imagem\n");
    else{
        image_print(img);

        image_free(img);
    }// else if

    return;
}// openImg

void convert_txt_to_imm(char* arq_txt, char* arq_imm){
    if(!checkFile(arq_imm)){
        Image* img = image_create_txt(arq_txt);
        // Error in the image_create
        if(!img)
            printf("\nErro ao criar imagem\n");
        else{
            int res = image_write_bin(img, arq_imm);

            if(res == -1)
                printf("\nIMG não alocada\n");
            else if(res)
                printf("\nErro ao abrir \"%s\"\n", arq_imm);
            else
                printf("\nArquivo convertido\n");

            image_free(img);
        }// else if
    }// if

    return;
}// convert_txt_to_imm

void segmentImg(int thr, char* arq_org, char* arq_dest){
    if(!checkFile(arq_dest)){
        Image* img_imm = image_create_imm(arq_org);
        // Error in the image_create
        if(!img_imm)
            printf("\nErro ao criar imagem\n");
        else{
            Image* seg_img = NULL;
            int res = image_segment(img_imm, &seg_img, thr);

            if(res == -1)
                printf("\nImg não alocada\n");
            else if(res == -2)
                printf("\nMemória Insuficiente\n");
            else if(res == -4)
                printf("\nErro ao abrir \"%s\"\n", arq_dest);
            else{
                if(image_write_bin(seg_img, arq_dest))
                    printf("\nErro ao abrir \"%s\"\n", arq_dest);
                else
                    printf("\nArquivo segmentado\n");
            }// else if

            if(seg_img)
                image_free(seg_img);
            image_free(img_imm);
        }// else if
    }// if

    return;
}// segmentImg

void ccImg(char* arq_org, char* arq_dest){
    if(!checkFile(arq_dest)){
        Image* img_seg = image_create_imm(arq_org);
        // Error in the image_create
        if(!img_seg)
            printf("\nErro ao criar imagem\n");
        else{
            Image* cc_img = NULL;
            int res = image_DLcomponents(img_seg, &cc_img);

            if(res == -1)
                printf("\nImg não alocada\n");
            else if(res == -2)
                printf("\nMemória Insuficiente\n");
            else if(res == -4)
                printf("\nErro ao abrir \"%s\"\n", arq_dest);
            else if(res == -5)
                printf("\n\"%s\" não está segmentado\n", arq_org);
            else{
                if(image_write_bin(cc_img, arq_dest))
                    printf("\nErro ao abrir \"%s\"\n", arq_dest);
                else
                    printf("\nObjetos detectados\n");
            }// else if

            if(cc_img)
                image_free(cc_img);
            image_free(img_seg);
        }// else if
    }// if

    return;
}// ccImg

void labyrinthImg(char* arq_org, char* arq_dest){
    if(!checkFile(arq_dest)){
        Image* img_seg = image_create_imm(arq_org);
        // Error in the image_create
        if(!img_seg)
            printf("\nErro ao criar imagem\n");
        else{
            Image* lab_img = NULL;
            int res = image_labyrinth(img_seg, &lab_img);

            if(res == -1)
                printf("\nImg não alocada\n");
            else if(res == -2)
                printf("\nMemória Insuficiente\n");
            else if(res == -4)
                printf("\nErro ao abrir \"%s\"\n", arq_dest);
            else if(res == -5)
                printf("\n\"%s\" não está segmentado\n", arq_org);
            else if(res == -6)
                printf("\n\"%s\" não é uma imagem válida\n", arq_org);
            else{
                if(image_write_bin(lab_img, arq_dest))
                    printf("\nErro ao abrir \"%s\"\n", arq_dest);
                else
                    printf("\nLabirinto resolvido\n");
            }// else if

            if(lab_img)
                image_free(lab_img);
            image_free(img_seg);
        }// else if
    }// if

    return;
}// labyrinthImg