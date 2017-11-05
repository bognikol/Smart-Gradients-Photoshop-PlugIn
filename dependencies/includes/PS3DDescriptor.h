/*
    File: PS3DDescriptor.h

    Copyright 2006 by Adobe Systems, Inc.
	
	This file has some helpers for packing descriptors for filter plugins to communicate with Photoshop's 3D layers
    
 */

#pragma once 

#include "PITypes.h"
#include "PIActions.h"
//#include "PIUtilities.h"
#include "PI3D.h"
#include "PSScene.h"

//This is a data structure that gives Photoshop everything it needs for 3D
//There is some redundency between this data and PSScene.  This allows PS to not
//have to open the scene descriptor, which was not possible for some formats in the beginning.

typedef struct {

//Layer data
	VPoint				layerOrigin;		//layer's current origin (0,0) is teh document origin
	VPoint				layerSize;			//width and height of layer
	FloatPoint			layerScale;			//2D scaling that has been applied
	nativeFloat			duration;			//Duration of layer in seconds - default to 10 seconds
	
//Size of the original rendering window - field of view is determined off of this.
	VPoint				sceneOrigin;		//offset from layerOrigin
	VPoint				sceneSize;

//Path to the original 3D file - can be NULL if there was none
	uint16				*scenePath;			//path to the original file

//3D Tool info
	int32				stateIndex;			//Currently selected view - default to 1 (Default View).  This is a 1 based list
	U3DStateArray		stateList;			//Number of additional camera views on top of the 7 built-in views
											//Only the camera is looked at.  Can be NULL.
											//Names of the additional camera views in the tool's menu.  Can be NULL.

//Layers palette
	Boolean				texturesExternal;   //Only set to false for u3d and kmz where textures are inside the 3d blob
	Boolean				texturesEnabled;	//The master eyeball in layers palette
	Boolean				*textureEnabled;	//The eyeball on each layer.  This array should be textures.length in size.
	U3DTextureArray		textureList;		//textureNames - a full path to the origin texture file.  Used to enable/disable Replace Textures among other things
											//texturePaths - path to the current temp file for this texture 
											//textureType - Only really used in CS3 to check for bump maps to disable cross sectioning.  Will have uses in CS4+.

//State and scene data
	RenderState			currentRenderState; //This determines how things currently get displayed - camera pos, object pos, cross section, etc... - see PI3D.h 
	PI3DScene			*currentScene;		//Holds the scene graph - see PSScene.h

//Texture function
	GetTextureSizeFunction	getTextureSizeFunction;
	GetTextureFunction		getTextureFunction;

//Transfer function	
	PIActionDescriptor		gradientDesc;
	PIActionDescriptor		gradientDesc2;
	U3DBoundingBox			initialBoundingBox;

} PI3DLayer;



#ifdef __cplusplus
extern "C" {
#endif

//Pass in teh procs and the layer data structure filled in and wil give back the descriptor for Photoshop
OSErr		PI3DInitialize3DLayer(PI3DLayer *layerData,VPoint *docSize);
OSErr		PI3DKill3DLayer(PI3DLayer *layerData);

OSErr		PI3DMakeLayerDescriptor(PI3DDescriptorProcs *procs, PI3DLayer *theLayer, PIActionDescriptor theLayerDescriptor);
OSErr		PI3DGetLayerFromDescriptor(PI3DDescriptorProcs *procs, PIActionDescriptor theLayerDescriptor, PI3DLayer *theLayer);

OSErr		PI3DMakeStateDescriptor(PI3DDescriptorProcs *procs,RenderState *renderState, uint16 *name, PIActionDescriptor stateDescriptor);
OSErr		PI3DGetStateFromDescriptor(PI3DDescriptorProcs *procs, PIActionDescriptor stateDesc, uint16 *viewName, RenderState *state);

#ifdef __cplusplus
}
#endif