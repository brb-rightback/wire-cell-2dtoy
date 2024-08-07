#include "WCP2dToy/ChargeSolving.h"


using namespace WCP;

WCP2dToy::ChargeSolving::ChargeSolving(const WCP::GeomDataSource& gds, LowmemTiling& tiling)
  : gds(gds)
  , tiling(tiling)
{
  ndirect_solved = 0;
  nL1_solved = 0;

  // initialize the cell weight map
  init_cell_weight_map(9);
  
  // divide into small groups
  divide_groups();

}

WCP2dToy::ChargeSolving::~ChargeSolving(){
}



void WCP2dToy::ChargeSolving::L1_resolve(float weight, float reduce_weight_factor){
  update_cell_weight_map(weight, reduce_weight_factor);
  ccmap.clear();
  
  // loop existing matrices and resolve ...
  for (auto it = group_matrices.begin(); it!= group_matrices.end(); it++){
    MatrixSolver *matrix = (*it);
    if (matrix->get_solve_flag()==2){
      matrix->L1_Solve(cell_weight_map);
      GeomCellSelection cells = matrix->get_all_cells();
      for (auto it = cells.begin(); it!=cells.end(); it++){
	MergeGeomCell *mcell = (MergeGeomCell*)(*it);
	//	std::cout << ccmap[mcell] << " ";
	ccmap[mcell] = matrix->get_mcell_charge(mcell) ;
	//std::cout << ccmap[mcell] << std::endl;
      }
    }
  }
}

void WCP2dToy::ChargeSolving::update_cell_weight_map(float weight, float reduce_weight_factor){
  for (auto it= cell_weight_map.begin(); it!=cell_weight_map.end(); it++){
    const GeomCell *mcell = it->first;
    float temp_weight = weight;
    if (front_connectivity.find(mcell)!=front_connectivity.end())
      temp_weight /= reduce_weight_factor;
    if (back_connectivity.find(mcell)!=back_connectivity.end())
      temp_weight /= reduce_weight_factor;
    cell_weight_map[mcell] = temp_weight;
    //std::cout << temp_weight << std::endl;
  }
}

void WCP2dToy::ChargeSolving::init_cell_weight_map(float weight){
  GeomCellSelection& one_wire_cells = tiling.get_one_good_wire_cells();
  GeomCellSelection& two_wire_cells = tiling.get_two_good_wire_cells();
  GeomCellSelection& three_wire_cells = tiling.get_three_good_wire_cells();

  GeomCellMap& cell_wire_map = tiling.get_cell_wires_map();
  GeomWireMap& wire_cell_map = tiling.get_wire_cells_map();

  for (auto it = three_wire_cells.begin(); it!=three_wire_cells.end(); it++){
    const GeomCell *mcell = (*it);
    cell_weight_map[mcell] = weight;
  }

  for (auto it = two_wire_cells.begin(); it!=two_wire_cells.end(); it++){
    const GeomCell *mcell = (*it);
    cell_weight_map[mcell] = weight;
  }

  for (auto it = one_wire_cells.begin(); it!=one_wire_cells.end(); it++){
    const GeomCell *mcell = (*it);
    cell_weight_map[mcell] = weight;
  }
  
}

float WCP2dToy::ChargeSolving::get_weight(WCP::GeomCell *cell){
  auto it = cell_weight_map.find(cell);
  if (it != cell_weight_map.end()){
    return it->second;
  }else{
    return -1;
  }
}

int WCP2dToy::ChargeSolving::get_matrix_id(WCP::GeomCell *cell){
  auto it = map_mc_matrix_id.find(cell);
  if (it != map_mc_matrix_id.end()){
    return it->second;
  }else{
    return -1;
  }
}
int WCP2dToy::ChargeSolving::get_mc_id(WCP::GeomCell *cell){
  auto it = map_mc_index.find(cell);
  if (it != map_mc_index.end()){
    return it->second;
  }else{
    return -1;
  }
}
double WCP2dToy::ChargeSolving::get_mc_lambda(WCP::GeomCell *cell){
  auto it = map_mc_lambda.find(cell);
  if (it != map_mc_lambda.end()){
    return it->second;
  }else{
    return -1;
  }
}
double WCP2dToy::ChargeSolving::get_mc_TOL(WCP::GeomCell *cell){
  auto it = map_mc_TOL.find(cell);
  if (it != map_mc_TOL.end()){
    return it->second;
  }else{
    return -1;
  }
}

int WCP2dToy::ChargeSolving::get_mw_id(WCP::GeomWire *mwire){
  auto it = map_mw_index.find(mwire);
  if (it!= map_mw_index.end()){
    return it->second;
  }else{
    return -1;
  }
}
double WCP2dToy::ChargeSolving::get_W(WCP::GeomWire *mwire){
  auto it = map_W.find(mwire);
  if (it != map_W.end()){
    return it->second;
  }else{
    return 0;
  }
}
double WCP2dToy::ChargeSolving::get_G(WCP::GeomCell *cell, WCP::GeomWire *mwire){
  auto it = map_G.find(std::make_pair(cell, mwire));
  if (it != map_G.end()){
    return it->second;
  }else{
    return 0;
  }
}


void WCP2dToy::ChargeSolving::update_information(){
  GeomCellMap& cell_wire_map = tiling.get_cell_wires_map();
  GeomWireMap& wire_cell_map = tiling.get_wire_cells_map();

  int matrix_id = 1;
  for (auto it = group_matrices.begin(); it!= group_matrices.end(); it++){
    MatrixSolver *matrix = (*it);
    matrix->set_id(matrix_id);
    WCP::GeomCellSelection all_cells = matrix->get_all_cells();
    for (auto it1 = all_cells.begin(); it1!=all_cells.end(); it1++){
      map_mc_matrix_id[*it1] = matrix->get_id();
      int c_index = matrix->get_mc_index(*it1);
      map_mc_index[*it1] = c_index;
      map_mc_TOL[*it1] = matrix->get_TOL();
      map_mc_lambda[*it1] = matrix->get_lambda();

      for (auto it2 = cell_wire_map[*it1].begin(); it2!= cell_wire_map[*it1].end(); it2++){
	  const GeomWire *mwire = *it2;
	  int w_index = matrix->get_mw_index(mwire);
	  map_mw_index[mwire] = w_index;
	  map_W[mwire] = matrix->get_W_value(w_index);
	  map_G[std::make_pair(*it1, mwire)] = matrix->get_G_value(w_index, c_index);
      }
      
      //std::cout << matrix->get_TOL() << " " << matrix->get_lambda() << std::endl;
      //      if (cell_weight_map.find(*it1) == cell_weight_map.end())
      //	std::cout << "something wrong" << std::endl;
      //      std::cout << cell_weight_map[*it1] << std::endl;
    }
    matrix_id++;
  }
}


void WCP2dToy::ChargeSolving::divide_groups(){

  GeomCellSelection& one_wire_cells = tiling.get_one_good_wire_cells();
  GeomCellSelection& two_wire_cells = tiling.get_two_good_wire_cells();
  GeomCellSelection& three_wire_cells = tiling.get_three_good_wire_cells();
  
  WireChargeMap& wire_charge_map = tiling.get_wire_charge_map();
  WireChargeMap& wire_charge_error_map = tiling.get_wire_charge_error_map();
  
  GeomWireSelection good_wires = tiling.get_all_good_wires();
  GeomWireSelection bad_wires = tiling.get_all_bad_wires();

  GeomCellMap& cell_wire_map = tiling.get_cell_wires_map();
  GeomWireMap& wire_cell_map = tiling.get_wire_cells_map();
  
  // original cell/wire holders
  GeomCellSetp all_cells;
  GeomWireSetp all_wires;

  std::copy(good_wires.begin(), good_wires.end(), std::inserter(all_wires, all_wires.end()));
  std::copy(three_wire_cells.begin(), three_wire_cells.end(), std::inserter(all_cells, all_cells.end()));
  std::copy(two_wire_cells.begin(), two_wire_cells.end(), std::inserter(all_cells, all_cells.end()));
  std::copy(one_wire_cells.begin(), one_wire_cells.end(), std::inserter(all_cells, all_cells.end()));

  
  // final cell/wire holders
  // std::cout << all_wires.size() << " " << all_cells.size() << std::endl;

  while(all_wires.size()){
    //temporary cell/wire holders
    GeomCellSelection temp_cells;
    GeomWireSelection temp_wires;
    
    GeomCellSelection grouped_cells;
    GeomWireSelection grouped_wires;
    
    // start one good wire, move it in
    auto it5 = all_wires.begin();
    const GeomWire* mwire = *it5;
    all_wires.erase(mwire);
    //all_wires.pop_front();
    temp_wires.push_back(mwire);
    grouped_wires.push_back(mwire);
    
    while(temp_wires.size()!=0){
      // find all the cells associated with it, move in 
      temp_cells.clear();
      for (auto it = temp_wires.begin(); it!= temp_wires.end(); it++){
	const GeomWire *mwire = *it;
	for (auto it1 = wire_cell_map[mwire].begin(); it1!= wire_cell_map[mwire].end(); it1++){
	  const GeomCell *mcell = *it1;
	  auto it2 = all_cells.find(mcell);
	  if (it2 != all_cells.end()){
	    // fill in the temp cells ... 
	    temp_cells.push_back(mcell);
	    // remove from the all_cells
	    all_cells.erase(it2);
	    // fill in grouped_wires
	    grouped_cells.push_back(mcell);
	  }
	}
      }
      temp_wires.clear();
      // find all the wires associated with these cells, move in
      for (auto it = temp_cells.begin(); it!= temp_cells.end(); it++){
	const GeomCell *mcell = *it;
	for (auto it1 = cell_wire_map[mcell].begin(); it1!= cell_wire_map[mcell].end(); it1++){
	  const GeomWire *mwire = *it1;
	  auto it2 = all_wires.find(mwire);
	  if (it2!= all_wires.end()){
	    temp_wires.push_back(mwire);
	    all_wires.erase(it2);
	    grouped_wires.push_back(mwire);
	  }
	}
      }
      
    }

    // std::cout << "Xin " << grouped_wires.size() << " " << grouped_cells.size() << std::endl;
    final_cells_vec.push_back(grouped_cells);
    final_wires_vec.push_back(grouped_wires);

    MatrixSolver *matrix = new MatrixSolver(grouped_cells, grouped_wires, cell_wire_map, wire_cell_map, wire_charge_map, wire_charge_error_map);

    // do L1 solve ... 
    if (matrix->get_solve_flag()==0){
      matrix->L1_Solve(cell_weight_map);
    }
    
    if (matrix->get_solve_flag()==1){
      ndirect_solved ++;
    }else if (matrix->get_solve_flag()==2){
      nL1_solved++;
    }

    for (auto it = grouped_cells.begin(); it!=grouped_cells.end(); it++){
      MergeGeomCell *mcell = (MergeGeomCell*)(*it);
      //test
      // if (matrix->get_solve_flag()==1){
      // 	ccmap[mcell] = matrix->get_mcell_charge(mcell) ;
      // }else{
      // 	ccmap[mcell] = 10000;
      // }
      
      ccmap[mcell] = matrix->get_mcell_charge(mcell) ;
      //   std::cout << matrix->get_mcell_charge(mcell) << std::endl;
    }
    
    group_matrices.push_back(matrix);
    
  }

  
  
  // std::cout << one_wire_cells.size() << " " << two_wire_cells.size() << " " << three_wire_cells.size() << " " << good_wires.size() << " " << bad_wires.size() << " " << cell_wire_map.size() << " " << wire_cell_map.size() << std::endl;
  // divide all wires into seperate groups ...

}

double WCP2dToy::ChargeSolving::get_chi2(){
  return chi2;
}

void WCP2dToy::ChargeSolving::Update_ndf_chi2(){
  ndf = 0;
  L1_ndf.clear();
  direct_ndf.clear();
  for (auto it = group_matrices.begin(); it!=group_matrices.end(); it++){
    MatrixSolver* matrix = (*it);
    if (matrix->get_solve_flag()==1){
      ndf += matrix->get_direct_ndf();
      direct_ndf.push_back(matrix->get_direct_ndf());
      //std::cout << "1 " << matrix->get_direct_ndf() << std::endl;
    }else if (matrix->get_solve_flag()==2){
      ndf += matrix->get_L1_ndf();
      L1_ndf.push_back(matrix->get_L1_ndf());
      //std::cout << "2 " << matrix->get_L1_ndf() << std::endl;
    }
    // std::cout << ndf << std::endl;
  }

  chi2 = 0;
  L1_chi2_base.clear();
  L1_chi2_penalty.clear();
  direct_chi2.clear();
  for (auto it = group_matrices.begin(); it!=group_matrices.end(); it++){
    MatrixSolver* matrix = (*it);
    if (matrix->get_solve_flag()==1){
      chi2 += matrix->get_direct_chi2();
      direct_chi2.push_back(matrix->get_direct_chi2());
      //std::cout << "1 " << matrix->get_direct_chi2() << std::endl;
    }else if (matrix->get_solve_flag()==2){
      chi2 += matrix->get_L1_chi2_base();
      L1_chi2_base.push_back(matrix->get_L1_chi2_base());
      L1_chi2_penalty.push_back(matrix->get_L1_chi2_penalty());
      //std::cout << "2 " << matrix->get_L1_chi2_base() << std::endl;
    }
    //    std::cout << chi2 << std::endl;
  }
  
  
}

int WCP2dToy::ChargeSolving::get_ndf(){
  
  
  return ndf;
}


