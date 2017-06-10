#include "node.h"
#include <stdlib.h>
#include <stdio.h>
#include "file.h"
#include <string.h>
#include "lists.h"
#include "hmaps.h"

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
Node* createNode(char* nome)
{
	Node* n = (Node*)malloc(sizeof(Node));
	n->childCount = 0;
	n->depth = 0;
	n->parent = NULL;
	n->nodeChildren = createNodeHMap();
	n->fileChildren = createFileHMap();
	n->name = (char*)malloc(sizeof(char)*(strlen(nome)+1));
	strcpy(n->name, nome);
	return n;
}

//deletes a node, it does not check if other nodes are child of this one
void deleteNode(Node* node)
{
	if (node)
	{
		deleteFileHMap(node->fileChildren);
		deleteNodeHMap(node->nodeChildren);
		free (node->name);
		free (node);
	}
}

//set a node as child of this node, retuns 1 if the operation has success,
//returns 0 if it fails. it may fail beucase of the 2 is null, because the parent is full
//or the child already has a parent or if the tree is too deep
int addNodeNodeChild(Node* node, Node* child)
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
int addNodeFileChild(Node* node, File* child)
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
void removeNodeNodeChild(Node* child)
{
	Node *node = child->parent;
	if (!node || !child || child->childCount != 0)
	{
		return;
	}
	if (nodeHMapRemove(child, node->nodeChildren))
	{
		node->childCount--;
		deleteNode(child);
	}
}

//remove a file from a node
void removeNodeFileChild(Node* node, File* child)
{
	if (!child)
		return;
	if (fileHMapRemove(child, node->fileChildren))
	{
		node->childCount--;
		destroyFile(child);
	}
}

//removes all the children of a node, then deletes the node
void recursiveRemoveNode(Node* node, Node* parent)
{
	if (!node)
		return;
	int a = 0;
	FileList *fl;
	NodeList *nl;
	for (a = 0; a < HMAP_SIZE; a++)
	{
		while ((fl = getHMapFileList(node->fileChildren, a)))
			removeNodeFileChild(node, getFileFromList(fl));

		while ((nl = getHMapNodeList(node->nodeChildren, a)))
			recursiveRemoveNode(getNodeFromList(nl), node);
	}
	removeNodeNodeChild(node);
}

char* getNodeName(Node* node)
{
	if (!node)
		return NULL;
	return node->name;
}

Node* getNodeChildren(char* name, Node* parent)
{
	if (!parent)
		return NULL;
	return nodeHMapFind(name, parent->nodeChildren);
}

File* getFileChildren(char* name, Node* parent)
{
	if (!parent)
		return NULL;
	return fileHMapFind(name, parent->fileChildren);
}

int getNodeDepth(Node* n)
{
	if (!n)
		return 0;
	return n->depth;
}

Node* getNodeParent(Node* n)
{
	if (!n)
		return NULL;
	return n->parent;
}

void printTree(Node* root)
{
	int a = 0; 
	int b = 0;

	for (b = 0; b < getNodeDepth(root); b++)
		putchar('-');
	printf("%s\n", getNodeName(root));	
	for (a = 0; a < HMAP_SIZE; a++)
	{
		FileList* f = getHMapFileList(root->fileChildren, a);
		while (f)
		{
			for (b = 0; b < getNodeDepth(root) + 1; b++)
				putchar('-');
			printf("%s\n", getFileName(getFileFromList(f)));
			f = getNextFileList(f);
		}	
	}
	for (a = 0; a < HMAP_SIZE; a++)
	{
		NodeList* f =  getHMapNodeList(root->nodeChildren, a);
		while (f)
		{
			printTree(getNodeFromList(f));
			f = getNextNodeList(f);
		}
	}

}

void printPath(Node* n)
{
	if (n)
	{
		if (getNodeParent(n))
			printPath(getNodeParent(n));
		printf("%s", getNodeName(n));
	}
}

void findInNodeAndPrint(Node* n, char* name)
{
	if (!n)
		return;

	int a;
	File *file;
	NodeList *node;

	for (a = 0; a < HMAP_SIZE; a++)
	{
		node = getHMapNodeList(n->nodeChildren, a);
		while (node)
		{
			findInNodeAndPrint(getNodeFromList(node), name);
			node = getNextNodeList(node);
		}
		
	}
	file = getFileChildren(name, n);
	if (strcmp(name, getNodeName(n)) == 0)
	{
		printPath(n);		
		printf("\n");
	}
	if (file)
	{
		printPath(n);
		printf("%s", getFileName(file));
		printf("\n");
	}
}
