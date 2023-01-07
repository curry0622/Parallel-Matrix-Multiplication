NVFLAGS := -std=c++11 -Xptxas="-v" -arch=sm_61 
CFLAGS  := -O3
LDFLAGS := -lm
TARGETS := mm_1gpu
TESTCASE_PATH = /home/pp22/pp22s69/share/testcases

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

mm_1gpu: mm_1gpu.cu
	nvcc $(NVFLAGS) $(LDFLAGS) -o $@ $?

verify: $(TARGETS)
	./mm_1gpu $(TESTCASE_PATH)/$(t) tmp.out
	python3 ../../scripts/verify.py -f1 $(TESTCASE_PATH)/$(firstword $(subst ., ,$(t))).out -f2 tmp.out

judge: $(TARGETS)
	python3 ../../scripts/judge.py -e ./mm_1gpu -t $(TESTCASE_PATH) -gpu 1
