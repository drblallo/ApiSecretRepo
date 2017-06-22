#include "fileReader.h"
#include <stdlib.h>
#define BUFFER_SIZE 128
#include <unistd.h>
#include <stdio.h>

char buffer[BUFFER_SIZE];

struct flr
{
	unsigned targetChar;
	char lastOne;
};

void fileReaderUpdate(FileReader* fr)
{
	fr->lastOne = buffer[BUFFER_SIZE-1];
	int t = read(0, buffer,BUFFER_SIZE);
	fr->targetChar = 0;
	if (t < BUFFER_SIZE)
		buffer[t] = '|';
}

FileReader* fileReaderCreate()
{
	FileReader* fr = (FileReader*)malloc(sizeof(FileReader));

	int t = read(0, buffer, BUFFER_SIZE);
	fr->targetChar = 0;
	if (t < BUFFER_SIZE)
		buffer[t] = '|';
	fr->lastOne = '\n';
	return fr;
}


char fileReaderGetChar(FileReader* fr)
{
	if (!fr)
		return '|';

	char toBeReturned = buffer[fr->targetChar];
	fr->targetChar++;
	
	if (fr->targetChar >= BUFFER_SIZE)
		fileReaderUpdate(fr);

	return toBeReturned;	
}

char fileReaderPeek(FileReader* fr)
{
	if (!fr)
		return '|';
	return buffer[fr->targetChar];
}

void fileReaderDestroy(FileReader* fr)
{
	if (!fr)
		return;
	free (fr);
}

char fileReaderGetPrev(FileReader* fr)
{
	if (!fr)
		return '|';
	if (fr->targetChar == 0)
	{
		return fr->lastOne;
	}
	return buffer[fr->targetChar-1];
}
