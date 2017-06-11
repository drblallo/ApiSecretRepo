#pragma once

typedef struct fldt FileData;

FileData* fileDataCreate();
void fileDataWrite(FileData *data, char* dataToWrite); 
void fileDataWriteChar(FileData *data, char dataToWrite); 
void fileDataPrint(FileData *data); 
void fileDataDestroy(FileData *data); 
