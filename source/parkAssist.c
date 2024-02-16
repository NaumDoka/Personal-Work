#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include "parkAssist.h"
#include "surroundViewsCameras.h"

int initParkAssist(){
    FILE *file = fopen("park assist.log", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Writing formatted date and time to the file
    fprintf(file, "Start\n");
    fclose(file);
}

uint16_t readParkAssist(int option) {
    FILE *fp;
    if(option == 1){
        fp = fopen("/dev/urandom", "r");
        if (fp == NULL) {
            perror("Error opening /dev/urandom");
            exit(EXIT_FAILURE);
        }
    } else {
        const char *filename = "urandomARTIFICIALE.binary";
        fp = fopen(filename, "rb");

        if (fp == NULL) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }
    }
    unsigned char byte = 0;

    // Read a single byte
    if (fread(&byte, 1, 1, fp) != 1) {
        fprintf(stderr, "Error reading from file\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    fclose(fp);
    uint16_t output = (uint16_t)(byte << 8) | readSurroundViews(option);
    unsigned char buffer[2];
    buffer[0] = byte;
    buffer[1] = readSurroundViews(option);
    FILE *logFile = fopen("park assist.log", "a");
    if (logFile != NULL) {
        fprintf(logFile, "Park Assist bytes: ");
        for (int i = 0; i < 2; i++) {
            fprintf(logFile, "%02x", buffer[i]);
        }
        fprintf(logFile, "\n");
        fclose(logFile);
    }
    return output;
}