#include "unity.h"
#include <stdint.h>
#include <malloc.h>
#include "LinkedList.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_listCreate_should_create_list_()
{
	LinkedList *list ;
	
  list = listCreate();
  
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_NULL(list->head);
	TEST_ASSERT_NULL(list->tail);
	
	listDestroy(list);
}

void test_listElementCreate_should_create_the_element()
{
  ListElement *element ;
  
  element = listElementCreate(0x12345678,0xDEADBEEF);
  
  TEST_ASSERT_EQUAL(0x12345678,element->address);
  TEST_ASSERT_EQUAL(0xDEADBEEF,element->machineCode);
  TEST_ASSERT_NULL(element->next);
  
  free(element);
}

void test_listAddTail_should_add_element_to_list_1element()
{
  LinkedList *list ;
  
  list = listCreate() ;
  
  listAddTail(list,0x11112222,0x33334444);
  
  TEST_ASSERT_EQUAL(0x11112222,list->head->address);
  TEST_ASSERT_EQUAL(0x33334444,list->head->machineCode);
  
  TEST_ASSERT_EQUAL(0x11112222,list->tail->address);
  TEST_ASSERT_EQUAL(0x33334444,list->tail->machineCode);
  
  listDestroy(list);
}

void test_listAddTail_should_add_element_to_list_2element()
{
  LinkedList *list ;
  
  list = listCreate() ;
  
  listAddTail(list,0x11112222,0x33334444);
  listAddTail(list,0xAAAAAAAA,0xBBBBBBBB);
  
  TEST_ASSERT_EQUAL(0x11112222,list->head->address);
  TEST_ASSERT_EQUAL(0x33334444,list->head->machineCode);
  
  TEST_ASSERT_EQUAL(0xAAAAAAAA,list->tail->address);
  TEST_ASSERT_EQUAL(0xBBBBBBBB,list->tail->machineCode);
  
  listDestroy(list);
}

void test_listAddTail_should_add_element_to_list_3element()
{
  LinkedList *list ;
  
  list = listCreate() ;
  
  listAddTail(list,0x11112222,0x33334444);
  listAddTail(list,0xAAAAAAAA,0xBBBBBBBB);
  listAddTail(list,0xCCCCCCCC,0xDDDDDDDD);
  
  TEST_ASSERT_EQUAL(0x11112222,list->head->address);
  TEST_ASSERT_EQUAL(0x33334444,list->head->machineCode);
  
  TEST_ASSERT_EQUAL(0xAAAAAAAA,list->head->next->address);
  TEST_ASSERT_EQUAL(0xBBBBBBBB,list->head->next->machineCode);
  
  TEST_ASSERT_EQUAL(0xCCCCCCCC,list->tail->address);
  TEST_ASSERT_EQUAL(0xDDDDDDDD,list->tail->machineCode);
  
  listDestroy(list);
}

void test_listRetrieveSelected_should_return_the_element_if_found()
{
  LinkedList *list ;
  ListElement *found;
  
  list = listCreate() ;
  
  listAddTail(list,0x11112222,0x33334444);
  listAddTail(list,0xAAAAAAAA,0xBBBBBBBB);
  listAddTail(list,0xCCCCCCCC,0xDDDDDDDD);
  
  found = listElementRetrieveSelected(list,0xAAAAAAAA);
  
  TEST_ASSERT_EQUAL(0xAAAAAAAA,found->address);
  TEST_ASSERT_EQUAL(0xBBBBBBBB,found->machineCode);
  
  listDestroy(list);
}

void test_listRetrieveSelected_should_return_NULL_if_not_found()
{
  LinkedList *list ;
  ListElement *found;
  
  list = listCreate() ;
  
  listAddTail(list,0x11112222,0x33334444);
  listAddTail(list,0xAAAAAAAA,0xBBBBBBBB);
  listAddTail(list,0xCCCCCCCC,0xDDDDDDDD);
  
  found = listElementRetrieveSelected(list,0x12345678);
  
  TEST_ASSERT_NULL(found);
  
  listDestroy(list);
}


void test_listlistElementRemoveSelected_should_remove_element_from_list_removeHeadcase()
{
  LinkedList *list ;
  
  list = listCreate() ;
  
  listAddTail(list,0x11112222,0x33334444);
  listAddTail(list,0xAAAAAAAA,0xBBBBBBBB);
  listAddTail(list,0xCCCCCCCC,0xDDDDDDDD);
  
  listElementRemoveSelected(list,0x11112222);
  
  TEST_ASSERT_EQUAL(0xAAAAAAAA,list->head->address);
  
  listDestroy(list);
}

void test_listlistElementRemoveSelected_should_remove_element_from_list_removeMiddlecase()
{
  LinkedList *list ;
  
  list = listCreate() ;
  
  listAddTail(list,0x11112222,0x33334444);
  listAddTail(list,0xAAAAAAAA,0xBBBBBBBB);
  listAddTail(list,0xCCCCCCCC,0xDDDDDDDD);
  
  listElementRemoveSelected(list,0xAAAAAAAA);
  
  TEST_ASSERT_EQUAL(0xCCCCCCCC,list->head->next->address);
  
  listDestroy(list);
}


void test_listlistElementRemoveSelected_should_remove_element_from_list_removeTailcase()
{
  LinkedList *list ;
  
  list = listCreate() ;
  
  listAddTail(list,0x11112222,0x33334444);
  listAddTail(list,0xAAAAAAAA,0xBBBBBBBB);
  listAddTail(list,0xCCCCCCCC,0xDDDDDDDD);
  
  listElementRemoveSelected(list,0xCCCCCCCC);
  
  TEST_ASSERT_EQUAL(0xAAAAAAAA,list->tail->address);
  TEST_ASSERT_NULL(list->tail->next);
  
  listDestroy(list);
}