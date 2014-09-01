#General defines
CC = gcc
LEX = flex
YACC = bison
LIB = -lc -lfl
IN_LEX = prac3.l
IN_YACC = prac3.y

OBJ = prac3.o prac3l.o prac3y.o
SRC = prac3.c
SRCL = prac3l.tmp.c
SRCY = prac3y.tmp.c
SRCH = prac3.h
SRCSYM = symtab.c
YHEADER = prac3y.h
LIBS = fgs_stack.c fgs_interpreter_ops.c fgs_file_treatment.c fgs_prints.c

INTC = fgs.c
INTH = fgs.h

INTTESTC = fgs_copia.c
INTHCOPY = fgs_copia.h

BIN = prac3.exe
INTBIN = fgs.exe
INTTEST = fgs_copia.exe

LFLAGS = -n -o $(SRCL)
YFLAGS = -d -v -o $(SRCY) --defines=$(YHEADER)
CFLAGS = -ansi -g
OTHERS = prac3y.h prac3y.tmp.output log.txt code.c3a output.byt *.stackdump *.bfgs

FIBON = example_fibonacci_complexe.txt

EXEMPLE = <prova_exemple.txt >output/output_exemple.txt
EXEMPLE2 = <prova_exemple_2.txt >output/output_exemple_2.txt
EXEMPLE_EXTRES = <prova_funcions_extres.txt >output/output_extres.txt
EXEMPLE_ERRORS_OP = <prova_errors_operacions.txt >output/output_exemple_errors_op.txt
EXEMPLE_ERRORS_FL = <prova_errors_fluxe.txt >output/output_exemple_errors_fl.txt

all : compile

testfgs : compile compile_copia
	./$(INTBIN)

testfgscopia : compile_copia
	./$(INTBINCOPY)
	
compile : $(SRCL) $(SRCY)
	$(CC) -o $(INTTEST) $(CFLAGS) $(SRC) $(SRCL) $(SRCY) $(SRCSYM) $(INTC) $(INTTESTC) $(LIBS)
	
valgrind : compile_copia
	valgrind --tool=memcheck --leak-check=yes ./$(INTBINCOPY)

$(SRCL) : $(IN_LEX)
	$(LEX) $(LFLAGS) $<

$(SRCY) : $(IN_YACC)
	$(YACC) $(YFLAGS) $<

clean : 
	rm -f *~ $(BIN)* $(OTHERS) $(SRCL) $(SRCY) $(INTBINCOPY) $(INTBIN) output/*
