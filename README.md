# Parallel_NMS
Parallel CUDA implementation of NON maximum Suppression

Scanning the classifier across all positions and scales in the image yields multiple detections for the same object at similar scales and positions. Multiple overlapping detections need to be fused together. Standard mean shift algorithm requires a lot of random memory reads and writes. So we have come up with a much simpler parallelizable algorithm which takes the window with the maximum score, and then reject the remaining candidate windows if they have an intersection over union (IoU) larger than a learned threshold. We use the concept of adopting a map/reduce parallelization pattern which uses a boolean matrix both to encode candidate object detections and to compute their cluster Representatives
Non-maximum suppression is used as an intermediate step in many comptuer vision algorithms.

This repo gives you CPU and GPU implementation of NMS. I did not find good GPU implementation of NMS so wrote my own.

References: David Oro, Carles Fernandez, Xavier Martorell, Javier Hernando. Work-Efficient Parallel
non-maximum suppression for embedded GPU architecture,Herta Security, Barcelona, Spain.
