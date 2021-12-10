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

class my_light {

public:

	SbVec3f location;

  my_light(OSUInventorScene *scene)	// constructor
   {
    for (int j = 0; j < scene->Lights.getLength(); j++) 
	 {
       SoLight * light = (SoLight *) scene->Lights[j];
       SoType light_type = light->getTypeId();
    	
	  if (light_type == SoPointLight::getClassTypeId()) 
	   {
         SoPointLight * point_light = (SoPointLight *) light;
         location = point_light->location.getValue();
       }
	 }
	}
};