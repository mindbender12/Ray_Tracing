//******************************header file includes****************************** //
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

 using namespace std;

 void usage_error();
 float intersect(SbVec3f,SbVec3f,float,SbVec3f);

 /***********************************main function starts here****************************/
 int main(int argc, char **argv)
{
 
 float minT=-1,k,aspectRatio,dist,imWidth,imHeight;
 SbVec3f rayOrig,rayDirection, center, intersection_point,intersection_normal,centerImagePlane,ImPlaneCorner;
 SbVec3f uEye,vEye,nEye,pixelEyeDist,pixelCenterLoc,illumination(0,0,1),pixelColor;
 int XRes,YRes;
 FILE *fp;
 fp = fopen("output.ppm","w");
 XRes = atoi(argv[2]);
 YRes = atoi(argv[3]);
 aspectRatio = XRes/YRes;

 if (argc != 4)
  {
    cout<<"usage: "<< argv[0] <<" <filename>\n";
    usage_error();
  }

  SoDB::init();
  OSUInventorScene *scene = new OSUInventorScene(argv[1]);
  my_camera mc(scene);  
  my_light light(scene);
  my_sphere sphere(scene);

  //creating output ppm file and writing header of the file
  fprintf(fp,"P3 %d %d %d\n",XRes,YRes,255);

  for (int i = 0; i < scene->Objects.getLength(); i++)
  {
    OSUObjectData * obj = (OSUObjectData *)scene->Objects[i];
    cout<<"center is: "<<i<<" th sphere "<<sphere.centerSphere[i][0]<<sphere.centerSphere[i][1]<<sphere.centerSphere[i][2]<<endl;
  }
 
  rayOrig = mc.camera_position;
  cout<<"rayOrig: "<<rayOrig[0]<<rayOrig[1]<<rayOrig[2]<<endl;
  cout<<"mc.camera_direction: "<<mc.camera_direction[0]<<"|"<<mc.camera_direction[1]<<"|"<<mc.camera_direction[2]<<endl;
  
  //Calculating the eye plane basis vectors
  nEye = mc.camera_direction*(-1);
  nEye.normalize();//normalizing nEye vector
  uEye = mc.camera_up.cross(nEye);// uEye = view_up X nEye
  uEye.normalize();
  vEye = nEye.cross(uEye);

  cout<<"nEye: "<<nEye[0]<<"|"<<nEye[1]<<"|"<<nEye[2]<<endl;
  cout<<"uEye: "<<uEye[0]<<"|"<<uEye[1]<<"|"<<uEye[2]<<endl;
  cout<<"vEye: "<<vEye[0]<<"|"<<vEye[1]<<"|"<<vEye[2]<<endl;
  cout<<"mc.heightAngle:  "<<mc.heightAngle<<endl;

  //Image plane setup
  dist = 1;
  imHeight = 2*dist*tan(mc.heightAngle/2);
  imWidth = imHeight*aspectRatio;
  centerImagePlane = (rayOrig-nEye)*dist;
  cout<<centerImagePlane[0]<<centerImagePlane[1]<<centerImagePlane[1]<<endl;
  ImPlaneCorner = centerImagePlane-uEye*(imWidth/2)-vEye*(imHeight/2);  
  float pixelWidth = imWidth/XRes,pixelHeight = imHeight/YRes;
  int max_color = 255;

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
		pixelCenterLoc = ImPlaneCorner + (imWidth/XRes)*j*(uEye) + (imHeight/YRes)*i*(vEye);
		pixelEyeDist = pixelCenterLoc-rayOrig;//(s-e) vector
          
		float R=0.0,G=0.0,B=0.0;
		float minMinT = XRes;
        
		 //cout<<"sphere.no_of_sphere: "<<sphere.no_of_sphere<<endl;
		 //object wise loop
		 for(int q=0;q<sphere.no_of_sphere;q++) 
		  {
		    minT = intersect(rayOrig,pixelEyeDist,sphere.radius[q],sphere.centerSphere[q]);
            
			//cout<<"minT "<<minT<<endl;		  
		    //cout<<"minT is: for pixel ("<<i<<","<<j<<") is:  "<< minT<<endl;

		 if(minT > 0)
		    {
		  intersection_point = rayOrig + pixelEyeDist*minT;
		  intersection_normal = (intersection_point-sphere.centerSphere[q]);
		  intersection_normal.normalize();
		  k = fabs(intersection_normal.dot(illumination));
		  // cout<<"k: "<<k<<endl;
		  pixelColor = k*sphere.color[q];
		  if(minT < minMinT) 
		  {
		  R = pixelColor[0];
		  G = pixelColor[1];
		  B = pixelColor[2];
		  minMinT = minT;
		  }

	   }
		// cout<<"R: "<<R<<"  G: "<<G<<"  B: "<<B<<endl;
		 }
		fprintf(fp,"%d %d %d\n",int(255*R),int(255*G),int(255*B));		 
 	  }  
  }

  fclose(fp);
  system("pause");
  return 0;
 }
void usage_error()
 {
  cerr << "Usage: sample_read_iv {filename}" << endl;
  system ("pause");
  exit(10);
 }


//intersect function definition
 float intersect(SbVec3f rayOrig,SbVec3f rayDirection,float radius,SbVec3f centerSphere)
 {
	  float A,B,C,minT=-1;
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
	    float t0 = (-B-(sqrt(checkVal)))/(2*A);
	    float t1 = (-B+(sqrt(checkVal)))/(2*A);

	    if(t0<t1 && t0>0)
			minT = t0;
		else if(t0>t1 && t1>0)
			minT =t1;
		else//when t0=t1 
			minT = t1; 
     
	     }
	 return minT;
 }