#pragma once

struct ndlst;
typedef struct ndlst NodeList; 

struct fllst;
typedef struct fllst FileList;

typedef struct fl File;
typedef struct nd Node;

FileList* createFileList(File *file); //DONE
void insertInFileList(File *file, FileList *ls); //DONE
FileList* findFileList(char* name, FileList *fl); //DONE
int deleteFileList(FileList* fl); //DONE
void recursiveDeleteFileList(FileList* file); //DONE
File* getFileFromList(FileList* file); //DONE
FileList* getNextFileList(FileList* file); //DONE

NodeList* createNodeList(Node *node); //DONE
void insertInNodeList(Node *node, NodeList *ls); //DONE
NodeList* findNodeList(char* name, NodeList* ls); //DONE
int deleteNodeList(NodeList* node); //DONE
void recursiveDeleteNodeList(NodeList* node); //DONE
Node* getNodeFromList(NodeList* file); //DONE
NodeList* getNextNodeList(NodeList* file); //DONE
