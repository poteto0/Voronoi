#include "Mplot.hpp"

std::vector<Mplot*> Mplot::Mplots;
bool Mplot::non_window = true;

// 色一覧 //
std::vector<float> Mplot::red = {1, 0, 0};
std::vector<float> Mplot::green = {0, 1, 0};
std::vector<float> Mplot::blue = {0, 0, 1};
std::vector<float> Mplot::black = {0, 0, 0};
std::vector<float> Mplot::white = {1, 1, 1};
std::vector<float> Mplot::yellow = {1, 1, 0};
std::vector<float> Mplot::lightcoral = {1, 0, 1};
std::vector<float> Mplot::skyblue = {0, 1, 1};

std::vector<Mplot::rec_type> Mplot::rec_types = {{0,std::string(".mp4"),std::string("mp4v")},
					{1,std::string(".mov"),std::string("IYUV")},
					{2,std::string(".avi"),std::string("IYUV")}};

// コンストラクタ定義 //
Mplot::Mplot(int w, int h, float input_xmin, float input_xmax, float input_ymin, float input_ymax) { // サイズ指定あり
	this_number = Mplots.size();
	if(this_number < 1) {
		std::atexit(Mplot::saveall);
	}
	Mplots.push_back(this);

	W = w; // 最初のwindowのサイズ(幅)
	H = h; // 最初のwindowのサイズ(縦)
	WindowPosition_x = W * this_number;
	WindowPosition_y = 0;

	xmin = input_xmin;
	xmax = input_xmax;
	ymin = input_ymin;
	ymax = input_ymax;
	background_color = white;
	plot_state = false;
	WinID = 0;
}

// コンストラクタ定義 //
Mplot::Mplot(float input_xmin, float input_xmax, float input_ymin, float input_ymax) {// サイズ指定なし
	this_number = Mplots.size();
	if(this_number < 1) {
		std::atexit(Mplot::saveall);
	}
	Mplots.push_back(this);

	W = 500; // 最初のwindowのサイズ(幅)
	H = 500; // 最初のwindowのサイズ(縦)
	WindowPosition_x = W * this_number;
	WindowPosition_y = 0;

	xmin = input_xmin;
	xmax = input_xmax;
	ymin = input_ymin;
	ymax = input_ymax;
	background_color = white;
	plot_state = false;
	WinID = 0;
	std::atexit(Mplot::saveall);
}

Mplot::~Mplot() {
	if(on_rec && !saved) {
		videowriter.release();
		saved = true;
	}
	glutDestroyWindow(WinID);
}


// 動画の保存 //
void Mplot::rec(const std::string & fn, int fps_, int type) {
	if (type >= rec_types.size()) error("invalid type number");
	on_rec = true;
	file_name = fn + rec_types[type].file_ext;
	fps = fps_;
	rec_type_num = type;
	video_buf = cv::Mat::zeros(cv::Size(W, H), CV_8UC3);
	video = cv::Mat::zeros(cv::Size(W, H), CV_8UC3);
	videowriter.open(std::string(file_name), cv::VideoWriter::fourcc(rec_types[type].codec[0], rec_types[type].codec[1], rec_types[type].codec[2], rec_types[type].codec[3]),
	                 fps , cv::Size(video.cols, video.rows));
}

// 動画の保存(実行日込み) //
void Mplot::rec_filename_with_timestamp(const std::string & fn, int fps_, int type) {
	if (type >= rec_types.size()) error("invalid type number");
	on_rec = true;
	file_name = fn + YYYYMMDDHHMMSS() + rec_types[type].file_ext;
	fps = fps_;
	rec_type_num = type;
	video_buf = cv::Mat::zeros(cv::Size(W, H), CV_8UC3);
	video = cv::Mat::zeros(cv::Size(W, H), CV_8UC3);
	videowriter.open(std::string(file_name), cv::VideoWriter::fourcc(rec_types[type].codec[0], rec_types[type].codec[1], rec_types[type].codec[2], rec_types[type].codec[3]),
	                 fps , cv::Size(video.cols, video.rows));
}

// スタック画像保存 //
void Mplot::stack(const std::string & fn) {
	on_stack = true;
	folder_name = std::string(std::filesystem::absolute(fn));
	if(!std::filesystem::create_directory(folder_name)) warning("folder was updated.");
	if(!std::filesystem::is_directory(folder_name)) error("couldn't make folder");
	if(!on_rec){
		video_buf = cv::Mat::zeros(cv::Size(W, H), CV_8UC3);
		video = cv::Mat::zeros(cv::Size(W, H), CV_8UC3);
	}
}

// スタック画像保存(実行日込み) //
void Mplot::stack_filename_with_timestamp(const std::string & fn) {
	on_stack = true;
	folder_name = std::string(std::filesystem::absolute(fn)) + YYYYMMDDHHMMSS();
	if(!std::filesystem::create_directory(folder_name)) warning("folder was updated.");
	if(!std::filesystem::is_directory(folder_name)) error("couldn't make folder");
	if(!on_rec){
		video_buf = cv::Mat::zeros(cv::Size(W, H), CV_8UC3);
		video = cv::Mat::zeros(cv::Size(W, H), CV_8UC3);
	}
}

// プロットの宣言(最初に書く) //
void Mplot::SetPicture_first(void) {
	if(!plot_state) {
		SetAnimation();
	}
	glutSetWindow(WinID);
	glutMainLoopEvent();
	glClearColor(background_color[0], background_color[1], background_color[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

// プロット終了後に書く //
void Mplot::SetPicture_end(void) {
	glutSwapBuffers();
	if(on_rec || on_stack) {
		glReadPixels(0, 0, W, H, GL_RGB, GL_UNSIGNED_BYTE, video_buf.data);
		cv::flip(video_buf, video, 0);
		cv::cvtColor(video, video, cv::COLOR_RGB2BGR);
		if(on_rec) {
			videowriter << video;
		}
		if(on_stack) {
			std::ostringstream oss;
			oss << std::setfill('0') << std::setw(8) << stack_num << ".png";
			cv::imwrite((folder_name / std::filesystem::path(oss.str())).string(), video);

			stack_num++;
		}

	}
}

// ウィンドウの大きさ取得 //
void Mplot::WindowSize(int w, int h) {
	W = w;
	H = h;
}

void Mplot::resize(int w, int h) {
	int num = GetNowNumber();

	glOrtho(Mplots[num]->xmin, Mplots[num]->xmax, Mplots[num]->ymin, Mplots[num]->ymax,
	 -(Mplots[num]->xmax - Mplots[num]->xmin) / 2, (Mplots[num]->xmax - Mplots[num]->xmin) / 2);
}

void Mplot::SetAnimation(void) {
	#pragma omp critical
	{
		if (non_window) {
			non_window = false;
			int argc = 0;
			glutInit(&argc, NULL);
		}
	glutInitWindowPosition(WindowPosition_x, WindowPosition_y);
	glutInitWindowSize(W, H);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	WinID = glutCreateWindow((std::string("Window") + std::to_string(this_number + 1)).data());
	glutReshapeFunc(resize);
	glutDisplayFunc(junk);
	}
	plot_state = true;
}

void Mplot::junk(void) {
}

void Mplot::BackgroundColor(std::vector<float> &color1) {
	background_color[0] = color1[0];
	background_color[1] = color1[1];
	background_color[2] = color1[2];
}

void Mplot::lim(float input_xmin, float input_xmax, float input_ymin, float input_ymax) {
	xmin = input_xmin;
	xmax = input_xmax;
	ymin = input_ymin;
	ymax = input_ymax;
}

int Mplot::GetNowNumber() {
	int win = glutGetWindow();
	int nownumber = -1;
	for (Mplot* mp : Mplots)
	{
		if (mp->WinID == win) {
			nownumber =  mp->this_number;
		}
	}
	if(nownumber != -1) {
		return  nownumber;
	}
	else {
		// error("Can't get number of window.");
		return -1;
	}
}

void Mplot::saveall() {
	for (Mplot* mp : Mplots)
	{
		if(mp->on_rec && !mp->saved) {
			mp->videowriter.release();
			mp->saved = true;
		}
	}
}

void Mplot::error(const std::string & message) {
	std::cout << "Error with Mplot [" << this_number << "] : " << message << std::endl;
	getchar();
	exit(1);
}

void Mplot::warning(const std::string & message) {
	std::cout << "Warning with Mplot [" << this_number << "] : " << message << std::endl;
}

std::string Mplot::YYYYMMDDHHMMSS() {
//現在時刻取得
	time_t now = time(NULL);
	struct tm *pnow = localtime(&now);

	std::ostringstream oss;
	oss <<
	    std::setfill('0') << std::setw(4) << std::to_string(pnow->tm_year + 1900) <<
	    std::setfill('0') << std::setw(2) << std::to_string(pnow->tm_mon + 1) <<
	    std::setfill('0') << std::setw(2) << std::to_string(pnow->tm_mday) <<
	    std::setfill('0') << std::setw(2) << std::to_string(pnow->tm_hour) <<
	    std::setfill('0') << std::setw(2) << std::to_string(pnow->tm_min) <<
	    std::setfill('0') << std::setw(2) << std::to_string(pnow->tm_sec);

	return oss.str();
}
