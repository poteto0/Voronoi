#include "Calculator.hpp"

Calculator::Calculator(int a){
};

// 距離計算 //
double Calculator::E_dis(const double xi, const double yi, const double xj, const double yj){
  double dis = 0;
  dis = sqrt((xi-xj)*(xi-xj)+(yi-yj)*(yi-yj));
  return dis;
}

// 角度計算 //
double Calculator::E_angle (const double xi, const double yi, const double xj, const double yj) {
  double theta = 0;
  double rx = 0; // x距離
  double ry = 0; // y距離
  rx = xj - xi;
  ry = yj - yi;
  theta = atan2(ry, rx); // 個体間角度計算

  return theta;
}

// 中心座標 //
double *Calculator::E_center(const int cell[], const double rx[][5]){
  double *out;
  double sum_rx = 0;
  double sum_ry = 0;
  out = new double[2];
  for(int i=0; i<6; i++){
    sum_rx += rx[cell[i]][0];
    sum_ry += rx[cell[i]][1];
  }
  out[0] = sum_rx / 6;
  out[1] = sum_ry / 6;
  return out;
}

// 面積計算 //
double Calculator::E_area (const int cell[], const double rx[][5]){
  double area = 0;
  double x1 = rx[cell[0]][0];
  double y1 = rx[cell[0]][1];
  for(int i=1; i<5; i++){
    double x2 = rx[cell[i]][0];
    double y2 = rx[cell[i]][1];
    double x3 = rx[cell[i+1]][0];
    double y3 = rx[cell[i+1]][1];
    area += abs((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1));
  }
  return area/2;
}

// 周囲長計算 //
double Calculator::E_perimeter(const int cell[], const double rx[][5]){
  double sum_dis = 0;
  for(int i=0; i<6; i++){
    double x1 = rx[cell[i]][0];
    double y1 = rx[cell[i]][1];
    double x2 = rx[cell[(int)((i+1)%6)]][0];
    double y2 = rx[cell[(int)((i+1)%6)]][1];
    double dis = E_dis(x1, y1, x2, y2);
    sum_dis += dis;
  }
  return sum_dis;
}
