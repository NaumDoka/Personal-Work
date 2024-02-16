#include <stdio.h>
#include <time.h>
#include "throttleControl.h"

int initThrottle(){
    FILE *file = fopen("throttle.log", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Writing formatted date and time to the file
    fprintf(file, "Start\n");
    fclose(file);
}

int increase5() {
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
    FILE *file = fopen("throttle.log", "a");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Writing formatted date and time to the file
    fprintf(file, "%s, INCREASE 5(INCREMENTO 5, in the code)\n", dateTimeStr);

    // Closing the file
    fclose(file);

    return 5;
}
