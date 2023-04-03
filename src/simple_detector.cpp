#include "DD4hep/DetFactoryHelper.h"
#include "DD4hep/OpticalSurfaces.h"
#include "DD4hep/Printout.h"
#include "DDRec/DetectorData.h"
#include <XML/Helper.h>

using namespace dd4hep;

// create the detector
static Ref_t createDetector(Detector &desc, xml::Handle_t handle, SensitiveDetector sens)
{
  xml::DetElement detElem = handle;

  // Get detector name and ID from compact file
  std::string detName = detElem.nameStr();
  int detID = detElem.id();

  // Get z position from compact file
  auto zpos = detElem.attr<double>(_Unicode(zpos));

  // Setup which kind of sensitive detector is
  sens.setType("calorimeter");

  // Create the mother Detector element to be returned at the end
  DetElement det(detName, detID);

  // How to define one detector:
  // Define geometrical shape
  Box siSolid(19 * cm / 2.,
              19 * cm / 2.,
              19 * cm / 2.);
  // Define volume (shape+material)
  Volume siVol(detName +"_sensor", siSolid, desc.material("Silicon"));
  siVol.setVisAttributes(desc.visAttributes("sensor_vis"));
  siVol.setSensitiveDetector(sens);
  siVol.setLimitSet(desc, detElem.limitsStr());


  // Place our mother volume in the world
  Volume wVol = desc.pickMotherVolume(det);

  // setup mother volume as transparent.
  // This is just for example, bad practice in general!
  wVol.setVisAttributes(desc.visAttributes("no_vis"));


  PlacedVolume siPV = wVol.placeVolume(siVol, Position(0, 0, zpos));

  // Assign the system ID to our mother volume
  siPV.addPhysVolID("system", detID);

  // Associate the silicon Placed Volume to the detector element.
  det.setPlacement(siPV);

  return det;
}

DECLARE_DETELEMENT(MYCUBE, createDetector)
