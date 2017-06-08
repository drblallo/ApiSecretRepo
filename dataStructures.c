#include "dataStructures.h"
#include "stdlib.h"
#include <string.h>
#include <stdio.h>

int endLineReached = 0;

File* createFile(char* nane)
{
	int l = strlen(nane);
	File *f = (File*)malloc(sizeof(File));
	f->name = (char*)malloc(sizeof(char) * (l+1));
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
			writeFileData(data->next, &dataToWrite[s]);
			break;
		}
		data->data[data->count] = dataToWrite[s];
		s++;
		data->count++;
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
		destroyFile(fl->file);
		free (fl);
		return 1;
	}
	return 0;
}

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

FileList* findFileList(char* name, FileList *fl)
{
	if (strcmp(name, fl->file->name) == 0)
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

	if (strcmp(name, ls->node->name) == 0)
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

void deleteNode(Node* node)
{
	if (node)
	{
		free (node->name);
		free (node);
	}
}

int addNodeNodeChild(Node* node, Node* child)
{
	if (node->depth >= 255)
		return 0;

	if (node->childCount >= 1024)
		return 0;

	nodeHMapAdd(child, &node->nodeChildren);
	child->depth = node->depth + 1;
	child->parent = node;
	node->childCount++;
	return 1;
}

int addNodeFileChild(Node* node, File* child)
{
	if (!node)
		return 0;

	if (node->depth >= MAX_TREE_DEPTH)
		return 0;

	if (node->childCount >= MAX_TREE_CHILD_COUNT)
		return 0;

	fileHMapAdd(child, &node->fileChildren);
	node->childCount++;
	
	return 1;
}

void removeNodeNodeChild(Node* node, Node* child)
{
	if (!child || child->childCount != 0)
	{
		return;
	}
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
	}
}

void recursiveRemoveNode(Node* node, Node* parent)
{
	if (!node)
		return;
	int a = 0;
	for (a = 0; a < HMAP_SIZE; a++)
	{
		while (node->fileChildren.list[a])
			removeFileNodeChild(node, node->fileChildren.list[a]->file);
		while (node->nodeChildren.list[a])
			recursiveRemoveNode(node->nodeChildren.list[a]->node, node);
	}
	removeNodeNodeChild(parent, node);
}

short hash(char* name)
{
	int a = 0;
	short hash = 0;
	while (name[a] != '\0')
	{
		hash += name[a];
		a++;
	}
	return hash % HMAP_SIZE;
}

void fileHMapAdd(File *file, FileHMap *map)
{
	if (!map->list[(hash(file->name))])
		map->list[(hash(file->name))] = createFileList(file);
	else
		insertInFileList(file, map->list[hash(file->name)]);	
}

void nodeHMapAdd(Node* file, NodeHMap* map)
{
	if (!map->list[(hash(file->name))])
		map->list[(hash(file->name))] = createNodeList(file);
	else
		insertInNodeList(file, map->list[hash(file->name)]);
}

File* fileHMapFind(char *string, FileHMap* map)
{
	if (!map->list[hash(string)])
		return NULL;
	return findFileList(string, map->list[hash(string)])->file;
}

Node* nodeHMapFind(char *string, NodeHMap* map)
{
	if (!map->list[hash(string)])
		return NULL;
	return findNodeList(string, map->list[hash(string)])->node;
}

int fileHMapRemove(File* file, FileHMap *map)
{
	FileList *f = map->list[hash(file->name)];
	if (!f)
		return 0;
	if (file == f->file)
		map->list[hash(file->name)] = f->next;
	return deleteFileList(findFileList(file->name, f));
}

int nodeHMapRemove(Node* file, NodeHMap *map)
{
	NodeList *f =  map->list[hash(file->name)];

	if (!f)
		return 0;

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

int getNextString(char* buffer, FILE* f)
{
	int a = 0;
	char c = '0';

	while ((c = fgetc(f)) != '\n' && c != '/' && c != '\0' && c != ' ')
	{
		buffer[a] = c;
		a++;

		if (a >= MAX_BUFFER_SIZE - 2)
			return 0;

		
	}
	if (c == '/')
	{
		buffer[a] = c;
		a++;
	}
	if (c == '\n')
		endLineReached = 1;

	buffer[a] = '\0';
	return 1;
}

Node* locateInNode(Node* source, char* name)
{
	if (!source)
		return NULL;

	return nodeHMapFind(name, &source->nodeChildren);
}

Node* locateRecursive(Node* source, FILE* f, char* buffer, int quietMode)
{
	Node *m = source;
	Node *supp;
	char c;
	
	do 
	{
		if (!getNextString(buffer, f))
			return NULL;

		if (!quietMode)
			printf("#next target dir: %s\n", buffer);

		if (buffer[strlen(buffer)-1] != '/')
		{
			if (!quietMode)
				printf("#trying to move to a file\n");
			return m;
		}

		supp = locateInNode(m, buffer);

		if (supp)
		{
			m = supp;
			if (!quietMode)
				printf("#moved to dir: %s\n", m->name);
		}
		else
		{
			if (!quietMode)
				printf("#failed to locate dir: %s\n", m->name);

			if (((c = fgetc(f)) != '\n') && c != ' ')
			{
				fseek(f, -1, SEEK_CUR);
				return NULL;
			}
			else
			{
				fseek(f, -1, SEEK_CUR);
				return m;
			}
		}

	}while (supp != NULL && !endLineReached);

	return m;
}

int FSCreateFile(FILE* f, Node *root, char* buffer, int quietMode)
{
	if (!quietMode)
		printf(">Called Create File Command\n");

	Node* n = locateRecursive(root, f, buffer, quietMode);
	File* file = NULL;

	if (strlen(buffer) == 0)
		return 0;
	if (buffer[strlen(buffer)-1] == '/')
		return 0;

	if (n)
	{

		if (!quietMode)
			printf("triyng to create\n");
		file = createFile(buffer);

		if (!quietMode)
			printf("created File\n");

		addNodeFileChild(n, file);

		if (!quietMode)
			printf("added file to node\n");
		return 1;
	}
	return 0;	
}

int FSCreateDir(FILE* f, Node* root, char* buffer, int quietMode)
{
	if (!quietMode)
		printf(">Called Create Dir Command\n");

	Node* n = locateRecursive(root, f, buffer, quietMode);

	if (strlen(buffer) == 0)
		return 0;
	if (buffer[strlen(buffer)-1] != '/')
		return 0;

	if (n)
	{
		if (!quietMode)
			printf("adding dir called: %s\n", buffer);
		addNodeNodeChild(n, createNode(buffer));
		return 1;
	}
	return 0;
}

void FSPrintTree(Node* root)
{
	
	int a = 0; 
	int b = 0;

	for (b = 0; b < root->depth; b++)
		putchar('-');
	printf("%s\n", root->name);	
	for (a = 0; a < HMAP_SIZE; a++)
	{
		FileList* f = root->fileChildren.list[a];
		while (f)
		{
			for (b = 0; b < root->depth + 1; b++)
				putchar('-');
			printf("%s\n", f->file->name);
			f = f->next;
		}	
	}
	for (a = 0; a < HMAP_SIZE; a++)
	{
		NodeList* f = root->nodeChildren.list[a];
		while (f)
		{
			FSPrintTree(f->node);
			f = f->next;
		}
	}
}


void FSRead(FILE* f, Node *root, char* buffer, int quietMode)
{
	Node* n = locateRecursive(root, f, buffer, quietMode);
	File* file;
	if (n)
	{
		file = fileHMapFind(buffer, &n->fileChildren);
		if (file)
		{
			printeFile(file);
			putchar('\n');
		}
	}
}

void FSWrite(FILE* f, Node *root, char* buffer, int quietMode)
{
	Node* n = locateRecursive(root, f, buffer, quietMode);
	File* file = NULL;
	char c = 0;
	if (n)
	{
		if (!quietMode)
			printf("#Searching for file\n");
		file = fileHMapFind(buffer, &n->fileChildren);
		if (file)
		{
			if (!quietMode)
				printf("#writing\n");

			while ((c = fgetc(f)) != '\"')
				;
			while ((c = fgetc(f)) != '\"')
			{
				writeFileChar(file, c);	
			}
		}
	}
	else
	{
		if (!quietMode)
			printf("#no dir\n");
	}
}


void FSDelete(FILE* f, Node *root, char* buffer, int quietMode)
{
	Node* n = locateRecursive(root, f, buffer, quietMode);
	File* file;
	if (n)
	{
		if (strlen(buffer) == 0)
		{
			if (n != root)
				removeNodeNodeChild(n->parent, n);
		}
		else
		{
			file = fileHMapFind(buffer, &n->fileChildren);
			if (file)
				removeFileNodeChild(n, file);	
		}
	}
}


void FSDeleteRecursive(FILE* f, Node *root, char* buffer, int quietMode)
{
	Node* n = locateRecursive(root, f, buffer, quietMode);
	File* file;
	if (n)
	{
		if (strlen(buffer) == 0)
		{
			if (n != root)
				recursiveRemoveNode(n, n->parent);
		}
		else
		{
			file = fileHMapFind(buffer, &n->fileChildren);
			if (file)
				removeFileNodeChild(n, file);	
		}
	}
}


void FSFind(FILE* f, Node *root, char* buffer, int quietMode)
{
	char c;
	int a = 0;
	fseek(f, -1, SEEK_CUR);
	while ((c = fgetc(f)) != '\n' && c != ' ')
	{
		buffer[a] = c;	
		a++;
	}
	if (c == '\n')
		endLineReached = 1;
	buffer[a] = '\0';

	if (!quietMode)
		printf("%s\n", buffer);
	findInNodeAndPrint(root, buffer);
}

void printPath(Node* n)
{
	if (n)
	{
		if (n->parent)
			printPath(n->parent);
		printf("%s", n->name);
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
		node = n->nodeChildren.list[a];
		while (node)
		{
			findInNodeAndPrint(node->node, name);
			node = node->next;
		}
		
	}
	file = fileHMapFind(name, &n->fileChildren);
	if (strcmp(name, n->name) == 0)
	{
		printPath(n);		
		printf("\n");
	}
	if (file)
	{
		printPath(n);
		printf("%s", file->name);
		printf("\n");
	}
}
