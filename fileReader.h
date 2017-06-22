#pragma once

typedef struct flr FileReader;
char fileReaderGetChar(FileReader* fr);
FileReader* fileReaderCreate();
char fileReaderPeek(FileReader* fr);
void fileReaderDestroy(FileReader* fr);
char fileReaderGetPrev(FileReader* fr);
