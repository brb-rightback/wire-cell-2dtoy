#include "WCPSst/GeomDataSource.h"
#include "WCP2dToy/uBooNE_light_reco.h"
#include "TH1F.h"
#include "TH2F.h"

#include <iostream>

using namespace WCP;
using namespace std;

int main(int argc, char* argv[])
{
  if (argc < 4) {
    cerr << "usage: wire-cell-uboone /path/to/ChannelWireGeometry.txt /path/to/celltree.root eve_num " << endl;
    return 1;
  }
  TH1::AddDirectory(kFALSE);

  WCPSst::GeomDataSource gds(argv[1]);
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
  int eve_num = atoi(argv[3]);

  WCP2dToy::uBooNE_light_reco uboone_flash(root_file);
  //uboone_flash.load_event(eve_num);
  uboone_flash.load_event_raw(eve_num);
    
  TFile *file1 = new TFile(root_file);
  TTree *T = (TTree*)file1->Get("/Event/Sim");
  
  TClonesArray* cosmic_hg_wf = new TClonesArray;
  TClonesArray* cosmic_lg_wf = new TClonesArray;
  TClonesArray* beam_hg_wf = new TClonesArray;
  TClonesArray* beam_lg_wf = new TClonesArray;
  vector<short> *cosmic_hg_opch = new vector<short>;
  vector<short> *cosmic_lg_opch = new vector<short>;
  vector<short> *beam_hg_opch = new vector<short>;
  vector<short> *beam_lg_opch = new vector<short>;
  vector<double> *cosmic_hg_timestamp = new vector<double>;
  vector<double> *cosmic_lg_timestamp = new vector<double>;
  vector<double> *beam_hg_timestamp = new vector<double>;
  vector<double> *beam_lg_timestamp = new vector<double>;
  vector<short> *opch_to_opdet = new vector<short>; 
  std::vector<float> *op_gain = new std::vector<float>;
  std::vector<float> *op_gainerror = new std::vector<float>; 
  double triggerTime;
  
  // for "saturation" waveforms
  TClonesArray* op_wf = new TClonesArray("TH1S");
  std::vector<short> *op_femch = new std::vector<short>;
  //std::vector<int> *op_femch = new std::vector<int>;
  std::vector<double> *op_timestamp = new std::vector<double>;
  /*
  std::vector<float> *op_gain = new std::vector<float>;
  std::vector<float> *op_gainerror = new std::vector<float>; 
  double triggerTime;
  */
  int event_no, run_no, subrun_no;
  
  T->SetBranchAddress("cosmic_hg_wf",&cosmic_hg_wf);
  T->SetBranchAddress("cosmic_lg_wf",&cosmic_lg_wf);
  T->SetBranchAddress("beam_hg_wf",&beam_hg_wf);
  T->SetBranchAddress("beam_lg_wf",&beam_lg_wf);
  T->SetBranchAddress("cosmic_hg_opch",&cosmic_hg_opch);
  T->SetBranchAddress("cosmic_lg_opch",&cosmic_lg_opch);
  T->SetBranchAddress("beam_hg_opch",&beam_hg_opch);
  T->SetBranchAddress("beam_lg_opch",&beam_lg_opch);
  T->SetBranchAddress("cosmic_hg_timestamp",&cosmic_hg_timestamp);
  T->SetBranchAddress("cosmic_lg_timestamp",&cosmic_lg_timestamp);
  T->SetBranchAddress("beam_hg_timestamp",&beam_hg_timestamp);
  T->SetBranchAddress("beam_lg_timestamp",&beam_lg_timestamp);
  T->SetBranchAddress("opch_to_opdet",&opch_to_opdet); 
  T->SetBranchAddress("op_gain",&op_gain);
  T->SetBranchAddress("op_gainerror",&op_gainerror);
  /*
  T->SetBranchAddress("op_femch",&op_femch);
  T->SetBranchAddress("op_gain",&op_gain);
  T->SetBranchAddress("op_gainerror",&op_gainerror);
  T->SetBranchAddress("op_timestamp",&op_timestamp);
  T->SetBranchAddress("op_wf",&op_wf);
  */
  T->SetBranchAddress("triggerTime",&triggerTime);
  T->SetBranchStatus("eventNo",1);
  T->SetBranchAddress("eventNo" , &event_no);
  T->SetBranchStatus("runNo",1);
  T->SetBranchAddress("runNo"   , &run_no);
  T->SetBranchStatus("subRunNo",1);
  T->SetBranchAddress("subRunNo", &subrun_no);
  
  T->GetEntry(eve_num);

  TFile *file = new TFile(Form("flash_%d_%d_%d.root",run_no, subrun_no, event_no),"RECREATE");
  TTree *t1 = new TTree("T_data","T_data");
  t1->SetDirectory(file);

  t1->Branch("op_gain",&op_gain);
  t1->Branch("op_gainerror",&op_gainerror);
  t1->Branch("op_femch",&op_femch);
  t1->Branch("op_timestamp",&op_timestamp);
  t1->Branch("op_wf",&op_wf,256000,0);
  t1->Branch("triggerTime",&triggerTime);
  
  t1->Branch("runNo",&run_no);
  t1->Branch("subRunNo",&subrun_no);
  t1->Branch("eventNo",&event_no);

  op_wf = uboone_flash.get_rawWfm();
  op_femch = uboone_flash.get_rawChan();
  op_timestamp = uboone_flash.get_rawTimestamp();

  t1->Fill();

  TH2F *h1 = new TH2F("hraw","hraw",1500,0,1500,32,0,32);
  TH2F *h2 = new TH2F("hdecon","hdecon",250,0,250,32,0,32);
  TH2F *h3 = new TH2F("hl1","hl1",250,0,250,32,0,32);
  h1->SetDirectory(file);
  h2->SetDirectory(file);
  h3->SetDirectory(file);
  for (int i=0;i!=32;i++){
    TH1F *h10 = uboone_flash.get_raw_hist(i);
    TH1F *h20 = uboone_flash.get_decon_hist(i);
    TH1F *h30 = uboone_flash.get_l1_hist(i);
    for (int j=0;j!=1500;j++){
      h1->SetBinContent(j+1,i+1,h10->GetBinContent(j+1));
    }
    for (int j=0;j!=250;j++){
      h2->SetBinContent(j+1,i+1,h20->GetBinContent(j+1));
      h3->SetBinContent(j+1,i+1,h30->GetBinContent(j+1));
    }
  }

  TH1F *h_totPE = (TH1F*)uboone_flash.get_totPE()->Clone("totPE");
  TH1F *h_mult = (TH1F*)uboone_flash.get_mult()->Clone("mult");
  TH1F *h_l1_mult = (TH1F*)uboone_flash.get_l1_mult()->Clone("l1_mult");
  TH1F *h_l1_totPE = (TH1F*)uboone_flash.get_l1_totPE()->Clone("l1_totPE");

  h_totPE->SetDirectory(file);
  h_mult->SetDirectory(file);
  h_l1_mult->SetDirectory(file);
  h_l1_totPE->SetDirectory(file);

  TTree *T_flash = new TTree("T_flash","T_flash");
  T_flash->SetDirectory(file);
  int type;
  double low_time, high_time;
  double time;
  double total_PE;
  double PE[32],PE_err[32];
  std::vector<int> fired_channels;
  std::vector<double> l1_fired_time;
  std::vector<double> l1_fired_pe;

  T_flash->Branch("type",&type);
  T_flash->Branch("low_time",&low_time);
  T_flash->Branch("high_time",&high_time);
  T_flash->Branch("time",&time);
  T_flash->Branch("total_PE",&total_PE);
  T_flash->Branch("PE",PE,"PE[32]/D");
  T_flash->Branch("PE_err",PE_err,"PE_err[32]/D");
  T_flash->Branch("fired_channels",&fired_channels);
  T_flash->Branch("l1_fired_time",&l1_fired_time);
  T_flash->Branch("l1_fired_pe",&l1_fired_pe);

  WCP::OpflashSelection& flashes = uboone_flash.get_flashes();
  for (auto it = flashes.begin(); it!=flashes.end(); it++){
    fired_channels.clear();
    
    Opflash *flash = (*it);
    type = flash->get_type();
    low_time = flash->get_low_time();
    high_time = flash->get_high_time();
    time = flash->get_time();
    total_PE = flash->get_total_PE();
    for (int i=0;i!=32;i++){
      PE[i] = flash->get_PE(i);
      PE_err[i] = flash->get_PE_err(i);
      if (flash->get_fired(i))
	fired_channels.push_back(i);
    }
    l1_fired_time = flash->get_l1_fired_time();
    l1_fired_pe = flash->get_l1_fired_pe();
    T_flash->Fill();
    
  }

  file->Write();
  file->Close();
  return 1;
}
