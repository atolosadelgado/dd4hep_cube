#include "GridHyperboloid.h"

namespace dd4hep {
namespace DDSegmentation {

void GridHyperboloid::Initialize()
{
    // define type and description
  _type = "GridHyperboloid";
  _description = "Theta segmentation in the global coordinates";

  // register all necessary parameters
  // registerParameter("grid_size_theta", "Cell size in Theta", m_gridSizeTheta, 1., SegmentationParameter::LengthUnit);
  // registerParameter("offset_theta", "Angular offset in theta", m_offsetTheta, 0., SegmentationParameter::AngleUnit, true);
  // registerIdentifier("identifier_theta", "Cell ID identifier for theta", m_thetaID, "theta");
  registerIdentifier("identifier_layer", "Cell ID identifier for layer", m_layerID, "layer");
}

/// constructor using an encoding string
GridHyperboloid::GridHyperboloid(const std::string& cellEncoding) : Segmentation(cellEncoding) {
  this->Initialize();
}

GridHyperboloid::GridHyperboloid(const BitFieldCoder* decoder) : Segmentation(decoder) {
  this->Initialize();
}

/// determine the local based on the cell ID
Vector3D GridHyperboloid::position(const CellID& cID) const {
  // return positionFromRThetaPhi(1.0, theta(cID), 0.);
  return positionFromRThetaPhi(1.0, 0.5, 0.);
}

/// determine the cell ID based on the position
CellID GridHyperboloid::cellID(const Vector3D& /* localPosition */, const Vector3D& globalPosition, const VolumeID& vID) const {
  CellID cID = vID;
  // double lTheta = thetaFromXYZ(globalPosition);
  _decoder->set(cID, m_layerID, 1/*positionToBin(lTheta, m_gridSizeTheta, m_offsetTheta)*/);
  return cID;
}

/// determine the pseudorapidity based on the current cell ID
//double GridHyperboloid::theta() const {
//  CellID thetaValue = (*_decoder)[m_thetaID].value();
//  return binToPosition(thetaValue, m_gridSizeTheta, m_offsetTheta);
//}

/// determine the polar angle theta based on the cell ID
// double GridHyperboloid::theta(const CellID& cID) const {
//   CellID thetaValue = _decoder->get(cID, m_thetaID);
//   return binToPosition(thetaValue, m_gridSizeTheta, m_offsetTheta);
// }

} // namespace DDSegmentation
} // namespace dd4hep

