IDIR =.
CC=g++
CFLAGS=-Wall -g
OMPFLAGS=-fopenmp


_DEPS = population.h individual.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ_OMP = tspga-omp.o individual-omp.o population-omp.o
OBJ_OMP = $(patsubst %,%,$(_OBJ_OMP))

_OBJ_SEQ = tspga-seq.o individual-seq.o population-seq.o
OBJ_SEQ = $(patsubst %,%,$(_OBJ_SEQ))

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(OMPFLAGS)

.PHONY: clean all config

TARGETS = sequential openmp

all: $(TARGETS)

sequential: $(OBJ_SEQ)
	$(CC) -o $@ $^ $(CFLAGS)

openmp: $(OBJ_OMP)
	$(CC) -o $@ $^ $(CFLAGS) $(OMPFLAGS)

clean:
	rm -f *.o *~ sequential openmp 
