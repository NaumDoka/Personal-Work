#include <stdio.h>
#include <time.h>
#include "brakeByWire.h"

int initBrake(){
    FILE *file = fopen("brake.log", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Writing formatted date and time to the file
    fprintf(file, "Start\n");
    fclose(file);
}

int brake5() {
    // Buffer to hold the formatted date and time
    char dateTimeStr[100];
    time_t currentTime;
    struct tm *localTime;

    // Getting current time
    time(&currentTime);
    localTime = localtime(&currentTime);

    // Formatting date and time
    strftime(dateTimeStr, sizeof(dateTimeStr), "%Y-%m-%d %H:%M:%S", localTime);
    // Opening file for writing
    FILE *file = fopen("brake.log", "a");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Writing formatted date and time to the file
    fprintf(file, "%s, BRAKE 5\n", dateTimeStr);

    // Closing the file
    fclose(file);

    return 5;
}

int stop(){
    // Buffer to hold the formatted date and time
    char dateTimeStr[100];
    time_t currentTime;
    struct tm *localTime;

    // Getting current time
    time(&currentTime);
    localTime = localtime(&currentTime);

    // Formatting date and time
    strftime(dateTimeStr, sizeof(dateTimeStr), "%Y-%m-%d %H:%M:%S", localTime);
    // Opening file for writing
    FILE *file = fopen("brake.log", "a");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Writing formatted date and time to the file
    fprintf(file, "%s, AUTO STOP\n", dateTimeStr);

    // Closing the file
    fclose(file);

    return 0;
}