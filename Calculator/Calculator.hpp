#ifndef __CLASS_CALCULATOR

#include <cmath>

#define __CLASS_CALCULATOR

class Calculator
{
public:
  // コンストラクタ //
  Calculator(int a);

  // 距離計算 //
  double E_dis(const double xi, const double yi, const double xj, const double yj);

  // 角度計算 //
  double E_angle (const double xi, const double yi, const double xj, const double yj);

  // 中心座標 //
  double *E_center(const int cell[], const double rx[][5]);

  // 面積計算 //
  double E_area (const int cell[], const double rx[][5]);

  // 周囲長計算 //
  double E_perimeter(const int cell[], const double rx[][5]);

};

#endif
