NVFLAGS := -std=c++11 -Xptxas="-v" -arch=sm_61 
CFLAGS  := -O3
XCFLAGS := -fopenmp
LDFLAGS := -lm
EXES    := mm_2gpus

all: $(EXES)

clean:
	rm -f $(EXES)

mm_2gpus: mm_2gpus.cu
	nvcc $(NVFLAGS) $(CFLAGS) $(LDFLAGS) -Xcompiler="$(XCFLAGS)" -o $@ $?
