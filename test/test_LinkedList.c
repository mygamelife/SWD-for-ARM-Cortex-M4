#include "unity.h"
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include "LinkedList.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_initLinkedList_should_set_headtail_NULL_readIndex_storeIndex_0()
{
  LinkedList list ;
  initLinkedList(&list);
  
  TEST_ASSERT_EQUAL(NULL,list.head);
  TEST_ASSERT_EQUAL(NULL,list.tail);
  TEST_ASSERT_EQUAL(0,list.readIndex);
  TEST_ASSERT_EQUAL(0,list.storeIndex);
  
}

void test_listElementCreate_should_allocate_memory_for_listElement_and_init_all_data_to_zero()
{
  ListElement *listElement ;
  
  listElement = listElementCreate();
  
  TEST_ASSERT_NOT_NULL(listElement);
  
  TEST_ASSERT_EQUAL(0,listElement->data[0]);
  TEST_ASSERT_EQUAL(0,listElement->data[1]);
  TEST_ASSERT_EQUAL(0,listElement->data[2]);
  TEST_ASSERT_EQUAL(0,listElement->data[3]);
  TEST_ASSERT_EQUAL(0,listElement->data[4]);
  TEST_ASSERT_EQUAL(0,listElement->data[5]);
  TEST_ASSERT_EQUAL(0,listElement->data[6]);
  TEST_ASSERT_EQUAL(0,listElement->data[7]);
  TEST_ASSERT_EQUAL(0,listElement->data[8]);
  TEST_ASSERT_EQUAL(0,listElement->data[9]);
  TEST_ASSERT_EQUAL(0,listElement->data[10]);
  TEST_ASSERT_EQUAL(0,listElement->data[11]);
  TEST_ASSERT_EQUAL(0,listElement->data[12]);
  TEST_ASSERT_EQUAL(0,listElement->data[13]);
  TEST_ASSERT_EQUAL(0,listElement->data[14]);
  TEST_ASSERT_EQUAL(0,listElement->data[15]);
  TEST_ASSERT_EQUAL(0,listElement->data[16]);
  TEST_ASSERT_EQUAL(0,listElement->data[17]);
  TEST_ASSERT_EQUAL(0,listElement->data[18]);
  TEST_ASSERT_EQUAL(0,listElement->data[19]);
  
  TEST_ASSERT_EQUAL(NULL,listElement->next);
  
  listElementDestroy(listElement);
}

void test_listAddData_and_listReadData()
{
  LinkedList list ;
  initLinkedList (&list);
  
  uint8_t data0 = 0 ;    uint8_t data10 = 0 ;
  uint8_t data1 = 0 ;    uint8_t data11 = 0 ;
  uint8_t data2 = 0 ;    uint8_t data12 = 0 ;
  uint8_t data3 = 0 ;    uint8_t data13 = 0 ;
  uint8_t data4 = 0 ;    uint8_t data14 = 0 ;
  uint8_t data5 = 0 ;    uint8_t data15 = 0 ;
  uint8_t data6 = 0 ;    uint8_t data16 = 0 ;
  uint8_t data7 = 0 ;    uint8_t data17 = 0 ;
  uint8_t data8 = 0 ;    uint8_t data18 = 0 ;
  uint8_t data9 = 0 ;    uint8_t data19 = 0 ;

  uint8_t data20 = 0;    uint8_t data21 = 0 ;
  uint8_t data22 = 0;    uint8_t data23 = 0xFF ;
  
  
  listAddData(&list,0x0);
  listAddData(&list,0x1);
  listAddData(&list,0x2);
  listAddData(&list,0x3);
  listAddData(&list,0x4);
  listAddData(&list,0x5);
  listAddData(&list,0x6);
  listAddData(&list,0x7);
  listAddData(&list,0x8);
  listAddData(&list,0x9);
  listAddData(&list,0x10);
  listAddData(&list,0x11);
  listAddData(&list,0x12);
  listAddData(&list,0x13);
  listAddData(&list,0x14);
  listAddData(&list,0x15);
  listAddData(&list,0x16);
  listAddData(&list,0x17);
  listAddData(&list,0x18);
  listAddData(&list,0x19);
  listAddData(&list,0x20);
  listAddData(&list,0x21);
  listAddData(&list,0x22);
  
  data0 = listReadData(&list);
  data1 = listReadData(&list);
  data2 = listReadData(&list);
  data3 = listReadData(&list);
  data4 = listReadData(&list);
  data5 = listReadData(&list);
  data6 = listReadData(&list);
  data7 = listReadData(&list);
  data8 = listReadData(&list);
  data9 = listReadData(&list);
  data10 = listReadData(&list);
  data11 = listReadData(&list);
  data12 = listReadData(&list);
  data13 = listReadData(&list);
  data14 = listReadData(&list);
  data15 = listReadData(&list);
  data16 = listReadData(&list);
  data17 = listReadData(&list);
  data18 = listReadData(&list);
  data19 = listReadData(&list);
  data20 = listReadData(&list);
  data21 = listReadData(&list);
  data22 = listReadData(&list);
  data23 = listReadData(&list);

  TEST_ASSERT_EQUAL(0x0,data0);
  TEST_ASSERT_EQUAL(0x1,data1);
  TEST_ASSERT_EQUAL(0x2,data2); 
  TEST_ASSERT_EQUAL(0x3,data3);
  TEST_ASSERT_EQUAL(0x4,data4);
  TEST_ASSERT_EQUAL(0x5,data5);
  TEST_ASSERT_EQUAL(0x6,data6);
  TEST_ASSERT_EQUAL(0x7,data7);
  TEST_ASSERT_EQUAL(0x8,data8); 
  TEST_ASSERT_EQUAL(0x9,data9); 
  TEST_ASSERT_EQUAL(0x10,data10);
  TEST_ASSERT_EQUAL(0x11,data11);
  TEST_ASSERT_EQUAL(0x12,data12);
  TEST_ASSERT_EQUAL(0x13,data13);
  TEST_ASSERT_EQUAL(0x14,data14);
  TEST_ASSERT_EQUAL(0x15,data15);
  TEST_ASSERT_EQUAL(0x16,data16);
  TEST_ASSERT_EQUAL(0x17,data17);
  TEST_ASSERT_EQUAL(0x18,data18);
  TEST_ASSERT_EQUAL(0x19,data19);
  TEST_ASSERT_EQUAL(0x20,data20);
  TEST_ASSERT_EQUAL(0x21,data21);
  TEST_ASSERT_EQUAL(0x22,data22);
  TEST_ASSERT_EQUAL(0x0,data23);
  
  destroyAllElements(&list);
}