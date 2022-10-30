#include "OutputToCSV.hpp"

#include <iostream>
#include <stdio.h>

//--- コンストラクタ定義 ---//
OutputToCSV::OutputToCSV(FILE *ff, std::string pn){
  set(ff, pn);
}

//--- デストラクタ ---//
OutputToCSV::~OutputToCSV(){
  if(fname != NULL) delete[] fname;
  if(fp != NULL) delete[] fp;
}

//--- セッタ ---//
void OutputToCSV::set(FILE *ff, std::string pn){
  fp = ff;
  path = pn;
  fname = new char[36];
}

//--- ファイル名に入れる変数のセッタ ---//
void OutputToCSV::set(int ll, int ee, int pp){
  l = ll;
  epoch = ee;
  pattern = pp;
}

//--- int型:2つ、bool型:1つ ---//
void OutputToCSV::toCSV(std::string a_t, std::string b_t, std::string c_t, std::vector<int> &a, std::vector<int> &b, std::vector<bool> &c){
  sprintf(fname, "%s%d%s%d.csv", path.c_str(), l+1, "pattern", pattern + 1); // エポックあり
  if( (fp = fopen(fname, "w")) == NULL) { // ファイルのオープン
    printf( "結果ファイルがオープンできませんでした\n" );
    exit( 1 );
  }
  fprintf( fp, "%s,%s,%s\n", a_t.c_str(), b_t.c_str(), c_t.c_str()); // ヘッダ
  for(int i = 0; i<a.size(); i++){ // レコードを挿入
    std::stringstream bool_data;
    bool_data << std::boolalpha << c[i];
    fprintf( fp, "\%d,%d,%s\n", a[i], b[i], bool_data.str().c_str());
  }
  printf( "%s に結果が出力されました\n", fname );
  fclose( fp ); // ファイルを閉じる
}

//--- double型:4つ ---//
void OutputToCSV::toCSV(std::string a_t, std::string b_t, std::string c_t, std::string d_t, std::vector<double> &a, std::vector<double> &b, std::vector<double> &c, std::vector<double> &d){
  sprintf(fname, "%s%d%s%d%s%d.csv", path.c_str(), l+1, "epoch", epoch+1, "pattern", pattern+1); // エポックあり
  if( (fp = fopen(fname, "w")) == NULL) { // ファイルのオープン
    printf( "結果ファイルがオープンできませんでした\n" );
    exit( 1 );
  }
  fprintf( fp, "%s,%s,%s,%s\n", a_t.c_str(), b_t.c_str(), c_t.c_str(), d_t.c_str());
  for(int i = 0; i < a.size(); i+=5){
    fprintf( fp, "\%f,%f,%f,%f\n", a[i], b[i], c[i], d[i]);
  }
  printf( "%s に結果が出力されました\n", fname );
  fclose( fp );
}

//--- double型:1つ ---//
void OutputToCSV::toCSV(std::string a_t, std::vector<double> &a){
  sprintf(fname, "%s.csv", path.c_str()); // エポックあり
  if( (fp = fopen(fname, "w")) == NULL) { // ファイルのオープン
    printf( "結果ファイルがオープンできませんでした\n" );
    exit( 1 );
  }
  fprintf( fp, "%s\n", a_t.c_str());
  for(int i = 0; i < a.size(); i+=5){
    fprintf( fp, "\%f\n", a[i]);
  }
  printf( "%s に結果が出力されました\n", fname );
  fclose( fp );
}
