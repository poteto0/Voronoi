#ifndef __CLASS__TOCSV

#include <vector>
#include <cmath>

#define __CLASS__TOCSV

//--- CSV保存クラス ---//
class OutputToCSV
{
public:
  char *fname; // ファイル名
  FILE *fp; // ファイル
  std::string path; // パス
  // --- ファイル名に含む変数 ---//
  int l; // l
  int epoch; // エポック
  int pattern; // パターン

  //--- コンストラクタ ---//
  OutputToCSV(FILE *ff, std::string pn);
  ~OutputToCSV(); // デストラクタ

  //--- セッタ ---//
  void set(FILE *ff, std::string pn);

  //--- ファイル名に入れる変数のセッタ ---//
  void set(int ll, int ee, int pp);

  //--- int型:2つ、bool型:1つ ---//
  void toCSV(std::string a_t, std::string b_t, std::string c_t, std::vector<int> &a, std::vector<int> &b, std::vector<bool> &c);

  //--- double型:4つ ---//
  void toCSV(std::string a_t, std::string b_t, std::string c_t, std::string d_t, std::vector<double> &a, std::vector<double> &b, std::vector<double> &c, std::vector<double> &d);

  //--- double型:1つ ---//
  void toCSV(std::string a_t, std::vector<double> &a);
};

#endif
