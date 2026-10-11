/*
File:   commands.h
Author: Taylor Robbins
Date:   10\10\2026
*/

#ifndef _COMMANDS_H
#define _COMMANDS_H

#include "symbol_set.h"

typedef struct RegisteredCommand RegisteredCommand;
struct RegisteredCommand
{
	const char* name;
	const char* description;
};

#define SYMBOL_SET_DEFINE COMMANDS
#define COMMANDS_Type    RegisteredCommand
#define COMMANDS_elf_section         ".sy.cmds"
#define COMMANDS_wasm_section        "_sy_cmds"
#define COMMANDS_wasm_section_cident _sy_cmds
#define COMMANDS_coff_a_section      ".sy$cmds_a"
#define COMMANDS_coff_m_section      ".sy$cmds_m"
#define COMMANDS_coff_z_section      ".sy$cmds_z"
#define COMMANDS_marker  cmd
#include "symbol_set.define.h"

#define COMMAND(cmdName, cmdDescriptionLit) SyDefine(COMMANDS, cmdName) = { .name = #cmdName, .description = cmdDescriptionLit }
#define COMMAND_DECL(cmdName) extern SyDeclare(COMMANDS, cmdName)

//In commands1.c
COMMAND_DECL(test1);
// COMMAND_DECL(test2);
// COMMAND_DECL(test3);
// COMMAND_DECL(test4);
// COMMAND_DECL(test5);

//In commands2.c
// COMMAND_DECL(test6);
COMMAND_DECL(test7);
// COMMAND_DECL(test8);
// COMMAND_DECL(test9);

#endif //  _COMMANDS_H
