#include "Voronoi.hpp"

// 最短距離のセルを取得 //
int Voronoi::getNearestCell(const int index, const std::vector<Point> cx, const Point p1){
	Vertex v; // 距離計算用
	// 新しい点が含まれるボロノイセルを求める => 1番近い中心座標を求める //
	double min_dis = v.abs(cx[0], p1); // 最短距離
	int which = 0; // 最も近い中心座標の番号
  if(cx[0] == p1){
    min_dis = v.abs(cx[1], p1);
    which = 1;
  }
	for(int i=1; i<cx.size()-1; i++){
		if(min_dis > v.abs(cx[i], p1) && cx[i] != p1){ // 同じ点でなければ
			min_dis = v.abs(cx[i], p1);
      which = i;
		}
	}
	return which;
}

// 分け隔てらているかどうか //
bool Voronoi::isDevide(const int index, const int which, const std::vector<Point> cx, const std::vector<Edge> e){
	bool flag = false;
	Edge edge(cx[index], cx[which]);
	edge.isVertex[0] = true;
	edge.isVertex[1] = true;
	for(int i=0; i<e.size()-1; i++){
		if((e[i].cell[0] == which && e[i].cell[1] == index)||(e[i].cell[0] == index && e[i].cell[1] == which)){
			flag = true;
		} else if(edge.isCross(e[i])){
			std::cout << "CROSS" << std::endl;
			flag = true;
		}
	}
	return flag;
}

// ボロノイセルを再構成 //
std::vector<Point> Voronoi::reconstructCell(const int which, std::vector<Point> cx){
	std::vector<Point> val(cx.size());
	for(int i=0; i<cx.size(); i++){
    if(i==which) val[i] = Point(1000,1000);
    else val[i] = cx[i];
	}
	return val;
}

// 所属するボロノイセルを求める //
int Voronoi::getWhichCell(const int index, std::vector<Point> cx, const Point p1, const std::vector<Edge> e){
	int which = getNearestCell(index, cx, p1); // 最短距離のセルを求める
	bool flag = isDevide(index, which, cx, e); // 分け隔てられているか
	while(flag){ // falseになるまで
    cx = reconstructCell(which, cx);
		which = getNearestCell(index, cx, p1); // 最短距離のセルを求める
		flag = isDevide(index, which, cx, e); // 分け隔てられているか
	}
	return which;
}
