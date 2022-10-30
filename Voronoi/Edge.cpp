#include "Edge.hpp"

// EdgeにVertexを設定 //
Edge Edge::set(Vertex v){
  if((!isVertex[0] && isVertex[1]) || (isVertex[0] && !isVertex[1])){ // 片方のみ頂点がある場合
    // バーテックス頂点出ない方を優先 //
    if(!isVertex[1]){
      rx[1] = v;
      isVertex[1] = true;
    }else if(!isVertex[0]){
      rx[0] = v;
      isVertex[0] = true;
    }
  } else { // 両方ともバーテックス頂点である/でない時
    // エッジにVertexを設定(2つあるVertexから近い方を設定) //
    if(v.abs(rx[0], v) >= v.abs(rx[1], v)){
      rx[1] = v;
      isVertex[1] = true;
    }
    else{
      rx[0] = v;
      isVertex[0] = true;
    }
  }
  return *this;
}

Edge Edge::setVertex(Vertex v1, Vertex v2){
  rx[0] = v1;
  rx[1] = v2;
  isVertex[0] = true;
  isVertex[1] = true;
  a = - (rx[1].y - rx[0].y);
  b = (rx[1].x - rx[0].x);
  c = - a*rx[0].x - b*rx[0].y;

  return *this;
}

Edge Edge::setCell(int cell1, int cell2){
  cell[0] = cell1;
  cell[1] = cell2;

  return *this;
}

// 辺の中点を取る //
Point Edge::Mean(){
  return Point((rx[0].x+rx[1].x)/2,(rx[0].y+rx[1].y)/2);
}
Vertex Edge::Mean(bool flag){
  return Vertex((rx[0].x+rx[1].x)/2,(rx[0].y+rx[1].y)/2);
}

// 交点座標の取得 //
// 交点の取得 //
Vertex Edge::getIntersection(Edge edge1){
  Vertex rx;
  rx.x = (edge1.b * c - b * edge1.c) / (edge1.a * b - a * edge1.b);
  rx.y = (edge1.a * c - a * edge1.c) / (a* edge1.b - edge1.a * b);
  return rx;
}
Vertex Edge::getIntersection(Edge edge1, Edge edge2){
  Vertex rx;
  rx.x = (edge2.b * edge1.c - edge1.b * edge2.c) / (edge2.a * edge1.b - edge1.a * edge2.b);
  rx.y = (edge2.a * edge1.c - edge1.a * edge2.c) / (edge1.a * edge2.b - edge2.a * edge1.b);
  return rx;
}

// 平行かどうか //
bool Edge::isParallel(Edge edge1){
  if((edge1.a == 0 && a == 0) || (edge1.b == 0 && b == 0)) return true;
  return ((edge1.a / edge1.b) == (a / b));
}
bool Edge::isParallel(Edge edge1, Edge edge2){
  if((edge1.a == 0 && edge2.a == 0) || (edge1.b == 0 && edge2.b == 0)) return true;
  return ((edge1.a / edge1.b) == (edge2.a / edge2.b));
}

// 交点を持つかどうか //
bool Edge::isCross(Edge edge1){
  // 平行でなければ交点がバーテックス頂点の間にいるか判断 //
  if(!(this->isParallel(edge1)) && (edge1.isVertex[0] && edge1.isVertex[1]) && (isVertex[0] && isVertex[1])){
    Vertex v = this->getIntersection(edge1); // 交点を取得
    // 交点の位置から判断
    return (((v.x > edge1.rx[0].x && v.x < edge1.rx[1].x) || (v.x > edge1.rx[1].x && v.x < edge1.rx[0].x)) \
           && ((v.x > rx[0].x && v.x < rx[1].x) || (v.x > rx[1].x && v.x < rx[0].x))) || \
           (((v.y > edge1.rx[0].y && v.y < edge1.rx[1].y) || (v.y > edge1.rx[1].y && v.y < edge1.rx[0].y)) \
           && ((v.y > rx[0].y && v.y < rx[1].y) || (v.y > rx[1].y && v.y < rx[0].y)));
  }else if(!(this->isParallel(edge1)) && (edge1.isVertex[0] && edge1.isVertex[1])){
    Vertex v = this->getIntersection(edge1); // 交点を取得
    // 交点の位置から判断
    return ((v.x > edge1.rx[0].x && v.x < edge1.rx[1].x) || (v.x > edge1.rx[1].x && v.x < edge1.rx[0].x));
  } else if(!(this->isParallel(edge1)) && (isVertex[0] && isVertex[1])){
    Vertex v = this->getIntersection(edge1); // 交点を取得
    // 交点の位置から判断
    return ((v.x > rx[0].x && v.x < rx[1].x) || (v.x > rx[1].x && v.x < rx[0].x));
  } else return (!(this->isParallel(edge1)));
}

bool Edge::isCross(Edge edge1, Edge edge2){
  // 平行でなければ交点がバーテックス頂点の間にいるか判断 //
  if(!isParallel(edge1, edge2) && \
  ((edge1.isVertex[0] && edge1.isVertex[1])) || ((edge2.isVertex[0] && edge2.isVertex[1]))){
    Vertex v = getIntersection(edge1, edge2);
    if((v.x > edge1.rx[0].x && v.x < edge1.rx[1].x) || (v.x > edge1.rx[1].x && v.x < edge1.rx[0].x)) return true;
    else if((v.x > edge2.rx[0].x && v.x < edge2.rx[1].x) || (v.x > edge2.rx[1].x && v.x < edge2.rx[0].x)) return true;
    else return false;
  } else return (!isParallel(edge1, edge2));
}

// エッジとの間に別のエッジがあるかどうか //
// 所属したセルを分け隔てるエッジか
// Yes -> 別のエッジはない
// No -> 別のエッジがある
bool Edge::isDevid(int which){
  bool flag = false;
  for(int i=0; i<2; i++){
    if(this->cell[i] == which) flag = true;
  }
  return flag;
}
