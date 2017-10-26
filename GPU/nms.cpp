#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sstream>
#include "opencv2/imgproc/imgproc.hpp"
#include <fstream>
#include <iomanip>
#include <math.h>
#include <stdbool.h>
using namespace cv;

/*
void IOUcalc()
{
	if(D[i])
	A =  
	float left = max(), right = min(a[2], b[2]);
	float top = max(a[1], b[1]), bottom = min(a[3], b[3]);
	float width = max(right - left + 1, 0.f), height = max(bottom - top + 1, 0.f);
	float interS = width * height;
	float Sa = (a[2] - a[0] + 1) * (a[3] - a[1] + 1);
	float Sb = (b[2] - b[0] + 1) * (b[3] - b[1] + 1);
	return interS / (Sa + Sb - interS);
}*/
struct boxes
{
	float x,y,w,h,s;

}typedef box;





bool* nms(box *b, int count)
{
	float w,h,a;
	float theta = 0.6;
	printf("B[1] = %f",min((b[0].x + b[0].w),(b[1].x + b[1].w)) - max(b[0].x, b[1].x));
	bool res[count];

    for(int i=0; i<count; i++)
    {
    	for(int j=0; j<count; j++)
    	{
    		if(b[i].s < b[j].s)
	 		{
	 			a = (float)(b[j].x + 1)*(b[j].y + 1);
	 			w = (float)max((float)0,(min((b[i].x + b[i].w),(b[j].x + b[j].w)) - max(b[i].x,b[j].x)));  
	 			h = (float)max((float)0,min((b[i].y + b[i].h),(b[j].y + b[j].h)) - max(b[i].y,b[j].y));  
	 			if((w*h)/a > theta && b[j].w > 0)
	 			{
	 				printf("\n%d--%d\n",i,j);
	 				
	 				res[j] = true; 
	 			}
			}

    	}
    	
    }

   return res;


}

int main()
{
	Mat temp = imread("/home/jeetkanjani7/crop001025b.png",1);
	
	int count =3;
	box b[3];
	b[1].x = 16; b[1].y = 12; b[1].w = 64; b[1].h = 128; b[1].s = 0.79062;
	b[2].x = 12; b[2].y = 14; b[2].w = 70; b[2].h = 141; b[2].s = 0.60434 ;
	b[0].x = 11; b[0].y = 6; b[0].w = 74; b[0].h = 148; b[0].s = 0.11855;


	bool *res = nms(b,3);
	for(int i =0; i<3 ; i++)
	{
		if(!(res+i))
		{
			printf("Results= %d",i);
			rectangle(temp,Point(b[i].x,b[i].y),Point(b[i].x + b[i].w,b[i].y + b[i].h),Scalar(100,100,100),2,8,0);
		}
	}


	imwrite("hello.png",temp);
	return 0;
}