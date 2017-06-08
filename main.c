#include "dataStructures.h"
#include "string.h"

int getCommand(char* buffer, FILE* f)
{
	int a = 0;
	char c = '0';

	while ((c = fgetc(f)) != ' ' && c != '\n')
	{	
		buffer[a] = c;
		a++;
		if (a >= MAX_COMMAND_SIZE - 2)
			return 0;
	}
	buffer[a] = '\0';

	if (c == '\n')
	{
		endLineReached = 1;
		return 0;
	}
	
	while (fgetc(f) == ' ')
		;

	return 1;
		
}

void mainLoop(FILE* f, Node* root, int quietMode)
{
	char buffer[MAX_BUFFER_SIZE];
	char commandBuffer[MAX_COMMAND_SIZE];
		
	while (fgetc(f) != -1)
	{
		fseek(f, -1, SEEK_CUR);
		if (getCommand(commandBuffer, f))
		{
			if (!quietMode)
				printf("%s\n", commandBuffer);
			
			if (strcmp(commandBuffer, (char*)"create") == 0)
				FSCreateFile(f, root, buffer, quietMode);	

			if (strcmp(commandBuffer, (char*)"create_dir") == 0)
				FSCreateDir(f, root, buffer, quietMode);

			if (strcmp(commandBuffer, (char*)"read") == 0)
				FSRead(f, root, buffer, quietMode);

			if (strcmp(commandBuffer, (char*)"write") == 0)
				FSWrite(f, root, buffer, quietMode);

			if (strcmp(commandBuffer, (char*)"delete") == 0)
				FSDelete(f, root, buffer, quietMode);

			if (strcmp(commandBuffer, (char*)"delete_r") == 0)
				FSDeleteRecursive(f, root, buffer, quietMode);

			if (strcmp(commandBuffer, (char*)"find") == 0)
				FSFind(f, root, buffer, quietMode);
		}
		if (!endLineReached)
			while (fgetc(f) != '\n')
				;
		endLineReached = 0;
	}
}

int main(int argc, char **argv)
{
	int quietMode = 1;
	FILE* f = fopen(argv[1], "r");
	Node* root = createNode((char*)"/");

	if (argc > 2 && argv[2][0] == 'a')
		quietMode = 0;

	mainLoop(f, root, quietMode);
	
	if (!quietMode)
		FSPrintTree(root);

	return 0;
}
