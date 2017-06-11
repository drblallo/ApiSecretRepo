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

typedef enum 
{
	DIR_EXIST, 
	FILE_EXIST, 
	DIR_PARENT_EXIST, 
	FILE_PARENT_EXIST, 
	BROKEN_PATH
} locateResult;

typedef enum 
{
	DIR_NAME, 
	FILE_NAME, 
	FINAL_DIR_NAME, 
	ZERO_LENGHT_NAME, 
	BROKEN_NAME
} nextStringResult;

typedef struct nd Node;

int endLineReached;

int FSCreateFile(FILE* f, Node *root, char* buffer); 
int FSCreateDir(FILE* f, Node *root, char* buffer); 
void FSPrintTree(Node* root); 
void FSRead(FILE* f, Node *root, char* buffer); 
void FSWrite(FILE* f, Node *root, char* buffer); 
void FSDelete(FILE* f, Node *root, char* buffer); 
void FSDeleteRecursive(FILE* f, Node *root, char* buffer);
void FSDeleteRoot(Node *root); 
void FSFind(FILE* f, Node *root, char* buffer); 
