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
	float radius;
    SbVec3f center_sphere,color;
	float red,green,blue;
    SbVec3f diffuseComponents;

my_sphere(OSUInventorScene *scene)
  {
	for (int i = 0; i < scene->Objects.getLength(); i++) 
    {
    OSUObjectData * obj = (OSUObjectData *)scene->Objects[i];
	shape_type = obj->shape->getTypeId();

	if (shape_type == SoSphere::getClassTypeId()) 
	{
	SoSphere * sphere = (SoSphere *) obj->shape;
    radius = sphere->radius.getValue();
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
    F = S*R*T; 
	SbVec3f pos(0,0,0);
    F.multVecMatrix(pos, pos);
	center_sphere = pos;	
	//center_sphere.setValue(0,0,0);
	// object material (color)
	SoMaterial * material = obj->material;
	red = material->diffuseColor[0][0];
	green = material->diffuseColor[0][1];
	blue = material->diffuseColor[0][2];
	color.setValue(red,green,blue);
	}
    }
}
};




