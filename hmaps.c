#include "hmaps.h"
#include "lists.h"
#include "node.h"
#include "file.h"
#include <stdlib.h>

typedef struct ndhmp 
{
	NodeList *list[HMAP_SIZE];
} NodeHMap;

typedef struct flhmp
{
	FileList *list[HMAP_SIZE];	
} FileHMap;

//allocates a file maps 
FileHMap* fileHMapCreate()
{
	FileHMap* f = (FileHMap*)malloc(sizeof(FileHMap));
	int a = 0;

	for (a = 0; a < HMAP_SIZE; a++)
		f->list[a] = NULL;

	return f;
}

//allocates a node map
NodeHMap* nodeHMapCreate()
{
	NodeHMap* f = (NodeHMap*)malloc(sizeof(NodeHMap));
	int a = 0;

	for (a = 0; a < HMAP_SIZE; a++)
		f->list[a] = NULL;

	return f;
}

//deallocate a file map, as well as all the lists inside it
void fileHMapDestroy(FileHMap* map)
{
	if (!map)
		return;
	int a;

	for (a = 0; a < HMAP_SIZE; a++)
		fileListRecursiveDelete(map->list[a]);

	free(map);
}

//deallocates a node map, as well as all the lists inside it
void nodeHMapDestroy(NodeHMap* map)
{
	if (!map)
		return;
	int a;

	for (a = 0; a < HMAP_SIZE; a++)
		nodeListRecursiveDelete(map->list[a]);

	free(map);
}

//simple hashing function
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
	char* name = fileGetName(file);
	int hashed = hash(name);

	if (!map->list[hashed])
		map->list[hashed] = fileListCreate(file);
	else
		fileListInsert(file, map->list[hashed]);	
}

//add a node to a hash map
void nodeHMapAdd(Node* file, NodeHMap* map)
{
	char* name = nodeGetName(file);
	int hashed = hash(name);

	if (!map->list[hashed])
		map->list[hashed] = nodeListCreate(file);
	else
		nodeListInsert(file, map->list[hashed]);
}

//return a file if it's inside a hash map, NULL otherwise
File* fileHMapFind(char *string, FileHMap* map)
{
	if (!map->list[hash(string)])
		return NULL;

	return fileListGetFile(fileListFindByName(string, map->list[hash(string)]));
}

//return a node if it's inside a hash map, NULL otherwise
Node* nodeHMapFind(char *string, NodeHMap* map)
{
	if (!map->list[hash(string)])
		return NULL;

	return nodeListGetNode(nodeListFindByName(string, map->list[hash(string)]));
}

//removes a file from a hash map, does not delete the file
int fileHMapRemove(File* file, FileHMap *map)
{
	int hashed = hash(fileGetName(file));
	FileList *f = map->list[hashed];

	if (!f)
		return 0;

	if (file == fileListGetFile(f))
		map->list[hashed] = fileListGetNext(f);

	return fileListDestroy(fileListFind(file, f));
}

//removes a node from a hash map, does not delete the file
int nodeHMapRemove(Node* file, NodeHMap *map)
{
	char* name = nodeGetName(file);
	if (!map || !name || !file)
		return 0;

	int hashed = hash(name);
	NodeList *f = map->list[hashed];

	if (!f)
		return 0;

	if (file == nodeListGetNode(f))
		map->list[hashed] = nodeListGetNext(f);

	return nodeListDestroy(nodeListFind(file, f)); 
}

//return the target list inside the map
FileList* fileHMapGetList(FileHMap* map, int target)
{
	if (!map)
		return NULL;

	if (target < 0 || target >= HMAP_SIZE)
		return NULL;

	return map->list[target];
}

//returns the target list inside the map
NodeList* nodeHMapGetList(NodeHMap* map, int target)
{
	if (!map)
		return NULL;

	if (target < 0 || target >= HMAP_SIZE)
		return NULL;

	return map->list[target];
}

//returns the first element of the map, NULL if the map is empty
NodeList* nodeHMapGetFront(NodeHMap* map)
{
	int a;
	if (!map)
		return NULL;

	for (a = 0; a < HMAP_SIZE; a++)
	{
		if (map->list[a] != NULL)
			return map->list[a];
	}

	return NULL;
}

//returns the first element of the map, NULL if the map is empty
FileList* fileHMapGetFront(FileHMap* map)
{
	int a;
	if (!map)
		return NULL;

	for (a = 0; a < HMAP_SIZE; a++)
	{
		if (map->list[a] != NULL)
			return map->list[a];
	}

	return NULL;
}
