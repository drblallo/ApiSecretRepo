#include "dataStructures.h"
#include "stdlib.h"
#include <string.h>
#include <stdio.h>

File* createFile(char* nane)
{
	int l = strlen(nane);
	File *f = (File*)malloc(sizeof(File));
	f->name = (char*)malloc(sizeof(char) * l);
	strcpy(f->name, nane);	
	return f;
}

void destroyFile(File* file)
{
	if (file != NULL)
	{
		free(file->name);
		free(file);
		destoyFileData(file->data.next);
	}
}

FileData* createFileData()
{
	FileData* d = (FileData*)malloc(sizeof(FileData));
	d->next = NULL;
	d->data[0] = '\0';
	d->count = 1;
	return d;
}

void destoyFileData(FileData *data)
{
	if (data)
	{
		destoyFileData(data->next);
		free(data);
	}
}

void writeFileData(FileData *data, char* dataToWrite)
{
	unsigned s = 0;
	while (dataToWrite[s] != '\0')
	{
		if (data->count >= FILE_DATA_SIZE)
		{
			if (!data->next)
			{
				data->next = createFileData();
				writeFileData(data->next, &dataToWrite[s]);
				break;
			}	
			data->data[data->count] = dataToWrite[s];
			s++;
			data->count++;
		}
	}
}

void printFileData(FileData *data)
{
	int a;
	for (a = 0; a < data->count; a++)
	{
		putchar(data->data[a]);
	}	
	if (data->next)
		printFileData(data->next);
}

FileList* createFileList(File* file)
{
	FileList* f = (FileList*)malloc(sizeof(FileList));
	f->file = file;
	f->next = NULL;
	f->prev = NULL;
	return f;
}

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

void insertInFileList(File *file, FileList* ls)
{
	FileList* f = createFileList(file);
	if (ls->prev)
	{
		ls->prev->next = f;
		f->prev = ls->prev;
	}
	ls->prev = f;
	f->next = ls;
}

FileList* findFileList(char* name, FileList *fl)
{
	if (strcmp(name, fl->file->name))
		return fl;

	if (fl->next)
		return findFileList(name, fl->next);

	return NULL;
}

void recursiveDeleteFileList(FileList* file)
{
	if (file)
	{
		if (file->next)
			recursiveDeleteFileList(file->next);
		deleteFileList(file);
	}	
}

NodeList* createNodeList(Node* node)
{
	NodeList* n = (NodeList*)malloc(sizeof(NodeList));
	n->next = NULL;
	n->prev = NULL;
	n->node = node;
	return n;
}

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

NodeList* findNodeList(char* name, NodeList* ls)
{
	if (!ls)
		return NULL;

	if (strcmp(name, ls->node->name))
		return ls;

	if (ls->next)
	{
		return findNodeList(name, ls->next);
	}

	return NULL;
}

void recursiveDeleteNodeList(NodeList* file)
{
	if (file)
	{
		if (file->next)
			recursiveDeleteNodeList(file->next);
		deleteNodeList(file);
	}	
}

Node* createNode(char* nome)
{
	Node* n = (Node*)malloc(sizeof(Node));
	int a;
	n->childCount = 0;
	n->depth = 0;
	n->name = (char*)malloc(sizeof(char)*strlen(nome));
	strcpy(n->name, nome);
	for (a = 0; a < HMAP_SIZE; a++)
	{
		n->fileChildren.list[a] = NULL;
		n->nodeChildren.list[a] = NULL;
	}
	return n;
}

void deleteNode(Node* node)
{
	int a;
	if (node)
	{
		for (a = 0; a < HMAP_SIZE; a++)
		{
			if (node->nodeChildren.list[a])
				recursiveDeleteNodeList(node->nodeChildren.list[a]);
			if (node->fileChildren.list[a])
				recursiveDeleteFileList(node->fileChildren.list[a]);
		}	
		free (node);
	}
}

int addNodeNodeChild(Node* node, Node* child)
{
	if (node->depth >= 255)
		return 0;

	if (node->childCount >= 1024)
		return 0;

	insertInNodeList(child, node->nodeChildren.list[0]);
	child->depth = node->depth + 1;
	node->childCount++;
	return 1;
}

int addNodeFileChild(Node* node, File* child)
{
	if (node->depth >= 255)
		return 0;

	if (node->childCount >= 1024)
		return 0;

	insertInFileList(child, node->fileChildren.list[0]);
	node->childCount++;
	return 1;
}

void removeNodeNodeChild(Node* node, Node* child)
{
	if (child->childCount != 0 || !child)
		return;
	if (nodeHMapRemove(child, &(node->nodeChildren)))
	{
		node->childCount--;
		deleteNode(child);
	}
}

void removeFileNodeChild(Node* node, File* child)
{
	if (!child)
		return;
	if (fileHMapRemove(child, &(node->fileChildren)))
	{
		node->childCount--;
		destroyFile(child);
	}
}

void recursiveRemoveNode(Node* node, Node* parent)
{
	int a = 0;
	for (a = 0; a < HMAP_SIZE; a++)
	{
		recursiveDeleteFileList(node->fileChildren.list[a]);
		while (node->nodeChildren.list[a] != NULL)
			recursiveRemoveNode(node->nodeChildren.list[a]->node, node);
	}
	removeNodeNodeChild(parent, node);
}

short hash(char* name)
{
	int a = 0;
	short hash = 0;
	while (name[a] != '\0')
		hash += name[a];
	return hash % 32;
}

void fileHMapAdd(File *file, FileHMap *map)
{
	if (!map->list[(hash(file->name))])
		map->list[(hash(file->name))] = createFileList(file);
	insertInFileList(file, map->list[hash(file->name)]);	
}

void nodeHMapAdd(Node* file, NodeHMap* map)
{
	if (!map->list[(hash(file->name))])
		map->list[(hash(file->name))] = createNodeList(file);

	insertInNodeList(file, map->list[hash(file->name)]);
}

File* fileHMapFind(char *string, FileHMap* map)
{
	return findFileList(string, map->list[hash(string)])->file;
}

Node* nodeHMapFind(char *string, NodeHMap* map)
{
	return findNodeList(string, map->list[hash(string)])->node;
}

int fileHMapRemove(File* file, FileHMap *map)
{
	FileList *f = map->list[hash(file->name)];
	if (file == f->file)
		map->list[hash(file->name)] = f->next;
	return deleteFileList(findFileList(file->name, f));
}

int nodeHMapRemove(Node* file, NodeHMap *map)
{
	NodeList *f = map->list[hash(file->name)];
	if (file == f->node)
		map->list[hash(file->name)] = f->next;
	return deleteNodeList(findNodeList(file->name, f)); 
}

void writeFile(File* file, char* data)
{
	writeFileData(&(file->data), data);
}

void writeFileChar(File* file, char data)
{
	char c[2];
	c[0] = data;
	c[1] = '\0';
	writeFile(file, c);
}

void printeFile(File* file)
{
	printFileData(&file->data);
}
