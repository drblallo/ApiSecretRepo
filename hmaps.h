#pragma once
#define HMAP_SIZE 32

struct ndhmap;
typedef struct ndhmp NodeHMap;

struct flhmap;
typedef struct flhmp FileHMap;

typedef struct fl File;
typedef struct nd Node;

typedef struct fllst FileList;
typedef struct ndlst NodeList;

FileHMap* createFileHMap(); 
void fileHMapAdd(File *file, FileHMap *map); //DONE
File* fileHMapFind(char *string, FileHMap *map); //DONE
int fileHMapRemove(File *file, FileHMap *map); //DONE
void deleteFileHMap(FileHMap* map);
FileList* getHMapFileList(FileHMap* map, int target); //DONE

NodeHMap* createNodeHMap(); 
void nodeHMapAdd(Node *node, NodeHMap *map); //DONE
Node* nodeHMapFind(char *string, NodeHMap *map); //DONE
int nodeHMapRemove(Node *node, NodeHMap *map); //DONE
void deleteNodeHMap(NodeHMap* map);
NodeList* getHMapNodeList(NodeHMap* map, int target); //DONE
