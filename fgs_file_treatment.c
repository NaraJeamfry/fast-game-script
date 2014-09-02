#include <stdlib.h>
#include <sys/stat.h>
#include "fgs.h"
#include "fgs_internals.h"

void addExtensionIfNeeded(char * name)
{
	char *extension, *auxName;
	extension = strrchr(name, '.');
	if(extension && !strcmp(extension, ".fgs"))
	{
		auxName = malloc((extension-name) + 5);
		if(!auxName)
		{
			printf("WOW WO WOW\n");
			exit(3);
		}
		strcpy(auxName, name);
		extension = (auxName + (extension-name)+1);
		extension[0] = 'b';
		extension[1] = 'f';
		extension[2] = 'g';
		extension[3] = 's';
		extension[4] = '\0';
		
		/* free(name); */
		name = auxName;
	}
	else if(!extension)
	{
		auxName = malloc(strlen(name)+5);
		strcpy(auxName, name);
		strcat(auxName, ".fgs");
		/* free(name);  */
		name = auxName;
	}
}

FILE * open_file(char * name)
{
	FILE * file;
	
	file = fopen(name, "rb");
	
	if(file==NULL)
	{
		fprintf(stdout, "Error while opening file %s\n", name);
		exit(1);
	}
	
	return file;
}

unsigned char * parse_file(fgs_state * fgs, char * filename)
{
	int lSize;
	unsigned char * result;
	
	FILE * fi;
	
	fi = open_file(filename);
	
	fseek(fi, 0, SEEK_END);
	lSize = ftell(fi);
	rewind(fi);
	
	result = malloc(sizeof(char)*lSize);
	if(result==NULL)
	{
		fprintf(stdout, "--parse_file-- Could not allocate this much memory.\n");
		exit(3);
	}
	
	int i = fread(result, 1, lSize, fi);
	if(i!=lSize)
	{
		if(verbose)
		fprintf(stdout, "--parse_file-- I have read the same number of bytes that the file has but something went wrong.\n");
	}
	
	fclose(fi);
	
	return result;
}

void add_loaded_file(fgs_state * fgs, char * name, char * code)
{
	time_t aux;
	int found = 0;
	
	bfgsfile * compiled;
	bfgsfile * last = NULL, *auxFile = fgs_get_codes(fgs);
	
	if(time(&aux)==-1)
	{
		/* treat time error */
		return;
	}
	
	while(!auxFile && found)
	{
		if(!strcmp(auxFile->name, name))
		{
			found = 1;
			last = auxFile;
		}
		else
		{
			last = auxFile;
			auxFile = auxFile->next;
		}
	}
	
	if(found)
	{
		last->load_time = aux;
		free(last->code);
		last->code = code;
	}
	else
	{
		compiled = malloc(sizeof(bfgsfile));
		compiled->load_time = aux;
		compiled->name = malloc(strlen(name)+1);
		if(compiled->name == NULL)
		{
			printf("Couldn't allocate memory");
			exit(3);
		}
		strcpy(compiled->name, name);
		compiled->code = code;
		compiled->next = NULL;
		
		if(last)
			last->next = compiled;
		else
			fgs_set_code(fgs, compiled);
	}
}

/**	Changes fgs extension to bfgs in a string.
	changeSourceToByteName
	
	Takes the file name from the name parameter and changes its extension to the
	bytecode file extension (bfgs).
	
	\param dest NULL pointer to which the new filename will be malloc'ed. Needs to be freed.
	\param name String containing the original filename (ends with fgs).
*/
void changeSourceToByteName(char ** dest, char * name)
{
	if(!name)
	{
		print("Filename is empty.\n");
		return;
	}
	
	char * pch = strrchr(name, '.');
	
	if(!pch)
	{
		print("File does not have a correct extension.\n");
		return;
	}
	
	*dest = malloc(sizeof(char) * (pch-name+1+5));
	strcpy(*dest, name);
	pch = strrchr(*dest, '.');
	pch[1] = 'b';
	pch[2] = 'f';
	pch[3] = 'g';
	pch[4] = 's';
	pch[5] = '\0';
	
	if(verbose)
		printf("--changeSourceToByteName-- Correctly changed %s to %s.\n", name, *dest);
}

void compileFile(fgs_state *fgs, char * name)
{
	FILE * source;
	char * pch, *output;

	addExtensionIfNeeded(name);
	
	if(verbose)
		printf("--compileFile-- Checking if %s is already compiled...\n", name);
		
	if(fileAlreadyCompiled(fgs_get_files(fgs), name))
	{
		if(verbose)
			printf("--compileFile-- File is up to date.\n\n", name);
		return;
	}
	
	if(verbose)
		fprintf(stdout, "--compileFile-- Trying to open \"%s\"\n", name);
	
	source = fopen(name, "r");
	
	if(!source)
	{
		printf("couldn't open file \"%s\" for reading\n", name);
		exit(1);
	}
	
	changeSourceToByteName(&output, name);
	
	if(verbose)
		fprintf(stdout, "--compileFile-- Trying to save on \"%s\"\n", output);
	
	parseFile(source, output);
	
	fclose(source);
	
	if(verbose)
		printf("--compileFile-- Adding file to compiled files on actual state\n");
	add_compiled_file(fgs, name);
	
	free(output);
}

/**	Adds a file to the compilated list on the state fgs.
	
	This function will check the actual time and add it to the compilation
	info for the file. If the file does exist, it will update the compilation time.
	
	\param fgs The FGS state
	\param name The name of the file
*/
void add_compiled_file(fgs_state *fgs, char *name)
{
	time_t aux;
	int found = 0;
	
	fgsfile * compiled;
	fgsfile * last = NULL, *auxFile = fgs_get_files(fgs);
	
	if(time(&aux)==-1)
	{
		/* treat time error */
		return;
	}
	
	while(!auxFile && found)
	{
		if(!strcmp(auxFile->name, name))
		{
			found = 1;
			last = auxFile;
		}
		else
		{
			last = auxFile;
			auxFile = auxFile->next;
		}
	}
	
	if(found)
	{
		last->compilation_time = aux;
	}
	else
	{
		compiled = malloc(sizeof(fgsfile));
		compiled->compilation_time = aux;
		compiled->name = malloc(strlen(name)+1);
		strcpy(compiled->name, name);
		compiled->next = NULL;
		
		if(last)
			last->next = compiled;
		else
			fgs_set_file(fgs, compiled);
	}
	
}

int fileAlreadyLoaded(bfgsfile *filestruct, char * name)
{
	bfgsfile *aux = filestruct;
	time_t actual, compilation;
	struct stat file_stat;
	int found = 0;
	
	while(aux && !found)
	{
		if(!strcmp(aux->name, name))
			found = 1;
		else
			aux = aux->next;
	}
	
	if(!aux)
	{
		/* if(verbose) */
			printf("--fileAlreadyLoaded-- File is not loaded\n");
		return 0;
	}
	
	compilation = aux->load_time;
	
	if(stat(name, &file_stat))
	{
		perror("--fileAlreadyCompiled-- stat");
		exit(3);
	}
	
	if(file_stat.st_mtime > compilation)
	{
		printf("--fileAlreadyLoaded-- File needs reload\n");
		return 0;
	}
	else
	{
		return 1;
	}
}

int fileAlreadyCompiled(fgsfile *filestruct, char * name)
{
	
	addExtensionIfNeeded(name);
	
	fgsfile *aux = filestruct;
	time_t actual, compilation;
	struct stat file_stat;
	int found = 0;
	
	while(aux && !found)
	{
		if(!strcmp(aux->name, name))
			found = 1;
		else
			aux = aux->next;
	}
	
	if(!aux)
	{
		/* if(verbose) */
			printf("--fileAlreadyCompiled-- File is not compiled\n");
		return 0;
	}
	
	compilation = aux->compilation_time;
	
	if(stat(name, &file_stat))
	{
		perror("--fileAlreadyCompiled-- stat");
		exit(3);
	}
	
	if(file_stat.st_mtime > compilation)
	{
		printf("--fileAlreadyCompiled-- File needs compilation\n");
		return 0;
	}
	else
	{
		return 1;
	}
	
}