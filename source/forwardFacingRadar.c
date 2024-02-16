#include <stdio.h>
#include <stdlib.h>
#include "forwardFacingRadar.h"

int initForwardFacingRadar(){
    FILE *file = fopen("radar.log", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Writing formatted date and time to the file
    fprintf(file, "Start\n");
    fclose(file);
}

void readForwardFacingRadar(unsigned char buffer[], size_t size, int option) {
    if(option == 1){
        FILE *fp = fopen("/dev/urandom", "r");
        FILE *logFile;
    
        if (fp == NULL) {
            perror("Error opening /dev/urandom");
            return;
        }
    
        // Read 8 bytes into the buffer
        if (fread(buffer, 1, size, fp) != size) {
            fprintf(stderr, "Error reading from /dev/urandom\n");
        } else {
            // Open radar.log for appending
            logFile = fopen("radar.log", "a");
            if (logFile != NULL) {
                fprintf(logFile, "Random bytes: ");
                for (size_t i = 0; i < size; i++) {
                    fprintf(logFile, "%02x", buffer[i]); // Print each byte in hexadecimal
                }
                fprintf(logFile, "\n");
                fclose(logFile);
            }
        }
    
        fclose(fp);
    } else{
        const char *inputFilename = "urandomARTIFICIALE.binary";
        const char *outputFilename = "radar.log";
        FILE *inputFile = fopen(inputFilename, "rb");
        FILE *outputFile;
        unsigned char buffer[8];

        if (inputFile == NULL) {
            perror("Error opening input file");
            exit(EXIT_FAILURE);
        }

        if (fread(buffer, 1, sizeof(buffer), inputFile) != sizeof(buffer)) {
            perror("Error reading from input file");
            fclose(inputFile);
            exit(EXIT_FAILURE);
        }

        fclose(inputFile);

        outputFile = fopen(outputFilename, "a"); // Open in append mode
        if (outputFile == NULL) {
            perror("Error opening output file");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < sizeof(buffer); i++) {
            fprintf(outputFile, "%02x ", buffer[i]);
        }
        fprintf(outputFile, "\n");

        fclose(outputFile);
    }
}
