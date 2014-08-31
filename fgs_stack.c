#include <stdlib.h>
#include <stdio.h>
#include "fgs_stack.h"

stack * StackInit(int size)
{
	stack * result = malloc(sizeof(stack));
	
	result->top = -1;
	result->maxSize = size;
	
	return result;
}

char StackEmpty(stack * st)
{
	return (st->top < 0) ? 1 : 0;
}

char StackFull(stack * st)
{
/* 	fprintf(stdout, "Top: %d. Max Size: %d.", st->top, st->maxSize-1);
 */	char aux = (st->top >= st->maxSize-1) ? 1 : 0;
/* 	fprintf(stdout, "Result: %d.\n", aux);
 */	return aux;
	
}

char StackDestroy(stack * st)
{
	stacke * element;

	while(!StackEmpty(st))
	{
		element = st->contents->last;
		free(st->contents);
		st->contents = element;
	}
	
	st->contents = NULL;
	st->maxSize = 0;
	st->top = -1;
}

int StackPushI(stack * st, stacke * elem)
{
	stacke * element;
	if(!StackFull(st))
	{
		if(stackverbose)
			fprintf(stdout, "--StackPushI-- Correctly pushed value %d.\n", elem->value.literalI);
		element = st->contents;
		st->contents = malloc(sizeof(stacke));

		if(st->contents == NULL)
		{
			printf("Cannot allocate memory. Exiting...\n");
			exit(3);
		}
		
		st->contents->value.literalI = elem->value.literalI;
		st->contents->type=0;
		st->contents->last = element;
		st->top++;
	}
}

int StackPushF(stack * st, stacke * elem)
{
	stacke * element;
	if(!StackFull(st))
	{
		if(stackverbose)
			fprintf(stdout, "--StackPushF-- Correctly pushed value %d.\n", elem->value.literalI);
		element = st->contents;
		st->contents = malloc(sizeof(stacke));

		if(st->contents == NULL)
		{
			printf("Cannot allocate memory. Exiting...\n");
			exit(3);
		}
		st->contents->value.literalF = elem->value.literalF;
		st->contents->type = 1;
		st->contents->last = element;
		st->top++;
	}
}

int StackPushS(stack * st, stacke * elem)
{
	stacke * element;
	if(!StackFull(st))
	{
		if(stackverbose)
			fprintf(stdout, "--StackPushS-- Correctly pushed value %d.\n", elem->value.literalI);
		element = st->contents;
		st->contents = malloc(sizeof(stacke));
		if(st->contents == NULL)
		{
			printf("Cannot allocate memory. Exiting...\n");
			exit(3);
		}
		st->contents->value.literalS = elem->value.literalS;
		st->contents->type = 2;
		st->contents->last = element;
		st->top++;
	}
}

int StackPopI(stack * st, stacke * data)
{
	stacke * element;
	if(!StackEmpty(st))
	{
		element = st->contents;
		if(element->type == 1)
		{
			if(stackverbose)	printf("--StackPopI-- Warning: Unexpected format. Converting Float to Integer\n");
			element->value.literalI = (int)element->value.literalF;
			element->type = 0;
		}
		else if(element->type == 2)
		{
			if(stackverbose)	printf("--StackPopI-- Cannot convert from string\n");
			free(element->value.literalS);
			free(element);
			return 0;
		}
		st->contents = element->last;
		st->top--;

		if(memcpy(data, element, sizeof(stacke))==NULL)
		{
			if(stackverbose)	printf("--StackPopI-- Cannot pop value: Error copying memory.\n");
			free(element);
			return 0;		
		}
		free(element);

		if(stackverbose)
			fprintf(stdout, "--StackPopI-- Correctly poped value %d.\n", data->value.literalI);
		return 1;
	}
	if(stackverbose) printf("--StackPopI-- Cannot pop value: Stack is empty.\n");
	return 0;
}

int StackPopF(stack * st, stacke * data)
{
	stacke * element;
	if(!StackEmpty(st))
	{
		element = st->contents;
		if(element->type == 0)
		{
			if(stackverbose)	printf("--StackPopF-- Warning: Unexpected format. Converting Integer to Float\n");
			element->value.literalF = (float)element->value.literalI;
			element->type = 1;
		}
		else if(element->type == 2)
		{
			if(stackverbose)	printf("--StackPopF-- Cannot convert to float from string\n");
			free(element->value.literalS);
			free(element);
			return 0;
		}
		st->contents = element->last;
		st->top--;
	
		if(memcpy(data, element, sizeof(stacke))==NULL)
		{
			if(stackverbose)	printf("--StackPopF-- Cannot pop value: Error copying memory.\n");
			free(element);
			return 0;		
		}
		free(element);

		if(stackverbose)
			fprintf(stdout, "--StackPopF-- Correctly poped value %f.\n", data->value.literalF);
		return 1;
	}
	if(stackverbose) printf("--StackPopF-- Cannot pop value: Stack is empty.\n");
	return 0;
}

int StackPopS(stack * st, stacke * data)
{
	stacke * element;
	if(!StackEmpty(st))
	{
		element = st->contents;
		if(element->type == 0)
		{
			if(stackverbose)	printf("--StackPopS-- Unexpected type: converting int to string\n");
			
			int aux = element->value.literalI;
			element->value.literalS = malloc(sizeof(char)*11);
			sprintf(element->value.literalS, "%d", aux);
			element->type = 2;
		}
		else if(element->type == 1)
		{
			if(stackverbose)	printf("--StackPopS-- Unexpected type: converting float to string\n");
			
			float aux = element->value.literalI;
			element->value.literalS = malloc(sizeof(char)*20);
			if(element->value.literalS==NULL) 
			{
				printf("--StackPopS-- Error allocating memory. Exiting...\n");
				exit(3);
			}
			sprintf(element->value.literalS, "%f", aux);
			element->type = 2;
		}	
		st->contents = element->last;
		st->top--;

		if(memcpy(data, element, sizeof(stacke))==NULL)
		{
			if(stackverbose)	printf("--StackPopS-- Cannot pop value: Error copying memory.");
			free(element->value.literalS);
			free(element);
			return 0;		
		}
		free(element->value.literalS);
		free(element);

		if(stackverbose)
			fprintf(stdout, "--StackPopS-- Correctly poped value %s.\n", data->value.literalS);
		return 1;
	}
	if(stackverbose) printf("--StackPopS-- Cannot pop value: Stack is empty.\n");
	return 0;
}
