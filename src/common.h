/*
File:   common.h
Author: Taylor Robbins
Date:   10\04\2026
*/

#ifndef _COMMON_H
#define _COMMON_H

#include "symbol_set.h"

#if SY__OS_WASM
#define EXPORT_FUNC(nameStrLit) __attribute__((export_name(nameStrLit)))
#else
#define EXPORT_FUNC(nameStrLit) //nothing
#endif

#define IMPORT_FUNC(nameStrLit) __attribute__((import_module("env"), import_name(nameStrLit)))

typedef struct RegisteredCommand RegisteredCommand;
struct RegisteredCommand
{
	const char* name;
	const char* description;
};

#define SYMBOL_SET_DEFINE COMMANDS
#define COMMANDS_Type    RegisteredCommand
#define COMMANDS_elf_section    ".sy.cmd"
#define COMMANDS_wasm_section   "sy_cmd"
#define COMMANDS_coff_a_section ".sy$cmd_a"
#define COMMANDS_coff_m_section ".sy$cmd_m"
#define COMMANDS_coff_z_section ".sy$cmd_z"
#define COMMANDS_marker  cmd
#include "symbol_set.define.h"

#define COMMAND(cmdName, cmdDescriptionLit) SyDefine(COMMANDS, cmdName) = { .name = #cmdName, .description = cmdDescriptionLit }

#endif //  _COMMON_H
