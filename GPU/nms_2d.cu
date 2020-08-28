#include <stdio.h>
#include <cuda_runtime.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdbool.h>
#include <math.h>
using namespace cv;
using namespace std;

#define BLOCKSIZE 32 //The number of threads per block should be not greater than 1024

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
    float ai = (float)(b1.w)*(b1.h);
    float aj = (float)(b2.w)*(b2.h);
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
    int count = 6000;

    bool *h_res =(bool *)malloc(sizeof(bool)*count);

    for(int i=0; i<count; i++)
    {
        h_res[i] = true;
    }


    box b[count];
    
    std::ifstream in;
    std::string line;
    
    in.open("boxes.txt"); //y1, x1, y2, x2
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
            i+=1;
            if(i==count) break;
        }
    }
    in.close();
    
    in.open("scores.txt");
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
    
    box *d_b;
    bool *d_res;

    gpuErrchk(cudaMalloc((void**)&d_res, count*sizeof(bool)));
    gpuErrchk(cudaMemcpy(d_res, h_res,sizeof(bool)*count, cudaMemcpyHostToDevice));

    gpuErrchk(cudaMalloc((void**)&d_b,sizeof(box)*count));
    gpuErrchk(cudaMemcpy(d_b, b,sizeof(box)*count, cudaMemcpyHostToDevice));
    
    //Setting 1: can only work when count <= 1024
    //NMS_GPU<<<dim3(1,count,1),count>>>(d_b,d_res);
    
    //Setting 2: work when count > 1024
    //NMS_GPU<<<dim3(count,count,1), 1>>>(d_b,d_res);
    
    //Setting 3: work when count > 1024, faster than Setting 2
    dim3 gridSize(int(ceil(float(count)/BLOCKSIZE)), int(ceil(float(count)/BLOCKSIZE)),1);
    dim3 blockSize(BLOCKSIZE, BLOCKSIZE, 1);
    NMS_GPU<<<gridSize, blockSize>>>(d_b,d_res);
    
    cudaThreadSynchronize();

    gpuErrchk(cudaMemcpy(h_res, d_res, sizeof(bool)*count, cudaMemcpyDeviceToHost));

    printf("Suppressed box id:\n");
    for(int i =0; i<count ; i++)
    {

        if(*(h_res+i) != true)
        {
            printf("%d ",i);
        }
    }

    return 0;
}
