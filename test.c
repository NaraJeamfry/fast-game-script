#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "fgs.h"

int test;

int testFunction(fgs_state * FGS, char * function)
{
	int i, result;
	if(test)
		printf("Now, we're gonna try to load %s function\n", function);

	for(i=0; i<1000; i++)
	{
		fgs_call_function(FGS, function);
		result += pop_valueI(FGS);
	}

	if(!result)
	{
		printf("Function %s failed test.\n", function);
	}
	else
	{
		if(test)
			printf("Function %s passed test.\n", function);
	}

	if(test)
		printf("--------------\n");
}

void testFunctionI(fgs_state *FGS, char * function, int expect)
{
	int i, result;
	if(test)
		printf("Now, we're gonna try to load %s function\n", function);

	for(i=0; i<1000; i++)
	{
		fgs_call_function(FGS, function);
		result += pop_valueI(FGS);
	}

	if(result != expect)
	{
		printf("Function %s failed test.\n", function);
	}
	else
	{
		if(test)
			printf("Function %s passed test.\n", function);
	}

	if(test)
		printf("--------------\n");
}

int main(int argc, char **argv)
{
	if(verbose)
		fprintf(stdout, "Benvingut a l'intèrpret de FastGameScript!\n\n");

	int result;
	char c;
	
	while((c = getopt(argc, argv, "vgsdt")) != -1)
	{
		if(test)
			fprintf(stdout, "Found option %c\n", c);
		switch(c)
		{
			case 'v':
				verbose = 1;
				break;
			case 'g':
				bisonverbose = 1;
				break;
			case 's':
				stackverbose = 1;
				break;
			case 'd':
				debug = 1;
				break;
			case 't':
				test = 1;
				break;
			case '?':
				printf("Unknown option %c.", c);
				exit(3);
				break;
			default: 
				printf("This shouldn't happen?");
				exit(99);
				break;
		}
	}
	
	fgs_state *FGS;
	
	FGS = fgs_start_context();
	
	frame * basicFrame;
	
	if(test)
		printf("--------------\n");

	fgs_load_script(FGS, "test/prova_condicional_else.fgs");
	fgs_load_script(FGS, "test/prova_condicional_if.fgs");
	fgs_load_script(FGS, "test/prova_aritmetica_mul.fgs");
	fgs_load_script(FGS, "test/prova_aritmetica_sum.fgs");
	fgs_load_script(FGS, "test/prova_fibonacci.fgs");
	/*fgs_load_script(FGS, "test/prova_condicional_if.fgs");
	fgs_load_script(FGS, "test/prova_condicional_if.fgs");
	fgs_load_script(FGS, "test/prova_condicional_if.fgs");
	*/
		


	result +=testFunction(FGS, "prova_condicional_else");
	result +=testFunction(FGS, "prova_condicional_if");
	result +=testFunction(FGS, "prova_aritmetica_mul");
	result +=testFunction(FGS, "prova_aritmetica_sum");
	result +=testFunctionI(FGS, "prova_fibonacci", 514229);
	/*result +=testFunction(FGS, "prova_condicional_if");
	result +=testFunction(FGS, "prova_condicional_if");
	result +=testFunction(FGS, "prova_condicional_if");
	result +=testFunction(FGS, "prova_condicional_if");*/
	
	destroy_context(FGS);
	return 0;
}