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
#include <chrono>
#include <stdbool.h>
using namespace cv;
using namespace std;



struct boxes
{
	float x,y,w,h,s;

}typedef box;



float IOUcalc(box b1, box b2)
{
	float ai = (float)(b1.w )*(b1.h );
	float aj = (float)(b2.w)*(b2.h);

	float x_inter, x2_inter, y_inter, y2_inter;

	x_inter = max(b1.x,b2.x);
	y_inter = max(b1.y,b2.y);
	
	x2_inter = min((b1.x + b1.w),(b2.x + b2.w));
	y2_inter = min((b1.y + b1.h),(b2.y + b2.h));
	
	float w = (float)max((float)0, x2_inter - x_inter);  
	float h = (float)max((float)0, y2_inter - y_inter);  
	
	float inter = ((w*h)/(ai + aj - w*h));
	//cout << inter;
	return inter;
}


void nms_best(box *b, int count, bool *res)
{

	float theta = 0.6;

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

	time_t start, end; 
	std::ifstream in;
    std::string line;

	time(&start); 
  	
	int count=6000;
	
	box b[count];

	/*
	int ref_x = rand()%(1920 -600);
	int ref_y = rand()%(1080 -600);
	*/

	bool result[count];


	in.open("../boxes.txt"); //y1, x1, y2, x2
    if (in.is_open()) 
    {
        int i = 0;
        while(getline(in, line))
        {
            istringstream iss(line);
            iss >> b[i].y;
            iss >> b[i].x;
            iss >> b[i].h; //y2
            iss >> b[i].w; //x2
            b[i].h-=b[i].y; //y2 -> h
            b[i].w-=b[i].x; //x2 -> w
            result[i] = true;
			i+=1;

            if(i==count) break;
        }
    }
    in.close();


    in.open("../scores.txt");
    if (in.is_open()) 
    {
        int i = 0;
        while(in >> b[i].s)
        {
            i+=1;
            if(i==count) break;
        }
    }
    in.close();

	
	nms_best(b, count, result);
	
	// unsync the I/O of C and C++. 
    ios_base::sync_with_stdio(false); 
  
    // Recording end time. 
    time(&end); 
  
    // Calculating total time taken by the program. 
    double time_taken = double(end - start); 
    cout << "Time taken by program is : " << fixed 
         << time_taken << setprecision(5); 
    cout << " sec " << endl; 

	for(int i=0; i<count ; i++)
	{

		if(result[i])
		{
			printf("%d ",i);	
		}
		
	}
	return 0;
}
