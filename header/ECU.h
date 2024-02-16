#ifndef ECU_H
#define ECU_H

#include <stdint.h> 


int run(int option);
int isInList(uint16_t value, const uint16_t *list, size_t listSize);
void logECU(const char *text);

#endif
