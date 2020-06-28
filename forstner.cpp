// forstner.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

int main()
{
    Mat src = imread("D:\\lena.jpg");
    Mat srcimg = imread("D:\\lena.jpg", 0);
    if (!srcimg.data)
    {
        return -1;
    }
    int rows = srcimg.rows;
    int cols = srcimg.cols;
    Point* p = new Point[rows, cols];
    Mat gray1;
    srcimg.convertTo(gray1, CV_64F);
    double w = 0, q = 0;
    double Tq = 0.9;
    double Tw = 1000;

    uchar gu, gv;
    vector<Point2i> corners;
    int nCount = 0;
    double sum = 0, ave = 0;
    for (int y = 2; y < rows - 3; y++)
    {
        for (int x = 2; x < cols - 3; x++)
        {
            // 计算Robert梯度
            gu = srcimg.at<uchar>(y + 1, x + 1) - srcimg.at<uchar>(y, x);
            gv = srcimg.at<uchar>(y + 1, x) - srcimg.at<uchar>(x, y + 1);
            double gu2 = 0, gv2 = 0, gugv = 0;

            //计算窗口的灰度协方差矩阵
            for (int i = y - 1; i <= y + 1; i++)
            {
                for (int j = x - 1; j <= x + 1; j++)
                {
                    gu2 += (gray1.at<double>(i + 1, j + 1) - gray1.at<double>(i, j)) * (gray1.at<double>(i + 1, j + 1) - gray1.at<double>(i, j));
                    gv2 += (gray1.at<double>(i, j + 1) - gray1.at<double>(i + 1, j)) * (gray1.at<double>(i, j + 1) - gray1.at<double>(i + 1, j));
                    gugv += (gray1.at<double>(i + 1, j + 1) - gray1.at<double>(i, j)) * (gray1.at<double>(i, j + 1) - (gray1.at<double>(i + 1, j)));
                }
            }

            //计算其兴趣值q w
            if ((gu2 + gv2) == 0)
            {
                w = 0;
                q = 0;
            }
            else
            {
                w = (gu2 * gv2 - gugv * gugv) / (gu2 + gv2);
                q = 4 * (gu2 * gv2 - gugv * gugv) / (gu2 + gv2) / (gu2 + gv2);
            }
            sum += w;
            nCount++;
            if (q > Tq && w > Tw)
            {
                corners.push_back(Point2i(x, y));
                nCount++;
            }
        }
    }
    for (vector<Point2i>::iterator it = corners.begin(); it != corners.end(); it++)
    {
        circle(src, *it, 2, Scalar(0, 0, 255));
    }
    imwrite("Forstner.jpg", src);
    imshow("Forstner", src);
    waitKey(0);

    return 0;
}
