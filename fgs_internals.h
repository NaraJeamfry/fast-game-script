
fgsfile * fgs_get_files(fgs_state *);
void fgs_set_file(fgs_state *, fgsfile *);

bfgsfile * fgs_get_codes(fgs_state *);
void fgs_set_code(fgs_state *, bfgsfile *);

/* State Management */

void initialize_state(fgs_state *);

/* File Treatment */
void add_compiled_file(fgs_state *, char * filename);
int fileAlreadyCompiled(fgsfile *, char * filename);

void compileFile(fgs_state *, char * filename);
unsigned char * parse_file(fgs_state *, char *);