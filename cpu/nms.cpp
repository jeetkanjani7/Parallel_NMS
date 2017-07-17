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

void nms_best(box *b, int count, bool *res)
{
	float w,h,a;
	float theta = 0.6;
	printf("B[1] = %f",min((b[0].x + b[0].w),(b[1].x + b[1].w)) - max(b[0].x, b[1].x));

    for(int i=0; i<count; i++)
    {
    	res[i] = true;
    }

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
	 			
	 				res[j] = false; 
	 			}
			}

    	}
    	
    }
    
}




void nms_binary(box *b, int count, bool *res)
{
	float w,h,a;
	float theta = 0.6;
	printf("B[1] = %f",min((b[0].x + b[0].w),(b[1].x + b[1].w)) - max(b[0].x, b[1].x));
	bool bool_mat[count*count];

    for(int i=0; i<count; i++)
    {
    	for(int j=0; j<count; j++)
    	{
    		bool_mat[i*count + j] = false;
    		if(b[i].s < b[j].s)
	 		{
	 			a = (float)(b[j].x + 1)*(b[j].y + 1);
	 			w = (float)max((float)0,(min((b[i].x + b[i].w),(b[j].x + b[j].w)) - max(b[i].x,b[j].x)));  
	 			h = (float)max((float)0,min((b[i].y + b[i].h),(b[j].y + b[j].h)) - max(b[i].y,b[j].y));  
	 			if((w*h)/a > theta && b[j].w > 0)
	 			{
	 				printf("\n%d--%d\n",i,j);
	 				bool_mat[i*count + j] = true;

	 			}

			}


    	}
    	
    }

    for(int i=0; i<count; i++)
    {
    	for(int j=0; j<count; j++)
    	{
  			printf("%d  ",(bool_mat[i*count + j]) ? 1 : 0);
    	}
    	printf("\n");
    }

     int sum[count];
    for(int j=0; j<count; j++)
    {
    	sum[j] = 0;
    	for(int i=0; i<count; i++)
    	{
    		sum[j] += (bool_mat[i*count + j]) ? 1 : 0;

    	}
    	if(sum[j] != 0)
    	{
    		res[j] = false;
    	}
    	else
    	{
    		res[j] = true;
    	}
    }
    




}

int main()
{
	Mat temp = imread("/home/jeetkanjani7/crop001025b.png",1);
	
	int count =3;
	box b[3];
	b[1].x = 16; b[1].y = 12; b[1].w = 64; b[1].h = 128; b[1].s = 0.79062;
	b[2].x = 12; b[2].y = 14; b[2].w = 70; b[2].h = 141; b[2].s = 0.60434 ;
	b[0].x = 11; b[0].y = 6; b[0].w = 74; b[0].h = 148; b[0].s = 0.11855;
	
	bool res1[count] = {false,false,false};
	bool res2[count] = {false,false,false};

	
	nms_best(b,3,res1);
	nms_binary(b, 3, res2);

	for(int i =0; i<3 ; i++)
	{
		printf("%d -- %d\n",res1[i]?1:0,res2[i]?1:0);
		if(res2[i] != res1[i])
		{
			printf("Sorry bruh! %d",i);
			return 1;
		}
		
	}
	

	for(int i =0; i<3 ; i++)
	{
		if(res2[i])
		{
			printf("Results= %d",i);
			rectangle(temp,Point(b[i].x,b[i].y),Point(b[i].x + b[i].w,b[i].y + b[i].h),Scalar(100,100,100),2,8,0);
		}
	}

	imshow("nms_bool",temp);
	waitKey(0);
	return 0;
}