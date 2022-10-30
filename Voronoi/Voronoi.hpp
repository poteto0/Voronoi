#ifndef __CLASS__VORONOI

#include <vector>
#include "Vertex.hpp"
#include "Edge.hpp"
#include "Cell.hpp"
#include "Triangle.hpp"

#define __CLASS__VORONOI

class Voronoi{
public:
  std::vector<Edge> edge; // ボロノイ辺
	std::vector<Vertex> vertex; // ボロノイ頂点
  std::vector<Cell> cell; // ボロノイセル
  std::vector<Triangle> triangle; // ドロネー三角形

  ~Voronoi(){ // デストラクタ
    std::vector<Edge>().swap(edge);
    std::vector<Vertex>().swap(vertex);
    std::vector<Triangle>().swap(triangle);
  }

  // 最短距離のセルを取得 //
  int getNearestCell(const int index, const std::vector<Point> cx, const Point p1);

  // 分け隔てらているかどうか //
  bool isDevide(const int index, const int which,  const std::vector<Point> cx, const std::vector<Edge> e);

  // ボロノイセルを再構成 //
  std::vector<Point> reconstructCell(const int which, std::vector<Point> cx);

  // 所属するボロノイセルを求める //
  int getWhichCell(const int index, std::vector<Point> cx, const Point p1, const std::vector<Edge> e);

};

#endif
