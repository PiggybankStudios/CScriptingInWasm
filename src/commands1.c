/*
File:   commands1.c
Author: Taylor Robbins
Date:   10\04\2026
Description: 
	** None
*/

#include "common.h"

COMMAND(test1, "description1");
COMMAND(test2, "description2");
COMMAND(test3, "description3");
COMMAND(test4, "description4");
COMMAND(test5, "description5");

__attribute__((__section__("custom_section"))) int thingInCustomSectionD = 0xDDDDDDDD;
