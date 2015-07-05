#ifndef WIRECELL2dToy_TOYSIGNALWIEN_H
#define WIRECELL2dToy_TOYSIGNALWIEN_H

#include "WireCellNav/FrameDataSource.h"
#include "WireCellNav/GeomDataSource.h"
#include "TH1F.h"
#include "TGraph.h"

namespace WireCell2dToy {
  class ToySignalWienFDS : public WireCell::FrameDataSource
  {
  public:
    ToySignalWienFDS(WireCell::FrameDataSource& fds, const WireCell::GeomDataSource& gds, int bins_per_frame1 = 9600, int nframes_total = -1);
    ~ToySignalWienFDS();

    virtual int size() const;
    virtual int jump(int frame_number);
    //fixed it ...
    

    void Save();
    
  private:
    WireCell::FrameDataSource& fds;
    const WireCell::GeomDataSource& gds;
    int  max_frames;
    int nbin;

    int nwire_u, nwire_v, nwire_w;

    TH1F **hu;
    TH1F **hv;
    TH1F **hw;

    TGraph *gu, *gv, *gw;

    TH1F *hur, *hvr, *hwr;
    
    TH1 *hmr_u, *hpr_u;
    TH1 *hmr_v, *hpr_v;
    TH1 *hmr_w, *hpr_w;
    
  };
}

#endif