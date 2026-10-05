/*
File:   main.c
Author: Taylor Robbins
Date:   10\04\2026
Description: 
	** None
*/

#define SY__MAIN 1

#include "common.h"

#include "symbol_set.h"

#include <stdio.h>

COMMAND(local1, "This is a description of local1");
COMMAND(local2, "This is a description of local2");
COMMAND(local3, "This is a description of local3");
COMMAND(local4, "This is a description of local4");

__attribute__((__section__("custom_section"))) int thingInCustomSectionA = 0xAAAAAAAA;
__attribute__((__section__("custom_section"))) int thingInCustomSectionC = 0xCCCCCCCC;
__attribute__((__section__("custom_section"))) int thingInCustomSectionB = 0xBBBBBBBB;
extern int thingInCustomSectionD;
extern const char __start_custom_section;
extern const char __end_custom_section;

EXPORT_FUNC("MyFunction") int MyFunction()
{
	// SY__U32 local1Id = SyID(COMMANDS, local1);
	// SY__U32 local2Id = SyID(COMMANDS, local2);
	// SY__U32 local3Id = SyID(COMMANDS, local3);
	// SY__U32 local4Id = SyID(COMMANDS, local4);
	// SY__U32 test1Id = SyID(COMMANDS, test1);
	// SY__U32 test2Id = SyID(COMMANDS, test2);
	// SY__U32 test3Id = SyID(COMMANDS, test3);
	// SY__U32 test4Id = SyID(COMMANDS, test4);
	// SY__U32 test5Id = SyID(COMMANDS, test5);
	// SY__U32 test6Id = SyID(COMMANDS, test6);
	// SY__U32 test7Id = SyID(COMMANDS, test7);
	// SY__U32 test8Id = SyID(COMMANDS, test8);
	// SY__U32 test9Id = SyID(COMMANDS, test9);
	// printf("test1 has ID %u\n", test1Id);
	// printf("test2 has ID %u\n", test2Id);
	// printf("test3 has ID %u\n", test3Id);
	// printf("test4 has ID %u\n", test4Id);
	// printf("test5 has ID %u\n", test5Id);
	// printf("test6 has ID %u\n", test6Id);
	// printf("test7 has ID %u\n", test7Id);
	// printf("test8 has ID %u\n", test8Id);
	// printf("test9 has ID %u\n", test9Id);
	// int result = 0;
	// for (SyEachID(COMMANDS, cmdId))
	// {
	// 	RegisteredCommand* cmd = SyAddressFromID(COMMANDS, cmdId);
	// 	printf("Command %u: \"%s\"\n", cmdId, cmd->name);
	// 	result += cmdId;
	// }
	// return (int)(test1Id + test2Id + test3Id + test4Id + test5Id + test6Id + test7Id + test8Id + test9Id);
	// return (int)(local1Id + local2Id + local3Id + local4Id);
	// return result;
	int result = thingInCustomSectionA + thingInCustomSectionB + thingInCustomSectionC + thingInCustomSectionD;
	return ((int)&__end_custom_section) - ((int)&__start_custom_section) + result;
	// return 0;
}

#if SY__OS_WINDOWS
int main(int argc, char* argv[])
{
	return MyFunction();
}
#endif