#include "LinkedList.h"

LinkedList *listCreate()
{
	LinkedList *newlist = malloc(sizeof(LinkedList));
	
	newlist->head = NULL ;
	newlist->tail = NULL ;
	
	return newlist ;
}

void listDestroy(LinkedList *listToDestroy)
{
  ListElement *nextElement ;
  
  while(listToDestroy->head !=NULL)
  {
    nextElement = listToDestroy->head->next ;
    free(listToDestroy->head);
    listToDestroy->head = nextElement ;
  }
  
  listToDestroy->tail == NULL ;
}

ListElement* listElementCreate(uint32_t address,uint32_t machineCode)
{
  ListElement *newElement = malloc(sizeof(ListElement));
  
  newElement->address = address ;
  newElement->machineCode = machineCode ;
  newElement->next =  NULL;
  
  return newElement ;
}


void listAddTail(LinkedList *list,uint32_t address,uint32_t machineCode)
{
  ListElement *elementToAdd = listElementCreate(address,machineCode);
  
  if(list->head == NULL && list->tail == NULL)
  {
    list->head = elementToAdd ;
    list->tail = elementToAdd ;
  }
  else
  {
    list->tail->next = elementToAdd ;
    list->tail = elementToAdd ;
  }
  
}

ListElement* listElementRetrieveSelected(LinkedList *list,uint32_t address)
{
  ListElement *current ;
   
  for(current = list->head ; current != NULL ; current = current->next)
  {
    if(current->address == address)
      return current ;
  }
  
  return NULL ;
}

void listElementRemoveSelected(LinkedList *list,uint32_t address)
{
  ListElement *current , *previous = NULL ;
  
  for(current = list->head ; current != NULL ; previous = current, current = current->next)
  {
    if(current->address == address)
    {
      if(previous == NULL)
        list->head = current->next;
      else
        previous->next = current->next;
      
      if(current == list->tail)
        list->tail = previous;
      
      free(current);
      
      return ;
    }
  }
  
}