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

  // Setup which kind of sensitive detector is
  sens.setType("calorimeter");

  // Create the mother Detector element to be returned at the end
  DetElement det(detName, detID);

  double twist_angle = 180*dd4hep::deg;
  double rmin = 5*dd4hep::cm;
  double rmax = 10*dd4hep::cm;
  double dz = 10*dd4hep::cm;
  double dphi = 90*dd4hep::deg;
  TwistedTube myshape( twist_angle,  rmin,  rmax,  dz,  dphi);
  // Define volume (shape+material)
  Volume siVol(detName +"_sensor", myshape, desc.material("Silicon"));
  siVol.setVisAttributes(desc.visAttributes("sensor_vis"));
  siVol.setSensitiveDetector(sens);


  // Place our mother volume in the world
  Volume wVol = desc.pickMotherVolume(det);

  PlacedVolume siPV = wVol.placeVolume(siVol);

  // Assign the system ID to our mother volume
  siPV.addPhysVolID("system", detID);

  // Associate the silicon Placed Volume to the detector element.
  det.setPlacement(siPV);

  return det;
}

DECLARE_DETELEMENT(MYCUBE, createDetector)
