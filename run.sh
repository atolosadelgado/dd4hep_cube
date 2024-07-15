cmake -B build -S . -D CMAKE_INSTALL_PREFIX=install
cmake --build build -j 7 --target install
export LD_LIBRARY_PATH=$PWD/install/lib:$LD_LIBRARY_PATH
ddsim --compactFile compact/simple_detector.xml -N 1 --runType qt --macroFile vis.mac --enableG4GPS
