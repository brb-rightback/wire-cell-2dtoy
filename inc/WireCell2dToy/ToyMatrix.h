#ifndef WIRECELL_TOYMATRIX_H
#define WIRECELL_TOYMATRIX_H

#include "TMatrixD.h"
#include "TVectorD.h"
#include "WireCellData/GeomCell.h"
#include "WireCellData/GeomWire.h"
#include "WireCellData/MergeGeomCell.h"
#include "WireCellData/MergeGeomWire.h"
#include "WireCell2dToy/MergeToyTiling.h"

namespace WireCell2dToy{
  class ToyMatrix {
  public:
    ToyMatrix(WireCell2dToy::ToyTiling& toytiling, WireCell2dToy::MergeToyTiling& mergetiling);
    virtual ~ToyMatrix();

    

    double Get_Chi2(){return chi2;};
    float Get_Solve_Flag(){return solve_flag;};

    const TMatrixD* Get_MA(){return MA;};
    const TMatrixD* Get_MB(){return MB;};
    const TMatrixD* Get_MAT(){return MAT;};
    const TMatrixD* Get_MBT(){return MBT;};
    const TMatrixD* Get_Vy(){return Vy;};
    const TMatrixD* Get_VBy(){return VBy;};
    const TMatrixD* Get_Vx(){return Vx;};
    const TMatrixD* Get_VBy_inv(){return VBy_inv;};
    const TMatrixD* Get_Vx_inv(){return Vx_inv;};
    const TMatrixD* Get_MC(){return MC;};
    const TMatrixD* Get_MC_inv(){return MC_inv;};
    
    double Get_Cell_Charge( const WireCell::GeomCell *cell, int flag = 1) ;
    

  protected:

    int Solve();

    TMatrixD *MA, *MB, *MAT, *MBT;
    TMatrixD *Vy, *VBy, *Vx, *VBy_inv, *Vx_inv;

    TMatrixD *MC, *MC_inv;

    TVectorD *Wy, *Cx, *dCx;
    double chi2;
    int solve_flag;

    void Buildup_index(WireCell2dToy::MergeToyTiling& mergetiling);

    WireCell::WireIndexMap mwimap, swimap;
    WireCell::CellIndexMap mcimap;

    int mcindex;
    int mwindex;
    int swindex;
  };
}

#endif
