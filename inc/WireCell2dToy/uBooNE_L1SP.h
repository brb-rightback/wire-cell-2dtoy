#ifndef WIRECELL_UBOONE_L1SP_H
#define WIRECELL_UBOONE_L1SP_H

#include "WireCellData/GeomWire.h"
#include "TH2F.h"
#include "TGraph.h"


namespace WireCell2dToy{
  class uBooNE_L1SP {
  public:
    uBooNE_L1SP(TH2F *hv_raw, TH2F *hv_decon, int nrebin);
    ~uBooNE_L1SP();

    void AddWires(int time_slice, WireCell::GeomWireSelection& wires);
    void Form_rois(int pad = 2);
    
    void L1_fit(int wire_index, int start_tick, int end_tick);
    
  protected:
    TH2F *hv_raw;
    TH2F *hv_decon;
    TGraph *gv, *gw;
    
    int nrebin;
    // wire index --> time
    std::map<int,std::vector<int>> init_map;
  };
}

#endif