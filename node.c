#include "node.h"
#include <stdlib.h>
#include "file.h"
#include <string.h>
#include "lists.h"

//allocates a node
Node* createNode(char* nome)
{
	Node* n = (Node*)malloc(sizeof(Node));
	int a;
	n->childCount = 0;
	n->depth = 0;
	n->parent = NULL;
	n->name = (char*)malloc(sizeof(char)*(strlen(nome)+1));
	strcpy(n->name, nome);
	for (a = 0; a < HMAP_SIZE; a++)
	{
		n->fileChildren.list[a] = NULL;
		n->nodeChildren.list[a] = NULL;
	}
	return n;
}

//deletes a node, it does not check if other nodes are child of this one
void deleteNode(Node* node)
{
	if (node)
	{
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

	nodeHMapAdd(child, &node->nodeChildren);
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


	fileHMapAdd(child, &node->fileChildren);
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
	if (nodeHMapRemove(child, &(node->nodeChildren)))
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
	if (fileHMapRemove(child, &(node->fileChildren)))
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
	for (a = 0; a < HMAP_SIZE; a++)
	{
		while (node->fileChildren.list[a])
			removeNodeFileChild(node, getFileFromList(node->fileChildren.list[a]));
		while (node->nodeChildren.list[a])
			recursiveRemoveNode(getNodeFromList(node->nodeChildren.list[a]), node);
	}
	removeNodeNodeChild(node);
}

char* getNodeName(Node* node)
{
	if (!node)
		return NULL;
	return node->name;
}
