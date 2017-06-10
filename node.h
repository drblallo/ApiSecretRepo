#pragma once
#define MAX_TREE_DEPTH 255
#define MAX_TREE_CHILD_COUNT 1023

struct nd;
typedef struct nd Node;
typedef struct ndhmp NodeHMap;
typedef struct flhmp FileHMap;
typedef struct fl File;
typedef struct nd Node;


Node* createNode(char* name); //DONE
int addNodeNodeChild(Node* node, Node* child); //DONE
int addNodeFileChild(Node* node, File* child); //DONE
void removeNodeNodeChild(Node* child); //DONE
void recursiveRemoveNode(Node* node, Node* parent); //DONE
void removeNodeFileChild(Node* node, File* child); //DONE
void deleteNode(Node* node); //DONE
void findInNodeAndPrint(Node* n, char* name); //DONE
char* getNodeName(Node* n); //DONE
Node* getNodeChildren(char* name, Node* parent); //DONE
File* getFileChildren(char* name, Node* parent); //DONE
int getNodeDepth(Node* n); //DONE
Node* getNodeParent(Node* n); //DONE
void printTree(Node* n); //DONE
void printPath(Node* n); //DONE
