#pragma once
#define MAX_BUFFER_SIZE 257
#define MAX_COMMAND_SIZE 15
#include <stdio.h>

#define QUIET

#ifndef QUIET 
#define WRITE(text) printf(text); putchar('\n')
#else
#define WRITE(text) do {} while(0)
#endif

#ifndef QUIET 
#define WRITE_L(text) printf(text)
#else
#define WRITE_L(text) do {} while(0)
#endif

#ifndef QUIET 
#define WRITE_S(text) printf("%s\n", text) 
#else
#define WRITE_S(text) do {} while(0)
#endif

#ifndef QUIET 
#define WRITE_I(text) printf("%i", text) 
#else
#define WRITE_I(text) do {} while(0)
#endif

typedef enum 
{
	//DIR_EXIST, 
	FILE_EXIST, 
	DIR_EXIST, 
	//DIR_PARENT_EXIST, 
	FILE_PARENT_EXIST, 
	BROKEN_PATH
} locateResult;

typedef enum 
{
	DIR_NAME, 
	FILE_OR_DIR_NAME, 
	ZERO_LENGHT_NAME, 
	BROKEN_NAME
} nextStringResult;

typedef struct flr FileReader;
typedef struct nd Node;

int endLineReached;

int FSCreateFile(FileReader* f, Node *root, char* buffer); 
int FSCreateDir(FileReader* f, Node *root, char* buffer); 
void FSPrintTree(Node* root); 
void FSRead(FileReader* f, Node *root, char* buffer); 
void FSWrite(FileReader* f, Node *root, char* buffer); 
void FSDelete(FileReader* f, Node *root, char* buffer); 
void FSDeleteRecursive(FileReader* f, Node *root, char* buffer);
void FSDeleteRoot(Node *root); 
void FSFind(FileReader* f, Node *root, char* buffer); 
