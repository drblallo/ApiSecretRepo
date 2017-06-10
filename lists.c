#include "lists.h"
#include "file.h"
#include "node.h"
#include <stdlib.h>
#include <string.h>

struct ndlst
{
	Node *node;
	struct ndlst *next;
	struct ndlst *prev;
}; 

struct fllst
{
	File *file;
	struct fllst *next;	
	struct fllst *prev;
};

//allocates a element of a file list
FileList* createFileList(File* file)
{
	FileList* f = (FileList*)malloc(sizeof(FileList));
	f->file = file;
	f->next = NULL;
	f->prev = NULL;
	return f;
}

//delete the list element but note the underlying file
int deleteFileList(FileList* fl)
{
	if (fl)
	{
		if (fl->prev)
		{
			fl->prev->next = fl->next;
		}
		if (fl->next)
			fl->next->prev = fl->prev;	
		free (fl);
		return 1;
	}
	return 0;
}

//insert a new element in front of the provided one
void insertInFileList(File *file, FileList* ls)
{
	if (!ls)
		return;
	FileList* f = createFileList(file);
	if (ls->prev)
	{
		ls->prev->next = f;
		f->prev = ls->prev;
	}
	ls->prev = f;
	f->next = ls;
}

//returns the FileList if it finds a file with such name, NULL otherwhise
FileList* findFileList(char* name, FileList *fl)
{
	if (strcmp(name, getFileName(fl->file)) == 0)
		return fl;

	if (fl->next)
		return findFileList(name, fl->next);

	return NULL;
}

//delete all the elements from this element to the end of the list
void recursiveDeleteFileList(FileList* file)
{
	if (file)
	{
		if (file->next)
			recursiveDeleteFileList(file->next);
		deleteFileList(file);
	}	
}

//creats a list element pointing to the node
NodeList* createNodeList(Node* node)
{
	NodeList* n = (NodeList*)malloc(sizeof(NodeList));
	n->next = NULL;
	n->prev = NULL;
	n->node = node;
	return n;
}

//deallocate the list element, but not the underlying node
int deleteNodeList(NodeList* node)
{
	if (!node)
		return 0;
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	free(node);

	return 1;
}

//insert a new list element in front of the provided one
void insertInNodeList(Node* node, NodeList *ls)
{
	NodeList* n = createNodeList(node);
	if (ls->prev)
	{
		ls->prev->next = n;
		n->prev = ls->prev;
	}

	if (ls->next)
	{
		n->next = ls;
		ls->prev = n;
	}
}

//search for a node with the provided name in the list and returns it, NULL otherwhise
NodeList* findNodeList(char* name, NodeList* ls)
{
	if (!ls)
		return NULL;

	if (strcmp(name, getNodeName(ls->node)) == 0)
		return ls;

	if (ls->next)
	{
		return findNodeList(name, ls->next);
	}

	return NULL;
}

//deletes all the elements from this to the end of the list
void recursiveDeleteNodeList(NodeList* file)
{
	if (file)
	{
		if (file->next)
			recursiveDeleteNodeList(file->next);
		deleteNodeList(file);
	}	
}

Node* getNodeFromList(NodeList* file)
{
	if (!file)
		return NULL;

	return file->node;
}

File* getFileFromList(FileList* file)
{
	if (!file)
		return NULL;

	return file->file;
}

NodeList* getNextNodeList(NodeList* file)
{
	if (!file)
		return NULL;
	return file->next;
}

FileList* getNextFileList(FileList* file)
{
	if (!file)
		return NULL;
	return file->next;
}
