#ifndef __CLASS__CELL

#include <vector>
#include <iostream>
#include "Vertex.hpp"
#include "Edge.hpp"

#define __CLASS__CELL

class Cell { // 細胞クラス
public:
  Point cx; // 中心座標
  std::vector<Vertex> rx; // 頂点座標
  std::vector<Edge> edge; // Edge情報(2次元配列[細胞番号, そこに所属する頂点])

  // コンストラクタ //
  Cell(Point p){
    cx = p;
  }
  // デストラクタ //
  ~Cell(){
    std::vector<Vertex>().swap(rx);
    std::vector<Edge>().swap(edge);
  }

  void addVertex(Vertex v){
    rx.push_back(v);
  }

  void addEdge(Edge e){
    edge.push_back(e);
  }
};

//--- 出力ストリームにsを挿入 ---//
inline std::ostream& operator<<(std::ostream& s, const Cell& c){
  return s << "中心座標:" << c.cx << '\n' \
           << "頂点座標:" << c.rx[0] << '\n' \
           << "エッジ:" << c.edge[0];
}

#endif
