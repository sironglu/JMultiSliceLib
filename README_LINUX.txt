# This fork is for building JMultiSliceLib & JMSBench1 under Linux with CUDA support. 
# Three files are added:
# /patch_for_linux.diff
# /src/Makefile
# /test/JMSBench1/Makefile

# How to build:
module load cuda intel

git clone https://github.com/sironglu/JMultiSliceLib.git

cd JMultiSliceLib/
patch -p2 < patch_for_linux.diff

cd src
make -j 8
cd ../test/JMSBench1/
make -j 8

# If there is an error:
# Load fftw module if fftw3.h is missing. 
# Load gcc/8, or add -std=c++11 after g++ in the Makefile if there is an error: ‘constexpr’ does not name a type. 
# On my cluster, I do
# module load cuda/11.2 intel/2020.4 gcc/8.2.0 fftw/3.3.8
# If cuda_runtime.h is still missing:
# export CPLUS_INCLUDE_PATH=${INCLUDE}

# Please modify the two Makefiles for performance tuning. 
