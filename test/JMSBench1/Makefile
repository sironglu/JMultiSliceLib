CPP = g++
CPPFLAGS = -Wall -O2 
INC = -I../../src/
LIB = ../../src/libJMultiSlice.a -lcudart -lcufft -L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl 

objects = JMSBench1.o multislice.o params.o perfclock.o prm_detector.o prm_main.o prm_probe.o prm_result.o prm_sample.o prm_scan.o prm_slice_file_info.o prm_slice.o

JMSBench1 : $(objects) 
	$(CPP) -o $@ $(objects) $(LIB)

all: JMSBench1

$(objects): %.o: %.cpp
	$(CPP) -c $(CPPFLAGS) $(INC) $< -o $@

.PHONY: clean
clean:
	rm *.o
