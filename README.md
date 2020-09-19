# Parallel NMS

Parallel CUDA implementation of NON maximum Suppression. I did not find good GPU implementation of NMS so wrote my own.

Scanning the classifier across all positions and scales in the image yields multiple detections for the same object at similar scales and positions. NMS is the process of selecting a single representative candidate within this cluster of detections. It has a wide range of applications including object detection speech segmentation, etc.

Problems with the existing standard NMS techniques:

* Standard mean shift algorithm requires a lot of random memory reads and writes.
* k-means requires a predetermined number of clusters which is unknown and difficult to estimate before time. Also, it only identifies convex clusters - so it cannot handle non linear data.
* Affinity propagation clustering algorithm is unworkable as it reports latency of 1000 ms to cluster around 250 canditates.

The proposal addresses these issue by adopting a map/reduce parallization pattern which uses a boolean matrix to encode candidate object detection bounding boxes.

For a boolean matrix of size dmax * dmax:

* assign 1 or 0 based on the IOU of two bbox’s di and dj being greater than a threshold.
* cancel out (assign -1) to (di, dj) where Area(di) is strictly less than Area(dj).
* perform and reductions across rows and put results into a vector dmax (1 only if all candidates in the row are not -1).

The above technique will result into a single representative per cluster.

GPU implementation is divided into Map and Reduce Kernel:

* The Map kernel creates the area of the box and creates the boolean matrix.
* Reduce kernel performs the and reduction.

The repository gives you CPU and GPU implementation of NMS.

References:

[1] David Oro, Carles Fernandez, Xavier Martorell, Javier Hernando. Work-Efficient Parallel non-maximum suppression for embedded GPU architecture,Herta Security, Barcelona, Spain.

### Project Structure

```

├── CPU
│   ├── Makefile
│   ├── nms.cpp
│   └── nms.o
├── GPU
│   ├── Makefile
│   ├── nms_2d
│   ├── nms_2d.cu
│   └── nms_2d.o
├── README.md
└── scores.txt
└── boxes.txt
```

