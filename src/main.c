/*
File:   main.c
Author: Taylor Robbins
Date:   10\04\2026
Description: 
	** None
*/

#include "common.h"

#define SY__MAIN 1
#include "symbol_set.h"

// #include <stdio.h>

__attribute__((export_name("MyFunction"))) int MyFunction()
{
	SY__U32 test1Id = SyID(COMMANDS, test1);
	SY__U32 test2Id = SyID(COMMANDS, test2);
	SY__U32 test3Id = SyID(COMMANDS, test3);
	SY__U32 test4Id = SyID(COMMANDS, test4);
	SY__U32 test5Id = SyID(COMMANDS, test5);
	SY__U32 test6Id = SyID(COMMANDS, test6);
	SY__U32 test7Id = SyID(COMMANDS, test7);
	SY__U32 test8Id = SyID(COMMANDS, test8);
	SY__U32 test9Id = SyID(COMMANDS, test9);
	// printf("test1 has ID %u\n", test1Id);
	// printf("test2 has ID %u\n", test2Id);
	// printf("test3 has ID %u\n", test3Id);
	// printf("test4 has ID %u\n", test4Id);
	// printf("test5 has ID %u\n", test5Id);
	// printf("test6 has ID %u\n", test6Id);
	// printf("test7 has ID %u\n", test7Id);
	// printf("test8 has ID %u\n", test8Id);
	// printf("test9 has ID %u\n", test9Id);
	return (int)(test1Id + test2Id + test3Id + test4Id + test5Id + test6Id + test7Id + test8Id + test9Id);
}