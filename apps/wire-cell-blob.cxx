#include "WireCellSst/GeomDataSource.h"
//#include "WireCellSst/ToyuBooNEFrameDataSource.h"
#include "WireCellSst/ToyuBooNESliceDataSource.h"
#include "WireCell2dToy/ToyEventDisplay.h"
#include "WireCell2dToy/ToyTiling.h"
#include "WireCell2dToy/MergeToyTiling.h"
#include "WireCell2dToy/TruthToyTiling.h"
#include "WireCell2dToy/BlobToyTiling.h"


#include "WireCellData/MergeGeomCell.h"
#include "WireCellData/GeomCluster.h"
//#include "WireCellNav/SliceDataSource.h"
#include "WireCell2dToy/ToyMatrix.h"
#include "WireCell2dToy/ToyMatrixIterate.h"
#include "WireCell2dToy/ToyMatrixMarkov.h"
#include "WireCell2dToy/ToyMetric.h"

#include "WireCellNav/FrameDataSource.h"
#include "WireCellNav/SimDataSource.h"
#include "WireCellNav/SliceDataSource.h"
#include "WireCellSst/Util.h"
#include "WireCellData/SimTruth.h"
#include "WireCell2dToy/ToyDepositor.h"
#include "WireCellNav/GenerativeFDS.h"

#include "TApplication.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TFile.h"
#include "TGraph2D.h"
#include "TColor.h"
#include <iostream>

using namespace WireCell;
using namespace std;

double rms(double a1, double a2, double a3){
  double ave = (a1 + a2 + a3)/3.;
  double rms1 = sqrt(pow(a1-ave,2) + pow(a2-ave,2));
  double rms2 = sqrt(pow(a2-ave,2) + pow(a3-ave,2));
  double rms3 = sqrt(pow(a1-ave,2) + pow(a3-ave,2));
  
  double rms;
  if (rms1 < rms2){
    rms = rms1;
  }else{
    rms = rms2;
  } 
  if (rms > rms3){
    rms = rms3;
  }
  
  //double rms  = sqrt((pow(a1-ave,2) + pow(a2-ave,2) + pow(a3-ave,2))/2.);
  return rms;
}


int main(int argc, char* argv[])
{
  if (argc < 3) {
    cerr << "usage: wire-cell-uboone /path/to/ChannelWireGeometry.txt /path/to/celltree.root" << endl;
    return 1;
  }
  
  
  WireCellSst::GeomDataSource gds(argv[1]);
  std::vector<double> ex = gds.extent();
  cerr << "Extent: "
       << " x:" << ex[0]/units::mm << " mm"
       << " y:" << ex[1]/units::m << " m"
       << " z:" << ex[2]/units::m << " m"
       << endl;
  
  
  const char* root_file = argv[2];
  const char* tpath = "/Event/Sim";
  
  WireCell::FrameDataSource* fds = 0;
  fds = WireCellSst::make_fds(root_file);
  if (!fds) {
    cerr << "ERROR: failed to get FDS from " << root_file << endl;
    return 1;
  }
  

  
  WireCell::ToyDepositor toydep(fds);
  const PointValueVector pvv = toydep.depositions(1);
  

  WireCell::GenerativeFDS gfds(toydep,gds,2400,5);
  gfds.jump(1);

  WireCellSst::ToyuBooNESliceDataSource sds(gfds,1500); //set threshold at 2000 electrons

  

  const int N = 100000;
  Double_t x[N],y[N],z[N];
  Double_t xt[N],yt[N],zt[N];
  int ncount = 0;
  int ncount_t = 0;
  

  WireCell2dToy::ToyTiling **toytiling = new WireCell2dToy::ToyTiling*[2400];
  WireCell2dToy::MergeToyTiling **mergetiling = new WireCell2dToy::MergeToyTiling*[2400];
  WireCell2dToy::TruthToyTiling **truthtiling = new WireCell2dToy::TruthToyTiling*[2400];
  WireCell2dToy::BlobToyTiling **blobtiling = new WireCell2dToy::BlobToyTiling*[2400];
  
  WireCell2dToy::ToyMatrix **toymatrix = new WireCell2dToy::ToyMatrix*[2400];
  WireCell2dToy::ToyMatrixMarkov **toymatrix_markov = new WireCell2dToy::ToyMatrixMarkov*[2400];
  WireCell2dToy::ToyMatrix **blobmatrix = new WireCell2dToy::ToyMatrix*[2400];
  
  WireCell2dToy::ToyMatrixMarkov **blobmatrix_markov = new WireCell2dToy::ToyMatrixMarkov*[2400];
  WireCell2dToy::ToyMetric toymetric, blobmetric;
 
  //add in cluster
  GeomClusterSet cluster_set, cluster_delset;
  
  GeomCellSelection total_cells;
  GeomCellSelection total_recon_cells;
  GeomCellSelection total_corner_cells;
  CellChargeMap total_ccmap;
  
  CellChargeMap total_scrms;
  CellChargeMap total_scmap;


  Double_t charge_min = 10000;
  Double_t charge_max = 0;
    
  int ncount_mcell = 0;
  
  //simple cosmic
  int start_num =185;
  int end_num = 185;


  //nue cc 
  // int start_num =356;
  // int end_num = 356;
    
  //delta 
  // int start_num =680;
  // int end_num = 680;

  //complicated blob
  // int start_num = 452;
  // int end_num = 456;
  
  WireCell::Slice slice;
  for (int i=start_num;i!=end_num+1;i++){
    sds.jump(i);
    slice = sds.get();
          
    toytiling[i] = new WireCell2dToy::ToyTiling(slice,gds);
    mergetiling[i] = new WireCell2dToy::MergeToyTiling(*toytiling[i],i);
    truthtiling[i] = new WireCell2dToy::TruthToyTiling(*toytiling[i],pvv,i,gds);
    GeomCellSelection allcell = toytiling[i]->get_allcell();
    GeomCellSelection allmcell = mergetiling[i]->get_allcell();
    GeomWireSelection allmwire = mergetiling[i]->get_allwire();
    cout << i << " " << allcell.size() << " " << allmcell.size() << " " << allmwire.size()  << endl;
    
    for (int j=0;j!=allcell.size();j++){
      total_cells.push_back(allcell.at(j));
    }


    toymatrix[i] = new WireCell2dToy::ToyMatrix(*toytiling[i],*mergetiling[i]);
    if (toymatrix[i]->Get_Solve_Flag()==0)
      toymatrix_markov[i] = new WireCell2dToy::ToyMatrixMarkov(toymatrix[i],&allmcell);    
    cout << "chi2: " << toymatrix[i]->Get_Chi2() << endl;
    cout << "NDF: " << toymatrix[i]->Get_ndf() << endl;

    

    
    // // for now put this part here
    // blobtiling[i] = new WireCell2dToy::BlobToyTiling(*toytiling[i],*mergetiling[i],*toymatrix[i],i,2);
    // blobmatrix[i] = new WireCell2dToy::ToyMatrix(*toytiling[i],*blobtiling[i]);

    // GeomCellSelection ballmcell = blobtiling[i]->get_allcell();
    // GeomWireSelection ballmwire = blobtiling[i]->get_allwire();
    // cout << i << " " << ballmcell.size() << " " << ballmwire.size()  << endl;
    
    // if (blobmatrix[i]->Get_Solve_Flag()==0)
    //   blobmatrix_markov[i] = new WireCell2dToy::ToyMatrixMarkov(blobmatrix[i],&ballmcell);
    
    // cout << "chi2: " << blobmatrix[i]->Get_Chi2() << endl;
    // cout << "NDF: " << blobmatrix[i]->Get_ndf() << endl;
    
    CellChargeMap ccmap = truthtiling[i]->ccmap();
    total_ccmap.insert(ccmap.begin(),ccmap.end());

    if (toymatrix[i]->Get_Solve_Flag()!=0)
	toymetric.Add(allmcell,*toymatrix[i],ccmap);
    toymetric.AddSolve(toymatrix[i]->Get_Solve_Flag());
    
    // if (blobmatrix[i]->Get_Solve_Flag()!=0)
    //   blobmetric.Add(ballmcell,*blobmatrix[i],ccmap);
    // blobmetric.AddSolve(blobmatrix[i]->Get_Solve_Flag());
    
    
    
    for (auto it = ccmap.begin();it!=ccmap.end(); it++){
      Point p = it->first->center();
      xt[ncount_t] = i*0.32;
      yt[ncount_t] = p.y/units::cm;
      zt[ncount_t] = p.z/units::cm;
      ncount_t ++;
      
      double charge = it->second;
      if (charge > charge_max) charge_max = charge;
      if (charge < charge_min) charge_min = charge;
      // cout << it->second << endl;
    }
    
    
    //loop through merged cell and compare with truth cells
    for (int j=0;j!=allmcell.size();j++){
      MergeGeomCell *mcell = (MergeGeomCell*)allmcell[j];
      mcell->CheckContainTruthCell(ccmap);
      // 	cout << mergetiling.wires(*allmcell[j]).size() << endl;
    }
    
    
   
    
  }

  toymetric.Print();
  std::cout << std::endl;
  blobmetric.Print();
  

  //do display
  
  TApplication theApp("theApp",&argc,argv);
  theApp.SetReturnFromRun(true);
  
  TCanvas c1("ToyMC","ToyMC",800,600);
  //TCanvas c2("ToyMC","ToyMC",800,600);
  
  WireCell2dToy::ToyEventDisplay display(c1, gds);
  //WireCell2dToy::ToyEventDisplay display1(c2, gds);
  display.charge_min = charge_min;
  display.charge_max = charge_max;
  // display.charge_min = 0.;
  // display.charge_max = 1.;
  
  // display1.charge_min = charge_min;
  // display1.charge_max = charge_max;
  
  gStyle->SetOptStat(0);
  
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  Int_t MyPalette[NCont];
  Double_t stops[NRGBs] = {0.0, 0.34, 0.61, 0.84, 1.0};
  Double_t red[NRGBs] = {0.0, 0.0, 0.87 ,1.0, 0.51};
  Double_t green[NRGBs] = {0.0, 0.81, 1.0, 0.2 ,0.0};
  Double_t blue[NRGBs] = {0.51, 1.0, 0.12, 0.0, 0.0};
  Int_t FI = TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
  for (int kk=0;kk!=NCont;kk++) MyPalette[kk] = FI+kk;
  gStyle->SetPalette(NCont,MyPalette);
  
  display.init(0,10.3698,-2.33/2.,2.33/2.);
  //display.init(8.61,8.66,0.0,0.06);
  // display1.init(1.4,1.65,0.7,1.0);
  
  
  display.draw_mc(1,WireCell::PointValueVector(),"colz");
  
  display.draw_slice(slice,"");
  display.draw_cells(total_cells,"*same");
  //display.draw_mergecells(mergetiling[i]->get_allcell(),"*same",1); //0 is normal, 1 is only draw the ones containt the truth cell
  
  display.draw_cells(total_recon_cells,"*same",1);
  //display.draw_truthcells_charge(total_ccmap,"*same",FI);
  //display.draw_truthcells_charge(total_scmap,"*same",FI);
  //display.draw_truthcells_charge(total_scrms,"*same",FI);
  display.draw_truthcells(total_ccmap,"*same");
  display.draw_cells(total_corner_cells,"*same",2);
  //display.draw_reconcells(mergetiling[i]->get_allcell(),toymatrix[i],"*same",1);
  //display.draw_reconcells(blobtiling[i]->get_allcell(),blobmatrix[i],"*same",2);
  
  // display1.draw_mc(1,WireCell::PointValueVector(),"colz");
  // display1.draw_truthcells_charge(total_ccmap,"*same",FI);
  

  theApp.Run();
  
  
  
  TFile *file = new TFile("shower3D.root","RECREATE");
  TGraph2D *g = new TGraph2D();
  TGraph2D *gt = new TGraph2D();
  TGraph2D *g_rec = new TGraph2D();
  Double_t x_save, y_save, z_save;
  Double_t charge_save;
  Double_t chi2_save;
  Double_t ndf_save;
  Double_t ncell_save;
  ncount = 0;
  int ncount1 = 0;
  ncount_t = 0;
  //save results 
  for (int i=start_num;i!=end_num+1;i++){
    //truth
    CellChargeMap ccmap = truthtiling[i]->ccmap();
    for (auto it = ccmap.begin();it!=ccmap.end(); it++){
      Point p = it->first->center();
      x_save = i*0.32;
      y_save = p.y/units::cm;
      z_save = p.z/units::cm;
      charge_save = it->second;
      
      gt->SetPoint(ncount_t,x_save,y_save,z_save);
      //t_true->Fill();
            
      ncount_t ++;
    }
    
    //recon 1
    GeomCellSelection allcell = toytiling[i]->get_allcell();
    for (int j=0;j!=allcell.size();j++){
      
      Point p = allcell[j]->center();
      x_save = i*0.32;
      y_save = p.y/units::cm;
      z_save = p.z/units::cm;
      

      g->SetPoint(ncount,x_save,y_save,z_save);
      //t_rec->Fill();

      ncount ++;
    }

    //recon 2 with charge
    GeomCellSelection allmcell = mergetiling[i]->get_allcell();
    for (int j=0;j!=allmcell.size();j++){
      MergeGeomCell *mcell = (MergeGeomCell*)allmcell[j];
      double charge = toymatrix[i]->Get_Cell_Charge(mcell,1);
      if (charge>2000){
	//truth
	for (int k=0;k!=mcell->get_allcell().size();k++){
	  Point p = mcell->get_allcell().at(k)->center();
	  x_save = i*0.32;
	  y_save = p.y/units::cm;
	  z_save = p.z/units::cm;
	  charge_save = charge/mcell->get_allcell().size();
	  ncell_save = mcell->get_allcell().size();
	  chi2_save = toymatrix[i]->Get_Chi2();
	  ndf_save = toymatrix[i]->Get_ndf();

	  g_rec->SetPoint(ncount1,x_save,y_save,z_save);
	  //  t_rec_charge->Fill();
	  
	  ncount1 ++;
	}
      }
    }

    //save all results
    // file->Write(Form("toytiling_%d",i),toytiling[i]);
    // file->Write(Form("mergetiling_%d",i),mergetiling[i]);
    // file->Write(Form("truthtiling_%d",i),truthtiling[i]);
    // file->Write(Form("toymatrix_%d",i),toymatrix[i]);

  }

  g->Write("shower3D");
  gt->Write("shower3D_truth");
  g_rec->Write("shower3D_charge");
  file->Write();
  file->Close();
  
  
  return 0;
  
} // main()