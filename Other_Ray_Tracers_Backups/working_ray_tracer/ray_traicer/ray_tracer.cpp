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

 int main(int argc, char **argv)//main function starts here
{
 
 float min_t=-1,k;
 SbVec3f ray_orig,ray_dir, center, intersection_point,intersection_normal,centerImagePlane,ImPlaneCorner;
 SbVec3f u_eye,v_eye,n_eye,N_eye,pixelEyeDist,pixelLoc,illumination(0,0,1);
 SbVec3f diffuseComponent(1,0,0);
 int X_Res,Y_Res;
 float aspect_ratio =1.0;
 float dist;
 float imWidth,imHeight;
 FILE *fp,*fp1;
 fp = fopen("output.ppm","w");
 fp1 = fopen("help.txt","w");
 X_Res = atoi(argv[2]);
 Y_Res = atoi(argv[3]);
 aspect_ratio = X_Res/Y_Res;

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
  cout<<"camera rotation angle: "<<mc.camera_rotation_angle<<endl;

  ray_orig = mc.camera_position;
  cout<<"ray_orig: "<<ray_orig[0]<<ray_orig[1]<<ray_orig[2]<<endl;

  cout<<"mc.camera_direction: "<<mc.camera_direction[0]<<"|"<<mc.camera_direction[1]<<"|"<<mc.camera_direction[2]<<endl;

  n_eye = mc.camera_direction*(-1);
  cout<<"n_eye: "<<n_eye[0]<<"|"<<n_eye[1]<<"|"<<n_eye[2]<<endl;
  n_eye.normalize();//normalizing n_eye vector
  u_eye = mc.camera_up.cross(n_eye);// u_eye = view_up X n_eye
  u_eye.normalize();
  v_eye = n_eye.cross(u_eye);

  cout<<"n_eye: "<<n_eye[0]<<"|"<<n_eye[1]<<"|"<<n_eye[2]<<endl;
  cout<<"u_eye: "<<u_eye[0]<<"|"<<u_eye[1]<<"|"<<u_eye[2]<<endl;
  cout<<"v_eye: "<<v_eye[0]<<"|"<<v_eye[1]<<"|"<<v_eye[2]<<endl;
  cout<<"mc.heightAngle:  "<<mc.heightAngle<<endl;

  //Image plane setup
  dist = 1;
  imHeight = 2*dist*tan(mc.heightAngle/2);
  imWidth = imHeight*aspect_ratio;
  centerImagePlane = (ray_orig-n_eye)*dist;
  cout<<centerImagePlane[0]<<centerImagePlane[1]<<centerImagePlane[1]<<endl;
  ImPlaneCorner = centerImagePlane-u_eye*(imWidth/2)-v_eye*(imHeight/2);  
  float pixelWidth = imWidth/X_Res;
  float pixelHeight = imHeight/Y_Res;
  int max_color = 255;
  cout<<imWidth<<imHeight<<endl;
  cout<<"pixelWidth: "<<pixelWidth<<endl;
  cout<<"pixelHeight: "<<pixelHeight<<endl;
  cout<<"X_Y RES"<<X_Res<<Y_Res<<endl;

  cout<<"imwidth:  "<<imWidth<<endl;
  cout<<"imheight:  "<<imHeight<<endl;
  cout<<"centerImagePlane:  "<<centerImagePlane[0]<<"|"<<centerImagePlane[1]<<"|"<<centerImagePlane[2]<<endl;
  cout<<"ImPlaneCorner:  "<<ImPlaneCorner[0]<<"|"<<ImPlaneCorner[1]<<"|"<<ImPlaneCorner[2]<<endl;
  cout<<"sphere.center_sphere: "<<sphere.center_sphere[0]<<"|"<<sphere.center_sphere[1]<<"|"<<sphere.center_sphere[2]<<endl;

  //creating output ppm file and writing header of the file
  fprintf(fp,"P3 %d %d %d\n",X_Res,Y_Res,max_color);
  SbVec3f pixelColor;
  

  //cout<<sphere.diffuseComponents[0]<<"|"<<sphere.diffuseComponents[1]<<"|"<<sphere.diffuseComponents[2]<<endl;

  //************************************************Loop starts here**************************************//
  for(int i=0;i<Y_Res;i++)
  {
	  for(int j=0;j<X_Res;j++)
	  {
		  
		  pixelLoc[0] = ImPlaneCorner[0]+ (pixelWidth/2) + pixelWidth*j;
		  pixelLoc[1] = ImPlaneCorner[1] + (pixelHeight/2) + pixelHeight*i;
		  pixelLoc[2] = ImPlaneCorner[2] ;

		  //cout<<j*(imWidth/X_Res)<<"|"<<i*(imHeight/Y_Res)<<endl;
		  //if(j == 12)
		  //cout<<i<<" "<<j<<" "<<pixelLoc[0]<<"  |  "<<pixelLoc[1]<<"  |  "<<pixelLoc[2]<<endl;

		  pixelEyeDist = pixelLoc-ray_orig;//(s-e) vector
         // cout<<pixelEyeDist[0]<<"  |  "<<pixelEyeDist[1]<<"  |  "<<pixelEyeDist[2]<<endl;

		  min_t = intersect(ray_orig,pixelEyeDist,sphere.radius,sphere.center_sphere);
		  // cout<<"ray_orig: "<<ray_orig[0]<<"|"<<ray_orig[1]<<"|"<<ray_orig[2]<<endl;
		   //cout<<"pixelEyeDist: "<<pixelEyeDist[0]<<"|"<<pixelEyeDist[1]<<"|"<<pixelEyeDist[2]<<endl;
		   //cout<<"radius"<<sphere.radius<<endl;
		  //cout<<"sphere.center_sphere: "<<sphere.center_sphere[0]<<"|"<<sphere.center_sphere[1]<<"|"<<sphere.center_sphere[2]<<endl;
		  
		 //  cout<<"min_t is: for pixel ("<<i<<","<<j<<") is:  "<< min_t<<endl;

		  if(min_t > 0)
		  {
          intersection_point = ray_orig + pixelEyeDist*min_t;
		 /* intersection_point[0] = ray_orig[0] +(pixelEyeDist[0]*min_t);
		  intersection_point[1] = ray_orig[1] +(pixelEyeDist[1]*min_t);
		  intersection_point[2] = ray_orig[2] +(pixelEyeDist[2]*min_t);
         */ 
		  //cout<< intersection_point[0] <<"|"<< intersection_point[1] <<"|"<< intersection_point[2] <<endl;	     

		  intersection_normal = (intersection_point-sphere.center_sphere);
		  intersection_normal.normalize();
		//  cout<< intersection_normal[0] <<"|"<< intersection_normal[1] <<"|"<< intersection_normal[2] <<endl;
		 
		  k = fabs(intersection_normal.dot(illumination));
		 // cout<< intersection_normal[0] <<"|"<< intersection_normal[1] <<"|"<< intersection_normal[2] <<endl;
		 // cout<<"k: "<<k<<endl;
		  pixelColor = k*sphere.color;
		  }

		//  cout<<diffuseComponent[0]<<"|"<<diffuseComponent[1]<<"|"<<diffuseComponent[2]<<endl;
		//cout<<sphere.color[0]<<"|"<<sphere.color[1]<<"|"<<sphere.color[2]<<endl;
		//  cout<<"AAAAAAAA"<<endl;
		//  cout<<pixelColor[0]<<"|"<<pixelColor[1]<<"|"<<pixelColor[2]<<endl;

		  if(min_t == -1.0)
            fprintf(fp,"%d %d %d\n",0,0,0);
		  else
		    fprintf(fp,"%d %d %d\n",int(255*pixelColor[0]),int(255*pixelColor[1]),(255*pixelColor[2]));
		  //  fprintf(fp,"%d %d %d\n",255,0,0);
		 
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
 float intersect(SbVec3f ray_orig,SbVec3f ray_dir,float radius,SbVec3f center_sphere)
 {
	  float A,B,C,min_t=-1;
	  ray_dir.normalize(); //normalizing the vector
	  SbVec3f a,b;

	 //finding the coefficients for calculating the disciminant
     A = ray_dir.dot(ray_dir);
	 a = (ray_orig - center_sphere);
	 B = 2*(ray_dir.dot(a));
	 C = a.dot(a) - (radius*radius);
 
     //checking if B^2-4C >0  
     float check_val = B*B-4*A*C;
	 //cout<<check_val;
     if (check_val>=0)
      {
	    float t0 = (-B-(sqrt(check_val)))/(2*A);
	    float t1 = (-B+(sqrt(check_val)))/(2*A);

	    if(t0<t1 && t0>0)
			min_t = t0;
		else if(t0>t1 && t1>0)
			min_t =t1;
		else//when t0=t1 
			min_t = t1; 
     
	     }
	 return min_t;
 }