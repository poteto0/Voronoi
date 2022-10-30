#ifndef __CLASS__POINT

#define __CLASS__POINT

class Point{ // 点クラス
public:
  float x; // x座標
  float y; // y座標

  // コンストラクタ //
  Point(double xx=0, double yy=0){
    x = xx;
    y = yy;
  }

  //--- 演算子 ---//
  // 2項*演算子 Point * double//
  friend Point operator*(const Point& p1, const double a){
    return Point(p1.x * a, p1.y * a);
  }
  // 2項+演算子 Point + Point //
  friend Point operator+(const Point& p1, const Point& p2){
    return Point(p1.x + p2.x, p1.y + p2.y);
  }
  // 2項比較演算子 Point == Point/
  friend bool operator==(const Point& p1, const Point& p2){
    return (abs(p1.x-p2.x) < 0.001 && abs(p1.y-p2.y)<0.001);
  }
  // 2項比較演算子 Point != Point//
  friend bool operator!=(const Point& p1, const Point& p2){
    return !(p1==p2);
  }
};

//--- 出力ストリームにsを挿入 ---//
inline std::ostream& operator<<(std::ostream& s, const Point& p){
  return s << "(x,y) = (" << p.x << "," << p.y << ")";
}

#endif
