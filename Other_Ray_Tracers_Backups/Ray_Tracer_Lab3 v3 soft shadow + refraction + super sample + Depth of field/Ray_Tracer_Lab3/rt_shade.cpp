//******************************************************************************** //
//                              Author: Soumya Dutta                               //
//                              CSE681 Introduction to Computer Graphics           //
//                              Lab Assignment 3 (Autumn 2011)                     //
//                              Date: November 16, 2011                            //  
//******************************Header file includes*******************************//

#include <iostream>
#include <math.h>
#include <vector>

#include <Inventor/SbLinear.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoLight.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoLists.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoNormalElement.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/elements/SoTextureImageElement.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoCoordinate4.h>
#include <Inventor/nodes/SoLight.h>
#include <Inventor/nodes/SoSpotLight.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoVertexShape.h>
#include <Inventor/nodes/SoNonIndexedShape.h>
#include "OSUInventor.h"
#include "my_camera.h"
#include "my_light.h"
#include "sphere.h"
#include "time.h"

 using namespace std;

 void usage_error();
 SbVec3f Shade(int,SbVec3f,SbVec3f, my_sphere, my_light,int,int,int,int,int);
 float intersect(SbVec3f,SbVec3f,float,SbVec3f); 
 /***********************************main function starts here****************************/
 int main(int argc, char **argv)
{
 
 float minT=-1,k,aspectRatio,dist=10,imWidth,imHeight;
 SbVec3f rayOrig,rayDirection, center, intersection_point,intersection_normal,centerImagePlane,ImPlaneCorner,pixelColorFinal,pixelColorFinalSuper(0,0,0),pixelColorFinalDOF(0,0,0);
 SbVec3f uEye,vEye,nEye,pixelEyeDist,pixelCenterLoc,pixelColor,pixelColorTemp(0,0,0),pixelColorFinalTemp(0,0,0);
 int XRes,YRes,requiredReflection=1,requiredShadow=1,refractionRequired=1,requiredSupSample=1,requiredDof=1;
 FILE *fp;
 fp = fopen("output.ppm","w");
 XRes = atoi(argv[2]);
 YRes = atoi(argv[3]);
 requiredDof = atoi(argv[8]);
 requiredReflection = atoi(argv[4]);
 requiredShadow = atoi(argv[5]);
 refractionRequired =atoi(argv[6]);
 requiredSupSample=atoi(argv[7]);
 aspectRatio = float(XRes/YRes);
 SbVec3f subPixelCenter,subPixelCorner,pixelColorFinalSup(0,0,0);
 time_t seconds;
 time(&seconds);
 srand((unsigned int) seconds); 
 int focalLength = 10; // Change this to get variation in Depth of Field
 float lensParam = 1.5; // lens component increase this to get more blurring , for small values lensParam depth of field effect decreases

 
 if (argc != 9)
  {
    cout<<"usage: "<< argv[0] <<" <filename>\n";
    usage_error();
  }

  SoDB::init();
  OSUInventorScene *scene = new OSUInventorScene(argv[1]);
  int noOfObjects = scene->Objects.getLength();
  int noOfLights = scene->Lights.getLength();
  cout<<"noOfObjects: "<<noOfObjects<<endl;
  cout<<"noOfLights: "<<noOfLights<<endl;
  my_camera mc(scene);  
  my_light light(scene);
  my_sphere sphere(scene);

  //creating output ppm file and writing header of the file
  fprintf(fp,"P3 %d %d %d\n",XRes,YRes,255);
  
  rayOrig = mc.camera_position;
  cout<<"rayOrig: "<<rayOrig[0]<<rayOrig[1]<<rayOrig[2]<<endl;
  cout<<"mc.camera_direction: "<<mc.camera_direction[0]<<"|"<<mc.camera_direction[1]<<"|"<<mc.camera_direction[2]<<endl;
  
  //Calculating the eye plane basis vectors
  nEye = mc.camera_direction*(-1);
  nEye.normalize();//normalizing nEye vector
  uEye = mc.camera_up.cross(nEye);// uEye = view_up X nEye
  uEye.normalize();
  vEye = nEye.cross(uEye);  

  // Below cout statements are for debugging only
  cout<<"nEye: "<<nEye[0]<<"|"<<nEye[1]<<"|"<<nEye[2]<<endl;
  cout<<"uEye: "<<uEye[0]<<"|"<<uEye[1]<<"|"<<uEye[2]<<endl;
  cout<<"vEye: "<<vEye[0]<<"|"<<vEye[1]<<"|"<<vEye[2]<<endl;
  cout<<"mc.heightAngle:  "<<mc.heightAngle<<endl;


  /*  Image plane stup  */
  dist = 1;
  imHeight = 2*dist*tan(mc.heightAngle/2);
  imWidth = imHeight*aspectRatio;
  centerImagePlane = rayOrig-nEye;
  cout<<centerImagePlane[0]<<centerImagePlane[1]<<centerImagePlane[1]<<endl;
  ImPlaneCorner = centerImagePlane-uEye*(imWidth/2)-vEye*(imHeight/2);  
  float pixelWidth = imWidth/XRes,pixelHeight = imHeight/YRes;
  int max_color = 255;

  // Below cout statements are for debugging only
  cout<<"pixelWidth: "<<pixelWidth<<endl;
  cout<<"pixelHeight: "<<pixelHeight<<endl;
  cout<<"X_Y RES"<<XRes<<YRes<<endl;

  cout<<"imwidth:  "<<imWidth<<endl;
  cout<<"imheight:  "<<imHeight<<endl;
  cout<<"centerImagePlane:  "<<centerImagePlane[0]<<"|"<<centerImagePlane[1]<<"|"<<centerImagePlane[2]<<endl;
  cout<<"ImPlaneCorner:  "<<ImPlaneCorner[0]<<"|"<<ImPlaneCorner[1]<<"|"<<ImPlaneCorner[2]<<endl;


  //************************************************Loop starts here**************************************//
   for(int i=YRes-1;i>=0;i--)//the working version had YRes here
    { 
	 for(int j=0;j<XRes;j++)
	  {
		/****************     Calling Shade function to calculate the pixel color for primary ray      ***************/
        /*  Inside Shade the intersection function is called which returns the min_t value for each intersection point.  */
        /*  If no intersection happens then intersect function returns -1   */

		if(requiredSupSample==0) 
		{	 
	    pixelCenterLoc = ImPlaneCorner + (imWidth/XRes)*j*(uEye) + (imHeight/YRes)*i*(vEye);
		pixelEyeDist = pixelCenterLoc-rayOrig;//(s-e) vector
		SbVec3f pointAimed = rayOrig + focalLength*pixelEyeDist;// focalLength is the focal distance.

		    /* Calculation for DOF when no super sampling required*/
		    if(requiredDof ==0) 
		        {
		          pixelColorFinal = Shade(noOfObjects,rayOrig,pixelEyeDist,sphere,light,noOfLights,1,requiredReflection,requiredShadow,refractionRequired); 
		          pixelColorFinal = 255*pixelColorFinal;
		        }
			else if (requiredDof == 1) 
			    {
					//Form the plane and basis vectors
			        SbVec3f tempDir = pixelEyeDist; // pixelEyeDist is the main ray direction
			        SbVec3f vUpDof,uDof,nDof,vDof;					
					nDof = (-1)*pixelEyeDist;
					
			        if(tempDir[0] >= tempDir[1] && tempDir[0] >= tempDir[2])
				       vUpDof.setValue(1,0,0);
			        if(tempDir[1] >= tempDir[0] && tempDir[1] >= tempDir[2])
				       vUpDof.setValue(0,1,0);
			        if(tempDir[2] >= tempDir[0] && tempDir[2] >= tempDir[1])
				       vUpDof.setValue(0,0,1);

                    nDof.normalize();               
			        uDof = vUpDof.cross(nDof);
			        vDof = nDof.cross(uDof);

					  for(int di=0; di<4; di++)
					   {
						 for(int dj =0;dj<4;dj++)
						 {
                            float dk = rand()/float(RAND_MAX+1);
                            float dl = rand()/float(RAND_MAX+1);                            

							SbVec3f dofRayOrig = rayOrig  -(lensParam/2)*uDof - (lensParam/2)*vDof;
                            dofRayOrig = dofRayOrig + lensParam*(dk)*uDof + lensParam*(dl)*vDof;
							SbVec3f dofRayDir = pointAimed - dofRayOrig;
							dofRayDir.normalize();
							pixelColorFinal = Shade(noOfObjects,dofRayOrig,dofRayDir,sphere,light,noOfLights,1,requiredReflection,requiredShadow,refractionRequired); 
							pixelColorTemp += pixelColorFinal;
		                  }
		                }
					  pixelColorFinal = pixelColorTemp/16;
			          pixelColorTemp.setValue(0,0,0);	
			          pixelColorFinal = 255*pixelColorFinal;

			    }
		}
		else if(requiredSupSample==1)
		{	    
		   if(requiredDof == 0) 
		    {
			 for(int k =0; k<4; k++)
			 {
				for(int l=0;l<4;l++)
				{
					float du = rand()/float(RAND_MAX+1);
					float dv = rand()/float(RAND_MAX+1);
					pixelCenterLoc = ImPlaneCorner + (imWidth/XRes)*(j+du)*(uEye) + (imHeight/YRes)*(i+dv)*(vEye);
					pixelEyeDist = pixelCenterLoc-rayOrig;//(s-e) vector
					pixelEyeDist.normalize();
					pixelColorFinal = Shade(noOfObjects,rayOrig,pixelEyeDist,sphere,light,noOfLights,1,requiredReflection,requiredShadow,refractionRequired); 
					pixelColorFinalSup += pixelColorFinal;
				}
			 }
			 pixelColorFinal = pixelColorFinalSup/16;			 
			 pixelColorFinalSup.setValue(0,0,0);
			 pixelColorFinal = 255*pixelColorFinal;
		   }
		   else
		   {
			  	// Implement DOF + Super Sample here	
		   }
		   
		}

	   /* Truncating the value of each pixel to maximum at 255 */
		if(pixelColorFinal[0] > 255)
	     pixelColorFinal[0] = 255;
       if(pixelColorFinal[1] > 255)
         pixelColorFinal[1] = 255;
       if(pixelColorFinal[2] > 255)
	     pixelColorFinal[2] = 255;
      
	   /* writing pixel values in output file */
		fprintf(fp,"%d %d %d\n",int(pixelColorFinal[0]),int(pixelColorFinal[1]),int(pixelColorFinal[2]));		 
 	}  
  }

  fclose(fp);
  system("pause");
  return 0;
 }
/* End of Main Function */


/* Intersect function definition  */
float intersect(SbVec3f rayOrig,SbVec3f rayDirection,float radius,SbVec3f centerSphere)
 {
	  float A,B,C,minT=-1.0;
	  rayDirection.normalize(); //normalizing the vector
	  SbVec3f a,b;

	 //finding the coefficients for calculating the disciminant
     A = rayDirection.dot(rayDirection);
	 a = (rayOrig - centerSphere);
	 B = 2*(rayDirection.dot(a));
	 C = a.dot(a) - (radius*radius);
 
     //checking if B^2-4C >0  
     float checkVal = B*B-4*A*C;
	 //cout<<checkVal;
     if (checkVal>=0)
      {
	    float t0 = (-B-(sqrtf(checkVal)))/(2*A);
	    float t1 = (-B+(sqrtf(checkVal)))/(2*A);

	    if(t0<t1 && t0>=0)
			minT = t0;
		else if(t0>t1 && t1>=0)
			minT =t1;
		else
			minT = -1.0;
     
	     }
	 return minT;
 }

/* Error function definition */ 
void usage_error()
 {
  cerr << "Usage: sample_read_iv {filename}" << endl;
  system ("pause");
  exit(10);
 }


/* Shade Function Definition */
SbVec3f Shade(int noOfObjects,SbVec3f rayOrig, SbVec3f pixelEyeDist, my_sphere sphere, my_light light, int noOfLights,int recursiondepth, int requiredReflection, int requiredShadow, int refractionRequired)
  {
	 float R=0.15,G=0.15,B=0.15,temp,checkshadow=-1;
	 float minT=0,diffuseColor=0,ambientColor=0,specularColortemp=0,*tempT,min=1,temp1=0,diffuseColorSub=0;
	 tempT = (float *)malloc(noOfObjects*sizeof(float));
	 SbVec3f intersection_point,intersection_normal,pixelColor,pixelColorFinal,refractionRay(0,0,0),pixelEyeDistNormalized,TransmittedRay,specularColorSub(0,0,0);
	 SbVec3f shadowRay,viewRay,intensity(1,1,1),specularColor(0,0,0),shadowRayOrig,shadowRaydir,reflectionRay,returnShadeColor(0,0,0),returnRefractColor(0,0,0); 
	 int loc=0, intersection=0,sp,intersectLight=0;
	 int MAXRECURSION=2;
	 float shine = 0.5,trans=1, Ni = 1/1.5,epsilon = 0.0000009,epsilon1= 0.009,epsilon2= 0.000009,temp2=0;// epsilon2 for reflection
	 pixelEyeDistNormalized = pixelEyeDist;
	 pixelEyeDistNormalized.normalize();
	 float diffuselight=0;
	 SbVec3f specularColorlight(0,0,0);

	 
  //object wise loop
   for(int q=0;q<noOfObjects;q++)
   {
	/* Calling the Intersect Function for Intersection Calculation */ 
	   tempT[q] = intersect(rayOrig,pixelEyeDist,sphere.radius[q],sphere.centerSphere[q]);
	   if (tempT[q] == -1.0)// means no intersection
		   tempT[q]=11111; // set tempT[q] = 11111
   }

   min = tempT[0]; // min corresponds to the closest sphere for a single pixel, so the pixel will have the color of this sphere
   loc =0; // loc corresponds the number of sphere which is the closest one
   for(int q=0;q<noOfObjects;q++)
   {
	  if(min>tempT[q])
	  {	  min = tempT[q];
	      loc = q;
	  }
   }  
  
	if((min >= 0) && (min != 11111))// min = 11111 means background pixel
	 {
		  diffuseColor =0; 
		  diffuseColorSub = 0;
		  specularColorlight.setValue(0,0,0);
		  diffuselight=0;
		  specularColorSub.setValue(0,0,0);
		  temp = 0;
		  specularColortemp =0;
		  //pixelColor.setValue(0,0,0);
		  intersection_point = rayOrig + (pixelEyeDist)*min;		  
		  //intersection_point = intersection_point + pixelEyeDist*epsilon;// epsilon = 0.009 added to remove the self-intersection issue
		
		  intersection_normal = (intersection_point-sphere.centerSphere[loc]);
          intersection_normal.normalize();

		  shadowRayOrig = intersection_point + pixelEyeDist*epsilon;
		 		 
          // Ambient Color is constant as we have assumed and it is Ka*Ia
		  pixelColor[0] = (sphere.ambientColor[loc][0]*sphere.diffColor[loc][0]);
		  pixelColor[1] = (sphere.ambientColor[loc][1]*sphere.diffColor[loc][1]);
		  pixelColor[2] = (sphere.ambientColor[loc][2]*sphere.diffColor[loc][2]);
		 

		  // Starting calculation of diffuse and specular color for all the lights
		  /********************light loop********************/
         
 if(requiredShadow == 1) 
    {
		for( int r =0; r<noOfLights; r++)
		  {
           shadowRaydir = light.location[r] - shadowRayOrig;// direction of shadow ray
		   light.location[r] = light.location[r] - shadowRayOrig;
		   light.location[r].normalize();// now light.location[r] is the light vector
		   temp = intersection_normal.dot(light.location[r]);

		   if (temp > 0) // the intersection point facing light source
	       {
			   
            //object wise checking the intersection with shadow ray
            for(int q=0;q<noOfObjects;q++)
              {
				  if(sphere.transparency[q]<=0)
				  {
			    /* Calling the Intersect Function for Intersection calculation */ 
	            checkshadow = intersect(shadowRayOrig,shadowRaydir,sphere.radius[q],sphere.centerSphere[q]);
				if (checkshadow == -1.0)
				  {
				  intersection = 0; // no intersection
				  }
				else
				 {   
					 intersection = 1;
				     break; 
     		      }		
			   } 
			}
		  
	   	if(intersection == 0) // no intersection
		 {		
	       // diffuse component calculation	   
		   diffuseColor = diffuseColor + max<float>((intersection_normal.dot(light.location[r])),0); //diffuse component = (N.L)*Kd*Id, diffuseColor = (N.L)
		   
		   //specular component	calculation	   
		   shadowRay = 2*intersection_normal*temp - light.location[r];
		   shadowRay.normalize();
		   viewRay = rayOrig - intersection_point;
		   viewRay.normalize(); 
		   specularColortemp = viewRay.dot(shadowRay);
           specularColortemp = pow(specularColortemp,50);
		   specularColor[0] = specularColor[0] + specularColortemp*(intensity[0])*(sphere.specularColor[loc][0]);
	       specularColor[1] = specularColor[1] + specularColortemp*(intensity[1])*(sphere.specularColor[loc][1]);
		   specularColor[2] = specularColor[2] + specularColortemp*(intensity[2])*(sphere.specularColor[loc][2]);
				
		   } // if(intersection == 0) loop Ends

		  } // if (temp >0) loop ends		     
		  
		if(recursiondepth < MAXRECURSION) //raflection should still happen even the point is in shadow.. 
		 {    
			if(requiredReflection == 1) // Add reflection only if required. Determined through runtime binary parameter requiredReflection (1,0)
		        {
		        //Here Formulate Reflection Ray and Call Shade again (Recursive Call)
		         temp1 = pixelEyeDist.dot(intersection_normal);
                 reflectionRay = pixelEyeDist - 2*temp1*(intersection_normal);
		         returnShadeColor = returnShadeColor + Shade(noOfObjects,(intersection_point + pixelEyeDist*epsilon2),reflectionRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired); 
		         returnShadeColor[0] = shine*returnShadeColor[0];
		         returnShadeColor[1] = shine*returnShadeColor[1];
		         returnShadeColor[2] = shine*returnShadeColor[2];
		  		
				}

				if(refractionRequired ==1)
				{
				 if(sphere.transparency[loc]>0)
				 {
					 
					 temp2 = pixelEyeDistNormalized.dot(intersection_normal); 
					
					//Here Formulate Refraction Ray and Call Shade again (Recursive Call)
			    	if(temp2 < 0) //(IncomingRay.Normal < 0) Ray is coming from air to object 
					{
						
						//refractionRay = refract(pixelEyeDist,intersection_normal,Ni,temp2); // refract returns the refraction ray direction 
						float rootContent = (1 - Ni * Ni * (1 - (intersection_normal.dot(-pixelEyeDistNormalized) * (intersection_normal.dot(-pixelEyeDistNormalized)))));
	                    rootContent = sqrtf(rootContent);
		                TransmittedRay = (Ni * (intersection_normal.dot(-pixelEyeDistNormalized)) - rootContent) * intersection_normal - ( Ni * -pixelEyeDistNormalized );
	                    returnRefractColor = returnRefractColor + Shade(noOfObjects,(intersection_point + (pixelEyeDist*epsilon1)),TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired); 
						
				    }
					else
					{
						Ni = 1/Ni;
						//refractionRay = refract(pixelEyeDist,intersection_normal,1/Ni,temp2); // refract returns the refraction ray direction 
						float rootContent = (1 - Ni * Ni * (1 - (-intersection_normal.dot(-pixelEyeDistNormalized) * (-intersection_normal.dot(-pixelEyeDistNormalized)))));
		                if(rootContent>0.0)
	                      {
		                     rootContent = sqrtf(rootContent);
		                     TransmittedRay = (Ni * (-intersection_normal.dot(-pixelEyeDistNormalized)) - rootContent)*(-intersection_normal) - ( Ni * -pixelEyeDistNormalized );
	                      	returnRefractColor = returnRefractColor + Shade(noOfObjects,(intersection_point + (pixelEyeDist*epsilon1)),TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired); 
						 }  
					}
					
				 } //if(sphere.transparency[loc]>0) Loop ends

				}//if(refractionRequired ==1) Ends

	  }// recursion depth loop ends	

	}// light loop ends for with shadow part		  

  } // if(requiredShadow == 1) part ends.. to confirm that shadow is only calculated when required , determined according to the runtime argument

 else if(requiredShadow == 0) // else shadow is not required, else part of if(requiredShadow == 1)
	{
		for( int r =0; r<noOfLights; r++)
		  {   
           shadowRaydir = light.location[r] - shadowRayOrig;// direction of shadow ray
		   light.location[r] = light.location[r] - shadowRayOrig;
		   light.location[r].normalize();// now light.location[r] is the light vector
		   temp = intersection_normal.dot(light.location[r]);

	      if (temp >= 0) // the intersection point facing light source then it get color no shadow chek is happening here
	       {
		       
		   // diffuse component calculation	   
		    diffuseColor = diffuseColor + max<float>((intersection_normal.dot(light.location[r])),0); //diffuse component = (N.L)*Kd*Id, diffuseColor = (N.L)
		   
		   //specular component	calculation	   
		   shadowRay = 2*intersection_normal*temp - light.location[r];
		   shadowRay.normalize();
		   viewRay = rayOrig - intersection_point;
		   viewRay.normalize(); 
		   specularColortemp = viewRay.dot(shadowRay);
           specularColortemp = pow(specularColortemp,50);
		   specularColor[0] = specularColor[0] + specularColortemp*(intensity[0])*(sphere.specularColor[loc][0]);
	       specularColor[1] = specularColor[1] + specularColortemp*(intensity[1])*(sphere.specularColor[loc][1]);
		   specularColor[2] = specularColor[2] + specularColortemp*(intensity[2])*(sphere.specularColor[loc][2]);			  
		
	        }
	  if(recursiondepth < MAXRECURSION) // raflection should still happen even the point is in shadow.. 
		      {    
				  if(requiredReflection == 1) // Add reflection only if required. Determined through runtime binary parameter requiredReflection (1,0)
		          {
		        //Here Formulate Reflection Ray and Call Shade again (Recursive Call)
		         temp1 = pixelEyeDist.dot(intersection_normal);
                 reflectionRay = pixelEyeDist - 2*temp1*(intersection_normal);
		         returnShadeColor = returnShadeColor + Shade(noOfObjects,intersection_point,reflectionRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired); 
		         returnShadeColor[0] = shine*returnShadeColor[0];
		         returnShadeColor[1] = shine*returnShadeColor[1];
		         returnShadeColor[2] = shine*returnShadeColor[2];		  		
				  }

				if(refractionRequired ==1)
				{
				 if(sphere.transparency[loc]>0)
				 {
					 temp2 = pixelEyeDistNormalized.dot(intersection_normal); 
					
					//Here Formulate Refraction Ray and Call Shade again (Recursive Call)
			    	if(temp2 < 0) //(ViewRay.Normal < 0) Ray is coming from air to object 
					{
						//refractionRay = refract(pixelEyeDist,intersection_normal,Ni,temp2); // refract returns the refraction ray direction 
						float rootContent = (1 - Ni * Ni * (1 - (intersection_normal.dot(-pixelEyeDistNormalized) * (intersection_normal.dot(-pixelEyeDistNormalized)))));
	                    rootContent = sqrtf(rootContent);
		                TransmittedRay = (Ni * (intersection_normal.dot(-pixelEyeDistNormalized)) - rootContent) * intersection_normal - ( Ni * -pixelEyeDistNormalized );
	                    returnRefractColor = returnRefractColor + Shade(noOfObjects,intersection_point,TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired); 
						
				    }
					else
					{
						Ni = 1/Ni;
						//refractionRay = refract(pixelEyeDist,intersection_normal,1/Ni,temp2); // refract returns the refraction ray direction 
						float rootContent = (1 - Ni * Ni * (1 - (-intersection_normal.dot(-pixelEyeDistNormalized) * (-intersection_normal.dot(-pixelEyeDistNormalized)))));
		                if(rootContent>0.0)
	                      {
		                     rootContent = sqrtf(rootContent);
		                     TransmittedRay = (Ni * (-intersection_normal.dot(-pixelEyeDistNormalized)) - rootContent)*(-intersection_normal) - ( Ni * -pixelEyeDistNormalized );
	                      	returnRefractColor = returnRefractColor + Shade(noOfObjects,intersection_point,TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired); 
						 }  
					}
					
				 } //if(sphere.transparency[loc]>0) Loop ends

				}//if(refractionRequired ==1) Ends

	  }// recursion depth loop ends		
		
	  }// light loop ends for without shadow
		
	}

 else // soft shadow calculation with Phong Model requiredShadow = 2 here
	{		
		for( int r =0; r<noOfLights; r++)
		  {
           SbVec3f lightCenter = light.location[r];
		   light.location[r] = light.location[r] - shadowRayOrig;
		   light.location[r].normalize();// now light.location[r] is the light vector
		   temp = intersection_normal.dot(light.location[r]);
		   
		   if (temp > 0) // the intersection point facing light source
	       {
			   //Form the plane and basis vectors
			   SbVec3f tempLight = light.location[r];
			   SbVec3f vUpSoft,uSoft,nSoft,vSoft;
			   nSoft = (-1)*light.location[r];
			 if(tempLight[0] >= tempLight[1] && tempLight[0] >= tempLight[2])
				vUpSoft.setValue(1,0,0);
			 if(tempLight[1] >= tempLight[0] && tempLight[1] >= tempLight[2])
				vUpSoft.setValue(0,1,0);
			 if(tempLight[2] >= tempLight[0] && tempLight[2] >= tempLight[1])
				vUpSoft.setValue(0,0,1);
               
			  //vUpSoft.normalize();
			  uSoft = vUpSoft.cross(nSoft);
			  vSoft = nSoft.cross(uSoft);
					 
			  diffuseColorSub = 0;
			  specularColorSub.setValue(0,0,0);
			                
			  for(int k=0;k<6;k++)
			  {
				  for(int l=0;l<6;l++)
				  {
					 float dx = rand()/float(RAND_MAX+1);
					 float dy = rand()/float(RAND_MAX+1);

					SbVec3f subPixelCenter = lightCenter  -(2)*uSoft - (2)*vSoft;// radius = 3
                    subPixelCenter = subPixelCenter + 4*(dx)*uSoft + 4*(dy)*vSoft;
                    SbVec3f softShadowRayDir = subPixelCenter - shadowRayOrig;	
					shadowRayOrig = shadowRayOrig + 0.000007*softShadowRayDir;

					//float root = 3/sqrtf(pow((lightCenter[0]-shadowRayOrig[0]),2)+ pow((lightCenter[1]-shadowRayOrig[1]),2)+pow((lightCenter[2]-shadowRayOrig[2]),2));
					//SbVec3f softShadowRayDir =( lightCenter  + uSoft*root*cos(3*dx) + vSoft*root*sin(3*dy))- shadowRayOrig;
					//shadowRayOrig = shadowRayOrig + 0.0001*softShadowRayDir;
				     		   
                      //object wise checking the intersection with shadow ray
                      for(int q=0;q<noOfObjects;q++)
                       {
				          if(sphere.transparency[q]<=0)
				           {
			                  /* Calling the Intersect Function for Intersection calculation */ 
	                           checkshadow = intersect(shadowRayOrig,softShadowRayDir,sphere.radius[q],sphere.centerSphere[q]);
				                   if (checkshadow == -1.0) // Ray does not intersect with any of the objects
				                     {
										intersection = 0;
								      }
								   else 
				                     {   
					                     intersection = 1;
				                         break;
									 }
     		               }
					    }

               if(intersection==0)
			    {
				 diffuseColorSub += 1;
				 specularColorSub[0] += 1;
	             specularColorSub[1] += 1;
		         specularColorSub[2] += 1;					  
	       		
		         } // if(intersection == 0) loop Ends
			   else
			    {
					
				 diffuseColorSub += 0;
				 specularColorSub[0] += 0;
	             specularColorSub[1] += 0;
		         specularColorSub[2] += 0;					  
	       		}
			 
		      }
		    } // end internal for loop

			
		   // diffuse component calculation	   
		   diffuseColor = diffuseColor + max<float>((intersection_normal.dot(light.location[r])),0); //diffuse component = (N.L)*Kd*Id, diffuseColor = (N.L)
		  		   
		   //specular component	calculation	   
		   shadowRay = 2*intersection_normal*temp - light.location[r];
		   shadowRay.normalize();
		   viewRay = rayOrig - intersection_point;
		   viewRay.normalize(); 
		   specularColortemp = pow((viewRay.dot(shadowRay)),50);
           specularColor[0] = specularColor[0] + specularColortemp*(intensity[0])*(sphere.specularColor[loc][0]);
	       specularColor[1] = specularColor[1] + specularColortemp*(intensity[1])*(sphere.specularColor[loc][1]);
		   specularColor[2] = specularColor[2] + specularColortemp*(intensity[2])*(sphere.specularColor[loc][2]);
			
          } // end of N.L check

		   

		   diffuseColor = diffuseColor*(diffuseColorSub/36);
		   specularColor[0] = specularColor[0]*(specularColorSub[0]/36);
		   specularColor[1] = specularColor[1]*(specularColorSub[1]/36);
		   specularColor[2] = specularColor[2]*(specularColorSub[2]/36);

	  	  
	if(recursiondepth < MAXRECURSION) //raflection should still happen even the point is in shadow.. 
		 {    
				if(requiredReflection == 1) // Add reflection only if required. Determined through runtime binary parameter requiredReflection (1,0)
		        {
		        //Here Formulate Reflection Ray and Call Shade again (Recursive Call)
		         temp1 = pixelEyeDist.dot(intersection_normal);
                 reflectionRay = pixelEyeDist - 2*temp1*(intersection_normal);
		         returnShadeColor = returnShadeColor + Shade(noOfObjects,intersection_point,reflectionRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired); 
		         returnShadeColor[0] = shine*returnShadeColor[0];
		         returnShadeColor[1] = shine*returnShadeColor[1];
		         returnShadeColor[2] = shine*returnShadeColor[2];
		  		
				}

				if(refractionRequired ==1)
				{
				 if(sphere.transparency[loc]>0)
				 {
					 
					 temp2 = pixelEyeDistNormalized.dot(intersection_normal); 
					
					//Here Formulate Refraction Ray and Call Shade again (Recursive Call)
			    	if(temp2 < 0) //(ViewRay.Normal < 0) Ray is coming from air to object 
					{
						
						//refractionRay = refract(pixelEyeDist,intersection_normal,Ni,temp2); // refract returns the refraction ray direction 
						float rootContent = (1 - Ni * Ni * (1 - (intersection_normal.dot(-pixelEyeDistNormalized) * (intersection_normal.dot(-pixelEyeDistNormalized)))));
	                    rootContent = sqrtf(rootContent);
		                TransmittedRay = (Ni * (intersection_normal.dot(-pixelEyeDistNormalized)) - rootContent) * intersection_normal - ( Ni * -pixelEyeDistNormalized );
	                    returnRefractColor = returnRefractColor + Shade(noOfObjects,(intersection_point + (pixelEyeDist*epsilon1)),TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired); 
						
				    }
					else
					{
						Ni = 1/Ni;
						//refractionRay = refract(pixelEyeDist,intersection_normal,1/Ni,temp2); // refract returns the refraction ray direction 
						float rootContent = (1 - Ni * Ni * (1 - (-intersection_normal.dot(-pixelEyeDistNormalized) * (-intersection_normal.dot(-pixelEyeDistNormalized)))));
		                if(rootContent>0.0)
	                      {
		                     rootContent = sqrtf(rootContent);
		                     TransmittedRay = (Ni * (-intersection_normal.dot(-pixelEyeDistNormalized)) - rootContent)*(-intersection_normal) - ( Ni * -pixelEyeDistNormalized );
	                      	returnRefractColor = returnRefractColor + Shade(noOfObjects,(intersection_point + (pixelEyeDist*epsilon1)),TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired); 
						 }  
					}
					
				 } //if(sphere.transparency[loc]>0) Loop ends

				}//if(refractionRequired ==1) Ends

	  }// recursion depth loop ends	

	  }// light loop ends for with shadow part  

		  diffuselight = diffuselight + diffuseColor;		  
		  specularColorlight[0] = specularColorlight[0] + specularColor[0];
		  specularColorlight[1] = specularColorlight[1] + specularColor[1];
		  specularColorlight[2] = specularColorlight[2] + specularColor[2];		  

	} // if(requiredShadow == 2) part ends.. to confirm that soft shadow is only calculated when required, determined according to the runtime argument

     if(requiredShadow == 2 )
	 {
		  pixelColor[0] = pixelColor[0] + (1-sphere.transparency[loc])*diffuselight*(sphere.diffColor[loc][0])+ specularColorlight[0]+ returnShadeColor[0]+ sphere.transparency[loc]*returnRefractColor[0];
		  pixelColor[1] = pixelColor[1] + (1-sphere.transparency[loc])*diffuselight*(sphere.diffColor[loc][1])+ specularColorlight[1]+ returnShadeColor[1]+ sphere.transparency[loc]*returnRefractColor[1];
     	  pixelColor[2] = pixelColor[2] + (1-sphere.transparency[loc])*diffuselight*(sphere.diffColor[loc][2])+ specularColorlight[2]+ returnShadeColor[2]+ sphere.transparency[loc]*returnRefractColor[2];
	 }
	 else
	 {
		  pixelColor[0] = pixelColor[0] + (1-sphere.transparency[loc])*(diffuseColor*(sphere.diffColor[loc][0])+ specularColor[0]+ returnShadeColor[0])+ sphere.transparency[loc]*returnRefractColor[0];
		  pixelColor[1] = pixelColor[1] + (1-sphere.transparency[loc])*(diffuseColor*(sphere.diffColor[loc][1])+ specularColor[1]+ returnShadeColor[1])+ sphere.transparency[loc]*returnRefractColor[1];
     	  pixelColor[2] = pixelColor[2] + (1-sphere.transparency[loc])*(diffuseColor*(sphere.diffColor[loc][2])+ specularColor[2]+ returnShadeColor[2])+ sphere.transparency[loc]*returnRefractColor[2];
	 } 
				
		  R = pixelColor[0];
		  G = pixelColor[1];
		  B = pixelColor[2];	  

		 } //if((min >= 0) && (min != 11111)) part ends

	 else // min = 11111 means background pixel
	 {
		 R = 0.15;
		 G = 0.15;
		 B = 0.15;
			 
	 }//if(minT > 0) loop ends	    

		pixelColorFinal[0] = R; 
		pixelColorFinal[1] = G;
		pixelColorFinal[2] = B;
       
return pixelColorFinal;
 }


