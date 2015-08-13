#include "LinkedList.h"
#include <string.h>
void initLinkedList(LinkedList *list)
{
  list->head = NULL;
  list->tail = NULL ;
  
  list->readIndex = 0 ;
  list->storeIndex = 0;
}

ListElement *listElementCreate()
{
  ListElement *listElement = malloc(sizeof(ListElement));
  
  memset(((uint8_t *)(listElement->data)),0,SIZE);
  listElement->next = NULL ;
  
  return listElement;
}

void listElementDestroy(ListElement *listElement)
{
	listElement->next = NULL ;
	free(listElement);
}

void destroyAllElements(LinkedList *list)
{
  ListElement *nextElement ;
  
  if (list->head != NULL)
  {
    nextElement = (list->head)->next ;
    if(nextElement == NULL)
      listElementDestroy(list->head);
    
    else
    {
      do
      {
        listElementDestroy(list->head);
        nextElement = (list->head)->next ;
        list->head = nextElement ;
      }while(nextElement != NULL);
    }
       
  }
  initLinkedList(list);
}

void listAddData(LinkedList *list,uint8_t dataToAdd)
{
  ListElement *newElement , *current;
  int i = 0 ;
  int index = 0;
  
  if (list->head == NULL || list->storeIndex % SIZE == 0 )
  {
    newElement = listElementCreate();
    if(list->head == NULL)
      list->head = newElement ;    
    else
      (list->tail)->next = newElement ;
    
    list->tail = newElement ;
  }
 
  current = list->head ;
  for (i = 0 ; i != list->storeIndex / SIZE ; i++)
    current = current->next ;

  index =(list->storeIndex % SIZE);
  
  current->data[index] = dataToAdd ;
  
  list->storeIndex ++ ;
}

uint8_t listReadData(LinkedList *list)
{
  ListElement *current ;
  uint8_t dataToRead = 0 ;
  int i = 0 ,index = 0;
  
  if(list->readIndex >= list->storeIndex)
    return 0;
  
  current = list->head ;
  for(i = 0 ; i < (list->readIndex / SIZE) ; i ++)
    current = current->next ;
  
  index = (list->readIndex % SIZE) ;
  
  dataToRead = current->data[index];
  
  list->readIndex ++ ;
  
  return dataToRead ;
}