#include <XML/Helper.h>
#include "DD4hep/DetFactoryHelper.h"
#include "DD4hep/Printout.h"
#include "DD4hep/Detector.h"

// create the detector
static dd4hep::Ref_t createDetector(dd4hep::Detector &desc, dd4hep::xml::Handle_t handle, dd4hep::SensitiveDetector sens)
{
  dd4hep::xml::DetElement detElem = handle;

  // Get detector name and ID from compact file
  std::string detName = detElem.nameStr();
  int detID = detElem.id();

  // Get z position from compact file
  auto zpos = detElem.attr<double>(_Unicode(zpos));

  // Setup which kind of sensitive detector is
  sens.setType("calorimeter");

  // Create the mother Detector element to be returned at the end
  dd4hep::DetElement det(detName, detID);


  /* Define atoms to be repeated along X and Y
   * within a cell
   * 1 atom = cube with side length of 'a'
   */
  double a = 1*dd4hep::um;
  dd4hep::Box atomSolid( a/2,a/2,a/2);
  // Define volume (shape+material)
  dd4hep::Volume aVol(detName +"_atomA", atomSolid, desc.material("Silicon"));
  aVol.setVisAttributes(desc.visAttributes("sensor_vis"));
  // siVol.setSensitiveDetector(sens);
  // siVol.setLimitSet(desc, detElem.limitsStr());
  dd4hep::Volume bVol(detName +"_atomB", atomSolid, desc.material("Silicon"));

  /* crystal cell, made of atoms A and B as in the picture:
   *   ---------
   *  | a  | b  |
   *  |____|____|
   *  |    |    |
   *  | b  | a  |
   *   ---------
   */
  dd4hep::Box cellSolid(a, a, a);
  dd4hep::Volume cellVol(detName +"_cell", cellSolid, desc.material("Air"));
  cellVol.placeVolume(aVol,dd4hep::Position(-a/2,a/2,0));
  cellVol.placeVolume(aVol,dd4hep::Position(a/2,-a/2,0));
  cellVol.placeVolume(bVol,dd4hep::Position(a/2,a/2,0));
  cellVol.placeVolume(bVol,dd4hep::Position(-a/2,-a/2,0));
  cellVol.setVisAttributes(desc.visAttributes("no_vis"));

  /* Create envelope to repeat 1000 times the cell along X axis
   *
   */
  dd4hep::Box kiloXcellSolid( 500*a,a,a);
  dd4hep::Volume kiloXcellVol(detName +"kiloXcell", kiloXcellSolid, desc.material("Air"));

  for( int nx = -500; nx<500; ++nx)
    kiloXcellVol.placeVolume(cellVol,dd4hep::Position( (nx+0.5)*a,0,0));

  /* Create envelope to repeat 1000 times the kiloXcell along Y axis
   * kiloXYcell will contain 1000x1000 times the original cell of A-B atoms
   */
  dd4hep::Box kiloXYcellSolid( 500*a,500*a,a);
  dd4hep::Volume kiloXYcellVol(detName +"kiloXYcell", kiloXYcellSolid, desc.material("Air"));
  for( int ny = -500; ny<500; ++ny)
    kiloXYcellVol.placeVolume(kiloXcellVol,dd4hep::Position( 0, (ny+0.5)*a,0));

  // Place our mother volume in the world
  dd4hep::Volume wVol = desc.pickMotherVolume(det);

  // setup mother volume as transparent.
  // This is just for example, bad practice in general!
  wVol.setVisAttributes(desc.visAttributes("no_vis"));


  dd4hep::PlacedVolume siPV = wVol.placeVolume(kiloXYcellVol, dd4hep::Position(0, 0, 0));

  // Assign the system ID to our mother volume
  siPV.addPhysVolID("system", detID);

  // Associate the silicon Placed Volume to the detector element.
  det.setPlacement(siPV);

  return det;
}

DECLARE_DETELEMENT(MYCUBE, createDetector)
