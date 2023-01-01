CC = gcc
CXX = g++
LDLIBS = -lpng
CFLAGS = -lm -O3 -march=native -pipe -fopenmp
CXXFLAGS = $(CFLAGS)
TARGETS = mm_openmp
TESTCASE_PATH = /home/pp22/pp22s69/share/testcases

.PHONY: all
all: $(TARGETS)

verify: $(TARGETS)
	srun -N1 -n1 -c$(c) ./mm_openmp $(TESTCASE_PATH)/$(t) tmp.out
	python3 ../../scripts/verify.py -f1 $(TESTCASE_PATH)/$(firstword $(subst ., ,$(t))).out -f2 tmp.out

judge: $(TARGETS)
	python3 ../../scripts/judge.py -e ./mm_openmp -t $(TESTCASE_PATH) -c $(c)

.PHONY: clean
clean:
	rm -f $(TARGETS) $(TARGETS:=.o)
