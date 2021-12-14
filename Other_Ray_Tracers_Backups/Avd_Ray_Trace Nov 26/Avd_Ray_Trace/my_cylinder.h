//******************************************************************************** //
//                              Author: Soumya Dutta                               //
//                              CSE681 Introduction to Computer Graphics           //
//                              Lab Assignment 4 (Autumn 2011)                     //
//                              Date: December 01, 2011                            //  
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
#include <Inventor/nodes/SoCylinder.h>
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


class my_cylinder 
{
public:
     
	SoType shape_type;
	float radius[100], height[100];
	float shininess[100],transparency[100];
    SbVec3f centerCylinder[100],diffColor[100],ambientColor[100],specularColor[100];
	float dred,dgreen,dblue,sred,sgreen,sblue,ared,agreen,ablue;    
	int noOfCylinder;
    SbMatrix Scale[100],Finv[100],FinvT[100];
	SbMatrix T[100],S[100],R[100],F[100];
	SoSFBitMask parts;	
	float hmin[100]; 
    float hmax[100];

my_cylinder(OSUInventorScene *scene)// constructor takes scene as input
  {
	noOfCylinder = 0;
	int index=0;    
    for (int ii = 0; ii < scene->Objects.getLength(); ii++) 
    {	
		OSUObjectData * obj = (OSUObjectData *)scene->Objects[ii];
		shape_type = obj->shape->getTypeId();
		if (shape_type == SoCylinder::getClassTypeId()) 
			{				
				noOfCylinder = noOfCylinder+1;	
				SoCylinder * cylinder = (SoCylinder *) obj->shape;	
				radius[index] = cylinder->radius.getValue();
				height[index] = cylinder->height.getValue();				
				SoTransform *transformation = obj->transformation; //obj is the current inventor cylinder 
				SbVec3f scale_vector = transformation->scaleFactor.getValue(); 
				SbRotation rotation = transformation->rotation.getValue();
				SbVec3f rotation_axis;
				float rotation_angle;
				rotation.getValue(rotation_axis, rotation_angle);
				SbVec3f translation_vector = transformation->translation.getValue();				
				T[index].setTranslate(translation_vector);
				R[index].setRotate(rotation);
				S[index].setScale(scale_vector);
				//radius[index] = radius[index]*scale_vector[0]; // commented for nonuniform scaling 
				//height[index] = height[index]*scale_vector[0]; // commented for nonuniform scaling
				F[index] = S[index]*R[index]*T[index];// F Total Transformation matrix		
				SbVec3f pos(0,0,0);
				//F[index].multVecMatrix(pos, pos); // commented for nonuniform scaling 
				Finv[index] = F[index].inverse(); 
				FinvT[index]= Finv[index].transpose();
				centerCylinder[index] = pos;	
				hmin[index] = -height[index]/2;
				hmax[index] =  height[index]/2;
				SoMaterial * material = obj->material;
				dred = material->diffuseColor[0][0];
				dgreen = material->diffuseColor[0][1];
				dblue = material->diffuseColor[0][2];
				diffColor[index].setValue(dred,dgreen,dblue);
				ared = material->ambientColor[0][0];
				agreen = material->ambientColor[0][1];
				ablue = material->ambientColor[0][2];
				ambientColor[index].setValue(ared,agreen,ablue);
				sred = material->specularColor[0][0];
				sgreen = material->specularColor[0][1];
				sblue = material->specularColor[0][2];
				specularColor[index].setValue(sred,sgreen,sblue);	
				shininess[index] = material->shininess[0];
				transparency[index] = material->transparency[0];
				index++;
			}
	}
}

};

