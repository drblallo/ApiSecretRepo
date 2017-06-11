#pragma once
#define HMAP_SIZE 32

typedef struct ndhmp NodeHMap;
typedef struct flhmp FileHMap;

typedef struct fl File;
typedef struct nd Node;

typedef struct fllst FileList;
typedef struct ndlst NodeList;

FileHMap* fileHMapCreate(); 
void fileHMapAdd(File *file, FileHMap *map); 
File* fileHMapFind(char *string, FileHMap *map); 
int fileHMapRemove(File *file, FileHMap *map); 
void fileHMapDestroy(FileHMap* map);
FileList* fileHMapGetFront(FileHMap* map); 
FileList* fileHMapGetList(FileHMap* map, int list);

NodeHMap* nodeHMapCreate(); 
void nodeHMapAdd(Node *node, NodeHMap *map); 
Node* nodeHMapFind(char *string, NodeHMap *map); 
int nodeHMapRemove(Node *node, NodeHMap *map); 
void nodeHMapDestroy(NodeHMap* map);
NodeList* nodeHMapGetFront(NodeHMap* map); 
NodeList* nodeHMapGetList(NodeHMap* map, int list);
