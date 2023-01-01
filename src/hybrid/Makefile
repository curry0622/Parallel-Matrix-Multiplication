CC = mpicc
CXX = mpicxx
LDLIBS = -lpng
CFLAGS = -lm -O3 -march=native -pipe -fopenmp
CXXFLAGS = $(CFLAGS)
TARGETS = mm_hybrid

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	rm -f $(TARGETS) $(TARGETS:=.o)
