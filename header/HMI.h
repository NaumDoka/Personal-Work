#ifndef HMI_H
#define HMI_H

struct HMI{
    char *fifoPath;
    struct timeval tv;
    fd_set readfds;
    char input[100];
    int stdin_fd;
};

void openOutputTerminal(const char *fifoPath);
void writeToOutputTerminal(const char *message);
void closeOutputTerminal();
void initHMI(struct HMI *hmi);
int HMILoop(struct HMI *hmi);

#endif
