#include "dataStructures.h"
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "node.h"
#include "fileReader.h"

//returns the next word in the buffer,
//a word is delimitated by \n, \0, /, and spaces
//if it ends with a /, the slash is carried over in to the word
nextStringResult getNextString(char* buffer, FileReader* f)
{
	int index = 0;
	char c = '0';
	nextStringResult toReturn = FILE_OR_DIR_NAME;

	while ((c = fileReaderGetChar(f)) != '\n' && c != '/' && c != '\0' && c != ' ')
	{
		buffer[index] = c;
		index++;

		if (index >= MAX_BUFFER_SIZE - 2)
		{
			WRITE("NEXT_STRING: buffer overflow, discarded");
			buffer[0] = '\0';
			return BROKEN_NAME;
		}
	}

	if (index == 0)
	{
		WRITE("NEXT_STRING: next string has lenght 0, discarded");
		buffer[0] = '\0';
		return ZERO_LENGHT_NAME;
	}

	if (c == '/')
	{
		WRITE_L("NEXT_STRING: found a dir: ");
		toReturn = DIR_NAME;
	}
	else
	{
		WRITE_L("NEXT_STRING: found a file: ");
	}

	if (c == '\n')
		endLineReached = 1;

	buffer[index] = '\0';
	WRITE_S(buffer);
	return toReturn;
}



//return the the parernt dir if a path to a file is in the buffer
//out is equal to 1 if the buffer contains a name of a file that is inside the node
//
//out is equal to 0 if the path provided pointed to a existing dir
//out is equal to 2 if the path provided points to the parent of the non existing last dir
//whose name is left in the buffer 
//out is equal to -1 if the path was broken
//out is equal to 3 if the file is not inside the node
Node* locateRecursive(Node* source, FileReader* f, char* buffer, locateResult* out)
{
	Node *m = source;
	nextStringResult result = BROKEN_NAME;
	
	do 
	{
		if ((result = getNextString(buffer, f)) == BROKEN_NAME || result == ZERO_LENGHT_NAME)
		{
			WRITE("LOCATE: path somehow broken, aborting");
			*out = BROKEN_PATH;
			return NULL;
		}

		/*if (result == FINAL_DIR_NAME)
		{
			supp = nodeGetNodeChildren(buffer, m);
			if (supp)
			{
				WRITE_L("LOCATE: found dir all the way down: ");
				WRITE_S(nodeGetName(m));
				*out = DIR_EXIST;
				m = supp;	 
			}			
			else
			{
				WRITE_L("LOCATE: found dir parent: ");
				WRITE_S(nodeGetName(m));
				*out = DIR_PARENT_EXIST;
			}
			return m;
		}*/

		if (result == FILE_OR_DIR_NAME)
		{
			if (nodeGetFileChildren(buffer, m))
			{
				WRITE_L("LOCATE: found parent dir of existing file: ");
				WRITE_S(nodeGetName(m));
				*out = FILE_EXIST;
				return m;
			}
			if (nodeGetNodeChildren(buffer, m))
			{
				WRITE_L("LOCATE: dir of existing dir: ");
				WRITE_S(nodeGetName(m));
				*out = DIR_EXIST;
				return nodeGetNodeChildren(buffer, m);
			}
			WRITE_L("LOCATE: found parent dir of non existing file: ");
			WRITE_S(nodeGetName(m));
			*out = FILE_PARENT_EXIST;
			return m;
		}

		m = nodeGetNodeChildren(buffer, m);

	}while (m && !endLineReached);

	WRITE_L("LOCATE: something whent wrong: ");
	WRITE_S(buffer);
	*out = BROKEN_PATH;
	return NULL;
}

int FSCreateFile(FileReader* f, Node *root, char* buffer)
{

	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);
	File* file = NULL;
	if (out == BROKEN_PATH)
	{
		printf("no\n");
		WRITE("->CREATE: path does not point to a dir with a empty resource, aborting");
		return 0;
	}

	if (out == FILE_EXIST || out == DIR_EXIST)
	{
		WRITE("->CREATE: trying to create a file that already exist");
		printf("no\n");
		return 0;
	}

	if (strlen(buffer) == 0)
	{
		WRITE("->CREATE: trying to create a 0 lenght file name, aborting");
		printf("no\n");
		return 0;
	}

	file = fileCreate(buffer);
	nodeAddFileChild(n, file);
	printf("ok\n");

	WRITE("->CREATE: done");
	return 1;
}

int FSCreateDir(FileReader* f, Node* root, char* buffer)
{

	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);

	if (out == BROKEN_PATH)
	{
		WRITE("->CREATE_DIR: path does not point to a dir with a empty resource, aborting");
		printf("no\n");
		return 0;
	}
	if (out == FILE_EXIST || out == DIR_EXIST)
	{
		WRITE("->CREATE_DIR: dir or file already exist");
		printf("no\n");
		return 0;
	}

	int t = strlen(buffer);
	if (t == 0)
	{
		WRITE("->CREATE_DIR: trying to create a 0 lenght file name, aborting");
		printf("no\n");
		return 0;
	}
			
	nodeAddNodeChild(n, nodeCreate(buffer));
	WRITE("->CREATE_DIR: done");
	printf("ok\n");
	return 1;
}

//prints the structure of the tree
void FSPrintTree(Node* root)
{
	nodePrintTree(root);	
}


void FSRead(FileReader* f, Node *root, char* buffer)
{
	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);
	File* file;

	if (out != FILE_EXIST)
	{
		WRITE("->READ: path does not point to a file, aborting");
		printf("no\n");
		return;
	}

	printf("contenuto ");
	file = nodeGetFileChildren(buffer, n);
	WRITE("->READ: done");
	filePrint(file);
	putchar('\n');
}

void FSWrite(FileReader* f, Node *root, char* buffer)
{
	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);
	File* file = NULL;
	char c = 0;

	if (out != FILE_EXIST)
	{
		WRITE("->WRITE: path does not point to a file, aborting");
		printf("no\n");
		return;
	}

	file = nodeGetFileChildren(buffer, n);
	fileClear(file);

	while ((c = fileReaderGetChar(f)) != '\"')
		;

	int a = 0;
	while ((c = fileReaderGetChar(f)) != '\"' && c != '\n')
	{
		fileWriteChar(file, c);	
		a++;
	}

	if (c == '\n')
		endLineReached = 1;
 
	printf("ok %i\n", a);
	WRITE("->WRITE: done");
}


void FSDelete(FileReader* f, Node *root, char* buffer)
{
	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);
	File* file;
	if (out == BROKEN_PATH || out == FILE_PARENT_EXIST)
	{
		WRITE("->DELETE: path not pointing to a resource, aborting");
		printf("no\n");
		return;
	}

	if (out == DIR_EXIST)
	{
		if (n != root)
		{
			if (nodeRemoveNodeChild(n))
		{
			WRITE("->DELETE: done");
			printf("ok\n");
		}
		else
		{
			printf("no\n");
		}
	}
	else
	{
		WRITE("->DELETE: trying to delete root, aborting");
		printf("no\n");
	}
}
if (out == FILE_EXIST)
{
	WRITE("->DELETE: Trying to delete a file");
	file = nodeGetFileChildren(buffer, n);
	nodeRemoveFileChild(n, file);	
	WRITE("->DELETE: done");
	printf("ok\n");
}
}


void FSDeleteRecursive(FileReader* f, Node *root, char* buffer)
{
	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);
	File* file;

	if (out == FILE_EXIST)
	{
		file = nodeGetFileChildren(buffer, n);
		nodeRemoveFileChild(n, file);	
		WRITE("->DELETE_R: done");
		printf("no\n");
		return;
	}

	if (out != DIR_EXIST)
	{
		WRITE("->DELETE_R: target dir does not exist, aborting");
		printf("no\n");
		return;
	}

	if (n == root)
	{
		WRITE("->DELETE_R: trying to delete root, aborting");
		printf("no\n");
		return;
	}
	nodeRecursiveRemove(n, nodeGetParent(n));
	WRITE("->DELETE_R: done");
	printf("ok\n");
}

void FSDeleteRoot(Node *root)
{
	if (!root)
		return;	

	if (nodeGetParent(root))
		return;

	nodeRecursiveRemove(root, NULL);	
	nodeDestroy(root);
	WRITE("->DELETE_ROOT: done");
}

void FSFind(FileReader* f, Node *root, char* buffer)
{
	char c;
	int a = 1;
	buffer[0] = fileReaderGetPrev(f);
	while ((c = fileReaderGetChar(f)) != '\n' && c != ' ')
	{
		buffer[a] = c;	
		a++;
	}
	if (c == '\n')
		endLineReached = 1;
	buffer[a] = '\0';
	WRITE("Searching for:");
	WRITE_S(buffer);
	int printedOk = 0;
	nodeFindAndPrint(root, buffer, &printedOk);
	if (!printedOk)
		printf("no\n");
}


