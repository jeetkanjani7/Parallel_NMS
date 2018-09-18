#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sstream>
#include "opencv2/imgproc/imgproc.hpp"
#include <fstream>
#include <iomanip>
#include <math.h>
#include <stdbool.h>
using namespace cv;
using namespace std;



struct boxes
{
	float x,y,w,h,s;

}typedef box;



float IOUcalc(box b1, box b2)
{
	float ai = (float)(b1.w + 1)*(b1.h + 1);
	float aj = (float)(b2.w + 1)*(b2.h + 1);
	float x_inter, x2_inter, y_inter, y2_inter;

	x_inter = max(b1.x,b2.x);
	y_inter = max(b1.y,b2.y);
	
	x2_inter = min((b1.x + b1.w),(b2.x + b2.w));
	y2_inter = min((b1.y + b1.h),(b2.y + b2.h));
	
	float w = (float)max((float)0, x2_inter - x_inter);  
	float h = (float)max((float)0, y2_inter - y_inter);  
	
	float inter = ((w*h)/(ai + aj - w*h));
	return inter;
}


void nms_best(box *b, int count, bool *res)
{

	float theta = 0.15;

    for(int i=0; i<count; i++)
    {
    	res[i] = true;
    }

    for(int i=0; i<count; i++)
    {
    	 
    	for(int j=0; j<count; j++)
    	{

    		if(b[i].s > b[j].s)
	 		{
	 		
	 			if(IOUcalc(b[i],b[j]) > theta)
	 			{ 
	 				res[j] = false; 
	 			}

			}
    			
    		

    	}
    	
    }
    
}







/*void nms_binary(box *b, int count, bool *res)
{
	float w,h,a;
	float theta = 0.01;
	printf("B[1] = %f",min((b[0].x + b[0].w),(b[1].x + b[1].w)) - max(b[0].x, b[1].x));
	bool bool_mat[count*count];

    for(int i=0; i<count; i++)
    {
    	float ai = (float)(b[i].x + 1)*(b[i].y + 1);
    	for(int j=0; j<count; j++)
    	{
    		bool_mat[i*count + j] = false;
    		printf("\nScore: %f",b[j].s);
    		if(b[i].s > b[j].s)
	 		{
	 			
	 			float aj = (float)(b[j].x + 1)*(b[j].y + 1);
	 			w = (float)max((float)0,(min((b[i].x + b[i].w),(b[j].x + b[j].w)) - max(b[i].x,b[j].x)));  
	 			h = (float)max((float)0,min((b[i].y + b[i].h),(b[j].y + b[j].h)) - max(b[i].y,b[j].y));  
	 			if((w*h)/(ai + aj - w*h)> theta && b[j].w > 0)
	 			{
	 				//printf("\n%d--%d\n",i,j);
	 				bool_mat[i*count + j] = true;

	 			}

			}


    	}
    	
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
*/

box rnd_box(int ref_x, int ref_y)
{
	box b;
	b.x =ref_x + (rand()%300) - 200 ;
	b.y =ref_y + (rand()%150) - 100;
	b.w = rand()%370+30;
	b.h = b.w*2;
	b.s = (float)rand() / (float)RAND_MAX; 
	return b;
}

int main()
{
	string text;
	int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
	double fontScale = 1;
	int thickness = 2;


	int baseline=0;
	Size textSize = getTextSize(text, fontFace,
                            fontScale, thickness, &baseline);
	baseline += thickness;




	srand (time(NULL));
	Mat temp = imread("/home/jeetkanjani7/test_image.jpg",1);

	int count=10;
	
	box b[count];
	int ref_x = rand()%(1920 -600);
	int ref_y = rand()%(1080 -600);


	bool res1[count];
	bool res2[count];

	for(int i = 0; i<count; i++)
	{
		b[i] = rnd_box(ref_x ,ref_y );
		res1[i] = false;
		res2[i] = false;
	}
	

	


	
	nms_best(b,count,res2);
//	nms_binary(b, count, res1);
/*
	for(int i =0; i<count ; i++)
	{
	//	printf("%d -- %d\n",res1[i]?1:0,res2[i]?1:0);
		if(res2[i] != res1[i])
		{
			printf("Sorry bruh! %d",i);
			return 1;
		}
		
	}
	
*/


		
	Point textOrg((temp.cols - textSize.width)/2,
              (temp.rows + textSize.height)/2);


	for(int i =0; i<count ; i++)
	{
		std::ostringstream ss;
			ss <<b[i].s;
			string text(ss.str());
			Size textSize = getTextSize(text, fontFace,fontScale, thickness, &baseline);
		if(res2[i])
		{
			//printf("Results= %d",i);
			rectangle(temp,Point(b[i].x,b[i].y),Point(b[i].x + b[i].w,b[i].y + b[i].h),Scalar(255,0,0),2,8,0);
			//line(temp, Point(b[i].x,b[i].y),Point(b[i].x + b[i].w,b[i].y + b[i].h), Scalar(255, 0, 0));
			putText(temp, text,  Point(b[i].x,b[i].y), fontFace, fontScale,Scalar(255,0,0), thickness, 8);

		}
		else
		{
			rectangle(temp,Point(b[i].x,b[i].y),Point(b[i].x + b[i].w,b[i].y + b[i].h),Scalar(0,0,255),2,8,0);
			
			//line(temp, Point(b[i].x,b[i].y),Point(b[i].x + b[i].w,b[i].y + b[i].h), Scalar(255, 0, 0));
			putText(temp, text,  Point(b[i].x,b[i].y), fontFace, fontScale,Scalar(0,0,255), thickness, 8);
		}
		
	}

	

	imshow("nms_bool",temp);
	waitKey(0);
	return 0;
}
