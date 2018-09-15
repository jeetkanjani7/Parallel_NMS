#include <stdio.h>
#include <cuda_runtime.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdbool.h>
using namespace cv;
using namespace std;


#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
   if (code != cudaSuccess) 
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}


typedef struct
{
	float x,y,w,h,s;

}box;

__device__
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



__global__
void NMS_GPU(box *d_b, bool *d_res)
{
	int abs_y = (blockIdx.y * blockDim.y) + threadIdx.y;
	int abs_x = (blockIdx.x * blockDim.x) +threadIdx.x;


    
    float theta = 0.6;

    
  	if(d_b[abs_x].s < d_b[abs_y].s)
    	{
    		if(IOUcalc(d_b[abs_y],d_b[abs_x])>theta)
		{
	 		d_res[abs_x] = false; 
	 	}
    	}
  
}





int main()
{
	int count =3;
	Mat temp = imread("/home/jeetkanjani7/pedestrian_imgs/crop001025b.png",1);
	
	bool *h_res =(bool *)malloc(sizeof(bool)*count);
	
	for(int i=0; i<count; i++)
	{
		h_res[i] = true;
	}
	
	
	box b[3];
	b[1].x = 16; b[1].y = 12; b[1].w = 64; b[1].h = 128; b[1].s = 0.79062;
	b[2].x = 12; b[2].y = 14; b[2].w = 70; b[2].h = 141; b[2].s = 0.60434 ;
	b[0].x = 11; b[0].y = 6; b[0].w = 74; b[0].h = 148; b[0].s = 0.11855;

	
	box *d_b;
	bool *d_res;
	
	gpuErrchk(cudaMalloc((void**)&d_res, count*sizeof(bool)));
	gpuErrchk(cudaMemcpy(d_res, h_res,sizeof(bool)*count, cudaMemcpyHostToDevice));

	gpuErrchk(cudaMalloc((void**)&d_b,sizeof(box)*count));
	gpuErrchk(cudaMemcpy(d_b, b,sizeof(box)*count, cudaMemcpyHostToDevice));
		
	NMS_GPU<<<dim3(1,count,1),count>>>(d_b,d_res);
	
	cudaThreadSynchronize();
	
	gpuErrchk(cudaMemcpy(h_res, d_res, sizeof(bool)*count, cudaMemcpyDeviceToHost));
	
	for(int i =0; i<3 ; i++)
	{
		
		if(*(h_res+i) == true)
		{
			printf("Results= %d--%d ",i,*(h_res+i));
			rectangle(temp,Point(b[i].x,b[i].y),Point(b[i].x + b[i].w,b[i].y + b[i].h),Scalar(100,100,100),2,8,0);
		}
	}

	imshow("hello",temp);
	waitKey(0);
	return 0;
}
