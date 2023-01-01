CC = gcc
CXX = g++
LDLIBS = -lpng
CFLAGS = -lm -O3 -march=native -pipe -fopenmp
CXXFLAGS = $(CFLAGS)
TARGETS = mm_openmp

ifeq (verify,$(firstword $(MAKECMDGOALS)))
  TESTCASE_NAME := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(TESTCASE_NAME):;@:)
endif

.PHONY: all
all: $(TARGETS)

verify: $(TARGETS)
	srun -N1 -n1 -c$(c) ./mm_openmp ../../testcases/$(TESTCASE_NAME).in tmp.out
	python3 ../../scripts/verify.py -f1 ../../testcases/$(TESTCASE_NAME).out -f2 tmp.out

judge: $(TARGETS)
	python3 ../../scripts/judge.py -e ./mm_openmp -t ../../testcases -c $(c)

.PHONY: clean
clean:
	rm -f $(TARGETS) $(TARGETS:=.o)
