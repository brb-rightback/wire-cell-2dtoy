#include "WireCellSst/GeomDataSource.h"
#include "WireCellSst/DatauBooNEFrameDataSource.h"
#include "WireCellSst/ToyuBooNESliceDataSource.h"
#include "WireCell2dToy/ToyEventDisplay.h"
#include "WireCell2dToy/ToyTiling.h"
#include "WireCell2dToy/BadTiling.h"

#include "WireCell2dToy/MergeToyTiling.h"
#include "WireCell2dToy/TruthToyTiling.h"
#include "WireCell2dToy/SimpleBlobToyTiling.h"

#include "WireCell2dToy/ToyMatrix.h"
#include "WireCell2dToy/ToyMatrixExclusive.h"
#include "WireCell2dToy/ToyMatrixKalman.h"
#include "WireCell2dToy/ToyMatrixIterate.h"
#include "WireCell2dToy/ToyMatrixIterate_SingleWire.h"
#include "WireCell2dToy/ToyMatrixIterate_Only.h"


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
#include "WireCell2dToy/DataSignalWien_ROI.h"

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
    cerr << "usage: wire-cell-uboone /path/to/ChannelWireGeometry.txt /path/to/celltree.root eve_num " << endl;
    return 1;
  }

  int two_plane = 0;
  int save_file = 0;
  int nt_off1 = 0;
  int nt_off2 = 0;
      
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
  
  
  //float unit_dis = 1.01483;  // 58KV @ 226.5 V/cm
  //float unit_dis = 1.14753;  // 70 KV @ 226.5 V/cm
  float unit_dis = 1.119;  // 70 KV @ 273 V/cm

  //final offset after time scan (70kV)
  float toffset_1=-0.787;
  float toffset_2=-0.603;
  float toffset_3=0.0;
  

  int total_time_bin=9594;
  int recon_threshold = 2000;
  int frame_length = 3200;
  int max_events = 100;
  int eve_num  = atoi(argv[3]);
  int nrebin = 6;
  float threshold_u = 5.87819e+02 * 4.0;
  float threshold_v = 8.36644e+02 * 4.0;
  float threshold_w = 5.67974e+02 * 4.0;

  float threshold_ug = 755.96;
  float threshold_vg = 822.81;
  float threshold_wg = 510.84;
  
  int time_offset = -92.;
  
  const char* root_file = argv[2];
   
  int run_no, subrun_no, event_no;
  
  WireCellSst::DatauBooNEFrameDataSource data_fds(root_file,gds,total_time_bin);
  if (save_file != 2){
    data_fds.jump(eve_num);
    if (save_file == 1)
      data_fds.Save();
  }
  
  run_no = data_fds.get_run_no();
  subrun_no = data_fds.get_subrun_no();
  event_no = data_fds.get_event_no();
  
  cout << "Run No: " << run_no << " " << subrun_no << " " << eve_num << endl;
  
  ChirpMap& uplane_map = data_fds.get_u_cmap();
  ChirpMap& vplane_map = data_fds.get_v_cmap();
  ChirpMap& wplane_map = data_fds.get_w_cmap();
  
  cout << "Deconvolution with Wiener filter" << endl; 
  WireCell2dToy::DataSignalWienROIFDS wien_fds(data_fds,gds,uplane_map, vplane_map, wplane_map, total_time_bin/nrebin,max_events,toffset_1,toffset_2,toffset_3); // weiner smearing for hit identification
  wien_fds.set_flag_2D(0);
  
  if (save_file !=2 ){
    wien_fds.jump(eve_num);
    if (save_file == 1)
      wien_fds.Save();
  }else{
  }



  TFile *file = new TFile(Form("1D_display_%d_%d_%d.root",run_no,subrun_no,eve_num),"RECREATE");

  GeomWireSelection wires_u = gds.wires_in_plane(WirePlaneType_t(0));
  GeomWireSelection wires_v = gds.wires_in_plane(WirePlaneType_t(1));
  GeomWireSelection wires_w = gds.wires_in_plane(WirePlaneType_t(2));

  Int_t nwire_u = wires_u.size();
  Int_t nwire_v = wires_v.size();
  Int_t nwire_w = wires_w.size();

  TH2I *hu_orig = new TH2I("hu_orig","hu_orig",nwire_u,-0.5,nwire_u-0.5,total_time_bin,0,total_time_bin);
  TH2I *hv_orig = new TH2I("hv_orig","hv_orig",nwire_v,-0.5+nwire_u,nwire_v-0.5+nwire_u,total_time_bin,0,total_time_bin);
  TH2I *hw_orig = new TH2I("hw_orig","hw_orig",nwire_w,-0.5+nwire_u+nwire_v,nwire_w-0.5+nwire_u+nwire_v,total_time_bin,0,total_time_bin);

  TH1I *hu_baseline = new TH1I("hu_baseline","hu_threshold",nwire_u,-0.5,-0.5+nwire_u);
  TH1I *hv_baseline = new TH1I("hv_baseline","hv_threshold",nwire_v,-0.5+nwire_u,-0.5+nwire_u+nwire_v);
  TH1I *hw_baseline = new TH1I("hw_baseline","hw_threshold",nwire_w,-0.5+nwire_u+nwire_v,-0.5+nwire_u+nwire_v+nwire_w);

  
  TH1I *hu_threshold = new TH1I("hu_threshold","hu_basline",nwire_u,-0.5,-0.5+nwire_u);
  TH1I *hv_threshold = new TH1I("hv_threshold","hv_basline",nwire_v,-0.5+nwire_u,-0.5+nwire_u+nwire_v);
  TH1I *hw_threshold = new TH1I("hw_threshold","hw_basline",nwire_w,-0.5+nwire_u+nwire_v,-0.5+nwire_u+nwire_v+nwire_w);

  TH2F *hu_raw = new TH2F("hu_raw","hu_raw",nwire_u,-0.5,nwire_u-0.5,total_time_bin,0,total_time_bin);
  TH2F *hv_raw = new TH2F("hv_raw","hv_raw",nwire_v,-0.5+nwire_u,nwire_v-0.5+nwire_u,total_time_bin,0,total_time_bin);
  TH2F *hw_raw = new TH2F("hw_raw","hw_raw",nwire_w,-0.5+nwire_u+nwire_v,nwire_w-0.5+nwire_u+nwire_v,total_time_bin,0,total_time_bin);

  TH2F *hu_decon = new TH2F("hu_decon","hu_decon",nwire_u,-0.5,nwire_u-0.5,total_time_bin/nrebin,0,total_time_bin);
  TH2F *hv_decon = new TH2F("hv_decon","hv_decon",nwire_v,-0.5+nwire_u,nwire_v-0.5+nwire_u,total_time_bin/nrebin,0,total_time_bin);
  TH2F *hw_decon = new TH2F("hw_decon","hw_decon",nwire_w,-0.5+nwire_u+nwire_v,nwire_w-0.5+nwire_u+nwire_v,total_time_bin/nrebin,0,total_time_bin);
  

  TH2F *htemp;
  TH2F *htemp1;
  
  const Frame& frame = data_fds.get();
  size_t ntraces = frame.traces.size();
  for (size_t ind=0; ind<ntraces; ++ind) {
    const Trace& trace = frame.traces[ind];
    int tbin = trace.tbin;
    int chid = trace.chid;
    int nbins = trace.charge.size();
    WirePlaneType_t plane = gds.by_channel(chid).at(0)->plane();
    if (plane == WirePlaneType_t(0)){
      htemp = hu_raw;
      
    }else if (plane == WirePlaneType_t(1)){
      htemp = hv_raw;
      
      chid -= nwire_u;
    }else if (plane == WirePlaneType_t(2)){
      htemp = hw_raw;
     
      chid -= nwire_u + nwire_v;
    }
    for (int i = tbin;i!=tbin+nbins;i++){
      int tt = i+1;
      htemp->SetBinContent(chid+1,tt,trace.charge.at(i));
    }
  }
  
  const Frame& frame1 = wien_fds.get();
  ntraces = frame1.traces.size();
  for (size_t ind=0; ind<ntraces; ++ind) {
    const Trace& trace = frame1.traces[ind];
    int tbin = trace.tbin;
    int chid = trace.chid;
    int nbins = trace.charge.size();
    WirePlaneType_t plane = gds.by_channel(chid).at(0)->plane();
    if (plane == WirePlaneType_t(0)){
      htemp1 = hu_decon;
    }else if (plane == WirePlaneType_t(1)){
      htemp1 = hv_decon;
      chid -= nwire_u;
    }else if (plane == WirePlaneType_t(2)){
      htemp1 = hw_decon;
      chid -= nwire_u + nwire_v;
    }
     for (int i = tbin;i!=tbin+nbins;i++){
      int tt = i+1;
      htemp1->SetBinContent(chid+1,tt,trace.charge.at(i));
    }
  }

  // save original data ... 
  const char* tpath = "/Event/Sim";
  TFile tfile(root_file,"read");
  TTree* tree = dynamic_cast<TTree*>(tfile.Get(tpath));
  tree->SetBranchStatus("*",0);
  std::vector<int> *channelid = new std::vector<int>;
  TClonesArray* esignal = new TClonesArray;
  
  tree->SetBranchStatus("raw_channelId",1);
  tree->SetBranchAddress("raw_channelId", &channelid);
  tree->SetBranchStatus("raw_wf",1);
  tree->SetBranchAddress("raw_wf", &esignal);
  tree->GetEntry(eve_num);
  
  int nchannels = channelid->size();
  TH2I *htemp2;
  TH1I *htemp3 = new TH1I("htemp3","htemp3",4096,0,4096); //12-bit ADC
  TH1I *htemp4;

  for (size_t ind=0; ind < nchannels; ++ind) {
    TH1F* signal = dynamic_cast<TH1F*>(esignal->At(ind));
    int chid = channelid->at(ind);
     
    WirePlaneType_t plane = gds.by_channel(chid).at(0)->plane();
    if (plane == WirePlaneType_t(0)){
      htemp2 = hu_orig;
      htemp4 = hu_baseline;
    }else if (plane == WirePlaneType_t(1)){
      htemp2 = hv_orig;
      htemp4 = hv_baseline;
      chid -= nwire_u;
    }else if (plane == WirePlaneType_t(2)){
      htemp2 = hw_orig;
      htemp4 = hw_baseline;
      chid -= nwire_u + nwire_v;
    }
    
    htemp3->Reset();
    for (int ibin=0; ibin != total_time_bin; ibin++) {
      int tt = ibin+1;
      htemp2->SetBinContent(chid+1,tt,int(signal->GetBinContent(ibin+1)));
      htemp3->Fill(int(signal->GetBinContent(ibin+1)));
    }
    htemp4->SetBinContent(chid+1,htemp3->GetMaximumBin()-1);
  }

  std::vector<float>& uplane_rms = wien_fds.get_uplane_rms();
  std::vector<float>& vplane_rms = wien_fds.get_vplane_rms();
  std::vector<float>& wplane_rms = wien_fds.get_wplane_rms();
  for (Int_t i=0;i!=uplane_rms.size();i++){
    hu_threshold->SetBinContent(i+1,uplane_rms.at(i)*3.6 );
  }
  for (Int_t i=0;i!=vplane_rms.size();i++){
    hv_threshold->SetBinContent(i+1,vplane_rms.at(i)*3.6 );
  }
  for (Int_t i=0;i!=wplane_rms.size();i++){
    hw_threshold->SetBinContent(i+1,wplane_rms.at(i)*3.6 );
  }

  // finish saving

  TTree *Trun = new TTree("Trun","Trun");
  Trun->SetDirectory(file);

  int detector = 0; // MicroBooNE
  Trun->Branch("detector",&detector,"detector/I");

  Trun->Branch("eventNo",&event_no,"eventNo/I");
  Trun->Branch("runNo",&run_no,"runNo/I");
  Trun->Branch("subRunNo",&subrun_no,"subRunNo/I");
  
  Trun->Branch("unit_dis",&unit_dis,"unit_dis/F");
  Trun->Branch("toffset_uv",&toffset_1,"toffset_uv/F");
  Trun->Branch("toffset_uw",&toffset_2,"toffset_uw/F");
  Trun->Branch("toffset_u",&toffset_3,"toffset_u/F");
  Trun->Branch("total_time_bin",&total_time_bin,"total_time_bin/I");
  Trun->Branch("recon_threshold",&recon_threshold,"recon_threshold/I");
  Trun->Branch("frame_length",&frame_length,"frame_length/I");
  Trun->Branch("max_events",&max_events,"max_events/I");
  Trun->Branch("eve_num",&eve_num,"eve_num/I");
  Trun->Branch("nrebin",&nrebin,"nrebin/I");
  Trun->Branch("threshold_u",&threshold_u,"threshold_u/F");
  Trun->Branch("threshold_v",&threshold_v,"threshold_v/F");
  Trun->Branch("threshold_w",&threshold_w,"threshold_w/F");
  Trun->Branch("time_offset",&time_offset,"time_offset/I");

  Trun->Fill();

  TTree *T_lf = new TTree("T_lf","T_lf");
  Int_t channel;
  T_lf->SetDirectory(file);
  T_lf->Branch("channel",&channel,"channel/I");
  std::set<int> lf_noisy_channels = data_fds.get_lf_noisy_channels();
  for (auto it = lf_noisy_channels.begin(); it!= lf_noisy_channels.end(); it++){
    channel = *it;
    T_lf->Fill();
  }


  TTree *T_bad = new TTree("T_bad","T_bad");
  Int_t chid, plane;
  Int_t start_time,end_time;
  T_bad->Branch("chid",&chid,"chid/I");
  T_bad->Branch("plane",&plane,"plane/I");
  T_bad->Branch("start_time",&start_time,"start_time/I");
  T_bad->Branch("end_time",&end_time,"end_time/I");
  T_bad->SetDirectory(file);
  for (auto it = uplane_map.begin(); it!=uplane_map.end();it++){
    chid = it->first;
    plane = 0;
    start_time = it->second.first;
    end_time = it->second.second;
    T_bad->Fill();
  }
  for (auto it = vplane_map.begin(); it!=vplane_map.end();it++){
    chid = it->first + nwire_u;
    plane = 1;
    start_time = it->second.first;
    end_time = it->second.second;
    T_bad->Fill();
  }
  for (auto it = wplane_map.begin(); it!=wplane_map.end();it++){
    chid = it->first + nwire_u + nwire_v;
    plane = 2;
    start_time = it->second.first;
    end_time = it->second.second;
    T_bad->Fill();
  }
  



  file->Write();
  file->Close();
  
  
  
  // cout << "Deconvolution with Gaussian filter" << endl;
  // WireCell2dToy::DataSignalGausFDS gaus_fds(data_fds,gds,total_time_bin/nrebin,max_events,toffset_1,toffset_2,toffset_3); // gaussian smearing for charge estimation
  // if (save_file != 2){
  //   gaus_fds.jump(eve_num);
  //   if (save_file == 1)
  //     gaus_fds.Save();
  // }else{
  
  // }
  
  // data_fds.Clear();

  
  
  // GeomWireSelection wires_u = gds.wires_in_plane(WirePlaneType_t(0));
  // GeomWireSelection wires_v = gds.wires_in_plane(WirePlaneType_t(1));
  // GeomWireSelection wires_w = gds.wires_in_plane(WirePlaneType_t(2));

  // int nwire_u = wires_u.size();
  // int nwire_v = wires_v.size();
  // int nwire_w = wires_w.size();

}
