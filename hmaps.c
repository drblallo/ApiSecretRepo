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

FileHMap* createFileHMap()
{
	FileHMap* f = (FileHMap*)malloc(sizeof(FileHMap));
	int a = 0;

	for (a = 0; a < HMAP_SIZE; a++)
		f->list[a] = NULL;

	return f;
}

NodeHMap* createNodeHMap()
{
	NodeHMap* f = (NodeHMap*)malloc(sizeof(NodeHMap));
	int a = 0;

	for (a = 0; a < HMAP_SIZE; a++)
		f->list[a] = NULL;

	return f;
}

void deleteFileHMap(FileHMap* map)
{
	if (!map)
		return;
	int a;

	for (a = 0; a < HMAP_SIZE; a++)
		recursiveDeleteFileList(map->list[a]);

	free(map);
}

void deleteNodeHMap(NodeHMap* map)
{
	if (!map)
		return;
	int a;

	for (a = 0; a < HMAP_SIZE; a++)
		recursiveDeleteNodeList(map->list[a]);

	free(map);
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
	char* name = getFileName(file);
	int hashed = hash(name);

	if (!map->list[hashed])
		map->list[hashed] = createFileList(file);
	else
		insertInFileList(file, map->list[hashed]);	
}

//add a node to a hash map
void nodeHMapAdd(Node* file, NodeHMap* map)
{
	char* name = getNodeName(file);
	int hashed = hash(name);

	if (!map->list[hashed])
		map->list[hashed] = createNodeList(file);
	else
		insertInNodeList(file, map->list[hashed]);
}

//return a file if it's inside a hash map, NULL otherwise
File* fileHMapFind(char *string, FileHMap* map)
{
	if (!map->list[hash(string)])
		return NULL;

	return getFileFromList(findFileList(string, map->list[hash(string)]));
}

//return a node if it's inside a hash map, NULL otherwise
Node* nodeHMapFind(char *string, NodeHMap* map)
{
	if (!map->list[hash(string)])
		return NULL;

	return getNodeFromList(findNodeList(string, map->list[hash(string)]));
}

//removes a file from a hash map, does not delete the file
int fileHMapRemove(File* file, FileHMap *map)
{
	char* name = getFileName(file);
	FileList *f = map->list[hash(name)];

	if (!f)
		return 0;

	if (file == getFileFromList(f))
		map->list[hash(name)] = getNextFileList(f);

	return deleteFileList(findFileList(name, f));
}

//removes a node from a hash map, does not delete the file
int nodeHMapRemove(Node* file, NodeHMap *map)
{
	char* name = getNodeName(file);
	if (!map || !name || !file)
		return 0;

	int hashed = hash(name);
	NodeList *f = map->list[hashed];

	if (!f)
		return 0;

	if (file == getNodeFromList(f))
		map->list[hashed] = getNextNodeList(f);

	return deleteNodeList(findNodeList(name, f)); 
}

FileList* getHMapFileList(FileHMap* map, int target)
{
	if (!map)
		return NULL;
	if (target < 0 || target >= HMAP_SIZE)
		return NULL;

	return map->list[target];
}

NodeList* getHMapNodeList(NodeHMap* map, int target)
{
	if (!map)
		return NULL;
	if (target < 0 || target >= HMAP_SIZE)
		return NULL;

	return map->list[target];
}
