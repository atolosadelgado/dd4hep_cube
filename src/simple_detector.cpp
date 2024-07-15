#include "DD4hep/DetFactoryHelper.h"
#include "DD4hep/OpticalSurfaces.h"
#include "DD4hep/Printout.h"
#include "DDRec/DetectorData.h"
#include <XML/Helper.h>

using namespace dd4hep;


inline double Circumradius(double Apothem, int n){return Apothem/cos(TMath::Pi()/n);     }
inline double Apothem(double Circumradius, int n){return Circumradius*cos(TMath::Pi()/n);}

TessellatedSolid TessellatedTwistedTube(double twist_angle,  double rmin,  double rmax, double dz, int ncells )
{
  // number of segments per cell
  int tess_k = 3;
  // number of sides of polygon = number of cells * number of segments per cell
  int tess_n = ncells * tess_k;
  //--- calculate points along inner radius
  double tess_rin = Circumradius(rmin,ncells);
  double tess_rout=rmax;

  double poly_angle = 1.5*TMath::TwoPi()/tess_n * dd4hep::rad;
  double twist_angle_half = twist_angle/2.;

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
    point2d E;
    point2d F;
  };

  face fZneg;
  face fZpos;

  fZpos.A = { tess_rin*cos(  poly_angle + twist_angle_half ), tess_rin*sin(  poly_angle + twist_angle_half ) };
  fZpos.B = { tess_rin*cos(               twist_angle_half ), tess_rin*sin(               twist_angle_half ) };
  fZpos.C = { tess_rin*cos( -poly_angle + twist_angle_half ), tess_rin*sin( -poly_angle + twist_angle_half ) };

  fZneg.A = { tess_rin*cos(  poly_angle - twist_angle_half ), tess_rin*sin(  poly_angle - twist_angle_half ) };
  fZneg.B = { tess_rin*cos(             - twist_angle_half ), tess_rin*sin(             - twist_angle_half ) };
  fZneg.C = { tess_rin*cos( -poly_angle - twist_angle_half ), tess_rin*sin( -poly_angle - twist_angle_half ) };

  fZpos.D = { tess_rout*cos(  poly_angle + twist_angle_half ), tess_rout*sin(  poly_angle + twist_angle_half ) };
  fZpos.E = { tess_rout*cos(               twist_angle_half ), tess_rout*sin(               twist_angle_half ) };
  fZpos.F = { tess_rout*cos( -poly_angle + twist_angle_half ), tess_rout*sin( -poly_angle + twist_angle_half ) };

  fZneg.D = { tess_rout*cos(  poly_angle - twist_angle_half ), tess_rout*sin(  poly_angle - twist_angle_half ) };
  fZneg.E = { tess_rout*cos(             - twist_angle_half ), tess_rout*sin(             - twist_angle_half ) };
  fZneg.F = { tess_rout*cos( -poly_angle - twist_angle_half ), tess_rout*sin( -poly_angle - twist_angle_half ) };

  using Vertex = TessellatedSolid::Vertex;
  std::vector<Vertex> vertices;
  // preallocate space for 2 faces * 6 vertices/face
  vertices.reserve ( 14 );

  vertices.emplace_back ( Vertex ( fZpos.A.x, fZpos.A.y, dz ) );
  vertices.emplace_back ( Vertex ( fZpos.B.x, fZpos.B.y, dz ) );
  vertices.emplace_back ( Vertex ( fZpos.C.x, fZpos.C.y, dz ) );
  vertices.emplace_back ( Vertex ( fZpos.D.x, fZpos.D.y, dz ) );
  vertices.emplace_back ( Vertex ( fZpos.E.x, fZpos.E.y, dz ) );
  vertices.emplace_back ( Vertex ( fZpos.F.x, fZpos.F.y, dz ) );

  vertices.emplace_back ( Vertex ( fZneg.A.x, fZneg.A.y, -dz ) );
  vertices.emplace_back ( Vertex ( fZneg.B.x, fZneg.B.y, -dz ) );
  vertices.emplace_back ( Vertex ( fZneg.C.x, fZneg.C.y, -dz ) );
  vertices.emplace_back ( Vertex ( fZneg.D.x, fZneg.D.y, -dz ) );
  vertices.emplace_back ( Vertex ( fZneg.E.x, fZneg.E.y, -dz ) );
  vertices.emplace_back ( Vertex ( fZneg.F.x, fZneg.F.y, -dz ) );

  TessellatedSolid shape ( "tesstt", vertices );

  // fZpos
  shape->AddFacet(0,1,4,3);
  shape->AddFacet(4,1,2,5);
  // fZneg
  shape->AddFacet(10,7,6,9);
  shape->AddFacet(10,11,8,7);

  // external cyl surface
  shape->AddFacet(3,4,10);
  shape->AddFacet(3,10,9);
  shape->AddFacet(4,5,11);
  shape->AddFacet(4,11,10);

  // internal cyl surface
  shape->AddFacet(7,1,0);
  shape->AddFacet(6,7,0);
  shape->AddFacet(8,2,1);
  shape->AddFacet(7,8,1);
  //
  // // phi surfaces
  shape->AddFacet(0,3,6);
  shape->AddFacet(3,9,6);
  shape->AddFacet(8,11,5);
  shape->AddFacet(8,5,2);

  shape->CloseShape ( true, false, true );
  return shape;
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




  double twist_angle = 20*dd4hep::degree;
  double rmin = 3*dd4hep::cm;
  double rmax = 5*dd4hep::cm;
  double dz = 5*dd4hep::cm;
  int nsides = 12;
  // double dphi = 30*dd4hep::deg;
  double dphi = TMath::TwoPi()/nsides*dd4hep::rad;
  int nsegments = 1;
  // TwistedTube myshape( twist_angle,  rmin,  rmax, dz, dphi);
  TessellatedSolid myshape = TessellatedTwistedTube(twist_angle,  rmin,  rmax, dz, nsides);
  // Define volume (shape+material)
  // Box myshape( 1*dd4hep::cm,1*dd4hep::cm,1*dd4hep::cm);
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


  TwistedTube myshape2( twist_angle,  rmin,  rmax, dz, dphi);
  Volume siVol2(detName +"_sensor2", myshape2, desc.material("Silicon"));

  wVol.placeVolume(siVol2, Position(5*dd4hep::cm,0,0));



  return det;
}

DECLARE_DETELEMENT(MYCUBE, createDetector)
