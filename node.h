#pragma once
#include "hmaps.h"
#define MAX_TREE_DEPTH 255
#define MAX_TREE_CHILD_COUNT 1023

struct nd;
typedef struct nd Node;
struct nd
{
	char *name;
	NodeHMap nodeChildren;
	FileHMap fileChildren;
	unsigned childCount;
	unsigned depth;
	struct nd *parent;
};

Node* createNode(char* name); //DONE
int addNodeNodeChild(Node* node, Node* child); //DONE
int addNodeFileChild(Node* node, File* child); //DONE
void removeNodeNodeChild(Node* child); //DONE
void recursiveRemoveNode(Node* node, Node* parent); //DONE
void removeNodeFileChild(Node* node, File* child); //DONE
void deleteNode(Node* node); //DONE
void findInNodeAndPrint(Node* n, char* name); //DONE
char* getNodeName(Node* n); //DONE
