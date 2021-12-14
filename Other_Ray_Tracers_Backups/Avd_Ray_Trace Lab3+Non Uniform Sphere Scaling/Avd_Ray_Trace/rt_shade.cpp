//******************************************************************************** //
//                              Author: Soumya Dutta                               //
//                              CSE681 Introduction to Computer Graphics           //
//                              Lab Assignment 3 (Autumn 2011)                     //
//                              Date: November 18, 2011                            //  
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
#include "cube.h"
#include "time.h"

 using namespace std;

 void usage_error();
 SbVec3f Shade(int,SbVec3f,SbVec3f, my_sphere, my_light,int,int,int,int,int,int,my_cube);
 float intersect(SbVec3f,SbVec3f,float,SbVec3f); 
 float intersectCube(SbVec3f,SbVec3f,SbVec3f,SbVec3f);
 SbVec3f getNormalCube (SbVec3f,int,my_cube);
 /***********************************main function starts here****************************/
 int main(int argc, char **argv)
 {
 
 float minT=-1,k,aspectRatio,dist=10,imWidth,imHeight,noOfSphere=0,noOfCube=0;
 SbVec3f rayOrig,rayDirection, center, intersection_point,intersection_normal,centerImagePlane,ImPlaneCorner,pixelColorFinal,pixelColorFinalSuper(0,0,0),pixelColorFinalDOF(0,0,0);
 SbVec3f uEye,vEye,nEye,pixelEyeDist,pixelCenterLoc,pixelColor,pixelColorTemp(0,0,0),pixelColorFinalTemp(0,0,0),pixelColorFinalSuperSample(0,0,0);
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
 int focalLength = 12; // Change this to get variation in Depth of Field
 float lensParam = 3; // lens component increase this to get more blurring , for small values lensParam depth of field effect decreases

 
 if (argc != 9)
  {
    cout<<"usage: "<< argv[0] <<" <filename>\n";
    usage_error();
  }

  SoDB::init();
  OSUInventorScene *scene = new OSUInventorScene(argv[1]);
  int noOfObjects = scene->Objects.getLength();
  int noOfLights = scene->Lights.getLength();
  
  my_camera mc(scene);  
  my_light light(scene);
  my_sphere sphere(scene);
  my_cube cube(scene);
  noOfSphere = sphere.noOfSphere;
  noOfCube = cube.noOfCube;
  
  cout<<"noOfObjects: "<<noOfObjects<<endl;
  cout<<"noOfLights: "<<noOfLights<<endl;
  cout<<"noOfSphere: "<<sphere.noOfSphere<<endl;
  cout<<"noOfCube: "<<cube.noOfCube<<endl;

  for(int j=0;j<noOfSphere;j++)
	  cout<<"radius "<<j<<"th sphere: "<< sphere.radius[j]<<endl;

  for(int j=0;j<noOfCube;j++)
  {
	  cout<<"depth "<<j<<"th cube: "<< cube.depth[j]<<endl;
	  cout<<"height "<<j<<"th cube: "<< cube.height[j]<<endl;
	  cout<<"width "<< j<<"th cube: "<<cube.width[j]<<endl;
  }

 // Printing The Different Transformation Matrices 
  // for(int i=0;i<noOfSphere;i++)
  //{
	 // printf("Total Transformation matrix for sphere: %d\n",i);
	 //
	 // for(int j=0;j<4;j++)
	 // {
		//  for(int k=0;k<4;k++)
		//  {
		//	  printf("%f\t",sphere.F[i][j][k]);
		//  }
		//  printf("\n");
	 // }
  //}

  // for(int i=0;i<noOfSphere;i++)
  //{
	 // printf("Translation matrix for sphere: %d\n",i);
	 // for(int j=0;j<4;j++)
	 // {
		//  for(int k=0;k<4;k++)
		//  {
		//	  printf("%f\t",sphere.T[i][j][k]);
		//  }
		//  printf("\n");
	 // }
  //}

  //  for(int i=0;i<noOfSphere;i++)
  //{
	 // printf("Scaling matrix for sphere: %d\n",i);
	 // for(int j=0;j<4;j++)
	 // {
		//  for(int k=0;k<4;k++)
		//  {
		//	  printf("%f\t",sphere.S[i][j][k]);
		//  }
		//  printf("\n");
	 // }
  //}

	 //for(int i=0;i<noOfSphere;i++)
  //{
	 // printf("Rotation matrix for sphere: %d\n",i);
	 // for(int j=0;j<4;j++)
	 // {
		//  for(int k=0;k<4;k++)
		//  {
		//	  printf("%f\t",sphere.R[i][j][k]);
		//  }
		//  printf("\n");
	 // }
  //}


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
		SbVec3f pointAimed = rayOrig + mc.focalLength*pixelEyeDist;// focalLength is the focal distance.

		

		    /* Calculation for DOF when no super sampling required*/
		    if(requiredDof ==0) 
		        {
		          pixelColorFinal = Shade(noOfSphere,rayOrig,pixelEyeDist,sphere,light,noOfLights,1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
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

					  for(int di=0; di<10; di++)
					   {
						    float dk = rand()/float(RAND_MAX+1);
                            float dl = rand()/float(RAND_MAX+1);                            

							SbVec3f dofRayOrig = rayOrig  -(lensParam/2)*uDof - (lensParam/2)*vDof;
                            dofRayOrig = dofRayOrig + lensParam*(dk)*uDof + lensParam*(dl)*vDof;
							SbVec3f dofRayDir = pointAimed - dofRayOrig;
							dofRayDir.normalize();
							pixelColorFinal = Shade(noOfSphere,dofRayOrig,dofRayDir,sphere,light,noOfLights,1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
							pixelColorTemp += pixelColorFinal;
		                  
					  }
					  pixelColorFinal = pixelColorTemp/10;
			          pixelColorTemp.setValue(0,0,0);	
			          pixelColorFinal = 255*pixelColorFinal;

			    }
		}
		else if(requiredSupSample==1)
		{	
		  SbVec3f pointAimed = rayOrig + mc.focalLength*pixelEyeDist;
		   if(requiredDof == 0) 
		    {
			 for(int k =0; k<10; k++)
			 {
					float du = rand()/float(RAND_MAX+1);
					float dv = rand()/float(RAND_MAX+1);
					pixelCenterLoc = ImPlaneCorner + (imWidth/XRes)*(j+du)*(uEye) + (imHeight/YRes)*(i+dv)*(vEye);
					pixelEyeDist = pixelCenterLoc-rayOrig;//(s-e) vector
					//pixelEyeDist.normalize();
					pixelColorFinal = Shade(noOfSphere,rayOrig,pixelEyeDist,sphere,light,noOfLights,1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
					pixelColorFinalSup += pixelColorFinal;
				
			 }
			 pixelColorFinal = pixelColorFinalSup/10;			 
			 pixelColorFinalSup.setValue(0,0,0);
			 pixelColorFinal = 255*pixelColorFinal;
		   }

		   else // Implement DOF + Super Sample here
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

					for(int k =0; k<10; k++)// supersample loop
			          {

					  for(int di=0; di<10; di++)// DOF loop
					   {
			                float dk = rand()/float(RAND_MAX+1);
                            float dl = rand()/float(RAND_MAX+1);                            

							SbVec3f dofRayOrig = rayOrig  -(lensParam/2)*uDof - (lensParam/2)*vDof;
                            dofRayOrig = dofRayOrig + lensParam*(dk)*uDof + lensParam*(dl)*vDof;
							SbVec3f dofRayDir = pointAimed - dofRayOrig;
							dofRayDir.normalize();
							pixelColorFinal = Shade(noOfSphere,dofRayOrig,dofRayDir,sphere,light,noOfLights,1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
							pixelColorTemp += pixelColorFinal;
					  }
                      /*pixelColorTemp = pixelColorTemp/16;*/
					  pixelColorFinalSuperSample += pixelColorTemp/10;
					  pixelColorTemp.setValue(0,0,0);
			  	      
			   }
			   pixelColorFinal = pixelColorFinalSuperSample/10;
			   pixelColorFinal = 255*pixelColorFinal;
			   pixelColorFinalSuperSample.setValue(0,0,0);

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
	  //rayDirection.normalize(); //normalizing the vector this has to be commented if intersection is done in object space
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

/* intersectCube function definition */
 float intersectCube(SbVec3f rayOrig,SbVec3f rayDirection,SbVec3f pointFront,SbVec3f pointBack)
 {
	 float t1,t2,tnear = -1000,tfar = 1000,temp,returnvalue=0;
	 //rayDirection.normalize();

	 for(int i =0 ;i < 3; i++)
	 {
		 if(rayDirection[i] == 0)
		 {
            if(rayOrig[i] < pointFront[i] || rayOrig[i] > pointBack[i])
				returnvalue = -1;            
         }
		 else{
            t1 = (pointFront[i] - rayOrig[i])/rayDirection[i];
            t2 = (pointBack[i] - rayOrig[i])/rayDirection[i];
			if(t1 > t2)
				{
				temp = t1;
				t1 = t2;
				t2 = temp;
				}
			if(t1 > tnear)
				tnear = t1;
			if(t2 < tfar)
				tfar = t2;

			if(tnear > tfar)
               returnvalue = -1;
            if(tfar < 0)
               returnvalue = -1;           
			}	
	 }
	  if(returnvalue==-1)
		  return returnvalue;
      return tnear;  	  
 }

/* Definition of getNormalCube which returns the normal for each intersection point in cube */
 SbVec3f getNormalCube (SbVec3f point,int locCube,my_cube cube)
 {
	float EPS = 0.01;
	 
	if (fabs(point[0] - cube.pointFront[locCube][0]) <= EPS) return SbVec3f(-1,0,0);
	else if (fabs(point[0] - cube.pointBack[locCube][0]) <= EPS) return SbVec3f(1,0,0);
	else if (fabs(point[1] - cube.pointFront[locCube][1]) <= EPS) return SbVec3f(0,-1,0);
	else if (fabs(point[1] - cube.pointBack[locCube][1]) <= EPS) return SbVec3f(0,1,0);
	else if (fabs(point[2] - cube.pointFront[locCube][2]) <= EPS) return SbVec3f(0,0,-1);
	else if (fabs(point[2] - cube.pointBack[locCube][2]) <= EPS) return SbVec3f(0,0,1);

 }
/* Error function definition */ 
void usage_error()
 {
  cerr << "Usage: sample_read_iv {filename}" << endl;
 system ("pause");
  exit(10);
 }


/* Shade Function Definition */
SbVec3f Shade(int noOfSphere,SbVec3f rayOrig, SbVec3f pixelEyeDist, my_sphere sphere, my_light light, int noOfLights,int recursiondepth, int requiredReflection, int requiredShadow, int refractionRequired,int noOfCube,my_cube cube)
  {
	 float R=0.15,G=0.15,B=0.15,temp,checkshadow=-1;
	 float minT=0,diffuseColor=0,ambientColor=0,specularColortemp=0,*tempT,min=1,temp1=0,diffuseColorSub=0,*tempCubeT,minSphere=11111,minCube=11111;
	 tempT = (float *)malloc(noOfSphere*sizeof(float));
	 tempCubeT = (float *)malloc(noOfCube*sizeof(float));
	 SbVec3f intersection_point,intersection_normal,pixelColor,pixelColorFinal,refractionRay(0,0,0),pixelEyeDistNormalized,TransmittedRay,specularColorSub(0,0,0);
	 SbVec3f shadowRay,viewRay,intensity(1,1,1),specularColor(0,0,0),shadowRayOrig,shadowRaydir,reflectionRay,returnShadeColor(0,0,0),returnRefractColor(0,0,0); 
	 int loc=0,intersection1=0,intersection2=0,intersectLight=0,locCube=0;
	 int MAXRECURSION=3,cubeOrSphere=0;
	 float shine = 0.9,trans=1, Ni = 0.67,epsilon = 0.00009,epsilon1= -0.000009,epsilonReflectionSphere= 0.00009,temp2=0;// epsilon1 is for refraction of cubes
	 pixelEyeDistNormalized = pixelEyeDist;
	 pixelEyeDistNormalized.normalize();
	 float diffuselight=0,DiamLight=2;
	 float epsilonRefractSphere = 0.001;
	 float epsilonReflectionCube = -0.00009;
	 float epsilonShadow = 0.0007,epsi=0.000007;
	 SbVec3f specularColorlight(0,0,0);
	 int numShadowRays = 100;// Number of soft shadow ray sent from one pixel to simulate area light
	 SbVec3f rayO(0,0,0),Raydist(0,0,0),interSectionPointLocal(0,0,0),intersectionNormalLocal(0,0,0);

	 if(noOfSphere>0)
	   {
		   	  //sphere wise loop
			   for(int q=0;q<noOfSphere;q++)
			   {
				  if(sphere.S[q][0][0] == sphere.S[q][1][1] == sphere.S[q][2][2])
				  {
					  /* Calling the Intersect Function for Intersection Calculation */ 
				   tempT[q] = intersect(rayOrig,pixelEyeDistNormalized,sphere.radius[q],sphere.centerSphere[q]); // sending normalized ray direction
				   if (tempT[q] == -1.0)// means no intersection
					   tempT[q]=11111; // set tempT[q] = 11111
				  }
				  else
				  {

				  sphere.Finv[q].multVecMatrix(rayOrig, rayO); // Transforms ray origin from world space to object space
		          sphere.Finv[q].multDirMatrix(pixelEyeDist,Raydist); // Transforms ray direction from world space to object space, do not normalize ray direction

				/* Calling the Intersect Function for Intersection Calculation */ 
				   tempT[q] = intersect(rayO,Raydist,sphere.radius[q],sphere.centerSphere[q]);
				   if (tempT[q] == -1.0)// means no intersection
					   tempT[q]=11111; // set tempT[q] = 11111
				  }
			    }

			   minSphere = tempT[0]; // min corresponds to the closest sphere for a single pixel, so the pixel will have the color of this sphere
			   loc =0; // loc corresponds the number of sphere which is the closest one
			   for(int q=0;q<noOfSphere;q++)
			   {
				  if(minSphere>tempT[q])
				  {	  minSphere = tempT[q];
					  loc = q;
				  }
			   }
	   }


	   if(noOfCube > 0) 
	   {
			   //cube wise loop
			   for(int q=0;q<noOfCube;q++)
			   {
				/* Calling the intersectCube Function for Intersection Calculation */ 
				   tempCubeT[q] = intersectCube(rayOrig,pixelEyeDist,cube.pointFront[q],cube.pointBack[q]);
				   if (tempCubeT[q] == -1.0)// means no intersection
				   {
				   tempCubeT[q]=11111;
				   } // set tempCubeT[q] = 11111
			   }
			   
			   minCube = tempCubeT[0]; // minCube corresponds to the closest cube for a single pixel, so the pixel will have the color of this cube
			   locCube =0; // locCube corresponds the number of cube which is the closest one
			   for(int q=0;q<noOfCube;q++)
			   {
				  if(minCube>tempCubeT[q])
				  {	  minCube = tempCubeT[q];
					  locCube = q;
				  }
			   }
	   }	  

   if (minCube<minSphere)
   {
	   min = minCube;
       cubeOrSphere = 1; // cube is nearer
   }
   else
   {
	  min = minSphere;
      cubeOrSphere = 0;
   }
  
	if((min >= 0) && (min != 11111) && (cubeOrSphere == 0) )// calculate for sphere, min = 11111 means background pixel
	{
		  diffuseColor =0; 
		  diffuseColorSub = 0;
		  specularColorlight.setValue(0,0,0);
		  diffuselight=0;
		  specularColorSub.setValue(0,0,0);
		  temp = 0;
		  specularColortemp =0;

		   if(sphere.S[loc][0][0] == sphere.S[loc][1][1] == sphere.S[loc][2][2])
			{

          /* World Space Calculation */		  
		  intersection_point = rayOrig + (pixelEyeDist)*min;
		  /* Formation of shadow ray origin which is same as intersection point */
		  intersection_normal = (intersection_point-sphere.centerSphere[loc]);
          intersection_normal.normalize();	
		  /* World Space Calculation Ends*/
		   }

		   else // sphere has non uniform scaling
		   {
          /*For Instancing Forming new ray orig and direction*/
		  interSectionPointLocal = rayO + (Raydist)*min; // in object space 	  		  
		  intersectionNormalLocal = (interSectionPointLocal-sphere.centerSphere[loc]); // in object space	
		  intersectionNormalLocal.normalize();
		  sphere.F[loc].multVecMatrix(interSectionPointLocal, intersection_point);
		  sphere.FinvT[loc].multDirMatrix(intersectionNormalLocal,intersection_normal);		
		   }
		  /*For Instancing Forming new ray orig and direction ends*/

		  shadowRayOrig = intersection_point;
		  
		 		 
          // Ambient Color is constant as we have assumed and it is Ka*Ia
		  pixelColor[0] = (1-sphere.transparency[loc])*(sphere.ambientColor[loc][0]*sphere.diffColor[loc][0]);
		  pixelColor[1] = (1-sphere.transparency[loc])*(sphere.ambientColor[loc][1]*sphere.diffColor[loc][1]);
		  pixelColor[2] = (1-sphere.transparency[loc])*(sphere.ambientColor[loc][2]*sphere.diffColor[loc][2]);
		 

  // Starting calculation of diffuse and specular color for all the lights
  /********************light loop********************/
         
 if(requiredShadow == 1) 
 {
		for( int r =0; r<noOfLights; r++)
		{
           shadowRaydir = light.location[r] - shadowRayOrig;// direction of shadow ray
		   light.location[r] = light.location[r] - shadowRayOrig;
		   light.location[r].normalize();// now light.location[r] is the light vector		   
		   shadowRaydir.normalize();
		   temp = intersection_normal.dot(light.location[r]);

		   if (temp > 0) // the intersection point facing light source
		   {
			 
 	   		//Sphere wise checking the intersection with shadow ray
            for(int q=0;q<noOfSphere;q++)
			{								 
				if(sphere.transparency[q] <=0)
				   {
				/* Calling the Intersect Function for Intersection calculation */ 
				checkshadow = intersect((shadowRayOrig + epsilonShadow*shadowRaydir),shadowRaydir,sphere.radius[q],sphere.centerSphere[q]);
				
				if (checkshadow == -1.0)
				  {                      
				   intersection1 = 0; // no intersection
				  }	
				
				else {
					if(sphere.transparency[loc]>0)
					{
						intersection1 = 0;
					}
					else
					{
					intersection1 =1;
					break;
					}
					 }
				}
			}		   
			   
           
			//Cube wise checking the intersection with shadow ray
				for(int q=0;q<noOfCube;q++)
				  {
					 if(cube.transparency[q] <=0)
				   {
					 
					/* Calling the Intersect Function for Intersection calculation */ 
					checkshadow = intersectCube((shadowRayOrig - epsilonShadow*shadowRaydir),shadowRaydir,cube.pointFront[q],cube.pointBack[q]);
					if (checkshadow == -1.0 )
					  {
					  intersection2 = 0; // no intersection
					  }					
					else {
						 intersection2 =1;
						 break; 
						 }
					 }
				  }
		   
		  
	   	if(intersection1 == 0 && intersection2 ==0 ) // no intersection with cube and sphere
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

		  } // if (temp >=0) loop ends		     
		  
		if(recursiondepth < MAXRECURSION) //raflection should still happen even the point is in shadow.. 
		 {    
			if(requiredReflection == 1) // Add reflection only if required. Determined through runtime binary parameter requiredReflection (1,0)
		        {
		        //Here Formulate Reflection Ray and Call Shade again (Recursive Call)
		         temp1 = pixelEyeDist.dot(intersection_normal);
                 reflectionRay = pixelEyeDist - 2*temp1*(intersection_normal);
				 //reflectionRay.normalize();
		         returnShadeColor = returnShadeColor + Shade(noOfSphere,(intersection_point + reflectionRay*epsilonReflectionSphere),reflectionRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
				 returnShadeColor = sphere.shininess[loc]*returnShadeColor;	         
		  		
				}

				if(refractionRequired ==1)
				{
				 if(sphere.transparency[loc]>0)
				 {
					 
					temp2 = pixelEyeDistNormalized.dot(intersection_normal); 
					
					//Here Formulate Refraction Ray and Call Shade again (Recursive Call)
			    	if(temp2 <= 0) //(IncomingRay.Normal < 0) Ray is coming from air to object 
					{
						
						float rootContent = (1 - Ni * Ni * (1 - pow((intersection_normal.dot(-pixelEyeDistNormalized)),2)));
	                    rootContent = sqrtf(rootContent);
		                TransmittedRay = (Ni * (intersection_normal.dot(-pixelEyeDistNormalized)) - rootContent) * intersection_normal - ( Ni * -pixelEyeDistNormalized );
	                    TransmittedRay.normalize();
						returnRefractColor = returnRefractColor + Shade(noOfSphere,(intersection_point + (pixelEyeDist*epsilonRefractSphere)),TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
						
				    }
					else
					{
						Ni = 1/Ni;
						float rootContent = (1 - Ni * Ni * (1 - pow((intersection_normal.dot(-pixelEyeDistNormalized)),2)));
		                if(rootContent>0.0)
	                      {
		                    rootContent = sqrtf(rootContent);
		                    TransmittedRay = (Ni * (-intersection_normal.dot(-pixelEyeDistNormalized)) - rootContent)*(-intersection_normal) - ( Ni * -pixelEyeDistNormalized );
	                      	TransmittedRay.normalize();
							returnRefractColor = returnRefractColor + Shade(noOfSphere,(intersection_point + (pixelEyeDist*epsilonRefractSphere)),TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
						 }  
						else
						{
							temp1 = pixelEyeDist.dot(intersection_normal);
                            reflectionRay = pixelEyeDist - 2*temp1*(intersection_normal);
				            reflectionRay.normalize();
		                    returnRefractColor = returnRefractColor + Shade(noOfSphere,(intersection_point + reflectionRay*epsilonRefractSphere),reflectionRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
				            	
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

	      if (temp > 0) // the intersection point facing light source then it get color no shadow chek is happening here
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
		         returnShadeColor = returnShadeColor + Shade(noOfSphere,(intersection_point + pixelEyeDist*epsilonReflectionSphere),reflectionRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
				 returnShadeColor = sphere.shininess[loc]*returnShadeColor;
		         	 
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
	                    returnRefractColor = returnRefractColor + Shade(noOfSphere,(intersection_point + (TransmittedRay*epsilonRefractSphere)),TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
						
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
	                      	returnRefractColor = returnRefractColor + Shade(noOfSphere,(intersection_point + (TransmittedRay*epsilonRefractSphere)),TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
						 }  
						else
						{
							temp1 = pixelEyeDist.dot(intersection_normal);
                            reflectionRay = pixelEyeDist - 2*temp1*(intersection_normal);
				            reflectionRay.normalize();
		                    returnRefractColor = returnRefractColor + Shade(noOfSphere,(intersection_point + reflectionRay*epsilonRefractSphere),reflectionRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
				            	
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
			                
			  for(int k=0;k<numShadowRays;k++)
			  {
				    float dx = rand()/float(RAND_MAX+1);
					float dy = rand()/float(RAND_MAX+1);

					SbVec3f subPixelCenter = lightCenter  -(DiamLight/2)*uSoft - (DiamLight/2)*vSoft + DiamLight*(dx)*uSoft + DiamLight*(dy)*vSoft;// radius = 3                    
                    SbVec3f softShadowRayDir = subPixelCenter - shadowRayOrig;	
					shadowRayOrig = shadowRayOrig + epsi*softShadowRayDir;
					softShadowRayDir.normalize();

					if((intersection_normal.dot(softShadowRayDir))>=0)
					{
							     		   
                      //Sphere wise checking the intersection with shadow ray
                      for(int q=0;q<noOfSphere;q++)
                       {
				          if(sphere.transparency[q]<=0)
				           {
			                  /* Calling the Intersect Function for Intersection calculation */ 
	                           checkshadow = intersect((shadowRayOrig + epsilonShadow*softShadowRayDir),softShadowRayDir,sphere.radius[q],sphere.centerSphere[q]);
				                   if (checkshadow == -1.0) // Ray does not intersect with any of the objects
				                     {
										intersection1 = 0;
								      }
								   else 
				                     {   
					                     intersection1 = 1;
				                         break;
									 }
     		               }
					    }

				  //Cube wise checking the intersection with shadow ray
					for(int q=0;q<noOfCube;q++)
					  {
						 if(cube.transparency[q]<= 0)
						 {
						/* Calling the Intersect Function for Intersection calculation */ 
						checkshadow = intersectCube((shadowRayOrig - epsilonShadow*softShadowRayDir),softShadowRayDir,cube.pointFront[q],cube.pointBack[q]);
						if (checkshadow == -1.0 )
						  {
						  intersection2 = 0; // no intersection
						  }
						
						else {
							 intersection2 =1;
							  //break; 
							 }
						  }
					}
               
					   if(intersection1 == 0 && intersection2 ==0)
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
   
					} 
			  }// end internal for loop			
					

		   } // end of N.L check		   

		   diffuselight = diffuselight + diffuseColor/numShadowRays;
		   specularColorlight = specularColorlight+ specularColor/numShadowRays;
		   
	  	  
	if(recursiondepth < MAXRECURSION) //raflection should still happen even the point is in shadow.. 
		 {    
				if(requiredReflection == 1) // Add reflection only if required. Determined through runtime binary parameter requiredReflection (1,0)
		        {
		        //Here Formulate Reflection Ray and Call Shade again (Recursive Call)
		         temp1 = pixelEyeDist.dot(intersection_normal);
                 reflectionRay = pixelEyeDist - 2*temp1*(intersection_normal);
		         returnShadeColor = returnShadeColor + Shade(noOfSphere,(intersection_point + pixelEyeDist*epsilonReflectionSphere),reflectionRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
		         returnShadeColor = sphere.shininess[loc]*returnShadeColor;
		         
		  		
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
	                    returnRefractColor = returnRefractColor + Shade(noOfSphere,(intersection_point + (pixelEyeDist*epsilonRefractSphere)),TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
						
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
	                      	returnRefractColor = returnRefractColor + Shade(noOfSphere,(intersection_point + (pixelEyeDist*epsilonRefractSphere)),TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
						 } 
						else
						{
							temp1 = pixelEyeDist.dot(intersection_normal);
                            reflectionRay = pixelEyeDist - 2*temp1*(intersection_normal);
				            reflectionRay.normalize();
		                    returnRefractColor = returnRefractColor + Shade(noOfSphere,(intersection_point + reflectionRay*epsilonRefractSphere),reflectionRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
				            	
						}
					}
					
				 } //if(sphere.transparency[loc]>0) Loop ends

				}//if(refractionRequired ==1) Ends

	  }// recursion depth loop ends	

	  }// light loop ends for with shadow part

     diffuseColor = diffuselight;
	 specularColor = specularColorlight;

	     
	} // if(requiredShadow == 2) part ends.. to confirm that soft shadow is only calculated when required, determined according to the runtime argument

  		  pixelColor[0] = pixelColor[0] + (1-sphere.transparency[loc])*(diffuseColor*(sphere.diffColor[loc][0]))+ specularColor[0]+ (1-sphere.transparency[loc])*returnShadeColor[0]+ sphere.transparency[loc]*returnRefractColor[0];
		  pixelColor[1] = pixelColor[1] + (1-sphere.transparency[loc])*(diffuseColor*(sphere.diffColor[loc][1]))+ specularColor[1]+ (1-sphere.transparency[loc])*returnShadeColor[1]+ sphere.transparency[loc]*returnRefractColor[1];
     	  pixelColor[2] = pixelColor[2] + (1-sphere.transparency[loc])*(diffuseColor*(sphere.diffColor[loc][2]))+ specularColor[2]+ (1-sphere.transparency[loc])*returnShadeColor[2]+ sphere.transparency[loc]*returnRefractColor[2];
	 
				
		  R = pixelColor[0];
		  G = pixelColor[1];
		  B = pixelColor[2];

		 

 } //if((min >= 0) && (min != 11111)) sphere calculation part ends


 /********************************************** This part is for cube calculation *****************************/
 /********************************************** This part is for cube calculation *****************************/
 else if((min >= 0) && (min != 11111) && (cubeOrSphere ==1) )// calculate for cube, intersection point is on cube
	{
		  diffuseColor =0; 
		  diffuseColorSub = 0;
		  specularColorlight.setValue(0,0,0);
		  diffuselight=0;
		  specularColorSub.setValue(0,0,0);
		  temp = 0;
		  specularColortemp =0;
		  intersection_point = rayOrig + (pixelEyeDist)*min;		  
		  intersection_normal = getNormalCube(intersection_point,locCube,cube);	 

		  shadowRayOrig = intersection_point;
		 		 
          //Ambient Color is constant as we have assumed and it is Ka*Ia
		  pixelColor[0] = (cube.ambientColor[locCube][0]*cube.diffColor[locCube][0]);
		  pixelColor[1] = (cube.ambientColor[locCube][1]*cube.diffColor[locCube][1]);
		  pixelColor[2] = (cube.ambientColor[locCube][2]*cube.diffColor[locCube][2]);

		 

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
			   
            //Sphere wise checking the intersection with shadow ray
            for(int q=0;q<noOfSphere;q++)
              {	
			  			 
			    /* Calling the Intersect Function for Intersection calculation */ 
	            checkshadow = intersect((shadowRayOrig + epsilonShadow*shadowRaydir),shadowRaydir,sphere.radius[q],sphere.centerSphere[q]);
				if (checkshadow == -1.0 )
				  {
				  intersection1 = 0; // no intersection
				  }				
				else 
				    {
					 intersection1 =1;
				     break; 
				    }	
			
			}

			//Cube wise checking the intersection with shadow ray
			for(int q=0;q<noOfCube;q++)
              {
				 
			    /* Calling the Intersect Function for Intersection calculation */ 
	            checkshadow = intersectCube((shadowRayOrig + epsilonShadow*shadowRaydir),shadowRaydir,cube.pointFront[q],cube.pointBack[q]);
				if (checkshadow == -1.0 )
				  {
				     intersection2 = 0; // no intersection
				  }				
				else {
					 intersection2 =1;
				      break; 
				     }			   
			}
			
								
	   	if((intersection1 == 0) && (intersection2 ==0)) // no intersection with sphere and cube
		 {		
	    //   // diffuse component calculation	   
		   diffuseColor = diffuseColor + max<float>((intersection_normal.dot(light.location[r])),0); //diffuse component = (N.L)*Kd*Id, diffuseColor = (N.L)
		   //
		   //specular component	calculation	   
		   shadowRay = 2*intersection_normal*temp - light.location[r];
		   shadowRay.normalize();
		   viewRay = rayOrig - intersection_point;
		   viewRay.normalize(); 
		   specularColortemp = viewRay.dot(shadowRay);
           specularColortemp = pow(specularColortemp,50);
		   specularColor[0] = specularColor[0] + specularColortemp*(intensity[0])*(cube.specularColor[locCube][0]);
	       specularColor[1] = specularColor[1] + specularColortemp*(intensity[1])*(cube.specularColor[locCube][1]);
		   specularColor[2] = specularColor[2] + specularColortemp*(intensity[2])*(cube.specularColor[locCube][2]);
				
		   } // if(intersection == 0) loop Ends

		   } // if (temp >0) loop ends		     
		  
		if(recursiondepth < MAXRECURSION) //raflection should still happen even the point is in shadow.. 
		 {    
			if(requiredReflection == 1) // Add reflection only if required. Determined through runtime binary parameter requiredReflection (1,0)
		        {
		        //Here Formulate Reflection Ray and Call Shade again (Recursive Call)
		         temp1 = pixelEyeDist.dot(intersection_normal);
                 reflectionRay = pixelEyeDist - 2*temp1*(intersection_normal);
				 reflectionRay.normalize();
		         returnShadeColor = returnShadeColor + shine*Shade(noOfSphere,(intersection_point + pixelEyeDist*epsilonReflectionCube),reflectionRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
				 	
				}

				if(refractionRequired ==1)
				{
				 if(cube.transparency[locCube]>0)
				 {
					 
					 temp2 = pixelEyeDistNormalized.dot(intersection_normal); 
					
					//Here Formulate Refraction Ray and Call Shade again (Recursive Call)
			    	if(temp2 < 0) //(IncomingRay.Normal < 0) Ray is coming from air to object 
					{
						
						float rootContent = (1 - Ni * Ni * (1 - (intersection_normal.dot(-pixelEyeDistNormalized) * (intersection_normal.dot(-pixelEyeDistNormalized)))));
	                    rootContent = sqrtf(rootContent);
		                TransmittedRay = (Ni * (intersection_normal.dot(-pixelEyeDistNormalized)) - rootContent) * intersection_normal - ( Ni * -pixelEyeDistNormalized );
	                    returnRefractColor = returnRefractColor + Shade(noOfSphere,(intersection_point + (pixelEyeDist*(epsilon1))),TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
						
				    }
					else
					{
						Ni = 1/Ni;
						float rootContent = (1 - Ni * Ni * (1 - (-intersection_normal.dot(-pixelEyeDistNormalized) * (-intersection_normal.dot(-pixelEyeDistNormalized)))));
		                if(rootContent>=0.0)
	                      {
		                     rootContent = sqrtf(rootContent);
		                     TransmittedRay = (Ni * (-intersection_normal.dot(-pixelEyeDistNormalized)) - rootContent)*(-intersection_normal) - ( Ni * -pixelEyeDistNormalized );
	                      	returnRefractColor = returnRefractColor + Shade(noOfSphere,(intersection_point + (pixelEyeDist*(epsilon1))),TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
						 }  
						else
						{
							temp1 = pixelEyeDist.dot(intersection_normal);
                            reflectionRay = pixelEyeDist - 2*temp1*(intersection_normal);
				            reflectionRay.normalize();
		                    returnRefractColor = returnRefractColor + cube.shininess[locCube]*Shade(noOfSphere,(intersection_point + reflectionRay*epsilon1),reflectionRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
				            	
						}
					}
					
				 } //if(sphere.transparency[loc]>0) Loop ends

				}//if(refractionRequired ==1) Ends

	  }// recursion depth loop ends	

	}// light loop ends for with hard shadow part		

  } // if(requiredShadow == 1) part ends.. to confirm that shadow is only calculated when required , determined according to the runtime argument

 else if(requiredShadow == 0) // else shadow is not required, else part of if(requiredShadow == 1)
	{
		for( int r =0; r<noOfLights; r++)
		  {   
           shadowRaydir = light.location[r] - shadowRayOrig;// direction of shadow ray
		   light.location[r] = light.location[r] - shadowRayOrig;
		   light.location[r].normalize();// now light.location[r] is the light vector
		   temp = intersection_normal.dot(light.location[r]);

	      if (temp > 0) // the intersection point facing light source then it get color no shadow chek is happening here
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
		   specularColor[0] = specularColor[0] + specularColortemp*(intensity[0])*(cube.specularColor[locCube][0]);
	       specularColor[1] = specularColor[1] + specularColortemp*(intensity[1])*(cube.specularColor[locCube][1]);
		   specularColor[2] = specularColor[2] + specularColortemp*(intensity[2])*(cube.specularColor[locCube][2]);			  
		
	        }
	  if(recursiondepth < MAXRECURSION) // raflection should still happen even the point is in shadow.. 
		      {    
				  if(requiredReflection == 1) // Add reflection only if required. Determined through runtime binary parameter requiredReflection (1,0)
		          {
		        //Here Formulate Reflection Ray and Call Shade again (Recursive Call)
		         temp1 = pixelEyeDist.dot(intersection_normal);
                 reflectionRay = pixelEyeDist - 2*temp1*(intersection_normal);
				 reflectionRay.normalize();
		         returnShadeColor = returnShadeColor + shine*Shade(noOfSphere,(intersection_point + pixelEyeDist*epsilonReflectionCube),reflectionRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
		         }

				if(refractionRequired ==1)
				{
				 if(cube.transparency[locCube]>0)
				 {
					 temp2 = pixelEyeDistNormalized.dot(intersection_normal); 
					
					//Here Formulate Refraction Ray and Call Shade again (Recursive Call)
			    	if(temp2 < 0) //(ViewRay.Normal < 0) Ray is coming from air to object 
					{
						//refractionRay = refract(pixelEyeDist,intersection_normal,Ni,temp2); // refract returns the refraction ray direction 
						float rootContent = (1 - Ni * Ni * (1 - (intersection_normal.dot(-pixelEyeDistNormalized) * (intersection_normal.dot(-pixelEyeDistNormalized)))));
	                    rootContent = sqrtf(rootContent);
		                TransmittedRay = (Ni * (intersection_normal.dot(-pixelEyeDistNormalized)) - rootContent) * intersection_normal - ( Ni * -pixelEyeDistNormalized );
	                    returnRefractColor = returnRefractColor + Shade(noOfSphere,(intersection_point + (pixelEyeDist*epsilon1)),TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
						
				    }
					else
					{
						Ni = 1/Ni;
						//refractionRay = refract(pixelEyeDist,intersection_normal,1/Ni,temp2); // refract returns the refraction ray direction 
						float rootContent = (1 - Ni * Ni * (1 - (-intersection_normal.dot(-pixelEyeDistNormalized) * (-intersection_normal.dot(-pixelEyeDistNormalized)))));
		                if(rootContent>=0.0)
	                      {
		                     rootContent = sqrtf(rootContent);
		                     TransmittedRay = (Ni * (-intersection_normal.dot(-pixelEyeDistNormalized)) - rootContent)*(-intersection_normal) - ( Ni * -pixelEyeDistNormalized );
	                      	returnRefractColor = returnRefractColor + Shade(noOfSphere,(intersection_point + (pixelEyeDist*epsilon1)),TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
						 } 
						else
						{
							temp1 = pixelEyeDist.dot(intersection_normal);
                            reflectionRay = pixelEyeDist - 2*temp1*(intersection_normal);
				            reflectionRay.normalize();
		                    returnRefractColor = returnRefractColor + cube.shininess[locCube]*Shade(noOfSphere,(intersection_point + reflectionRay*epsilon1),reflectionRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
				            	
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
			                
			  for(int k=0;k<numShadowRays;k++)
			  {
				    float dx = rand()/float(RAND_MAX+1);
					 float dy = rand()/float(RAND_MAX+1);

					SbVec3f subPixelCenter = lightCenter  -(DiamLight/2)*uSoft - (DiamLight/2)*vSoft + DiamLight*(dx)*uSoft + DiamLight*(dy)*vSoft;// radius = 3                    
                    SbVec3f softShadowRayDir = subPixelCenter - shadowRayOrig;	
					shadowRayOrig = shadowRayOrig + epsi*softShadowRayDir;
					softShadowRayDir.normalize();

					if((intersection_normal.dot(softShadowRayDir))>=0)
					{
							     		   
                      //sphere wise checking the intersection with shadow ray
                      for(int q=0;q<noOfSphere;q++)
                       {	
						   	          
			                  /* Calling the Intersect Function for Intersection calculation */ 
	                           checkshadow = intersect((shadowRayOrig + epsilonShadow*softShadowRayDir),softShadowRayDir,sphere.radius[q],sphere.centerSphere[q]);
				                   if (checkshadow == -1.0) // Ray does not intersect with any of the objects
				                     {
										intersection1 = 0;
								      }
								   else 
				                     {   
					                     intersection1 = 1;
				                         break;
									 }
						  
						  
					    }

					  //Cube wise checking the intersection with shadow ray
						for(int q=0;q<noOfCube;q++)
						  {
							
							/* Calling the Intersect Function for Intersection calculation */ 
							checkshadow = intersectCube((shadowRayOrig + epsilonShadow*softShadowRayDir),softShadowRayDir,cube.pointFront[q],cube.pointBack[q]);
							if (checkshadow == -1.0 )							  
							    intersection2 = 0; // no intersection
							else {
								 intersection2 =1;
								  break; 
								 }
							  
							}
               
					   if(intersection1 == 0 && intersection2 ==0 )
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
						   specularColor[0] = specularColor[0] + specularColortemp*(intensity[0])*(cube.specularColor[locCube][0]);
						   specularColor[1] = specularColor[1] + specularColortemp*(intensity[1])*(cube.specularColor[locCube][1]);
						   specularColor[2] = specularColor[2] + specularColortemp*(intensity[2])*(cube.specularColor[locCube][2]);	
						      
						} // if(intersection == 0) loop Ends 
   
					} 
				  }// end internal for loop
					

		   } // end of N.L check		   

		   diffuselight = diffuselight + diffuseColor/numShadowRays;
		   specularColorlight = specularColorlight+ specularColor/numShadowRays;
		   
	  	  
	if(recursiondepth < MAXRECURSION) //raflection should still happen even the point is in shadow.. 
		 {    
				if(requiredReflection == 1) // Add reflection only if required. Determined through runtime binary parameter requiredReflection (1,0)
		        {
		        //Here Formulate Reflection Ray and Call Shade again (Recursive Call)
		         temp1 = pixelEyeDist.dot(intersection_normal);
                 reflectionRay = pixelEyeDist - 2*temp1*(intersection_normal);
				 reflectionRay.normalize();
		         returnShadeColor = returnShadeColor + shine*Shade(noOfSphere,(intersection_point + pixelEyeDist*epsilonReflectionCube),reflectionRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
		                  
		  		
				}

				if(refractionRequired ==1)
				{
				 if(cube.transparency[locCube]>0)
				 {
					 
					 temp2 = pixelEyeDistNormalized.dot(intersection_normal); 
					
					//Here Formulate Refraction Ray and Call Shade again (Recursive Call)
			    	if(temp2 < 0) //(ViewRay.Normal < 0) Ray is coming from air to object 
					{
						
						//refractionRay = refract(pixelEyeDist,intersection_normal,Ni,temp2); // refract returns the refraction ray direction 
						float rootContent = (1 - Ni * Ni * (1 - (intersection_normal.dot(-pixelEyeDistNormalized) * (intersection_normal.dot(-pixelEyeDistNormalized)))));
	                    rootContent = sqrtf(rootContent);
		                TransmittedRay = (Ni * (intersection_normal.dot(-pixelEyeDistNormalized)) - rootContent) * intersection_normal - ( Ni * -pixelEyeDistNormalized );
	                    returnRefractColor = returnRefractColor + Shade(noOfSphere,(intersection_point + (pixelEyeDist*epsilon1)),TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
						
				    }
					else
					{
						Ni = 1/Ni;
						//refractionRay = refract(pixelEyeDist,intersection_normal,1/Ni,temp2); // refract returns the refraction ray direction 
						float rootContent = (1 - Ni * Ni * (1 - (-intersection_normal.dot(-pixelEyeDistNormalized) * (-intersection_normal.dot(-pixelEyeDistNormalized)))));
		                if(rootContent>=0.0)
	                      {
		                     rootContent = sqrtf(rootContent);
		                     TransmittedRay = (Ni * (-intersection_normal.dot(-pixelEyeDistNormalized)) - rootContent)*(-intersection_normal) - ( Ni * -pixelEyeDistNormalized );
	                      	returnRefractColor = returnRefractColor + Shade(noOfSphere,(intersection_point + (pixelEyeDist*epsilon1)),TransmittedRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
						 } 
						else
						{
							temp1 = pixelEyeDist.dot(intersection_normal);
                            reflectionRay = pixelEyeDist - 2*temp1*(intersection_normal);
				            reflectionRay.normalize();
		                    returnRefractColor = returnRefractColor + shine*Shade(noOfSphere,(intersection_point + reflectionRay*epsilon1),reflectionRay,sphere,light,noOfLights,recursiondepth+1,requiredReflection,requiredShadow,refractionRequired,noOfCube,cube); 
				            	
						}
					}
					
				 } //if(sphere.transparency[loc]>0) Loop ends

				}//if(refractionRequired ==1) Ends

	  }// recursion depth loop ends	

	  }// light loop ends for with shadow part 

	  diffuseColor = diffuselight;
	  specularColor = specularColorlight;
	     
	} // if(requiredShadow == 2) part ends.. to confirm that soft shadow is only calculated when required, determined according to the runtime argument

	if(noOfSphere>0)
	{
	pixelColor[0] = pixelColor[0] + (1-sphere.transparency[loc])*diffuseColor*(cube.diffColor[locCube][0])+ specularColor[0]+ (1-sphere.transparency[loc])*returnShadeColor[0]+ returnRefractColor[0];
	pixelColor[1] = pixelColor[1] + (1-sphere.transparency[loc])*diffuseColor*(cube.diffColor[locCube][1])+ specularColor[1]+ (1-sphere.transparency[loc])*returnShadeColor[1]+ returnRefractColor[1];
    pixelColor[2] = pixelColor[2] + (1-sphere.transparency[loc])*diffuseColor*(cube.diffColor[locCube][2])+ specularColor[2]+ (1-sphere.transparency[loc])*returnShadeColor[2]+ returnRefractColor[2];
	}
	else
	{
	pixelColor[0] = pixelColor[0] + diffuseColor*(cube.diffColor[locCube][0])+ specularColor[0]+ returnShadeColor[0]+ returnRefractColor[0];
	pixelColor[1] = pixelColor[1] + diffuseColor*(cube.diffColor[locCube][1])+ specularColor[1]+ returnShadeColor[1]+ returnRefractColor[1];
    pixelColor[2] = pixelColor[2] + diffuseColor*(cube.diffColor[locCube][2])+ specularColor[2]+ returnShadeColor[2]+ returnRefractColor[2];
	}
				
		  R = pixelColor[0];
		  G = pixelColor[1];
		  B = pixelColor[2];	
	}

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


 

