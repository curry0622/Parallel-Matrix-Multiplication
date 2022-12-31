CC = gcc
CXX = g++
CXXFLAGS = -O3 -lm -march=native -pipe
CFLAGS = -O3 -lm -march=native -pipe
TARGETS = mm_seq

.PHONY: all
all: $(TARGETS)

ifeq (verify,$(firstword $(MAKECMDGOALS)))
  TESTCASE_NAME := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(TESTCASE_NAME):;@:)
endif

verify: $(TARGETS)
	@echo Verifying $(TESTCASE_NAME)
	./mm_seq ../../testcases/$(TESTCASE_NAME).in mine.out
	python3 ../../scripts/verify.py -f1 ../../testcases/$(TESTCASE_NAME).out -f2 mine.out -d 0

judge: $(TARGETS)
	for file in ../../testcases/*.in; do \
		./mm_seq $$file mine.out; \
		python3 ../../scripts/verify.py -f1 $${file%.in}.out -f2 mine.out -d 0; \
		rm -f mine.out; \
		echo; \
	done

.PHONY: clean
clean:
	rm -f $(TARGETS)
