/*
File:   js_bindings.h
Author: Taylor Robbins
Date:   10\07\2026
Description:
	** Imports that are provided by javascript in the html code (See `jsFunctionsForWasmImport`)
*/

#ifndef _JS_BINDINGS_H
#define _JS_BINDINGS_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

IMPORT_FUNC("jsPrintI32") void jsPrintI32(int32_t v);
IMPORT_FUNC("jsPrintU32") void jsPrintU32(uint32_t v);
IMPORT_FUNC("jsPrintF32") void jsPrintF32(float v);
IMPORT_FUNC("jsPrintF64") void jsPrintF64(double v);
IMPORT_FUNC("jsPrintPtr") void jsPrintPtr(const void* p);
IMPORT_FUNC("jsPrintStr") void jsPrintStr(const char* s, size_t n);
IMPORT_FUNC("jsPrintMem") void jsPrintMem(const void* p, size_t n);

IMPORT_FUNC("jsPrintNamedI32") void jsPrintNamedI32(const char* nameNt, int32_t v);
IMPORT_FUNC("jsPrintNamedPtr") void jsPrintNamedPtr(const char* nameNt, const void* p);
IMPORT_FUNC("jsPrintNamedStr") void jsPrintNamedStr(const char* nameNt, const char* s, size_t n);

void jsPrintStrNt(const char* nullTermStr)
{
	size_t length = (nullTermStr != nullptr) ? strlen(nullTermStr) : 0;
	jsPrintStr(nullTermStr, length);
}
#define jsPrintStrLit(strLit) jsPrintStr(strLit, sizeof(strLit)-1)

void jsPrintNamedStrNt(const char* nameNt, const char* nullTermStr)
{
	size_t length = (nullTermStr != nullptr) ? strlen(nullTermStr) : 0;
	jsPrintNamedStr(nameNt, nullTermStr, length);
}
#define jsPrintNamedStrLit(nameNt, strLit) jsPrintNamedStr((nameNt), strLit, sizeof(strLit)-1)

#endif //  _JS_BINDINGS_H
