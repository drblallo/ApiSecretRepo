#include "dataStructures.h"
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "node.h"

int endLineReached = 0;


//returns the next word in the buffer,
//a word is delimitated by \n, \0, /, and spaces
//if it ends with a /, the slash is carried over in to the word
//return is equal to zero if the string point to a dir
//1 if it's a file
//-1 if something went wrong
//2 if word has lenght 0
nextStringResult getNextString(char* buffer, FILE* f)
{
	int index = 0;
	char c = '0';
	char supp;
	nextStringResult toReturn = FILE_NAME;

	while ((c = fgetc(f)) != '\n' && c != '/' && c != '\0' && c != ' ')
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
		buffer[index] = c;
		index++;
		toReturn = DIR_NAME;
	
		supp = fgetc(f);	
		if (supp == '\n' || supp == '\0' || supp == ' ')
		{
			toReturn = FINAL_DIR_NAME;
			WRITE_L("Final: ");
		}
		fseek(f, -1, SEEK_CUR);
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
Node* locateRecursive(Node* source, FILE* f, char* buffer, locateResult* out)
{
	Node *m = source;
	Node *supp;
	nextStringResult result = BROKEN_NAME;
	
	do 
	{
		if ((result = getNextString(buffer, f)) == BROKEN_NAME || result == ZERO_LENGHT_NAME)
		{
			WRITE("LOCATE: path somehow broken, aborting");
			*out = BROKEN_PATH;
			return NULL;
		}

		if (result == FINAL_DIR_NAME)
		{
			supp = getNodeChildren(buffer, m);
			if (supp)
			{
				WRITE_L("LOCATE: found dir all the way down: ");
				WRITE_S(getNodeName(m));
				*out = DIR_EXIST;
				m = supp;	 
			}			
			else
			{
				WRITE_L("LOCATE: found dir parent: ");
				WRITE_S(getNodeName(m));
				*out = DIR_PARENT_EXIST;
			}
			return m;
		}

		if (result == FILE_NAME)
		{
			if (getFileChildren(buffer, m))
			{
				WRITE_L("LOCATE: found parent dir of existing file: ");
				WRITE_S(getNodeName(m));
				*out = FILE_EXIST;
			}
			else
			{
				WRITE_L("LOCATE: found parent dir of non existing file: ");
				WRITE_S(getNodeName(m));
				*out = FILE_PARENT_EXIST;
			}
			return m;
		}

		m = getNodeChildren(buffer, m);

	}while (m && !endLineReached);

	WRITE_L("LOCATE: something whent wrong: ");
	WRITE_S(buffer);
	*out = BROKEN_PATH;
	return NULL;
}

int FSCreateFile(FILE* f, Node *root, char* buffer)
{

	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);
	File* file = NULL;

	if (out != FILE_PARENT_EXIST)
	{
		if (out == FILE_EXIST)
		{
			WRITE("->CREATE: trying to create a file that already exist");
		}
		else
		{
			WRITE("->CREATE: path does not point to a dir with a empty resource, aborting");
		}
		return 0;
	}
	if (strlen(buffer) == 0)
	{
		WRITE("->CREATE: trying to create a 0 lenght file name, aborting");
		return 0;
	}

	file = createFile(buffer);
	addNodeFileChild(n, file);

	WRITE("->CREATE: done");
	return 1;
}

int FSCreateDir(FILE* f, Node* root, char* buffer)
{

	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);

	if (out != DIR_PARENT_EXIST)
	{
		if (DIR_EXIST)
		{
			WRITE("->CREATE_DIR: dir already exist");

		}
		else
		{
			WRITE("->CREATE_DIR: path does not point to a dir with a empty resource, aborting");
		}
		return 0;
	}
	if (strlen(buffer) == 0)
	{
		WRITE("->CREATE_DIR: trying to create a 0 lenght file name, aborting");
		return 0;
	}
	addNodeNodeChild(n, createNode(buffer));
	WRITE("->CREATE_DIR: done");
	return 1;
}

//prints the structure of the tree
void FSPrintTree(Node* root)
{
	printTree(root);	
}


void FSRead(FILE* f, Node *root, char* buffer)
{
	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);
	File* file;

	if (out != FILE_EXIST)
	{
		WRITE("->READ: path does not point to a file, aborting");
		return;
	}

	file = getFileChildren(buffer, n);
	WRITE("->READ: done");
	printeFile(file);
	putchar('\n');
}

void FSWrite(FILE* f, Node *root, char* buffer)
{
	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);
	File* file = NULL;
	char c = 0;

	if (out != FILE_EXIST)
	{
		WRITE("->WRITE: path does not point to a file, aborting");
		return;
	}

	file = getFileChildren(buffer, n);

	while ((c = fgetc(f)) != '\"')
		;

	while ((c = fgetc(f)) != '\"' && c != '\n')
		writeFileChar(file, c);	

	if (c == '\n')
		endLineReached = 1;

	WRITE("->WRITE: done");
}


void FSDelete(FILE* f, Node *root, char* buffer)
{
	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);
	File* file;
	if (out != DIR_EXIST && out != FILE_EXIST)
	{
		WRITE("->DELETE: path not pointing to a resource, aborting");
		return;
	}

	if (out == DIR_EXIST)
	{
		if (n != root)
		{
			removeNodeNodeChild(n);
			WRITE("->DELETE: done");
		}
		else
		{
			WRITE("->DELETE: trying to delete root, aborting");
		}
	}
	else
	{
		file = getFileChildren(buffer, n);
		removeNodeFileChild(n, file);	
		WRITE("->DELETE: done");
	}
}


void FSDeleteRecursive(FILE* f, Node *root, char* buffer)
{
	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);
	File* file;

	if (out == FILE_EXIST)
	{
		file = getFileChildren(buffer, n);
		removeNodeFileChild(n, file);	
		WRITE("->DELETE_R: done");
		return;
	}

	if (out != DIR_EXIST)
	{
		WRITE("->DELETE_R: target dir does not exist, aborting");
		return;
	}

	if (n == root)
	{
		WRITE("->DELETE_R: trying to delete root, aborting");
		return;
	}
	recursiveRemoveNode(n, getNodeParent(n));
	WRITE("->DELETE_R: done");
}

void FSDeleteRoot(Node *root)
{
	if (!root)
		return;	

	if (getNodeParent(root))
		return;

	recursiveRemoveNode(root, NULL);	
	deleteNode(root);
	WRITE("->DELETE_ROOT: done");
}

void FSFind(FILE* f, Node *root, char* buffer)
{
	char c;
	int a = 0;
	fseek(f, -1, SEEK_CUR);
	while ((c = fgetc(f)) != '\n' && c != ' ')
	{
		buffer[a] = c;	
		a++;
	}
	if (c == '\n')
		endLineReached = 1;
	buffer[a] = '\0';

	findInNodeAndPrint(root, buffer);
}


