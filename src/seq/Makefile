CC = gcc
CXX = g++
CXXFLAGS = -O3 -lm -march=native -pipe
CFLAGS = -O3 -lm -march=native -pipe
TARGETS = mm_seq
TESTCASE_PATH = /home/pp22/pp22s69/share/testcases

.PHONY: all
all: $(TARGETS)

verify: $(TARGETS)
	./mm_seq $(TESTCASE_PATH)/$(t) tmp.out
	python3 ../../scripts/verify.py -f1 $(TESTCASE_PATH)/$(firstword $(subst ., ,$(t))).out -f2 tmp.out

judge: $(TARGETS)
	python3 ../../scripts/judge.py -e ./mm_seq -t $(TESTCASE_PATH)

.PHONY: clean
clean:
	rm -f $(TARGETS)
