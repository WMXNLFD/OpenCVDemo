#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
//#include <opencv2/highgui/highgui.hpp>  
//#include<opencv2/imgproc/imgproc.hpp>



using namespace cv;
using namespace std;

Mat src, gray_src, dst, dst_canny;
int t1_value = 100;
int max_value = 255;
const char* OUTPUT_TITLE = "Result";

//const char* output_win = "findcontours-demo";
int threshold_value = 50; //设置低阈值
int threshold_max = 255;//设置高阈值  用作边缘提取 轮廓发现
//RNG rng;
void Demo_Contours(int, void*);

void Canny_Demo(int, void*);

void fillHole(const Mat srcBw, Mat& dstBw);

const char* output_win = "image moents demo";
RNG rng(12345);
int main();
void Demo_Moments(int, void*);


void processimage(Mat& frame)
{
	circle(frame, Point(cvRound(frame.cols / 2),
		cvRound(frame.rows / 2)), 150, Scalar(0, 0, 255), 2, 8);
}

int main()
{
	src = imread("D:/ljp/p1/1.bmp"); //F: / ljp / picture / 3.png
	if (src.empty())
	{
		printf("Could not find the image!\n");
		return -1;
	}

	char INPUT_TITLE[] = "input image";
	namedWindow(INPUT_TITLE, CV_WINDOW_AUTOSIZE);
	namedWindow(OUTPUT_TITLE, CV_WINDOW_AUTOSIZE);
	imshow(INPUT_TITLE, src);



	//掩膜操作	
	//Mat kernel = (Mat_<char>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0); //注意数字个数
	//filter2D(src, dst, src.depth(), kernel);
	//imshow("kernel", dst);


	//三通道取反操作
	//bitwise_not(src, dst);

	//3*3模糊 基于均值模糊
	//blur(src, dst, Size(5, 5), Point(-1, -1)); //数字尽量选择奇数
	//高斯模糊 基于权重模糊
	//GaussianBlur(src, dst, Size(5, 5), 11, 11);

	//Cannyb边缘检测
	// 1.转为灰度图像
	/*cvtColor(src, gray_src, CV_BGR2GRAY);
	createTrackbar("Threshold Value", OUTPUT_TITLE, &t1_value, max_value, Canny_Demo);
	Canny_Demo(0, 0);*/

	//// 中值滤波
	Mat moutput;
	medianBlur(src, moutput, 3);
	cvtColor(moutput, moutput, CV_BGR2GRAY);

	//// 霍夫圆检测

	Mat cir, cir1;
	vector<Vec3f> pcircles;
	HoughCircles(moutput, pcircles, CV_HOUGH_GRADIENT, 1, 30, 100, 90, 60, 130); //调参数 倒数第三个数值取完美圆大小
	src.copyTo(cir);
	for (size_t i = 0; i < pcircles.size(); i++) {
		Vec3f cc = pcircles[i];
		circle(cir, Point(cc[0], cc[1]), cc[2] + 8, Scalar(0, 0, 0), -1, LINE_AA); //倒数第二个thickness是线的宽度，当thickness宽度为-1时则会填充整个圆。
		//ellipse(cir, Point(cc[0], cc[1]), 2, 30, 50, 90, (0, 255, 255), 1, LINE_8, 0);
		//circle(cir, Point(cc[0], cc[1]), 2, Scalar(198, 23, 155), 2, LINE_AA); //画圆心
	}

	imshow("circle", cir);

	//填充 多边形圆弧
	//CvArr* tcir = (CvArr*)& cir;
	//CvMat* Mat = cvCreateMat(800, 800, );
	//CvPoint Points[4] = { {333, 480}, {360, 482},{351,670},{308,666}, };
	Point PointArray[4];
	PointArray[0] = Point(333, 480);
	PointArray[1] = Point(350, 480);
	PointArray[2] = Point(350, 670);
	PointArray[3] = Point(310, 666);
	fillConvexPoly(cir, PointArray, 4, Scalar(0, 0, 255));
	//Mat* cir = (Mat*)tcir;
	imshow("填充后", cir);




	//轮廓发现
	/*namedWindow("input-image", CV_WINDOW_AUTOSIZE);
	namedWindow(output_win, CV_WINDOW_AUTOSIZE);
	imshow("input-image", src);
	cvtColor(src, src, CV_BGR2GRAY);

	const char* trackbar_title = "Threshold Value:";
	createTrackbar(trackbar_title, output_win, &threshold_value, threshold_max, Demo_Contours);
	Demo_Contours(0, 0);*/

	/*
	//读取视频
	Mat frame;
	string filename = "F:/ljp/picture/3.gg";
	VideoCapture capture;
	capture.open(filename);

	double rate = capture.get(CV_CAP_PROP_FPS);//获取视频文件的帧率

	int delay = cvRound(1000.00 / rate);

	if (!capture.isOpened())
	{
		cout << "open failed" << endl;
		return -1;
	}
	else
	{
		while (true)
		{
			Mat frame;
			capture >> frame;//读出每一帧图像
			if (frame.empty())
				break;
			imshow("处理前视频", frame);
			processimage(frame);
			imshow("处理后视频", frame);
			waitKey(delay);
		}
	}
	system("pause");*/

	//二值图像孔洞填充算法

	cvtColor(cir, gray_src, CV_BGR2GRAY);
	//imshow("gray_src", gray_src);
	//Mat bw;
	//cv::threshold(gray_src, bw, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	//imshow("bw", bw);
	fillHole(gray_src, dst);
	imshow("孔洞填充后", dst);
	//孔洞填充后 进行轮廓描绘
	Mat dst1, dst2, dst3;
	//Canny(dst, dst1, 50,125);
	//imshow("填充后描线", dst1);
	//createTrackbar("Threshold Value", OUTPUT_TITLE, &t1_value, max_value, Canny_Demo);
	//Canny_Demo(0, 0);

	//凸包
	//convexHull(8, dst1);
	//imshow("凸包", dst1);


	Canny_Demo(0, 0);
	//fillHole(gray_src, dst3);
	//imshow("孔洞2", dst3);
	imshow("canny_demo", dst_canny);

	//轮廓点提取
	//cvtColor(dst_canny, dst_canny, CV_BGR2GRAY);
	//imshow("dst_canny", dst_canny);
	vector<vector<Point>>contours;
	//find
	findContours(dst_canny, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	//draw
	//createTrackbar("Threshold Value", OUTPUT_TITLE, &t1_value, max_value, Canny_Demo);
	vector<vector<Point>>hull(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		convexHull(Mat(contours[i]), hull[i], false);
	}
	//imshow("hull", hull);
	Mat result(dst_canny.size(), CV_8U, Scalar(0));
	//drawContours(result, contours, -1, Scalar(255),1);
	drawContours(result, hull, -1, Scalar(255), 1);
	imshow("画轮廓", result);

	//int s = 2 * 2 + 1;
	Mat structureElement = getStructuringElement(MORPH_RECT, Size(7, 7), Point(-1, -1));
	dilate(result, result, structureElement);
	imshow("erode", result);
	fillHole(result, result);
	imshow("孔洞2", result);


	/*
		////图形矩  1.提取图像轮廓 2.发现轮廓 3. 计算每个轮廓对象的矩 3. 计算每个对象的中心，弧长，面积。
		cvtColor(src, gray_src, CV_BGR2GRAY);
		GaussianBlur(gray_src, gray_src, Size(3, 3), 0, 0);
		char input_win[] = "input image";
		namedWindow(input_win, CV_WINDOW_AUTOSIZE);
		namedWindow(output_win, CV_WINDOW_AUTOSIZE);
		imshow(input_win, src);
		createTrackbar("Threshold Value : ", output_win, &threshold_value, threshold_max, Demo_Moments);
		Demo_Moments(0, 0);
		*/

		//



	waitKey(0);
	return 0;

}



//图形矩  1.提取图像轮廓 2.发现轮廓 3. 计算每个轮廓对象的矩 3. 计算每个对象的中心，弧长，面积。
void Demo_Moments(int, void*) {
	Mat canny_output;
	vector<vector<Point>> contours;
	vector<Vec4i> hierachy;

	Canny(gray_src, canny_output, threshold_value, threshold_value * 2, 3, false);
	findContours(canny_output, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<Moments> contours_moments(contours.size());
	vector<Point2f> ccs(contours.size());
	for (size_t i = 0; i < contours.size(); i++) {
		contours_moments[i] = moments(contours[i]);
		ccs[i] = Point(static_cast<float>(contours_moments[i].m10 / contours_moments[i].m00), static_cast<float>(contours_moments[i].m01 / contours_moments[i].m00));
	}

	Mat drawImg;// = Mat::zeros(src.size(), CV_8UC3);
	src.copyTo(drawImg);
	for (size_t i = 0; i < contours.size(); i++) {
		if (contours[i].size() < 100) {
			continue;
		}
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		printf("center point x : %.2f y : %.2f\n", ccs[i].x, ccs[i].y);
		printf("contours %d area : %.2f   arc length : %.2f\n", i, contourArea(contours[i]), arcLength(contours[i], true));
		drawContours(drawImg, contours, i, color, 2, 8, hierachy, 0, Point(0, 0));
		circle(drawImg, ccs[i], 2, color, 2, 8);
	}

	imshow(output_win, drawImg);
	return;
}



//二值图像孔洞填充算法
void fillHole(const Mat srcBw, Mat& dstBw)
{
	Size m_Size = srcBw.size();
	Mat Temp = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcBw.type());//延展图像
	srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));

	cv::floodFill(Temp, Point(350, 350), Scalar(255), 0, Scalar(10), Scalar(10));

	Mat cutImg;//裁剪延展的图像
	Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);
	imshow("cutImg", cutImg);

	dstBw = srcBw | (~cutImg);
}



void Demo_Contours(int, void*) {
	Mat canny_output;
	vector<vector<Point>> contours;
	vector<Vec4i> hierachy;
	Canny(src, canny_output, threshold_value, threshold_value * 2, 3, false);
	findContours(canny_output, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	dst = Mat::zeros(src.size(), CV_8UC3);
	RNG rng(12345);
	for (size_t i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(dst, contours, i, color, 2, 8, hierachy, 0, Point(0, 0));
	}
	imshow(output_win, dst);
}


void Canny_Demo(int, void*)
{
	Mat edge_output;
	blur(gray_src, gray_src, Size(3, 3), Point(-1, -1), BORDER_DEFAULT);
	Canny(gray_src, edge_output, t1_value, t1_value * 2, 3, false);

	//dst.create(src.size(), src.type());
	//src.copyTo(dst, edge_output);	
	//(edge_output, edge_output);
	dst_canny = ~edge_output;
	imshow(OUTPUT_TITLE, ~edge_output); //白底
}