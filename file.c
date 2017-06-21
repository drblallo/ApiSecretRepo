#include "file.h"
#include "filedata.h"
#include <string.h>
#include <stdlib.h>

typedef struct fl
{
	FileData *data;
	char *name;
	int depth;
} File;

//allocate a file with the privided name
//if the name has lenght 0, it returns NULL
//file data is set to \0
File* fileCreate(char* nane)
{
	int l = strlen(nane);
	if (l == 0)
		return NULL;
	File *f = (File*)malloc(sizeof(File));
	f->name = (char*)malloc(sizeof(char) * (l+1));
	strcpy(f->name, nane);	
	f->data =  fileDataCreate();
	f->depth = 0;
	return f;
}

//deallocate all data within the file
//deallocate the name
//deallocate the file
void fileDestroy(File* file)
{
	if (file)
	{
		if (file->data)
			fileDataDestroy(file->data);
		free(file->name);
		free(file);
	}
}


//returns a pointer to the string that contains the file name
char* fileGetName(File* file)
{
	if (!file)
		return NULL;
	return file->name;
}

//appends a string to the file
void fileWrite(File* file, char* data)
{
	fileDataWrite(file->data, data);
}

//appends a char to the file
void fileWriteChar(File* file, char data)
{
	fileDataWriteChar(file->data, data);
}

//prints the content of the file
void filePrint(File* file)
{
	fileDataPrint(file->data);
}

//removes the entire content of the file
void fileClearData(File* file)
{
	if (!file)
		return;

	fileDataDestroy(file->data);
	file->data = fileDataCreate();
}

//set the depth of a file
void fileSetDeapth(File* f, int deapth)
{
	if (!f)
		return;
	f->depth = deapth;
}

//returns the depth of a file
int fileGetDepth(File* f)
{
	if (!f)
		return -1;

	return f->depth;
}

void fileClear(File* f)
{
	if (!f)
		return;

	fileDataClear(f->data);
}
