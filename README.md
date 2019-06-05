## threading-opencv
Writing and testing faster multithreaded opencv programs with cpp interface. Tough to beat serial processing. 

OpenCV is an already fast library so speeding it up is really difficult. In this repository I try multiple threading approaches,
- Approach 1 : Split and merge.
Split single image into multiple rois and process them separately. It does not work.

- Approach 2 : Process multiple images simultaneously.
I instantiate multiple threads ( as much as the number of CPU cores) and operate multiple images simultaneously.
This resulted in a speedup of at least 4 times on 8 core machines even for lighter image operations

