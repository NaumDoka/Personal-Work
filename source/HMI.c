#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <ctype.h>
#include "HMI.h"

static FILE *fifo = NULL;

void openOutputTerminal(const char *fifoPath) {
    char command[256];
    sprintf(command, "gnome-terminal -- sh -c 'cat %s'", fifoPath);
    system(command);
    fifo = fopen(fifoPath, "w");
    if (fifo == NULL) {
        perror("Error opening named pipe");
        exit(1);
    }
}

void writeToOutputTerminal(const char *message) {
    if (fifo == NULL) {
        fprintf(stderr, "Pipe not open for writing\n");
        return;
    }

    fprintf(fifo, "%s", message);
    fflush(fifo);
}

void closeOutputTerminal() {
    if (fifo != NULL) {
        fclose(fifo);
    }
}

void initHMI(struct HMI *hmi){
    hmi->fifoPath = "/tmp/myfifo";
    mkfifo(hmi->fifoPath, 0666);
    openOutputTerminal(hmi->fifoPath);
    hmi->stdin_fd = fileno(stdin);
}

int HMILoop(struct HMI *hmi){
    FD_ZERO(&hmi->readfds);
    FD_SET(hmi->stdin_fd, &hmi->readfds);
    // Set timeout to 1 second
    hmi->tv.tv_sec = 1;
    hmi->tv.tv_usec = 0;
    // Wait for input (stdin) or timeout
    if (select(hmi->stdin_fd + 1, &hmi->readfds, NULL, NULL, &hmi->tv) > 0) {
        if (FD_ISSET(hmi->stdin_fd, &hmi->readfds)) {
            if (fgets(hmi->input, 100, stdin)) {
                for (int i = 0; hmi->input[i] != '\0'; i++) { //Turn the string to upper by turning each case to upper
                    hmi->input[i] = toupper(hmi->input[i]);
                }
                writeToOutputTerminal(hmi->input);
                if (strcmp(hmi->input, "START\n") == 0) 
                    return 1;
                if (strcmp(hmi->input, "PARK\n") == 0)
                    return 2;
                if (strcmp(hmi->input, "STOP\n") == 0)
                    return 3;
            }
        }
    } else {
        // Timeout occurred, print "Hello"
        //writeToOutputTerminal("Hello\n");
        return 0;
    }
}