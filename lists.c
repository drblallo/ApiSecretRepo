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
FileList* fileListCreate(File* file)
{
	FileList* f = (FileList*)malloc(sizeof(FileList));
	f->file = file;
	f->next = NULL;
	f->prev = NULL;
	return f;
}

//delete the list element but note the underlying file
int fileListDestroy(FileList* fl)
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

//insert a new element in lexografical order
FileList* fileListInsert(File *file, FileList* ls)
{
	if (!ls)
		return ls;

	int s = strcmp(fileGetName(file), fileGetName(fileListGetFile(ls)));

	if (s == 0)
		return ls;

	if (s < 0)
	{
		FileList* f = fileListCreate(file);
		if (ls->prev)
		{
			ls->prev->next = f;
			f->prev = ls->prev;
		}
		ls->prev = f;
		f->next = ls;
		return f;
	}	
	else
	{
		if (ls->next)
			fileListInsert(file, ls->next);
		else
		{
			FileList* f = fileListCreate(file);
			f->prev = ls;
			ls->next = f;	
		}
		return ls;
	}
}

//returns the FileList pointing to a file with the same name, NULL if it's not found in the list
FileList* fileListFindByName(char* name, FileList *fl)
{
	if (!fl)
		return NULL;

	while (strcmp(name, fileGetName(fl->file)) != 0)
	{

		if (!fl->next)
			return NULL;
		
		fl = fl->next;
	}
	return fl;
}

//retuns the FileList pointing to the same file, NULL it it's not found in the list
FileList* fileListFind(File* f, FileList* fl)
{
	if (!fl)
		return NULL;

	while (fl->file != f)
	{
		if (!fl->next)
			return NULL;

		fl = fl->next;
	}

	return fl;
}

//delete all the elements from this element to the end of the list
void fileListRecursiveDelete(FileList* file)
{
	if (file)
	{
		if (file->next)
			fileListRecursiveDelete(file->next);
		fileListDestroy(file);
	}	
}

//returns the next element of the list, returns NULL if it's the last element
FileList* fileListGetNext(FileList* file)
{
	if (!file)
		return NULL;

	return file->next;
}

//returns the file this list member is pointing at
File* fileListGetFile(FileList* file)
{
	if (!file)
		return NULL;

	return file->file;
}

//creats a list element pointing to the node
NodeList* nodeListCreate(Node* node)
{
	NodeList* n = (NodeList*)malloc(sizeof(NodeList));
	n->next = NULL;
	n->prev = NULL;
	n->node = node;
	return n;
}

//deallocate the list element, but not the underlying node
int nodeListDestroy(NodeList* node)
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

//insert a new list element in lexografical order
NodeList* nodeListInsert(Node* node, NodeList *ls)
{
	if (!ls || !node)
		return ls;
	
	int t = strcmp(nodeGetName(node), nodeGetName(ls->node));
	if (t == 0)
		return ls;

	if (t < 0)
	{
		NodeList* n = nodeListCreate(node);
		if (ls->prev)
		{
			ls->prev->next = n;
			n->prev = ls->prev;
		}

		n->next = ls;
		ls->prev = n;
		return n;
	}
	else
	{
		if (ls->next)
			nodeListInsert(node, ls->next);
		else
		{
			NodeList* n = nodeListCreate(node);
			n->prev = ls;
			ls->next = n;
		}
		return ls;

	}
}

//search for a node with the provided name in the list and returns it, NULL otherwhise
NodeList* nodeListFindByName(char* name, NodeList* ls)
{
	if (!ls)
		return NULL;

	while (strcmp(name, nodeGetName(ls->node)) != 0)
	{
		if (!ls->next)
			return NULL;

		ls = ls->next;
	}
	return ls;
}

//search for a node in the list and returns it, NULL otherwhise
NodeList* nodeListFind(Node* n, NodeList* ls)
{
	if (!ls)
		return NULL;

	while (n != ls->node)
	{
		if (!ls->next)
			return NULL;

		ls = ls->next;
	}
	return ls;
}

//deletes all the elements from this to the end of the list
void nodeListRecursiveDelete(NodeList* file)
{
	if (file)
	{
		if (file->next)
			nodeListRecursiveDelete(file->next);
		nodeListDestroy(file);
	}	
}

//retuns the node inside a list element
Node* nodeListGetNode(NodeList* file)
{
	if (!file)
		return NULL;

	return file->node;
}

//returns the next element of the list
NodeList* nodeListGetNext(NodeList* file)
{
	if (!file)
		return NULL;
	return file->next;
}

