#pragma once

typedef struct fl File;

File* fileCreate(char* name); 
void fileWrite(File* file, char* data); 
void fileWriteChar(File* file, char character); 
void filePrint(File* file); 
void fileDestroy(File* file); 
char* fileGetName(File* file); 
void fileClearData(File* file);
void fileSetDeapth(File* f, int deapth);
int fileGetDepth(File* f);
void fileClear(File* f);
