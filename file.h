#pragma once

struct fl;
typedef struct fl File;

File* createFile(char* name); //DONE
void writeFile(File* file, char* data); //DONE
void writeFileChar(File* file, char character); //DONE
void printeFile(File* file); //DONE
void destroyFile(File* file); //DONE
char* getFileName(File* file); //DONE
