This program was developed in Stdsun Server of Computer Science Department of OSU

Input: A data file which has the specification of the 3D scene to be generated (extension .iv )
Output: A .ppm file (output.ppm) which contains the pixel map of the image, can be opened with Gimp or IrfanView for viewing. 


NOTE: Supersampling and Depth of Field is implemented separately. You have to run one by truning off the other. It was taking very long time to render the image, so I did not do them at the same time.
      How ever you can easily see the effect by turning off one of them. Default value of focalLength = 12. You can modify this value in order to change the effect of Depth of field.
	  
	  Please see the sample outputs that I have rendered, The code that I am submitting will give texture image map only one a single sphere.
	  In order to get the textures on more than one sphere the code needs to be modified. However I am sending the outputs so that You can check the kind of images I am getting
	  I have submitted the images to Prof. Shen for image contest. If you want to see the images please let me know, I can come and show you by rendering the images in my Laptop.
	  In the present version of file the cylinder will always have texture and one of the sphere will have the input.jpg mapped in it. I am sending one sample input.jpg (this image file should 
	  have resolution 800X800). You can render my_iv_1.iv to see the different effects implemented in this lab.
	   	  
List of required Files:

OSUInventor.C
OSUInventor.h
rt.C (Contains the main function)
sphere.h
my_camera.h
my_light.h
cube.h
Makefile
frame.C
huffman.C
jpeg.C
myio.C
quant.C
scan.C
yuv.C
jpeg.h
frame.h
huffman.h
jpeg.h
myio.h
quant.h
scan.h
yuv.h
datastruct.h
pixel.h
input.jpg
ReadMe.txt
watch.iv
my_iv_1.iv
my_iv_2.jpg
my_iv_1.jpg
watch.jpg


To run the code:

1. Put all the files in a directory, Put a Data file to be rendered.
2. Make the program. It will create the object files and the executable rt.
3. Now run the executable: sample command to run -> rt my_iv_1.iv 500 500 1 1 1 0 0 input.jpg( will render my_iv_1.iv file with the following features on in order of parameter reflection,hard shadow, refraction, super sampling, depth of field)
4. Here you can give the runtime arguments. The code takes 10 arguments: 
   The command (i.e. name of the executable), 
   name of the file, 
   X_Resolution, 
   Y_Resolution, 
   reflection flag 1 = enable, 0 = disable 
   shadowflag(0 = no shadow, 1 = hard shadow, 2 = soft shadow) 
   refractionflag 0 = disable, 1 = enable
   supersample flag 0 = disable, 1 = enable
   depth of field flag 0 = disable, 1 = enable 
   input.jpg file that will be mapped to the sphere (this file should have resolution 800X800)
   
5. The output.ppm file will be generated in the project folder.

    To Run the file watch.iv the input command is: rt watch.iv 500 500 1 1 1 0 0 input.jpg
	To Run the file my_iv_1.jpg the input command is: rt my_iv_1.iv 500 500 1 1 1 0 0 input.jpg
	To Run the file my_iv_2.jpg the input command is: rt my_iv_2.iv 500 500 1 1 1 0 0 input.jpg
	

