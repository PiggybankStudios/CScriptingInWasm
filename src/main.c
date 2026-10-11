/*
File:   main.c
Author: Taylor Robbins
Date:   10\04\2026
Description: 
	** None
*/

#define SY__MAIN 1

#include "common.h"
#include "js_bindings.h"
#include "commands.h"

COMMAND(local1, "This is a description of local1");
COMMAND(local2, "This is a description of local2");
COMMAND(local3, "This is a description of local3");
COMMAND(local4, "This is a description of local4");

#if SY__OS_WASM
__attribute__((__section__("custom_section"))) int thingInCustomSectionA = 0xAAAAAAAA;
__attribute__((__section__("custom_section"))) int thingInCustomSectionC = 0xCCCCCCCC;
__attribute__((__section__("custom_section"))) int thingInCustomSectionB = 0xBBBBBBBB;
extern int thingInCustomSectionD;
extern const char __start_custom_section;
extern const char __stop_custom_section;
#endif //SY__OS_WASM

EXPORT_FUNC("Add") int Add(int left, int right) { return left + right; }

EXPORT_FUNC("PrintCommandsInfo") int PrintCommandsInfo()
{
	int result = 0;
	
	SY__U32 local1Id = SyID(COMMANDS, local1);
	SY__U32 local2Id = SyID(COMMANDS, local2);
	SY__U32 local3Id = SyID(COMMANDS, local3);
	SY__U32 local4Id = SyID(COMMANDS, local4);
	SY__U32 test1Id = SyID(COMMANDS, test1);
	// SY__U32 test2Id = SyID(COMMANDS, test2);
	// SY__U32 test3Id = SyID(COMMANDS, test3);
	// SY__U32 test4Id = SyID(COMMANDS, test4);
	// SY__U32 test5Id = SyID(COMMANDS, test5);
	// SY__U32 test6Id = SyID(COMMANDS, test6);
	SY__U32 test7Id = SyID(COMMANDS, test7);
	// SY__U32 test8Id = SyID(COMMANDS, test8);
	// SY__U32 test9Id = SyID(COMMANDS, test9);
	
	#if SY__OS_WINDOWS
	printf("test1 has ID %u\n", test1Id);
	// printf("test2 has ID %u\n", test2Id);
	// printf("test3 has ID %u\n", test3Id);
	// printf("test4 has ID %u\n", test4Id);
	// printf("test5 has ID %u\n", test5Id);
	// printf("test6 has ID %u\n", test6Id);
	printf("test7 has ID %u\n", test7Id);
	// printf("test8 has ID %u\n", test8Id);
	// printf("test9 has ID %u\n", test9Id);
	#endif
	
	#if SY__OS_WASM
	jsPrintNamedPtr("__start_custom_section", &__start_custom_section);
	jsPrintNamedPtr("__stop_custom_section", &__stop_custom_section);
	jsPrintNamedI32("sectionSize", (int32_t)((int)&__stop_custom_section) - ((int)&__start_custom_section));
	jsPrintNamedPtr("thingInCustomSectionA", &thingInCustomSectionA);
	jsPrintNamedPtr("thingInCustomSectionB", &thingInCustomSectionB);
	jsPrintNamedPtr("thingInCustomSectionC", &thingInCustomSectionC);
	jsPrintNamedPtr("thingInCustomSectionD", &thingInCustomSectionD);
	jsPrintNamedI32("Command Count", SyCount(COMMANDS));
	jsPrintNamedI32("local1 ID", local1Id);
	jsPrintNamedI32("local2 ID", local2Id);
	jsPrintNamedI32("local3 ID", local3Id);
	jsPrintNamedI32("local4 ID", local4Id);
	jsPrintNamedI32("test1 ID", test1Id);
	// jsPrintNamedI32("test2 ID", test2Id);
	// jsPrintNamedI32("test3 ID", test3Id);
	// jsPrintNamedI32("test4 ID", test4Id);
	// jsPrintNamedI32("test5 ID", test5Id);
	// jsPrintNamedI32("test6 ID", test6Id);
	jsPrintNamedI32("test7 ID", test7Id);
	// jsPrintNamedI32("test8 ID", test8Id);
	// jsPrintNamedI32("test9 ID", test9Id);
	#endif //SY__OS_WASM
	
	for (SyEachID(COMMANDS, cmdId))
	{
		RegisteredCommand* cmd = SyAddressFromID(COMMANDS, cmdId);
		#if SY__OS_WINDOWS
		printf("Command %u: \"%s\"\n", cmdId, cmd->name);
		#else
		char indexedCmdStr[] = "Command[0xXX]";
		indexedCmdStr[10] = ((cmdId & 0xF0) >= 10) ? ('A' + ((cmdId & 0xF0) - 10)) : ('0' + (cmdId & 0xF0));
		indexedCmdStr[11] = ((cmdId & 0x0F) >= 10) ? ('A' + ((cmdId & 0x0F) - 10)) : ('0' + (cmdId & 0x0F));
		jsPrintNamedStrNt(indexedCmdStr, cmd->name);
		#endif
	}
	
	jsPrintMem(&SY__WASM_START_SYMBOL(COMMANDS), SyCount(COMMANDS) * SyStride(COMMANDS));
	
	return result;
}

#if SY__OS_WINDOWS
int main(int argc, char* argv[])
{
	return PrintCommandsInfo();
}
#endif

#if SY__OS_WASM
size_t strlen(const char* nullTermStr)
{
	size_t result = 0;
	while (nullTermStr != nullptr && nullTermStr[result] != '\0') { result++; }
	return result;
}
#endif
