#ifndef __CLASS__GET__OBJECT

#include <vector>
#include <algorithm>  // std::unique
#include "Voronoi.cpp"
#include "Cell.hpp"
#include "Triangle.cpp"
#include "Circle.cpp"

#define __CLASS__GET__OBJECT

// オブジェクト取得 //
class getObject{
public:
  // コンストラクタ //
  getObject(){}

  // ボロノイの取得 //
  template<typename V> Voronoi getVoronoi(const std::vector<V> cx);

  // ドロネーイの三角形取得 //
  template<typename V> std::vector<Triangle> getDelaunay(const std::vector<V> cx);

  // 垂直二等分線の取得 //
  Edge getPB(const Point p1, const Point p2, const int index1, const int index2){
    // 中心座標
    Vertex mx = Vertex((p1.x + p2.x)/2, (p1.y + p2.y)/2);
		double vs = sqrt(pow((p1.x - p2.x),2.0) + pow((p1.y - p2.y),2.0)); // 直線長さ
		Point v; // 単位方向ベクトル
	  v.x = (p1.x - p2.x) / vs;
	  v.y = (p1.y - p2.y) / vs;
		Edge h; // 法線
		h.a = v.x;
	  h.b = v.y;
	  h.c = - mx.x * h.a - mx.y * h.b;
    h.rx[0] = Vertex(mx.x - v.y, mx.y + v.x);
    h.rx[1] = Vertex(mx.x + v.y, mx.y - v.x);
    h.cell[0] = index1; // エッジにボロノイセルを追加
    h.cell[1] = index2;

    return h;
  }

  // 三角形が1番外側にあるか //
  bool isOutsideTriangle(Triangle tri);

  // どの辺が外側にあるか //
  int OutsideEdge(Triangle tri);
};

#endif
