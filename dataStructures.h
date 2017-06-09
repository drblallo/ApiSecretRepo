#pragma once
#define HMAP_SIZE 32
#define FILE_DATA_SIZE 256
#define MAX_TREE_DEPTH 255
#define MAX_TREE_CHILD_COUNT 1023
#define MAX_BUFFER_SIZE 257
#define MAX_COMMAND_SIZE 15
#include <stdio.h> 
//#define QUIET

#ifndef QUIET 
#define WRITE(text) printf(text); putchar('\n')
#else
#define WRITE(text) do {} while(0)
#endif

#ifndef QUIET 
#define WRITE_S(text) printf("%s\n", text) 
#else
#define WRITE_S(text) do {} while(0)
#endif

int endLineReached;
typedef struct nd Node;

typedef struct ndlst
{
	Node *node;
	struct ndlst *next;
	struct ndlst *prev;
} NodeList; 

typedef struct ndhmp 
{
	NodeList *list[HMAP_SIZE];
} NodeHMap;

typedef struct fldt
{
	char data[FILE_DATA_SIZE];
	struct fldt* next;
	int count;
} FileData;

typedef struct fl
{
	FileData data;
	char *name;
} File;

typedef struct fllst
{
	File *file;
	struct fllst *next;	
	struct fllst *prev;
} FileList;

typedef struct flhmp
{
	FileList *list[HMAP_SIZE];	
} FileHMap;

struct nd
{
	char *name;
	NodeHMap nodeChildren;
	FileHMap fileChildren;
	unsigned childCount;
	unsigned depth;
	struct nd *parent;
};

File* createFile(char* name); //DONE
void writeFile(File* file, char* data); //DONE
void writeFileChar(File* file, char character); //DONE
void printeFile(File* file); //DONE
void destroyFile(File* file); //DONE

FileData* createFileData(); //DONE
void writeFileData(FileData *data, char* dataToWrite); //DONE
void printFileData(FileData *data); //DONE
void destoyFileData(FileData *data); //DONE

FileList* createFileList(File *file); //DONE
void insertInFileList(File *file, FileList *ls); //DONE
FileList* findFileList(char* name, FileList *fl); //DONE
int deleteFileList(FileList* fl); //DONE
void recursiveDeleteFileList(FileList* file); //DONE

NodeList* createNodeList(Node *node); //DONE
void insertInNodeList(Node *node, NodeList *ls); //DONE
NodeList* findNodeList(char* name, NodeList* ls); //DONE
int deleteNodeList(NodeList* node); //DONE
void recursiveDeleteNodeList(NodeList* node); //DONE

Node* createNode(char* name); //DONE
int addNodeNodeChild(Node* node, Node* child); //DONE
int addNodeFileChild(Node* node, File* child); //DONE
void removeNodeNodeChild(Node* child); //DONE
void recursiveRemoveNode(Node* node, Node* parent); //DONE
void removeNodeFileChild(Node* node, File* child); //DONE
void deleteNode(Node* node); //DONE
void findInNodeAndPrint(Node* n, char* name); //DONE

void fileHMapAdd(File *file, FileHMap *map); //DONE
File* fileHMapFind(char *string, FileHMap *map); //DONE
int fileHMapRemove(File *file, FileHMap *map); //DONE

void nodeHMapAdd(Node *node, NodeHMap *map); //DONE
Node* nodeHMapFind(char *string, NodeHMap *map); //DONE
int nodeHMapRemove(Node *node, NodeHMap *map); //DONE

int getNextString(char* buffer, FILE* f); //DONE
Node* locateInNode(Node* source, char* name); //DONE
Node* locateRecursive(Node* source, FILE* f, char* buffer); //DONE

int FSCreateFile(FILE* f, Node *root, char* buffer); //DONE
int FSCreateDir(FILE* f, Node *root, char* buffer); //DONE
void FSPrintTree(Node* root); //DONE
void FSRead(FILE* f, Node *root, char* buffer); //DONE
void FSWrite(FILE* f, Node *root, char* buffer); //DONE
void FSDelete(FILE* f, Node *root, char* buffer); //DONE
void FSDeleteRecursive(FILE* f, Node *root, char* buffer); //DONE
void FSDeleteRoot(Node *root); //DONE
void FSFind(FILE* f, Node *root, char* buffer); //DONE
