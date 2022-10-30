#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <unistd.h>
#include "Mplot/Mplot.cpp"
#include "Mplot/ColorRamp.cpp"
#include "OutputToCSV/OutputToCSV.cpp"
#include "Calculator/Calculator.cpp"
#include "Force/Force.cpp"
#include "InitialPosition/InputCSV.cpp"

// 使用宣言 //
using namespace std;

#define N 6 // 細胞数

// 制約
double S0 = 4.1; // 基準の高さ
double V0 = 10; // 基準体積
double h0 = 1; // 基準の高さ
// 変数
double k = 0.05; // 弾性係数[1/2スケール]
double tau = 0;//.12 * k * pow(S0, 3/2); // 張力
double rou = -0.00004 * k * S0; // 収縮性[1/2スケール]
double rate = 0.001;
// データ出力用 //
FILE *fp;
// 描画範囲
int width = 500;
int height = 500;

double t = 0; // 時間

// 細胞 //
typedef struct cell {
  double rx[6+(N-1)*3+1][5] = {}; // 各頂点の位置座標 [x,y,cell1,cell2,cell3]
  double dx[6+(N-1)*3+1][2] = {}; // 速度
  int edge[(3+N*4)*2][2] = {};
  int cell[N][6] = {};
  double perimeter[N] = {};
  double cx[N][2] = {}; // 中心座標
  double area[N] = {}; // 面積
  double (*elasticity)[2] = new double[6+(N-1)*3+1][2];
  double (*tension)[2] = new double[6+(N-1)*3+1][2];
  double (*contractility)[2] = new double[6+(N-1)*3+1][2];
  double V[N] = {}; // 体積
  double Us[N][6] = {}; // 界面エネルギーUs
  double Uv[N] = {};
} my_cell;

int main(int argc, char const *argv[]){ //main
  int count = 0;
  vector<double> ave_per;
  vector<double> ave_area;
  double per = 0;
  double are = 0;

  my_cell c;
  Mplot mp(width,height,-25,25,-25,25);
  OutputToCSV otcsv(fp, "test/");
  Calculator calculator(1);
  Force force(N, k, tau, rou);
  InputCSV inputcsv;

  //--- ファイル読み取り ---//
  double sum_rx = 0;
  double sum_ry = 0;
  string file_vertex = "data/vertex.csv";
  string file_edge = "data/edge.csv";
  string file_hex = "data/hex.csv";
  ifstream ifs(file_vertex);
  ifstream ifs2(file_edge);
  ifstream ifs3(file_hex);
  string line;
  while (getline(ifs, line)) {
    double *out = inputcsv.split(line, ',');
    for(int i=0; i<5;i++){
      c.rx[count][i] = out[i];
    }
    count ++;
  }
  count = 0;
  while (getline(ifs2, line)) {
    int *out = inputcsv.split(line, ',', true);
    c.edge[count][0] = out[0];
    c.edge[count][1] = out[1];
    count ++;
  }
  count = 0;
  while (getline(ifs3, line)) {
    int *out = inputcsv.split(line, ',', true, true);
    for(int i=0; i<6; i++) c.cell[count][i] = out[i];
    count ++;
  }

  while(t<2000){
    mp.SetPicture_first();
    per = 0;
    are = 0;

    for(int a=0; a<N; a++){
      c.perimeter[a] = calculator.E_perimeter(c.cell[a], c.rx); // 周囲長計算
      double *center = calculator.E_center(c.cell[a], c.rx); // 中心座標計算
      c.cx[a][0] = center[0];
      c.cx[a][1] = center[1];
      c.area[a] = calculator.E_area(c.cell[a], c.rx);
      are += c.area[a];
      per += c.perimeter[a];
    }

    ave_area.push_back(are/6);

    c.contractility = force.calculateContractility(c.cell, c.perimeter, c.rx);
    c.elasticity = force.calculateElasticity(c.cell, c.area, S0, c.rx);
    c.tension = force.calculateTension(c.edge, c.rx);
    for(int i=0; i<6+(N-1)*3+1; i++){
      //cout << "頂点:" << i << "(x,y) = (" << c.elasticity[i][0] << "," << c.elasticity[i][1] << ")" << endl;
      //c.tension[i] = tension[i];
      c.dx[i][0] = - (c.elasticity[i][0]);// + c.elasticity[i][0]);//(c.elasticity[i][0] + c.contractility[i][0] + c.tension[i][0]); //c.elasticity[i][0] c.contractility[i][0] + + c.tension[i];
      c.dx[i][1] = - (c.elasticity[i][1]);// + c.elasticity[i][1]);//(c.elasticity[i][1] + c.contractility[i][1] + c.tension[i][1]);//c.elasticity[i][1] c.contractility[i][1] + c.tension[i];
    }

    for(int i=0; i<6+(N-1)*3+1; i++){
      c.rx[i][0] += rate * c.dx[i][0];
      c.rx[i][1] += rate * c.dx[i][1];
    }

    for(int i=0; i<3+N*4; i++){
      double x1 = c.rx[c.edge[i][0]][0];
      double y1 = c.rx[c.edge[i][0]][1];
      double x2 = c.rx[c.edge[i][1]][0];
      double y2 = c.rx[c.edge[i][1]][1];
      // 各頂点を結ぶ
      double LINE[2][2] = {{x1,y1},{x2,y2}};
      mp.lines(LINE,Mplot::black, 1, false);
    }
    //usleep(1* 10000);
    //if((int)(t)%100 ==0) cout << t << endl;
    mp.SetPicture_end();
    t += 0.01;
  }

  otcsv.toCSV("perimeter",ave_area);

  return 0;
}
