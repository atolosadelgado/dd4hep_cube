Source the paths, for example, from cvmfs

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

To run the simulation of 1GeV alpha particle we can execute this:

```shell
ddsim --compactFile compact/simple_detector.xml --runType qt --enableGun --gun.particle alpha --gun.energy 1*GeV  -N 1 --outputFile example_edm4hep.root --macroFile vis.mac --gun.distribution uniform --gun.isotrop true
```

The track can be something like ![A track of 10GeV alpha in lead](https://mattermost.web.cern.ch/files/f1tnt4n4n7dabk7zpbnwxbyxdc/public?h=usG4tmiWGuhWBAujICrw-K5bv63s6TR0izjSpG4CvjM)

If we inspect the output rootfile,

```shell
root example_edm4hep.root
```

There is only one entry (1 event), so we can show the contents

```cpp
events->Show(0)
======> EVENT:0
 MCParticles     = (vector<edm4hep::MCParticleData>*)0xbaae4d0
 MCParticles.PDG = 1000020040
 MCParticles.generatorStatus = 1
 MCParticles.simulatorStatus = 150994944
 MCParticles.charge = 2.000000
 MCParticles.time = 0.000000
 MCParticles.mass = 3.72738
 MCParticles.vertex.x = 0
 MCParticles.vertex.y = 0
 MCParticles.vertex.z = 0
 MCParticles.endpoint.x = -0.910603
 MCParticles.endpoint.y = -1.9604
 MCParticles.endpoint.z = 0.601719
 MCParticles.momentum.x = -0.428134
 MCParticles.momentum.y = -0.854188
 MCParticles.momentum.z = 0.295066
 MCParticles.momentumAtEndpoint.x = -0.000000
 MCParticles.momentumAtEndpoint.y = -0.000000
 MCParticles.momentumAtEndpoint.z = 0.000000
 MCParticles.spin.x = 0.000000
 MCParticles.spin.y = 0.000000
 MCParticles.spin.z = 0.000000
 MCParticles.colorFlow.a = 0
 MCParticles.colorFlow.b = 0
 MCParticles.parents_begin = 0
 MCParticles.parents_end = 0
 MCParticles.daughters_begin = 0
 MCParticles.daughters_end = 0
 MY_HITS         = (vector<edm4hep::SimTrackerHitData>*)0xf1fb900
 MY_HITS.cellID  = 1
 MY_HITS.EDep    = 0.131812
 MY_HITS.time    = 0.015610
 MY_HITS.pathLength = 2.246921
 MY_HITS.quality = 0
 MY_HITS.position.x = -0.455302
 MY_HITS.position.y = -0.980202
 MY_HITS.position.z = 0.30086
 MY_HITS.momentum.x = -0.214067
 MY_HITS.momentum.y = -0.427094
 MY_HITS.momentum.z = 0.147533
 MY_HITS#0       = (vector<podio::ObjectID>*)0xe405300
 MY_HITS#0.index = 0
 MY_HITS#0.collectionID = 3
 EventHeader     = (vector<edm4hep::EventHeaderData>*)0xc526b00
 EventHeader.eventNumber = 0
 EventHeader.runNumber = 0
 EventHeader.timeStamp = 1675937276
 EventHeader.weight = 0.000000
 PARAMETERS      = (podio::GenericParameters*)0xfd8e4f0
```
