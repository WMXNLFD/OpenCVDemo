//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include <math.h>
//#include <algorithm>
//
//using namespace cv;
//using namespace std;
//
//Mat src, gray_src, dst;
//Rect rect;
//Point pt;
//bool flag_mouse_contained;
//vector<vector<Point>> contours;
//void  on_Mouse(int event, int x, int y, int flags, void* param);
//void Mouse_Callback(int event, int x, int y, int flags, void* userdata);
//
//
//
//int main(int argc, char** argv) {
//	src = imread("D:/ljp/p1/1/2.bmp");
//	if (src.empty()) {
//		printf("could not load image...\n");
//		return -1;
//	}
//	//namedWindow("input image", CV_WINDOW_AUTOSIZE);
//	//imshow("input image", src);	
//
//	//计算测试时间
//	//double time = cvGetTickCount();
//	
//	Mat src_fill = src.clone();
//	Point PointArray[4];
//	PointArray[0] = Point(343, 397);
//	PointArray[1] = Point(350, 397);
//	PointArray[2] = Point(350, 700);
//	PointArray[3] = Point(307, 700);
//	fillConvexPoly(src_fill, PointArray, 4, Scalar(255, 255, 255));
//	//imshow("填充后", src_fill);
//
//	Mat imageGray;	
//	cvtColor(src_fill, imageGray, CV_RGB2GRAY, 0);
//	Mat imageMask = Mat(src.size(), CV_8UC1, Scalar::all(0));
//
//	//通过阈值处理生成Mask
//	threshold(imageGray, imageMask, 240, 255, CV_THRESH_BINARY);
//	Mat Kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
//	//对Mask膨胀处理，增加Mask面积
//	dilate(imageMask, imageMask, Kernel);
//
//	//图像修复
//	inpaint(src_fill, imageMask, src_fill, 1, INPAINT_NS); //INPAINT_TELEA
//	//imshow("Mask", imageMask);
//	//imshow("修复后", src_fill);
//
//	
//	//高斯模糊
//	Mat blurImage;
//	GaussianBlur(src_fill, blurImage, Size(15, 15), 0, 0);
//	//imshow("blur", blurImage);
//
//	//灰度化
//	cvtColor(blurImage, gray_src, COLOR_BGR2GRAY);
//	//imshow("gray_src", gray_src);
//	
//	//二值化
//	Mat binaryImage;
//	threshold(gray_src, binaryImage, 0, 255, THRESH_BINARY | THRESH_OTSU);
//	//imshow("threshold", binaryImage);
//
//	// morphology operation
//	Mat morhpImage;
//	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(15,15), Point(-1, -1));
//	morphologyEx(binaryImage, morhpImage, MORPH_OPEN, kernel, Point(-1, -1));
//	//imshow("MORPH_OPEN", morhpImage);
//
//	// dilate image 膨胀操作
//	kernel = getStructuringElement(MORPH_ELLIPSE, Size(7,7), Point(-1, -1));
//	dilate(morhpImage, morhpImage, kernel,Point(-1,-1),1);
//	//imshow("dilate image", morhpImage);
//
//	//轮廓
//	vector<vector<Point>> contours;
//	vector<Vec4i> hireachy; //层次
//	findContours(morhpImage, contours, hireachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
//	//Mat resultImage = Mat::zeros(src.size(), CV_8UC3); //把轮廓点画在上面
//	Mat printImage = src.clone(); //把轮廓点画在图上
//	for (size_t t = 0; t < contours.size(); t++) {
//		// 面积过滤
//		double area = contourArea(contours[t]);  //计算轮廓面积		
//		if (area < 1000) continue;
//		// 横纵比过滤
//		Rect rect = boundingRect(contours[t]);
//		float ratio = float(rect.width) / float(rect.height);  //宽高之比
//
//		if (rect.width < 372) {  //ratio < 1.5 && ratio > 0.8
//			cout << "第" << t + 1 << "个面积为：" << area << endl;
//			cout << t << endl;
//			cout << "rect.width = " << rect.width << endl;
//			cout << "rect.height = " << rect.height << endl;
//			cout << "ratio=" << ratio << endl;
//			cout << endl;
//			drawContours(printImage, contours, t, Scalar(0, 0, 255), 2, 8, hireachy, 0, Point());
//		}
//	}
//
//	//cout << "行数：" << contours.size() << endl << "列数" << contours[0].size() << endl;
//	/*for(auto i = 0; i < contours.size(); i ++)
//		printf("第%d个轮廓，数据点为：%d\n", i + 1, contours[i].size());
//	for (auto i = 0; i < contours.size(); i++)
//		cout << contours[i] << endl;*/
//
//
//	//printf("execution time(ms):%.2f\n", (getTickCount() - time) / getTickFrequency() * 1000);
//	//imshow("contours", printImage);
//	/*
//	//识别红色区域
//	Mat hsvImage;
//	cvtColor(printImage, hsvImage, CV_BGR2HSV);
//
//	Mat resultGray = Mat(hsvImage.rows, hsvImage.cols, CV_8U, cv::Scalar(255));
//	Mat resultColor = Mat(hsvImage.rows, hsvImage.cols, CV_8UC3, cv::Scalar(255, 255, 255));
//	double H = 0.0, S = 0.0, V = 0.0;
//	
//	for (int i = 0; i < hsvImage.rows; i++)
//	{
//		for (int j = 0; j < hsvImage.cols; j++)
//		{
//			H = hsvImage.at<Vec3b>(i, j)[0];  //获取图像每个像素点的H,S,V分量
//			S = hsvImage.at<Vec3b>(i, j)[1];
//			V = hsvImage.at<Vec3b>(i, j)[2];
//
//			if ((H >= 0 && H < 10) || (H >= 156 && H < 180)) //饱和度 S >= 70 && S < 155) || (S >= 35)   //((H >= 0 && H < 24) && V >= 215)
//			{	
//				if ((S >= 43) && (V >= 46))
//				{
//					resultGray.at<uchar>(i, j) = 0;
//					resultColor.at<Vec3b>(i, j)[0] = printImage.at<Vec3b>(i, j)[0];
//					resultColor.at<Vec3b>(i, j)[1] = printImage.at<Vec3b>(i, j)[1];
//					resultColor.at<Vec3b>(i, j)[2] = printImage.at<Vec3b>(i, j)[2];
//				}							
//			}
//		}
//	}
//	imshow("resultGray", resultGray);
//	imshow("resultColor", resultColor);*/
//
//	
//	//用鼠标拖动识别轮廓
//	dst = printImage.clone();
//	namedWindow("modifiedimg");
//	imshow("modifiedimg", dst);
//
//
//	setMouseCallback("modifiedimg", Mouse_Callback, 0);
//	
//
//
//
//	waitKey(0);
//	return 0;
//}
//
//void  on_Mouse(int event, int x, int y, int flags, void* param)
//{
//
//	if (event == EVENT_LBUTTONDOWN && (flags & EVENT_FLAG_LBUTTON))
//	{
//		pt = Point(x, y);
//		cout << contours[14].size() << endl;
//		cout << pt << endl;
//		
//		contours[14].push_back(pt);
//		cout << contours[14].size() << endl;
//
//		//cout << contours[14] << endl;
//		//sort(contours[14].begin(), contours[14].end());
//		//cout << contours[14] << endl;
//		//drawContours(dst, contours[14], -1, (0, 0, 255), 1, 8);		
//		//cout << contours[14].size() << endl;
//		imshow("modifiedimg", dst);
//	}
//	//else if (event == EVENT_LBUTTONDOWN && (flags & EVENT_FLAG_LBUTTON)) //(event == EVENT_MOUSEMOVE
//	//{
//	//	Point pt(x, y);
//	//	//line(dst, previousPoint, pt, Scalar(0, 0, 255), 1, 8, 0);
//	//	contours[14].push_back(pt);
//	//	drawContours(dst, contours[14], -1, (0, 0, 255), 1, 8);
//	//	previousPoint = pt;
//	//	imshow("modifiedimg", dst);
//	//}
//}
//	
//void Mouse_Callback(int event, int x, int y, int flags, void* userdata)
//{
//	switch (event)
//	{
//	case EVENT_LBUTTONDOWN:
//		if (rect.contains(Point(x, y)))
//		{
//			flag_mouse_contained = true;
//			pt = Point(x, y);
//		}
//		break;
//	case EVENT_MOUSEMOVE:
//		if (flag_mouse_contained == true)
//		{
//			rect.x += (x - pt.x);   //当前鼠标坐标减去上一次回调事件的鼠标坐标
//			rect.y += (y - pt.y);
//		}
//		break;
//	case EVENT_LBUTTONUP:
//		flag_mouse_contained = false;
//		break;
//	}
//	pt = Point(x, y);   //记录当前鼠标坐标用于下一次回调事件
//}


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#define WINDOW_NAME "矩形拖动"

Mat src_image;
Mat dst_image;
Rect rect;
Point pt;
bool flag_mouse_contained;

void Mouse_Callback(int event, int x, int y, int flags, void* userdata)
{
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		if (rect.contains(Point(x, y)))
		{
			flag_mouse_contained = true;
			pt = Point(x, y);
		}
		break;
	case EVENT_MOUSEMOVE:
		if (flag_mouse_contained == true)
		{
			rect.x += (x - pt.x);   //当前鼠标坐标减去上一次回调事件的鼠标坐标
			rect.y += (y - pt.y);
		}
		break;
	case EVENT_LBUTTONUP:
		flag_mouse_contained = false;
		break;
	}
	pt = Point(x, y);   //记录当前鼠标坐标用于下一次回调事件
}

int main(int argc, char** argv)
{
	flag_mouse_contained = false;
	src_image = Mat(256, 256, CV_32FC3, Scalar(255, 255, 255));
	namedWindow(WINDOW_NAME, WINDOW_AUTOSIZE);
	setMouseCallback(WINDOW_NAME, Mouse_Callback, 0);
	rect = Rect(50, 50, 50, 50);
	src_image.copyTo(dst_image);    //类似于winform中的Paint事件，只修改dst_image，scr_image保持不变
	rectangle(dst_image, rect, Scalar(0, 0, 0));
	imshow(WINDOW_NAME, dst_image);
	while (1)
	{
		if (flag_mouse_contained == true)
		{
			src_image.copyTo(dst_image);
			rectangle(dst_image, rect, Scalar(0, 0, 0));
			imshow(WINDOW_NAME, dst_image);
		}
		if (waitKey(10) == 27)  //等待事件必须大于0，如果小于等于0，则程序会进入无限等待
		{
			return 0;
		}
	}
	//cvDrawRect()
	
	return 0;
}