#include "WireCell2dToy/ToySignalWien.h"

#include "WireCellData/GeomWire.h"
#include "TFile.h"
#include "TVirtualFFT.h"
#include "TRandom.h"
#include "TF1.h"

using namespace WireCell;

WireCell2dToy::ToySignalWienFDS::ToySignalWienFDS(WireCell::FrameDataSource& fds, const WireCell::GeomDataSource& gds,
						  int bins_per_frame1, int nframes_total)
  : fds(fds)
  , gds(gds)
  , max_frames(nframes_total)
{  
  bins_per_frame = bins_per_frame1;

  GeomWireSelection wires_u = gds.wires_in_plane(WirePlaneType_t(0));
  GeomWireSelection wires_v = gds.wires_in_plane(WirePlaneType_t(1));
  GeomWireSelection wires_w = gds.wires_in_plane(WirePlaneType_t(2));

  nwire_u = wires_u.size();
  nwire_v = wires_v.size();
  nwire_w = wires_w.size();

  hu = new TH1F*[nwire_u];
  hv = new TH1F*[nwire_v];
  hw = new TH1F*[nwire_w];
  
  nbin = fds.Get_Bins_Per_Frame();
  
  for (int i=0;i!=nwire_u;i++){
    hu[i] = new TH1F(Form("U4_%d",i),Form("U4_%d",i),nbin,0,nbin);
  }
  for (int i=0;i!=nwire_v;i++){
    hv[i] = new TH1F(Form("V4_%d",i),Form("V4_%d",i),nbin,0,nbin);
  }
  for (int i=0;i!=nwire_w;i++){
    hw[i] = new TH1F(Form("W4_%d",i),Form("W4_%d",i),nbin,0,nbin);
  }
  
  #include "data.txt"

  gu = new TGraph(5000,xu,yu);
  gv = new TGraph(5000,xv,yv);
  gw = new TGraph(5000,xw,yw);

  hur = new TH1F("hur1","hur1",nbin,0,nbin); // half us tick
  hvr = new TH1F("hvr1","hvr1",nbin,0,nbin); // half us tick
  hwr = new TH1F("hwr1","hwr1",nbin,0,nbin); // half us tick
  
  for (int i=0; i!=nbin; i++){  
    double time = hur->GetBinCenter(i+1)/2.-50;
    hur->SetBinContent(i+1,gu->Eval(time));
    hvr->SetBinContent(i+1,gv->Eval(time));
    hwr->SetBinContent(i+1,gw->Eval(time));
  } 
  
  hmr_u = 0;
  hmr_v = 0;
  hmr_w = 0;
  hpr_u = 0;
  hpr_v = 0;
  hpr_w = 0;
  
}

int WireCell2dToy::ToySignalWienFDS::size() const{
  return max_frames;
}

void WireCell2dToy::ToySignalWienFDS::Save(){
  TFile *file = new TFile("temp_wien.root","RECREATE");
  for (int i=0;i!=nwire_u;i++){
    TH1F *huu = (TH1F*)hu[i]->Clone(Form("U1_%d",i));
  }
  for (int i=0;i!=nwire_v;i++){
    TH1F *hvv = (TH1F*)hv[i]->Clone(Form("V1_%d",i));
  }
  for (int i=0;i!=nwire_w;i++){
    TH1F *hww = (TH1F*)hw[i]->Clone(Form("W1_%d",i));
  }
  file->Write();
  file->Close();
}

int WireCell2dToy::ToySignalWienFDS::jump(int frame_number){
  // fill the frame data ... 
  frame.clear();
  int scale = nbin/bins_per_frame;

  
  fds.jump(frame_number);
  const Frame& frame1 = fds.get();
  size_t ntraces = frame1.traces.size();

  TVirtualFFT::SetTransform(0);
  
  
  hmr_u = hur->FFT(hmr_u,"MAG");
  hmr_v = hvr->FFT(hmr_v,"MAG");
  hmr_w = hwr->FFT(hmr_w,"MAG");

  hpr_u = hur->FFT(hpr_u,"PH");
  hpr_v = hvr->FFT(hpr_v,"PH");
  hpr_w = hwr->FFT(hpr_w,"PH");
    
  TH1 *hm = 0;
  TH1 *hp = 0;
  double value_re[9600];
  double value_im[9600];
  TVirtualFFT *ifft;
  TH1 *fb = 0;
  int n = nbin;
  TH1 *hmr, *hpr;
  
  // do FFT again to remove response function and apply filter

  TF1 *filter_u = new TF1("filter_u","(x>0.0)*gaus*exp(-0.5*pow(x/[3],[4]))");
  double par[5]={1.73/0.959301, 1.69, 1.55, 0.19, 3.75};
  filter_u->SetParameters(par);

  TF1 *filter_v = new TF1("filter_v","(x>0.0)*gaus*exp(-0.5*pow(x/[3],[4]))");
  double par1[5]={1.74/0.941034, 1.46, 1.33, 0.23, 4.89};
  filter_v->SetParameters(par1);

  TF1 *filter_w = new TF1("filter_y","(x>0.0)*[0]*exp(-0.5*(((x-[1])/[2])^2)^[3])");
  double par2[4]={1.03/0.995635, 0.08, 0.15, 2.17};
  filter_w->SetParameters(par2);


  
  
 
      

      // double rho = hm->GetBinContent(j+1)/hmr->GetBinContent(j+1)*filter_u->Eval(freq);
      
  for (size_t ind=0; ind<ntraces; ++ind) {
    const Trace& trace = frame1.traces[ind];
    int tbin = trace.tbin;
    int chid = trace.chid;
    int nbins = trace.charge.size();
    
    //std::cout << tbin << " " << chid << " " << nbins << std::endl;

    TH1F *htemp;
    TF1 *filter;
    
    if (chid < nwire_u){
      htemp = hu[chid];
      hmr = hmr_u;
      hpr = hpr_u;
      filter = filter_u;
    }else if (chid < nwire_u + nwire_v){
      htemp = hv[chid - nwire_u];
      hmr = hmr_v;
      hpr = hpr_v;
      filter = filter_v;
    }else{
      htemp = hw[chid - nwire_u - nwire_v];
      hmr = hmr_w;
      hpr = hpr_w;
      filter = filter_w;
    }

    for (int i = tbin;i!=tbin+nbins;i++){
      htemp->SetBinContent(i+1,trace.charge.at(i));
    }
        
    hm = htemp->FFT(hm,"MAG");
    hp = htemp->FFT(hp,"PH");
    
    for (int i=0;i!=nbin;i++){
      
      double freq;
      if (i < nbin/2.){
       	freq = i/(1.*nbin)*2.;
      }else{
       	freq = (nbin - i)/(1.*nbin)*2.;
      }

      double rho = hm->GetBinContent(i+1)/hmr->GetBinContent(i+1)*filter->Eval(freq);
      double phi = hp->GetBinContent(i+1) - hpr->GetBinContent(i+1);

      if (i==0) rho = 0;
      value_re[i] = rho*cos(phi)/nbin;
      value_im[i] = rho*sin(phi)/nbin;
    }
    
    ifft = TVirtualFFT::FFT(1,&n,"C2R M K");
    ifft->SetPointsComplex(value_re,value_im);
    ifft->Transform();
    fb = TH1::TransformHisto(ifft,fb,"Re");

    for (int i=0;i!=nbin;i++){
      htemp->SetBinContent(i+1,fb->GetBinContent(i+1)/( 7.8*4096./2000.));
    }

    

    //correct baseline 
    double max = htemp->GetMaximum();
    double min = htemp->GetMinimum();
    int nbin_b = max - min;
    if (nbin_b ==0) nbin_b = 1;
    TH1F *h1 = new TH1F("h1","h1",nbin_b,min,max);
    for (int j=0;j!=nbin;j++){
      h1->Fill(htemp->GetBinContent(j+1));
    }
    float ped = h1->GetMaximumBin()*(max-min)/(nbin_b*1.) + min;
    float ave=0,ncount = 0;
    for (int j=0;j!=nbin;j++){
      if (fabs(htemp->GetBinContent(j+1)-ped)<400){
    	ave +=htemp->GetBinContent(j+1);
    	ncount ++;
      }
    }
    if (ncount==0) ncount=1;
    ave = ave/ncount;
    
    for (int j=0;j!=nbin;j++){
      double content = htemp->GetBinContent(j+1);
      content -= ave;
      htemp->SetBinContent(j+1,content);
    }
    delete h1;
    
    //save into frame

    Trace t;
    t.chid = chid;
    t.tbin = tbin;
    t.charge.resize(bins_per_frame, 0.0);
    for (int j=0;j!=bins_per_frame;j++){
      t.charge.at(j) = 0;
      for (int k=0;k!=scale;k++){
	t.charge.at(j) += htemp->GetBinContent(scale*j+k+1);
      }
    }
    frame.traces.push_back(t);

  }
 
  delete filter_u;
  delete filter_v;
  delete filter_w;
  
  frame.index = frame_number;
  return frame.index;
}

WireCell2dToy::ToySignalWienFDS::~ToySignalWienFDS(){
  for (int i=0;i!=nwire_u;i++){
    delete hu[i] ;
  }
  delete hu;
  for (int i=0;i!=nwire_v;i++){
    delete hv[i] ;
  }
  delete hv;
  for (int i=0;i!=nwire_w;i++){
    delete hw[i] ;
  }
  delete hw;

  delete gu;
  delete gv;
  delete gw;

  delete hur;
  delete hvr;
  delete hwr;
}