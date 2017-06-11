#pragma once

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
void fileHMapApplyToAllMembers(FileHMap* map, void (*f) (File*));

NodeHMap* nodeHMapCreate(); 
void nodeHMapAdd(Node *node, NodeHMap *map); 
Node* nodeHMapFind(char *string, NodeHMap *map); 
int nodeHMapRemove(Node *node, NodeHMap *map); 
void nodeHMapDestroy(NodeHMap* map);
NodeList* nodeHMapGetFront(NodeHMap* map); 
void nodeHMapApplyToAllMembers(NodeHMap* map, void (*f) (Node*));
void nodeHMapApplyToAllMembersString(NodeHMap* map, void (*f) (Node*, char*), char* s);
