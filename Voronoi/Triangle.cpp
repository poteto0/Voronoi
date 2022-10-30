#include "Triangle.hpp"

// 三角形の外心を取得 //
Vertex Triangle::getCircumcenter(){
  Vertex v; // 距離計算用
  double l_i = v.abs(cx[0],cx[1]);
  double l_j = v.abs(cx[1],cx[2]);
  double l_k = v.abs(cx[2],cx[0]);
  double lambda1 = getLambda(l_i,l_j,l_k);
  double lambda2 = getLambda(l_j,l_i,l_k);
  double lambda3 = getLambda(l_k,l_i,l_j);
  Point rx_p = cx[2]*(lambda1/(lambda1+lambda2+lambda3)) + cx[0]*(lambda2/(lambda1+lambda2+lambda3)) + cx[1]*(lambda3/(lambda1+lambda2+lambda3));
  return Vertex(rx_p.x, rx_p.y);
}

// 面積の計算 //
float Triangle::Area(){
  float area = abs((cx[1].x-cx[0].x)*(cx[2].y-cx[0].y)-(cx[2].x-cx[0].x)*(cx[1].y-cx[0].y));
  return area/2;
}

// どのセルを分けてるか //
int *Triangle::DevidedCell(Edge e){
  int *which_cell = new int [2];
  int which = -1;
  for(int i=0; i<3; i++){
    if(e.isCross(edge[i])) which = i;
  }
  if(which == -1){
    which_cell[0] = -1;
    which_cell[1] = -1;
  }else{
    which_cell = edge[which].cell;
  }
  return which_cell;
}

// 点が内部にあるか //
bool Triangle::isIncluded(Vertex v){
  float area = this->Area(); // 三角形の面積
  float area_sum = 0; // 内部の点と三角形の3頂点により作られる三角形の面積の合計
  Point p; // 頂点をポイントに変換
  bool flag = this->isOnEdge(v); // 線上にあるか
  p.x = v.x;
  p.y = v.y;
  for(int i=0; i<3; i++){
    Triangle tri(cx[i],cx[(int)(i+1)%3], p);
    area_sum += tri.Area();
  }
  return (area == area_sum && !flag); // 等しかったら内部または線上にある
}

// 点が内部にあるか(Point) //
bool Triangle::isIncluded(Point p){
  float area = this->Area(); // 三角形の面積
  float area_sum = 0; // 内部の点と三角形の3頂点により作られる三角形の面積の合計
  bool flag = this->isOnEdge(p); // 線上にあるか
  for(int i=0; i<3; i++){
    Triangle tri(cx[i],cx[(int)(i+1)%3], p);
    area_sum += tri.Area();
  }
  return (abs(area - area_sum) < 0.001 * area) && (!flag); // 等しかったら内部または線上にある
}

// 点が直線上にあるか //
bool Triangle::isOnEdge(Vertex v){
  bool flag = false;
  for(int i=0; i<3; i++){
    if((edge[i].a*v.x + edge[i].b*v.y + edge[i].c == 0) && v.isBetween(edge[i].rx[0], edge[i].rx[1])){
      flag = true;
      break;
    }
    if(flag) break;
  }
  return flag;
}

// 点が直線上にあるか(Point) //
bool Triangle::isOnEdge(Point p){
  bool flag = false;
  for(int i=0; i<3; i++){
    if(edge[i].a*p.x + edge[i].b*p.y + edge[i].c == 0){
      flag = true;
      break;
    }
  }
  return flag;
}

// 三角形が交差するか(頂点以外で) //
bool Triangle::isCross(const Triangle tri){
  bool flag = false;
  for(int i=0; i<3; i++){
    for(int j=0; j<3; j++){
      if(edge[i].isCross(tri.edge[j])){
        Vertex v = edge[i].getIntersection(tri.edge[j]);
        flag = !(v == cx[0] || v == cx[1] || v == cx[2]);
        if(flag) break;
      }
    }
    if(flag) break;
  }
  return flag;
}

// 三角形が内部にあるか //
// 全ての点が内部または線上にある -> 三角形が内部にある
bool Triangle::isInvolved(const Triangle tri){
  bool flag = (this->isOnEdge(tri.cx[0]) || this->isIncluded(tri.cx[0])) && \
              (this->isOnEdge(tri.cx[1]) || this->isIncluded(tri.cx[1])) && \
              (this->isOnEdge(tri.cx[2]) || this->isIncluded(tri.cx[2]));
  return flag;
}

// 各辺へ垂直二等分線を引く //
std::vector<Edge> Triangle::PB(const int *index){
  std::vector<Edge> pbs(3);
  if(this->isIncluded(vertex)){ // 外心が内部にあれば前と同じ
    for(int i=0; i<3; i++){
      Edge pb(edge[i].Mean(), vertex);
      pb.cell[0] = index[i];
      pb.cell[1] = index[(int)(i+1)%3];
      pb.isVertex[0] = true; // 外心側のみTrue
      pbs[i] = pb;
    }
  } else { // 外心が外部にあるなら(線上を含む)
    std::vector<Point> ps;
    for(int i=0; i<3; i++){
      ps.push_back(edge[i].Mean()); // 終点
    }
    for(int i=0; i<3; i++){
      Triangle tri(ps[(int)(i+2)%3], ps[(int)(i+1)%3], vertex); // 終点2つと外心で三角形を作る
      if(tri.isIncluded(ps[i])){ // 対象の終点が三角形の中にいるか
        //Yes -> 逆側に出す
        ps[i].x = 2*vertex.x - ps[i].x;
        ps[i].y = 2*vertex.y - ps[i].y;
        Edge pb(ps[i], vertex);
        pb.cell[0] = index[i];
        pb.cell[1] = index[(int)(i+1)%3];
        pb.isVertex[0] = true; // 外心側のみTrue
        pbs[i] = pb;
      }else{ //No -> 今まで通り
        Edge pb(ps[i], vertex);
        pb.cell[0] = index[i];
        pb.cell[1] = index[(int)(i+1)%3];
        pb.isVertex[0] = true; // 外心側のみTrue
        pbs[i] = pb;
      }
    }
  }
  return pbs;
}

// 三角形が隣り合うか //
bool Triangle::isNeighbor(const Triangle tri){
  bool flag = false;
  for(int i=0; i<3; i++){
    for(int j=0; j<3; j++){
      if(this->edge[i] == tri.edge[j]){
        flag = true;
      }
    }
  }

  return flag;
}
