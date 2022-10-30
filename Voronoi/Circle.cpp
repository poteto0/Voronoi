#include "Circle.hpp"

// コンストラクタ //
Circle::Circle(Point cc, float rr){
  cx = cc;
  r = rr;
}

// コンストラクタ(三角形から) //
Circle::Circle(Triangle tri){
  setCircle(tri);
}

// 三角形の外接円をセット //
void Circle::setCircle(Triangle tri){
  cx = tri.vertex;
  r = sqrt((tri.cx[0].x - cx.x)*(tri.cx[0].x - cx.x) + (tri.cx[0].y - cx.y)*(tri.cx[0].y - cx.y));
}

// 円の内部に含まれるか //
bool Circle::isIncluded(Point p){
  float dis = sqrt((cx.x - p.x)*(cx.x - p.x) + (cx.y - p.y)*(cx.y - p.y));
  return (dis < 0.999*r);
}
