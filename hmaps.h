#pragma once
#define HMAP_SIZE 32
#include "lists.h"

struct ndhmap;
typedef struct ndhmp NodeHMap;

struct flhmap;
typedef struct flhmp FileHMap;

typedef struct fl File;
typedef struct nd Node;

typedef struct ndhmp 
{
	NodeList *list[HMAP_SIZE];
} NodeHMap;

typedef struct flhmp
{
	FileList *list[HMAP_SIZE];	
} FileHMap;

void fileHMapAdd(File *file, FileHMap *map); //DONE
File* fileHMapFind(char *string, FileHMap *map); //DONE
int fileHMapRemove(File *file, FileHMap *map); //DONE

void nodeHMapAdd(Node *node, NodeHMap *map); //DONE
Node* nodeHMapFind(char *string, NodeHMap *map); //DONE
int nodeHMapRemove(Node *node, NodeHMap *map); //DONE
