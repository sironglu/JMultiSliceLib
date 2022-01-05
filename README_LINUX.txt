module load cuda intel
git clone https://github.com/sironglu/JMultiSliceLib.git
cd JMultiSliceLib/
patch -p2 < patch_for_linux.diff
cd src
make -j 8
cd ../test/JMSBench1/
make -j 8
