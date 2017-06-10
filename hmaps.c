#include "hmaps.h"
#include "lists.h"
#include "node.h"
#include "file.h"
#include <stdlib.h>

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
	if (!map || !file->name || !file)
		return 0;

	NodeList *f = map->list[hash(file->name)];

	if (!f)
		return 0;

	if (file == getNodeFromList(f))
		map->list[hash(file->name)] = getNextNodeList(f);

	return deleteNodeList(findNodeList(file->name, f)); 
}

