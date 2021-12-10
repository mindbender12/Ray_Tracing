//******************************************************************************** //
//                              Author: Soumya Dutta                               //
//                              CSE681 Introduction to Computer Graphics           //
//                              Lab Assignment 2 (Autumn 2011)                     //
//                              Date: October 31, 2011                             //  
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
	float shininess[100];
    SbVec3f centerSphere[100],diffColor[100],ambientColor[100],specularColor[100];
	double dred,dgreen,dblue,sred,sgreen,sblue,ared,agreen,ablue;
    SbVec3f diffuseComponents[100];
	int no_of_sphere;
    SbMatrix Scale;

my_sphere(OSUInventorScene *scene)// constructor
  {
	no_of_sphere = scene->Objects.getLength();
	for (int i = 0; i < scene->Objects.getLength(); i++) 
    {	
    OSUObjectData * obj = (OSUObjectData *)scene->Objects[i];
	shape_type = obj->shape->getTypeId();
	if (shape_type == SoSphere::getClassTypeId()) 
	{
	SoSphere * sphere = (SoSphere *) obj->shape;
    radius[i] = sphere->radius.getValue();
	SoTransform *transformation = obj->transformation; //obj is the current inventor sphere 
	SbVec3f scale_vector = transformation->scaleFactor.getValue(); 
    SbRotation rotation = transformation->rotation.getValue();
    SbVec3f rotation_axis;
    float rotation_angle;
    rotation.getValue(rotation_axis, rotation_angle);
    SbVec3f translation_vector = transformation->translation.getValue();
	SbMatrix T,S,R,F;
    T.setTranslate(translation_vector);
    R.setRotate(rotation);
    S.setScale(scale_vector);
	radius[i] = radius[i]*scale_vector[0];	
    F = S*R*T;// F Total Transformation matrix
	Scale = F;
	SbVec3f pos(0,0,0);
    F.multVecMatrix(pos, pos);
	centerSphere[i] = pos;	
	SoMaterial * material = obj->material;
	dred = material->diffuseColor[0][0];
	dgreen = material->diffuseColor[0][1];
	dblue = material->diffuseColor[0][2];
	diffColor[i].setValue(dred,dgreen,dblue);
	ared = material->ambientColor[0][0];
	agreen = material->ambientColor[0][1];
	ablue = material->ambientColor[0][2];
	ambientColor[i].setValue(ared,agreen,ablue);
	sred = material->specularColor[0][0];
	sgreen = material->specularColor[0][1];
	sblue = material->specularColor[0][2];
	specularColor[i].setValue(sred,sgreen,sblue);	
	shininess[i] = material->shininess[0];
	}
  }
}
};


