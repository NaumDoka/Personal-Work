#include <stdio.h>
#include <time.h>
#include "steerByWire.h"

int initSteer(){
    FILE *file = fopen("steer.log", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Writing formatted date and time to the file
    fprintf(file, "Start\n");
    fclose(file);
}

int right() {
    FILE *file = fopen("steer.log", "a");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Writing formatted date and time to the file
    fprintf(file, "NO ACTION, IM TURNING RIGHT\n");

    // Closing the file
    fclose(file); 

    return 4;
}

int left() {
    // Opening file for writing
    FILE *file = fopen("steer.log", "a");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Writing formatted date and time to the file
    fprintf(file, "NO ACTION, IM TURNING LEFT\n");

    // Closing the file
    fclose(file);

    return 4;
}