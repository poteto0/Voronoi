#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include "Mplot/Mplot.cpp"
#include "Mplot/ColorRamp.cpp"

// 使用宣言 //
using namespace std;

#define N 6 // 細胞数
#define R 2 // 長さ

// 変数
double derta = 1; // 界面エネルギー密度
double rou = 1; // 隣接セル間の圧力差
// 制約
double S0 = 10; // 基準の高さ
double V0 = 10; // 基準体積
double h0 = 1; // 基準の高さ
// 描画範囲
int width = 500;
int height = 500;

double t = 0; // 時間

// 細胞 //
typedef struct cell {
  double rx[N][6] = {}; // 各頂点の位置座標
  double ry[N][6] = {};
  double dx[N][6] = {};
  double dy[N][6] = {};
  double centerx[N] = {}; // 中心座標
  double centery[N] = {};
  double S[N] = {}; // 面積
  double V[N] = {}; // 体積
  double Us[N][6] = {}; // 界面エネルギーUs
  double Uv[N] = {};
  vector<vector<int>> edges;
} my_cell;

vector<double> split(string& input, char delimiter)
{
    istringstream stream(input);
    string field;
    vector<double> result;
    while (getline(stream, field, delimiter)) {
      result.push_back(stod(field));
    }
    return result;
}

// egdeの判定 //
vector<vector<int>> edgesDecision(const int nn, const double rx[][6], const double ry[][6]){
  vector<vector<int>> edges(nn, vector<int>(6));
  for(int a=0; a<nn-1; a++){ // 細胞数回す
    for(int i=0; i<6; i++){ // ノードを回す
      edges.at(a).at(i) = 0;
    }
  }
  for(int a=0; a<nn-1; a++){ // 細胞数回す
    for(int i=0; i<6; i++){ // ノードを回す
      for(int b=a+1; b<nn; b++){ // 他の細胞を回す
        for(int j=0; j<6; j++){ // 他細胞のノードを回す
          if(rx[a][i]==rx[b][j] && ry[a][i]==ry[b][j]){ // ノードが一致したら
            if(j!=0){
              if(rx[a][(int)((i+1)%6)]==rx[b][j-1]){ // 2つのノードが一致したら
                edges.at(a).at(i) = b+1;
                edges.at(b).at(j-1) = a+1;
              }
            }else {
              if(rx[a][(int)((i+1)%6)]==rx[b][5]){ // 2つのノードが一致したら
                edges.at(a).at(i) = b+1;
                edges.at(b).at(5) = a+1;
              }
            }
          }
        }
      }
    }
  }
  return edges;
}

// 距離計算 //
double E_dis(const double xi, const double yi, const double xj, const double yj){
  double dis = 0;
  dis = sqrt((xi-xj)*(xi-xj)+(yi-yj)*(yi-yj));
  return dis;
}

// 角度計算 //
double E_angle (double xi, double yi, double xj, double yj) {
	double theta = 0;
	double rx = 0; // x距離
	double ry = 0; // y距離
	rx = xj - xi;
	ry = yj - yi;
	theta = atan2(ry, rx); // 個体間角度計算

	return theta;
}

// 中心座標 //
double *E_center(const double rx[], const double ry[]){
  double *center;
  double sum_rx = 0;
  double sum_ry = 0;
  for(int i=0; i<6; i++){
    sum_rx += rx[i];
    sum_ry += ry[i];
  }
  center[0] = sum_rx / 6;
  center[1] = sum_ry / 6;
  return center;
}

// 面積計算 //
double E_area (double cx, double cy, const double rx[], const double ry[]){
  double args[6] = {};
  double dis[6] = {};
  double area = 0;
  for(int i=0; i<6; i++){
    args[i] = E_angle(cx, cy, rx[i], ry[i]);
    dis[i] = E_dis(cx, cy, rx[i], ry[i]);
  }
  for(int i=0; i<6; i++){
    double arg = abs(args[i] - args[(int)((i+1)%6)]);
    area += 0.5 * sin(arg) * dis[i] * dis[(int)((i+1)%6)];
  }
  return area;
}

// edge毎のUsを計算 //
double calculateUsByEdges(const double derta, const double xi, const double yi, const double xj, const double yj){
  double dis = 0;
  dis = E_dis(xi, yi, xj, yj);
  return dis * derta;
}

// edge毎のUsを計算 //
double calculateUsByEdges(const double derta, int a, int i, const double rx[][6], const double ry[][6], vector<vector<int>> edges){
  double dis = 0;
  double UsByEdges = 0;
  if(edges.at(a).at(i) != 0){
    int cell = edges.at(a).at(i) - 1;
    for(int j=0; j<6; j++){
      if(edges.at(cell).at(j) == a+1){
        int node = j;
        dis = E_dis(rx[a][i], ry[a][i], rx[cell][node], ry[cell][node]);
        UsByEdges = dis * derta;
      }
    }
  }
  return UsByEdges;
}

// 個体へのUsを計算 //
double calculateUsSum(const int nn, const int a, double derta, const double rx[][6], const double ry[][6], vector<vector<int>> edges){
  int node = 0;
  double UsByEdges = 0;
  double sum = 0;
  for(int i=0; i<6; i++){
    if(edges.at(a).at(i) != 0){
      int cell = edges.at(a).at(i) - 1;
      for(int j=0; j<6; j++){
        if(edges.at(cell).at(j) == a+1) node = j;
      }
      UsByEdges = calculateUsByEdges(derta, rx[a][i], ry[a][i], rx[cell][node], ry[cell][node]);
      sum += UsByEdges;
    }
  }
  return sum;
}

double calculateUv(const double rou, const double s0, const double ss){
  double Uv = 0;
  Uv = rou * abs(s0 - ss);
  return Uv;
}

int main(int argc, char const *argv[]){ //main
  vector<double> data;
  string name_file;

  my_cell c;
  Mplot mp(width,height,0,25,0,25);

  //--- ファイル読み取り ---//
  for(int a=0; a<N; a++){
    double sum_rx = 0;
    double sum_ry = 0;
    name_file = "data/Cell6vertecies" + to_string(a+1) + ".csv";
    ifstream ifs(name_file);
    string line;
    while (getline(ifs, line)) {
      data = split(line, ',');
      for(int i=0; i<6; i++){ // 頂点数
        c.rx[a][i] = data.at(i*2+1);
        c.ry[a][i] = data.at((i+1)*2);
      }
    }

    c.rx[0][0] = 1;
    c.ry[0][0] = 0;
    for(int a=0; a<N; a++){
      for(int i=0; i<6; i++){
        if(a==0 && i==0) continue;
        c.rx[a][i] = c.rx
      }
    }
    double *center = E_center(c.rx[a], c.ry[a]);
    c.centerx[a] = center[0];
    c.centery[a] = center[1];
  }

  // edge判定 //
  c.edges = edgesDecision(N, c.rx, c.ry);

  // シミュレーション
  while(t < 1000){
    mp.SetPicture_first();
    // 個体毎に回す //
    for(int a=0; a<N; a++){
      // 界面エネルギー計算 //
      for(int i=0; i<6; i++){
        c.Us[a][i] += calculateUsByEdges(derta, a, i, c.rx, c.ry, c.edges);
      }

      // 面積計算 //
      double *center = E_center(c.rx[a], c.ry[a]);
      c.centerx[a] = center[0];
      c.centery[a] = center[1];
      c.S[a] = E_area(c.centerx[a], c.centery[a], c.rx[a], c.ry[a]);
      c.Uv[a] = calculateUv(rou, S0, c.S[a]);
    }

    //cout << c.centerx[0] << endl;

    for(int a=0; a<N; a++){
      for(int i=0; i<6; i++){
        c.dx[a][i] -= 0.00000000000001 * (c.Us[a][i] + c.Uv[a]);
        c.dy[a][i] -= 0.00000000000001 * (c.Us[a][i] + c.Uv[a]);
      }
    }

    for(int a=0; a<N; a++){
      for(int i=0; i<6; i++){
        c.rx[a][i] += c.dx[a][i];
        c.ry[a][i] += c.dy[a][i];
      }
    }

    for(int a=0; a<N; a++){ // 細胞数
      for(int i=0; i<6; i++){ // 頂点数
        // 各頂点を結ぶ
        double LINE[2][2] = {{c.rx[a][i],c.ry[a][i]},{c.rx[a][(int)((i+1)%6)],c.ry[a][(int)((i+1)%6)]}};
        mp.lines(LINE,Mplot::black, 1, false);
      }
    }
    mp.SetPicture_end();
    t += 0.01;
  }

  return 0;
}
