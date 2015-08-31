#ifndef ProgramWorker_H
#define ProgramWorker_H

#include "Tlv_Worker.h"
#include "CoreDebug.h"
#include "swdStub.h"

/* swdStub instruction */
void loadEraseSectorInstruction(uint32_t *startSector, uint32_t *endSector);
void loadMassEraseInstruction(uint32_t bankSelect);
void loadCopyFromSRAMToFlashInstruction(uint32_t *dataAddress, uint32_t *destAddress, int size);

void writeTargetRegister(Tlv_Session *session, uint32_t *registerAddress, uint32_t *data);
void readTargetRegister(Tlv_Session *session, uint32_t *registerAddress);

#endif // ProgramWorker_H
