/*
    File: PSScene.h

    Copyright 2006 by Adobe Systems, Inc.
	
	This file defines the basic data structure of a 3D scene in Photoshop
	    
 */

#pragma once 

#include "PITypes.h"
#include "PIActions.h"
#include "PIBufferSuite.h"
#include "PIStringTerminology.h"
#include "PITerminology.h"
#include "ASZStringSuite.h"
#include "PI3D.h"

//These must get filled in
typedef struct {
	PSActionDescriptorProcs		*actionDescriptorProcs;
	PSActionListProcs			*actionListProcs;
	PSBasicActionControlProcs	*actionControlProcs;
	ASZStringSuite				*zStringProcs;
	HandleProcs					*handleProcs;
} PI3DDescriptorProcs;

typedef struct {
	PI3DScene			*scene;
	void				*userData;						/* we use this for the file stream */
	
	char				padding[512];
} PI3DImport;

typedef struct PI3DPosition
	{
	PI3DFloat				x;								// x location
	PI3DFloat				y;								// y location
	PI3DFloat				z;								// z location
	PI3DFloat				xAngle;							// x angle
	PI3DFloat				yAngle;							// y angle
	PI3DFloat				zAngle;							// z angle	

/* Reserved for future expansion */
	char					reserved [128]; 				// Set to zero 
	}
PI3DPosition, *PI3DPositionPtr;

#ifdef __cplusplus
extern "C" {
#endif

// Parse your 3d format - this function must be implemented or stubbed out by the user
int					PI3DParseFile (PI3DImport *importer); 
void				PI3DUpdateRenderState(RenderState *renderState);

// Utility
void				PI3DCleanupName (char *);	// cleanup malformed name
void*				PI3DMemoryAlloc(size_t size); // allocation routine
void				PI3DMemoryFree(void* p); //deallocation routine

// List helpers
void				PI3DListAdd (PI3DList **root, PI3DList *new_node);
void*				PI3DListSearch (PI3DList **root, const char *name);
void				PI3DListDelete (PI3DList **root);
void*				PI3DListFindAll(PI3DScene* scene, const char *name);

// Initialization helpers
void				PI3DInitializeRenderSettings(U3DRenderSettings	&renderSettings); // Initialize render settings
void				PI3DSceneInitGlobals(PI3DScene *scene); // Initialize the globals - do for each format
PI3DMaterial		*PI3DCreateMaterial (void); //create new material
PI3DKeyFrame		*PI3DCreateKeyframe (void);	//create new keyframe
PI3DCamera			*PI3DCreateCamera (void);	//create new camera
PI3DLight			*PI3DCreateLight (void);	//create new light
PI3DMesh			*PI3DCreateMesh (char *name, int vertices, int faces, int textures, int normals, int lines, int colors); //create new mesh
PI3DAnimationData	*PI3DCreateAnimation (int32 flags, int32 keys); //create new animation

// Mesh helpers
Boolean				PI3DAddVertexToMesh(PI3DMesh *mesh, PI3DVector v, int32 &maxVertices);
Boolean				PI3DAddNormalToMesh(PI3DMesh *mesh, PI3DVector n, int32 &maxNormals);
Boolean				PI3DAddUVToMesh(PI3DMesh *mesh, PI3DPoint uv, int32 &maxUVs);
Boolean				PI3DAddFaceToMesh(PI3DMesh *mesh, PI3DFace *f, int32 &maxFaces);

// Material helpers
PI3DMaterial		*PI3DUpdateMaterials (PI3DScene *scene, const char *new_material, int ext); //update material list
int32				PI3DFindMaterial(PI3DScene *scene, const char *name);
PI3DMaterial		*PI3DGetMaterial(PI3DScene *scene, int32 index);

//matrix helpers
void				PI3DIdentityMatrix(PI3DMatrix & matrix);
void				PI3DCopyMatrix(PI3DMatrix & matrixsrc, PI3DMatrix & matrixdst);
void				PI3DInvertMatrix(PI3DMatrix & matrix, PI3DMatrix & mout);
void				PI3DTransformPoint(const PI3DVector& p, const PI3DMatrix & matrix, PI3DVector & pout);
void				PI3DInvertQuat(const PI3DQuat& quat, PI3DQuat & quatout);
void				PI3DQuatRotateMatrix(const PI3DQuat& quat, PI3DMatrix & matrixout);
void				PI3DMatrixMultiply( const PI3DMatrix &_a, const PI3DMatrix & _b, PI3DMatrix & matrixout);
void				PI3DTransform(const PI3DMatrix & matrixin1, const PI3DMatrix & matrixin2, PI3DMatrix & matrixout);
void				PI3DSetScale(const PI3DVector& s, PI3DMatrix & matrixout);
void				PI3DSetTranslate(const PI3DVector& s, PI3DMatrix & matrixout);
void				PI3DScale(PI3DMatrix & matrixin, const PI3DVector& s, PI3DMatrix & matrixout);
void				PI3DOffset(const PI3DVector& p, PI3DMatrix & matrix);
void				PI3DTranslate(PI3DMatrix & matrixin, const PI3DVector& s, PI3DMatrix & matrixout);
void				PI3DMatrixToPosition(PI3DMatrix & matrix,PI3DVector & position, PI3DVector & angle);
nativeFloat			PI3DDotProduct(const PI3DVector& v1, const PI3DVector& v2);
void				PI3DNormalize(PI3DVector & v);
void				PI3DVectorTransform(const PI3DMatrix & matrix , const PI3DVector & p, PI3DVector & dst);
void				PI3DMultiplyQuat(const PI3DQuat& u, const PI3DQuat& q, PI3DQuat &result);
void				SetMatrixToIdentity(PI3DMatrix &matrix);
void				SetMatrixToRotateZ(PI3DMatrix &matrix, PI3DFloat angleZRad);
void				PI3DMatrixToVectorAngle(PI3DMatrix & matrix,PI3DVector & position, PI3DVector & angle);
bool				PI3DIsIdentityMatrix(PI3DMatrix & matrix);
bool				PI3DDecompose(PI3DMatrix & min, PI3DVector & position, PI3DQuat & rotation, PI3DVector & scale);


// Scene calls
PI3DScene			*PI3DCreateScene(void *userData); // Call CreateScene to set up the scene graph
void				PI3DKillScene(PI3DScene *scene); // Call KillScene to tear down the scene graph

// Parse the Scene into and out of the descriptors to pass the scene to and from the engine
short				PI3DParseSceneIntoDescriptor(PI3DDescriptorProcs *procs,PI3DScene	*scene,PIDescriptorHandle *sceneDescriptor);
short				PI3DParseDescriptorIntoScene(PI3DDescriptorProcs *procs,PIDescriptorHandle sceneDescriptor,PI3DScene **scene);		

// String/path helpers
void				PI3DGetPathFromSpec(SPPlatformFileSpecificationW *fileSpec,uint16 *path);
uint32				PI3DStringLength16(const uint16 *string);
uint32				PI3DStringLengthW(const wchar_t *string);
void				PI3DStringCopyW16(uint16 *destString,const wchar_t *sourceString);
void				PI3DStringCopyWC(char *destString,const wchar_t *sourceString);
void				PI3DStringCopy16C(char *destString,const uint16 *sourceString);
void				PI3DStringCopy16W(wchar_t *destString,const uint16 *sourceString);
void				PI3DStringCopyC16(uint16 *destString,const char *sourceString);
void				PI3DStringAppend16(uint16 *destString, uint32 destSize, uint16 *sourceString);
void				PI3DStringAppend16C(uint16 *destString, uint32 destSize, char *sourceString);

void				PI3DTrimRight(char *str);
void				PI3DTrimLeft(char *str);
Boolean				PI3DSplitString(char *leftStr, char *srcStr, bool splitOnWhitespace, char splitChar);
	
//Lights
void				PI3DAddInfiniteLight(PI3DLight **lightList,float dirx, float diry, float dirz, float red, float green, float blue);
void				PI3DAddPointLight(PI3DLight **lightList,float cx, float cy, float cz, float red, float green, float blue, bool cameraBound);
void				PI3DAddSpotLight(PI3DLight **lightList,float cx, float cy, float cz, float red, float green, float blue, bool cameraBound);
PI3DLight*			PI3DCreateInfiniteLight(float dirx, float diry, float dirz, float red, float green, float blue, bool cameraBound, int lightCount);
PI3DLight*			PI3DCreatePointLight(float cx, float cy, float cz, float red, float green, float blue, bool cameraBound, int lightCount);
PI3DLight*			PI3DCreateSpotLight(float cx, float cy, float cz, float red, float green, float blue, bool cameraBound, int lightCount);
void				PI3DAddDefaultLights(PI3DScene *scene);
#ifdef __cplusplus
}
#endif
