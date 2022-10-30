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
#include <random>
#include <algorithm>
#include "Mplot/Mplot.cpp"
#include "Mplot/ColorRamp.cpp"
#include "OutputToCSV/OutputToCSV.cpp"
#include "Calculator/Calculator.cpp"
#include "Voronoi/getObject.cpp"
#include "Voronoi/Cell.hpp"

// 使用宣言 //
using namespace std;

int main(int argc, char const *argv[]){ //main
  Mplot mp(500,500,0,60,0,60);
	getObject gObj;
	vector<Point> cx; // 中心座標配列

	random_device rd;
	mt19937 engine;
	std::uniform_int_distribution<float> randomx(20, 40);
	std::uniform_int_distribution<float> randomy(20, 40);

  // ランダム座標 //
	for(int i=0; i<10; i++){
    engine.seed(rd());
		cx.push_back(Point(randomx(engine),randomy(engine)));
  }

  Voronoi v = gObj.getVoronoi(cx);
	double t = 0;
  while(true){
    mp.SetPicture_first();

		for(int i=0; i<v.edge.size(); i++){
			double LINE[2][2] = {{v.edge[i].rx[0].x,v.edge[i].rx[0].y},{v.edge[i].rx[1].x,v.edge[i].rx[1].y}};
      mp.lines(LINE, Mplot::black, 1, false);
		}
    for(int i=0; i<v.cell.size(); i++){
      mp.polygon(mp.circle(v.cell[i].cx.x, v.cell[i].cx.y, 0.3), Mplot::black);
			float A[1][2] = {{v.cell[i].cx.x, v.cell[i].cx.y+1}};
			string d = to_string(i);
			mp.draw_text(d, A, Mplot::black);
    }
		for(int i=0; i<v.vertex.size(); i++)
			mp.polygon(mp.circle(v.vertex[i].x, v.vertex[i].y, 0.5), Mplot::red);
    mp.SetPicture_end();
		t+=0.1;
  }

	std::vector<Point>().swap(cx);

	//system("leaks a.out");

  return 0;
}
