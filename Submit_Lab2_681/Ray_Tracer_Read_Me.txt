//******************************************************************************** //
//                              Author: Soumya Dutta                               //
//                              CSE681 Introduction to Computer Graphics           //
//                              Lab Assignment 2 (Autumn 2011)                     //
//                              Date: October 31, 2011                             //  
//*********************************************************************************//



This program was developed in Stdsun Server of Computer Science Department of OSU

Input: A data file which has the specification of the 3D scene to be generated (extension .iv )
Output: A .ppm file (output.ppm) which contains the pixel map of the image, can be opened with Gimp for viewing. 

List of required Files:

OSUInventor.C
OSUInventor.h
rt.C (Contains the main function)
sphere.h
my_camera.h
my_light.h
Makefile
Ray Tracer Read Me.txt


To run the code:

1. Put all the files in a directory, Put a Data file to be rendered.
2. Make the program. It will create the object files and the executable rt.
3. Now run the executable: sample command to run -> rt sphere3s.iv 1000 1000 1 1 ( will render sphere3s.iv file with shadow and reflection on )
4. Here you can give the runtime arguments. The code takes 6 arguments: The command, name of the file, X_Resolution, Y_Resolution shadow_enable/diasble reflection_enable/disable. 
   shadow_enable/diasble = 1 means shadow is on, 0 means shadow is off. Similarly reflection_enable/disable = 1 means reflection is on and 0 means no reflection.
5. The output.ppm file will be generated in the project folder.

**********************************************************************************
* If you have any problem or concern please let me know, email: dutta.33@osu.edu * 
**********************************************************************************



