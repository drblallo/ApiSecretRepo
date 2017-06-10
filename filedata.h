#pragma once
#define FILE_DATA_SIZE 256

struct fldt;
typedef struct fldt FileData;

FileData* createFileData(); //DONE
void writeFileData(FileData *data, char* dataToWrite); //DONE
void writeCharToFileData(FileData *data, char dataToWrite); //DONE
void printFileData(FileData *data); //DONE
void destoyFileData(FileData *data); //DONE
FileData* getNextFileData(FileData *data); //DONE
