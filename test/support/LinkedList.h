#ifndef LinkedList_H
#define LinkedList_H

#define SIZE 20

typedef struct LinkedList LinkedList;
typedef struct ListElement ListElement ;

#include <stdint.h>
#include <string.h>
#include <malloc.h>

struct LinkedList 
{
	ListElement *head,*tail ;
  int readIndex ;
  int storeIndex ;
};

struct ListElement
{
  uint8_t data[SIZE];
  ListElement *next ;
};

void initLinkedList(LinkedList *list);
ListElement *listElementCreate();
void listElementDestroy(ListElement *listElement);
void destroyAllElements(LinkedList *list);


#endif // LinkedList_H
