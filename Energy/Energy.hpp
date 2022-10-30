#ifndef __CLASS_ENERGY

#include <cmath>

#define __CLASS_ENERGY

class Energy
{
public:
  int n;
  double k;
  double tau;
  double rou;
  double *out_con;
  double *out_ten;
  double *out_ela;

  // コンストラクタ //
  Energy(int nn, double kk, double tt, double rr);
  ~Energy(); // デストラクタ

  // セッタ //
  void set(int nn, double kk, double tt, double rr);

  // 弾性力 //
  double *calculateElasticity(const int cell[][6], const double area[], const double ss, const double rx[][5]);

  // 線張力 //
  double *calculateTension(const int edge[][2], const double rx[][5]);

  //　収縮性 //
  double *calculateContractility(const int cell[][6], const double perimeter[], const double rx[][5]);

  // 距離計算 //
  double E_dis(const double xi, const double yi, const double xj, const double yj){
    double dis = 0;
    dis = sqrt((xi-xj)*(xi-xj)+(yi-yj)*(yi-yj));
    return dis;
  }

};


#endif
