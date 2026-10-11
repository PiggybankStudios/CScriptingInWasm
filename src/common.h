/*
File:   common.h
Author: Taylor Robbins
Date:   10\04\2026
*/

#ifndef _COMMON_H
#define _COMMON_H

#include "symbol_set.h"

#if SY__OS_WINDOWS
#include <stdio.h>
#include <stddef.h>
#endif
#include <stdint.h>
#include <string.h>

#if SY__OS_WASM
#define EXPORT_FUNC(nameStrLit) __attribute__((export_name(nameStrLit)))
#else
#define EXPORT_FUNC(nameStrLit) //nothing
#endif

#define IMPORT_FUNC(nameStrLit) __attribute__((import_module("env"), import_name(nameStrLit)))

#endif //  _COMMON_H
