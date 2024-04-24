#HOW TO BUILD THE GEOMETRY TREE AS MATRIOSKA
============================================

## Compile project

Source the paths to the commands of DD4hep and its dependencies, for example, from cvmfs

```shell
source /cvmfs/sft.cern.ch/lcg/views/dev4/latest/x86_64-centos7-gcc11-opt/setup.sh 
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
To use the Geant4+Qt

```shell
ddsim --compactFile compact/simple_detector.xml --runType qt --macroFile vis.mac
```


Check overlaps, using Geant4 check
```shell
ddsim --compactFile ./compact/simple_detector.xml --runType run --part.userParticleHandler='' --macroFile overlap.mac >> overlapDump.txt
```

