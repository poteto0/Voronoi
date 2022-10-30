#ifdef GL_SILENCE_DEPRECATION // インクルードブロック

#include <GL/freeglut.h>
#include <vector>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <filesystem>

#include <time.h>

#define GL_SILENCE_DEPRECATION


#ifdef _WIN32 // インクルードブロック
#include <Eigen/Core>　
#endif
#ifdef __linux__
#include <eigen3/Eigen/Core>
#endif
#ifdef __APPLE__
#include <eigen3/Eigen/Core>
#endif


class Mplot
{
public:
	// コンストラクタ宣言 //
	Mplot(int w, int h, float input_xmin, float input_xmax, float input_ymin, float input_ymax); // プロットの表示サイズあり
	Mplot(float input_xmin, float input_xmax, float input_ymin, float input_ymax); // プロットの表示サイズ指定なし
	~Mplot();

	static std::vector<Mplot*> Mplots;
	static bool non_window;

	int this_number;
	int W; // 最初のwindowのサイズ(幅)
	int H; // 最初のwindowのサイズ(縦)
	int WindowPosition_x;
	int WindowPosition_y;
	double xmin;
	double xmax;
	double ymin;
	double ymax;

	// メソッドの定義 //
	void rec(const std::string & fn, int fps_, int type);
	void stack(const std::string & fn);
	void rec_filename_with_timestamp(const std::string & fn, int fps_, int type);
	void stack_filename_with_timestamp(const std::string & fn);

	bool on_stack = false;
	int stack_num = 0;
	std::filesystem::path folder_name = "stack";

	bool on_rec = false;
	bool saved = false;
	int fps = 300;
	std::filesystem::path file_name = "movie";
	int rec_type_num = 0;
	cv::Mat video_buf;
	cv::Mat video;
	cv::VideoWriter videowriter;

	std::vector<float> background_color;


	int WinID;
	bool plot_state;
	void init();
	static void mouse(int button, int state, int x, int y);
	static void resize(int w, int h);
	static int GetNowNumber();
	static void junk(void);
	static void saveall(void);
	void WindowSize(int w, int h);
	void SetAnimation(void);
	void SetPicture_first(void);
	void SetPicture_end(void);

	void BackgroundColor(std::vector<float> &color1);

	void lim(float input_xmin, float input_xmax, float input_ymin, float input_ymax);

	void error(const std::string & message);
	void warning(const std::string & message);

	std::string YYYYMMDDHHMMSS();

	// 点プロット //
	template<typename V> void point(const V &xy_data, std::vector<float> &color1, float size) {
		glColor3f(color1[0], color1[1], color1[2]);
		glPointSize(size);
		glBegin(GL_POINTS);
		for (auto && xy : xy_data) {
			glVertex2i(xy[0] , xy[1]);
		}
		glEnd();
	}

	// 線プロット //
	template<typename V> void lines(const V &xy_data, std::vector<float> &color1, float lwd, bool loop) {
		glColor3f(color1[0], color1[1], color1[2]);
		glLineWidth(lwd);
		if(!loop) {
			glBegin(GL_LINES);
		}
		else {
			glBegin(GL_LINE_LOOP);
		}
		for (auto && xy : xy_data) {
			glVertex3f(xy[0], xy[1], 0);
		}

		glEnd();
	}

	// 中塗りプロット //
	template<typename V> void polygon(const V &xy_data, std::vector<float> &color1) {
		glColor3f(color1[0], color1[1], color1[2]);
		glBegin(GL_POLYGON);
		for (auto && xy : xy_data) {
			glVertex3f(xy[0], xy[1], 0);
		}
		glEnd();
	}

	// 四角形プロット //
	template<typename V> void rectangle(const V &xy_data, std::vector<float> &color1, float lwd) { // [1][4]で代入
		glColor3f(color1[0], color1[1], color1[2]);
		glLineWidth(lwd);
		glBegin(GL_LINE_LOOP);

		for (auto && xy : xy_data) {
			glVertex2f(xy[0], xy[1]);
			glVertex2f(xy[2], xy[1]);
			glVertex2f(xy[2], xy[3]);
			glVertex2f(xy[0], xy[3]);
		}

		glEnd();
	}

	// 四角形中塗りプロット //
	template<typename V> void fill_rectangle(const V &xy_data, std::vector<float> &color1, float lwd) { // [1][4]で代入
		glColor3f(color1[0], color1[1], color1[2]);
		glLineWidth(lwd);
		glBegin(GL_POLYGON);

		for (auto && xy : xy_data) {
			glVertex2f(xy[0]+0.1, xy[1]-0.1);
			glVertex2f(xy[2]-0.1, xy[1]-0.1);
			glVertex2f(xy[2]-0.1, xy[3]+0.1);
			glVertex2f(xy[0]-0.1, xy[3]+0.1);
		}

		glEnd();
	}

	// 三角形中塗りプロット //
	template<typename V> void fill_vertex(const V &xy_data, std::vector<float> &color1, float lwd){
		glColor3f(color1[0], color1[1], color1[2]);
		glLineWidth(lwd);
		glBegin(GL_POLYGON);

		for (auto && xy : xy_data) {
			glVertex2f(xy[0]+0.1, xy[1]-0.1);
		}

		glEnd();
	}

	// 文字プロット //
	template<typename V> void draw_text(std::string text, const V &xy_data, std::vector<float> &color1) {
		glColor3f(color1[0], color1[1], color1[2]); // 色
		glDisable(GL_LIGHTING);
    // 平行投影にする //
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, xmax, ymax, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // 画面上にテキスト描画 //
    glRasterPos2d(xy_data[0][0], ymax - xy_data[0][1]);
    int size = (int)text.size();
    for(int i = 0; i < size; ++i){
        char ic = text[i];
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ic);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
	}

private:
	int circle_n = 20;
	bool calcurated_circle_vector = false;
	std::vector<std::vector<double>> circle_vector;

public:
	// 円プロット //
	template<typename valuetype> std::vector<std::vector<valuetype>> circle(valuetype x, valuetype y, double r) {
		circle_vector.resize(circle_n);
		if(!calcurated_circle_vector) {
			for(int i = 0; i < circle_n; ++i) {
				circle_vector[i].resize(2);
				circle_vector[i][0] = cos(2.0 * M_PI * ((double)i / circle_n));
				circle_vector[i][1] = sin(2.0 * M_PI * ((double)i / circle_n));
			}
			calcurated_circle_vector = true;
		}
		std::vector<std::vector<valuetype>> xy_data;
		xy_data.resize(circle_n);
		for(int i = 0; i < circle_n; ++i) {
			xy_data[i].resize(2);
			xy_data[i][0] = x + r * circle_vector[i][0];
			xy_data[i][1] = y + r * circle_vector[i][1];
		}

		return xy_data;
	}
	// 扇形プロット //
	template<typename valuetype> std::vector<std::vector<valuetype>> fan(valuetype x, valuetype y, double r, double angle, const double direction) {
		circle_vector.resize(circle_n);
		for(int i = 0; i < circle_n; ++i) {
			circle_vector[i].resize(2);
			circle_vector[i][0] = cos(angle * ((double)(i-10) / circle_n) + direction);
			circle_vector[i][1] = sin(angle * ((double)(i-10) / circle_n) + direction);
		}
		calcurated_circle_vector = false;
		std::vector<std::vector<valuetype>> xy_data;
		xy_data.resize(circle_n+1);
		xy_data[0].resize(2);
		xy_data[0][0] = x;
		xy_data[0][1] = y;
		for(int i = 1; i < circle_n+1; ++i) {
			xy_data[i].resize(2);
			xy_data[i][0] = x + r * circle_vector[i-1][0];
			xy_data[i][1] = y + r * circle_vector[i-1][1];
		}

		return xy_data;
	}

	typedef struct rec_type_ {
		int num;
		std::string file_ext;
		std::string codec;
	}rec_type;

	static std::vector<rec_type> rec_types;

	// 色一覧宣言 //
	static std::vector<float> red;
	static std::vector<float> lightcoral;
	static std::vector<float> green;
	static std::vector<float> blue;
	static std::vector<float> black;
	static std::vector<float> white;
	static std::vector<float> yellow;
	static std::vector<float> purple;
	static std::vector<float> skyblue;
};

#endif
