//******************************************************************************** //
//                              Author: Soumya Dutta                               //
//                              CSE681 Introduction to Computer Graphics           //
//                              Lab Assignment 3 (Autumn 2011)                     //
//                              Date: October 18, 2011                             //  
//*********************************************************************************//



This program was developed in Stdsun Server of Computer Science Department of OSU

Input: A data file which has the specification of the 3D scene to be generated (extension .iv )
Output: A .ppm file (output.ppm) which contains the pixel map of the image, can be opened with Gimp or IrfanView for viewing. 


NOTE: Supersampling and Depth of Field is implemented separately. You have to run one by truning off the other. It was taking very long time to render the image, so I did not do them at the same time.
      How ever you can easily see the effect by turning off one of them. Default value of focalLength = 12. You can modify this value in order to change the effect of Depth of field.
	  
	  
List of required Files:

OSUInventor.C
OSUInventor.h
rt.C (Contains the main function)
sphere.h
my_camera.h
my_light.h
cube.h
Makefile
Ray Tracer Read Me.txt


To run the code:

1. Put all the files in a directory, Put a Data file to be rendered.
2. Make the program. It will create the object files and the executable rt.
3. Now run the executable: sample command to run -> rt sphereCycloid.iv 500 500 1 1 1 0 1( will render sphereCycloid.iv file with the following features on in order of parameter reflection,hard shadow, refraction, super sampling, depth of field)
4. Here you can give the runtime arguments. The code takes 9 arguments: 
   The command (i.e. name of the executable), 
   name of the file, 
   X_Resolution, 
   Y_Resolution, 
   reflection flag 1 = enable, 0 = disable 
   shadowflag(0 = no shadow, 1 = hard shadow, 2 = soft shadow) 
   refractionflag 0 = disable, 1 = enable
   supersample flag 0 = disable, 1 = enable
   depth of field flag 0 = disable, 1 = enable   
5. The output.ppm file will be generated in the project folder.

**********************************************************************************
* If you have any problem or concern please let me know, email: dutta.33@osu.edu *
**********************************************************************************



