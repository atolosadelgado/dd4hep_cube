#include "DD4hep/DetFactoryHelper.h"
#include "DD4hep/OpticalSurfaces.h"
#include "DD4hep/Printout.h"
#include "DDRec/DetectorData.h"
#include <XML/Helper.h>

using namespace dd4hep;
using namespace dd4hep::rec;
using dd4hep::SubtractionSolid;

// create the detector
static Ref_t createDetector(Detector &desc, xml::Handle_t handle, SensitiveDetector sens)
{
  xml::DetElement detElem = handle;
  std::string detName = detElem.nameStr();
  int detID = detElem.id();
  DetElement det(detName, detID);
  sens.setType("tracker");
  auto zpos = detElem.attr<double>(_Unicode(zpos));

  // We define our own mother volume (vacuum)
  // that will be inside of world (made of air)
  Box motherSolid(20 * cm / 2.,
                  20 * cm / 2.,
                  20 * cm / 2.);
  Volume motherVol("myworld", motherSolid, desc.material("Vacuum"));
  motherVol.setVisAttributes(desc.visAttributes("no_vis"));



  // How to define one detector:
  {
    // Define geometrical shape
    Box siSolid(19 * cm / 2.,
                19 * cm / 2.,
                19 * cm / 2.);
    // Define logical volume (geometry+material)
    Volume siVol(detName + "_R", siSolid, desc.material("Lead"));
      siVol.setVisAttributes(desc.visAttributes("sensor_vis"));
      siVol.setSensitiveDetector(sens);
      siVol.setLimitSet(desc, detElem.limitsStr());

    // Place it inside the mother volume
    PlacedVolume siPV = motherVol.placeVolume(siVol, Position(0, 0, zpos));
    
    // Add bitfield module to this particular detector
    siPV.addPhysVolID("module", 0);
  }

  // Place our mother volume in the world
  Volume wVol = desc.pickMotherVolume(det);
  PlacedVolume mPV = wVol.placeVolume(motherVol, Position(0, 0, 0));

  // Assign the system ID to our mother volume
  mPV.addPhysVolID("system", detID);

  // Set the physical volumes of the detector element. 
  det.setPlacement(mPV);
  return det;
}

DECLARE_DETELEMENT(MYCUBE, createDetector)
