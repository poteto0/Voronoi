#include "getObject.hpp"

template<typename V> Voronoi getObject::getVoronoi(const std::vector<V> cx){
	// まず三角形を描ききってそれからエッジや頂点を求める //
	Voronoi vor; // ボロノイ
	std::vector<Edge> edge; // エッジ
	std::vector<Vertex> vertex; // 頂点
	std::vector<Triangle> tris; // 三角形
	std::vector<Cell> cell(10,Point(0,0)); // セル(中心座標、頂点座標、エッジ)

	tris = this->getDelaunay(cx); // ドロネイ三角形を取得

	// 隣り合う三角形の外心を結ぶ //
	for(int i=0; i<tris.size(); i++){
		Vertex v = tris[i].vertex;
		vertex.push_back(v);
		for(int j=0; j<3; j++){
			if(tris[i].cell[j] != -1){
				cell[tris[i].cell[j]].cx = tris[i].cx[j]; // セルの中心座標を変える
				cell[tris[i].cell[j]].addVertex(v);
			}
		}
		if(isOutsideTriangle(tris[i])){
			// 外側にある場合には境界方向に引く -> 一旦コメントアウト
			int which = OutsideEdge(tris[i]);
			Edge e;
			e.setVertex(v, tris[i].edge[which].Mean(true));
			// 追加
			edge.push_back(e);
		}
		for(int j=i+1; j<tris.size(); j++){
			if(tris[i].isNeighbor(tris[j])){
				Edge e;
				e.setVertex(v, tris[j].vertex);
				int *which_cell = tris[i].DevidedCell(e);
				e.setCell(which_cell[0], which_cell[1]);
				// 追加
				edge.push_back(e);
				if(which_cell[0]!=-1)cell[which_cell[0]].addEdge(e);
				if(which_cell[1]!=-1)cell[which_cell[1]].addEdge(e);
			}
		}
	}

	vor.triangle = tris;
	vor.edge = edge;
	vor.vertex = vertex;
	vor.cell = cell;

	// メモリ解放 //
	std::vector<Triangle>().swap(tris);
	std::vector<Edge>().swap(edge);
	std::vector<Vertex>().swap(vertex);
	std::vector<Cell>().swap(cell);

	return vor;
}

// ドロネーイの三角形取得 //
template<typename V> std::vector<Triangle> getObject::getDelaunay(const std::vector<V> cx){
	std::vector<Triangle> tris; // 三角形
	std::vector<Point> pts; // 母点集合
	int n=6; // 三角形を6個作る
	pts.push_back(cx[0]);
	float area = 0;
	float area_sum = 0;

	// ①境界の三角形//
	for(int i=0; i<n; i++){
    float x1 = 30+20*cos(2.0 * M_PI * ((float)i/n));
    float y1 = 30+20*sin(2.0 * M_PI * ((float)i/n));
		float x2 = 30+20*cos(2.0 * M_PI * ((float)((int)(i+1)%n)/n));
    float y2 = 30+20*sin(2.0 * M_PI * ((float)((int)(i+1)%n)/n));
		Triangle tri(Point(x1,y1), Point(x2,y2) , cx[0]);
		area += tri.Area();
		area_sum += tri.Area();
		pts.push_back(Point(x1,y1));
		tri.set(-1,-1,0); // 境界は-1
		tris.push_back(tri); // 三角形を追加
  }

	// 母点のPop //
	for(int i=1; i<cx.size(); i++){
		pts.push_back(cx[i]); // 母点をPop
		// ②選ばれた点が三角形内にあるかどうか //
		auto i_tri = tris.begin();
		while(i_tri != tris.end()){
			if(i_tri->isIncluded(cx[i])){ // Yes
				// その点で三角形を分割して3つの三角形を作る //
				std::vector<Triangle> tri_test;
				for(int j=0; j<3; j++){
					Triangle tri_new(i_tri->cx[j], i_tri->cx[(int)(j+1)%3], cx[i]);
					tri_new.set(i_tri->cell[j], i_tri->cell[(int)(j+1)%3], i);
					tri_test.emplace_back(tri_new);
					area_sum += tri_new.Area(); // 総面積に追加
				}
				// 削除
				area_sum -= i_tri->Area();
				i_tri = tris.erase(i_tri);
				tris.insert(tris.end(), tri_test.begin(), tri_test.end());
				// メモリ解放 //
				std::vector<Triangle>().swap(tri_test);
				break;
			} else if(i_tri->isOnEdge(cx[i])){ // 線上に現れたとき
				// その点で三角形を分割して2つの三角形を作る //
				std::vector<Triangle> tri_test;
				for(int j=0; j<3; j++){
					if(!(i_tri->edge[j].a*cx[i].x + i_tri->edge[j].b*cx[i].y + i_tri->edge[j].c == 0)){
						Triangle tri_new(i_tri->cx[j], i_tri->cx[(int)(j+1)%3], cx[i]);
						tri_new.set(i_tri->cell[j], i_tri->cell[(int)(j+1)%3], i);
						tri_test.push_back(tri_new);
						area_sum += tri_new.Area();
					}
				}
				area_sum -= i_tri->Area();
				i_tri = tris.erase(i_tri);
				tris.insert(tris.end(), tri_test.begin(), tri_test.end());
				// メモリ解放 //
				std::vector<Triangle>().swap(tri_test);
				break;
			} else i_tri++;
		}
	}

	// 違反三角形があったときにflipして新たな三角形を追加 //
	std::vector<Triangle> tri_test;
	int num = 0; // ptsのイテレータ
	for(auto& pt: pts){
		auto i_tri = tris.begin();
		while(i_tri != tris.end()){
			Circle cir(*i_tri);
			if(cir.isIncluded(pt)){
				for(int i=0; i<3; i++){
					Triangle tri_new(pt, i_tri->cx[i], i_tri->cx[(int)(i+1)%3]);
					int index = 0;
					if(num == 0) index = 0;
					else if(num<7) index = -1;
					else index = num - 6;
					tri_new.set(index, i_tri->cell[i], i_tri->cell[(int)(i+1)%3]);
					bool flag = false;
					for(auto tri: tri_test){
						if(tri == tri_new) flag = true;
					}
					if(!flag){
						area_sum += tri_new.Area();
						tri_test.push_back(tri_new);
					}
				}
			}
			i_tri++;
		}
		num++;
	}
	tris.insert(tris.end(), tri_test.begin(), tri_test.end());
	// 違反三角形の削除
	auto i_tri2 = tris.begin();
	while(i_tri2 != tris.end()){
		Circle cir(*i_tri2);
		for(auto& pt: pts){
			if(cir.isIncluded(pt)){
				area_sum -= i_tri2->Area();
				i_tri2 = tris.erase(i_tri2);
				--i_tri2;
				break;
			}
		}
		++i_tri2;
	}

	// 重複の削除
	i_tri2 = tris.begin();
	for(int i=0; i<tris.size(); i++){
		for(int j=i+1; j<tris.size(); j++){
			if(tris[i] == tris[j]){
				i_tri2 = tris.erase(i_tri2);
				--i_tri2;
			}
		}
		++i_tri2;
	}
	int count = 0;
	while(abs(area-area_sum)>1&&count <2){
		num = 0; // ptsのイテレータ
		for(auto& pt: pts){
			for(auto& tri: tri_test){
				Circle cir(tri);
				if(cir.isIncluded(pt)){
					for(int i=0; i<3; i++){
						Triangle tri_new(pt, tri.cx[i], tri.cx[(int)(i+1)%3]);
						int index = 0;
						if(num == 0) index = 0;
						else if(num<7) index = -1;
						else index = num - 6;
						tri_new.set(index, tri.cell[i], tri.cell[(int)(i+1)%3]);
						bool flag = false;
						for(auto test: tris) if(test == tri_new) flag = true;
						if(!flag){
							area_sum += tri_new.Area();
							tris.push_back(tri_new);
							break;
						}
					}
				}
			}
			num++;
		}
		// 違反三角形の削除
		auto i_tri3 = tris.begin();
		while(i_tri3 != tris.end()){
			Circle cir(*i_tri3);
			for(auto& pt: pts){
				if(cir.isIncluded(pt)){
					std::cout << "違反->削除" << std::endl;
					area_sum -= i_tri3->Area();
					i_tri3 = tris.erase(i_tri3);
					--i_tri3;
					break;
				}
			}
			++i_tri3;
		}
		// 重複の削除
		i_tri3 = tris.begin();
		for(int i=0; i<tris.size(); i++){
			for(int j=i+1; j<tris.size(); j++){
				if(tris[i] == tris[j]){
					std::cout << "重複->削除" << std::endl;
					i_tri3 = tris.erase(i_tri3);
					--i_tri3;
				}
			}
			++i_tri3;
		}
		std::cout << "=> AREA:" << area_sum << "DEFF" << abs(area-area_sum) << std::endl;
		count++;
	}

	// メモリ解放 //
	std::vector<Point>().swap(pts);
	std::vector<Triangle>().swap(tri_test); // メモリ解放

	return tris;
}

// 三角形が1番外側にあるか //
bool getObject::isOutsideTriangle(Triangle tri){
	int count = 0;
	for(int i=0; i<3; i++){
		if(tri.cell[i] == -1) count++;
	}
	return (count==2);
}

// どの辺が外側にあるか //
int getObject::OutsideEdge(Triangle tri){
	int which = -1;
	for(int i=0; i<3; i++){
		if(tri.edge[i].cell[0] == -1 && tri.edge[i].cell[1] == -1) which = i;
	}
	return which;
}
