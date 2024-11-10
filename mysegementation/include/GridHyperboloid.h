#ifndef GRIDHYPERBOLOID_H
#define GRIDHYPERBOLOID_H

#include "DDSegmentation/Segmentation.h"

/* #include "DDSegmentation/SegmentationUtil.h" */
#include "TVector3.h"
#include <cmath>
#include <string>

/** GridTheta_k4geo Detector/detectorSegmentations/detectorSegmentations/GridTheta_k4geo.h GridTheta_k4geo.h
 *
 *  Segmentation in theta.
 *
 */

namespace dd4hep {
namespace DDSegmentation {
class GridHyperboloid : public Segmentation {
public:

  /// default constructor using an arbitrary type
  GridHyperboloid(const std::string& aCellEncoding);
  /// Default constructor used by derived classes passing an existing decoder
  GridHyperboloid(const BitFieldCoder* decoder);
  /// destructor
  virtual ~GridHyperboloid() = default;

  /// Initialize parameters, to be called inside the constructors
  void Initialize();

  /**  Determine position based on the cell ID.
   *   @warning This segmentation has no knowledge of z, assumed z=0
   *   @param[in] aCellId ID of a cell.
   *   return Position (z = 0).
   */
  virtual Vector3D position(const CellID& aCellID) const;

  /**  Determine the cell ID based on the 3D position.
   *   @param[in] aLocalPosition position, wrt cylindrical gas volume
   *   @param[in] aGlobalPosition not used
   *   @param[in] aVolumeId ID of a volume.
   *   return Cell ID.
   */
  virtual CellID cellID(const Vector3D& aLocalPosition, const Vector3D& aGlobalPosition,
                        const VolumeID& aVolumeID) const;
  /** Determine the layer based on the cell ID.
   *  @param[in] aCellId ID of a cell.
   *  return layer
   */
  int layer(const CellID& aCellID) const;

  /**  Set the field name used for layer
   *   @param[in] aFieldName Field name for theta.
   */
  inline void setFieldNameLayer(const std::string& fieldName) { m_layerID = fieldName; }
  inline const std::string fieldNameLayer() const { return m_layerID; }



  // /**  Determine the theta angle based on the cell ID.
  //  *   @param[in] aCellId ID of a cell.
  //  *   return Pseudorapidity.
  //  */
  // double theta(const CellID& aCellID) const;
  // /**  Get the grid size in theta angle.
  //  *   return Grid size in theta.
  //  */
  // inline double gridSizeTheta() const { return m_gridSizeTheta; }
  // /**  Get the coordinate offset in theta angle.
  //  *   return The offset in theta.
  //  */
  // inline double offsetTheta() const { return m_offsetTheta; }
  // /**  Get the field name used for theta angle
  //  *   return The field name for theta.
  //  */
  // inline const std::string& fieldNameTheta() const { return m_thetaID; }
  // /**  Set the grid size in theta angle.
  //  *   @param[in] aCellSize Cell size in theta.
  //  */
  // inline void setGridSizeTheta(double aCellSize) { m_gridSizeTheta = aCellSize; }
  // /**  Set the coordinate offset in theta angle.
  //  *   @param[in] aOffset Offset in theta.
  //  */
  // inline void setOffsetTheta(double offset) { m_offsetTheta = offset; }
  // /**  Set the field name used for theta angle.
  //  *   @param[in] aFieldName Field name for theta.
  //  */
  // inline void setFieldNameTheta(const std::string& fieldName) { m_thetaID = fieldName; }
  // /// calculates the Cartesian position from cylindrical coordinates (r, phi, theta)
  inline Vector3D positionFromRThetaPhi(double ar, double atheta, double aphi) const {
    return Vector3D(ar * std::cos(aphi), ar * std::sin(aphi), ar * std::cos(atheta)/std::sin(atheta));
  }
  // // // // /// calculates the theta angle from Cartesian coordinates
  // inline double thetaFromXYZ(const Vector3D& aposition) const {
  //   TVector3 vec(aposition.X, aposition.Y, aposition.Z);
  //   return vec.Theta();
  // }
  // /// from SegmentationUtil
  // /// to be removed once SegmentationUtil can be included w/o linker error
  // /// calculates the azimuthal angle phi from Cartesian coordinates
  // inline double phiFromXYZ(const Vector3D& aposition) const { return std::atan2(aposition.Y, aposition.X); }
  // /// from SegmentationUtil
  // /// to be removed once SegmentationUtil can be included w/o linker error
  // /// calculates the radius in the xy-plane from Cartesian coordinates
  // inline double radiusFromXYZ(const Vector3D& aposition) const {
  //   return std::sqrt(aposition.X * aposition.X + aposition.Y * aposition.Y);
  // }

protected:
  /// the field name used for layer
  std::string m_layerID;

  // /// determine the theta angle based on the current cell ID
  // double theta() const;
  // /// the grid size in theta
  // double m_gridSizeTheta;
  // /// the coordinate offset in theta
  // double m_offsetTheta;
  // /// the field name used for theta
  // std::string m_thetaID;
};
}
}
#endif /* DETSEGMENTATION_GRIDTHETA_H */
