#include "Energy.hpp"

// コンストラクタ //
Energy::Energy(int nn, double kk, double tt, double rr){
  set(nn, kk, tt, rr);
}

Energy::~Energy(){
  if(out_con != NULL) delete[] out_con;
  if(out_ela != NULL) delete[] out_ela;
  if(out_ten != NULL) delete[] out_ten;
}

// セッタ //
void Energy::set(int nn, double kk, double tt, double rr){
  n = nn;
  k = kk;
  tau = tt;
  rou = rr;
  out_con = new double[6+(nn-1)*3+1];
  out_ela = new double[6+(nn-1)*3+1];
  out_ten = new double[6+(nn-1)*3+1];
}

// 弾性力 //
double (*Energy::calculateElasticity(const int cell[][6], const double area[], const double ss, const double rx[][5]))[2]{
  // 初期化 //
  for(int i=0; i<6+(n-1)*3+1; i++){
    out_ela[i] = 0;
  }
  // 計算 //
  for(int a=0; a<n; a++){
    double coeff = k * pow((ss - area[a]),2);
    for(int i=0; i<6; i++){
      out_ela[cell[a][i]] += 0.5 * coeff;
    }
  }
  return out_ela;
}

// 線張力 //
double (*Energy::calculateTension(const int edge[][2], const double rx[][5]))[2]{
  // 初期化 //
  for(int i=0; i<6+(n-1)*3+1; i++){
    out_ten[i] = 0;
  }
  for(int i=0; i<(3+n*4)*2; i++){
    double *vec = new double[2];
    out_ten[edge[i][0]] += 0.25 * tau * E_dis(rx[edge[i][0]],rx[edge[i][1]);
    out_ten[edge[i][1]] += 0.25 * tau * E_dis(rx[edge[i][1]],rx[edge[i][0]);
  }
  return out_ten;
}

//　収縮性 //
double (*Energy::calculateContractility(const int cell[][6], const double perimeter[], const double rx[][5]))[2]{
  // 初期化 //
  for(int i=0; i<6+(n-1)*3+1; i++){
    out_con[i] = 0;
  }
  // 計算 //
  for(int a=0; a<n; a++){
    for(int i=0; i<6; i++){
      for(int j=2; j<5; j++){
        if(rx[cell[a][i]][j] < 0) break;
        out_con[cell[a][i]] += rou * pow(perimeter[(int)(rx[cell[a][i]][j])], 2.0);
      }
    }
  }

  return out_con;
}
