
OPENCV_LIBPATH=-I/usr/local/lib -I/usr/local/lib/x86_64-linux-gnu/ -I/usr/lib/x86_64-linux-gnu/

OPENCV_INCLUDEPATH=-I/usr/local/include,-I/usr/include/opencv

OPENCV_LIBS=-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs

CUDA_INCLUDEPATH=/usr/lib/cuda/include

NVCC_OPTS=-O3 -arch=sm_30 -Xcompiler -Wall -Xcompiler -Wextra -m64 

GCC_OPTS=-m64 

nms: nms.o 
	g++ -o nms nms.o -L $(OPENCV_LIBPATH) $(OPENCV_LIBS)  

#main.o: main.cpp 
#g++ -c main.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH) -I $(OPENCV_INCLUDEPATH)

nms.o: nms.cpp
	g++ -c nms.cpp $(OPENCV_INCLUDEPATH) $(OPENCV_LIBS)


clean:
	rm -f *.o *.png hw
