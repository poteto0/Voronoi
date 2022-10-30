#ifndef CLASS__CIRCLE

#include "Point.hpp"
#include <cmath>

#define CLASS__CIRCLE

class Circle
{
public:
  Point cx; // 中心座標
  float r; // 半径

  // コンストラクタ //
  Circle(Point cc, float rr);

  // コンストラクタ(三角形から) //
  Circle(Triangle tri);

  // 三角形の外接円をセット //
  void setCircle(Triangle tri);

  // 円の内部に含まれるか //
  bool isIncluded(Point p);

};

#endif
