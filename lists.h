#pragma once

typedef struct ndlst NodeList; 
typedef struct fllst FileList;

typedef struct fl File;
typedef struct nd Node;

FileList* fileListCreate(File *file); 
void fileListInsert(File *file, FileList *ls); 
FileList* fileListFindByName(char* name, FileList *fl); 
FileList* fileListFind(File* file, FileList* fl);
int fileListDestroy(FileList* fl); 
void fileListRecursiveDelete(FileList* file); 
File* fileListGetFile(FileList* file); 
FileList* fileListGetNext(FileList* file); 

NodeList* nodeListCreate(Node *node); 
void nodeListInsert(Node *node, NodeList *ls);
NodeList* nodeListFindByName(char* name, NodeList* ls); 
NodeList* nodeListFind(Node* n, NodeList* ls);
int nodeListDestroy(NodeList* node); 
void nodeListRecursiveDelete(NodeList* node); 
Node* nodeListGetNode(NodeList* file); 
NodeList* nodeListGetNext(NodeList* file); 
