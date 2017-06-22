#include "dataStructures.h"
#include "string.h"
#include "node.h"
#include "fileReader.h"

int commandCount = 0;
int getCommand(char* buffer, FileReader* f)
{
	int a = 0;
	char c = '0';
	WRITE("reading command");
	while ((c = fileReaderGetChar(f)) != ' ' && c != '\n' && c != '|')
	{	
		buffer[a] = c;
		a++;
		if (a >= MAX_COMMAND_SIZE - 2)
		{
			WRITE("A command was too long, aborted");
			return 0;
		}
	}
	buffer[a] = '\0';
	WRITE_S(buffer);
	WRITE_I(commandCount);
	WRITE(" COMMAND");
	commandCount++;

	if (c == '\n')
	{
		endLineReached = 1;
		return 0;
	}
	
	while (fileReaderGetChar(f) == ' ')
		;

	return 1;
		
}

void mainLoop(FileReader* f, Node* root)
{
	char buffer[MAX_BUFFER_SIZE];
	char commandBuffer[MAX_COMMAND_SIZE];
		
	buffer[0] = '\0';
	while (fileReaderPeek(f) != '|')
	{
		if (getCommand(commandBuffer, f))
		{

			if (strcmp(commandBuffer, (char*)"create") == 0)
				FSCreateFile(f, root, buffer);	

			if (strcmp(commandBuffer, (char*)"create_dir") == 0)
				FSCreateDir(f, root, buffer);

			if (strcmp(commandBuffer, (char*)"read") == 0)
				FSRead(f, root, buffer);

			if (strcmp(commandBuffer, (char*)"write") == 0)
				FSWrite(f, root, buffer);

			if (strcmp(commandBuffer, (char*)"delete") == 0)
				FSDelete(f, root, buffer);

			if (strcmp(commandBuffer, (char*)"delete_r") == 0)
				FSDeleteRecursive(f, root, buffer);

			if (strcmp(commandBuffer, (char*)"find") == 0)
				FSFind(f, root, buffer);

			if (strcmp(commandBuffer, (char*)"exit") == 0)
				return;
		}
		if (!endLineReached)
			while (fileReaderGetChar(f) != '\n')
				;
		endLineReached = 0;
		
	}
}

int main(int argc, char **argv)
{
	endLineReached = 0;
	WRITE("STARTING");
	Node* root = nodeCreate((char*)"/");
	FileReader* fr = fileReaderCreate();

	mainLoop(fr, root);
	
#ifndef QUIET
	FSPrintTree(root);
#endif

	fileReaderDestroy(fr);
	FSDeleteRoot(root);
	return 0;
}
