#include "WCP2dToy/ToyMatching.h"

#include "WCPRess/LassoModel.h"
#include "WCPRess/ElasticNetModel.h"
#include <Eigen/Dense>

#include "TChain.h"
#include "TGraph.h"
#include "TVector3.h"

#include "WCPData/TPCParams.h"
#include "WCPData/Singleton.h"
#include "WCPData/FlashTPCBundle.h"

#include <fstream>

using namespace Eigen;
using namespace WCP;

int WCP2dToy::convert_xyz_voxel_id(WCP::Point &p){
  // int voxel_x_id = std::round((p.x/units::cm+64.825-5.14667/2.)/5.14667);
  // int voxel_y_id = std::round((p.y/units::cm+193-5.14667/2.)/5.14667);
  // int voxel_z_id = std::round((p.z/units::cm+128.243-3.23122/2.)/3.23122);
  
  int voxel_x_id = std::round((p.x/units::cm+63.435-5.1096/2.)/5.1095);
  int voxel_y_id = std::round((p.y/units::cm+191.61-5.1096/2.)/5.1096);
  int voxel_z_id = std::round((p.z/units::cm+92.375-3.05437/2.)/3.05437);
  //fit
  // int voxel_y_id = std::round((p.y/units::cm+188.416-5.11698/2.)/5.11698);
  // int voxel_z_id = std::round((p.z/units::cm+88.3554-3.05768/2.)/3.05768);
  
  if (voxel_x_id<0) voxel_x_id=0;
  if (voxel_x_id>=75) voxel_x_id=74;
  if (voxel_y_id<0) voxel_y_id=0;
  if (voxel_y_id>=75) voxel_y_id=74;
  if (voxel_z_id<0) voxel_z_id=0;
  if (voxel_z_id>=400) voxel_z_id=399;

  int voxel_id = voxel_z_id*75*75 + voxel_y_id*75 + voxel_x_id;
  return voxel_id;
} 

/*
WCP2dToy::Photon_Library::Photon_Library(Int_t run_no, bool flag_data, bool flag_add_light_yield_err){
  rel_light_yield_err = 0;
  scaling_light_mag = 0.01 * 1.5;
  Double_t yield_run_no[37]={5590, 5934, 6207, 6427, 6617, 6854, 7059, 7305, 7648, 8199, 8518, 8871, 9209, 9468, 9652, 10478, 10701, 10924, 11197, 11605, 11816, 12021, 12344, 12505, 13521, 13725, 14034, 14256, 14527, 14773, 15013, 15426, 15922, 16218, 16643, 16977, 17417};
  Double_t yield_ratio[37]={1, 1, 1, 0.99, 1.01, 0.97, 0.97, 0.97, 0.97, 0.96, 0.97, 0.9, 0.83, 0.82, 0.8, 0.77, 0.77, 0.77, 0.76, 0.71, 0.73, 0.71, 0.7, 0.68, 0.64, 0.65, 0.65, 0.64, 0.64, 0.63, 0.63, 0.64, 0.64, 0.64, 0.64, 0.62, 0.62};
  Double_t yield_ratio_err[37]={0, 0, 0, 0.01, 0, 0.01, 0.01, 0, 0.01, 0.02, 0.01, 0.03, 0.05, 0.06, 0.06, 0.07, 0.05, 0.08, 0.08, 0.09, 0.09, 0.1, 0.16, 0.1, 0.1, 0.1, 0.11, 0.1, 0.11, 0.11, 0.11, 0.1, 0.1, 0.11, 0.11, 0.1, 0.1};
  Int_t start_run_no = 5590;
  Int_t end_run_no = 17417;
  TGraph gratio(37, yield_run_no, yield_ratio);
  TGraph gratio_err(37, yield_run_no, yield_ratio_err);

//  std::map<int,int> map_lib_pmt,map_pmt_lib;
  map_lib_pmt[1]=2; map_pmt_lib[2]=1; 
  map_lib_pmt[0]=4; map_pmt_lib[4]=0; 
  map_lib_pmt[3]=0; map_pmt_lib[0]=3; 
  map_lib_pmt[2]=5; map_pmt_lib[5]=2; 
  map_lib_pmt[5]=1; map_pmt_lib[1]=5; 
  map_lib_pmt[4]=6; map_pmt_lib[6]=4; 
  map_lib_pmt[6]=3; map_pmt_lib[3]=6; 
  
  map_lib_pmt[9]=7; map_pmt_lib[7]=9; 
  map_lib_pmt[7]=9; map_pmt_lib[9]=7; 
  map_lib_pmt[8]=11; map_pmt_lib[11]=8; 
  map_lib_pmt[11]=8; map_pmt_lib[8]=11;  
  map_lib_pmt[10]=12; map_pmt_lib[12]=10; 
  map_lib_pmt[12]=10; map_pmt_lib[10]=12; 

  map_lib_pmt[14]=13; map_pmt_lib[13]=14;  
  map_lib_pmt[13]=15; map_pmt_lib[15]=13; 
  map_lib_pmt[15]=17; map_pmt_lib[17]=15; 
  map_lib_pmt[17]=14; map_pmt_lib[14]=17; 
  map_lib_pmt[16]=18; map_pmt_lib[18]=16; 
  map_lib_pmt[18]=16; map_pmt_lib[16]=18; 

  map_lib_pmt[21]=19; map_pmt_lib[19]=21; 
  map_lib_pmt[22]=20; map_pmt_lib[20]=22; 
  map_lib_pmt[19]=21; map_pmt_lib[21]=19; 
  map_lib_pmt[20]=23; map_pmt_lib[23]=20; 
  map_lib_pmt[23]=24; map_pmt_lib[24]=23; 
  map_lib_pmt[24]=22; map_pmt_lib[22]=24; 

  map_lib_pmt[26]=25; map_pmt_lib[25]=26; 
  map_lib_pmt[27]=30; map_pmt_lib[30]=27; 
  map_lib_pmt[28]=31; map_pmt_lib[31]=28; 
  map_lib_pmt[31]=29; map_pmt_lib[29]=31;
  // original map
  map_lib_pmt[25]=28; map_pmt_lib[28]=25; 
  map_lib_pmt[30]=27; map_pmt_lib[27]=30; 
  map_lib_pmt[29]=26; map_pmt_lib[26]=29;

  // fixed map ... (if not swap in the flash reconstruction ...)
  // map_lib_pmt[25]=27; map_pmt_lib[27]=25; 
  // map_lib_pmt[30]=26; map_pmt_lib[26]=30; 
  // map_lib_pmt[29]=28; map_pmt_lib[28]=29;
  
  TChain *T = new TChain("/pmtresponse/PhotonLibraryData","/pmtresponse/PhotonLibraryData");
  T->AddFile("./uboone_photon_library.root");
  //std::cout << T->GetEntries();
  if (T->GetEntries()<=0) {
      std::cout << "Error: 2dToy::tpc_light_match failed to read uboone_photon_library.root"<<std::endl;
      exit(1);
  }
  Int_t Voxel;
  Int_t OpChannel;
  Float_t Visibility;
  T->SetBranchAddress("Voxel",&Voxel);
  T->SetBranchAddress("OpChannel",&OpChannel);
  T->SetBranchAddress("Visibility",&Visibility);

//  std::vector<std::list<std::pair<int,float>>> photon_library;
  library.resize(400*75*75);
  for (int i=0;i!=T->GetEntries();i++){
    T->GetEntry(i);
    library.at(Voxel).push_back(std::make_pair(OpChannel,Visibility));
  }
  
  // hack
  //run_no = 17394;
  
  // for data, scale down the light ...
  if (flag_data){
    double scaling_additional = 1;
    double scaling_additional_err = 0;
    if (run_no < start_run_no){
      scaling_additional = yield_ratio[0];
      scaling_additional_err = yield_ratio_err[0];
    }else if (run_no > end_run_no){
      scaling_additional = yield_ratio[36];
      scaling_additional_err = yield_ratio_err[36];
    }else{
      scaling_additional = gratio.Eval(run_no);
      scaling_additional_err = gratio_err.Eval(run_no);
    }
    scaling_light_mag *= scaling_additional;
    if (flag_add_light_yield_err)
      rel_light_yield_err = scaling_additional_err/scaling_additional;
  }
}

*/

void WCP2dToy::calculate_pred_pe(int run_no, double eventTime, int time_offset, int nrebin, double time_slice_width, WCP::Photon_Library *pl, FlashTPCBundle* bundle, std::vector<double>* pred_pmt_light,
				 std::vector<std::pair<WCP::PR3DCluster*,double>>* additional_clusters, PR3DClusterSelection* other_clusters, PR3DClusterSelection* more_clusters, bool &flag_good_bundle, bool flag_data, bool flag_timestamp){

  //  std::cout << "ZXin_4: " << eventTime << " " << flag_timestamp << std::endl;

  TPCParams& mp = Singleton<TPCParams>::Instance();
  
	double rel_light_yield_err = pl->rel_light_yield_err;
	double scaling_light_mag = pl->scaling_light_mag;
	std::map<int,int>* map_lib_pmt = &pl->map_lib_pmt;
	std::map<int,int>* map_pmt_lib = &pl->map_pmt_lib;
	std::vector<std::list<std::pair<int,float>>>* photon_library = &pl->library;

	double high_x_cut = 256 * units::cm;
	double high_x_cut_ext1 = + 1.2*units::cm;
	//double high_x_cut_ext1 = + 2.0*units::cm;
	double high_x_cut_ext2 = - 2.0*units::cm;
	double low_x_cut = 0*units::cm;
	double low_x_cut_ext1 = - 2*units::cm;
	double low_x_cut_ext2 = + 4.0*units::cm;

	Opflash *flash = bundle->get_flash();
	double offset_x = (flash->get_time() - time_offset)*2./nrebin*time_slice_width;
  	PR3DCluster* main_cluster = bundle->get_main_cluster();

  	double first_pos_x = (*((main_cluster->get_time_cells_set_map().begin())->second.begin()))->get_sampling_points().front().x;
  	double last_pos_x = (*((main_cluster->get_time_cells_set_map().rbegin())->second.begin()))->get_sampling_points().front().x;

	bool flag_spec_end = false;
	
	// improve the position code ... 
	if (first_pos_x - offset_x <= low_x_cut + low_x_cut_ext1 &&
	    first_pos_x - offset_x > low_x_cut - 120*units::cm ){
	  
	  std::map<int,SMGCSet>& time_cells_set_map = main_cluster->get_time_cells_set_map();
	  int num_mcells_outside = 0;
	  int num_time_slices_outside = 0;

	  int num_mcells_def_outside = 0;
	  
	  double prev_pos_x= first_pos_x;
	  double current_pos_x = first_pos_x;
	  for (auto it3 = time_cells_set_map.begin(); it3 != time_cells_set_map.end(); it3++){
	    current_pos_x = (*(it3->second.begin()))->get_sampling_points().front().x;


	      // if (flash->get_flash_id()==35&&abs(main_cluster->get_cluster_id()-5)<=0)
	      // std::cout << num_time_slices_outside<< " " << num_mcells_outside << "  " << (first_pos_x - offset_x)/units::cm << " " <<
	      // 	(prev_pos_x-offset_x)/units::cm << " " << (current_pos_x-offset_x)/units::cm << std::endl;
	    
	    if (current_pos_x -offset_x > low_x_cut + low_x_cut_ext1 && current_pos_x - prev_pos_x > 0.75*units::cm)
	      break;
	    if (num_time_slices_outside > 60) break;
	    
	    if (current_pos_x -offset_x < low_x_cut + low_x_cut_ext1) num_mcells_def_outside += it3->second.size();

	    num_time_slices_outside += 1;
	    num_mcells_outside += it3->second.size();
	    prev_pos_x = current_pos_x;
	  }
	  if (num_time_slices_outside <=36 && num_mcells_outside < 0.05*main_cluster->get_num_mcells()){
	    first_pos_x = current_pos_x;
	    if (num_time_slices_outside > 10 && fabs(current_pos_x - prev_pos_x)<10*units::cm)
	      flag_spec_end = true;
	  }else if (num_time_slices_outside <=60 && num_mcells_outside < 0.06*main_cluster->get_num_mcells() && fabs(current_pos_x - prev_pos_x)>10*units::cm){
	    first_pos_x = current_pos_x;
	  }else if (num_time_slices_outside <=25 && num_mcells_outside < 0.12 * main_cluster->get_num_mcells() && fabs(current_pos_x - prev_pos_x)>20*units::cm){
	    first_pos_x = current_pos_x;
	  }

	  if (num_mcells_def_outside < 0.0015 * main_cluster->get_num_mcells()&&num_mcells_def_outside>0)
	    first_pos_x = offset_x;
	  
	  // if (flash->get_flash_id()==26&&main_cluster->get_cluster_id()==22)
	  //   std::cout << num_mcells_outside << " " << main_cluster->get_num_mcells() << "  A " << (first_pos_x - offset_x)/units::cm << " " <<
	  //     (prev_pos_x-offset_x)/units::cm << " " << (current_pos_x-offset_x)/units::cm << " " << num_mcells_def_outside << std::endl;
	  
	}
	if (last_pos_x - offset_x >= high_x_cut + high_x_cut_ext1 &&
	    last_pos_x - offset_x < high_x_cut + 120*units::cm){
	  std::map<int,SMGCSet>& time_cells_set_map = main_cluster->get_time_cells_set_map();
	  int num_mcells_outside = 0;
	  int num_time_slices_outside = 0;
	  int num_mcells_def_outside = 0;
	  double prev_pos_x= last_pos_x;
	  double current_pos_x = last_pos_x;

	  for (auto it3 = time_cells_set_map.rbegin(); it3 != time_cells_set_map.rend(); it3++){
	    current_pos_x = (*(it3->second.begin()))->get_sampling_points().front().x;
	    if (current_pos_x -offset_x<high_x_cut + high_x_cut_ext1 && fabs(current_pos_x - prev_pos_x) > 0.75*units::cm)
	      break;
	    if (num_time_slices_outside > 60) break;


	    if (current_pos_x -offset_x>high_x_cut + high_x_cut_ext1) num_mcells_def_outside +=it3->second.size();

	    // if (flash->get_flash_id()==18&&main_cluster->get_cluster_id()==23)
	    //   std::cout << current_pos_x/units::cm << " " << it3->second.size() << std::endl;
	    
	    num_time_slices_outside += 1;
	    num_mcells_outside += it3->second.size();
	    prev_pos_x = current_pos_x;
	  }
	  if (num_time_slices_outside <=36 && num_mcells_outside < 0.05*main_cluster->get_num_mcells()){
	    last_pos_x = current_pos_x;
	    if (num_time_slices_outside > 10 && fabs(current_pos_x - prev_pos_x)<10*units::cm)
	      flag_spec_end = true;
	  }else if (num_time_slices_outside <=60 && num_mcells_outside < 0.06*main_cluster->get_num_mcells() && fabs(current_pos_x - prev_pos_x)>10*units::cm){
	    last_pos_x = current_pos_x;
	  }else if (num_time_slices_outside <=25 && num_mcells_outside < 0.12 * main_cluster->get_num_mcells() && fabs(current_pos_x - prev_pos_x)>20*units::cm){
	    last_pos_x = current_pos_x;
	  }
	  
	  if (num_mcells_def_outside < 0.0015 * main_cluster->get_num_mcells()&&num_mcells_def_outside>0)
	    last_pos_x = offset_x+high_x_cut;

	  //if (flash->get_flash_id()==26&&main_cluster->get_cluster_id()==22)
	  //  std::cout << flash->get_flash_id() << " "<< main_cluster->get_cluster_id() << " " << (first_pos_x-offset_x)/units::cm << " " << (last_pos_x-offset_x)/units::cm << " " << num_time_slices_outside << " " << num_mcells_outside << " " << main_cluster->get_num_mcells() << " " << fabs(current_pos_x - prev_pos_x)/units::cm << std::endl;
	  
	}
	
	// if (flash->get_flash_id()==16 && main_cluster->get_cluster_id()==13 )
	//   std::cout << flash->get_flash_id() << " "<< main_cluster->get_cluster_id() << " " << (first_pos_x-offset_x)/units::cm << " " << (last_pos_x-offset_x)/units::cm << std::endl;

	//	if (flash->get_flash_id()==26)
	//  std::cout << flash->get_flash_id() << " " << main_cluster->get_cluster_id() << " " << offset_x/units::cm << " " << (first_pos_x-offset_x)/units::cm << " " << (last_pos_x-offset_x)/units::cm << " " << std::endl;
	
  	if (first_pos_x-offset_x > low_x_cut + low_x_cut_ext1 -1.0*units::cm &&
  	    last_pos_x-offset_x > low_x_cut &&
  	    last_pos_x-offset_x < high_x_cut + high_x_cut_ext1 &&
  	    first_pos_x-offset_x < high_x_cut){

	  
	  
	  bundle->set_spec_end_flag(flag_spec_end);
	  
  	  if (first_pos_x-offset_x <=low_x_cut + low_x_cut_ext2 && first_pos_x-offset_x > low_x_cut + low_x_cut_ext1 - 1.0*units::cm ){
  	    // bundle->set_flag_close_to_PMT(true);
  	  //   bundle->set_flag_at_x_boundary(true);
  	  // }else if (first_pos_x-offset_x <= low_x_cut + low_x_cut_ext1 && first_pos_x-offset_x > low_x_cut + low_x_cut_ext1 -1.0*units::cm){
	    bundle->set_flag_close_to_PMT(true);
  	    bundle->set_flag_at_x_boundary(true);
	  }
  	  if (last_pos_x-offset_x >= high_x_cut + high_x_cut_ext2 && last_pos_x-offset_x < high_x_cut + high_x_cut_ext1){
  	    bundle->set_flag_at_x_boundary(true);
  	  }

  	  PR3DClusterSelection temp_clusters;
  	  temp_clusters.push_back(main_cluster);
  	  for (auto it3 = additional_clusters->begin(); it3!=additional_clusters->end(); it3++){
  	    temp_clusters.push_back(it3->first);
  	    other_clusters->push_back(it3->first);
  	  }

  	  for (auto it3 = temp_clusters.begin(); it3!=temp_clusters.end(); it3++){
  	    SMGCSelection& mcells = (*it3)->get_mcells();
  	    bool flag_save = true;

	    if ((*it3) == main_cluster)  flag_save = false;
	    
  	    for (auto it4 = mcells.begin(); it4!=mcells.end(); it4++){
  	      SlimMergeGeomCell *mcell = (*it4);
  	      if (mcell->get_q()>0){
  		PointVector& pts = mcell->get_sampling_points();
  		if (pts.at(0).x-offset_x < low_x_cut+low_x_cut_ext1 ||
  		    pts.at(0).x-offset_x > high_x_cut+high_x_cut_ext1){
  		  flag_save = false;
  		  continue;
  		}
		
  		float charge = mcell->get_q()/pts.size();
  		Point p;
  		for (size_t i=0;i!=pts.size();i++){
  		  p.x = pts.at(i).x - offset_x;
  		  p.y = pts.at(i).y;
  		  p.z = pts.at(i).z;

		  //correct for finite electron lifetime
		  float elifetime_ratio = mp.get_attenuation_ratio(p.x/time_slice_width * nrebin * 0.5*units::microsecond / units::millisecond);
		  //std::cout << p.x << " " << time_slice_width << " " << p.x/time_slice_width * nrebin * 0.5*units::microsecond / units::millisecond << " " << elifetime_ratio << std::endl;
		  
		  int voxel_id = WCP2dToy::convert_xyz_voxel_id(p);
		  std::list<std::pair<int,float>>& pmt_list = photon_library->at(voxel_id);
		  
		  for (auto it5 = pmt_list.begin(); it5!=pmt_list.end(); it5++){
		    pred_pmt_light->at((*map_lib_pmt)[it5->first]) += charge * it5->second / elifetime_ratio;
		  }
		}
  	      }
  	    }
  	    if (flag_save)
  	      more_clusters->push_back(*it3);
	    
  	  } // loop over all clusters within this TPC object...

	  // veto PMT 18 ([17])
	  double norm_factor[32];
	  for (int i=0;i!=32;i++){
	    norm_factor[i] = 1;
	  }
	  if (flag_data){
	    if ((run_no >= 12809 && (!flag_timestamp)) || (flag_timestamp && eventTime >= 1505170407))
	      norm_factor[17] = 0;
	  }

	  //std::cout << "kaka1 " << run_no << " " << eventTime << " " << flag_timestamp << " " << eventTime - 1505170407 << " " << norm_factor[17] << std::endl;
	  
  	  double sum1 = 0, sum2 = 0, max_pe = 0;
  	  for (size_t i=0;i!=32;i++){
  	    pred_pmt_light->at(i) *= scaling_light_mag * norm_factor[i];

  	    sum1 += flash->get_PE(i);
  	    sum2 += pred_pmt_light->at(i);
  	    if (pred_pmt_light->at(i) > max_pe)
  	      max_pe = pred_pmt_light->at(i);
  	  }
	  
	  // if (sum2 < sum1 * 3){ // three times allowrance ... 
	  flag_good_bundle = true;
	  // }
  	}
}

FlashTPCBundleSelection WCP2dToy::tpc_light_match(double eventTime, int time_offset, int nrebin, WCP::Photon_Library *pl, WCP::map_cluster_cluster_vec& group_clusters, WCP::OpflashSelection& flashes, Int_t run_no,  bool flag_data, bool flag_add_light_yield_err, bool flag_timestamp){

  double rel_light_yield_err = pl->rel_light_yield_err;
  double scaling_light_mag = pl->scaling_light_mag;
  std::map<int,int> *map_lib_pmt = &pl->map_lib_pmt;
  std::map<int,int> *map_pmt_lib = &pl->map_pmt_lib;
  std::vector<std::list<std::pair<int,float>>> *photon_library = &pl->library;

  // PMT data is based on OpChannel
  // Library is NOT ...
  // these are PE numbers ... 
  // Double_t cos_pe_low[32]={11,11,11,11,10,
  // 			   7,8,8,10,7,
  // 			   11,11,11,11,10,
  // 			   9,11,11,7,9,
  // 			   11,10,11,11,11,
  // 			   11,10,11,11,9,
  // 			   10,11};
  // Double_t cos_pe_mid[32]={34,32,28,35,22,
  // 			   23,22,24,33,30,
  // 			   35,35,33,36,33,
  // 			   33,36,33,19,27,
  // 			   32,23,42,32,33,
  // 			   34,24,33,35,25,
  // 			   32,34};

  Double_t cos_pe_low[32]={11,11,11,11,10,
			   7,8,8,10,7,
			   11,11,11,11,10,
			   9,11,11,7,9,
			   11,10,11,11,11,
			   11,11,10,11,11,
			   9,10};
  Double_t cos_pe_mid[32]={34,32,28,35,22,
			   23,22,24,33,30,
			   35,35,33,36,33,
			   33,36,33,19,27,
			   32,23,42,32,33,
			   34,34,24,33,35,
			   25,32};

  TPCParams& mp = Singleton<TPCParams>::Instance();
  double time_slice_width = mp.get_ts_width();

  int solv_type = 1; // new matching code ... 
  
  if (solv_type==1){
    FlashTPCBundleSet all_bundles;
    Flash_bundles_map flash_bundles_map;
    Cluster_bundles_map cluster_bundles_map;
    std::map<std::pair<Opflash*,PR3DCluster*>,FlashTPCBundle*> fc_bundles_map;
      
    int flash_index_id = 0;
    for (auto it1 = flashes.begin(); it1!=flashes.end(); it1++){
      Opflash *flash = (*it1);
      double offset_x = (flash->get_time() - time_offset)*2./nrebin*time_slice_width;
      int cluster_index_id = 0;
      for (auto it2 = group_clusters.begin(); it2!=group_clusters.end(); it2++){
  	PR3DCluster* main_cluster = it2->first;
  	std::vector<std::pair<WCP::PR3DCluster*,double>>& additional_clusters = it2->second;
  	FlashTPCBundle *bundle = new FlashTPCBundle(flash, main_cluster, flash_index_id, cluster_index_id);
  	bool flag_good_bundle = false;
	
  	std::vector<double>& pred_pmt_light = bundle->get_pred_pmt_light();
  	PR3DClusterSelection& other_clusters = bundle->get_other_clusters();
  	PR3DClusterSelection& more_clusters = bundle->get_more_clusters();

	//Call to calculate predicted pe for a given bundle
        calculate_pred_pe(run_no, eventTime, time_offset, nrebin, time_slice_width, pl, bundle, &pred_pmt_light, &additional_clusters, &other_clusters, &more_clusters, flag_good_bundle, flag_data, flag_timestamp);

  	if (flag_good_bundle){
  	  all_bundles.insert(bundle);
  	  if (flash_bundles_map.find(flash)==flash_bundles_map.end()){
  	    FlashTPCBundleSelection bundles;
  	    bundles.push_back(bundle);
  	    flash_bundles_map[flash] = bundles;
  	  }else{
  	    flash_bundles_map[flash].push_back(bundle);
  	  }
  	  if (cluster_bundles_map.find(main_cluster)==cluster_bundles_map.end()){
  	    FlashTPCBundleSelection bundles;
  	    bundles.push_back(bundle);
  	    cluster_bundles_map[main_cluster] = bundles;
  	  }else{
  	    cluster_bundles_map[main_cluster].push_back(bundle);
  	  }
  	  fc_bundles_map[std::make_pair(flash,main_cluster)] = bundle;

	  // if (flash->get_flash_id()==14)
	  //   std::cout << flash->get_flash_id() << " " << main_cluster->get_cluster_id() << " " << bundle << std::endl;
	  
  	}else{
  	  delete bundle;
  	}
	cluster_index_id++;
      }
      flash_index_id++;
    }

    
    // examine the bundles ... 
    //std::cout << "Starting: " << cluster_bundles_map.size() << " A " << flash_bundles_map.size() << " " << all_bundles.size() << std::endl;
    {
      FlashTPCBundleSelection to_be_removed;
      for (auto it = all_bundles.begin(); it!=all_bundles.end();it++){
	FlashTPCBundle *bundle = *it;
	if (!bundle->examine_bundle(cos_pe_low, cos_pe_mid)){
	  to_be_removed.push_back(bundle);
	}
      }
      
      for (auto it1 = to_be_removed.begin(); it1!=to_be_removed.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	all_bundles.erase(bundle);
	
	Opflash *flash = bundle->get_flash();
	PR3DCluster *cluster = bundle->get_main_cluster();
	
	fc_bundles_map.erase(std::make_pair(flash,cluster));
	
	{
	  FlashTPCBundleSelection& temp_bundles = flash_bundles_map[flash];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    flash_bundles_map.erase(flash);
	}
	{
	  FlashTPCBundleSelection& temp_bundles = cluster_bundles_map[cluster];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    cluster_bundles_map.erase(cluster);
	}
	
	delete bundle;
      }
      to_be_removed.clear();


      for (auto it = cluster_bundles_map.begin(); it!= cluster_bundles_map.end(); it++){
       	PR3DCluster *main_cluster = it->first;
       	FlashTPCBundleSelection& bundles = it->second;
       	bool flag_consist = false;
	FlashTPCBundleSelection temp_removed;
	
       	for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if (bundle->get_potential_bad_match_flag())
	    temp_removed.push_back(bundle);
	  if (bundle->get_consistent_flag() ||
	       (bundle->get_ks_dis() < 0.12 || bundle->get_chi2() < 3 * bundle->get_ndf() )&& bundle->get_ndf()>=3 ||
	       bundle->get_ks_dis()<0.33 && bundle->get_chi2() < 50 * bundle->get_ndf()&&bundle->get_ndf()>=5 && bundle->get_flag_close_to_PMT())
	    flag_consist = true;

	  // if (main_cluster->get_cluster_id()==17)
	  //   std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << " " << bundle->get_consistent_flag() << " " << bundle->get_flag_close_to_PMT() << " " << bundle->get_potential_bad_match_flag() << " " << flag_consist << std::endl;
	  
	}
	if (flag_consist)
	  to_be_removed.insert(to_be_removed.end(),temp_removed.begin(),temp_removed.end());
      }

      
      for (auto it1 = to_be_removed.begin(); it1!=to_be_removed.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	all_bundles.erase(bundle);
	
	Opflash *flash = bundle->get_flash();
	PR3DCluster *cluster = bundle->get_main_cluster();
	
	fc_bundles_map.erase(std::make_pair(flash,cluster));
	
	{
	  FlashTPCBundleSelection& temp_bundles = flash_bundles_map[flash];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    flash_bundles_map.erase(flash);
	}
	{
	  FlashTPCBundleSelection& temp_bundles = cluster_bundles_map[cluster];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    cluster_bundles_map.erase(cluster);
	}
	
	delete bundle;
      }
      
      
    }

    

    
    //    std::cout << "After Cleaning 1 : " << cluster_bundles_map.size() << " A " << flash_bundles_map.size() << " " << all_bundles.size() << std::endl;
    
    for (auto it = cluster_bundles_map.begin(); it!= cluster_bundles_map.end(); it++){
      PR3DCluster *main_cluster = it->first;
      FlashTPCBundleSelection& bundles = it->second;
      bool flag_tight_bundle = false;

      bool flag_highly_consistent_bundle = false;

      
      for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	FlashTPCBundle *bundle = *it1;

	//	std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << " " << bundle->get_consistent_flag() << " " << bundle->get_flag_close_to_PMT() << " " << bundle->get_potential_bad_match_flag() << std::endl;
	
	if (bundle->get_consistent_flag()){
	  flag_tight_bundle = true;
	  
	  if (bundle->get_ks_dis()<0.05 && bundle->get_ndf() >= 10 && bundle->get_chi2() < bundle->get_ndf()  * 12  ||
	      bundle->get_ks_dis()<0.07 && bundle->get_ndf() >= 10 && bundle->get_chi2() < bundle->get_ndf()  * 9  ||
	      bundle->get_ks_dis()<0.1 && bundle->get_ndf() >=5 && bundle->get_chi2() < bundle->get_ndf()  * 4  ||
	      bundle->get_ks_dis()<0.15 && bundle->get_ndf()>=5 && bundle->get_chi2() < bundle->get_ndf()  * 3 	      )
	    flag_highly_consistent_bundle = true;
	  //  break;
	}
      }
      
      if (!flag_tight_bundle){
	for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if (bundle->get_ks_dis()<0.07 && bundle->get_ndf()>=10 && bundle->get_chi2() < bundle->get_ndf() * 60){
	    bundle->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (bundle->get_ks_dis()<0.33 && bundle->get_ndf()>=3 && bundle->get_chi2() < bundle->get_ndf() * 10){
	    bundle->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (bundle->get_ks_dis()<0.33 && bundle->get_ndf()>=3 && (bundle->get_chi2() < bundle->get_ndf() * 50 && bundle->get_flag_close_to_PMT() || bundle->get_chi2() < bundle->get_ndf() * 16 && bundle->get_flag_at_x_boundary())){
	    bundle->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (bundle->get_ks_dis()<0.22 && bundle->get_ndf()>=3 && bundle->get_chi2() < bundle->get_ndf() * 16){
	    bundle->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (bundle->get_ks_dis()<0.16 && bundle->get_ndf()>=6 && bundle->get_chi2() < bundle->get_ndf() * 20){
	    bundle->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }
	}

	
	FlashTPCBundle *min_bundle = *bundles.begin();
	
	for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if (bundle->get_ks_dis()<min_bundle->get_ks_dis()){
	    min_bundle = bundle;
	  }
	}
	
	FlashTPCBundle *min_bundle1 = 0;
	for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if (bundle==min_bundle) continue;
	  if (min_bundle1==0) {
	    min_bundle1 = bundle;
	  }else if (bundle->get_ks_dis()<min_bundle1->get_ks_dis()){
	    min_bundle1 = bundle;
	  }
	}
	
	
	// if (main_cluster->get_cluster_id()==12)
	//   std::cout << "Xin: " << min_bundle->get_flash()->get_flash_id() << " " << min_bundle1->get_flash()->get_flash_id() << std::endl;
	

	bool flag_set = false;
	if (min_bundle->get_ks_dis()<0.15 && min_bundle->get_ndf()>=6 && min_bundle->get_chi2() < min_bundle->get_ndf() * 40){
	  min_bundle->set_consistent_flag(true);
	  flag_tight_bundle = true;
	  flag_set = true;
	}else if ( min_bundle->get_ks_dis()<0.11 && min_bundle->get_ndf() >= 3 && min_bundle->get_chi2() < min_bundle->get_ndf() * 36){
	  min_bundle->set_consistent_flag(true);
	  flag_tight_bundle = true;
	  flag_set = true;
	}else if (min_bundle->get_ks_dis()<0.075 && min_bundle->get_ndf()>=10 && min_bundle->get_chi2() < min_bundle->get_ndf() * 60){
	  min_bundle->set_consistent_flag(true);
	  flag_tight_bundle = true;
	  flag_set = true;
	}else if (min_bundle->get_ks_dis()<0.11 && min_bundle->get_ndf()>=10 && min_bundle->get_chi2() < min_bundle->get_ndf() * 120){
	  min_bundle->set_consistent_flag(true);
	  flag_tight_bundle = true;
	  flag_set = true;
	}else if (min_bundle->get_ks_dis()<0.17 && min_bundle->get_ndf()>=10 && min_bundle->get_chi2() < min_bundle->get_ndf() * 40){
	  min_bundle->set_consistent_flag(true);
	  flag_tight_bundle = true;
	  flag_set = true;
	}else if (min_bundle->get_ks_dis()<0.22 && min_bundle->get_ndf()>=20 && min_bundle->get_chi2() < min_bundle->get_ndf() * 40){
	  min_bundle->set_consistent_flag(true);
	  flag_tight_bundle = true;
	  flag_set = true;
	}else if (min_bundle->get_flag_at_x_boundary() && min_bundle->get_ks_dis()<0.3 && min_bundle->get_ndf()>=10 && min_bundle->get_chi2() < min_bundle->get_ndf() * 30){
	  min_bundle->set_consistent_flag(true);
	  flag_tight_bundle = true;
	  flag_set = true;
	}
	
	if (!flag_set && min_bundle1!=0){
	  if (min_bundle1->get_ks_dis()<0.15 && min_bundle1->get_ndf()>=6 && min_bundle1->get_chi2() < min_bundle1->get_ndf() * 40){
	    min_bundle1->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if ( min_bundle1->get_ks_dis()<0.11 && min_bundle1->get_ndf() >= 3 && min_bundle1->get_chi2() < min_bundle1->get_ndf() * 36){
	    min_bundle1->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (min_bundle1->get_ks_dis()<0.075 && min_bundle1->get_ndf()>=10 && min_bundle1->get_chi2() < min_bundle1->get_ndf() * 60){
	    min_bundle1->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (min_bundle1->get_ks_dis()<0.11 && min_bundle1->get_ndf()>=10 && min_bundle1->get_chi2() < min_bundle1->get_ndf() * 120){
	    min_bundle1->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (min_bundle1->get_ks_dis()<0.17 && min_bundle1->get_ndf()>=10 && min_bundle1->get_chi2() < min_bundle1->get_ndf() * 40){
	    min_bundle1->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (min_bundle1->get_ks_dis()<0.22 && min_bundle1->get_ndf()>=20 && min_bundle1->get_chi2() < min_bundle1->get_ndf() * 40){
	    min_bundle1->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (min_bundle1->get_flag_at_x_boundary() && min_bundle1->get_ks_dis()<0.3 && min_bundle1->get_ndf()>=10 && min_bundle1->get_chi2() < min_bundle1->get_ndf() * 30){
	    min_bundle1->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }
	  
	  //else if (min_bundle1->get_ks_dis()<0.17 && min_bundle1->get_ndf()>=10 && min_bundle1->get_chi2() < min_bundle1->get_ndf() * 40){
	  // min_bundle1->set_consistent_flag(true);
	  // flag_tight_bundle = true;
	  //}
	}
      }

      
       if (!flag_tight_bundle){
	// last pieces ... 
	for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if (bundle->get_flag_close_to_PMT() && bundle->get_ks_dis()<0.7 && bundle->get_ndf()>5 && bundle->get_chi2() < bundle->get_ndf() * 40){
	    bundle->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }
	}
      }
     


      if (flag_tight_bundle){
	if (flag_highly_consistent_bundle){
	  for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	    FlashTPCBundle *bundle = *it1;
	    if (bundle->get_consistent_flag()){
	      if (bundle->get_ks_dis()<0.15 && bundle->get_ndf() >=5 && bundle->get_chi2() < bundle->get_ndf()  * 9 ){
	      }else if (bundle->get_ks_dis()<0.075 && bundle->get_ndf() >=8 && bundle->get_chi2() < bundle->get_ndf()  * 12){
	      }else if (bundle->get_ks_dis()<0.07 && bundle->get_ndf() >= 16 && bundle->get_chi2() < bundle->get_ndf()  * 20){
	      }else{
		bundle->set_consistent_flag(false);
	      }
	      
	      
	    }
	  }

	 
	  
	  
	  
	}

	FlashTPCBundleSelection ndf1_bundles;
	FlashTPCBundleSelection other_bundles;
	for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if (bundle->get_consistent_flag()){
	    if (bundle->get_ndf()==1){
	      ndf1_bundles.push_back(bundle);
	      	}else{
	      other_bundles.push_back(bundle);
	    }
	  }
	}

	// now remove ... 
	bool flag_temp = false;
	// if (main_cluster->get_cluster_id()==15)
	//   std::cout << "Xin: " << other_bundles.size() << " " << ndf1_bundles.size() << std::endl;
	
	for (auto it1 = other_bundles.begin(); it1!=other_bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if (flash_bundles_map.find(bundle->get_flash())!=flash_bundles_map.end()){
	    FlashTPCBundleSelection bundles1 = flash_bundles_map[bundle->get_flash()];
	    int temp_num = 0;
	    for (auto it2 = bundles1.begin(); it2!=bundles1.end(); it2++){
	      if ((*it2)->get_consistent_flag())
		temp_num++;
	    }
	    // if (bundle->get_main_cluster()->get_cluster_id()==15)
	    //   std::cout << bundle->get_flash()->get_flash_id() << " " << temp_num << std::endl;
	    
	    if (temp_num==1){
	      flag_temp = true;
	      break;
	    }
	    
	  }
	}
	
	if (flag_temp){
	  for (auto it1 = ndf1_bundles.begin(); it1!=ndf1_bundles.end(); it1++){
	    FlashTPCBundle *bundle = *it1;
	    bundle->set_consistent_flag(false);
	  }
	}
	
	
      }
    }

    //
    for (auto it = cluster_bundles_map.begin(); it!= cluster_bundles_map.end(); it++){
      PR3DCluster *main_cluster = it->first;
      FlashTPCBundleSelection& bundles = it->second;

      FlashTPCBundleSelection temp_bundles;
      bool flag_remove = false;

      for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	if (bundle->get_consistent_flag() && bundle->get_spec_end_flag()){
	  temp_bundles.push_back(bundle);
	}else if (bundle->get_consistent_flag()){
	  flag_remove = true;
	}
      }
      
      if (flag_remove){
	for (auto it1 = temp_bundles.begin(); it1!=temp_bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  bundle->set_consistent_flag(false);
	}
      }

      // examine the again
      flag_remove = false;
      temp_bundles.clear();
      for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	if (bundle->get_consistent_flag()){
	  if (bundle->get_ks_dis() < 0.06 && bundle->get_chi2() < 3.*bundle->get_ndf() && bundle->get_flag_at_x_boundary()){
	    flag_remove = true;
	  }else{
	    temp_bundles.push_back(bundle);
	  }
	}
      }
      if (flag_remove){
	for (auto it1 = temp_bundles.begin(); it1!=temp_bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  bundle->set_consistent_flag(false);
	}
      }
      
    }
    
    //  std::cout << "After Cleaning 1 : " << cluster_bundles_map.size() << " A " << flash_bundles_map.size() << " " << all_bundles.size() << std::endl;


    // // std::cout << std::endl << std::endl;
    // for (auto it = all_bundles.begin(); it!=all_bundles.end();it++){
    //   FlashTPCBundle *bundle = *it;
      
    //   //  if (bundle->get_flash()->get_flash_id()==29 && bundle->get_main_cluster()->get_cluster_id()==19)
    //   //	bundle->set_consistent_flag(1);
    //   //   if ( bundle->get_consistent_flag() || bundle->get_flag_at_x_boundary())
    //   std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << " " << bundle->get_consistent_flag()  << std::endl;

      
    // }
    // std::cout << std::endl << std::endl;

    

    // further examine the map ...
    {
      Flash_bundles_map flash_good_bundles_map;
      Cluster_bundles_map cluster_good_bundles_map;
      FlashTPCBundleSelection good_bundles;
            
      Flash_bundles_map flash_other_bundles_map;
      Cluster_bundles_map cluster_other_bundles_map;
      
      for (auto it = all_bundles.begin(); it!=all_bundles.end();it++){
	FlashTPCBundle *bundle = *it;
	Opflash *flash = bundle->get_flash();
	PR3DCluster *cluster = bundle->get_main_cluster();

	if (bundle->get_consistent_flag()){
	  if (flash_good_bundles_map.find(flash)==flash_good_bundles_map.end()){
	    FlashTPCBundleSelection bundles;
	    bundles.push_back(bundle);
	    flash_good_bundles_map[flash] = bundles;
	  }else{
	    flash_good_bundles_map[flash].push_back(bundle);
	  }
	  if (cluster_good_bundles_map.find(cluster)==cluster_good_bundles_map.end()){
	    FlashTPCBundleSelection bundles;
	    bundles.push_back(bundle);
	    cluster_good_bundles_map[cluster] = bundles;
	  }else{
	    cluster_good_bundles_map[cluster].push_back(bundle);
	  }
	  good_bundles.push_back(bundle);
	}else if (bundle->get_flag_at_x_boundary()){
	  if (flash_other_bundles_map.find(flash)==flash_other_bundles_map.end()){
	    FlashTPCBundleSelection bundles;
	    bundles.push_back(bundle);
	    flash_other_bundles_map[flash] = bundles;
	  }else{
	    flash_other_bundles_map[flash].push_back(bundle);
	  }
	  if (cluster_other_bundles_map.find(cluster)==cluster_other_bundles_map.end()){
	    FlashTPCBundleSelection bundles;
	    bundles.push_back(bundle);
	    cluster_other_bundles_map[cluster] = bundles;
	    }else{
	    cluster_other_bundles_map[cluster].push_back(bundle);
	  }
	}
	// if ( bundle->get_consistent_flag() || bundle->get_flag_at_x_boundary())
	// 	std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << std::endl;
      }

      for (auto it = cluster_good_bundles_map.begin(); it!= cluster_good_bundles_map.end(); it++){
	PR3DCluster *cluster =  it->first;
	FlashTPCBundleSelection& bundles = it->second;

	
	if (bundles.size()>1){ // more than one flash

	  // find the min bundle ... 
	  FlashTPCBundle *min_bundle = bundles.at(0);
	  for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	    FlashTPCBundle *bundle = *it1;
	    if (bundle->get_ks_dis()+0.003 * flash_good_bundles_map[bundle->get_flash()].size() < min_bundle->get_ks_dis() + 0.003 * flash_good_bundles_map[min_bundle->get_flash()].size())
	      min_bundle = bundle;
	  }

	  if (min_bundle->get_ks_dis() < 0.18){
	    //std::cout << min_bundle->get_flash()->get_flash_id() << " Xin: " << min_bundle->get_main_cluster()->get_cluster_id() << std::endl;

	    FlashTPCBundleSelection temp_bundles;
	    
	    //examine the rest of bundles;
	    for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	      FlashTPCBundle *bundle = *it1;
	      if (bundle==min_bundle) continue;
	      bool flag_remove = false;
	      
	      if (min_bundle->get_ks_dis() +0.015 < bundle->get_ks_dis() &&
		  min_bundle->get_chi2()/min_bundle->get_ndf() *1.1 < bundle->get_chi2()/bundle->get_ndf()){
		// prepare remove bundle from the list ...
		flag_remove = true;
	      }else if(min_bundle->get_ks_dis() < bundle->get_ks_dis() &&
		       min_bundle->get_chi2()/min_bundle->get_ndf() * 3 < bundle->get_chi2()/bundle->get_ndf()){
		flag_remove = true;
	      }else if (min_bundle->get_ks_dis() +0.025 < bundle->get_ks_dis() &&
			min_bundle->get_chi2()/min_bundle->get_ndf() *0.85 < bundle->get_chi2()/bundle->get_ndf()){
		flag_remove = true;
		if (bundle->get_ks_dis() < 0.075&&bundle->get_chi2()/bundle->get_ndf()<1.6)
		  flag_remove = false;
	      }else if (min_bundle->get_ks_dis() +0.03 < bundle->get_ks_dis() &&
			min_bundle->get_chi2()/min_bundle->get_ndf() *0.75 < bundle->get_chi2()/bundle->get_ndf() &&
			min_bundle->get_ks_dis() <0.06){
		flag_remove = true;
	      }else if (min_bundle->get_ks_dis()+0.075 < bundle->get_ks_dis() &&
			min_bundle->get_ks_dis()<0.075 &&
			min_bundle->get_chi2()/min_bundle->get_ndf() /15. < bundle->get_chi2()/bundle->get_ndf()){
		flag_remove = true;
	      }else if (flash_good_bundles_map[min_bundle->get_flash()].size()==1){
		if (min_bundle->get_ks_dis()<0.06){
		  if (min_bundle->get_chi2()/min_bundle->get_ndf() < bundle->get_chi2()/bundle->get_ndf() * 2.2){
		    flag_remove = true;
		  }else if (min_bundle->get_ks_dis() + 0.06 < bundle->get_ks_dis() &&
			    min_bundle->get_chi2()/min_bundle->get_ndf() < bundle->get_chi2()/bundle->get_ndf() * 3.0){
		    flag_remove = true;
		  }else if (min_bundle->get_chi2()/min_bundle->get_ndf()<9 && min_bundle->get_ks_dis() + 0.01 < bundle->get_ks_dis()){
		    flag_remove = true;
		  }else if (min_bundle->get_ks_dis()<0.05 && min_bundle->get_chi2()/min_bundle->get_ndf()<38 && min_bundle->get_ks_dis() + 0.05 < bundle->get_ks_dis()){
		    flag_remove = true;
		  }
		}else{
		  if (min_bundle->get_ks_dis() + 0.015 < bundle->get_ks_dis() &&
		      min_bundle->get_chi2()/min_bundle->get_ndf()*0.75 < bundle->get_chi2()/bundle->get_ndf() &&flash_good_bundles_map[bundle->get_flash()].size()>1
		      ){
		    flag_remove = true;
		  }else if (min_bundle->get_ks_dis() + 0.08 < bundle->get_ks_dis() &&
			    flash_good_bundles_map[bundle->get_flash()].size()>1 &&
			    min_bundle->get_chi2()/min_bundle->get_ndf() < 25
			    ){
		    flag_remove = true;
		  }
		}
	      }
	      
	      // if (min_bundle->get_main_cluster()->get_cluster_id()==23){
	      // 	std::cout << min_bundle->get_flash()->get_flash_id() << " A " << min_bundle->get_main_cluster()->get_cluster_id() << " " << flag_remove << " " << flash_good_bundles_map[min_bundle->get_flash()].size()<< std::endl;
	      // 	//flag_remove = true;
	      // }
	      
	      if (flag_remove){
		if (flash_good_bundles_map[bundle->get_flash()].size()>1){
		  
		  FlashTPCBundle *min_bundle1 = flash_good_bundles_map[bundle->get_flash()].at(0);
		  FlashTPCBundle *min_bundle2 = flash_good_bundles_map[bundle->get_flash()].at(0);
		  for (auto it2 = flash_good_bundles_map[bundle->get_flash()].begin(); it2!=flash_good_bundles_map[bundle->get_flash()].end(); it2++){
		    FlashTPCBundle *bundle1 = *it2;
		    if (bundle1->get_ks_dis() < min_bundle1->get_ks_dis()){
		      min_bundle1 = bundle1;
		      min_bundle2 = min_bundle1;
		    }
		  }
		  if (bundle!=min_bundle1){
		    temp_bundles.push_back(bundle);
		  }else{
		    if (min_bundle1->get_ks_dis() + 0.01 > min_bundle2->get_ks_dis() &&
			min_bundle1->get_chi2() > min_bundle2->get_chi2()*0.85){
		      temp_bundles.push_back(bundle);
		    }else if (min_bundle1->get_chi2()/min_bundle1->get_ndf() > min_bundle2->get_chi2()/min_bundle2->get_ndf() * 3 &&
			      min_bundle1->get_ndf()>=5 && 
			      min_bundle2->get_ks_dis() < 0.12){
		      temp_bundles.push_back(bundle);
		    }
		  }
		}
	      }
	    }
	  

	    for (auto it1 = temp_bundles.begin(); it1!=temp_bundles.end(); it1++){
	      FlashTPCBundle *bundle = *it1;
	      PR3DCluster *cluster =  bundle->get_main_cluster();
	      Opflash *flash = bundle->get_flash();
	      
	      bundle->set_consistent_flag(false);
	      flash_good_bundles_map[flash].erase(find(flash_good_bundles_map[flash].begin(),flash_good_bundles_map[flash].end(),bundle));
	      cluster_good_bundles_map[cluster].erase(find(cluster_good_bundles_map[cluster].begin(),cluster_good_bundles_map[cluster].end(),bundle));
	      // remove from 
	    }
	  }
	}
      }


      //   std::cout << "After Cleaning 1 : " << cluster_bundles_map.size() << " A " << flash_bundles_map.size() << " " << all_bundles.size() << std::endl;


    // // std::cout << std::endl << std::endl;
    // for (auto it = all_bundles.begin(); it!=all_bundles.end();it++){
    //   FlashTPCBundle *bundle = *it;
      
    //   //  if (bundle->get_flash()->get_flash_id()==29 && bundle->get_main_cluster()->get_cluster_id()==19)
    //   //	bundle->set_consistent_flag(1);
    //   //   if ( bundle->get_consistent_flag() || bundle->get_flag_at_x_boundary())
    //   std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << " " << bundle->get_consistent_flag()  << std::endl;

      
    // }
    // std::cout << std::endl << std::endl;
      

      // new round according to chi2 .. .
      for (auto it = cluster_good_bundles_map.begin(); it!= cluster_good_bundles_map.end(); it++){
	PR3DCluster *cluster =  it->first;
	FlashTPCBundleSelection& bundles = it->second;

	
	if (bundles.size()>1){ // more than one flash
	  // find the min bundle ... 
	  FlashTPCBundle *min_bundle = bundles.at(0);
	  for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	    FlashTPCBundle *bundle = *it1;
	    if (bundle->get_chi2()/bundle->get_ndf() < min_bundle->get_chi2()/min_bundle->get_ndf())
	      min_bundle = bundle;
	  }

	  if (min_bundle->get_ks_dis() < 0.15){
	    FlashTPCBundleSelection temp_bundles;
	    
	    //examine the rest of bundles;
	    for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	      FlashTPCBundle *bundle = *it1;
	      if (bundle==min_bundle) continue;
	      bool flag_remove = false;
	      
	      if (flash_good_bundles_map[min_bundle->get_flash()].size()==1){
		if (min_bundle->get_ks_dis()<0.08){
		  if (min_bundle->get_chi2()/min_bundle->get_ndf() *1.1 < bundle->get_chi2()/bundle->get_ndf() ){
		    flag_remove = true;
		  }
		}else if (min_bundle->get_ks_dis()<0.1 && min_bundle->get_chi2()/min_bundle->get_ndf() *1.33< bundle->get_chi2()/bundle->get_ndf()){
		  flag_remove = true;
		}else if (min_bundle->get_ks_dis()<0.15 && min_bundle->get_chi2()/min_bundle->get_ndf() * 3 < bundle->get_chi2()/bundle->get_ndf()){
		  flag_remove = true;
		}
	      }else if (flash_good_bundles_map[min_bundle->get_flash()].size()>=2){
		if (min_bundle->get_ks_dis()<0.1 && min_bundle->get_chi2()/min_bundle->get_ndf() * 6 < bundle->get_chi2()/bundle->get_ndf()){
		  flag_remove = true;
		}
	      }

	      // if (min_bundle->get_main_cluster()->get_cluster_id()==2){
	      // 	std::cout << min_bundle->get_flash()->get_flash_id() << " A " << min_bundle->get_main_cluster()->get_cluster_id() << " " << flag_remove << " " << flash_good_bundles_map[min_bundle->get_flash()].size()<< " " << flash_good_bundles_map[bundle->get_flash()].size() << " " << bundle->get_flash()->get_flash_id() << std::endl;
	      // }
	      
	      
	      if (flag_remove){
		if (flash_good_bundles_map[bundle->get_flash()].size()>flash_good_bundles_map[min_bundle->get_flash()].size() && flash_good_bundles_map[min_bundle->get_flash()].size()<=2){
		  if (bundle!=min_bundle){
		    temp_bundles.push_back(bundle);
		  }
		}else{
		  if (bundle!=min_bundle){
		    if (min_bundle->get_ks_dis() < 0.1){
		      if (min_bundle->get_ks_dis() * pow(min_bundle->get_chi2()/min_bundle->get_ndf(),0.8) < 2.0 * bundle->get_ks_dis() * pow(bundle->get_chi2()/bundle->get_ndf(),0.8) && flash_good_bundles_map[bundle->get_flash()].size() > 1){
		  	temp_bundles.push_back(bundle);
		      }
		    }// else{
		    //   if (min_bundle->get_ks_dis() * pow(min_bundle->get_chi2()/min_bundle->get_ndf(),0.8) < 3.33 * bundle->get_ks_dis() * pow(bundle->get_chi2()/bundle->get_ndf(),0.8)){
		    // 	temp_bundles.push_back(bundle);
		    //   }
		    // }
		  }
		  // if (min_bundle->get_main_cluster()->get_cluster_id()==2)
		  //   std::cout << min_bundle->get_ks_dis() * pow(min_bundle->get_chi2()/min_bundle->get_ndf(),0.8) << " " << bundle->get_ks_dis() * pow(bundle->get_chi2()/bundle->get_ndf(),0.8) << std::endl;
		}
	      }
	    }
	  

	    for (auto it1 = temp_bundles.begin(); it1!=temp_bundles.end(); it1++){
	      FlashTPCBundle *bundle = *it1;
	      PR3DCluster *cluster =  bundle->get_main_cluster();
	      Opflash *flash = bundle->get_flash();
	      
	      bundle->set_consistent_flag(false);
	      flash_good_bundles_map[flash].erase(find(flash_good_bundles_map[flash].begin(),flash_good_bundles_map[flash].end(),bundle));
	      cluster_good_bundles_map[cluster].erase(find(cluster_good_bundles_map[cluster].begin(),cluster_good_bundles_map[cluster].end(),bundle));
	      // remove from 
	    }
	  }
	}
      }
      //finish chi2 ...
      
      

      //

      
      FlashTPCBundleSelection to_be_removed;
      for (auto it = cluster_good_bundles_map.begin(); it!= cluster_good_bundles_map.end(); it++){
	PR3DCluster *cluster =  it->first;
	FlashTPCBundleSelection& bundles = it->second;
	bool flag_remove_other = false;
	for (auto it1 = bundles.begin(); it1!= bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  Opflash *flash = bundle->get_flash();
	  
	  if (flash_good_bundles_map[flash].size()==1){
	    flag_remove_other = true;
	    break;
	  }
	}
	if (flag_remove_other){
	  if (cluster_other_bundles_map.find(cluster)!=cluster_other_bundles_map.end()){
	    std::copy(cluster_other_bundles_map[cluster].begin(),
		      cluster_other_bundles_map[cluster].end(),
		      std::back_inserter(to_be_removed));
	  }
	}else{
	  if (cluster_other_bundles_map.find(cluster)!=cluster_other_bundles_map.end()){
	    for (auto it1 = cluster_other_bundles_map[cluster].begin(); it1!= cluster_other_bundles_map[cluster].end(); it1++){
	      FlashTPCBundle *bundle1 = *it1;
	      if (bundle1->get_chi2() > bundle1->get_ndf()*25 && (!bundle1->get_flag_close_to_PMT())){
		to_be_removed.push_back(bundle1);
	      }
	    }
	  }
	}
	
      }

      for (auto it1 = to_be_removed.begin(); it1!=to_be_removed.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	all_bundles.erase(bundle);
	
	Opflash *flash = bundle->get_flash();
	PR3DCluster *cluster = bundle->get_main_cluster();
	
	fc_bundles_map.erase(std::make_pair(flash,cluster));
	
	//	bundles.erase(find(bundles.begin(),bundles.end(),bundle));
	{
	  FlashTPCBundleSelection& temp_bundles = cluster_bundles_map[cluster];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    cluster_bundles_map.erase(cluster);
	}

	{
	  FlashTPCBundleSelection& temp_bundles = flash_bundles_map[flash];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    flash_bundles_map.erase(flash);
	}
	
	delete bundle;
      }
      
    }

    for (auto it = cluster_bundles_map.begin(); it!= cluster_bundles_map.end(); it++){
      PR3DCluster *main_cluster = it->first;
      FlashTPCBundleSelection& bundles = it->second;
      bool flag_tight_bundle = false;

      for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	if (bundle->get_consistent_flag()){
	  flag_tight_bundle = true;
	  break;
	}
      }

      // if (main_cluster->get_cluster_id()==5)
      // 	std::cout << "Xin: " << flag_tight_bundle << " " << bundles.size() << std::endl;
      
      
      // clean up the map ...
      if (flag_tight_bundle){
	// all_bundles;
	// flash_bundles_map;
	// cluster_bundles_map;
	// std::map<std::pair<Opflash*,PR3DCluster*>,FlashTPCBundle*> fc_bundles_map;
	FlashTPCBundleSelection to_be_removed;
	for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if ((!bundle->get_consistent_flag()) && (!bundle->get_flag_at_x_boundary())){
	    if (bundle->get_ks_dis() < 0.06 && bundle->get_chi2() < 60*bundle->get_ndf() &&
	    	bundle->get_flash()->get_total_PE() > 100 && bundle->get_flash()->get_type()==2 &&
	    	bundle->get_total_pred_light() > 30){
	    }else{
	      to_be_removed.push_back(bundle);
	    }
	  }
	  //	  std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << std::endl;
	}
	for (auto it1 = to_be_removed.begin(); it1!=to_be_removed.end(); it1++){
	  FlashTPCBundle *bundle = *it1;

	  // std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << std::endl;
	   
	  all_bundles.erase(bundle);
	  
	  Opflash *flash = bundle->get_flash();
	  PR3DCluster *cluster = bundle->get_main_cluster();
	  
	  fc_bundles_map.erase(std::make_pair(flash,cluster));

	  bundles.erase(find(bundles.begin(),bundles.end(),bundle));

	  FlashTPCBundleSelection& temp_bundles = flash_bundles_map[flash];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    flash_bundles_map.erase(flash);
	  delete bundle;
	}
      }
    }


    // additional check
    for (auto it = flash_bundles_map.begin(); it!=flash_bundles_map.end(); it++){
      Opflash *flash = it->first;
      if (flash->get_type()==2){ // beam ...
	FlashTPCBundleSelection& bundles = it->second;
	FlashTPCBundleSelection consistent_bundles;
	FlashTPCBundle *min_bundle = 0;
	double min_score = 1e9;
	for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if (min_bundle==0){
	    min_bundle = bundle;
	  }else{
	    if (bundle->get_ks_dis() * pow(bundle->get_chi2()/bundle->get_ndf(),0.8) <
		min_bundle->get_ks_dis() * pow(min_bundle->get_chi2()/min_bundle->get_ndf(),0.8))
	      min_bundle = bundle;
	  }
	  if (bundle->get_consistent_flag()){
	    consistent_bundles.push_back(bundle);
	  }
	}

	if (consistent_bundles.size()==0){
	  PR3DCluster *cluster = min_bundle->get_main_cluster();
	  FlashTPCBundleSelection& bundles = cluster_bundles_map[cluster];
	  FlashTPCBundleSelection consistent_bundles;
	  for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	    FlashTPCBundle *bundle = *it1;
	    if (bundle->get_consistent_flag())
	      consistent_bundles.push_back(bundle);
	  }
	  if (consistent_bundles.size()==0){
	    if (min_bundle->examine_beam_bundle())
	      min_bundle->set_consistent_flag(true);
	  }
	}
      }
    }
    
    // finish further examine the bundle ... 
    
    //  std::cout << "After Cleaning 2 : " << cluster_bundles_map.size() << " A " << flash_bundles_map.size() << " " << all_bundles.size() << std::endl;


    // // std::cout << std::endl << std::endl;
    // for (auto it = all_bundles.begin(); it!=all_bundles.end();it++){
    //   FlashTPCBundle *bundle = *it;
      
    //   //  if (bundle->get_flash()->get_flash_id()==29 && bundle->get_main_cluster()->get_cluster_id()==19)
    //   //	bundle->set_consistent_flag(1);
    //   //   if ( bundle->get_consistent_flag() || bundle->get_flag_at_x_boundary())
    //   std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << " " << bundle->get_consistent_flag()  << std::endl;

      
    // }
    // std::cout << std::endl << std::endl;

    
   

    
    // examining flash ... 
    for (auto it = flash_bundles_map.begin(); it!=flash_bundles_map.end(); it++){
      Opflash *flash = it->first;
      FlashTPCBundleSelection& bundles = it->second;
      FlashTPCBundleSelection consistent_bundles;
      FlashTPCBundleSelection remaining_bundles;
      FlashTPCBundleSelection to_be_removed;
      
      for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	if (bundle->get_consistent_flag()){
	  consistent_bundles.push_back(bundle);
	}else{
	  remaining_bundles.push_back(bundle);
	}
      }

      if (consistent_bundles.size()>0){
	for (auto it1 = remaining_bundles.begin(); it1!=remaining_bundles.end(); it1++){
	  FlashTPCBundle *bundle1 = *it1;
	  bool flag_remove = true;
	  for (auto it2 = consistent_bundles.begin(); it2!=consistent_bundles.end(); it2++){
	    FlashTPCBundle *bundle2 = *it2;
	    if (bundle2->examine_bundle(bundle1,cos_pe_low, cos_pe_mid)){
	      flag_remove = false;
	      break;
	    }
	  }
	  if (flag_remove)
	    to_be_removed.push_back(bundle1);
	}
      }

      
      for (auto it1 = to_be_removed.begin(); it1!=to_be_removed.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	all_bundles.erase(bundle);
	
	Opflash *flash = bundle->get_flash();
	PR3DCluster *cluster = bundle->get_main_cluster();
	
	fc_bundles_map.erase(std::make_pair(flash,cluster));

	// remaing flash
	bundles.erase(find(bundles.begin(),bundles.end(),bundle));
	
	FlashTPCBundleSelection& temp_bundles = cluster_bundles_map[cluster];
	temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	if (temp_bundles.size()==0)
	  cluster_bundles_map.erase(cluster);
	delete bundle;
      }
    }


    

    
    // std::cout << "After Cleaning 3 : " << cluster_bundles_map.size() << " A " << flash_bundles_map.size() << " " << all_bundles.size() << std::endl;
    
    // // std::cout << std::endl << std::endl;
    // for (auto it = all_bundles.begin(); it!=all_bundles.end();it++){
    //   FlashTPCBundle *bundle = *it;
      
    // //   if ( bundle->get_consistent_flag() || bundle->get_flag_at_x_boundary())
    //   std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << " " << bundle->get_consistent_flag() << std::endl;
    // }
    // std::cout << std::endl << std::endl;



    // matching code // first round ... 
    {
      // regularization strength ... 
      double lambda = 0.1; // note the coefficient is all around 1
      //form matrix ...
      double fudge_factor1 = 0.06; // add 6% relative uncertainty for pe
      double fudge_factor2 = 1.0; // increase the original uncertainties by 50% ... 
      int num_unknowns = all_bundles.size();
      std::map<PR3DCluster*, int> map_tpc_index;
      std::map<Opflash*, int> map_flash_index;
    
      int tpc_index = 0;
      
      for(auto it=cluster_bundles_map.begin(); it!=cluster_bundles_map.end(); it++){
	PR3DCluster* main_cluster = it->first;
	map_tpc_index[main_cluster] = tpc_index;
	tpc_index++;
      }
      
      // improve the chisquare definition ...
      double delta_track = 0.01; // track can only be used once
      double delta_flash = 0.025;
      //    double delta_flash1 = 0.1;
      
      double num_unused_flash = flash_bundles_map.size() - cluster_bundles_map.size();
      if (num_unused_flash<0) num_unused_flash = 0;
      
      VectorXd M = VectorXd::Zero(32*flash_bundles_map.size()); // measurement from each PMT from each flash
      MatrixXd R = MatrixXd::Zero(32*flash_bundles_map.size(), num_unknowns+flash_bundles_map.size()); // unknowns to measurement matrix
      VectorXd MF = VectorXd::Zero(map_tpc_index.size() + flash_bundles_map.size());
      MatrixXd RF = MatrixXd::Zero(map_tpc_index.size() + flash_bundles_map.size(), num_unknowns + flash_bundles_map.size()); // penalty matrix term
      std::vector<std::pair<Opflash*,PR3DCluster*>> total_pairs;
      std::vector<double> total_weights;
      
      size_t i=0;
      for (auto it = flash_bundles_map.begin(); it != flash_bundles_map.end(); it++){
	Opflash *flash = it->first;
	FlashTPCBundleSelection& bundles = it->second;
	for (size_t j=0;j!=32;j++){
	  double pe = flash->get_PE(j);
	  double pe_err = sqrt(pow(flash->get_PE_err(j)*fudge_factor2,2) + pow(pe*fudge_factor1,2) + pow(pe*rel_light_yield_err,2));
	  // if (pe/pe_err>0)
	  //   std::cout << i << " A " << j << " " << pe/pe_err << std::endl;
	  
	  M(32*i+j) = pe/pe_err;
	  R(32*i+j,num_unknowns+i) = pe/pe_err; // flash alone term
	}
	
	for (size_t j=0;j!=bundles.size();j++){
	  FlashTPCBundle *bundle = bundles.at(j);
	  PR3DCluster* main_cluster = bundle->get_main_cluster();
	  std::vector<double>& pred_pmt_light = bundle->get_pred_pmt_light();
	  for (size_t k=0;k!=32;k++){
	    double pe = flash->get_PE(k);
	    double pe_err = sqrt(pow(flash->get_PE_err(k)*fudge_factor2,2) + pow(pe*fudge_factor1,2)+ pow(pe*rel_light_yield_err,2));
	    R(32*i+k,total_pairs.end()-total_pairs.begin()) = 1./pe_err * pred_pmt_light.at(k);
	  }
	  
	  total_pairs.push_back(std::make_pair(flash,main_cluster));
	  
	  if (bundle->get_flag_at_x_boundary()){
	    total_weights.push_back(0.2);
	  }else{
	    total_weights.push_back(1.0);
	  }
	}
	
	i++;
      }
      
      for (auto it = flash_bundles_map.begin(); it != flash_bundles_map.end(); it++){
	Opflash *flash = it->first;
	total_weights.push_back(1);
      }
      
      
      
      // normalization of the flashes 
      i=0;
      for (auto it = flash_bundles_map.begin(); it != flash_bundles_map.end(); it++){
	MF(cluster_bundles_map.size()+i) = 0;//num_unused_flash/delta_flash;
	RF(cluster_bundles_map.size()+i,num_unknowns+i) = 1./delta_flash;
	
	map_flash_index[it->first] = num_unknowns + i;
	//	MF(cluster_bundles_map.size()+flash_bundles_map.size()) = num_unused_flash/delta_flash1;
	//	RF(cluster_bundles_map.size()+flash_bundles_map.size(),num_unknowns+i) = 1./delta_flash1;
	i++;
      }
      
      // normalization of the tracks
      i=0;
      for (auto it=cluster_bundles_map.begin(); it!=cluster_bundles_map.end(); it++){
	MF(i) = 1./delta_track;
	i++;
      }
      {
	for (size_t i=0; i!=total_pairs.size(); i++){
	  Opflash *flash = total_pairs.at(i).first;
	  PR3DCluster *main_cluster = total_pairs.at(i).second;
	  RF(map_tpc_index[main_cluster], i) = 1./delta_track;	
	}
      }
      
      
      MatrixXd RT = R.transpose();
      MatrixXd RFT = RF.transpose();
      
      VectorXd W = RT * M + RFT * MF;
      MatrixXd G = RT * R + RFT * RF;

      // for (size_t i = 0; i!= 32*flash_bundles_map.size(); i++){
      // 	std::cout << i << " " << M(i) << std::endl;
      // }
      
      
      WCP::LassoModel m2(lambda, 100000, 0.01);
      m2.SetData(G, W);

      std::vector<double> init_values;
      init_values.resize(num_unknowns + flash_bundles_map.size(),0);
     
      for (size_t i=0; i!=total_pairs.size(); i++){
	FlashTPCBundle *bundle = fc_bundles_map[std::make_pair(total_pairs.at(i).first, total_pairs.at(i).second)];
	if (bundle->get_consistent_flag()){
	  init_values.at(i) = 1;
	}else if (bundle->get_flag_at_x_boundary()){
	  init_values.at(i) = 0.5;
	}
      }
      m2.Set_init_values(init_values);
      
      for (size_t i=0; i!=total_weights.size(); i++){
	m2.SetLambdaWeight(i,total_weights.at(i));
      }
      m2.Fit();
      VectorXd beta = m2.Getbeta();

      
      FlashTPCBundleSelection to_be_removed;
      for (size_t i=0;i!=total_pairs.size();i++){
	//	std::cout << i << " " << beta(i)  << std::endl;
	if (beta(i) < 0.05){
	  to_be_removed.push_back(fc_bundles_map[std::make_pair(total_pairs.at(i).first, total_pairs.at(i).second)]);
	}
      }
      
      for (auto it1 = to_be_removed.begin(); it1!=to_be_removed.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	all_bundles.erase(bundle);
	
	Opflash *flash = bundle->get_flash();
	PR3DCluster *cluster = bundle->get_main_cluster();
	
	
	
	{
	  FlashTPCBundleSelection& temp_bundles = flash_bundles_map[flash];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    flash_bundles_map.erase(flash);
	}
	{
	  FlashTPCBundleSelection& temp_bundles = cluster_bundles_map[cluster];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    cluster_bundles_map.erase(cluster);
	}
	
	//	fc_bundles_map.erase(std::make_pair(flash,cluster));
	//	delete bundle;
      }
    }
    
    //   std::cout << "After Cleaning 4 : " << cluster_bundles_map.size() << " A " << flash_bundles_map.size() << " " << all_bundles.size() << std::endl;
    
    // regularization strength ... 
    double lambda = 0.1; // note the coefficient is all around 1
    //form matrix ...
    double fudge_factor1 = 0.05; // add 6% relative uncertainty for pe
    double fudge_factor2 = 1.0; // increase the original uncertainties by 50% ... 
    int num_unknowns = all_bundles.size();
    std::map<PR3DCluster*, int> map_tpc_index;
    std::map<Opflash*, int> map_flash_index;
    
    int tpc_index = 0;
    
    for(auto it=cluster_bundles_map.begin(); it!=cluster_bundles_map.end(); it++){
      PR3DCluster* main_cluster = it->first;
      map_tpc_index[main_cluster] = tpc_index;
      tpc_index++;
    }
    
    // improve the chisquare definition ...
    double delta_track = 0.01; // track can only be used once
    
    VectorXd M = VectorXd::Zero(32*flash_bundles_map.size()); // measurement from each PMT from each flash
    MatrixXd R = MatrixXd::Zero(32*flash_bundles_map.size(), num_unknowns); // unknowns to measurement matrix
    VectorXd MF = VectorXd::Zero(map_tpc_index.size() );
    MatrixXd RF = MatrixXd::Zero(map_tpc_index.size() , num_unknowns ); // penalty matrix term
    std::vector<std::pair<Opflash*,PR3DCluster*>> total_pairs;
    std::vector<double> total_weights;
    
    size_t i=0;
    for (auto it = flash_bundles_map.begin(); it != flash_bundles_map.end(); it++){
      Opflash *flash = it->first;
      FlashTPCBundleSelection& bundles = it->second;
      for (size_t j=0;j!=32;j++){
	double pe = flash->get_PE(j);
	double pe_err = sqrt(pow(flash->get_PE_err(j)*fudge_factor2,2) + pow(pe*fudge_factor1,2)+ pow(pe*rel_light_yield_err,2));
	
	M(32*i+j) = pe/pe_err;
	//	R(32*i+j,num_unknowns+i) = pe/pe_err; // flash alone term
      }
      
      for (size_t j=0;j!=bundles.size();j++){
	FlashTPCBundle *bundle = bundles.at(j);
	PR3DCluster* main_cluster = bundle->get_main_cluster();
	std::vector<double>& pred_pmt_light = bundle->get_pred_pmt_light();
	for (size_t k=0;k!=32;k++){
	  double pe = flash->get_PE(k);
	  double pe_err = sqrt(pow(flash->get_PE_err(k)*fudge_factor2,2) + pow(pe*fudge_factor1,2)+ pow(pe*rel_light_yield_err,2));
	  R(32*i+k,total_pairs.end()-total_pairs.begin()) = 1./pe_err * pred_pmt_light.at(k);
	}
	
	total_pairs.push_back(std::make_pair(flash,main_cluster));
	
	if (bundle->get_flag_at_x_boundary()){
	  total_weights.push_back(0.2);
	}else{
	  total_weights.push_back(1.0);
	}
      }
      
      i++;
    }
    
    // normalization of the tracks
    i=0;
    for (auto it=cluster_bundles_map.begin(); it!=cluster_bundles_map.end(); it++){
      MF(i) = 1./delta_track;
      i++;
    }
    {
      for (size_t i=0; i!=total_pairs.size(); i++){
	Opflash *flash = total_pairs.at(i).first;
	PR3DCluster *main_cluster = total_pairs.at(i).second;
	RF(map_tpc_index[main_cluster], i) = 1./delta_track;	
      }
    }
    
      
    MatrixXd RT = R.transpose();
    MatrixXd RFT = RF.transpose();
    
    VectorXd W = RT * M + RFT * MF;
    MatrixXd G = RT * R + RFT * RF;
    
    WCP::LassoModel m2(lambda, 100000, 0.01);
    m2.SetData(G, W);
    for (size_t i=0; i!=total_weights.size(); i++){
      m2.SetLambdaWeight(i,total_weights.at(i));
    }
    m2.Fit();
    VectorXd beta = m2.Getbeta();
    
    // for (auto it = flash_bundles_map.begin(); it != flash_bundles_map.end(); it++){
    //   Opflash *flash = it->first;
    //   std::cout << flash->get_flash_id() << " " << beta(map_flash_index[flash]) << std::endl;
    // }


    // form matched pairs according to TPC cluster 
    std::map<int,std::pair<Opflash*,double>> matched_pairs;
    for (size_t i=0;i!=total_pairs.size();i++){
      if(beta(i)!=0){
  	int tpc_index = map_tpc_index[total_pairs.at(i).second];
  	Opflash *flash = total_pairs.at(i).first;
  	if (matched_pairs.find(tpc_index)==matched_pairs.end()){
  	  matched_pairs[tpc_index] = std::make_pair(flash,beta(i));
  	}else{
  	  if (beta(i) > matched_pairs[tpc_index].second){
  	    matched_pairs[tpc_index] = std::make_pair(flash,beta(i));
  	  }
  	}
	//std::cout << i << " Q " <<  tpc_index << " " << flash->get_flash_id() << " " << total_pairs.at(i).second->get_cluster_id() << " " << total_weights.at(i) << " " << beta(i)  << " " << flash->get_time() << std::endl;
      }
    }
    
    FlashTPCBundleSelection results_bundles;
    FlashTPCBundleSet results_bundles_set;
    // return bundles ...    
    for (auto it = group_clusters.begin(); it!=group_clusters.end(); it++){
      PR3DCluster* main_cluster = it->first;

      // {
      // 	ToyPointCloud *pcloud = main_cluster->get_point_cloud();
      // 	Point test_p(236.3*units::cm,13.4*units::cm,392.6*units::cm);
      // 	if (pcloud->get_closest_dis(test_p) < 1*units::cm)
      // 	  std::cout  << " C " << main_cluster->get_cluster_id() << " " << pcloud->get_closest_dis(test_p)/units::cm << std::endl;
      // }
      
      if (map_tpc_index.find(main_cluster)!=map_tpc_index.end()){
	int tpc_index = map_tpc_index[main_cluster];
	if (matched_pairs.find(tpc_index)!=matched_pairs.end()){
	  Opflash* flash = matched_pairs[tpc_index].first;
	  double strength = matched_pairs[tpc_index].second;
	  FlashTPCBundle* bundle = fc_bundles_map[std::make_pair(flash,main_cluster)];
	  bundle->set_strength(strength);
	  results_bundles_set.insert(bundle);
	  //	  results_bundles.push_back(bundle);
	}else{
	  Opflash *flash = 0;
	  double strength  =0;
	  FlashTPCBundle *bundle = new FlashTPCBundle(flash,main_cluster,-1,tpc_index);
	  PR3DClusterSelection& other_clusters = bundle->get_other_clusters();
	  for (auto it1 = it->second.begin(); it1!= it->second.end(); it1++){
	    other_clusters.push_back(it1->first);
	  }
	  results_bundles_set.insert(bundle);
	  //	  results_bundles.push_back(bundle);
	}
      }else{
	Opflash *flash = 0;
	double strength  =0;
	FlashTPCBundle *bundle = new FlashTPCBundle(flash,main_cluster,-1,-1);
	PR3DClusterSelection& other_clusters = bundle->get_other_clusters();
	for (auto it1 = it->second.begin(); it1!= it->second.end(); it1++){
	  other_clusters.push_back(it1->first);
	}
	results_bundles_set.insert(bundle);
	//	results_bundles.push_back(bundle);
      }
    }
    std::copy(results_bundles_set.begin(), results_bundles_set.end(), std::back_inserter(results_bundles));
    
    // std::cout << "Final: " << std::endl;
    // for (auto it = results_bundles.begin(); it!= results_bundles.end(); it++){
    //   FlashTPCBundle *bundle = (*it);
    //   Opflash *flash = bundle->get_flash();
    //   if (flash!=0){
    // 	std::cout << flash->get_flash_id() << " " << flash->get_time() << " " << bundle->get_main_cluster()->get_cluster_id() << std::endl;
    //   }else{
    // 	std::cout << -1 << " " << bundle->get_main_cluster()->get_cluster_id() << " " << cluster_bundles_map[bundle->get_main_cluster()].size() << std::endl;
    //   }
    // }

    
    organize_matched_bundles(results_bundles, cos_pe_low, cos_pe_mid, fc_bundles_map);


    // std::cout << "Final1: " << std::endl;
    // for (auto it = results_bundles.begin(); it!= results_bundles.end(); it++){
    //   FlashTPCBundle *bundle = (*it);
    //   Opflash *flash = bundle->get_flash();
    //   if (flash!=0){
    // 	std::cout << flash->get_flash_id() << " " << flash->get_time() << " " << bundle->get_main_cluster()->get_cluster_id() << std::endl;
    //   }else{
    // 	std::cout << -1 << " " << bundle->get_main_cluster()->get_cluster_id() << " " << cluster_bundles_map[bundle->get_main_cluster()].size() << std::endl;
    //   }
    // }

    
    return results_bundles;
  }
}


void WCP2dToy::organize_matched_bundles(WCP::FlashTPCBundleSelection& results_bundles,  Double_t *cos_pe_low, Double_t *cos_pe_mid, std::map<std::pair<Opflash*,PR3DCluster*>,FlashTPCBundle*>& fc_bundles_map){
  std::map<Opflash*, FlashTPCBundleSelection> flash_bundles_map;

  // get all the flashes and associated bundles for more than one bundle ... 
  for (auto it = results_bundles.begin(); it!= results_bundles.end(); it++){
    FlashTPCBundle *bundle = (*it);
    Opflash *flash = bundle->get_flash();
    if (flash!=0){
      // std::cout << flash->get_flash_id() << " " << flash->get_time() << " " << bundle->get_main_cluster()->get_cluster_id() << std::endl;
      
      if (flash_bundles_map.find(flash)==flash_bundles_map.end()){
	FlashTPCBundleSelection bundles;
	bundles.push_back(bundle);
	flash_bundles_map[flash] = bundles;
      }else{
	flash_bundles_map[flash].push_back(bundle);
      }
    }
  }

  // {
  //   OpflashSelection flashes;
  //   for (auto it= flash_bundles_map.begin(); it!=flash_bundles_map.end(); it++){
  //     // std::cout << it->first->get_flash_id() << " " << it->first->get_time() << " " << it->second.size() << std::endl;
      
  //     if (it->second.size()==1){
  // 	flashes.push_back(it->first);
  // 	//      flash_bundles_map.erase(it);
  // 	//      std::cout << it->second.size() << std::endl;
  // 	//      }else{
  // 	//      std::cout << "A: " << it->second.size() << std::endl;
  //     }
  //   }
  //   for (auto it = flashes.begin(); it!=flashes.end(); it++){
  //     flash_bundles_map.erase(*it);
  //   }
  // }


  FlashTPCBundleSelection second_round_bundles;
  FlashTPCBundleSelection third_round_bundles;
  
  for (auto it= flash_bundles_map.begin(); it!=flash_bundles_map.end(); it++){
    FlashTPCBundleSelection& old_bundles = it->second;
    Opflash *flash = it->first;
    
    FlashTPCBundleSelection to_be_removed;
    
    // bool flag_close_to_PMT = false;
    // bool flag_at_x_boundary = false;
    // PR3DClusterSelection main_clusters;
    // PR3DClusterSelection other_clusters;
    // PR3DClusterSelection more_clusters;
    // std::vector<double> pred_pmt_light(32,0);

    FlashTPCBundle* best_bundle = 0;
    double best_value = 1e9;

    // std::cout << "Flash: " << flash->get_flash_id() << std::endl;
    
    for (auto it1 = old_bundles.begin(); it1!=old_bundles.end(); it1++){
      FlashTPCBundle* bundle = (*it1);

      double value = bundle->get_ks_dis() * pow(bundle->get_chi2()/std::max(bundle->get_ndf(),1),0.8);
      
      if (bundle->get_flag_at_x_boundary()){
      	value *= 0.8;
	if (bundle->get_flag_close_to_PMT()){
	  value *= 0.8;
	  
	  if (best_bundle!=0){
	    if (best_bundle->get_ks_dis() > 0.2 && best_bundle->get_chi2()>best_bundle->get_ndf()*60)
	      value *= 0.75;
	  }
	}
      }


      if (value < best_value){
	best_value = value;
	best_bundle = bundle;
      }
      
      // std::cout << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_flag_close_to_PMT() << " " << value << std::endl;
    }
    
    //  std::cout << best_bundle->get_ks_dis() << " " << best_bundle->get_chi2() << " " << best_bundle->get_ndf() << std::endl;
    
    for (auto it1 = old_bundles.begin(); it1!=old_bundles.end(); it1++){
      FlashTPCBundle* bundle = (*it1);
      //  std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << " " << bundle->get_flag_close_to_PMT() << std::endl;

      if (bundle!=best_bundle){
	//	std::cout << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf()<< " " << best_bundle->examine_bundle(bundle,cos_pe_low, cos_pe_mid) << std::endl;
	
	if (best_bundle->examine_bundle_rank(bundle,cos_pe_low, cos_pe_mid)){
	  // std::cout << best_bundle->get_other_clusters().size() << std::endl;
	  best_bundle->add_bundle(bundle,cos_pe_low,cos_pe_mid);
	  
	  // update group clusters ... 
	  
	  
	  // std::cout << best_bundle->get_other_clusters().size() << std::endl;
	  to_be_removed.push_back(bundle);
	}else{
	  second_round_bundles.push_back(bundle);
	  //	  bundle->set_flash(0);
	}
      }
    }

    for (auto it = to_be_removed.begin(); it!=to_be_removed.end(); it++){

      //FlashTPCBundle *bundle = *it;
      //std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << " " << bundle->get_flag_close_to_PMT() << std::endl;
      
      results_bundles.erase(find(results_bundles.begin(),results_bundles.end(),*it));
    }

    if (best_bundle!=0){
      if (flash->get_type()==2) // beam discriminator ... 
	best_bundle->examine_merge_clusters();
    }
    
  }

  {
    FlashTPCBundleSelection to_be_removed;
    std::set<Opflash*> tried_flashes;
    //  std::cout << second_round_bundles.size() << std::endl;
    for (auto it = second_round_bundles.begin(); it!=second_round_bundles.end(); it++){
      FlashTPCBundle *bundle = (*it);
      PR3DCluster *main_cluster = bundle->get_main_cluster();
      bool flag_used = false;
      for (auto it1 = results_bundles.rbegin(); it1!=results_bundles.rend(); it1++){
	FlashTPCBundle *best_bundle = (*it1);

	if (find(second_round_bundles.begin(), second_round_bundles.end(), best_bundle) != second_round_bundles.end()) continue;
	//if (best_bundle == bundle) continue;
	
	Opflash *flash = best_bundle->get_flash();
	if (fc_bundles_map.find(std::make_pair(flash, main_cluster))!=fc_bundles_map.end()){
	  FlashTPCBundle *test_bundle = fc_bundles_map[std::make_pair(flash, main_cluster)];
	  tried_flashes.insert(flash);
	  if (best_bundle->examine_bundle_rank(test_bundle,cos_pe_low, cos_pe_mid)){
	    //  std::cout << flash->get_flash_id() << " " << best_bundle->get_main_cluster()->get_cluster_id() << " " << best_bundle << " " << bundle << std::endl;
	    //std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << " " << bundle->get_flag_close_to_PMT() << std::endl;
	    best_bundle->add_bundle(test_bundle,cos_pe_low,cos_pe_mid);
	    flag_used = true;
	    break;
	  }
	}
      }
      
      if (!flag_used){
	third_round_bundles.push_back(bundle);
	//bundle->set_flash(0);
	//std::cout << " A " << bundle->get_main_cluster()->get_cluster_id() << std::endl; 
      }else{
	//std::cout << " B " << bundle->get_main_cluster()->get_cluster_id() << std::endl; 
	to_be_removed.push_back(bundle);
      }
      //std::cout << " A " << bundle->get_main_cluster()->get_cluster_id() << std::endl; 
      //bundle->set_flash(0);
    }
    
    for (auto it = to_be_removed.begin(); it!=to_be_removed.end(); it++){
      //      FlashTPCBundle *bundle = *it;
      // std::cout << bundle << " " << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << " " << bundle->get_flag_close_to_PMT() << std::endl;
      
      results_bundles.erase(find(results_bundles.begin(),results_bundles.end(),*it));
    }

    
    for (auto it = third_round_bundles.begin(); it!=third_round_bundles.end(); it++){
      FlashTPCBundle *bundle = (*it);
      PR3DCluster *main_cluster = bundle->get_main_cluster();
      FlashTPCBundleSelection temp_bundles;
      for (auto it1 = fc_bundles_map.begin(); it1!=fc_bundles_map.end(); it1++){
    	if (it1->first.second == main_cluster && it1->second != bundle &&
    	    tried_flashes.find(it1->first.first)==tried_flashes.end()){
    	  temp_bundles.push_back(it1->second);
    	}
      }
      if (temp_bundles.size()==1){
    	results_bundles.push_back(temp_bundles.front());
    	tried_flashes.insert(temp_bundles.front()->get_flash());
      } else if (temp_bundles.size()>1){
    	FlashTPCBundle *min_bundle = temp_bundles.front();
    	double min_val = min_bundle->get_ks_dis() * pow(min_bundle->get_chi2()/min_bundle->get_ndf(),0.8);
    	for (size_t ii=1; ii!=temp_bundles.size(); ii++){
    	  if (temp_bundles.at(ii)->get_ks_dis() * pow(temp_bundles.at(ii)->get_chi2()/temp_bundles.at(ii)->get_ndf(),0.8) < min_val){
    	    min_val = temp_bundles.at(ii)->get_ks_dis() * pow(temp_bundles.at(ii)->get_chi2()/temp_bundles.at(ii)->get_ndf(),0.8);
    	    min_bundle = temp_bundles.at(ii);
    	  }
    	}
    	results_bundles.push_back(min_bundle);
    	tried_flashes.insert(min_bundle->get_flash());
      }
      bundle->set_flash(0);
    }
    

    std::set<FlashTPCBundle*> remaining_bundles(results_bundles.begin(), results_bundles.end());
    for (auto it = fc_bundles_map.begin(); it!=fc_bundles_map.end(); it++){
      FlashTPCBundle *bundle = it->second;
      if (remaining_bundles.find(bundle)==remaining_bundles.end()){
	delete bundle;
      }
    }
    
  }
  FlashTPCBundleSet remaining_bundles(results_bundles.begin(), results_bundles.end());
  results_bundles.clear();
  std::copy(remaining_bundles.begin(), remaining_bundles.end(), std::back_inserter(results_bundles));
  // 

  
}


/*

FlashTPCBundleSelection WCP2dToy::tpc_light_match_ana(int time_offset, int nrebin, std::map<WCP::PR3DCluster*,std::vector<std::pair<WCP::PR3DCluster*,double>>>& group_clusters, WCP::OpflashSelection& flashes){

  std::map<int, std::vector<double> > pmt_pos_map;
  {
    // read in PMT positions
    std::ifstream infile("./input_data_files/uBOpDetGeometry.txt");
    TString temp;
    infile >> temp >> temp >> temp >> temp >> temp;
    Int_t OpDetid;
    Int_t OpChannel;
    Double_t x,y,z;
    for (int i=0;i!=32;i++){
      infile >> OpDetid >> OpChannel >> x >> y >> z;
      // x-= 10;
      std::vector<double> pos_vec;
      pos_vec.push_back(x);
      pos_vec.push_back(y);
      pos_vec.push_back(z);
      pmt_pos_map[OpDetid] = pos_vec;
    }
  }

  // load in the analytical functions
    Double_t _fx1[20] = {5,    15,    25,   35,   45,
			55,   65,   75,   85,   95,
			105,   115,   125,   135,   145,
			155,   165,   175,   185,   195};
   Double_t _fy1[20] = {1,   1,   1,   0.823624,   0.715388,
			0.646044,   0.588849,   0.575943,   0.582408,   0.576089,
			0.580018,   0.575588,   0.557143,   0.556176,   0.558695,
			0.558078,   0.559247,   0.542881,   0.544605,   0.544605};
   Double_t _fx3[20] = {5,    15,    25,   35,   45,
			55,   65,   75,   85,   95,
			105,   115,   125,   135,   145,
			155,   165,   175,   185,   195};
   Double_t _fy3[20] = {0,   0,   0,   0.324246,   0.576424,
			0.770749,   0.947143,   0.986532,   0.9755,   1.00555,
			0.9974,   1.02315,   1.05233,   1.05233,   1.06443,
			1.06312,   1.05925,   1.08835,   1.10186,   1.10186};

   TGraph *g1 = new TGraph(20,_fx1,_fy1);
   TGraph *g2 = new TGraph(20,_fx3,_fy3);

   Double_t _fx2[100]={5, 15, 25, 35, 45, 55, 65, 75, 85, 95, 105, 115, 125, 135, 145, 155, 165, 175, 185, 195, 205, 215, 225, 235, 245, 255, 265, 275, 285, 295, 305, 315, 325, 335, 345, 355, 365, 375, 385, 395, 405, 415, 425, 435, 445, 455, 465, 475, 485, 495, 505, 515, 525, 535, 545, 555, 565, 575, 585, 595, 605, 615, 625, 635, 645, 655, 665, 675, 685, 695, 705, 715, 725, 735, 745, 755, 765, 775, 785, 795, 805, 815, 825, 835, 845, 855, 865, 875, 885, 895, 905, 915, 925, 935, 945, 955, 965, 975, 985, 995};
   Double_t _fy2[100]={0.287339, 0.154971, 0.0530511, 0.023104, 0.0129285, 0.00819706, 0.00555778, 0.00405815, 0.00312001, 0.00244182, 0.00195733, 0.00158211, 0.00129476, 0.00107603, 0.000898136, 0.00075793, 0.000643301, 0.000544246, 0.000457731, 0.000392602, 0.000338131, 0.000291944, 0.000252584, 0.000218904, 0.000189452, 0.000163954, 0.000141267, 0.000119913, 0.000105198, 9.20097e-05, 8.09518e-05, 7.12056e-05, 6.30406e-05, 5.57458e-05, 4.92975e-05, 4.35345e-05, 3.8491e-05, 3.40909e-05, 3.01154e-05, 2.66476e-05, 2.36417e-05, 2.09674e-05, 1.86273e-05, 1.66184e-05, 1.48374e-05, 1.32267e-05, 1.18402e-05, 1.0617e-05, 9.58763e-06, 8.69192e-06, 7.91104e-06, 7.22985e-06, 6.6587e-06, 6.18978e-06, 5.77432e-06, 5.38415e-06, 5.05169e-06, 4.79224e-06, 4.58191e-06, 4.36328e-06, 4.16866e-06, 4.01194e-06, 3.88083e-06, 3.76831e-06, 3.67494e-06, 3.61619e-06, 3.56548e-06, 3.49053e-06, 3.43314e-06, 3.4035e-06, 3.40832e-06, 3.40506e-06, 3.3574e-06, 3.32827e-06, 3.36113e-06, 3.35521e-06, 3.28421e-06, 3.28024e-06, 3.27836e-06, 3.29369e-06, 3.26025e-06, 3.21626e-06, 3.20056e-06, 3.15027e-06, 3.19043e-06, 3.24328e-06, 3.30175e-06, 3.27816e-06, 3.22626e-06, 3.3214e-06, 3.38108e-06, 3.48751e-06, 3.39843e-06, 3.43438e-06, 3.54759e-06,3.54759e-06,3.54759e-06,3.54759e-06,3.54759e-06,3.54759e-06};
   TGraph *g3 = new TGraph(100,_fx2,_fy2);
  
  Double_t cos_pe_low[32]={11,11,11,11,10,
			   7,8,8,10,7,
			   11,11,11,11,10,
			   9,11,11,7,9,
			   11,10,11,11,11,
			   11,11,10,11,11,
			   9,10};
  Double_t cos_pe_mid[32]={34,32,28,35,22,
			   23,22,24,33,30,
			   35,35,33,36,33,
			   33,36,33,19,27,
			   32,23,42,32,33,
			   34,34,24,33,35,
			   25,32};
  
  std::map<int,int> map_lib_pmt,map_pmt_lib;
  map_lib_pmt[1]=2; map_pmt_lib[2]=1; 
  map_lib_pmt[0]=4; map_pmt_lib[4]=0; 
  map_lib_pmt[3]=0; map_pmt_lib[0]=3; 
  map_lib_pmt[2]=5; map_pmt_lib[5]=2; 
  map_lib_pmt[5]=1; map_pmt_lib[1]=5; 
  map_lib_pmt[4]=6; map_pmt_lib[6]=4; 
  map_lib_pmt[6]=3; map_pmt_lib[3]=6; 
  
  map_lib_pmt[9]=7; map_pmt_lib[7]=9; 
  map_lib_pmt[7]=9; map_pmt_lib[9]=7; 
  map_lib_pmt[8]=11; map_pmt_lib[11]=8; 
  map_lib_pmt[11]=8; map_pmt_lib[8]=11;  
  map_lib_pmt[10]=12; map_pmt_lib[12]=10; 
  map_lib_pmt[12]=10; map_pmt_lib[10]=12; 

  map_lib_pmt[14]=13; map_pmt_lib[13]=14;  
  map_lib_pmt[13]=15; map_pmt_lib[15]=13; 
  map_lib_pmt[15]=17; map_pmt_lib[17]=15; 
  map_lib_pmt[17]=14; map_pmt_lib[14]=17; 
  map_lib_pmt[16]=18; map_pmt_lib[18]=16; 
  map_lib_pmt[18]=16; map_pmt_lib[16]=18; 

  map_lib_pmt[21]=19; map_pmt_lib[19]=21; 
  map_lib_pmt[22]=20; map_pmt_lib[20]=22; 
  map_lib_pmt[19]=21; map_pmt_lib[21]=19; 
  map_lib_pmt[20]=23; map_pmt_lib[23]=20; 
  map_lib_pmt[23]=24; map_pmt_lib[24]=23; 
  map_lib_pmt[24]=22; map_pmt_lib[22]=24; 

  map_lib_pmt[26]=25; map_pmt_lib[25]=26; 
  map_lib_pmt[27]=30; map_pmt_lib[30]=27; 
  map_lib_pmt[28]=31; map_pmt_lib[31]=28; 
  map_lib_pmt[31]=29; map_pmt_lib[29]=31;
  // original map
  map_lib_pmt[25]=28; map_pmt_lib[28]=25; 
  map_lib_pmt[30]=27; map_pmt_lib[27]=30; 
  map_lib_pmt[29]=26; map_pmt_lib[26]=29;

 

  TPCParams& mp = Singleton<TPCParams>::Instance();
  double time_slice_width = mp.get_ts_width();
  
  //Figure out how to use library ... 
  //flashes measurement = 32 * num_flashes 
  const int num_flashes = flashes.size();
  const int num_tpc_objs = group_clusters.size();


  // {
  //   int temp_num = 0;
  //   for (auto it = group_clusters.begin(); it!= group_clusters.end(); it++){
  //     temp_num += it->second.size()+1;
  //   }
  //   std::cout << live_clusters.size() << " " << temp_num << std::endl;
  // }
  
  //std::cout << num_flashes << " " << num_tpc_objs << std::endl;
  double high_x_cut = 256 * units::cm;
  double high_x_cut_ext1 = + 1.2*units::cm;
  double high_x_cut_ext2 = - 2.0*units::cm;
  double low_x_cut = 0*units::cm;
  double low_x_cut_ext1 = - 2*units::cm;
  double low_x_cut_ext2 = + 2.0*units::cm;
  double scaling_light_mag = 0.01 * 1.5;

  int solv_type = 1; // new matching code ... 
  
  if (solv_type==1){
    FlashTPCBundleSet all_bundles;
    Flash_bundles_map flash_bundles_map;
    Cluster_bundles_map cluster_bundles_map;
    std::map<std::pair<Opflash*,PR3DCluster*>,FlashTPCBundle*> fc_bundles_map;
      
    int flash_index_id = 0;
    for (auto it1 = flashes.begin(); it1!=flashes.end(); it1++){
      Opflash *flash = (*it1);
      double offset_x = (flash->get_time() - time_offset)*2./nrebin*time_slice_width;
      int cluster_index_id = 0;
      for (auto it2 = group_clusters.begin(); it2!=group_clusters.end(); it2++){
  	PR3DCluster* main_cluster = it2->first;
  	std::vector<std::pair<WCP::PR3DCluster*,double>>& additional_clusters = it2->second;
  	FlashTPCBundle *bundle = new FlashTPCBundle(flash, main_cluster, flash_index_id, cluster_index_id);
  	bool flag_good_bundle = false;
	
  	std::vector<double>& pred_pmt_light = bundle->get_pred_pmt_light();
  	PR3DClusterSelection& other_clusters = bundle->get_other_clusters();
  	PR3DClusterSelection& more_clusters = bundle->get_more_clusters();

  	double first_pos_x = (*((main_cluster->get_time_cells_set_map().begin())->second.begin()))->get_sampling_points().front().x;
  	double last_pos_x = (*((main_cluster->get_time_cells_set_map().rbegin())->second.begin()))->get_sampling_points().front().x;

	bool flag_spec_end = false;
	
	// improve the position code ... 
	if (first_pos_x - offset_x <= low_x_cut + low_x_cut_ext1 &&
	    first_pos_x - offset_x > low_x_cut - 120*units::cm ){
	  
	  std::map<int,SMGCSet>& time_cells_set_map = main_cluster->get_time_cells_set_map();
	  int num_mcells_outside = 0;
	  int num_time_slices_outside = 0;

	  int num_mcells_def_outside = 0;
	  
	  double prev_pos_x= first_pos_x;
	  double current_pos_x = first_pos_x;
	  for (auto it3 = time_cells_set_map.begin(); it3 != time_cells_set_map.end(); it3++){
	    current_pos_x = (*(it3->second.begin()))->get_sampling_points().front().x;


	      // if (flash->get_flash_id()==35&&abs(main_cluster->get_cluster_id()-5)<=0)
	      // std::cout << num_time_slices_outside<< " " << num_mcells_outside << "  " << (first_pos_x - offset_x)/units::cm << " " <<
	      // 	(prev_pos_x-offset_x)/units::cm << " " << (current_pos_x-offset_x)/units::cm << std::endl;
	    
	    if (current_pos_x -offset_x > low_x_cut + low_x_cut_ext1 && current_pos_x - prev_pos_x > 0.75*units::cm)
	      break;
	    if (num_time_slices_outside > 60) break;
	    
	    if (current_pos_x -offset_x < low_x_cut + low_x_cut_ext1) num_mcells_def_outside += it3->second.size();

	    num_time_slices_outside += 1;
	    num_mcells_outside += it3->second.size();
	    prev_pos_x = current_pos_x;
	  }
	  if (num_time_slices_outside <=36 && num_mcells_outside < 0.05*main_cluster->get_num_mcells()){
	    first_pos_x = current_pos_x;
	    if (num_time_slices_outside > 10 && fabs(current_pos_x - prev_pos_x)<10*units::cm)
	      flag_spec_end = true;
	  }else if (num_time_slices_outside <=60 && num_mcells_outside < 0.06*main_cluster->get_num_mcells() && fabs(current_pos_x - prev_pos_x)>10*units::cm){
	    first_pos_x = current_pos_x;
	  }else if (num_time_slices_outside <=25 && num_mcells_outside < 0.12 * main_cluster->get_num_mcells() && fabs(current_pos_x - prev_pos_x)>20*units::cm){
	    first_pos_x = current_pos_x;
	  }

	  if (num_mcells_def_outside < 0.0015 * main_cluster->get_num_mcells()&&num_mcells_def_outside>0)
	    first_pos_x = offset_x;
	  
	  //  if (flash->get_flash_id()==61&&main_cluster->get_cluster_id()==1)
	  // std::cout << num_mcells_outside << " " << main_cluster->get_num_mcells() << "  A " << (first_pos_x - offset_x)/units::cm << " " <<
	  //  (prev_pos_x-offset_x)/units::cm << " " << (current_pos_x-offset_x)/units::cm << " " << num_mcells_def_outside << std::endl;
	  
	}
	if (last_pos_x - offset_x >= high_x_cut + high_x_cut_ext1 &&
	    last_pos_x - offset_x < high_x_cut + 120*units::cm){
	  std::map<int,SMGCSet>& time_cells_set_map = main_cluster->get_time_cells_set_map();
	  int num_mcells_outside = 0;
	  int num_time_slices_outside = 0;
	  int num_mcells_def_outside = 0;
	  double prev_pos_x= last_pos_x;
	  double current_pos_x = last_pos_x;

	  for (auto it3 = time_cells_set_map.rbegin(); it3 != time_cells_set_map.rend(); it3++){
	    current_pos_x = (*(it3->second.begin()))->get_sampling_points().front().x;
	    if (current_pos_x -offset_x<high_x_cut + high_x_cut_ext1 && fabs(current_pos_x - prev_pos_x) > 0.75*units::cm)
	      break;
	    if (num_time_slices_outside > 60) break;


	    if (current_pos_x -offset_x>high_x_cut + high_x_cut_ext1) num_mcells_def_outside +=it3->second.size();
	    
	    num_time_slices_outside += 1;
	    num_mcells_outside += it3->second.size();
	    prev_pos_x = current_pos_x;
	  }
	  if (num_time_slices_outside <=36 && num_mcells_outside < 0.05*main_cluster->get_num_mcells()){
	    last_pos_x = current_pos_x;
	    if (num_time_slices_outside > 10 && fabs(current_pos_x - prev_pos_x)<10*units::cm)
	      flag_spec_end = true;
	  }else if (num_time_slices_outside <=60 && num_mcells_outside < 0.06*main_cluster->get_num_mcells() && fabs(current_pos_x - prev_pos_x)>10*units::cm){
	    last_pos_x = current_pos_x;
	  }else if (num_time_slices_outside <=25 && num_mcells_outside < 0.12 * main_cluster->get_num_mcells() && fabs(current_pos_x - prev_pos_x)>20*units::cm){
	    last_pos_x = current_pos_x;
	  }
	  
	  if (num_mcells_def_outside < 0.0015 * main_cluster->get_num_mcells()&&num_mcells_def_outside>0)
	    last_pos_x = offset_x+high_x_cut;

	  // if (flash->get_flash_id()==19&&main_cluster->get_cluster_id()==19)
	  //   std::cout << flash->get_flash_id() << " "<< main_cluster->get_cluster_id() << " " << (first_pos_x-offset_x)/units::cm << " " << (last_pos_x-offset_x)/units::cm << " " << num_time_slices_outside << " " << num_mcells_outside << " " << main_cluster->get_num_mcells() << " " << fabs(current_pos_x - prev_pos_x)/units::cm << std::endl;
	  
	}
	
	// if (flash->get_flash_id()==16 && main_cluster->get_cluster_id()==13 )
	//   std::cout << flash->get_flash_id() << " "<< main_cluster->get_cluster_id() << " " << (first_pos_x-offset_x)/units::cm << " " << (last_pos_x-offset_x)/units::cm << std::endl;

	//	if (flash->get_flash_id()==14)
	//      std::cout << flash->get_flash_id() << " " << main_cluster->get_cluster_id() << " " << offset_x/units::cm << " " << (first_pos_x-offset_x)/units::cm << " " << (last_pos_x-offset_x)/units::cm << " " << std::endl;
	
  	if (first_pos_x-offset_x > low_x_cut + low_x_cut_ext1 -1.0*units::cm &&
  	    last_pos_x-offset_x > low_x_cut &&
  	    last_pos_x-offset_x < high_x_cut + high_x_cut_ext1 &&
  	    first_pos_x-offset_x < high_x_cut){
	  
	  bundle->set_spec_end_flag(flag_spec_end);
	  
  	  if (first_pos_x-offset_x <=low_x_cut + low_x_cut_ext2 && first_pos_x-offset_x > low_x_cut + low_x_cut_ext1 - 1.0*units::cm ){
  	    // bundle->set_flag_close_to_PMT(true);
  	  //   bundle->set_flag_at_x_boundary(true);
  	  // }else if (first_pos_x-offset_x <= low_x_cut + low_x_cut_ext1 && first_pos_x-offset_x > low_x_cut + low_x_cut_ext1 -1.0*units::cm){
	    bundle->set_flag_close_to_PMT(true);
  	    bundle->set_flag_at_x_boundary(true);
	  }
  	  if (last_pos_x-offset_x >= high_x_cut + high_x_cut_ext2 && last_pos_x-offset_x < high_x_cut + high_x_cut_ext1){
  	    bundle->set_flag_at_x_boundary(true);
  	  }

  	  PR3DClusterSelection temp_clusters;
  	  temp_clusters.push_back(main_cluster);
  	  for (auto it3 = additional_clusters.begin(); it3!=additional_clusters.end(); it3++){
  	    temp_clusters.push_back(it3->first);
  	    other_clusters.push_back(it3->first);
  	  }

  	  for (auto it3 = temp_clusters.begin(); it3!=temp_clusters.end(); it3++){
  	    SMGCSelection& mcells = (*it3)->get_mcells();
  	    bool flag_save = true;

	    if ((*it3) == main_cluster)  flag_save = false;
	    
  	    for (auto it4 = mcells.begin(); it4!=mcells.end(); it4++){
  	      SlimMergeGeomCell *mcell = (*it4);
  	      if (mcell->get_q()>0){
  		PointVector& pts = mcell->get_sampling_points();
  		if (pts.at(0).x-offset_x < low_x_cut+low_x_cut_ext1 ||
  		    pts.at(0).x-offset_x > high_x_cut+high_x_cut_ext1){
  		  flag_save = false;
  		  continue;
  		}
		
  		float charge = mcell->get_q()/pts.size();
  		Point p;
  		for (size_t i=0;i!=pts.size();i++){
  		  p.x = pts.at(i).x - offset_x;
  		  p.y = pts.at(i).y;
  		  p.z = pts.at(i).z;
		  
		  int voxel_id = WCP2dToy::convert_xyz_voxel_id(p);

		  std::list<std::pair<int,float>> pmt_list;
		  for (int jj=0;jj!=32;jj++){
		    TVector3 vec(p.y/units::cm - pmt_pos_map[jj].at(1), p.z/units::cm - pmt_pos_map[jj].at(2), p.x/units::cm - pmt_pos_map[jj].at(0));
		    double R = vec.Mag();
		    double costheta = vec.CosTheta();
		    double phi = vec.Phi();
		    pmt_list.push_back(std::make_pair(jj,(g1->Eval(R)+g2->Eval(R)*costheta) * g3->Eval(R)));
		  }
		  //std::list<std::pair<int,float>>& pmt_list = photon_library.at(voxel_id);
		  
		  
		  for (auto it5 = pmt_list.begin(); it5!=pmt_list.end(); it5++){
		    pred_pmt_light.at(map_lib_pmt[it5->first]) += charge * it5->second;
		  }
		}
  	      }
  	    }
  	    if (flag_save)
  	      more_clusters.push_back(*it3);
	    
  	  } // loop over all clusters within this TPC object...

  	  double sum1 = 0, sum2 = 0, max_pe = 0;
  	  for (size_t i=0;i!=32;i++){
  	    pred_pmt_light.at(i) *= scaling_light_mag;

  	    sum1 += flash->get_PE(i);
  	    sum2 += pred_pmt_light.at(i);
  	    if (pred_pmt_light.at(i) > max_pe)
  	      max_pe = pred_pmt_light.at(i);
  	  }
	  
	  // if (sum2 < sum1 * 3){ // three times allowrance ... 
	  flag_good_bundle = true;
	  // }
  	}

  	if (flag_good_bundle){
  	  all_bundles.insert(bundle);
  	  if (flash_bundles_map.find(flash)==flash_bundles_map.end()){
  	    FlashTPCBundleSelection bundles;
  	    bundles.push_back(bundle);
  	    flash_bundles_map[flash] = bundles;
  	  }else{
  	    flash_bundles_map[flash].push_back(bundle);
  	  }
  	  if (cluster_bundles_map.find(main_cluster)==cluster_bundles_map.end()){
  	    FlashTPCBundleSelection bundles;
  	    bundles.push_back(bundle);
  	    cluster_bundles_map[main_cluster] = bundles;
  	  }else{
  	    cluster_bundles_map[main_cluster].push_back(bundle);
  	  }
  	  fc_bundles_map[std::make_pair(flash,main_cluster)] = bundle;

	  // if (flash->get_flash_id()==14)
	  //   std::cout << flash->get_flash_id() << " " << main_cluster->get_cluster_id() << " " << bundle << std::endl;
	  
  	}else{
  	  delete bundle;
  	}
	cluster_index_id++;
      }
      flash_index_id++;
    }

    
    // examine the bundles ... 
    //std::cout << "Starting: " << cluster_bundles_map.size() << " A " << flash_bundles_map.size() << " " << all_bundles.size() << std::endl;
    {
      FlashTPCBundleSelection to_be_removed;
      for (auto it = all_bundles.begin(); it!=all_bundles.end();it++){
	FlashTPCBundle *bundle = *it;
	if (!bundle->examine_bundle(cos_pe_low, cos_pe_mid)){
	  to_be_removed.push_back(bundle);
	}
      }
      
      for (auto it1 = to_be_removed.begin(); it1!=to_be_removed.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	all_bundles.erase(bundle);
	
	Opflash *flash = bundle->get_flash();
	PR3DCluster *cluster = bundle->get_main_cluster();
	
	fc_bundles_map.erase(std::make_pair(flash,cluster));
	
	{
	  FlashTPCBundleSelection& temp_bundles = flash_bundles_map[flash];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    flash_bundles_map.erase(flash);
	}
	{
	  FlashTPCBundleSelection& temp_bundles = cluster_bundles_map[cluster];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    cluster_bundles_map.erase(cluster);
	}
	
	delete bundle;
      }
      to_be_removed.clear();


      for (auto it = cluster_bundles_map.begin(); it!= cluster_bundles_map.end(); it++){
       	PR3DCluster *main_cluster = it->first;
       	FlashTPCBundleSelection& bundles = it->second;
       	bool flag_consist = false;
	FlashTPCBundleSelection temp_removed;
	
       	for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if (bundle->get_potential_bad_match_flag())
	    temp_removed.push_back(bundle);
	  if (bundle->get_consistent_flag() ||
	       (bundle->get_ks_dis() < 0.12 || bundle->get_chi2() < 3 * bundle->get_ndf() )&& bundle->get_ndf()>=3 ||
	       bundle->get_ks_dis()<0.33 && bundle->get_chi2() < 50 * bundle->get_ndf()&&bundle->get_ndf()>=5 && bundle->get_flag_close_to_PMT())
	    flag_consist = true;

	  // if (main_cluster->get_cluster_id()==30)
	  //   std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << " " << bundle->get_consistent_flag() << " " << bundle->get_flag_close_to_PMT() << " " << bundle->get_potential_bad_match_flag() << " " << flag_consist << std::endl;
	  
	}
	if (flag_consist)
	  to_be_removed.insert(to_be_removed.end(),temp_removed.begin(),temp_removed.end());
      }

      
      for (auto it1 = to_be_removed.begin(); it1!=to_be_removed.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	all_bundles.erase(bundle);
	
	Opflash *flash = bundle->get_flash();
	PR3DCluster *cluster = bundle->get_main_cluster();
	
	fc_bundles_map.erase(std::make_pair(flash,cluster));
	
	{
	  FlashTPCBundleSelection& temp_bundles = flash_bundles_map[flash];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    flash_bundles_map.erase(flash);
	}
	{
	  FlashTPCBundleSelection& temp_bundles = cluster_bundles_map[cluster];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    cluster_bundles_map.erase(cluster);
	}
	
	delete bundle;
      }
      
      
    }

    

    
    //    std::cout << "After Cleaning 1 : " << cluster_bundles_map.size() << " A " << flash_bundles_map.size() << " " << all_bundles.size() << std::endl;
    
    for (auto it = cluster_bundles_map.begin(); it!= cluster_bundles_map.end(); it++){
      PR3DCluster *main_cluster = it->first;
      FlashTPCBundleSelection& bundles = it->second;
      bool flag_tight_bundle = false;

      bool flag_highly_consistent_bundle = false;

      
      for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	FlashTPCBundle *bundle = *it1;

	//	std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << " " << bundle->get_consistent_flag() << " " << bundle->get_flag_close_to_PMT() << " " << bundle->get_potential_bad_match_flag() << std::endl;
	
	if (bundle->get_consistent_flag()){
	  flag_tight_bundle = true;
	  
	  if (bundle->get_ks_dis()<0.05 && bundle->get_ndf() >= 10 && bundle->get_chi2() < bundle->get_ndf()  * 12  ||
	      bundle->get_ks_dis()<0.07 && bundle->get_ndf() >= 10 && bundle->get_chi2() < bundle->get_ndf()  * 9  ||
	      bundle->get_ks_dis()<0.1 && bundle->get_ndf() >=5 && bundle->get_chi2() < bundle->get_ndf()  * 4  ||
	      bundle->get_ks_dis()<0.15 && bundle->get_ndf()>=5 && bundle->get_chi2() < bundle->get_ndf()  * 3 	      )
	    flag_highly_consistent_bundle = true;
	  //  break;
	}
      }
      
      if (!flag_tight_bundle){
	for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if (bundle->get_ks_dis()<0.07 && bundle->get_ndf()>=10 && bundle->get_chi2() < bundle->get_ndf() * 60){
	    bundle->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (bundle->get_ks_dis()<0.33 && bundle->get_ndf()>=3 && bundle->get_chi2() < bundle->get_ndf() * 10){
	    bundle->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (bundle->get_ks_dis()<0.33 && bundle->get_ndf()>=3 && (bundle->get_chi2() < bundle->get_ndf() * 50 && bundle->get_flag_close_to_PMT() || bundle->get_chi2() < bundle->get_ndf() * 16 && bundle->get_flag_at_x_boundary())){
	    bundle->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (bundle->get_ks_dis()<0.22 && bundle->get_ndf()>=3 && bundle->get_chi2() < bundle->get_ndf() * 16){
	    bundle->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (bundle->get_ks_dis()<0.16 && bundle->get_ndf()>=6 && bundle->get_chi2() < bundle->get_ndf() * 20){
	    bundle->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }
	}

	
	FlashTPCBundle *min_bundle = *bundles.begin();
	
	for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if (bundle->get_ks_dis()<min_bundle->get_ks_dis()){
	    min_bundle = bundle;
	  }
	}
	
	FlashTPCBundle *min_bundle1 = 0;
	for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if (bundle==min_bundle) continue;
	  if (min_bundle1==0) {
	    min_bundle1 = bundle;
	  }else if (bundle->get_ks_dis()<min_bundle1->get_ks_dis()){
	    min_bundle1 = bundle;
	  }
	}
	
	
	// if (main_cluster->get_cluster_id()==12)
	//   std::cout << "Xin: " << min_bundle->get_flash()->get_flash_id() << " " << min_bundle1->get_flash()->get_flash_id() << std::endl;
	

	bool flag_set = false;
	if (min_bundle->get_ks_dis()<0.15 && min_bundle->get_ndf()>=6 && min_bundle->get_chi2() < min_bundle->get_ndf() * 40){
	  min_bundle->set_consistent_flag(true);
	  flag_tight_bundle = true;
	  flag_set = true;
	}else if ( min_bundle->get_ks_dis()<0.11 && min_bundle->get_ndf() >= 3 && min_bundle->get_chi2() < min_bundle->get_ndf() * 36){
	  min_bundle->set_consistent_flag(true);
	  flag_tight_bundle = true;
	  flag_set = true;
	}else if (min_bundle->get_ks_dis()<0.075 && min_bundle->get_ndf()>=10 && min_bundle->get_chi2() < min_bundle->get_ndf() * 60){
	  min_bundle->set_consistent_flag(true);
	  flag_tight_bundle = true;
	  flag_set = true;
	}else if (min_bundle->get_ks_dis()<0.11 && min_bundle->get_ndf()>=10 && min_bundle->get_chi2() < min_bundle->get_ndf() * 120){
	  min_bundle->set_consistent_flag(true);
	  flag_tight_bundle = true;
	  flag_set = true;
	}else if (min_bundle->get_ks_dis()<0.17 && min_bundle->get_ndf()>=10 && min_bundle->get_chi2() < min_bundle->get_ndf() * 40){
	  min_bundle->set_consistent_flag(true);
	  flag_tight_bundle = true;
	  flag_set = true;
	}else if (min_bundle->get_ks_dis()<0.22 && min_bundle->get_ndf()>=20 && min_bundle->get_chi2() < min_bundle->get_ndf() * 40){
	  min_bundle->set_consistent_flag(true);
	  flag_tight_bundle = true;
	  flag_set = true;
	}else if (min_bundle->get_flag_at_x_boundary() && min_bundle->get_ks_dis()<0.3 && min_bundle->get_ndf()>=10 && min_bundle->get_chi2() < min_bundle->get_ndf() * 30){
	  min_bundle->set_consistent_flag(true);
	  flag_tight_bundle = true;
	  flag_set = true;
	}
	
	if (!flag_set && min_bundle1!=0){
	  if (min_bundle1->get_ks_dis()<0.15 && min_bundle1->get_ndf()>=6 && min_bundle1->get_chi2() < min_bundle1->get_ndf() * 40){
	    min_bundle1->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if ( min_bundle1->get_ks_dis()<0.11 && min_bundle1->get_ndf() >= 3 && min_bundle1->get_chi2() < min_bundle1->get_ndf() * 36){
	    min_bundle1->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (min_bundle1->get_ks_dis()<0.075 && min_bundle1->get_ndf()>=10 && min_bundle1->get_chi2() < min_bundle1->get_ndf() * 60){
	    min_bundle1->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (min_bundle1->get_ks_dis()<0.11 && min_bundle1->get_ndf()>=10 && min_bundle1->get_chi2() < min_bundle1->get_ndf() * 120){
	    min_bundle1->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (min_bundle1->get_ks_dis()<0.17 && min_bundle1->get_ndf()>=10 && min_bundle1->get_chi2() < min_bundle1->get_ndf() * 40){
	    min_bundle1->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (min_bundle1->get_ks_dis()<0.22 && min_bundle1->get_ndf()>=20 && min_bundle1->get_chi2() < min_bundle1->get_ndf() * 40){
	    min_bundle1->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }else if (min_bundle1->get_flag_at_x_boundary() && min_bundle1->get_ks_dis()<0.3 && min_bundle1->get_ndf()>=10 && min_bundle1->get_chi2() < min_bundle1->get_ndf() * 30){
	    min_bundle1->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }
	  
	  //else if (min_bundle1->get_ks_dis()<0.17 && min_bundle1->get_ndf()>=10 && min_bundle1->get_chi2() < min_bundle1->get_ndf() * 40){
	  // min_bundle1->set_consistent_flag(true);
	  // flag_tight_bundle = true;
	  //}
	}
      }

      
       if (!flag_tight_bundle){
	// last pieces ... 
	for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if (bundle->get_flag_close_to_PMT() && bundle->get_ks_dis()<0.7 && bundle->get_ndf()>5 && bundle->get_chi2() < bundle->get_ndf() * 40){
	    bundle->set_consistent_flag(true);
	    flag_tight_bundle = true;
	  }
	}
      }
     


      if (flag_tight_bundle){
	if (flag_highly_consistent_bundle){
	  for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	    FlashTPCBundle *bundle = *it1;
	    if (bundle->get_consistent_flag()){
	      if (bundle->get_ks_dis()<0.15 && bundle->get_ndf() >=5 && bundle->get_chi2() < bundle->get_ndf()  * 9 ){
	      }else if (bundle->get_ks_dis()<0.075 && bundle->get_ndf() >=8 && bundle->get_chi2() < bundle->get_ndf()  * 12){
	      }else if (bundle->get_ks_dis()<0.07 && bundle->get_ndf() >= 16 && bundle->get_chi2() < bundle->get_ndf()  * 20){
	      }else{
		bundle->set_consistent_flag(false);
	      }
	      
	      
	    }
	  }

	 
	  
	  
	  
	}

	FlashTPCBundleSelection ndf1_bundles;
	FlashTPCBundleSelection other_bundles;
	for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if (bundle->get_consistent_flag()){
	    if (bundle->get_ndf()==1){
	      ndf1_bundles.push_back(bundle);
	      	}else{
	      other_bundles.push_back(bundle);
	    }
	  }
	}

	// now remove ... 
	bool flag_temp = false;
	// if (main_cluster->get_cluster_id()==15)
	//   std::cout << "Xin: " << other_bundles.size() << " " << ndf1_bundles.size() << std::endl;
	
	for (auto it1 = other_bundles.begin(); it1!=other_bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if (flash_bundles_map.find(bundle->get_flash())!=flash_bundles_map.end()){
	    FlashTPCBundleSelection bundles1 = flash_bundles_map[bundle->get_flash()];
	    int temp_num = 0;
	    for (auto it2 = bundles1.begin(); it2!=bundles1.end(); it2++){
	      if ((*it2)->get_consistent_flag())
		temp_num++;
	    }
	    // if (bundle->get_main_cluster()->get_cluster_id()==15)
	    //   std::cout << bundle->get_flash()->get_flash_id() << " " << temp_num << std::endl;
	    
	    if (temp_num==1){
	      flag_temp = true;
	      break;
	    }
	    
	  }
	}
	
	if (flag_temp){
	  for (auto it1 = ndf1_bundles.begin(); it1!=ndf1_bundles.end(); it1++){
	    FlashTPCBundle *bundle = *it1;
	    bundle->set_consistent_flag(false);
	  }
	}
	
	
      }
    }

    //
    for (auto it = cluster_bundles_map.begin(); it!= cluster_bundles_map.end(); it++){
      PR3DCluster *main_cluster = it->first;
      FlashTPCBundleSelection& bundles = it->second;

      FlashTPCBundleSelection temp_bundles;
      bool flag_remove = false;

      for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	if (bundle->get_consistent_flag() && bundle->get_spec_end_flag()){
	  temp_bundles.push_back(bundle);
	}else if (bundle->get_consistent_flag()){
	  flag_remove = true;
	}
      }
      
      if (flag_remove){
	for (auto it1 = temp_bundles.begin(); it1!=temp_bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  bundle->set_consistent_flag(false);
	}
      }

      // examine the again
      flag_remove = false;
      temp_bundles.clear();
      for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	if (bundle->get_consistent_flag()){
	  if (bundle->get_ks_dis() < 0.06 && bundle->get_chi2() < 3.*bundle->get_ndf() && bundle->get_flag_at_x_boundary()){
	    flag_remove = true;
	  }else{
	    temp_bundles.push_back(bundle);
	  }
	}
      }
      if (flag_remove){
	for (auto it1 = temp_bundles.begin(); it1!=temp_bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  bundle->set_consistent_flag(false);
	}
      }
      
    }
    
    //  std::cout << "After Cleaning 1 : " << cluster_bundles_map.size() << " A " << flash_bundles_map.size() << " " << all_bundles.size() << std::endl;


    // // std::cout << std::endl << std::endl;
    // for (auto it = all_bundles.begin(); it!=all_bundles.end();it++){
    //   FlashTPCBundle *bundle = *it;
      
    //   //  if (bundle->get_flash()->get_flash_id()==29 && bundle->get_main_cluster()->get_cluster_id()==19)
    //   //	bundle->set_consistent_flag(1);
    //   //   if ( bundle->get_consistent_flag() || bundle->get_flag_at_x_boundary())
    //   std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << " " << bundle->get_consistent_flag()  << std::endl;

      
    // }
    // std::cout << std::endl << std::endl;

    

    // further examine the map ...
    {
      Flash_bundles_map flash_good_bundles_map;
      Cluster_bundles_map cluster_good_bundles_map;
      FlashTPCBundleSelection good_bundles;
            
      Flash_bundles_map flash_other_bundles_map;
      Cluster_bundles_map cluster_other_bundles_map;
      
      for (auto it = all_bundles.begin(); it!=all_bundles.end();it++){
	FlashTPCBundle *bundle = *it;
	Opflash *flash = bundle->get_flash();
	PR3DCluster *cluster = bundle->get_main_cluster();

	if (bundle->get_consistent_flag()){
	  if (flash_good_bundles_map.find(flash)==flash_good_bundles_map.end()){
	    FlashTPCBundleSelection bundles;
	    bundles.push_back(bundle);
	    flash_good_bundles_map[flash] = bundles;
	  }else{
	    flash_good_bundles_map[flash].push_back(bundle);
	  }
	  if (cluster_good_bundles_map.find(cluster)==cluster_good_bundles_map.end()){
	    FlashTPCBundleSelection bundles;
	    bundles.push_back(bundle);
	    cluster_good_bundles_map[cluster] = bundles;
	  }else{
	    cluster_good_bundles_map[cluster].push_back(bundle);
	  }
	  good_bundles.push_back(bundle);
	}else if (bundle->get_flag_at_x_boundary()){
	  if (flash_other_bundles_map.find(flash)==flash_other_bundles_map.end()){
	    FlashTPCBundleSelection bundles;
	    bundles.push_back(bundle);
	    flash_other_bundles_map[flash] = bundles;
	  }else{
	    flash_other_bundles_map[flash].push_back(bundle);
	  }
	  if (cluster_other_bundles_map.find(cluster)==cluster_other_bundles_map.end()){
	    FlashTPCBundleSelection bundles;
	    bundles.push_back(bundle);
	    cluster_other_bundles_map[cluster] = bundles;
	    }else{
	    cluster_other_bundles_map[cluster].push_back(bundle);
	  }
	}
	// if ( bundle->get_consistent_flag() || bundle->get_flag_at_x_boundary())
	// 	std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << std::endl;
      }

      for (auto it = cluster_good_bundles_map.begin(); it!= cluster_good_bundles_map.end(); it++){
	PR3DCluster *cluster =  it->first;
	FlashTPCBundleSelection& bundles = it->second;

	
	if (bundles.size()>1){ // more than one flash

	  // find the min bundle ... 
	  FlashTPCBundle *min_bundle = bundles.at(0);
	  for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	    FlashTPCBundle *bundle = *it1;
	    if (bundle->get_ks_dis()+0.003 * flash_good_bundles_map[bundle->get_flash()].size() < min_bundle->get_ks_dis() + 0.003 * flash_good_bundles_map[min_bundle->get_flash()].size())
	      min_bundle = bundle;
	  }

	  if (min_bundle->get_ks_dis() < 0.18){
	    //std::cout << min_bundle->get_flash()->get_flash_id() << " Xin: " << min_bundle->get_main_cluster()->get_cluster_id() << std::endl;

	    FlashTPCBundleSelection temp_bundles;
	    
	    //examine the rest of bundles;
	    for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	      FlashTPCBundle *bundle = *it1;
	      if (bundle==min_bundle) continue;
	      bool flag_remove = false;
	      
	      if (min_bundle->get_ks_dis() +0.015 < bundle->get_ks_dis() &&
		  min_bundle->get_chi2()/min_bundle->get_ndf() *1.1 < bundle->get_chi2()/bundle->get_ndf()){
		// prepare remove bundle from the list ...
		flag_remove = true;
	      }else if(min_bundle->get_ks_dis() < bundle->get_ks_dis() &&
		       min_bundle->get_chi2()/min_bundle->get_ndf() * 3 < bundle->get_chi2()/bundle->get_ndf()){
		flag_remove = true;
	      }else if (min_bundle->get_ks_dis() +0.025 < bundle->get_ks_dis() &&
			min_bundle->get_chi2()/min_bundle->get_ndf() *0.85 < bundle->get_chi2()/bundle->get_ndf()){
		flag_remove = true;
		if (bundle->get_ks_dis() < 0.075&&bundle->get_chi2()/bundle->get_ndf()<1.6)
		  flag_remove = false;
	      }else if (min_bundle->get_ks_dis() +0.03 < bundle->get_ks_dis() &&
			min_bundle->get_chi2()/min_bundle->get_ndf() *0.75 < bundle->get_chi2()/bundle->get_ndf() &&
			min_bundle->get_ks_dis() <0.06){
		flag_remove = true;
	      }else if (min_bundle->get_ks_dis()+0.075 < bundle->get_ks_dis() &&
			min_bundle->get_ks_dis()<0.075 &&
			min_bundle->get_chi2()/min_bundle->get_ndf() /15. < bundle->get_chi2()/bundle->get_ndf()){
		flag_remove = true;
	      }else if (flash_good_bundles_map[min_bundle->get_flash()].size()==1){
		if (min_bundle->get_ks_dis()<0.06){
		  if (min_bundle->get_chi2()/min_bundle->get_ndf() < bundle->get_chi2()/bundle->get_ndf() * 2.2){
		    flag_remove = true;
		  }else if (min_bundle->get_ks_dis() + 0.06 < bundle->get_ks_dis() &&
			    min_bundle->get_chi2()/min_bundle->get_ndf() < bundle->get_chi2()/bundle->get_ndf() * 3.0){
		    flag_remove = true;
		  }else if (min_bundle->get_chi2()/min_bundle->get_ndf()<9 && min_bundle->get_ks_dis() + 0.01 < bundle->get_ks_dis()){
		    flag_remove = true;
		  }else if (min_bundle->get_ks_dis()<0.05 && min_bundle->get_chi2()/min_bundle->get_ndf()<38 && min_bundle->get_ks_dis() + 0.05 < bundle->get_ks_dis()){
		    flag_remove = true;
		  }
		}else{
		  if (min_bundle->get_ks_dis() + 0.015 < bundle->get_ks_dis() &&
		      min_bundle->get_chi2()/min_bundle->get_ndf()*0.75 < bundle->get_chi2()/bundle->get_ndf() &&flash_good_bundles_map[bundle->get_flash()].size()>1
		      ){
		    flag_remove = true;
		  }else if (min_bundle->get_ks_dis() + 0.08 < bundle->get_ks_dis() &&
			    flash_good_bundles_map[bundle->get_flash()].size()>1 &&
			    min_bundle->get_chi2()/min_bundle->get_ndf() < 25
			    ){
		    flag_remove = true;
		  }
		}
	      }
	      
	      // if (min_bundle->get_main_cluster()->get_cluster_id()==23){
	      // 	std::cout << min_bundle->get_flash()->get_flash_id() << " A " << min_bundle->get_main_cluster()->get_cluster_id() << " " << flag_remove << " " << flash_good_bundles_map[min_bundle->get_flash()].size()<< std::endl;
	      // 	//flag_remove = true;
	      // }
	      
	      if (flag_remove){
		if (flash_good_bundles_map[bundle->get_flash()].size()>1){
		  
		  FlashTPCBundle *min_bundle1 = flash_good_bundles_map[bundle->get_flash()].at(0);
		  FlashTPCBundle *min_bundle2 = flash_good_bundles_map[bundle->get_flash()].at(0);
		  for (auto it2 = flash_good_bundles_map[bundle->get_flash()].begin(); it2!=flash_good_bundles_map[bundle->get_flash()].end(); it2++){
		    FlashTPCBundle *bundle1 = *it2;
		    if (bundle1->get_ks_dis() < min_bundle1->get_ks_dis()){
		      min_bundle1 = bundle1;
		      min_bundle2 = min_bundle1;
		    }
		  }
		  if (bundle!=min_bundle1){
		    temp_bundles.push_back(bundle);
		  }else{
		    if (min_bundle1->get_ks_dis() + 0.01 > min_bundle2->get_ks_dis() &&
			min_bundle1->get_chi2() > min_bundle2->get_chi2()*0.85){
		      temp_bundles.push_back(bundle);
		    }else if (min_bundle1->get_chi2()/min_bundle1->get_ndf() > min_bundle2->get_chi2()/min_bundle2->get_ndf() * 3 &&
			      min_bundle1->get_ndf()>=5 && 
			      min_bundle2->get_ks_dis() < 0.12){
		      temp_bundles.push_back(bundle);
		    }
		  }
		}
	      }
	    }
	  

	    for (auto it1 = temp_bundles.begin(); it1!=temp_bundles.end(); it1++){
	      FlashTPCBundle *bundle = *it1;
	      PR3DCluster *cluster =  bundle->get_main_cluster();
	      Opflash *flash = bundle->get_flash();
	      
	      bundle->set_consistent_flag(false);
	      flash_good_bundles_map[flash].erase(find(flash_good_bundles_map[flash].begin(),flash_good_bundles_map[flash].end(),bundle));
	      cluster_good_bundles_map[cluster].erase(find(cluster_good_bundles_map[cluster].begin(),cluster_good_bundles_map[cluster].end(),bundle));
	      // remove from 
	    }
	  }
	}
      }


      //   std::cout << "After Cleaning 1 : " << cluster_bundles_map.size() << " A " << flash_bundles_map.size() << " " << all_bundles.size() << std::endl;


    // // std::cout << std::endl << std::endl;
    // for (auto it = all_bundles.begin(); it!=all_bundles.end();it++){
    //   FlashTPCBundle *bundle = *it;
      
    //   //  if (bundle->get_flash()->get_flash_id()==29 && bundle->get_main_cluster()->get_cluster_id()==19)
    //   //	bundle->set_consistent_flag(1);
    //   //   if ( bundle->get_consistent_flag() || bundle->get_flag_at_x_boundary())
    //   std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << " " << bundle->get_consistent_flag()  << std::endl;

      
    // }
    // std::cout << std::endl << std::endl;
      

      // new round according to chi2 .. .
      for (auto it = cluster_good_bundles_map.begin(); it!= cluster_good_bundles_map.end(); it++){
	PR3DCluster *cluster =  it->first;
	FlashTPCBundleSelection& bundles = it->second;

	
	if (bundles.size()>1){ // more than one flash
	  // find the min bundle ... 
	  FlashTPCBundle *min_bundle = bundles.at(0);
	  for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	    FlashTPCBundle *bundle = *it1;
	    if (bundle->get_chi2()/bundle->get_ndf() < min_bundle->get_chi2()/min_bundle->get_ndf())
	      min_bundle = bundle;
	  }

	  if (min_bundle->get_ks_dis() < 0.15){
	    FlashTPCBundleSelection temp_bundles;
	    
	    //examine the rest of bundles;
	    for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	      FlashTPCBundle *bundle = *it1;
	      if (bundle==min_bundle) continue;
	      bool flag_remove = false;
	      
	      if (flash_good_bundles_map[min_bundle->get_flash()].size()==1){
		if (min_bundle->get_ks_dis()<0.08){
		  if (min_bundle->get_chi2()/min_bundle->get_ndf() *1.1 < bundle->get_chi2()/bundle->get_ndf() ){
		    flag_remove = true;
		  }
		}else if (min_bundle->get_ks_dis()<0.1 && min_bundle->get_chi2()/min_bundle->get_ndf() *1.33< bundle->get_chi2()/bundle->get_ndf()){
		  flag_remove = true;
		}else if (min_bundle->get_ks_dis()<0.15 && min_bundle->get_chi2()/min_bundle->get_ndf() * 3 < bundle->get_chi2()/bundle->get_ndf()){
		  flag_remove = true;
		}
	      }else if (flash_good_bundles_map[min_bundle->get_flash()].size()>=2){
		if (min_bundle->get_ks_dis()<0.1 && min_bundle->get_chi2()/min_bundle->get_ndf() * 6 < bundle->get_chi2()/bundle->get_ndf()){
		  flag_remove = true;
		}
	      }

	      // if (min_bundle->get_main_cluster()->get_cluster_id()==2){
	      // 	std::cout << min_bundle->get_flash()->get_flash_id() << " A " << min_bundle->get_main_cluster()->get_cluster_id() << " " << flag_remove << " " << flash_good_bundles_map[min_bundle->get_flash()].size()<< " " << flash_good_bundles_map[bundle->get_flash()].size() << " " << bundle->get_flash()->get_flash_id() << std::endl;
	      // }
	      
	      
	      if (flag_remove){
		if (flash_good_bundles_map[bundle->get_flash()].size()>flash_good_bundles_map[min_bundle->get_flash()].size() && flash_good_bundles_map[min_bundle->get_flash()].size()<=2){
		  if (bundle!=min_bundle){
		    temp_bundles.push_back(bundle);
		  }
		}else{
		  if (bundle!=min_bundle){
		    if (min_bundle->get_ks_dis() < 0.1){
		      if (min_bundle->get_ks_dis() * pow(min_bundle->get_chi2()/min_bundle->get_ndf(),0.8) < 2.0 * bundle->get_ks_dis() * pow(bundle->get_chi2()/bundle->get_ndf(),0.8) && flash_good_bundles_map[bundle->get_flash()].size() > 1){
		  	temp_bundles.push_back(bundle);
		      }
		    }// else{
		    //   if (min_bundle->get_ks_dis() * pow(min_bundle->get_chi2()/min_bundle->get_ndf(),0.8) < 3.33 * bundle->get_ks_dis() * pow(bundle->get_chi2()/bundle->get_ndf(),0.8)){
		    // 	temp_bundles.push_back(bundle);
		    //   }
		    // }
		  }
		  // if (min_bundle->get_main_cluster()->get_cluster_id()==2)
		  //   std::cout << min_bundle->get_ks_dis() * pow(min_bundle->get_chi2()/min_bundle->get_ndf(),0.8) << " " << bundle->get_ks_dis() * pow(bundle->get_chi2()/bundle->get_ndf(),0.8) << std::endl;
		}
	      }
	    }
	  

	    for (auto it1 = temp_bundles.begin(); it1!=temp_bundles.end(); it1++){
	      FlashTPCBundle *bundle = *it1;
	      PR3DCluster *cluster =  bundle->get_main_cluster();
	      Opflash *flash = bundle->get_flash();
	      
	      bundle->set_consistent_flag(false);
	      flash_good_bundles_map[flash].erase(find(flash_good_bundles_map[flash].begin(),flash_good_bundles_map[flash].end(),bundle));
	      cluster_good_bundles_map[cluster].erase(find(cluster_good_bundles_map[cluster].begin(),cluster_good_bundles_map[cluster].end(),bundle));
	      // remove from 
	    }
	  }
	}
      }
      //finish chi2 ...
      
      

      //

      
      FlashTPCBundleSelection to_be_removed;
      for (auto it = cluster_good_bundles_map.begin(); it!= cluster_good_bundles_map.end(); it++){
	PR3DCluster *cluster =  it->first;
	FlashTPCBundleSelection& bundles = it->second;
	bool flag_remove_other = false;
	for (auto it1 = bundles.begin(); it1!= bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  Opflash *flash = bundle->get_flash();
	  
	  if (flash_good_bundles_map[flash].size()==1){
	    flag_remove_other = true;
	    break;
	  }
	}
	if (flag_remove_other){
	  if (cluster_other_bundles_map.find(cluster)!=cluster_other_bundles_map.end()){
	    std::copy(cluster_other_bundles_map[cluster].begin(),
		      cluster_other_bundles_map[cluster].end(),
		      std::back_inserter(to_be_removed));
	  }
	}else{
	  if (cluster_other_bundles_map.find(cluster)!=cluster_other_bundles_map.end()){
	    for (auto it1 = cluster_other_bundles_map[cluster].begin(); it1!= cluster_other_bundles_map[cluster].end(); it1++){
	      FlashTPCBundle *bundle1 = *it1;
	      if (bundle1->get_chi2() > bundle1->get_ndf()*25 && (!bundle1->get_flag_close_to_PMT())){
		to_be_removed.push_back(bundle1);
	      }
	    }
	  }
	}
	
      }

      for (auto it1 = to_be_removed.begin(); it1!=to_be_removed.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	all_bundles.erase(bundle);
	
	Opflash *flash = bundle->get_flash();
	PR3DCluster *cluster = bundle->get_main_cluster();
	
	fc_bundles_map.erase(std::make_pair(flash,cluster));
	
	//	bundles.erase(find(bundles.begin(),bundles.end(),bundle));
	{
	  FlashTPCBundleSelection& temp_bundles = cluster_bundles_map[cluster];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    cluster_bundles_map.erase(cluster);
	}

	{
	  FlashTPCBundleSelection& temp_bundles = flash_bundles_map[flash];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    flash_bundles_map.erase(flash);
	}
	
	delete bundle;
      }
      
    }

    for (auto it = cluster_bundles_map.begin(); it!= cluster_bundles_map.end(); it++){
      PR3DCluster *main_cluster = it->first;
      FlashTPCBundleSelection& bundles = it->second;
      bool flag_tight_bundle = false;

      for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	if (bundle->get_consistent_flag()){
	  flag_tight_bundle = true;
	  break;
	}
      }

      // if (main_cluster->get_cluster_id()==5)
      // 	std::cout << "Xin: " << flag_tight_bundle << " " << bundles.size() << std::endl;
      
      
      // clean up the map ...
      if (flag_tight_bundle){
	// all_bundles;
	// flash_bundles_map;
	// cluster_bundles_map;
	// std::map<std::pair<Opflash*,PR3DCluster*>,FlashTPCBundle*> fc_bundles_map;
	FlashTPCBundleSelection to_be_removed;
	for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if ((!bundle->get_consistent_flag()) && (!bundle->get_flag_at_x_boundary())){
	    if (bundle->get_ks_dis() < 0.06 && bundle->get_chi2() < 60*bundle->get_ndf() &&
	    	bundle->get_flash()->get_total_PE() > 100 && bundle->get_flash()->get_type()==2 &&
	    	bundle->get_total_pred_light() > 30){
	    }else{
	      to_be_removed.push_back(bundle);
	    }
	  }
	  //	  std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << std::endl;
	}
	for (auto it1 = to_be_removed.begin(); it1!=to_be_removed.end(); it1++){
	  FlashTPCBundle *bundle = *it1;

	  // std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << std::endl;
	   
	  all_bundles.erase(bundle);
	  
	  Opflash *flash = bundle->get_flash();
	  PR3DCluster *cluster = bundle->get_main_cluster();
	  
	  fc_bundles_map.erase(std::make_pair(flash,cluster));

	  bundles.erase(find(bundles.begin(),bundles.end(),bundle));

	  FlashTPCBundleSelection& temp_bundles = flash_bundles_map[flash];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    flash_bundles_map.erase(flash);
	  delete bundle;
	}
      }
    }


    // additional check
    for (auto it = flash_bundles_map.begin(); it!=flash_bundles_map.end(); it++){
      Opflash *flash = it->first;
      if (flash->get_type()==2){ // beam ...
	FlashTPCBundleSelection& bundles = it->second;
	FlashTPCBundleSelection consistent_bundles;
	FlashTPCBundle *min_bundle = 0;
	double min_score = 1e9;
	for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	  FlashTPCBundle *bundle = *it1;
	  if (min_bundle==0){
	    min_bundle = bundle;
	  }else{
	    if (bundle->get_ks_dis() * pow(bundle->get_chi2()/bundle->get_ndf(),0.8) <
		min_bundle->get_ks_dis() * pow(min_bundle->get_chi2()/min_bundle->get_ndf(),0.8))
	      min_bundle = bundle;
	  }
	  if (bundle->get_consistent_flag()){
	    consistent_bundles.push_back(bundle);
	  }
	}

	if (consistent_bundles.size()==0){
	  PR3DCluster *cluster = min_bundle->get_main_cluster();
	  FlashTPCBundleSelection& bundles = cluster_bundles_map[cluster];
	  FlashTPCBundleSelection consistent_bundles;
	  for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	    FlashTPCBundle *bundle = *it1;
	    if (bundle->get_consistent_flag())
	      consistent_bundles.push_back(bundle);
	  }
	  if (consistent_bundles.size()==0){
	    if (min_bundle->examine_beam_bundle())
	      min_bundle->set_consistent_flag(true);
	  }
	}
      }
    }
    
    // finish further examine the bundle ... 
    
    //  std::cout << "After Cleaning 2 : " << cluster_bundles_map.size() << " A " << flash_bundles_map.size() << " " << all_bundles.size() << std::endl;


    // // std::cout << std::endl << std::endl;
    // for (auto it = all_bundles.begin(); it!=all_bundles.end();it++){
    //   FlashTPCBundle *bundle = *it;
      
    //   //  if (bundle->get_flash()->get_flash_id()==29 && bundle->get_main_cluster()->get_cluster_id()==19)
    //   //	bundle->set_consistent_flag(1);
    //   //   if ( bundle->get_consistent_flag() || bundle->get_flag_at_x_boundary())
    //   std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << " " << bundle->get_consistent_flag()  << std::endl;

      
    // }
    // std::cout << std::endl << std::endl;

    
   

    
    // examining flash ... 
    for (auto it = flash_bundles_map.begin(); it!=flash_bundles_map.end(); it++){
      Opflash *flash = it->first;
      FlashTPCBundleSelection& bundles = it->second;
      FlashTPCBundleSelection consistent_bundles;
      FlashTPCBundleSelection remaining_bundles;
      FlashTPCBundleSelection to_be_removed;
      
      for (auto it1 = bundles.begin(); it1!=bundles.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	if (bundle->get_consistent_flag()){
	  consistent_bundles.push_back(bundle);
	}else{
	  remaining_bundles.push_back(bundle);
	}
      }

      if (consistent_bundles.size()>0){
	for (auto it1 = remaining_bundles.begin(); it1!=remaining_bundles.end(); it1++){
	  FlashTPCBundle *bundle1 = *it1;
	  bool flag_remove = true;
	  for (auto it2 = consistent_bundles.begin(); it2!=consistent_bundles.end(); it2++){
	    FlashTPCBundle *bundle2 = *it2;
	    if (bundle2->examine_bundle(bundle1,cos_pe_low, cos_pe_mid)){
	      flag_remove = false;
	      break;
	    }
	  }
	  if (flag_remove)
	    to_be_removed.push_back(bundle1);
	}
      }

      
      for (auto it1 = to_be_removed.begin(); it1!=to_be_removed.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	all_bundles.erase(bundle);
	
	Opflash *flash = bundle->get_flash();
	PR3DCluster *cluster = bundle->get_main_cluster();
	
	fc_bundles_map.erase(std::make_pair(flash,cluster));

	// remaing flash
	bundles.erase(find(bundles.begin(),bundles.end(),bundle));
	
	FlashTPCBundleSelection& temp_bundles = cluster_bundles_map[cluster];
	temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	if (temp_bundles.size()==0)
	  cluster_bundles_map.erase(cluster);
	delete bundle;
      }
    }


    

    
    // std::cout << "After Cleaning 3 : " << cluster_bundles_map.size() << " A " << flash_bundles_map.size() << " " << all_bundles.size() << std::endl;
    
    // // std::cout << std::endl << std::endl;
    // for (auto it = all_bundles.begin(); it!=all_bundles.end();it++){
    //   FlashTPCBundle *bundle = *it;
      
    // //   if ( bundle->get_consistent_flag() || bundle->get_flag_at_x_boundary())
    //   std::cout << bundle->get_flash()->get_flash_id() << " " << bundle->get_main_cluster()->get_cluster_id() << " " << bundle->get_flag_at_x_boundary() << " " << bundle->get_ks_dis() << " " << bundle->get_chi2() << " " << bundle->get_ndf() << " " << bundle->get_consistent_flag() << std::endl;
    // }
    // std::cout << std::endl << std::endl;



    // matching code // first round ... 
    {
      // regularization strength ... 
      double lambda = 0.1; // note the coefficient is all around 1
      //form matrix ...
      double fudge_factor1 = 0.06; // add 6% relative uncertainty for pe
      double fudge_factor2 = 1.0; // increase the original uncertainties by 50% ... 
      int num_unknowns = all_bundles.size();
      std::map<PR3DCluster*, int> map_tpc_index;
      std::map<Opflash*, int> map_flash_index;
    
      int tpc_index = 0;
      
      for(auto it=cluster_bundles_map.begin(); it!=cluster_bundles_map.end(); it++){
	PR3DCluster* main_cluster = it->first;
	map_tpc_index[main_cluster] = tpc_index;
	tpc_index++;
      }
      
      // improve the chisquare definition ...
      double delta_track = 0.01; // track can only be used once
      double delta_flash = 0.025;
      //    double delta_flash1 = 0.1;
      
      double num_unused_flash = flash_bundles_map.size() - cluster_bundles_map.size();
      if (num_unused_flash<0) num_unused_flash = 0;
      
      VectorXd M = VectorXd::Zero(32*flash_bundles_map.size()); // measurement from each PMT from each flash
      MatrixXd R = MatrixXd::Zero(32*flash_bundles_map.size(), num_unknowns+flash_bundles_map.size()); // unknowns to measurement matrix
      VectorXd MF = VectorXd::Zero(map_tpc_index.size() + flash_bundles_map.size());
      MatrixXd RF = MatrixXd::Zero(map_tpc_index.size() + flash_bundles_map.size(), num_unknowns + flash_bundles_map.size()); // penalty matrix term
      std::vector<std::pair<Opflash*,PR3DCluster*>> total_pairs;
      std::vector<double> total_weights;
      
      size_t i=0;
      for (auto it = flash_bundles_map.begin(); it != flash_bundles_map.end(); it++){
	Opflash *flash = it->first;
	FlashTPCBundleSelection& bundles = it->second;
	for (size_t j=0;j!=32;j++){
	  double pe = flash->get_PE(j);
	  double pe_err = sqrt(pow(flash->get_PE_err(j)*fudge_factor2,2) + pow(pe*fudge_factor1,2));
	  // if (pe/pe_err>0)
	  //   std::cout << i << " A " << j << " " << pe/pe_err << std::endl;
	  
	  M(32*i+j) = pe/pe_err;
	  R(32*i+j,num_unknowns+i) = pe/pe_err; // flash alone term
	}
	
	for (size_t j=0;j!=bundles.size();j++){
	  FlashTPCBundle *bundle = bundles.at(j);
	  PR3DCluster* main_cluster = bundle->get_main_cluster();
	  std::vector<double>& pred_pmt_light = bundle->get_pred_pmt_light();
	  for (size_t k=0;k!=32;k++){
	    double pe = flash->get_PE(k);
	    double pe_err = sqrt(pow(flash->get_PE_err(k)*fudge_factor2,2) + pow(pe*fudge_factor1,2));
	    R(32*i+k,total_pairs.end()-total_pairs.begin()) = 1./pe_err * pred_pmt_light.at(k);
	  }
	  
	  total_pairs.push_back(std::make_pair(flash,main_cluster));
	  
	  if (bundle->get_flag_at_x_boundary()){
	    total_weights.push_back(0.2);
	  }else{
	    total_weights.push_back(1.0);
	  }
	}
	
	i++;
      }
      
      for (auto it = flash_bundles_map.begin(); it != flash_bundles_map.end(); it++){
	Opflash *flash = it->first;
	total_weights.push_back(1);
      }
      
      
      
      // normalization of the flashes 
      i=0;
      for (auto it = flash_bundles_map.begin(); it != flash_bundles_map.end(); it++){
	MF(cluster_bundles_map.size()+i) = 0;//num_unused_flash/delta_flash;
	RF(cluster_bundles_map.size()+i,num_unknowns+i) = 1./delta_flash;
	
	map_flash_index[it->first] = num_unknowns + i;
	//	MF(cluster_bundles_map.size()+flash_bundles_map.size()) = num_unused_flash/delta_flash1;
	//	RF(cluster_bundles_map.size()+flash_bundles_map.size(),num_unknowns+i) = 1./delta_flash1;
	i++;
      }
      
      // normalization of the tracks
      i=0;
      for (auto it=cluster_bundles_map.begin(); it!=cluster_bundles_map.end(); it++){
	MF(i) = 1./delta_track;
	i++;
      }
      {
	for (size_t i=0; i!=total_pairs.size(); i++){
	  Opflash *flash = total_pairs.at(i).first;
	  PR3DCluster *main_cluster = total_pairs.at(i).second;
	  RF(map_tpc_index[main_cluster], i) = 1./delta_track;	
	}
      }
      
      
      MatrixXd RT = R.transpose();
      MatrixXd RFT = RF.transpose();
      
      VectorXd W = RT * M + RFT * MF;
      MatrixXd G = RT * R + RFT * RF;

      // for (size_t i = 0; i!= 32*flash_bundles_map.size(); i++){
      // 	std::cout << i << " " << M(i) << std::endl;
      // }
      
      
      WCP::LassoModel m2(lambda, 100000, 0.01);
      m2.SetData(G, W);

      std::vector<double> init_values;
      init_values.resize(num_unknowns + flash_bundles_map.size(),0);
     
      for (size_t i=0; i!=total_pairs.size(); i++){
	FlashTPCBundle *bundle = fc_bundles_map[std::make_pair(total_pairs.at(i).first, total_pairs.at(i).second)];
	if (bundle->get_consistent_flag()){
	  init_values.at(i) = 1;
	}else if (bundle->get_flag_at_x_boundary()){
	  init_values.at(i) = 0.5;
	}
      }
      m2.Set_init_values(init_values);
      
      for (size_t i=0; i!=total_weights.size(); i++){
	m2.SetLambdaWeight(i,total_weights.at(i));
      }
      m2.Fit();
      VectorXd beta = m2.Getbeta();

      
      FlashTPCBundleSelection to_be_removed;
      for (size_t i=0;i!=total_pairs.size();i++){
	//	std::cout << i << " " << beta(i)  << std::endl;
	if (beta(i) < 0.05){
	  to_be_removed.push_back(fc_bundles_map[std::make_pair(total_pairs.at(i).first, total_pairs.at(i).second)]);
	}
      }
      
      for (auto it1 = to_be_removed.begin(); it1!=to_be_removed.end(); it1++){
	FlashTPCBundle *bundle = *it1;
	all_bundles.erase(bundle);
	
	Opflash *flash = bundle->get_flash();
	PR3DCluster *cluster = bundle->get_main_cluster();
	
	
	
	{
	  FlashTPCBundleSelection& temp_bundles = flash_bundles_map[flash];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    flash_bundles_map.erase(flash);
	}
	{
	  FlashTPCBundleSelection& temp_bundles = cluster_bundles_map[cluster];
	  temp_bundles.erase(find(temp_bundles.begin(), temp_bundles.end(), bundle));
	  if (temp_bundles.size()==0)
	    cluster_bundles_map.erase(cluster);
	}
	
	//	fc_bundles_map.erase(std::make_pair(flash,cluster));
	//	delete bundle;
      }
    }
    
    //   std::cout << "After Cleaning 4 : " << cluster_bundles_map.size() << " A " << flash_bundles_map.size() << " " << all_bundles.size() << std::endl;
    
    // regularization strength ... 
    double lambda = 0.1; // note the coefficient is all around 1
    //form matrix ...
    double fudge_factor1 = 0.05; // add 6% relative uncertainty for pe
    double fudge_factor2 = 1.0; // increase the original uncertainties by 50% ... 
    int num_unknowns = all_bundles.size();
    std::map<PR3DCluster*, int> map_tpc_index;
    std::map<Opflash*, int> map_flash_index;
    
    int tpc_index = 0;
    
    for(auto it=cluster_bundles_map.begin(); it!=cluster_bundles_map.end(); it++){
      PR3DCluster* main_cluster = it->first;
      map_tpc_index[main_cluster] = tpc_index;
      tpc_index++;
    }
    
    // improve the chisquare definition ...
    double delta_track = 0.01; // track can only be used once
    
    VectorXd M = VectorXd::Zero(32*flash_bundles_map.size()); // measurement from each PMT from each flash
    MatrixXd R = MatrixXd::Zero(32*flash_bundles_map.size(), num_unknowns); // unknowns to measurement matrix
    VectorXd MF = VectorXd::Zero(map_tpc_index.size() );
    MatrixXd RF = MatrixXd::Zero(map_tpc_index.size() , num_unknowns ); // penalty matrix term
    std::vector<std::pair<Opflash*,PR3DCluster*>> total_pairs;
    std::vector<double> total_weights;
    
    size_t i=0;
    for (auto it = flash_bundles_map.begin(); it != flash_bundles_map.end(); it++){
      Opflash *flash = it->first;
      FlashTPCBundleSelection& bundles = it->second;
      for (size_t j=0;j!=32;j++){
	double pe = flash->get_PE(j);
	double pe_err = sqrt(pow(flash->get_PE_err(j)*fudge_factor2,2) + pow(pe*fudge_factor1,2));
	
	M(32*i+j) = pe/pe_err;
	//	R(32*i+j,num_unknowns+i) = pe/pe_err; // flash alone term
      }
      
      for (size_t j=0;j!=bundles.size();j++){
	FlashTPCBundle *bundle = bundles.at(j);
	PR3DCluster* main_cluster = bundle->get_main_cluster();
	std::vector<double>& pred_pmt_light = bundle->get_pred_pmt_light();
	for (size_t k=0;k!=32;k++){
	  double pe = flash->get_PE(k);
	  double pe_err = sqrt(pow(flash->get_PE_err(k)*fudge_factor2,2) + pow(pe*fudge_factor1,2));
	  R(32*i+k,total_pairs.end()-total_pairs.begin()) = 1./pe_err * pred_pmt_light.at(k);
	}
	
	total_pairs.push_back(std::make_pair(flash,main_cluster));
	
	if (bundle->get_flag_at_x_boundary()){
	  total_weights.push_back(0.2);
	}else{
	  total_weights.push_back(1.0);
	}
      }
      
      i++;
    }
    
    // normalization of the tracks
    i=0;
    for (auto it=cluster_bundles_map.begin(); it!=cluster_bundles_map.end(); it++){
      MF(i) = 1./delta_track;
      i++;
    }
    {
      for (size_t i=0; i!=total_pairs.size(); i++){
	Opflash *flash = total_pairs.at(i).first;
	PR3DCluster *main_cluster = total_pairs.at(i).second;
	RF(map_tpc_index[main_cluster], i) = 1./delta_track;	
      }
    }
    
      
    MatrixXd RT = R.transpose();
    MatrixXd RFT = RF.transpose();
    
    VectorXd W = RT * M + RFT * MF;
    MatrixXd G = RT * R + RFT * RF;
    
    WCP::LassoModel m2(lambda, 100000, 0.01);
    m2.SetData(G, W);
    for (size_t i=0; i!=total_weights.size(); i++){
      m2.SetLambdaWeight(i,total_weights.at(i));
    }
    m2.Fit();
    VectorXd beta = m2.Getbeta();
    
    // for (auto it = flash_bundles_map.begin(); it != flash_bundles_map.end(); it++){
    //   Opflash *flash = it->first;
    //   std::cout << flash->get_flash_id() << " " << beta(map_flash_index[flash]) << std::endl;
    // }


    // form matched pairs according to TPC cluster 
    std::map<int,std::pair<Opflash*,double>> matched_pairs;
    for (size_t i=0;i!=total_pairs.size();i++){
      if(beta(i)!=0){
  	int tpc_index = map_tpc_index[total_pairs.at(i).second];
  	Opflash *flash = total_pairs.at(i).first;
  	if (matched_pairs.find(tpc_index)==matched_pairs.end()){
  	  matched_pairs[tpc_index] = std::make_pair(flash,beta(i));
  	}else{
  	  if (beta(i) > matched_pairs[tpc_index].second){
  	    matched_pairs[tpc_index] = std::make_pair(flash,beta(i));
  	  }
  	}
	//std::cout << i << " Q " <<  tpc_index << " " << flash->get_flash_id() << " " << total_pairs.at(i).second->get_cluster_id() << " " << total_weights.at(i) << " " << beta(i)  << " " << flash->get_time() << std::endl;
      }
    }
    
    FlashTPCBundleSelection results_bundles;
    FlashTPCBundleSet results_bundles_set;
    // return bundles ...    
    for (auto it = group_clusters.begin(); it!=group_clusters.end(); it++){
      PR3DCluster* main_cluster = it->first;
      
      
      if (map_tpc_index.find(main_cluster)!=map_tpc_index.end()){
	int tpc_index = map_tpc_index[main_cluster];
	if (matched_pairs.find(tpc_index)!=matched_pairs.end()){
	  Opflash* flash = matched_pairs[tpc_index].first;
	  double strength = matched_pairs[tpc_index].second;
	  FlashTPCBundle* bundle = fc_bundles_map[std::make_pair(flash,main_cluster)];
	  bundle->set_strength(strength);
	  results_bundles_set.insert(bundle);
	  //results_bundles.push_back(bundle);
	}else{
	  Opflash *flash = 0;
	  double strength  =0;
	  FlashTPCBundle *bundle = new FlashTPCBundle(flash,main_cluster,-1,tpc_index);
	  PR3DClusterSelection& other_clusters = bundle->get_other_clusters();
	  for (auto it1 = it->second.begin(); it1!= it->second.end(); it1++){
	    other_clusters.push_back(it1->first);
	  }
	  results_bundles_set.insert(bundle);
	  //	  results_bundles.push_back(bundle);
	}
      }else{
	Opflash *flash = 0;
	double strength  =0;
	FlashTPCBundle *bundle = new FlashTPCBundle(flash,main_cluster,-1,-1);
	PR3DClusterSelection& other_clusters = bundle->get_other_clusters();
	for (auto it1 = it->second.begin(); it1!= it->second.end(); it1++){
	  other_clusters.push_back(it1->first);
	}
	results_bundles_set.insert(bundle);
	//	results_bundles.push_back(bundle);
      }
    }
    std::copy(results_bundles_set.begin(),results_bundles_set.end(),std::back_inserter(results_bundles));
    

    // std::cout << "Final: " << std::endl;
    // for (auto it = results_bundles.begin(); it!= results_bundles.end(); it++){
    //   FlashTPCBundle *bundle = (*it);
    //   Opflash *flash = bundle->get_flash();
    //   if (flash!=0){
    // 	std::cout << flash->get_flash_id() << " " << flash->get_time() << " " << bundle->get_main_cluster()->get_cluster_id() << std::endl;
    //   }else{
    //  	std::cout << -1 << " " << bundle->get_main_cluster()->get_cluster_id() << " " << cluster_bundles_map[bundle->get_main_cluster()].size() << std::endl;
    //   }
    // }

    
    organize_matched_bundles(results_bundles, cos_pe_low, cos_pe_mid, fc_bundles_map);


    // std::cout << "Final1: " << std::endl;
    // for (auto it = results_bundles.begin(); it!= results_bundles.end(); it++){
    //   FlashTPCBundle *bundle = (*it);
    //   Opflash *flash = bundle->get_flash();
    //   if (flash!=0){
    // 	std::cout << flash->get_flash_id() << " " << flash->get_time() << " " << bundle->get_main_cluster()->get_cluster_id() << std::endl;
    //   }else{
    // 	std::cout << -1 << " " << bundle->get_main_cluster()->get_cluster_id() << " " << cluster_bundles_map[bundle->get_main_cluster()].size() << std::endl;
    //   }
    // }

    delete g1;
    delete g2;
    delete g3;
    
    return results_bundles;
  }
  }
*/
