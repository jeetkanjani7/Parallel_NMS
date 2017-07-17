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


__global__
void NMS_GPU(box *d_b, int count, bool *d_res)
{
	//int abs_y = (blockIdx.y * blockDim.y) + threadIdx.y;
	//int abs_x = (blockIdx.x * blockDim.x) +threadId.x;
	int Id = (blockIdx.x * blockDim.x) + threadIdx.x;

    float w,h,a;
    float theta = 0.6;
   	printf("\n%d--%f\n",Id,d_b[Id].x);
    
    for(int i = 0;i<count; i++)
    {
    	
    	if(d_b[i].s<d_b[Id].s)
    	{
    		a = (float)(d_b[Id].x + 1)*(d_b[Id].y + 1);
	 		w = (float)max((float)0,(min((d_b[i].x + d_b[i].w),(d_b[Id].x + d_b[Id].w)) - max(d_b[i].x,d_b[Id].x)));  
	 		h = (float)max((float)0,min((d_b[i].y + d_b[i].h),(d_b[Id].y + d_b[Id].h)) - max(d_b[i].y,d_b[Id].y));  
	 		if((w*h)/a > theta && d_b[Id].w > 0)
	 		{
	 			d_res[i] = false; 
	 		}
    	}
    }
}



int main()
{
	Mat temp = imread("/home/jeetkanjani7/crop001025b.png",1);
	
	int count =3;
	bool *h_res =(bool *)malloc(sizeof(bool)*count);
	for(int i=0; i<count; i++)
	{
		h_res[i] = true;
	}
	
	
	box b[3];
	b[1].x = 16; b[1].y = 12; b[1].w = 64; b[1].h = 128; b[1].s = 0.79062;
	b[2].x = 12; b[2].y = 14; b[2].w = 70; b[2].h = 141; b[2].s = 0.60434 ;
	b[0].x = 11; b[0].y = 6; b[0].w = 74; b[0].h = 148; b[0].s = 0.11855;

	printf("%d",(int)b[0].x);
	box *d_b;
	bool *d_res;
	
	gpuErrchk(cudaMalloc((void**)&d_res, count*sizeof(bool)));
	gpuErrchk(cudaMemcpy(d_res, h_res,sizeof(bool)*count, cudaMemcpyHostToDevice));

	gpuErrchk(cudaMalloc((void**)&d_b,sizeof(box)*count));
	gpuErrchk(cudaMemcpy(d_b, b,sizeof(box)*count, cudaMemcpyHostToDevice));
	
	NMS_GPU<<<1,count>>>(b,count,d_res);
	cudaThreadSynchronize();
	
	gpuErrchk(cudaMemcpy(h_res, d_res, sizeof(bool)*count, cudaMemcpyDeviceToHost));
	
	for(int i =0; i<3 ; i++)
	{
		printf("\nResults= %d--%d ",i,*(h_res+i));
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
