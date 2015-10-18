#ifndef LinkedList_H
#define LinkedList_H

typedef struct LinkedList LinkedList;
typedef struct ListElement ListElement;

#include <stdint.h>
#include <malloc.h>

struct LinkedList 
{
	ListElement *head ;
  ListElement *tail ;
};

struct ListElement
{
  uint32_t address ;
  uint32_t machineCode ;
  ListElement *next;
};


LinkedList *listCreate();
void listDestroy(LinkedList *listToDestroy);

ListElement* listElementCreate(uint32_t address,uint32_t machineCode);
ListElement* listElementRetrieveSelected(LinkedList *list,uint32_t address);

void listAddTail(LinkedList *list,uint32_t address,uint32_t machineCode);
void listElementRemoveSelected(LinkedList *list,uint32_t address);

#endif // LinkedList_H
