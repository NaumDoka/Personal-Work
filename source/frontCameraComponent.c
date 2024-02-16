#include "frontCameraComponent.h"
#include <stdio.h>

int initFrontCameraComponent(){
    FILE *file = fopen("camera.log", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Writing formatted date and time to the file
    fprintf(file, "Start\n");
    fclose(file);
}

char* readLineFromFile(FILE *file) {
    static char line[256];  // Static buffer to store the line
    if (fgets(line, sizeof(line), file) == NULL) {
        return NULL;  // Return NULL if end-of-file or error
    }
    FILE *output_file = fopen("camera.log", "a");
    if (output_file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Writing formatted date and time to the file
    fprintf(output_file, "%s", line);

    // Closing the file
    fclose(output_file);
    return line;
}
