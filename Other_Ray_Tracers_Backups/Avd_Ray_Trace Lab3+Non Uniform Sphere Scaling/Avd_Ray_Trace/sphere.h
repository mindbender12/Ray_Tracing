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


class my_sphere 
{
public:
     
	SoType shape_type;
	double radius[100];
	float shininess[100],transparency[100];
    SbVec3f centerSphere[100],diffColor[100],ambientColor[100],specularColor[100];
	double dred,dgreen,dblue,sred,sgreen,sblue,ared,agreen,ablue;    
	int noOfSphere;
    SbMatrix Scale,T[100],S[100],R[100],F[100],Finv[100],FinvT[100];

my_sphere(OSUInventorScene *scene)// constructor takes scene as input
  {
	noOfSphere = 0;
	int j=0;
    for (int i = 0; i < scene->Objects.getLength(); i++) 
    {	
		OSUObjectData * obj = (OSUObjectData *)scene->Objects[i];
		shape_type = obj->shape->getTypeId();
		if (shape_type == SoSphere::getClassTypeId()) 
			{				
				noOfSphere = noOfSphere+1;	
				SoSphere * sphere = (SoSphere *) obj->shape;	
				radius[j] = sphere->radius.getValue();
				SoTransform *transformation = obj->transformation; //obj is the current inventor sphere 
				SbVec3f scale_vector = transformation->scaleFactor.getValue(); 
				SbRotation rotation = transformation->rotation.getValue();
				SbVec3f rotation_axis;
				float rotation_angle;
				rotation.getValue(rotation_axis, rotation_angle);
				SbVec3f translation_vector = transformation->translation.getValue();
				SbVec3f pos(0,0,0);
				T[j].setTranslate(translation_vector);
				R[j].setRotate(rotation);
				S[j].setScale(scale_vector);

				if(scale_vector[0] == scale_vector[1] == scale_vector[2])
				  {
					  F[j] = S[j]*R[j]*T[j];// F Total Transformation matrix
					  radius[j] = radius[j]*scale_vector[0];
					  F[j].multVecMatrix(pos, pos);
					  centerSphere[j] = pos;
				  }
				else
				{					
					F[j] = S[j]*R[j]*T[j];// F Total Transformation matrix
					Finv[j] = F[j].inverse(); 
					FinvT[j]= Finv[j].transpose();
					centerSphere[j] = pos;
				}			

				// Material Properties
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



