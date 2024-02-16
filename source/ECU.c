#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h> 
#include <sys/time.h>
#include <time.h>
#include <sys/select.h>
#include <stdbool.h>
#include "ECU.h"
#include "HMI.h"
#include "frontCameraComponent.h"
#include "throttleControl.h"
#include "brakeByWire.h"
#include "steerByWire.h"
#include "forwardFacingRadar.h"
#include "parkAssist.h"

int run(int option){
    FILE *file;
    file = fopen("frontCamera.data", "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    FILE *ecu_file;
    ecu_file = fopen("ECU.log", "w");
    if (ecu_file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    fprintf(ecu_file, "Start\n");
    fclose(ecu_file);
    char *line;
    struct HMI hmi;
    initHMI(&hmi);
    bool started = false;
    bool parking = false;
    int parking_seconds = 0;

    time_t start, current;
    time(&start);

    int speed = 0;
    int steer_counter = 0;
    bool rightLeft = true;

    uint16_t park_assist_list[] = {0x172A, 0xD693, 0x0000, 0xBDD8, 0xFAEE, 0x4300};
    size_t listSize = sizeof(park_assist_list) / sizeof(park_assist_list[0]);

    while (1) {
        int hmi_input = HMILoop(&hmi);
        switch (hmi_input)
        {
            case 0:
                //writeToOutputTerminal("Hello\n");
                break;
            case 1: //Start
                started = true;
                speed = 0;
                initThrottle();
                initBrake();
                initFrontCameraComponent();
                initSteer();
                initForwardFacingRadar();
                initParkAssist();
                break;
            case 2: //PARK
                initParkAssist();
                parking = true;
                started = false;
                parking_seconds = 0;
                break;
            case 3: //STOP
                started = false;
                stop();
                speed = 0;
                break;
            default:
                break;
        }
        if(started){
            time(&current);

            if (difftime(current, start) >= 1) {
                if(steer_counter == 0){
                    // Reset the start time
                    unsigned char randomData[8];
                    readForwardFacingRadar(randomData, sizeof(randomData), option);
                    writeToOutputTerminal("FORWARD FACING RADAR COMPONENT\n");
                    logECU("FORWARD FACING RADAR COMPONENT\n");
                    line = readLineFromFile(file);
                    if (line == NULL) {
                        writeToOutputTerminal("End of file reached...\n");
                        fclose(file);
                    }
                    char *endptr; //Pointer to do the conversion from string to int
                    int desired_speed = strtol(line, &endptr, 10);
                    if(endptr == line){
                        if(strcmp(line, "PERICOLO\n") == 0){
                            speed = 0;
                            stop();
                        }
                        else if(strcmp(line, "DESTRA\n") == 0){
                            steer_counter = 1;
                            rightLeft = true;
                        }
                        else if(strcmp(line, "SINISTRA\n") == 0){
                            steer_counter = 1;
                            rightLeft = false;
                        }
                        else if(strcmp(line, "PARCHEGGIO\n") == 0){
                            started = false;
                            parking = true;
                            initParkAssist();
                            parking_seconds = 0;
                        }
                    } else{
                        if(desired_speed > speed)
                            while(desired_speed != speed){
                                increase5();
                                speed += 5;
                                writeToOutputTerminal("INCREASE 5 (INCREMENTO 5)\n");
                                logECU("INCREASE 5 (INCREMENTO 5)\n");
                            }
                        else if(desired_speed < speed)
                            while(desired_speed != speed){
                                brake5();
                                speed -= 5;
                                writeToOutputTerminal("BRAKE 5(FRENO 5)\n");
                                logECU("BRAKE 5(FRENO 5)\n");
                            }
                    }
                } else{
                    steer_counter++;
                    writeToOutputTerminal("NO ACTION, IM TURNING ");
                    logECU("NO ACTION, IM TURNING ");
                    if(rightLeft){
                        writeToOutputTerminal("RIGHT\n");
                        logECU("RIGHT\n");
                        right();
                    }
                    else{
                        writeToOutputTerminal("LEFT\n");
                        logECU("LEFT\n");
                        left();
                    }
                    steer_counter = (steer_counter == 5) ? 0 : steer_counter;
                }
                start = current;
            }
        }
        else if(parking){
            time(&current);
            if (difftime(current, start) >= 1) {
                while(0 != speed){
                    brake5();
                    speed -= 5;
                    writeToOutputTerminal("BRAKE 5(FRENO 5)\n");
                    logECU("BRAKE 5(FRENO 5)\n");
                }
                
                for(int i = 0; i < 8; i++){
                    uint16_t park_assit_data = readParkAssist(option);
                    if (isInList(park_assit_data, park_assist_list, listSize)){
                        parking_seconds = 0;
                    }
                    writeToOutputTerminal("PARK ASSIST COMMAND\n");
                    logECU("PARK ASSIST COMMAND\n");
                }
                parking_seconds++;
                if(parking_seconds == 30){
                    parking = false;
                    started = false;
                    writeToOutputTerminal("DONE\n");
                    logECU("DONE\n");
                    break;
                }
                start = current;
            }
        }
    }

    closeOutputTerminal();
    unlink(hmi.fifoPath);
    return 0;
}

int isInList(uint16_t value, const uint16_t *list, size_t listSize) {
    for (size_t i = 0; i < listSize; ++i) {
        if (list[i] == value) {
            return true;
        }
    }
    return false;
}

void logECU(const char *text){
    FILE *file = fopen("ECU.log", "a"); // Open in append mode
    if (file == NULL) {
        perror("Error opening ECU.log");
        return;
    }

    // Append the text to the file
    fprintf(file, "%s", text); // You can also use fputs if newline isn't needed

    // Close the file
    fclose(file);
}