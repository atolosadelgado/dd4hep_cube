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
  sens.setType("OpticalTracker");

  // Create the mother Detector element to be returned at the end
  DetElement det(detName, detID);

  // How to define one detector:
  // Define geometrical shape
  double sizexyz = 50 * cm;
  Box laboratoryShape( sizexyz ,sizexyz ,sizexyz );
  // Define volume (shape+material)
  Volume laboratoryVol(detName +"_laboratory", laboratoryShape, desc.material("AirOptical"));
  laboratoryVol.setVisAttributes(desc.visAttributes("laboratory_vis"));
  laboratoryVol.setSensitiveDetector(sens);
  
  {
  Box sensorShape( sizexyz / 2.,sizexyz / 2.,sizexyz / 2.);
  // Define volume (shape+material)
  Volume sensorVol(detName +"_sensor", sensorShape, desc.material("SiliconOptical"));
  sensorVol.setVisAttributes(desc.visAttributes("sensor_vis"));
  laboratoryVol.placeVolume(sensorVol);	
  }

  // Place our mother volume in the world
  Volume wVol = desc.pickMotherVolume(det);

  // setup mother volume as transparent.
  // This is just for example, bad practice in general!
  wVol.setVisAttributes(desc.visAttributes("no_vis"));


  PlacedVolume laboratoryPV = wVol.placeVolume(laboratoryVol);

  // Assign the system ID to our mother volume
  laboratoryPV.addPhysVolID("system", detID);

  // Associate the silicon Placed Volume to the detector element.
  det.setPlacement(laboratoryPV);

  return det;
}

DECLARE_DETELEMENT(MYCUBE, createDetector)
