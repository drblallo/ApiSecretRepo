#include "node.h"
#include <stdlib.h>
#include "file.h"
#include <string.h>
#include "lists.h"
#include "hmaps.h"
#include <stdio.h>

struct nd
{
	char *name;
	NodeHMap *nodeChildren;
	FileHMap *fileChildren;
	unsigned childCount;
	unsigned depth;
	struct nd *parent;
};

//allocates a node
Node* nodeCreate(char* nome)
{
	Node* n = (Node*)malloc(sizeof(Node));
	n->childCount = 0;
	n->depth = 0;
	n->parent = NULL;
	n->nodeChildren = nodeHMapCreate();
	n->fileChildren = fileHMapCreate();
	n->name = (char*)malloc(sizeof(char)*(strlen(nome)+1));
	strcpy(n->name, nome);
	return n;
}

//deletes a node, it does not check if other nodes are child of this one
void nodeDestroy(Node* node)
{
	if (!node)
		return;

	fileHMapDestroy(node->fileChildren);
	nodeHMapDestroy(node->nodeChildren);
	free (node->name);
	free (node);
}

//set a node as child of this node, retuns 1 if the operation has success,
//returns 0 if it fails. it may fail beucase of the 2 is null, because the parent is full
//or the child already has a parent or if the tree is too deep
int nodeAddNodeChild(Node* node, Node* child)
{
	if (!node || !child)
		return 0;

	if (node->depth >= 255)
		return 0;

	if (node->childCount >= 1024)
		return 0;

	if (child->parent)
		return 0;

	nodeHMapAdd(child, node->nodeChildren);
	child->depth = node->depth + 1;
	child->parent = node;
	node->childCount++;
	return 1;
}

//set a file as a children of a node, retuns 1 if the operation is compleated, 0 otherwhise
//it may fail if the parent is full, or if the tree is too deep
int nodeAddFileChild(Node* node, File* child)
{
	if (!node || !child)
		return 0;

	if (node->depth >= MAX_TREE_DEPTH)
		return 0;

	if (node->childCount >= MAX_TREE_CHILD_COUNT)
		return 0;

	fileHMapAdd(child, node->fileChildren);
	node->childCount++;
	
	return 1;
}

//delete a node from the tree, the node must be empty and it must have a parent
//else it does nothing
int nodeRemoveNodeChild(Node* child)
{
	Node *node = child->parent;
	if (!node || !child || child->childCount != 0)
		return 0;

	if (nodeHMapRemove(child, node->nodeChildren))
	{
		node->childCount--;
		nodeDestroy(child);
		return 1;
	}
	return 0;
}

//remove a file from a node
void nodeRemoveFileChild(Node* node, File* child)
{
	if (!child)
		return;
	if (!node)
		return;

	if (fileHMapRemove(child, node->fileChildren))
	{
		node->childCount--;
		fileDestroy(child);
	}
}

//removes all the children of a node, then deletes the node
void nodeRecursiveRemove(Node* node, Node* parent)
{
	if (!node)
		return;

	FileList *fl;
	NodeList *nl;

	while ((fl = fileHMapGetFront(node->fileChildren)))
		nodeRemoveFileChild(node, fileListGetFile(fl));

	while ((nl = nodeHMapGetFront(node->nodeChildren)))
		nodeRecursiveRemove(nodeListGetNode(nl), node);

	nodeRemoveNodeChild(node);
}

//returns a pointer to the node name
char* nodeGetName(Node* node)
{
	if (!node)
		return NULL;

	return node->name;
}

//return a node child if present, else return NULL
Node* nodeGetNodeChildren(char* name, Node* parent)
{
	if (!parent)
		return NULL;

	return nodeHMapFind(name, parent->nodeChildren);
}

//return a file child if present, else return NULL
File* nodeGetFileChildren(char* name, Node* parent)
{
	if (!parent)
		return NULL;

	return fileHMapFind(name, parent->fileChildren);
}

//returns the depth of the provided node
int nodeGetDepth(Node* n)
{
	if (!n)
		return 0;

	return n->depth;
}

//returns the node parent of the provided node
Node* nodeGetParent(Node* n)
{
	if (!n)
		return NULL;

	return n->parent;
}

void nodePrintFileName(File* f)
{
	int b;
	for (b = 0; b < fileGetDepth(f); b++)
		putchar('-');

	printf("%s\n", fileGetName(f));
}

//prints the structure of the node and of his childrens
void nodePrintTree(Node* root)
{
	int b = 0;

	for (b = 0; b < nodeGetDepth(root); b++)
		putchar('-');

	printf("%s\n", nodeGetName(root));	

	fileHMapApplyToAllMembers(root->fileChildren, nodePrintFileName);
	nodeHMapApplyToAllMembers(root->nodeChildren, nodePrintTree);
}

void nodePrintPath(Node* n, int last)
{
	if (!n)
		return;

	if (nodeGetParent(n))
		nodePrintPath(nodeGetParent(n), 0);

	if (last || nodeGetName(n)[0] == '/')
		printf("%s", nodeGetName(n));
	else
		printf("%s/", nodeGetName(n));
}

void nodeFindAndPrint(Node* n, char* name, int* printedOk)
{
	if (!n)
		return;
	File *file;

	nodeHMapApplyToAllMembersString(n->nodeChildren, nodeFindAndPrint, name, printedOk);

	file = nodeGetFileChildren(name, n);
	if (strcmp(name, nodeGetName(n)) == 0)
	{
		*printedOk = 1;
		printf("ok ");
		nodePrintPath(n, 1);		
		printf("\n");
	}
	if (file)
	{
		*printedOk = 1;
		printf("ok ");
		nodePrintPath(n, 0);
		printf("%s", fileGetName(file));
		printf("\n");
	}
}
