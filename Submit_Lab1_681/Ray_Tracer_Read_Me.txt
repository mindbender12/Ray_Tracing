//******************************************************************************** //
//                              Author: Soumya Dutta                               //
//                              CSE681 Introduction to Computer Graphics           //
//                              Lab Assignment 1 (Autumn 2011)                     //
//                              Date: October 17, 2011                             //  
//*********************************************************************************//



This program was developed in Microsoft Visual Studio 2008 in Windows 7 environment. The Coin3D library functions were used for the convenience of coding and reading iv files. 

Input: A data file which has the specification of the 3D scene to be generated (extension .iv )
Output: A .ppm file (output.ppm) which contains the pixel map of the image, can be opened with Gimp for viewing. 

List of required Files:

OSUInventor.cpp
OSUInventor.h
rt.cpp (Contains the main function)
sphere.h
my_camera.h
my_light.h
ray_traicer.vcproj (Visual studio .vcproj file )
Ray Tracer Read Me.txt


To run the code:

1.	You need to put the coin3d.lib, coin3d.dll on the project folder and the data file you want to see. 
2.      Also in the Project Properties -> C/C++ -> Additional Include Directories: This project file might have the include path of my local system. You have to change it according to your directory structure to be able to access the Coin3D library files.
3.	Open the .vcproj file with Visual Studio 2008. This should open the project file along with all the .cpp and .h files.
4.	Now go to Project  Properties -> Configuration Properties -> Debugging -> Command Arguments:
        Here you can give the runtime arguments. The code takes 4 arguments: The command, name of the file, X_Resolution, Y_Resolution. So in the Command Arguments section if you want to see the output of the file sphere1.iv  at resolution 500X500 then you can give the arguments as  sphere1.iv 500 500. 

5.	Now you can run the code. The output.ppm file will be generated in the project folder.

**********************************************************************************
* If you have any problem or concern please let me know, email: dutta.33@osu.edu * 
**********************************************************************************



