#ifndef MAIN_H_
#define MAIN_H_

#include "detours.h"

#define ADDRESS_BUFFER 0x85d3b60
#define ADDRESS_BUFFER_LENGTH 0x85d4368

#define ADDRESS_WRITE_PACKET 0x08264920
#define ADDRESS_READ_PACKET 0 // TODO

void writePacket(bool);
void readPacket();

#endif /* MAIN_H_ */
