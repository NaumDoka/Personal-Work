#include <stdio.h>
#include <stdlib.h>
#include "surroundViewsCameras.h"

int initSurroundViews(){
    FILE *file = fopen("park assist.log", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Writing formatted date and time to the file
    fprintf(file, "Start\n");
    fclose(file);
}

unsigned char readSurroundViews(int option) {
    if(option == 1){
        FILE *fp = fopen("/dev/urandom", "r");
        unsigned char byte = 0;

        if (fp == NULL) {
            perror("Error opening /dev/urandom");
            exit(EXIT_FAILURE);
        }

        // Read a single byte
        if (fread(&byte, 1, 1, fp) != 1) {
            fprintf(stderr, "Error reading from /dev/urandom\n");
            fclose(fp);
            exit(EXIT_FAILURE);
        }

        fclose(fp);
        return byte;
    }
    const char *filename = "urandomARTIFICIALE.binary";
    FILE *file = fopen(filename, "rb");
    unsigned char byte = 0;

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    if (fread(&byte, 1, 1, file) != 1) {
        perror("Error reading from file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return byte;
}