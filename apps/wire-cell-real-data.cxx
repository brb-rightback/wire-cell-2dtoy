#include "WireCellSst/GeomDataSource.h"
#include "WireCellSst/DatauBooNEFrameDataSource.h"
#include "WireCellSst/ToyuBooNESliceDataSource.h"
#include "WireCell2dToy/ToyEventDisplay.h"
#include "WireCell2dToy/ToyTiling.h"
#include "WireCell2dToy/MergeToyTiling.h"
#include "WireCell2dToy/TruthToyTiling.h"
#include "WireCell2dToy/SimpleBlobToyTiling.h"

#include "WireCell2dToy/ToyMatrix.h"
#include "WireCell2dToy/ToyMatrixExclusive.h"
#include "WireCell2dToy/ToyMatrixKalman.h"
#include "WireCell2dToy/ToyMatrixIterate.h"
#include "WireCell2dToy/ToyMatrixMarkov.h"
#include "WireCell2dToy/ToyMetric.h"
#include "WireCell2dToy/BlobMetric.h"

#include "WireCellData/MergeGeomCell.h"
#include "WireCellData/MergeGeomWire.h"

#include "WireCellData/GeomCluster.h"
//#include "WireCellNav/SliceDataSource.h"


#include "WireCellNav/FrameDataSource.h"
#include "WireCellNav/SimDataSource.h"
#include "WireCellNav/SliceDataSource.h"
#include "WireCellSst/Util.h"
#include "WireCellData/SimTruth.h"
#include "WireCell2dToy/ToyDepositor.h"
#include "WireCellNav/GenerativeFDS.h"
#include "WireCell2dToy/ToySignalSimu.h"
#include "WireCell2dToy/ToySignalSimuTrue.h"
#include "WireCell2dToy/DataSignalGaus.h"
#include "WireCell2dToy/DataSignalWien.h"

#include "TApplication.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TFile.h"
#include "TGraph2D.h"
#include "TColor.h"
#include "TVectorD.h"
#include "TMatrixD.h"
#include <iostream>

using namespace WireCell;
using namespace std;




int main(int argc, char* argv[])
{
  if (argc < 4) {
    cerr << "usage: wire-cell-uboone /path/to/ChannelWireGeometry.txt /path/to/celltree.root eve_num -t[0,1]" << endl;
    return 1;
  }

  int two_plane = 0;

  for(Int_t i = 1; i != argc; i++){
     switch(argv[i][1]){
     case 't':
       two_plane = atoi(&argv[i][2]); 
       break;
     }
  }
  
  if (two_plane)
    cout << "Enable Two Plane Reconstruction " << endl; 

  
  WireCellSst::GeomDataSource gds(argv[1]);
  std::vector<double> ex = gds.extent();
  cerr << "Extent: "
       << " x:" << ex[0]/units::mm << " mm"
       << " y:" << ex[1]/units::m << " m"
       << " z:" << ex[2]/units::m << " m"
       << endl;

  cout << "Pitch: " << gds.pitch(WirePlaneType_t(0)) 
       << " " << gds.pitch(WirePlaneType_t(1)) 
       << " " << gds.pitch(WirePlaneType_t(2))
       << endl;
  cout << "Angle: " << gds.angle(WirePlaneType_t(0)) 
       << " " << gds.angle(WirePlaneType_t(1)) 
       << " " << gds.angle(WirePlaneType_t(2))
       << endl;
  
  
  const char* root_file = argv[2];
  const char* tpath = "/Event/Sim";
  
  WireCell::FrameDataSource* fds = 0;
  fds = WireCellSst::make_fds(root_file);
  if (!fds) {
    cerr << "ERROR: failed to get FDS from " << root_file << endl;
    return 1;
  }
  
  float unit_dis = 1.01483;  // 58KV @ 226.5 V/cm

  int max_events = 100;
  int eve_num  = atoi(argv[3]);

  
  TFile tfile(root_file,"read");
  TTree* sst = dynamic_cast<TTree*>(tfile.Get(tpath));

  int run_no, subrun_no, event_no;
  sst->SetBranchAddress("eventNo",&event_no);
  sst->SetBranchAddress("runNo",&run_no);
  sst->SetBranchAddress("subRunNo",&subrun_no);
  sst->GetEntry(eve_num);

  cout << run_no << " " << subrun_no << " " << eve_num << endl;


  WireCellSst::DatauBooNEFrameDataSource data_fds(*sst,gds,9592);
  data_fds.jump(eve_num);
  //data_fds.Save();

  WireMap& uplane_map = data_fds.get_u_map();
  WireMap& vplane_map = data_fds.get_v_map();
  WireMap& wplane_map = data_fds.get_w_map();

  // std::cout << uplane_map.size() << " " << vplane_map.size() << " " << wplane_map.size() << std::endl;
  

  int recon_threshold = 2000;
  

  cout << "Deconvolution with Gaussian filter" << endl;
  WireCell2dToy::DataSignalGausFDS gaus_fds(data_fds,gds,9592/4,max_events,1.834,1.555+1.834,-0.5); // gaussian smearing for charge estimation
  gaus_fds.jump(eve_num);
  // //gaus_fds.Save();

  cout << "Deconvolution with Wiener filter" << endl; 
  WireCell2dToy::DataSignalWienFDS wien_fds(data_fds,gds,9592/4,max_events,1.834,1.555+1.834,-0.5); // weiner smearing for hit identification
  wien_fds.jump(eve_num);
  //wien_fds.Save();

  std::vector<float>& uplane_rms = wien_fds.get_uplane_rms();
  std::vector<float>& vplane_rms = wien_fds.get_vplane_rms();
  std::vector<float>& wplane_rms = wien_fds.get_wplane_rms();

  // hack for now ...  remove the very busy wires ... 
  for (int i=0;i!=uplane_rms.size();i++){
    //cout << "U " << i << " " << uplane_rms.at(i) << endl;
    if (uplane_rms.at(i) > 1500) {
      uplane_rms.at(i) *=2;
      uplane_map.erase(i);
    }
  }
  for (int i=0;i!=vplane_rms.size();i++){
    //cout << "V " << i << " " << vplane_rms.at(i) << endl;
    if (vplane_rms.at(i) > 2000 && vplane_rms.at(i)<3000){
      vplane_rms.at(i) *=2;
      vplane_map.erase(i);
    }else if (vplane_rms.at(i)>=3000){
      vplane_rms.at(i) *=10;
      vplane_map.erase(i);
    }
  }
  for (int i=0;i!=wplane_rms.size();i++){
    //cout << "W " << i << " " << wplane_rms.at(i) << endl;
    if (wplane_rms.at(i) > 1000) {
      wplane_rms.at(i) *=2;
      wplane_map.erase(i);
    }
  }
  

  GeomWireSelection wires_u = gds.wires_in_plane(WirePlaneType_t(0));
  GeomWireSelection wires_v = gds.wires_in_plane(WirePlaneType_t(1));
  GeomWireSelection wires_w = gds.wires_in_plane(WirePlaneType_t(2));

  int nwire_u = wires_u.size();
  int nwire_v = wires_v.size();
  int nwire_w = wires_w.size();
  
  float threshold_u = 5.87819e+02 * 4.0;
  float threshold_v = 8.36644e+02 * 4.0;
  float threshold_w = 5.67974e+02 * 4.0;



  float threshold_ug = 755.96;
  float threshold_vg = 822.81;
  float threshold_wg = 510.84;
  
  int time_offset = -46;
 
  

 

  // WireCellSst::ToyuBooNESliceDataSource sds(wien_fds,gaus_fds,threshold_u, 
  // 					    threshold_v, threshold_w, 
  // 					    threshold_ug, 
  // 					    threshold_vg, threshold_wg, 
  // 					    nwire_u, 
  // 					    nwire_v, nwire_w); 

  WireCellSst::ToyuBooNESliceDataSource sds(wien_fds,gaus_fds,threshold_u, 
  					    threshold_v, threshold_w, 
  					    threshold_ug, 
  					    threshold_vg, threshold_wg, 
  					    nwire_u, 
  					    nwire_v, nwire_w,
  					    &uplane_rms, &vplane_rms, &wplane_rms); 
    
  
  
  int ncount = 0;
  int ncount1 = 0;
  int ncount2 = 0;

  int ncount_t = 0;
  

  WireCell2dToy::ToyTiling **toytiling = new WireCell2dToy::ToyTiling*[2400];
  WireCell2dToy::MergeToyTiling **mergetiling = new WireCell2dToy::MergeToyTiling*[2400];
  WireCell2dToy::ToyMatrix **toymatrix = new WireCell2dToy::ToyMatrix*[2400];
    
  //add in cluster
  GeomClusterSet cluster_set, cluster_delset;
  
  int ncount_mcell = 0;

  //  delete fds;

  int start_num = 0 ;
  int end_num = sds.size()-1;


  for (int i=start_num;i!=end_num+1;i++){
 
    sds.jump(i);
    WireCell::Slice slice = sds.get();

    
    toytiling[i] = new WireCell2dToy::ToyTiling(slice,gds,0.1,0.1,0.1,threshold_ug,threshold_vg, threshold_wg);
    if (two_plane)
      toytiling[i]->twoplane_tiling(gds,uplane_rms,vplane_rms,wplane_rms, uplane_map, vplane_map, wplane_map);


    GeomCellSelection allcell = toytiling[i]->get_allcell();
    GeomWireSelection allwire = toytiling[i]->get_allwire();

    cout << i << " " << allcell.size() << " " << allwire.size() << endl;

    mergetiling[i] = new WireCell2dToy::MergeToyTiling(*toytiling[i],i,3,1);
    
   
    GeomCellSelection allmcell = mergetiling[i]->get_allcell();
    GeomWireSelection allmwire = mergetiling[i]->get_allwire();
    
    cout << i << " " << allmcell.size() << " " << allmwire.size() << endl;
    

    toymatrix[i] = new WireCell2dToy::ToyMatrix(*toytiling[i],*mergetiling[i]);
    if (toymatrix[i]->Get_Solve_Flag()==0){
      WireCell2dToy::ToyMatrixIterate toymatrix_it(*toymatrix[i]);
    }
    
    cout << "chi2: " << toymatrix[i]->Get_Chi2() << endl;
    cout << "NDF: " << toymatrix[i]->Get_ndf() << endl;
    

  }
  

  std::cout << "Starting MCMC" << std::endl;

  // //without  time information
  // for (int i=start_num;i!=end_num+1;i++){
  //   if (toymatrix[i]->Get_Solve_Flag()==0){
  //     GeomCellSelection allmcell = mergetiling[i]->get_allcell();
  //     WireCell2dToy::ToyMatrixMarkov toymatrix_markov(toymatrix[i],&allmcell);
  //     cout << "chi2: " << i << " " << toymatrix[i]->Get_Chi2() << endl;
  //     cout << "NDF: " << toymatrix[i]->Get_ndf() << endl;
  //   }
  // }


  //with time information
  if (start_num != end_num){
    int first_solve = -1;
    for (int i=start_num; i!=end_num+1;i++){
      if (toymatrix[i]->Get_Solve_Flag()!=0){
  	first_solve = i;
  	break;
      }
    }
  
    if (first_solve <0){
      for (int i=start_num;i!=end_num+1;i++){
  	if (toymatrix[i]->Get_Solve_Flag()==0){
  	  GeomCellSelection allmcell = mergetiling[i]->get_allcell();
  	  WireCell2dToy::ToyMatrixMarkov toymatrix_markov(toymatrix[i],&allmcell);
  	  cout << "chi2: " << i << " " << toymatrix[i]->Get_Chi2() << endl;
  	  cout << "NDF: " << toymatrix[i]->Get_ndf() << endl;
  	}
      }
    }else{
      for (int i=first_solve+1;i<=end_num-1;i++){
  	if (toymatrix[i]->Get_Solve_Flag()==0){
  	  GeomCellSelection allmcell = mergetiling[i]->get_allcell();
  	  WireCell2dToy::ToyMatrixMarkov toymatrix_markov(*toymatrix[i-1],*toymatrix[i],*toymatrix[i+1],*mergetiling[i-1],*mergetiling[i],*mergetiling[i+1],&allmcell);
  	  cout << "chi2: " << i << " " << toymatrix[i]->Get_Chi2() << endl;
  	  cout << "NDF: " << toymatrix[i]->Get_ndf() << endl;
  	}
      }
      
      if (toymatrix[end_num]->Get_Solve_Flag()==0){
  	GeomCellSelection allmcell = mergetiling[end_num]->get_allcell();
  	WireCell2dToy::ToyMatrixMarkov toymatrix_markov(*toymatrix[end_num-1],*toymatrix[end_num],*toymatrix[end_num-1],*mergetiling[end_num-1],*mergetiling[end_num],*mergetiling[end_num-1],&allmcell);
  	cout << "chi2: " << end_num << " " << toymatrix[end_num]->Get_Chi2() << endl;
  	cout << "NDF: " << toymatrix[end_num]->Get_ndf() << endl;
      }
      
      // go to early ones 
      for (int i=first_solve-1;i>=start_num+1;i--){
  	if (toymatrix[i]->Get_Solve_Flag()==0){
  	  GeomCellSelection allmcell = mergetiling[i]->get_allcell();
  	  WireCell2dToy::ToyMatrixMarkov toymatrix_markov(*toymatrix[i-1],*toymatrix[i],*toymatrix[i+1],*mergetiling[i-1],*mergetiling[i],*mergetiling[i+1],&allmcell);
  	  cout << "chi2: " << i << " " << toymatrix[i]->Get_Chi2() << endl;
  	  cout << "NDF: " << toymatrix[i]->Get_ndf() << endl;
  	}
      }
      
      if (toymatrix[start_num]->Get_Solve_Flag()==0){
  	GeomCellSelection allmcell = mergetiling[start_num]->get_allcell();
  	WireCell2dToy::ToyMatrixMarkov toymatrix_markov(*toymatrix[start_num+1],*toymatrix[start_num],*toymatrix[start_num+1],*mergetiling[start_num+1],*mergetiling[start_num],*mergetiling[start_num+1],&allmcell);
  	cout << "chi2: " << start_num << " " << toymatrix[start_num]->Get_Chi2() << endl;
  	cout << "NDF: " << toymatrix[start_num]->Get_ndf() << endl;
      }
    }
  }


  


 
  
  //do clustering ... 
  for (int i=start_num;i!=end_num+1;i++){
    GeomCellSelection pallmcell = mergetiling[i]->get_allcell();
    GeomCellSelection allmcell;
    for (int j=0;j!=pallmcell.size();j++){
      const GeomCell* mcell = pallmcell[j];
      if (toymatrix[i]->Get_Cell_Charge(mcell)> recon_threshold){
	allmcell.push_back(mcell);
      }
    }
    
    
    if (cluster_set.empty()){
      // if cluster is empty, just insert all the mcell, each as a cluster
      for (int j=0;j!=allmcell.size();j++){
	GeomCluster *cluster = new GeomCluster(*((MergeGeomCell*)allmcell[j]));
	cluster_set.insert(cluster);
      }
    }else{
      for (int j=0;j!=allmcell.size();j++){
	int flag = 0;
	int flag_save = 0;
	GeomCluster *cluster_save = 0;
	
	cluster_delset.clear();
	
	// loop through merged cell
	for (auto it = cluster_set.begin();it!=cluster_set.end();it++){
	  //loop through clusters
	  
	  flag += (*it)->AddCell(*((MergeGeomCell*)allmcell[j]));
	  if (flag==1 && flag != flag_save){
	    cluster_save = *it;
	  }else if (flag>1 && flag != flag_save){
	    cluster_save->MergeCluster(*(*it));
	    cluster_delset.insert(*it);
	  }
	  flag_save = flag;
  	  
	}
	
	for (auto it = cluster_delset.begin();it!=cluster_delset.end();it++){
	  cluster_set.erase(*it);
	  delete (*it);
	}
	
	if (flag==0){
	  GeomCluster *cluster = new GeomCluster(*((MergeGeomCell*)allmcell[j]));
	  cluster_set.insert(cluster);
	}
	
      }
    }
    
    int ncount_mcell_cluster = 0;
    for (auto it = cluster_set.begin();it!=cluster_set.end();it++){
      ncount_mcell_cluster += (*it)->get_allcell().size();
    }
    ncount_mcell += allmcell.size();
    cout << i << " " << allmcell.size()  << " " << cluster_set.size()  << endl;
  }
  
  int ncount_mcell_cluster = 0;
  for (auto it = cluster_set.begin();it!=cluster_set.end();it++){
    ncount_mcell_cluster += (*it)->get_allcell().size();
  }
  cout << "Summary: " << ncount << " " << ncount_mcell << " " << ncount_mcell_cluster << endl;
  

  TFile *file = new TFile(Form("shower3D_data_%d.root",eve_num),"RECREATE");
  TTree *t_rec = new TTree("T_rec","T_rec");
  TTree *t_rec_charge = new TTree("T_rec_charge","T_rec_charge");
  // TTree *t_rec_charge_blob = new TTree("T_rec_charge_blob","T_rec_charge_blob");

  Double_t x_save, y_save, z_save;
  Double_t charge_save;
  Double_t ncharge_save;
  Double_t chi2_save;
  Double_t ndf_save;

 
  t_rec->SetDirectory(file);
  t_rec->Branch("x",&x_save,"x/D");
  t_rec->Branch("y",&y_save,"y/D");
  t_rec->Branch("z",&z_save,"z/D");
  
  t_rec_charge->SetDirectory(file);
  t_rec_charge->Branch("x",&x_save,"x/D");
  t_rec_charge->Branch("y",&y_save,"y/D");
  t_rec_charge->Branch("z",&z_save,"z/D");
  t_rec_charge->Branch("q",&charge_save,"q/D");
  t_rec_charge->Branch("nq",&ncharge_save,"nq/D");
  t_rec_charge->Branch("chi2",&chi2_save,"chi2/D");
  t_rec_charge->Branch("ndf",&ndf_save,"ndf/D");

  // //blob stuff
  // t_rec_charge_blob->SetDirectory(file);
  // t_rec_charge_blob->Branch("x",&x_save,"x/D");
  // t_rec_charge_blob->Branch("y",&y_save,"y/D");
  // t_rec_charge_blob->Branch("z",&z_save,"z/D");
  // t_rec_charge_blob->Branch("q",&charge_save,"q/D");
  // t_rec_charge_blob->Branch("nq",&ncharge_save,"nq/D");
  
  TGraph2D *g = new TGraph2D();
  TGraph2D *g_rec = new TGraph2D();
  // TGraph2D *g_rec_blob = new TGraph2D();

 
  //save results 
  for (int i=start_num;i!=end_num+1;i++){
    //recon 1
    GeomCellSelection allcell = toytiling[i]->get_allcell();
    for (int j=0;j!=allcell.size();j++){
      Point p = allcell[j]->center();
      x_save = i*unit_dis/10.*2- unit_dis/10.*3200/2. -time_offset*unit_dis/10.;
      y_save = p.y/units::cm;
      z_save = p.z/units::cm;
      

      g->SetPoint(ncount,x_save,y_save,z_save);
      t_rec->Fill();

      ncount ++;
    }

    //recon 2 with charge
    GeomCellSelection allmcell = mergetiling[i]->get_allcell();
    for (int j=0;j!=allmcell.size();j++){
      MergeGeomCell *mcell = (MergeGeomCell*)allmcell[j];
      double charge = toymatrix[i]->Get_Cell_Charge(mcell,1);
      if (charge> recon_threshold){
  	//truth
  	for (int k=0;k!=mcell->get_allcell().size();k++){
  	  Point p = mcell->get_allcell().at(k)->center();
  	  x_save = i*unit_dis/10.*2 - unit_dis/10.*3200/2. -time_offset*unit_dis/10.;
  	  y_save = p.y/units::cm;
  	  z_save = p.z/units::cm;
  	  charge_save = charge/mcell->get_allcell().size();
  	  ncharge_save = mcell->get_allcell().size();
  	  chi2_save = toymatrix[i]->Get_Chi2();
  	  ndf_save = toymatrix[i]->Get_ndf();
	  
  	  g_rec->SetPoint(ncount1,x_save,y_save,z_save);
  	  t_rec_charge->Fill();
	  
  	  ncount1 ++;
  	}
      }
    }

    
  //   //recon 3 with charge and deblob
  //   for (int j=0;j!=allmcell.size();j++){
  //     MergeGeomCell *mcell = (MergeGeomCell*)allmcell[j];
  //     double charge = toymatrix[i]->Get_Cell_Charge(mcell,1);
  //     //if (charge> recon_threshold && !(mcell->IsSimpleBlob() && mcell->IsBlob())){
  //     if (charge> recon_threshold ){
  // 	for (int k=0;k!=mcell->get_allcell().size();k++){
  // 	  Point p = mcell->get_allcell().at(k)->center();
  // 	  x_save = i*unit_dis/10.*2 - unit_dis/10.*3200/2. -time_offset*unit_dis/10.;
  // 	  y_save = p.y/units::cm;
  // 	  z_save = p.z/units::cm;
  // 	  charge_save = charge/mcell->get_allcell().size();
  // 	  ncharge_save = mcell->get_allcell().size();
	  
  // 	  g_rec_blob->SetPoint(ncount2,x_save,y_save,z_save);
  // 	  t_rec_charge_blob->Fill();
	  
  // 	  ncount2 ++;
  // 	}
  //     }
  // }
  //    // if (toymatrix[i]->GetSimpleBlobReduction()){
  //    //   for (int j=0;j!=blobtiling[i]->Get_Cells().size();j++){
  //    // 	 const GeomCell *cell = blobtiling[i]->Get_Cells().at(j);
  //    // 	 Point p = cell->center();
  //    // 	 x_save = i*0.32 - unit_dis/10.*3200/2.-time_shift*0.16;
  //    // 	 y_save = p.y/units::cm;
  //    // 	 z_save = p.z/units::cm;
  //    // 	 charge_save = blobtiling[i]->Get_Cell_Charge(cell,1);
  //    // 	 ncharge_save = 1;
	 
  //    // 	 g_rec_blob->SetPoint(ncount2,x_save,y_save,z_save);
  //    // 	 t_rec_charge_blob->Fill();
	  
  //    // 	 ncount2 ++;
  //    //   }
  //    // }
    
  //   //save all results
  //   // file->Write(Form("toytiling_%d",i),toytiling[i]);
  //   // file->Write(Form("mergetiling_%d",i),mergetiling[i]);
  //   // file->Write(Form("truthtiling_%d",i),truthtiling[i]);
  //   // file->Write(Form("toymatrix_%d",i),toymatrix[i]);

  }
 
  

  g->Write("shower3D");
  g_rec->Write("shower3D_charge");
  // g_rec_blob->Write("shower3D_charge_blob");
  
  Double_t x,y,z;
  //save cluster
  int ncluster = 0;
  for (auto it = cluster_set.begin();it!=cluster_set.end();it++){
    ncount = 0;
    TGraph2D *g1 = new TGraph2D();
    for (int i=0; i!=(*it)->get_allcell().size();i++){
      const MergeGeomCell *mcell = (const MergeGeomCell*)((*it)->get_allcell().at(i));
      for (int j=0; j!=mcell->get_allcell().size();j++){
  	Point p = mcell->get_allcell().at(j)->center();
  	x = mcell->GetTimeSlice()*0.32 - unit_dis/10.*3200/2.;
  	y = p.y/units::cm;
  	z = p.z/units::cm;
  	g1->SetPoint(ncount,x,y,z);
  	ncount ++;
      }
    }
    // cout << ncount << endl;
    g1->Write(Form("cluster_%d",ncluster));
    ncluster ++;
  }

 
  
  TTree *Trun = new TTree("Trun","Trun");
  Trun->SetDirectory(file);
  Trun->Branch("eventNo",&event_no,"eventNo/I");
  Trun->Branch("runNo",&run_no,"runNo/I");
  Trun->Branch("subRunNo",&subrun_no,"runRunNo/I");
  


  file->Write();
  file->Close();

  

  return 0;
  
} // main()