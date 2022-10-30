#ifndef __CLASS__EDGE

#include "Vertex.hpp"
#include "Line.hpp"
#include <iostream>

#define __CLASS__EDGE

class Edge : public Line { // Lineクラスのpublic派生Edgeクラス
public:
  Vertex *rx = new Vertex[2]; // 2つの頂点座標を持つ
  bool *isVertex = new bool[2]; // 終端と末端はボロノイ頂点か
  int *cell = new int[2]; // エッジがどのボロノイセルを持つか

  // コンストラクタ Pointから //
  Edge(Point p1=0, Point p2=0){
    rx[0].x = p1.x, rx[0].y = p1.y;
    rx[1].x = p2.x, rx[1].y = p2.y;
    isVertex[0] = false, isVertex[1] = false;
    // 係数計算
    a = - (p2.y - p1.y);
    b = (p2.x - p1.x);
    c = - a*p1.x - b*p1.y;
  }
  ~Edge(){ // デストラクタ
    rx = nullptr;
    isVertex = nullptr;
    cell = nullptr;
  }

  // 2項比較演算子 //
  friend bool operator==(const Edge& e1, const Edge& e2){
    return ((e1.rx[0] == e2.rx[0]&&e1.rx[1] == e2.rx[1])||(e1.rx[1] == e2.rx[0]&&e1.rx[0] == e2.rx[1]));
  }
  // 2項比較演算子 //
  friend bool operator!=(const Edge& e1, const Edge& e2){
    return !(e1==e2);
  }

  // EdgeにVertexを設定 //
  Edge set(Vertex v);
  Edge setVertex(Vertex v1, Vertex v2);
  Edge setCell(int cell1, int cell2);

  // 辺の中点を取る //
  Point Mean();
  Vertex Mean(bool flag);

  // 交点座標の取得 //
  // 交点の取得 //
  Vertex getIntersection(Edge edge1);
  Vertex getIntersection(Edge edge1, Edge edge2);

  // 平行かどうか //
  bool isParallel(Edge edge1);
  bool isParallel(Edge edge1, Edge edge2);

  // 交点を持つかどうか //
  bool isCross(Edge edge1);
  bool isCross(Edge edge1, Edge edge2);

  // エッジとの間に別のエッジがあるかどうか //
  bool isDevid(int which);

};

//--- 出力ストリームにsを挿入 ---//
inline std::ostream& operator<<(std::ostream& s, const Edge& e){
  if(e.b > 0 && e.c > 0){
    return s << "vertex1:" << e.rx[0] << '\n' << "vertex2:" << e.rx[1] << '\n' \
             << "Cell1:" << e.cell[0] << " Cell2:" << e.cell[1];
  }else if(e.b < 0 && e.c > 0){
    return s << "vertex1:" << e.rx[0] << '\n' << "vertex2:" << e.rx[1] << '\n' \
             << "Cell1:" << e.cell[0] << " Cell2:" << e.cell[1];
  }else{
    return s << "vertex1:" << e.rx[0] << '\n' << "vertex2:" << e.rx[1] << '\n' \
             << "Cell1:" << e.cell[0] << " Cell2:" << e.cell[1];
  }
}

#endif
