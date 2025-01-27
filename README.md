#HOW TO DD4hep
==============

## Compile project

Source the paths to the commands of DD4hep and its dependencies, for example, from cvmfs

```shell
source /cvmfs/sw-nightlies.hsf.org/key4hep/setup.sh -r 2025-01-20
```

To compile,
```shell
cmake -B build -S . -D CMAKE_INSTALL_PREFIX=install
cmake --build build -- install
```

After that, the install directory will look like this:

```shell
$ tree install/
install/
└── lib
    ├── libsimple_example.components
    └── libsimple_example.so

1 directory, 2 files
```

To let the programs know where to find our freshly built detector, we have to update this env variable

```shell
export LD_LIBRARY_PATH=$PWD/install/lib:$LD_LIBRARY_PATH
```

## Things to do after every change

Compile and install after every modification of the c++ detector constructor code.

```bash
cmake --build build -- install
```

## Visualize and debug geometry

To display the geometry

```bash
geoDisplay ./compact/simple_detector.xml
```

To convert the geometry into ROOT

```bash
./dd4hep2root -c  ./compact/simple_detector.xml -o arc_v0.root
```


To show materials in along a given line (in this case from origin to (0,0,-100)cm), check if the modified volume has the proper material

```bash
materialScan ./compact/simple_detector.xml 0 0 -30 0 0 30
```

Possible issue if `USE_GEANT4_UNITS` enabled when building DD4hep.

Check overlaps, using Geant4 check
```shell
ddsim --compactFile ./compact/simple_detector.xml --runType run --part.userParticleHandler='' --macroFile overlap.mac >> overlapDump.txt
```

## Run simulation and crosscheck

Example of how to build and test a pair of endcaps

```
source /cvmfs/sw-nightlies.hsf.org/key4hep/setup.sh -r 2025-01-20
cmake -B build -S . -D CMAKE_INSTALL_PREFIX=install
cmake --build build -j 8 -- install
export LD_LIBRARY_PATH=$PWD/install/lib:$PWD/install/lib64:$LD_LIBRARY_PATH
geoDisplay compact/simple_detector.xml
ddsim --compactFile compact/simple_detector.xml -N 10 -G --gun.position "(0,0,-50*cm)" --gun.direction "0,0,1" --outputFile kk.root
python3 myreader.py
```
