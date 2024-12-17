#include "DD4hep/DetFactoryHelper.h"
#include "DD4hep/OpticalSurfaces.h"
#include "DD4hep/Printout.h"
#include "DDRec/DetectorData.h"
#include <XML/Helper.h>

using namespace dd4hep;


inline double Circumradius(double Apothem, double dphi){return Apothem/cos(0.5*dphi/dd4hep::rad);}
// inline double Apothem(double Circumradius, int n){return Circumradius*cos(TMath::Pi()/n);}

Solid EightPointSolid_TwistedTube(double twist_angle,  double rmin,  double rmax, double dz, double dphi )
{

  // warning...
  // twisted tube and my implementation of twisted surfaces using generic trapezoid
  // asume radii at z=+/-dz
  // it seems ROOT hyperboloid requires radii at z=0, so conversion from rmin,rmax is needed

  // make generic trapezoid bigger, later intersected with hyperboloid of proper radii
  double trap_rin = 0; //0.9*rmin;
  double trap_rout= Circumradius(2*rmax, dphi);

  double poly_angle = dphi/2;
  double twist_angle_half = twist_angle/2.;
  // change sign, so the final shape has the same orientation as G4 twisted tube
  twist_angle_half*= -1;

  struct point2d
  {
    double x = {0.0};
    double y = {0.0};
  };

  struct face
  {
    point2d A;
    point2d B;
    point2d C;
    point2d D;
  };

  face fZneg;
  face fZpos;

  fZpos.A = { trap_rin*cos(  poly_angle + twist_angle_half ), trap_rin*sin(  poly_angle + twist_angle_half ) };
  fZpos.B = { trap_rin*cos( -poly_angle + twist_angle_half ), trap_rin*sin( -poly_angle + twist_angle_half ) };

  fZneg.A = { trap_rin*cos(  poly_angle - twist_angle_half ), trap_rin*sin(  poly_angle - twist_angle_half ) };
  fZneg.B = { trap_rin*cos( -poly_angle - twist_angle_half ), trap_rin*sin( -poly_angle - twist_angle_half ) };

  fZpos.C = { trap_rout*cos(  poly_angle + twist_angle_half ), trap_rout*sin(  poly_angle + twist_angle_half ) };
  fZpos.D = { trap_rout*cos( -poly_angle + twist_angle_half ), trap_rout*sin( -poly_angle + twist_angle_half ) };

  fZneg.C = { trap_rout*cos(  poly_angle - twist_angle_half ), trap_rout*sin(  poly_angle - twist_angle_half ) };
  fZneg.D = { trap_rout*cos( -poly_angle - twist_angle_half ), trap_rout*sin( -poly_angle - twist_angle_half ) };

  std::vector<double> vertices_array = {  fZpos.B.x, fZpos.B.y,
                                          fZpos.A.x, fZpos.A.y,
                                          fZpos.C.x, fZpos.C.y,
                                          fZpos.D.x, fZpos.D.y,
                                          fZneg.B.x, fZneg.B.y,
                                          fZneg.A.x, fZneg.A.y,
                                          fZneg.C.x, fZneg.C.y,
                                          fZneg.D.x, fZneg.D.y
                                        };

  // make the gen trap bigger, so it can protrude the hyperboloid and avoid visualization issues
  const double gtrap_dz = 1.1*dz;
  EightPointSolid gtrap_shape(dz, vertices_array.data() );

  //----------------- Hyperboloid -------------
      /// Calculate radius at z=L/2 given at z=0
    double rmin_z0 = rmin*cos(twist_angle/2/dd4hep::rad);
    double rmax_z0 = rmax*cos(twist_angle/2/dd4hep::rad);


  // ROOT hyperboloid require stereoangles stin and stout
  // tan(stereoangle) = R(z=0)   / (L/2) * tan( twist_angle/2)
  // Equation from Hoshina et al, 2003
  //-- stereo for rmin
  double stin  = atan( rmin_z0/dz*tan(twist_angle/2/dd4hep::rad)) * dd4hep::rad;
  //-- stereo for rmax
  double stout = atan( rmax_z0/dz*tan(twist_angle/2/dd4hep::rad)) * dd4hep::rad;

  Hyperboloid layer_s(rmin_z0, stin, rmax_z0, stout, dz);

  // create a twisted tube as intersection of the generic trapezoid and the hyperboloid
  Solid mytt = IntersectionSolid(layer_s,gtrap_shape);
  return mytt;
}

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




  double twist_angle = 90*dd4hep::degree;
  double rmin = 1*dd4hep::cm;
  double rmax = 5*dd4hep::cm;
  double dz = 20*dd4hep::cm;

  double dphi = 90*dd4hep::deg;
  double safe_factor=1-1e-2;
  // int nsides = 2;
  //   double dphi = TMath::TwoPi()/nsides*dd4hep::rad;
  TwistedTube myshape_tt( twist_angle,  rmin,  rmax, dz, dphi*safe_factor);
  Solid myshape = EightPointSolid_TwistedTube(twist_angle,  rmin,  rmax, dz, dphi*safe_factor);
  // Define volume (shape+material)
  Volume siVol(detName +"_sensor", myshape, desc.material("Silicon"));
  siVol.setVisAttributes(desc.visAttributes("vis1"));
  siVol.setSensitiveDetector(sens);

//   Tube swire_s{0,900*um,dz};
  Volume swire_v(detName +"_sw", myshape_tt, desc.material("Silicon"));

  auto rw = 0.5*(rmin+rmax);
  auto rw_zo = rw*cos(twist_angle/2/dd4hep::rad);
  auto stereoangle_w = atan( rw_zo/dz*tan(twist_angle/2/dd4hep::rad)) * dd4hep::rad;
  dd4hep::RotationX stereoTr( -stereoangle_w );
  dd4hep::Transform3D swireTr ( stereoTr * dd4hep::Translation3D(rw_zo,0.,0.) );
  siVol.placeVolume(swire_v);

//   Volume siVolbis(detName +"_sensorbis", myshape, desc.material("Silicon"));
//   siVolbis.setVisAttributes(desc.visAttributes("vis2"));

  // Place our mother volume in the world
  Volume wVol = desc.pickMotherVolume(det);

//   // Game 1
//   PlacedVolume siPV = wVol.placeVolume(siVol);
//   // Assign the system ID to our mother volume
//   siPV.addPhysVolID("system", detID);
//   // Associate the silicon Placed Volume to the detector element.
//   det.setPlacement(siPV);



//   TwistedTube myshape2( twist_angle,  rmin,  rmax, dz, dphi*safe_factor);
//   Volume siVol2(detName +"_sensor2", myshape2, desc.material("Silicon"));

//         Transform3D ttTr(RotationZ(0*dd4hep::deg), Translation3D(20*dd4hep::cm,0,0));
//   wVol.placeVolume(siVol2, ttTr);

// //   //game 2
// //   for(int i=0; i<12; ++i)
// //   {
// // //     if( i%2 == 0) continue;
// //       Transform3D ttTr(RotationZ(dphi*i), Translation3D(0*dd4hep::cm,0,0));
// //       if( i%2 == 0)
// //       {
// //         PlacedVolume siPV = wVol.placeVolume(siVol,ttTr);
// //         // Assign the system ID to our mother volume
// //         siPV.addPhysVolID("system", detID);
// //         // Associate the silicon Placed Volume to the detector element.
// //         det.setPlacement(siPV);
// //       }
// //       else
// //       {
// //         PlacedVolume siPV = wVol.placeVolume(siVolbis,ttTr);
// //         // Assign the system ID to our mother volume
// //         siPV.addPhysVolID("system", detID);
// //         // Associate the silicon Placed Volume to the detector element.
// //         det.setPlacement(siPV);
// //       }
// //   }
  //game 3
  for(int i=0; i<1; ++i)
  {
      Transform3D ttTr(RotationZ(dphi*i), Translation3D(0*dd4hep::cm,0,0));
        PlacedVolume siPV = wVol.placeVolume(siVol,ttTr);
        // Assign the system ID to our mother volume
        siPV.addPhysVolID("system", detID);
        // Associate the silicon Placed Volume to the detector element.
        det.setPlacement(siPV);
  }
  return det;
}

DECLARE_DETELEMENT(MyTT, createDetector)
