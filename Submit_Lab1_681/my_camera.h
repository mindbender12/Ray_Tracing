//******************************************************************************** //
//                              Author: Soumya Dutta                               //
//                              CSE681 Introduction to Computer Graphics           //
//                              Lab Assignment 1 (Autumn 2011)                     //
//                              Date: October 17, 2011                             //  
//******************************Header file includes*******************************//

#include <iostream>
#include <math.h>
#include <vector>

#include <Inventor/SbPList.h>
#include <Inventor/SoType.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoCamera.h>

#include "OSUInventor.h"

class my_camera {

public :
	SbVec3f camera_position;
	SbRotation camera_orientation;
	SbVec3f camera_rotation_axis;
	float camera_rotation_angle;
	float camera_aspectRatio, heightAngle,focalDistance;
	SoCamera * camera;
	SoType camera_type;
	SbVec3f camera_direction, camera_up;

  my_camera(OSUInventorScene *scene)//constructor
   {
    camera = scene->Camera;
    camera_position = camera->position.getValue();
    camera_orientation = camera->orientation.getValue();        
    camera_orientation.getValue(camera_rotation_axis, camera_rotation_angle);
    camera_aspectRatio= camera->aspectRatio.getValue();
    camera_type = camera->getTypeId();
	heightAngle = 0.78;
	//focalDistance = 1;	
	// calculate camera direction and camera up direction    
    camera_orientation.multVec(SbVec3f(0, 0, -1),camera_direction);
    camera_orientation.multVec(SbVec3f(0, 1, 0),camera_up);
	}

};


