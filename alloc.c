#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Q1Q2.h"
#include "structs.h"
#include "Q3Q4Q5Q6Q7.h"
#include "alloc.h"


/* memory alloc check */
void allocCheck(void* address)
{
	if (address == NULL)
	{
		printf("Memory alloc failiere!");
		exit(1);
	}
}