CPP = g++
CPPFLAGS = -Wall -O2 -fPIC 
LINK = -shared -fPIC 
LIB = -lcudart -lcufft  -L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl 

GENCODE_ARCH    := -gencode=arch=compute_30,code=\"sm_30,compute_30\" \
                   -gencode=arch=compute_35,code=\"sm_35,compute_35\" \
                   -gencode=arch=compute_50,code=\"sm_50,compute_50\"

cpp_objects = AtomSite.o Atoms.o JEElaSca.o JFFTCUDAcore.o JFFTMKLcore.o JFFTWcore.o JMultiSliceLib.o JMultiSlice.o JPlasmonMC.o JProbeGen.o M3D.o prime_numbers.o rng.o string_format.o Structure.o TextFileOps.o V3D.o wekoscat.o 
cu_objects = cu/ArrayOps.o cu/FFTCallbacks.o cu/scapot.o

all: libJMultiSlice.so libJMultiSlice.a

libJMultiSlice.so : $(cpp_objects) $(cu_objects)
	$(CPP) $(LINK)  -o $@ $^

libJMultiSlice.a : $(cpp_objects) $(cu_objects)
	ar rcs $@ $^
	ranlib $@

$(cpp_objects): %.o: %.cpp
	$(CPP) -c $(CPPFLAGS) $< -o $@

$(cu_objects): %.o: %.cu
	nvcc --compiler-options '$(CPPFLAGS)' $(GENCODE_ARCH) -c $< -o $@

.PHONY: clean
clean:
	rm *.o cu/*.o

