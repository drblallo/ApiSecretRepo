#include "dataStructures.h"
#include "stdlib.h"
#include <string.h>
#include <stdio.h>

int endLineReached = 0;

//allocate a file with the privided name
//if the name has lenght 0, it returns NULL
//file data is set to \0
File* createFile(char* nane)
{
	int l = strlen(nane);
	if (l == 0)
		return NULL;
	File *f = (File*)malloc(sizeof(File));
	f->name = (char*)malloc(sizeof(char) * (l+1));
	strcpy(f->name, nane);	
	f->data.next = NULL;
	f->data.count = 0;
	f->data.data[0] = '\0';
	return f;
}

//deallocate all data within the file
//deallocate the name
//deallocate the file
void destroyFile(File* file)
{
	if (file)
	{
		if (file->data.next)
			destoyFileData(file->data.next);
		free(file->name);
		free(file);
	}
}

//create a file data
//the base lenght is set to 0
//next is set to NULL
FileData* createFileData()
{
	FileData* d = (FileData*)malloc(sizeof(FileData));
	d->next = NULL;
	d->data[0] = '\0';
	d->count = 0;
	return d;
}

//deallocate all the data in the list
void destoyFileData(FileData *data)
{
	if (data)
	{
		destoyFileData(data->next);
		free(data);
	}
}

//write a single character in to a file
void writeCharToFileData(FileData *data, char dataToWrite)
{
	while (data->count >= FILE_DATA_SIZE)
	{
		if (!data->next)
			data->next = createFileData();
		data = data->next;
	}

	data->data[data->count] = dataToWrite;
	data->count++;

	if (data->count >= FILE_DATA_SIZE)
		data->next = createFileData();
	else
		data->data[data->count] = '\0';
}


//copies the string inside the provided file data
//if it overflows it automatically creates another one and starts writing in the next
void writeFileData(FileData *data, char* dataToWrite)
{
	unsigned s = 0;
	if (!data)
		return;
	if (data->count >= FILE_DATA_SIZE) //if it's full write in the next one
	{
		if (!data->next)
			data->next = createFileData();
		writeFileData(data->next, dataToWrite);
		return;
	}

	while (dataToWrite[s] != '\0') //while it's not the end of the string
	{
		writeCharToFileData(data, dataToWrite[s]);
		s++;	
	}	
}

//it prints the entire content of the file
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
	if (strcmp(name, fl->file->name) == 0)
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

	if (strcmp(name, ls->node->name) == 0)
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

//removes all the children of a node, then deletes the node
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
	removeNodeNodeChild(node);
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

//add a file to a hash map
void fileHMapAdd(File *file, FileHMap *map)
{
	if (!map->list[(hash(file->name))])
		map->list[(hash(file->name))] = createFileList(file);
	else
		insertInFileList(file, map->list[hash(file->name)]);	
}

//add a node to a hash map
void nodeHMapAdd(Node* file, NodeHMap* map)
{
	if (!map->list[(hash(file->name))])
		map->list[(hash(file->name))] = createNodeList(file);
	else
		insertInNodeList(file, map->list[hash(file->name)]);
}

//return a file if it's inside a hash map, NULL otherwise
File* fileHMapFind(char *string, FileHMap* map)
{
	if (!map->list[hash(string)])
		return NULL;

	return findFileList(string, map->list[hash(string)])->file;
}

//return a node if it's inside a hash map, NULL otherwise
Node* nodeHMapFind(char *string, NodeHMap* map)
{
	if (!map->list[hash(string)])
		return NULL;

	return findNodeList(string, map->list[hash(string)])->node;
}

//removes a file from a hash map, does not delete the file
int fileHMapRemove(File* file, FileHMap *map)
{
	FileList *f = map->list[hash(file->name)];

	if (!f)
		return 0;

	if (file == f->file)
		map->list[hash(file->name)] = f->next;

	return deleteFileList(findFileList(file->name, f));
}

//removes a node from a hash map, does not delete the file
int nodeHMapRemove(Node* file, NodeHMap *map)
{
	if (!map || !file->name || !file)
		return 0;

	NodeList *f = map->list[hash(file->name)];

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
	writeCharToFileData(&file->data, data);
}

void printeFile(File* file)
{
	printFileData(&file->data);
}

//returns the word in the buffer,
//a word is delimitated by \n, \0, /, and spaces
//if it ends with a /, the slash is carried over in to the word
//return is equal to zero if the string point to a dir
//1 if it's a file
//-1 if something went wrong
//2 if word has lenght 0
nextStringResult getNextString(char* buffer, FILE* f)
{
	int index = 0;
	char c = '0';
	char supp;
	nextStringResult toReturn = FILE_NAME;

	while ((c = fgetc(f)) != '\n' && c != '/' && c != '\0' && c != ' ')
	{
		buffer[index] = c;
		index++;

		if (index >= MAX_BUFFER_SIZE - 2)
		{
			WRITE("NEXT_STRING: buffer overflow, discarded");
			buffer[0] = '\0';
			return BROKEN_NAME;
		}
	}

	if (index == 0)
	{
		WRITE("NEXT_STRING: next string has lenght 0, discarded");
		buffer[0] = '\0';
		return ZERO_LENGHT_NAME;
	}

	if (c == '/')
	{
		WRITE_L("NEXT_STRING: found a dir: ");
		buffer[index] = c;
		index++;
		toReturn = DIR_NAME;
	
		supp = fgetc(f);	
		if (supp == '\n' || supp == '\0' || supp == ' ')
		{
			toReturn = FINAL_DIR_NAME;
			WRITE_L("Final: ");
		}
		fseek(f, -1, SEEK_CUR);
	}
	else
	{
		WRITE_L("NEXT_STRING: found a file: ");
	}

	if (c == '\n')
		endLineReached = 1;

	buffer[index] = '\0';
	WRITE_S(buffer);
	return toReturn;
}

//return the the parernt dir if a path to a file is in the buffer
//out is equal to 1 if the buffer contains a name of a file that is inside the node
//
//out is equal to 0 if the path provided pointed to a existing dir
//out is equal to 2 if the path provided points to the parent of the non existing last dir
//whose name is left in the buffer 
//out is equal to -1 if the path was broken
//out is equal to 3 if the file is not inside the node
Node* locateRecursive(Node* source, FILE* f, char* buffer, locateResult* out)
{
	Node *m = source;
	Node *supp;
	nextStringResult result = BROKEN_NAME;
	
	do 
	{
		if ((result = getNextString(buffer, f)) == BROKEN_NAME || result == ZERO_LENGHT_NAME)
		{
			WRITE("LOCATE: path somehow broken, aborting");
			*out = BROKEN_PATH;
			return NULL;
		}

		if (result == FINAL_DIR_NAME)
		{
			supp = nodeHMapFind(buffer, &m->nodeChildren);
			if (supp)
			{
				WRITE_L("LOCATE: found dir all the way down: ");
				WRITE_S(m->name);
				*out = DIR_EXIST;
				m = supp;	 
			}			
			else
			{
				WRITE_L("LOCATE: found dir parent: ");
				WRITE_S(m->name);
				*out = DIR_PARENT_EXIST;
			}
			return m;
		}

		if (result == FILE_NAME)
		{
			if (fileHMapFind(buffer, &m->fileChildren))
			{
				WRITE_L("LOCATE: found parent dir of existing file: ");
				WRITE_S(m->name);
				*out = FILE_EXIST;
			}
			else
			{
				WRITE_L("LOCATE: found parent dir of non existing file: ");
				WRITE_S(m->name);
				*out = FILE_PARENT_EXIST;
			}
			return m;
		}

		m = nodeHMapFind(buffer, &m->nodeChildren);;

	}while (m && !endLineReached);

	WRITE_L("LOCATE: something whent wrong: ");
	WRITE_S(buffer);
	*out = BROKEN_PATH;
	return NULL;
}

int FSCreateFile(FILE* f, Node *root, char* buffer)
{

	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);
	File* file = NULL;

	if (out != FILE_PARENT_EXIST)
	{
		if (out == FILE_EXIST)
		{
			WRITE("->CREATE: trying to create a file that already exist");
		}
		else
		{
			WRITE("->CREATE: path does not point to a dir with a empty resource, aborting");
		}
		return 0;
	}
	if (strlen(buffer) == 0)
	{
		WRITE("->CREATE: trying to create a 0 lenght file name, aborting");
		return 0;
	}

	file = createFile(buffer);
	addNodeFileChild(n, file);

	WRITE("->CREATE: done");
	return 1;
}

int FSCreateDir(FILE* f, Node* root, char* buffer)
{

	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);

	if (out != DIR_PARENT_EXIST)
	{
		if (DIR_EXIST)
		{
			WRITE("->CREATE_DIR: dir already exist");

		}
		else
		{
			WRITE("->CREATE_DIR: path does not point to a dir with a empty resource, aborting");
		}
		return 0;
	}
	if (strlen(buffer) == 0)
	{
		WRITE("->CREATE_DIR: trying to create a 0 lenght file name, aborting");
		return 0;
	}
	addNodeNodeChild(n, createNode(buffer));
	WRITE("->CREATE_DIR: done");
	return 1;
}

//prints the structure of the tree
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


void FSRead(FILE* f, Node *root, char* buffer)
{
	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);
	File* file;

	if (out != FILE_EXIST)
	{
		WRITE("->READ: path does not point to a file, aborting");
		return;
	}

	file = fileHMapFind(buffer, &n->fileChildren);
	WRITE("->READ: done");
	printeFile(file);
	putchar('\n');
}

void FSWrite(FILE* f, Node *root, char* buffer)
{
	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);
	File* file = NULL;
	char c = 0;

	if (out != FILE_EXIST)
	{
		WRITE("->WRITE: path does not point to a file, aborting");
		return;
	}

	file = fileHMapFind(buffer, &n->fileChildren);

	while ((c = fgetc(f)) != '\"')
		;

	while ((c = fgetc(f)) != '\"' && c != '\n')
		writeFileChar(file, c);	

	if (c == '\n')
		endLineReached = 1;

	WRITE("->WRITE: done");
}


void FSDelete(FILE* f, Node *root, char* buffer)
{
	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);
	File* file;
	if (out != DIR_EXIST && out != FILE_EXIST)
	{
		WRITE("->DELETE: path not pointing to a resource, aborting");
		return;
	}

	if (out == DIR_EXIST)
	{
		if (n != root)
		{
			removeNodeNodeChild(n);
			WRITE("->DELETE: done");
		}
		else
		{
			WRITE("->DELETE: trying to delete root, aborting");
		}
	}
	else
	{
		file = fileHMapFind(buffer, &n->fileChildren);
		removeFileNodeChild(n, file);	
		WRITE("->DELETE: done");
	}
}


void FSDeleteRecursive(FILE* f, Node *root, char* buffer)
{
	locateResult out;
	Node* n = locateRecursive(root, f, buffer, &out);
	File* file;

	if (out == FILE_EXIST)
	{
		file = fileHMapFind(buffer, &n->fileChildren);
		removeFileNodeChild(n, file);	
		WRITE("->DELETE_R: done");
		return;
	}

	if (out != DIR_EXIST)
	{
		WRITE("->DELETE_R: target dir does not exist, aborting");
		return;
	}

	if (n == root)
	{
		WRITE("->DELETE_R: trying to delete root, aborting");
		return;
	}
	recursiveRemoveNode(n, n->parent);
	WRITE("->DELETE_R: done");
}

void FSDeleteRoot(Node *root)
{
	if (!root)
		return;	

	if (root->parent)
		return;

	recursiveRemoveNode(root, NULL);	
	deleteNode(root);
	WRITE("->DELETE_ROOT: done");
}

void FSFind(FILE* f, Node *root, char* buffer)
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
