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

class my_cube
{
public:
	SoType shape_type;
	float width[1000];
    float height[1000];
    float depth[1000];
    SbVec3f pointFront[1000],pointBack[1000];
	int noOfCube;
	float shininess[1000],transparency[1000];
    SbVec3f centerSphere[1000],diffColor[1000],ambientColor[1000],specularColor[1000];
	double dred,dgreen,dblue,sred,sgreen,sblue,ared,agreen,ablue;
	SbVec3f pos;

my_cube(OSUInventorScene *scene)
{
	noOfCube =0;
	int j=0;
	float rotation_angle;
	for (int i = 0; i < scene->Objects.getLength(); i++) 
       {	
			OSUObjectData * obj = (OSUObjectData *)scene->Objects[i];
			shape_type = obj->shape->getTypeId();
			if (shape_type == SoCube::getClassTypeId()) 
			{
				SoCube * cube = (SoCube *) obj->shape;
				noOfCube = noOfCube+1;
				width[j] = cube->width.getValue();
				height[j] = cube->height.getValue();
				depth[j] = cube->depth.getValue();
				SoTransform *transformation = obj->transformation; //obj is the current inventor sphere 
				SbVec3f scale_vector = transformation->scaleFactor.getValue(); 
				SbRotation rotation = transformation->rotation.getValue();
				SbVec3f rotation_axis;				
				rotation.getValue(rotation_axis, rotation_angle);
				SbVec3f translation_vector = transformation->translation.getValue();
				SbMatrix T,S,R,F;
				T.setTranslate(translation_vector);
				R.setRotate(rotation);
				S.setScale(scale_vector);					
				F = S*T; //S*R*T;  Rotation is not in calculation for the time being; F Total Transformation matrix
				pos.setValue(0,0,0);
				F.multVecMatrix(pos, pos);
				width[j] = width[j]*scale_vector[2];
				height[j] = height[j]*scale_vector[0];
				depth[j] = depth[j]*scale_vector[1];
				pointFront[j] = pos - SbVec3f(width[j]/2,depth[j]/2,height[j]/2);				
				pointBack[j] = pos +  SbVec3f(width[j]/2,depth[j]/2,height[j]/2);				
				SoMaterial * material = obj->material;
				dred = material->diffuseColor[0][0];
				dgreen = material->diffuseColor[0][1];
				dblue = material->diffuseColor[0][2];
				diffColor[j].setValue(dred,dgreen,dblue);
				ared = material->ambientColor[0][0];
				agreen = material->ambientColor[0][1];
				ablue = material->ambientColor[0][2];
				ambientColor[j].setValue(ared,agreen,ablue);
				sred = material->specularColor[0][0];
				sgreen = material->specularColor[0][1];
				sblue = material->specularColor[0][2];
				specularColor[j].setValue(sred,sgreen,sblue);	
				shininess[j] = material->shininess[0];
				transparency[j] = material->transparency[0];
				j++;

			}
	  }
		
}
};

