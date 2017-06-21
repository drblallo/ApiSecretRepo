#pragma once
#define MAX_TREE_DEPTH 255
#define MAX_TREE_CHILD_COUNT 1023

typedef struct nd Node;
typedef struct ndhmp NodeHMap;
typedef struct flhmp FileHMap;
typedef struct fl File;
typedef struct nd Node;

Node* nodeCreate(char* name); 
int nodeAddNodeChild(Node* node, Node* child); 
int nodeAddFileChild(Node* node, File* child); 
int nodeRemoveNodeChild(Node* child); 
void nodeRecursiveRemove(Node* node, Node* parent); 
void nodeRemoveFileChild(Node* node, File* child); 
void nodeDestroy(Node* node); 
void nodeFindAndPrint(Node* n, char* name, int* printedOk); 
char* nodeGetName(Node* n); 
Node* nodeGetNodeChildren(char* name, Node* parent); 
File* nodeGetFileChildren(char* name, Node* parent); 
int nodeGetDepth(Node* n); 
Node* nodeGetParent(Node* n); 
void nodePrintTree(Node* n); 
void nodePrintPath(Node* n, int last); 
