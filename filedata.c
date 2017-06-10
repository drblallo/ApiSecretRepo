#include "filedata.h"
#include <stdlib.h>
#include <stdio.h>

struct fldt
{
	char data[FILE_DATA_SIZE];
	struct fldt* next;
	int count;
};

//create a file data
//the base lenght is set to 0
//next is set to NULL
FileData* createFileData()
{
	FileData* d = (FileData*)malloc(sizeof(FileData));
	d->next = NULL;
	d->data[0] = '\0';
	d->count = 0;
	return d;
}

//deallocate all the data in the list
void destoyFileData(FileData *data)
{
	if (data)
	{
		destoyFileData(data->next);
		free(data);
	}
}

//write a single character in to a file
void writeCharToFileData(FileData *data, char dataToWrite)
{
	while (data->count >= FILE_DATA_SIZE)
	{
		if (!data->next)
			data->next = createFileData();
		data = data->next;
	}

	data->data[data->count] = dataToWrite;
	data->count++;

	if (data->count >= FILE_DATA_SIZE)
		data->next = createFileData();
	else
		data->data[data->count] = '\0';
}


//copies the string inside the provided file data
//if it overflows it automatically creates another one and starts writing in the next
void writeFileData(FileData *data, char* dataToWrite)
{
	unsigned s = 0;
	if (!data)
		return;
	if (data->count >= FILE_DATA_SIZE) //if it's full write in the next one
	{
		if (!data->next)
			data->next = createFileData();
		writeFileData(data->next, dataToWrite);
		return;
	}

	while (dataToWrite[s] != '\0') //while it's not the end of the string
	{
		writeCharToFileData(data, dataToWrite[s]);
		s++;	
	}	
}

//it prints the entire content of the file
void printFileData(FileData *data)
{
	int a;
	for (a = 0; a < data->count; a++)
	{
		putchar(data->data[a]);
	}	
	if (data->next)
		printFileData(data->next);
}

FileData* getNextFileData(FileData *data)
{
	if (!data)
		return NULL;
	return data->next;
}
