#ifndef WireCell2dToy_TOYMATCHING_H
#define WireCell2dToy_TOYMATCHING_H

#include "WireCellData/PR3DCluster.h"
#include "WireCell2dToy/uBooNE_light_reco.h"

namespace WireCell2dToy{
  // time_offset in us
  int convert_xyz_voxel_id(WireCell::Point& p);

  void tpc_light_match(int time_offset, int nrebin, std::map<WireCell::PR3DCluster*,std::vector<std::pair<WireCell::PR3DCluster*,double>>>& group_clusters, WireCell::OpflashSelection& flashes);
}

#endif