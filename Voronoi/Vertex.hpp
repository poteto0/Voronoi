#ifndef __CLASS__VERTEX

#include "Point.hpp"
#include <iostream>
#include <cmath>

#define __CLASS__VERTEX

class Vertex : public Point { // Pointクラスのpublic派生Vertexクラス
public:
  // コンストラクタ //
  Vertex(float xx=0, float yy=0){
    x = xx;
    y = yy;
  }

  //--- 距離計算 ---//
  // Point && Point //
  double abs(const Point &a, const Point &b){
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
  }
  // Vertex && Point //
  double abs(const Vertex &a, const Point &b){
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
  }
  // Point && Vertex //
  double abs(const Point &a, const Vertex &b){
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
  }
  // Vertex && Vertex //
  double abs(const Vertex &a, const Vertex &b){
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
  }

  // 2点の間にあるかどうか //
  bool isBetween(const Vertex &a, const Point &b){
    return ((a.x < this->x && this->x < b.x) || (b.x < this->x && this->x < a.x));
  }

  //--- 演算子 ---//
  // 2項+演算子 Vertex + Vetex //
  friend Vertex operator+(const Vertex& v1, const Vertex& v2){
    return Vertex(v1.x + v2.x, v1.y + v2.y);
  }
  // 2項+演算子 Vertex + Point //
  friend Vertex operator+(const Vertex& v1, const Point& p1){
    return Vertex(v1.x + p1.x, v1.y + p1.y);
  }
  // 2項-演算子 Vertex + Vetex //
  friend Vertex operator-(const Vertex& v1, const Vertex& v2){
    return Vertex(v1.x - v2.x, v1.y - v2.y);
  }
  // 2項-演算子 Vertex + Point //
  friend Vertex operator-(const Vertex& v1, const Point& p1){
    return Vertex(v1.x - p1.x, v1.y - p1.y);
  }
  // 2項比較演算子 Vertex == Vertex//
  friend bool operator==(const Vertex& v1, const Vertex& v2){
    return (std::abs(v1.x-v2.x)<0.001 && std::abs(v1.y-v2.y)<0.001);
  }
  // 2項比較演算子 Vertex != Vertex//
  friend bool operator!=(const Vertex& v1, const Vertex& v2){
    return !(v1==v2);
  }
  // 2項比較演算子 Vertex == Point/
  friend bool operator==(const Vertex& v1, const Point& p1){
    return (std::abs(v1.x-p1.x)<0.001 && std::abs(v1.y-p1.y)<0.001);
  }
  // 2項比較演算子 Vertex != Point//
  friend bool operator!=(const Vertex& v1, const Point& p1){
    return !(v1==p1);
  }
  // 代入演算子 //
  Vertex& operator=(const Vertex& v1){
    x = v1.x;
    y = v1.y;
    return *this;
  }
};

//--- 出力ストリームにsを挿入 ---//
inline std::ostream& operator<<(std::ostream& s, const Vertex& v){
  return s << "(x,y) = (" << v.x << "," << v.y << ")";
}

#endif
