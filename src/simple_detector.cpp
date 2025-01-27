#include "DD4hep/DetFactoryHelper.h"
#include "DD4hep/OpticalSurfaces.h"
#include "DD4hep/Printout.h"
#include "DDRec/DetectorData.h"
#include <XML/Helper.h>


// create the detector
static dd4hep::Ref_t createDetector(dd4hep::Detector &desc, dd4hep::xml::Handle_t handle, dd4hep::SensitiveDetector sens)
{
  dd4hep::xml::DetElement detElem = handle;

  // Get detector name and ID from compact file
  std::string detName = detElem.nameStr();
  int detID = detElem.id();

  // Get z position from compact file
  auto zpos = detElem.attr<double>(_Unicode(zpos));
  auto size = detElem.attr<double>(_Unicode(size));

  // Setup which kind of sensitive detector is
  sens.setType("calorimeter");

  // Create the mother Detector element to be returned at the end
  // this det element is linked with an assembly of both endcaps
  dd4hep::DetElement det_master(detName, detID);

  // create DetElements for each endcap, as daughters of det_master
  dd4hep::DetElement det_right (det_master);
  dd4hep::DetElement det_left  (det_master);

  // How to define one detector:
  // Define geometrical shape
  dd4hep::Box myEndcapShape(size,size,size);
  // Define volume (shape+material)
  dd4hep::Volume myEndcapVolume(detName +"_sensor", myEndcapShape, desc.material("Silicon"));
  myEndcapVolume.setVisAttributes(desc.visAttributes("sensor_vis"));
  myEndcapVolume.setSensitiveDetector(sens);
  myEndcapVolume.setLimitSet(desc, detElem.limitsStr());

  dd4hep::Transform3D right_position(dd4hep::RotationZ( 000*dd4hep::deg), dd4hep::Translation3D(0, 0,  zpos));
  dd4hep::Transform3D left_position (dd4hep::RotationZ( 180*dd4hep::deg), dd4hep::Translation3D(0, 0, -zpos));

  // top volume of endcaps is an assembly
  dd4hep::Assembly endcaps_assembly("Endcaps_assembly");

  // place the endcap on the right and left
  auto myRightEndcap_pv = endcaps_assembly.placeVolume( myEndcapVolume, right_position );
  auto myLeftEndcap_pv  = endcaps_assembly.placeVolume( myEndcapVolume, left_position  );

  // mark each placed volume (pv) with the proper phys vol ID
  myRightEndcap_pv.addPhysVolID("IsRightEndcap", 1);
  myLeftEndcap_pv.addPhysVolID ("IsRightEndcap", 0);

  // link each pv with its corresponding det element
  det_right.setPlacement(myRightEndcap_pv);
  det_left.setPlacement( myLeftEndcap_pv );

  // Place top mother assembly volume in the world
  auto wVol = desc.pickMotherVolume(det_master);
  auto endcaps_assembly_pv = wVol.placeVolume(endcaps_assembly);

  // Assign the system ID to our mother volume
  endcaps_assembly_pv.addPhysVolID("system", detID);

  // link Endcap assembly with top DetElement to be returned
  det_master.setPlacement(endcaps_assembly_pv);

  return det_master;
}

DECLARE_DETELEMENT(MYCUBE, createDetector)

