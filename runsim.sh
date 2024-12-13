export LD_LIBRARY_PATH=$PWD/install/lib:$LD_LIBRARY_PATH
ddsim --compactFile compact/simple_detector.xml  --runType qt --macroFile vis.mac
