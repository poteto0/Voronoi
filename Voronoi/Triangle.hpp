#ifndef __CLASS__TRIANGLE

#include <cmath>
#include <iostream>
#include "Edge.cpp"
#include "Vertex.hpp"

#define __CLASS__TRIANGLE

class Triangle {
public:
  Vertex vertex;
  Point *cx; // 三角形の頂点
  Edge *edge; // 三角形の辺
  int *cell;

  // コンストラクタ //
  Triangle(const Point p1, const Point p2, const Point p3){
    cx = new Point [3];
    edge = new Edge [3];
    cx[0] = p1;
    cx[1] = p2;
    cx[2] = p3;
    edge[0] = Edge(p1, p2);
    edge[1] = Edge(p2, p3);
    edge[2] = Edge(p3, p1);
    for(int i=0; i<3; i++){
      for(int j=0; j<2; j++){
        edge[i].isVertex[j] = true;
      }
    }
    vertex = this->getCircumcenter();
  }
  ~Triangle(){// デストラクタ
    cx = new Point [3];
    edge = new Edge [3];
    cell = new int [3];
    if(cx != NULL) delete [] cx;
    if(edge != NULL) delete [] edge;
    if(cell != NULL) delete [] cell;
  }

  // セルの設定 //
  void set(int i1, int i2, int i3){
    cell = new int [3];
    cell[0] = i1;
    cell[1] = i2;
    cell[2] = i3;
    edge[0].cell[0] = i1;
    edge[0].cell[1] = i2;
    edge[1].cell[0] = i2;
    edge[1].cell[1] = i3;
    edge[2].cell[0] = i3;
    edge[2].cell[1] = i1;
  }

  // 三角形の外心を取得 //
  Vertex getCircumcenter();

  // 面積の計算 //
  float Area();

  // どのセルを分けてるか //
  int *DevidedCell(Edge e);

  // 点が内部にあるか //
  bool isIncluded(Vertex v);
  bool isIncluded(Point p); // Point

  // 点が直線上にあるか //
  bool isOnEdge(Vertex v);
  bool isOnEdge(Point p); // Point

  // 外心取得のためのラムダ計算 //
  double getLambda(const double l_i, const double l_j, const double l_k){
    return l_i*l_i*(l_j*l_j+l_k*l_k-l_i*l_i);
  }

  // 三角形が交差するか(頂点以外で) //
  bool isCross(const Triangle tri);

  // 三角形が内部にあるか //
  bool isInvolved(const Triangle tri);

  // 三角形が隣り合うか //
  bool isNeighbor(const Triangle tri);

  // 各辺へ垂直二等分線を引く //
  std::vector<Edge> PB(const int index[]);

  // 2項比較演算子 //
  friend bool operator==(const Triangle& t1, const Triangle& t2){
    bool flag = false;
    flag = (t1.cx[0] == t2.cx[0] || t1.cx[0] == t2.cx[1] || t1.cx[0] == t2.cx[2]) && \
           (t1.cx[1] == t2.cx[0] || t1.cx[1] == t2.cx[1] || t1.cx[1] == t2.cx[2]) && \
           (t1.cx[2] == t2.cx[0] || t1.cx[2] == t2.cx[1] || t1.cx[2] == t2.cx[2]);
    return flag;
  }

  // 2項比較演算子 //
  friend bool operator!=(const Triangle& t1, const Triangle& t2){
    return !(t1 == t2);
  }

};

//--- 出力ストリームにsを挿入 ---//
inline std::ostream& operator<<(std::ostream& s, const Triangle& tri){
  return s << "点1:" << tri.cx[0] << '\n' << "点2:" << tri.cx[1] << '\n' \
   << "点3:" << tri.cx[2] << '\n' << "外心: " << tri.vertex;
}

#endif
