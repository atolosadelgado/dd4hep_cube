#include "DD4hep/Factories.h"
#include "DD4hep/detail/SegmentationsInterna.h"

namespace {
template <typename T>
dd4hep::SegmentationObject* create_segmentation(const dd4hep::BitFieldCoder* decoder) {
  return new dd4hep::SegmentationWrapper<T>(decoder);
}
}

#include "GridTheta_k4geo.h"
DECLARE_SEGMENTATION(GridTheta, create_segmentation<dd4hep::DDSegmentation::GridTheta_k4geo>)


#include "GridHyperboloid.h"
DECLARE_SEGMENTATION(GridHyperboloid, create_segmentation<dd4hep::DDSegmentation::GridHyperboloid>)

