NVFLAGS := -std=c++11 -Xptxas="-v" -arch=sm_61 
CFLAGS  := -O3
LDFLAGS := -lm
EXES    := mm_1gpu

all: $(EXES)

clean:
	rm -f $(EXES)

mm_1gpu: mm_1gpu.cu
	nvcc $(NVFLAGS) $(LDFLAGS) -o $@ $?
