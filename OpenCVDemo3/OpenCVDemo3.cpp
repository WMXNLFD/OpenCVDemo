#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

Mat src, gray_src, dst;
int main(int argc, char** argv) {
	src = imread("D:/ljp/p1/1.bmp");
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	//namedWindow("input image", CV_WINDOW_AUTOSIZE);
	//imshow("input image", src);	

	//计算测试时间
	//double time = cvGetTickCount();
	
	Mat src_fill = src.clone();
	Point PointArray[4];
	PointArray[0] = Point(343, 397);
	PointArray[1] = Point(350, 397);
	PointArray[2] = Point(350, 700);
	PointArray[3] = Point(307, 700);
	fillConvexPoly(src_fill, PointArray, 4, Scalar(255, 255, 255));
	//imshow("填充后", src_fill);

	Mat imageGray;	
	cvtColor(src_fill, imageGray, CV_RGB2GRAY, 0);
	Mat imageMask = Mat(src.size(), CV_8UC1, Scalar::all(0));

	//通过阈值处理生成Mask
	threshold(imageGray, imageMask, 240, 255, CV_THRESH_BINARY);
	Mat Kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	//对Mask膨胀处理，增加Mask面积
	dilate(imageMask, imageMask, Kernel);

	//图像修复
	inpaint(src_fill, imageMask, src_fill, 1, INPAINT_NS); //INPAINT_TELEA
	//imshow("Mask", imageMask);
	imshow("修复后", src_fill);

	
	//高斯模糊
	Mat blurImage;
	GaussianBlur(src_fill, blurImage, Size(15, 15), 0, 0);
	//imshow("blur", blurImage);

	//灰度化
	cvtColor(blurImage, gray_src, COLOR_BGR2GRAY);
	//imshow("gray_src", gray_src);
	
	//二值化
	Mat binaryImage;
	threshold(gray_src, binaryImage, 0, 255, THRESH_BINARY | THRESH_OTSU);
	imshow("threshold", binaryImage);

	// morphology operation
	Mat morhpImage;
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(15,15), Point(-1, -1));
	morphologyEx(binaryImage, morhpImage, MORPH_OPEN, kernel, Point(-1, -1));
	imshow("MORPH_OPEN", morhpImage);

	// dilate image 膨胀操作
	kernel = getStructuringElement(MORPH_ELLIPSE, Size(7,7), Point(-1, -1));
	dilate(morhpImage, morhpImage, kernel,Point(-1,-1),1);
	imshow("dilate image", morhpImage);

	//轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy; //层次
	findContours(morhpImage, contours, hireachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	//Mat resultImage = Mat::zeros(src.size(), CV_8UC3); //把轮廓点画在上面
	Mat printImage = src.clone(); //把轮廓点画在图上
	for (size_t t = 0; t < contours.size(); t++) {
		// 面积过滤
		double area = contourArea(contours[t]);  //计算轮廓面积
		if (area < 10000) continue;
		// 横纵比过滤
		Rect rect = boundingRect(contours[t]);
		float ratio = float(rect.width) / float(rect.height);  //宽高之比

		if (rect.width < 372) {  //ratio < 1.5 && ratio > 0.8
			cout << t << endl;
			cout << "rect.width = " << rect.width << endl;
			cout << "rect.height = " << rect.height << endl;
			cout << "ratio=" << ratio << endl;
			drawContours(printImage, contours, t, Scalar(0, 0, 255), 2, 8, hireachy, 0, Point());
		}
	}

	//printf("execution time(ms):%.2f\n", (getTickCount() - time) / getTickFrequency() * 1000);
	imshow("contours", printImage);


	waitKey(0);
	return 0;
}