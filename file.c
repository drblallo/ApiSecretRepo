#include "file.h"
#include "filedata.h"
#include <string.h>
#include <stdlib.h>

typedef struct fl
{
	FileData *data;
	char *name;
} File;

//allocate a file with the privided name
//if the name has lenght 0, it returns NULL
//file data is set to \0
File* createFile(char* nane)
{
	int l = strlen(nane);
	if (l == 0)
		return NULL;
	File *f = (File*)malloc(sizeof(File));
	f->name = (char*)malloc(sizeof(char) * (l+1));
	strcpy(f->name, nane);	
	f->data =  createFileData();
	return f;
}

//deallocate all data within the file
//deallocate the name
//deallocate the file
void destroyFile(File* file)
{
	if (file)
	{
		if (file->data)
			destoyFileData(file->data);
		free(file->name);
		free(file);
	}
}


char* getFileName(File* file)
{
	if (!file)
		return NULL;
	return file->name;
}

void writeFile(File* file, char* data)
{
	writeFileData(file->data, data);
}

void writeFileChar(File* file, char data)
{
	writeCharToFileData(file->data, data);
}

void printeFile(File* file)
{
	printFileData(file->data);
}
