#include "unity.h"
#include "LoadElf.h"
#include "elf.h"
#include "ProgramElf.h"
#include "GetHeaders.h"
#include "Read_File.h"

void setUp(void) {}

void tearDown(void) {}

void test_isWithinPhysRange_should_get_1_if_within_the_range1(void)
{
  ElfData *elf = openElfFile("test/ElfFiles/ledFlash.elf");
  int result = isWithinPhysRange(elf, 0x8000000);
  
  TEST_ASSERT_EQUAL(1, result);
  
  closeElfData(elf);
}

void test_isWithinPhysRange_should_get_1_if_within_the_range2(void)
{
  ElfData *elf = openElfFile("test/ElfFiles/ledFlash.elf");
  int result = isWithinPhysRange(elf, 0x8000001);
  
  TEST_ASSERT_EQUAL(1, result);
  
  closeElfData(elf);
}

void test_isWithinPhysRange_should_get_0_if_out_of_range(void)
{
  ElfData *elf = openElfFile("test/ElfFiles/ledFlash.elf");
  int result = isWithinPhysRange(elf, 0x10000000);
  
  TEST_ASSERT_EQUAL(0, result);
  
  closeElfData(elf);
}

void test_isWithinVirtRange_should_get_0_if_out_of_range1(void)
{
  ElfData *elf = openElfFile("test/ElfFiles/ledFlash.elf");
  int result = isWithinVirtRange(elf, 0x10000000);
  
  TEST_ASSERT_EQUAL(0, result);
  
  closeElfData(elf);
}

void test_isWithinVirtRange_should_get_1_if_out_of_range2(void)
{
  ElfData *elf = openElfFile("test/ElfFiles/ledFlash.elf");
  int result = isWithinVirtRange(elf, 0x20000000);
  
  TEST_ASSERT_EQUAL(1, result);
  
  closeElfData(elf);
}

void test_getCorrectPhysAddress_should_get_correct_physical_address(void)
{
  ElfData *elf = openElfFile("test/ElfFiles/ledFlash.elf");
  uint32_t address = getCorrectPhysAddress(elf, 0x20000000);
  
  TEST_ASSERT_EQUAL_HEX32(0x08001064, address);
  
  closeElfData(elf);
}

void test_getLoadMemoryAddress_should_get_the_correct_lma(void)
{
  ElfData *elf = openElfFile("test/ElfFiles/ledFlash.elf");
  uint32_t address = getLoadMemoryAddress(elf, 1);
  
  TEST_ASSERT_EQUAL_HEX32(0x8000000, address);
  
  closeElfData(elf);
}

void test_getLoadMemoryAddress_should_get_the_correct_lma2(void)
{
  ElfData *elf = openElfFile("test/ElfFiles/ledFlash.elf");
  uint32_t address = getLoadMemoryAddress(elf, 8);
  
  TEST_ASSERT_EQUAL_HEX32(-1, address);
  
  closeElfData(elf);
}

void test_getProgramInfo_should_get_the_correct_info_load_ram(void)
{
  Program *p = getLoadableSection("test/ElfFiles/ledRam.elf");
  
  TEST_ASSERT_NOT_NULL(p);
  TEST_ASSERT_EQUAL(6, p->ssize);
  
  TEST_ASSERT_EQUAL_STRING(".isr_vector", p->ps[0].name);
  TEST_ASSERT_EQUAL(428, p->ps[0].size);
  TEST_ASSERT_EQUAL_HEX32(0x20000000, p->ps[0].address);
  
  TEST_ASSERT_EQUAL_STRING(".text", p->ps[1].name);
  TEST_ASSERT_EQUAL(3728, p->ps[1].size);
  TEST_ASSERT_EQUAL_HEX32(0x200001ac, p->ps[1].address);
  
  TEST_ASSERT_EQUAL_STRING(".rodata", p->ps[2].name);
  TEST_ASSERT_EQUAL(8, p->ps[2].size);
  TEST_ASSERT_EQUAL_HEX32(0x2000103C, p->ps[2].address);
  
  TEST_ASSERT_EQUAL_STRING(".init_array", p->ps[3].name);
  TEST_ASSERT_EQUAL(8, p->ps[3].size);
  TEST_ASSERT_EQUAL_HEX32(0x20001044, p->ps[3].address);
  
  TEST_ASSERT_EQUAL_STRING(".fini_array", p->ps[4].name);
  TEST_ASSERT_EQUAL(4, p->ps[4].size);
  TEST_ASSERT_EQUAL_HEX32(0x2000104C, p->ps[4].address);
  
  TEST_ASSERT_EQUAL_STRING(".data", p->ps[5].name);
  TEST_ASSERT_EQUAL(1088, p->ps[5].size);
  TEST_ASSERT_EQUAL_HEX32(0x20001050, p->ps[5].address);
}

void test_getProgramInfo_should_get_the_correct_info_load_flash(void)
{
  Program *p = getLoadableSection("test/ElfFiles/ledFlash.elf");
  
  TEST_ASSERT_NOT_NULL(p);
  TEST_ASSERT_EQUAL(6, p->ssize);
  
  TEST_ASSERT_EQUAL_STRING(".isr_vector", p->ps[0].name);
  TEST_ASSERT_EQUAL(428, p->ps[0].size);
  TEST_ASSERT_EQUAL_HEX32(0x8000000, p->ps[0].address);
  
  TEST_ASSERT_EQUAL_STRING(".text", p->ps[1].name);
  TEST_ASSERT_EQUAL(3748, p->ps[1].size);
  TEST_ASSERT_EQUAL_HEX32(0x080001ac, p->ps[1].address);
  
  TEST_ASSERT_EQUAL_STRING(".rodata", p->ps[2].name);
  TEST_ASSERT_EQUAL(8, p->ps[2].size);
  TEST_ASSERT_EQUAL_HEX32(0x08001050, p->ps[2].address);
  
  TEST_ASSERT_EQUAL_STRING(".init_array", p->ps[3].name);
  TEST_ASSERT_EQUAL(8, p->ps[3].size);
  TEST_ASSERT_EQUAL_HEX32(0x08001058, p->ps[3].address);
  
  TEST_ASSERT_EQUAL_STRING(".fini_array", p->ps[4].name);
  TEST_ASSERT_EQUAL(4, p->ps[4].size);
  TEST_ASSERT_EQUAL_HEX32(0x08001060, p->ps[4].address);
  
  TEST_ASSERT_EQUAL_STRING(".data", p->ps[5].name);
  TEST_ASSERT_EQUAL(1088, p->ps[5].size);
  TEST_ASSERT_EQUAL_HEX32(0x08001064, p->ps[5].address);
  
  delProgram(p);
}

void test_getEntryAddress_should_get_entry_address(void)
{
  Program *p = getLoadableSection("test/ElfFiles/ledRam.elf");
  
  TEST_ASSERT_EQUAL_HEX32(0x20000ff5, getEntryAddress(p));
  
  delProgram(p);
}