#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

Mat findColor(Mat src);

int main(int argc, char** argv) {

	String pathTest = "D:/ljp/p1/4/"; //测试图片文件夹地址
	String pathRight = "D:/ljp/p1/5/"; //正确识别图片文件夹地址
	
	vector<String> filenamesTest; //测试图片文件名
	vector<String> filenamesRight; //正确图片文件名

	glob(pathTest, filenamesTest); //opencv里面用来读取指定路径下文件名的一个很好用的函数
	glob(pathRight, filenamesRight);

	for (int i = 0; i < filenamesTest.size(); i++) {

		//if (i + 1 != 23) continue;

		Mat srcTest = imread(filenamesTest[i]); //读取每一张测试图片进行处理
		Mat srcRight = imread(filenamesRight[i]); //读取每一张正确图片进行处理

		if (srcTest.empty()) {
			printf("could not load image...\n");
			return -1;
		}

		//imshow("1", srcRight);

		Mat srcTestROI = findColor(srcTest);
		Mat srcRightROI = findColor(srcRight);

		//imshow("srcTestROI", srcTestROI);
		//imshow("srcRightROI", srcRightROI);

		//灰度化
		Mat grayTest, grayRight;
		cvtColor(srcTestROI, grayTest, COLOR_BGR2GRAY);
		cvtColor(srcRightROI, grayRight, COLOR_BGR2GRAY);

		//二值化
		Mat binaryTest, binaryRight;
		threshold(grayTest, binaryTest, 0, 255, THRESH_BINARY | THRESH_OTSU);
		threshold(grayRight, binaryRight, 0, 255, THRESH_BINARY | THRESH_OTSU);

		//imshow("binaryTest", binaryTest);
		//imshow("binaryRight", binaryRight);
		

		
		//找轮廓
		vector<vector<Point>> contoursTest;
		vector<vector<Point>> contoursRight;		

		vector<Vec4i> hireachy; //层次
		findContours(binaryTest, contoursTest, hireachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		findContours(binaryRight, contoursRight, hireachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

		cout << contoursTest.size() << "  " << contoursRight.size() << endl;	

		if (contoursTest.size() == 0 || contoursRight.size() == 0)
		{
			cout << "识别失败！！" << endl;
		}
		else
		{
			Mat resultTest = Mat::zeros(srcTestROI.size(), CV_8UC3); //把轮廓点画在上面
			Mat resultRight = Mat::zeros(srcRightROI.size(), CV_8UC3); //把轮廓点画在上面

			drawContours(resultTest, contoursTest, -1, Scalar(0, 255, 255), CV_FILLED, 8, hireachy, 0, Point());
			drawContours(resultRight, contoursRight, -1, Scalar(0, 255, 255), CV_FILLED, 8, hireachy, 0, Point());

			/*
			cout << contourArea(contoursTest[0]) << endl; //contoursTest 是数组 必须指定画哪个 contoursTest[0]第一个
			for (size_t t = 0; t < contoursTest.size(); t++) {
				// 面积过滤
				double area = contourArea(contoursTest[t]);  //计算轮廓面积
				cout << area << endl;
			}
			*/	

			//imshow("resultTest", resultTest);
			//imshow("resultRight", resultRight);

			double areaTest = contourArea(contoursTest[0]);  //计算测试图片轮廓面积
			double areaRight = contourArea(contoursRight[0]);  //计算准确图片轮廓面积

			if (areaTest == 0 || areaRight == 0)
			{
				printf("识别失败!!");
			}
			else
			{
				printf("第%d张测试图片像素点：%.2f\n", i + 1, areaTest);
				printf("第%d张正确图片像素点：%.2f\n", i + 1, areaRight);
				printf("第%d张测试准确率：%.2f%%\n", i + 1, areaTest / areaRight * 100);
			}

		}

		cout << endl;
	
	}	
	waitKey(0);
	return 0;
}


Mat findColor(Mat src)
{
	//识别红色区域
	Mat hsvImage;
	cvtColor(src, hsvImage, CV_BGR2HSV);

	Mat resultColor = Mat(hsvImage.rows, hsvImage.cols, CV_8UC3, cv::Scalar(0, 0, 0));

	double H = 0.0, S = 0.0, V = 0.0;
	for (int i = 0; i < hsvImage.rows; i++)
	{
		for (int j = 0; j < hsvImage.cols; j++)
		{
			H = hsvImage.at<Vec3b>(i, j)[0];  //获取图像每个像素点的H,S,V分量
			S = hsvImage.at<Vec3b>(i, j)[1];
			V = hsvImage.at<Vec3b>(i, j)[2];

		if ((H >= 0 && H < 10) || (H >= 156 && H < 180))
			if ((S >= 43) && (V >= 46))
			{
				resultColor.at<Vec3b>(i, j)[0] = src.at<Vec3b>(i, j)[0];
				resultColor.at<Vec3b>(i, j)[1] = src.at<Vec3b>(i, j)[1];
				resultColor.at<Vec3b>(i, j)[2] = src.at<Vec3b>(i, j)[2];
			}
		}
	}	
	return resultColor; //返回值
}



