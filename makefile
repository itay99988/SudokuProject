CC = gcc
OBJS = main.o game.o mainAux.o parser.o solver.o stack.o undoList.o tools.o ILPSolver.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra \
-Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS)  $(GUROBI_LIB) -o $@ -lm

main.o: main.c game.h SPBufferset.h
	$(CC) $(COMP_FLAG) -c $*.c
game.o: game.h undoList.h mainAux.h solver.h parser.h
	$(CC) $(COMP_FLAG) -c $*.c
mainAux.o: mainAux.h game.h solver.h tools.h
	$(CC) $(COMP_FLAG) -c $*.c
parser.o: parser.h game.h solver.h undoList.h tools.h mainAux.h ILPSolver.h
	$(CC) $(COMP_FLAG) -c $*.c
solver.o: solver.h game.h stack.h mainAux.h ILPSolver.h
	$(CC) $(COMP_FLAG) -c $*.c
stack.o: stack.h
	$(CC) $(COMP_FLAG) -c $*.c
undoList.o: undoList.h
	$(CC) $(COMP_FLAG) -c $*.c
tools.o: tools.h game.h solver.h
	$(CC) $(COMP_FLAG) -c $*.c
ILPSolver.o: ILPSolver.h game.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
