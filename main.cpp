#pragma warning (disable:4996)
#pragma warning (disable:4819)
#pragma warning (disable:4244)

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

bool turn = true;

#define PLUS	 1
#define MINUS	-1

static int counter= 0;


void Affine(const Mat src, Mat& dst, const Point2f srcTri[3], Point2f dstTri[3]) 
{
	Mat matrix(2, 3, CV_32FC1);

	if (turn) 
	{
		dstTri[1].x += MINUS;
		dstTri[2].x += PLUS;
		dstTri[1].y += MINUS;
		dstTri[2].y += PLUS;

	}
	else {
		dstTri[1].x += PLUS;
		dstTri[2].x += MINUS;
		dstTri[1].y += PLUS;
		dstTri[2].y += MINUS;
	}

	matrix = getAffineTransform(srcTri, dstTri);

	warpAffine(src, dst, matrix, dst.size());

}
void Perspective(const Mat src, Mat& dst, const Point2f srcQuad[4], Point2f dstQuad[4])
{
	Mat matrix(2, 4, CV_32FC1);

	if (turn) {
		dstQuad[0].x += MINUS;
		dstQuad[1].x += PLUS;
		dstQuad[2].x += PLUS;
		dstQuad[3].x += MINUS;
		dstQuad[0].y += PLUS;
		dstQuad[1].y += PLUS;
		dstQuad[2].y += MINUS;
		dstQuad[3].y += MINUS;
	}
	else {
		dstQuad[0].x -= MINUS;
		dstQuad[1].x -= PLUS;
		dstQuad[2].x -= PLUS;
		dstQuad[3].x -= MINUS;
		dstQuad[0].y -= PLUS;
		dstQuad[1].y -= PLUS;
		dstQuad[2].y -= MINUS;
		dstQuad[3].y -= MINUS;
	}
	matrix = getPerspectiveTransform(srcQuad, dstQuad);
	warpPerspective(src, dst, matrix, dst.size());

}

int main(int argc, char** argv)
{
	String openImagePath = "lenna.bmp";						// Image Path That You Want To Open.
	String originalName = "Original Image";

	if (argc >= 2) {
		openImagePath = argv[1];
	}

	Point2f srcTri[3];
	Point2f dstTri[3];

	Point2f srcQuad[4];
	Point2f dstQuad[4];

	Mat src, affine, perspective;

	src = imread(openImagePath, IMREAD_COLOR);


	GaussianBlur(src, src, Size(3, 3), 1);
	resize(src, src, Size(), 0.7, 0.7, INTER_LINEAR);

	affine = Mat(src.rows, src.cols, src.type());
	perspective = Mat(src.rows, src.cols, src.type());


	//Set Points

	srcTri[0] = Point2f(0, 0);
	srcTri[1] = Point2f(src.cols - 1, 0);
	srcTri[2] = Point2f(0, src.rows - 1);

	dstTri[0] = Point2f(src.cols*0.00, src.rows *0.10);
	dstTri[1] = Point2f(src.cols*0.90, src.rows* 0.20);
	dstTri[2] = Point2f(src.cols*0.10, src.rows* 0.80);

	srcQuad[0] = Point2f(0, 0);
	srcQuad[1] = Point2f(src.cols - 1, 0);
	srcQuad[2] = Point2f(0, src.rows - 1);
	srcQuad[3] = Point2f(src.cols - 1, src.rows-1);

	dstQuad[0] = Point2f(src.cols * 0.2, src.rows * 0.1);
	dstQuad[1] = Point2f(src.cols * 0.8, src.rows * 0.1);
	dstQuad[2] = Point2f(src.cols * 0.1, src.rows * 0.9);
	dstQuad[3] = Point2f(src.cols * 0.9, src.rows * 0.9);


	namedWindow("original");
	namedWindow("Affine");
	namedWindow("Perspective");
	
	moveWindow("original", 0, 0);
	moveWindow("Affine", src.cols, 0);
	moveWindow("Perspective", src.cols * 2, 0);


	imshow("original", src);

	for (;;)
	{
		Affine(src, affine, srcTri, dstTri);
		Perspective(src, perspective, srcQuad, dstQuad);
		imshow("Affine", affine);
		imshow("Perspective", perspective);
		if (++counter == 40) {
			counter = -40;
			turn = !turn;
		}
	
		if (waitKey(20) >= 0)
			break;

	}
	return 0;
}
