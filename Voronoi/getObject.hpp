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
    // 単位方向ベクトル
		Point v = Point((p1.x - p2.x) / vs, (p1.y - p2.y) / vs);
    // 法線
		Edge h(Point(mx.x - v.y, mx.y + v.x), Point(mx.x + v.y, mx.y - v.x));
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
