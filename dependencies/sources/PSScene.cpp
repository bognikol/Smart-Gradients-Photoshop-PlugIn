#define _CRT_SECURE_NO_DEPRECATE 1
// #include "PProjectPCH.h"
#include "PSScene.h"
#include "PITerminology.h"
#include "PIActions.h"
#include "PIBufferSuite.h"
#include "PI3D.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#pragma warning(disable:4800)
#undef __ppc__
#ifndef __ppc__
#define LOW_BYTE_FIRST 1
#endif

#if (LOW_BYTE_FIRST == 1)

#  define SWAP32_IF_BIGENDIAN(number)
#  define SWAP64_IF_BIGENDIAN(number)

#else
/* Swap bytes in 32 bit value.  */
#define U3D_BYTE_SWAP_32(x) \
     ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) |               \
      (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))
/* Swap bytes in 64 bit value.  */
#define U3D_BYTE_SWAP_64(x) \
	(  ((((long long)x) & 0xff00000000000000ull) >> 56)                                    \
	| ((((long long)x) & 0x00ff000000000000ull) >> 40)                                 \
	| ((((long long)x) & 0x0000ff0000000000ull) >> 24)                                  \
	| ((((long long)x) & 0x000000ff00000000ull) >> 8)                                   \
	| ((((long long)x) & 0x00000000ff000000ull) << 8)                                   \
	| ((((long long)x) & 0x0000000000ff0000ull) << 24)                                  \
	| ((((long long)x) & 0x000000000000ff00ull) << 40)                                  \
	| ((((long long)x) & 0x00000000000000ffull) << 56))


#  define SWAP64_IF_BIGENDIAN(num) num = U3D_BYTE_SWAP_64(num)
#  define SWAP32_IF_BIGENDIAN(num) num = U3D_BYTE_SWAP_32(num)

#endif

extern PSBufferSuite1 *sPBufferSuite1;
extern Malloc3D g3DMallocFunction;
extern Free3D g3DFreeFunction;

void* PI3DMemoryAlloc(size_t size)
{
	return malloc(size);
}

void PI3DMemoryFree(void* p)
{
	free(p);
}
/* Swap bytes in 64 bit value.  */
static void PI3DByteSwapDouble(double *x)
{
#ifdef __ppc__
    uint64 words = *(uint64 *)(x);
    SWAP64_IF_BIGENDIAN(words);
	*x = *((double*)&words);
#endif
}

void PI3DByteSwapFloat(float *x)
{
#ifdef __ppc__
    uint32 words = *(uint32 *)(x);
    words=U3D_BYTE_SWAP_32(words);
	*x = *((float*)&words);
#endif
}
/******************************************************************************/
#if Macintosh
#pragma mark -
#endif
/******************************************************************************/
/* Insert a new node into the list */
void PI3DListAdd (PI3DList **root, PI3DList *new_node)
{
#if 1
	new_node->next = NULL;
	if (*root == NULL)
	{
		*root = new_node;
	}
	else
	{
		PI3DList		*p = *root;
		while (p->next)
		{
			p = (PI3DList *)p->next;
		}
		p->next = new_node;
	}
#else
	new_node->next = *root;
	*root = new_node;
#endif
}

/* Find the node with the specified name in the scene */
void *PI3DListFindAll(PI3DScene *scene, const char *name)
{
	PI3DList *p = NULL;
	p = (PI3DList*)PI3DListSearch((PI3DList **)&scene->cameraList, name);
	if (p) return p;
	p = (PI3DList*)PI3DListSearch((PI3DList **)&scene->keyFrameList, name);
	if (p) return p;
	p = (PI3DList*)PI3DListSearch((PI3DList **)&scene->lightList, name);
	if (p) return p;
	p = (PI3DList*)PI3DListSearch((PI3DList **)&scene->matPropList, name);
	if (p) return p;
	p = (PI3DList*)PI3DListSearch((PI3DList **)&scene->meshList, name);
	return p;
}

/* Find the node with the specified name */
void *PI3DListSearch (PI3DList **root, const char *name)
{
	PI3DList *p;
	for (p = *root; p != (PI3DList *) NULL; p = (PI3DList *) p->next)
	{
		if (strcmp (p->name, name) == 0)
		{
			break;
		}
	}
	return (void *)p;
}

/* Delete the entire list */
void PI3DListDelete (PI3DList **root)
{
	PI3DList *temp;

	while (*root != (PI3DList *) NULL) 
	{
		temp = *root;
		*root = (PI3DList *) (*root)->next;
		PI3DMemoryFree (temp);
	}
}
/******************************************************************************/
#if Macintosh
#pragma mark -
#endif
/******************************************************************************/
void		PI3DInitializeRenderSettings(U3DRenderSettings	&renderSettings)
	{
	memset(&renderSettings,0,sizeof(U3DRenderSettings));
	renderSettings.renderFaces=true;				//Flag for controlling whether or not to render faces
	renderSettings.faceStyle=0x00000004;			//Constant, Flat, Smooth, Bounding Box, Ray Traced, Unlit Texture, Face IDs, Depth, Normals
	renderSettings.faceColor[0]=1;					//Face colors in illustration modes
	renderSettings.faceColor[1]=1;					//Face colors in illustration modes
	renderSettings.faceColor[2]=1;					//Face colors in illustration modes
	renderSettings.textureType=0;					//When rendering diffuse only, what kind of texture do we display
	renderSettings.antiAliasQuality=1;				//1, 2, 4
	renderSettings.rayDepth=4;						//# of bounces (unused)
	renderSettings.showReflections=true;			//Flag for controlling whether or not to render reflections
	renderSettings.showRefractions=true;			//Flag for controlling whether or not to render refractions
	renderSettings.showShadows=true;				//Flag for controlling whether or not to render shadows
	renderSettings.removeBackfaces=false;			//Flag for controlling whether or not to remove back facing polygons
	renderSettings.renderEdges=false;				//Flag for controlling whether or not to render edges
	renderSettings.useAdvancedIllumination=true;	//Flag for controlling whether or not to use advanced illumination
	renderSettings.lineStyle=0x00000001;			//Constant, Flat, Smooth, Bounding Box, Illustrative
	renderSettings.lineColor[0]=0;					//Line colors in illustration modes
	renderSettings.lineColor[1]=0;					//Line colors in illustration modes
	renderSettings.lineColor[2]=0;					//Line colors in illustration modes
	renderSettings.lineWidth=1;						//Width of wireframe lines
	renderSettings.creaseValue=0;					//Threshold angle for illustration angle, 0 for simple wireframe - svakhine
	renderSettings.removeBackfaceLines=true;		//Flag for controlling whether or not to remove back facing lines
	renderSettings.removeHiddenLines=true;			//Flag for controlling visibility of lines thatw oudl not be seen in full face rendering
	renderSettings.renderVertices=false;			//Flag for controlling whether or not to render vertices
	renderSettings.vertexStyle=0x00000001;			//Constant, Flat, Smooth, Bounding Box
	renderSettings.vertexColor[0]=0;				//Vertex colors in illustration modes	
	renderSettings.vertexColor[1]=0;				//Vertex colors in illustration modes	
	renderSettings.vertexColor[2]=0;				//Vertex colors in illustration modes	
	renderSettings.vertexRadius=1;					//Radius of drawn vertices
	renderSettings.removeBackfaceVertices=true;		//Flag for controlling whether or not to remove back facing vertices
	renderSettings.removeHiddenVertices=true;		//Flag for controlling vertices of lines thatw oudl not be seen in full face rendering
	renderSettings.renderVolume=false;				//Flag for controlling whether or not to render volumes
	renderSettings.volumeStyle=0x00200000;			//MIP, Alpha, TransferFunction, etc.
	renderSettings.gradientEnhancement=true;		//Enhance Gradients	
	renderSettings.opacityScale=1.0;				//Coefficient for scaling the opacity of each voxel
	renderSettings.renderStereo=false;				//Flag for controlling whether or not to render in stereo
	renderSettings.stereoStyle=0x00004000;			//Red-Blue, Vertical Interlaced, Horizontal Interlaced
	renderSettings.stereoOffset=5;					//Eye separation in bbox coordinates
	renderSettings.stereoLineSpacing=40;			//Spacing to use for interlacing
	renderSettings.projection=0;					//Rendering into textures
	renderSettings.blendUnlitWithBaseValue=false;
	renderSettings.drawGroundPlane=false;
	renderSettings.drawLights=false;
	renderSettings.videoAtHighestQuality=false;		//Turn on ray tracing with advanced illumination when rendering video 
	renderSettings.focalPlane=0;					//Eye separation in bbox coordinates

	//Volume lookup
	for(int i=0;i<kTransferFunctionSize;i++)
		{
		renderSettings.transferFunction[0][i]=(((nativeFloat)kMaxTransferFunctionValue)*(((nativeFloat)i)/((nativeFloat)kTransferFunctionSize)));	
		renderSettings.transferFunction[1][i]=(((nativeFloat)kMaxTransferFunctionValue)*(((nativeFloat)i)/((nativeFloat)kTransferFunctionSize)));	
		renderSettings.transferFunction[2][i]=(((nativeFloat)kMaxTransferFunctionValue)*(((nativeFloat)i)/((nativeFloat)kTransferFunctionSize)));	
		renderSettings.transferFunction[3][i]=(((nativeFloat)kMaxTransferFunctionValue)*(((nativeFloat)i)/((nativeFloat)kTransferFunctionSize)));	
		}
	}

void PI3DSceneInitGlobals(PI3DScene *scene)
{
	scene->globalAmbientColor.red = 0.1f;
	scene->globalAmbientColor.green = 0.1f;
	scene->globalAmbientColor.blue = 0.1f;
	scene->globalNumFrames = 0;
	scene->globalAnimationStartTime = 0;
	scene->globalFPS = 30.0f;
	scene->globalUnits = 1.0f; //inches
	scene->geoReference = NULL;
	scene->texturesAreFlipped = false;
	strcpy (scene->customData, "");
}

PI3DMaterial *PI3DCreateMaterial()
{
	PI3DMaterial *new_mprop;
	PI3DColor defaultDiffuseSpec;
	PI3DColor defaultAmbient;
	PI3DColor black;

	black.blue = 0.0;
	black.green = 0.0;
	black.red = 0.0;
	defaultAmbient.blue = 30.0/255.0;
	defaultAmbient.green = 30.0/255.0;
	defaultAmbient.red = 30.0/255.0;
	defaultDiffuseSpec.blue = 192.0/255.0;
	defaultDiffuseSpec.green = 192.0/255.0;
	defaultDiffuseSpec.red= 192.0/255.0;

	new_mprop = (PI3DMaterial *) PI3DMemoryAlloc (sizeof(*new_mprop));
	if (!new_mprop)
		return NULL;

	memset(new_mprop,0,sizeof(PI3DMaterial));
	strcpy (new_mprop->name, "");
	new_mprop->ambient = defaultAmbient;
	new_mprop->diffuse = defaultDiffuseSpec;
	new_mprop->specular = defaultDiffuseSpec;
	new_mprop->emissive = black;
	new_mprop->shininess = 0.2;
	new_mprop->glossiness =1.0;
	new_mprop->transparency = 0.0;
	new_mprop->reflection = 0.15;
	new_mprop->selfIllumination = 0;
	new_mprop->shading = PI3DPhong;
	new_mprop->decal = false;
	new_mprop->wireframe = false;
	new_mprop->wireframesize = 0.0f;
	new_mprop->indexofrefraction = 1.0f; // Neutral
	new_mprop->twoSided = true;
	new_mprop->preservation = NULL;
	
	for(int i = 0; i<kMax3DMapTypes;i++)
	{
		strcpy (new_mprop->maps[i].map, "");
		new_mprop->maps[i].strength = 1.0f;
		new_mprop->maps[i].uscale = 1.0f;
		new_mprop->maps[i].vscale = 1.0f;
		new_mprop->maps[i].uoffset = 0.0f;
		new_mprop->maps[i].voffset = 0.0f;
		new_mprop->maps[i].angle = 0.0f;
		new_mprop->maps[i].flags = PI3DColorMap;
		new_mprop->maps[i].mapIs3DSStyle = false;
		new_mprop->maps[i].valueMin = 0;
		new_mprop->maps[i].valueMax = 1;
	}
	return new_mprop;
}

PI3DCamera *PI3DCreateCamera (void)
{
	PI3DCamera *new_cprop;

	new_cprop = (PI3DCamera *) PI3DMemoryAlloc (sizeof(*new_cprop));
	if (!new_cprop)
		return NULL;
	memset(new_cprop,0,sizeof(PI3DCamera));
	strcpy (new_cprop->name, "");
	new_cprop->aspectratio = 0.0;
	new_cprop->bank = 0.0;
	new_cprop->lens = 42.0;
	new_cprop->ortho = false;
	new_cprop->pos[0] = 0.0;
	new_cprop->pos[1] = 0.0;
	new_cprop->pos[2] = 0.0;
	new_cprop->target[0] = 0.0;
	new_cprop->target[1] = 0.0;
	new_cprop->target[2] = 0.0;
	new_cprop->zoomfactor = 0.0;
	new_cprop->preservation = NULL;
	return new_cprop;
}

PI3DLight *PI3DCreateLight (void)
{
	PI3DLight *new_lprop;

	new_lprop = (PI3DLight *) PI3DMemoryAlloc (sizeof(*new_lprop));
	if (!new_lprop)
		return NULL;
	memset(new_lprop,0,sizeof(PI3DLight));
	strcpy (new_lprop->name, "");
	new_lprop->attenuation = false;
	new_lprop->attenuationAbc.a = 0.0;
	new_lprop->attenuationAbc.b = 0.0;
	new_lprop->attenuationAbc.c = 0.0;
	new_lprop->attenuationType = PI3DLinearAttenuation;
	new_lprop->col.red = 0.0;
	new_lprop->col.green = 0.0;
	new_lprop->col.blue = 0.0;
	new_lprop->falloff = 180.0;
	new_lprop->hotspot = 0.7f*new_lprop->falloff;
	new_lprop->innerRadius = 0;
	new_lprop->multiple = 1.0;
	new_lprop->outerRadius = 0;
	new_lprop->pos[0] = 0.0;
	new_lprop->pos[1] = 0.0;
	new_lprop->pos[2] = 0.0;
	new_lprop->target[0] = 0.0;
	new_lprop->target[1] = 0.0;
	new_lprop->target[2] = 0.0;
	new_lprop->shadowFlag = false;
	new_lprop->isOn = true;
	new_lprop->ssmultiple = 0.0f;
	new_lprop->type = PI3DGlobalAmbientLight;
	new_lprop->preservation = NULL;
	return new_lprop;
}

PI3DKeyFrame *PI3DCreateKeyframe()
{
	PI3DKeyFrame *new_kfprop;

	new_kfprop = (PI3DKeyFrame *) PI3DMemoryAlloc (sizeof(*new_kfprop));
	if (!new_kfprop)
		return NULL;
	memset(new_kfprop,0,sizeof(PI3DKeyFrame));
	strcpy (new_kfprop->name, "");
	strcpy (new_kfprop->instancename, "");
	new_kfprop->flags1 = 0;
	new_kfprop->flags2 = 0;
	new_kfprop->nodeID = 0;
	new_kfprop->parentNodeID = 0;
	new_kfprop->nodeType = PI3DObjectNode;
	new_kfprop->pivot[0] = 0.0;
	new_kfprop->pivot[1] = 0.0;
	new_kfprop->pivot[2] = 0.0;
	new_kfprop->usepivot = true;
	new_kfprop->matrixPresent = false;
	new_kfprop->userange = false;
	new_kfprop->positionData = NULL;
	new_kfprop->rotationData = NULL;
	new_kfprop->scaleData = NULL;
	new_kfprop->fOVData = NULL;
	new_kfprop->rollData = NULL;
	new_kfprop->fallData = NULL;
	new_kfprop->colData = NULL;
	new_kfprop->hotData = NULL;
	new_kfprop->morphData = NULL;
	new_kfprop->hideData = NULL;
	new_kfprop->preservation = NULL;
	return new_kfprop;
}

/* Create a new mesh */
PI3DMesh *PI3DCreateMesh (char *name, int vertices, int faces, int textures, int normals, int lines, int colors)
{
	PI3DMesh *new_mesh;

	new_mesh = (PI3DMesh *) PI3DMemoryAlloc (sizeof(*new_mesh));
	if (!new_mesh)
		return NULL;
	memset(new_mesh,0,sizeof(PI3DMesh));
	strcpy (new_mesh->name, name);
	new_mesh->next = NULL;
	new_mesh->flags = 0x57;

	new_mesh->smoothingGroupPresent = false;
	new_mesh->matrixPresent = false;

	new_mesh->vertices = vertices;

	if (vertices <= 0)
	{
		new_mesh->vertex = NULL;
	}
	else
	{
		new_mesh->vertex = (PI3DVector *) PI3DMemoryAlloc(vertices * sizeof(*new_mesh->vertex));
		if (!new_mesh->vertex )
			return NULL;

	}

	new_mesh->colors = colors;

	if (colors <= 0)
	{
		new_mesh->color = NULL;
	}
	else
	{
		new_mesh->color = (PI3DVector *) PI3DMemoryAlloc(colors * sizeof(*new_mesh->color));
		if (!new_mesh->color )
			return NULL;
	}

	new_mesh->lines = lines;

	if (lines <= 0)
	{
		new_mesh->linecolors = NULL;
	}
	else
	{
		new_mesh->linecolors = (PI3DVector *) PI3DMemoryAlloc(lines * sizeof(*new_mesh->linecolors));
		if (!new_mesh->linecolors )
			return NULL;
	}

	new_mesh->textures = textures;

	if (textures <= 0)
	{
		new_mesh->texture = NULL;
	}
	else
	{
		new_mesh->texture = (PI3DPoint *) PI3DMemoryAlloc(textures * sizeof(*new_mesh->texture));
		if (!new_mesh->texture )
			return NULL;
	}

	new_mesh->normals = normals;

	if (normals <= 0)
	{
		new_mesh->normal = NULL;
	}
	else
	{
		new_mesh->normal = (PI3DVector *) PI3DMemoryAlloc(normals * sizeof(*new_mesh->normal));
		if (!new_mesh->normal )
			return NULL;
	}


	new_mesh->faces = faces;

	if (faces <= 0)
	{
		new_mesh->face = NULL;
		//new_mesh->mtl = NULL;
	}
	else
	{
		new_mesh->face = (PI3DFace *) PI3DMemoryAlloc (faces * sizeof(*new_mesh->face));
		if (!new_mesh->face )
			return NULL;
		//new_mesh->mtl = (PI3DMaterial **) PI3DMemoryAlloc (faces * sizeof(*new_mesh->mtl));
	}

	if (lines <= 0)
	{
		new_mesh->line = NULL;
		new_mesh->linematerial = NULL;
	}
	else
	{
		new_mesh->line = (PI3DVector *) PI3DMemoryAlloc (lines * sizeof(*new_mesh->line));
		if (!new_mesh->line )
			return NULL;
		new_mesh->linematerial = (int32 *) PI3DMemoryAlloc (lines * sizeof(*new_mesh->linematerial));
		if (!new_mesh->linematerial )
			return NULL;
	}

	new_mesh->hidden = 0;
	new_mesh->shadow = 1;
	
	new_mesh->isVolume = 0;
	new_mesh->isShadowCatching = 1;
	new_mesh->isShadowCasting = 1;
	new_mesh->isShadowInvisible = 0;
	
	new_mesh->preservation = NULL;
	return new_mesh;
}

/* Create a new animation */
PI3DAnimationData *PI3DCreateAnimation (int32 flags, int32 keys)
{
	PI3DAnimationData *new_anim;

	new_anim = (PI3DAnimationData *) PI3DMemoryAlloc (sizeof(*new_anim));
	if (!new_anim)
		return NULL;
	memset(new_anim,0,sizeof(PI3DAnimationData));
	new_anim->flags = flags;
	new_anim->keys = keys;

	if (keys <= 0)
	{
		new_anim->aFrameData = NULL;
	}
	else
	{
		new_anim->aFrameData = (PI3DFrameVector *) PI3DMemoryAlloc(keys * sizeof(*new_anim->aFrameData));
		if (!new_anim->aFrameData)
			return new_anim;

		for (int x = 0;x< keys; x++)
			memset((void*)&new_anim->aFrameData[x],0,sizeof(*new_anim->aFrameData));
	}

	return new_anim;
}
/******************************************************************************/
#if Macintosh
#pragma mark -
#endif
/******************************************************************************/

/* Add a vertex to a mesh */
Boolean PI3DAddVertexToMesh(PI3DMesh *mesh, PI3DVector v, int32 &maxVertices)
{
	if (mesh->vertices == 0)
	{ // the list is empty - create a starter list
		mesh->vertex = (PI3DVector *)PI3DMemoryAlloc(sizeof(PI3DVector) * maxVertices);
		if (!mesh->vertex)
			return false;
	}
	else if (mesh->vertices == maxVertices)
	{ // the list is full - expand it
		maxVertices *= 2;

		PI3DVector	*newList = (PI3DVector *)PI3DMemoryAlloc(sizeof(PI3DVector) * maxVertices);
		if (newList == NULL)
		{
			maxVertices = mesh->vertices + 100;
			newList = (PI3DVector *)PI3DMemoryAlloc(sizeof(PI3DVector) * maxVertices);
		}
		if (newList == NULL)
			return false;
		for (int32 i = 0; i < mesh->vertices; i++)
		{
			newList[i][0] = mesh->vertex[i][0];
			newList[i][1] = mesh->vertex[i][1];
			newList[i][2] = mesh->vertex[i][2];
		}
		PI3DMemoryFree(mesh->vertex);
		mesh->vertex = newList;
	}
	// switch coordinates around to make OBJ "up" correspond to Photoshop's "up"
	mesh->vertex[mesh->vertices][0] = v[0];
	mesh->vertex[mesh->vertices][1] = -v[2];
	mesh->vertex[mesh->vertices][2] = v[1];
	mesh->vertices++;
	return true;
}
/* Add a normal to a mesh */
Boolean PI3DAddNormalToMesh(PI3DMesh *mesh, PI3DVector n, int32 &maxNormals)
{
	if (mesh->normals == 0)
	{ // the list is empty - create a starter list
		mesh->normal = (PI3DVector *)PI3DMemoryAlloc(sizeof(PI3DVector) * maxNormals);
		if (!mesh->normal)
			return false;
	}
	else if (mesh->normals == maxNormals)
	{ // the list is full - expand it
		maxNormals *= 2;

		PI3DVector	*newList = (PI3DVector *)PI3DMemoryAlloc(sizeof(PI3DVector) * maxNormals);
		if (newList == NULL)
		{
			maxNormals = mesh->normals + 100;
			newList = (PI3DVector *)PI3DMemoryAlloc(sizeof(PI3DVector) * maxNormals);
		}
		if (newList == NULL)
			return false;
		for (int32 i = 0; i < mesh->normals; i++)
		{
			newList[i][0] = mesh->normal[i][0];
			newList[i][1] = mesh->normal[i][1];
			newList[i][2] = mesh->normal[i][2];
		}
		PI3DMemoryFree(mesh->normal);
		mesh->normal = newList;
	}
	// switch coordinates around to make OBJ "up" correspond to Photoshop's "up"
	mesh->normal[mesh->normals][0] = n[0];
	mesh->normal[mesh->normals][1] = -n[2];
	mesh->normal[mesh->normals][2] = n[1];
	mesh->normals++;
	return true;
}
/* Add a texture coordinate (UV) to a mesh */
Boolean PI3DAddUVToMesh(PI3DMesh *mesh, PI3DPoint uv, int32 &maxUVs)
{
	if (mesh->textures == 0)
	{ // the list is empty - create a starter list
		mesh->texture = (PI3DPoint *)PI3DMemoryAlloc(sizeof(PI3DPoint) * maxUVs);
		if (!mesh->texture)
			return false;
	}
	else if (mesh->textures == maxUVs)
	{ // the list is full - expand it
		maxUVs *= 2;

		PI3DPoint	*newList = (PI3DPoint *)PI3DMemoryAlloc(sizeof(PI3DPoint) * maxUVs);
		if (newList == NULL)
		{
			maxUVs = mesh->textures + 100;
			newList = (PI3DPoint *)PI3DMemoryAlloc(sizeof(PI3DPoint) * maxUVs);
		}
		if (newList == NULL)
			return false;
		for (int32 i = 0; i < mesh->textures; i++)
		{
			newList[i][0] = mesh->texture[i][0];
			newList[i][1] = mesh->texture[i][1];
		}
		PI3DMemoryFree(mesh->texture);
		mesh->texture = newList;
	}
	// switch coordinates around to make OBJ "up" correspond to Photoshop's "up"
	mesh->texture[mesh->textures][0] = uv[0];
	mesh->texture[mesh->textures][1] = uv[1];
	mesh->textures++;
	return true;
}

/* Add a face to a mesh */
Boolean PI3DAddFaceToMesh(PI3DMesh *mesh, PI3DFace *f, int32 &maxFaces)
{
	if (mesh->faces == 0)
	{ // the list is empty - create a starter list
		mesh->face = (PI3DFace *)PI3DMemoryAlloc(sizeof(PI3DFace) * maxFaces);
		if (!mesh->face)
			return false;
		memset(mesh->face,0,sizeof(PI3DFace));
	}
	else if (mesh->faces == maxFaces)
	{ // the list is full - expand it
		maxFaces *= 2;

		PI3DFace		*newFaces = (PI3DFace *)PI3DMemoryAlloc(sizeof(PI3DFace) * maxFaces);
		if (newFaces == NULL)
		{
			maxFaces = mesh->faces + 100;
			newFaces = (PI3DFace *)PI3DMemoryAlloc(sizeof(PI3DFace) * maxFaces);
		}
		if (newFaces == NULL)
			return false;
		for (int32 i = 0; i < mesh->faces; i++)
		{
			newFaces[i] = mesh->face[i];
		}
		PI3DMemoryFree(mesh->face);
		mesh->face = newFaces;
	}
	mesh->face[mesh->faces] = *f;
	mesh->faces++;
	return true;
}


/******************************************************************************/
#if Macintosh
#pragma mark -
#endif
/******************************************************************************/

/* Add a new material to the material list */
PI3DMaterial *PI3DUpdateMaterials (PI3DScene *scene, const char *new_material, int ext)
{
	PI3DMaterial *p;

	p = (PI3DMaterial *) PI3DListSearch((PI3DList **)&scene->matPropList, new_material);

	if (p == NULL)
	{
		p = PI3DCreateMaterial();
		strcpy (p->name, new_material);
		//•••p->external = ext;
		PI3DListAdd((PI3DList **)&scene->matPropList, reinterpret_cast<PI3DList *>(p));
	}
	return p;
}

/* Find a material by name in the scene, and return its index number. */
/* If the material is not found, return -1. */
int32 PI3DFindMaterial(PI3DScene *scene, const char *name)
{
	int32		i = 0;
	PI3DMaterial	*p;
	for (p = scene->matPropList; p != NULL; p = (PI3DMaterial *)p->next)
	{
		if (strcmp(p->name, name) == 0)
		{
			return i;
		}
		i++;
	}
	return -1;
}

/* Fetch a material from a scene by its index number. */
PI3DMaterial *PI3DGetMaterial(PI3DScene *scene, int32 index)
{
	int32		i = 0;
	PI3DMaterial	*p;
	for (p = scene->matPropList; p != NULL; p = (PI3DMaterial *)p->next)
	{
		if (i == index)
		{
			return p;
		}
		i++;
	}
	return NULL;
}

/******************************************************************************/
#if Macintosh
#pragma mark -
#endif
/******************************************************************************/
void PI3DInvertMatrix(PI3DMatrix & matrix, PI3DMatrix & mout)
{
	double pos, neg, t;
	PI3DIdentityMatrix(mout);
	// Calculate the determinant of submatrix A and determine if the
	// the matrix is singular as limited by the double precision
	// floating-point data representation.

	pos = neg = 0.0;
	t = matrix[0][0] * matrix[1][1] * matrix[2][2];
	if ( t >= 0.0 )
		pos += t;
	else
		neg += t;

	t = matrix[1][0] * matrix[2][1] * matrix[0][2];
	if ( t >= 0.0 )
		pos += t;
	else
		neg += t;

	t = matrix[2][0] * matrix[0][1] * matrix[1][2];
	if ( t >= 0.0 )
		pos += t;
	else
		neg += t;

	t = -matrix[2][0] * matrix[1][1] * matrix[0][2];
	if ( t >= 0.0 )
		pos += t;
	else
		neg += t;

	t = -matrix[1][0] * matrix[0][1] * matrix[2][2];
	if ( t >= 0.0 )
		pos += t;
	else
		neg += t;

	t = -matrix[0][0] * matrix[2][1] * matrix[1][2];
	if ( t >= 0.0 )
		pos += t;
	else
		neg += t;

	double det = pos + neg;

	// Is the submatrix A singular?
	if ( fabs(det) < ((pos - neg) * 1.0e-12) )
	{
		//Set();
		return ;
	}
	if ( det == 0 )
	{
		//Set();
		return ;

	}
	// Calculate inverse(A) = adj(A) / det(A)

	double det_1 = 1.0 / det;
	mout[0][0] = ( (matrix[1][1] * matrix[2][2] - matrix[2][1] * matrix[1][2] ) * det_1);
	mout[0][1] = ( - (matrix[0][1] * matrix[2][2] - matrix[2][1] * matrix[0][2] ) * det_1);
	mout[0][2] = ( (matrix[0][1] * matrix[1][2] - matrix[1][1] * matrix[0][2] ) * det_1);
	mout[1][0] = ( - (matrix[1][0] * matrix[2][2] - matrix[2][0] * matrix[1][2] ) * det_1);
	mout[1][1] = ( (matrix[0][0] * matrix[2][2] - matrix[2][0] * matrix[0][2] ) * det_1);
	mout[1][2] = ( - (matrix[0][0] * matrix[1][2] - matrix[1][0] * matrix[0][2] ) * det_1);
	mout[2][0] = ( (matrix[1][0] * matrix[2][1] - matrix[2][0] * matrix[1][1] ) * det_1);
	mout[2][1] = ( - (matrix[0][0] * matrix[2][1] - matrix[2][0] * matrix[0][1] ) * det_1);
	mout[2][2] = ( (matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1] ) * det_1);

	/* Calculate -C * inverse(A) */
	mout[3][0] = - (matrix[3][0] * mout[0][0] +
		matrix[3][1] * mout[1][0] +
		matrix[3][2] * mout[2][0] );

	mout[3][1] = - (matrix[3][0] * mout[0][1] +
		matrix[3][1] * mout[1][1] +
		matrix[3][2] * mout[2][1] );


	mout[3][2] = - (matrix[3][0] * mout[0][2] +
		matrix[3][1] * mout[1][2] +
		matrix[3][2] * mout[2][2] );
}

void PI3DTransformPoint(const PI3DVector& p, const PI3DMatrix & matrix, PI3DVector & pout)
{
	pout[0] = (p[0] * matrix[0][0] + p[1] * matrix[1][0] + p[2] * matrix[2][0] + matrix[3][0]);
	pout[1] = (p[0] * matrix[0][1] + p[1] * matrix[1][1] + p[2] * matrix[2][1] + matrix[3][1]);
	pout[2] = (p[0] * matrix[0][2] + p[1] * matrix[1][2] + p[2] * matrix[2][2] + matrix[3][2]);
}

void PI3DCopyMatrix(PI3DMatrix & matrixsrc, PI3DMatrix & matrixdst)
{
	for(uint32 i=0;i<4;i++)
		for(uint32 j=0;j<4;j++)
			matrixdst[i][j] = matrixsrc[i][j];
}

void PI3DIdentityMatrix(PI3DMatrix & matrix)
{
	memset(matrix, 0, sizeof(PI3DMatrix));
	matrix[0][0] = matrix[1][1] = matrix[2][2] = matrix[3][3] = 1.0;
};

void PI3DInvertQuat(const PI3DQuat& quat, PI3DQuat & quatout)
{
	double norm =quat.w * quat.w + quat.x * quat.x + quat.y * quat.y + quat.z * quat.z;
	if ( norm > 0.0f )
	{
		norm = 1.0 / norm;
		quatout.w = (float)(quat.w*norm);
		quatout.x = (float)( -(quat.x*norm));
		quatout.y = (float)( -(quat.y*norm));
		quatout.z = (float)( -(quat.z*norm));
	}
}

void PI3DMultiplyQuat(const PI3DQuat& u, const PI3DQuat& q, PI3DQuat &result)
{
	result.w = u.w*q.w - u.x*q.x - u.y*q.y - u.z*q.z;
	result.x = u.w*q.x + u.x*q.w + u.y*q.z - u.z*q.y;
	result.y = u.w*q.y + u.y*q.w + u.z*q.x - u.x*q.z;
	result.z = u.w*q.z + u.z*q.w + u.x*q.y - u.y*q.x;
};

void PI3DQuatRotateMatrix(const PI3DQuat& quat, PI3DMatrix & matrixout)
{
	nativeFloat wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
	// calculate coefficients
	x2 = quat.x + quat.x;
	y2 = quat.y + quat.y;
	z2 = quat.z + quat.z;
	xx = quat.x * x2;
	xy = quat.x * y2;
	xz = quat.x * z2;
	yy = quat.y * y2;
	yz = quat.y * z2;
	zz = quat.z * z2;
	wx = quat.w * x2;
	wy = quat.w * y2;
	wz = quat.w * z2;

	matrixout[0][0] = 1.0 - (yy + zz);
	matrixout[0][1] = xy - wz;
	matrixout[0][2] = xz + wy;
	matrixout[0][3] = 0.0;

	matrixout[1][0] = xy + wz;
	matrixout[1][1] = 1.0 - (xx + zz);
	matrixout[1][2] = yz - wx;
	matrixout[1][3] = 0.0;

	matrixout[2][0] = xz - wy;
	matrixout[2][1] = yz + wx;
	matrixout[2][2] = 1.0 - (xx + yy);
	matrixout[2][3] = 0.0;

	matrixout[3][0] = 0;
	matrixout[3][1] = 0;
	matrixout[3][2] = 0;
	matrixout[3][3] = 1;

};

void PI3DMatrixMultiply( const PI3DMatrix &_a, const PI3DMatrix & _b, PI3DMatrix & matrixout)
{
	for ( int i = 0; i < 4; i++ )
	{
		for ( int j = 0; j < 4; j++ )
		{
			matrixout[i][j] = (
				_a[i][0] * _b[0][j] +
				_a[i][1] * _b[1][j] +
				_a[i][2] * _b[2][j] +
				_a[i][3] * _b[3][j]);
		}
	}
};


void PI3DTransform(const PI3DMatrix & matrixin1, const PI3DMatrix & matrixin2, PI3DMatrix & matrixout)
{
	PI3DMatrixMultiply(matrixin1, matrixin2, matrixout);
}

void PI3DSetScale(const PI3DVector& s, PI3DMatrix & matrixout)
{
	memset(matrixout, 0, sizeof(PI3DMatrix));
	matrixout[0][0] = s[0];
	matrixout[1][1] = s[1];
	matrixout[2][2] = s[2];
	matrixout[3][3] = 1.0;
}

void PI3DScale(PI3DMatrix & matrixin, const PI3DVector& s, PI3DMatrix & matrixout)
{
	if ( (s[0] == 1.0f) && (s[1] == 1.0f) && (s[2] == 1.0f) )
	{
		PI3DCopyMatrix(matrixin,matrixout);
		return ;
	}
	PI3DMatrix m;
	PI3DSetScale(s,m);
	PI3DTransform(matrixin, m, matrixout);
};

void PI3DSetTranslate(const PI3DVector& s, PI3DMatrix & matrixout)
{
	PI3DIdentityMatrix(matrixout);
	matrixout[3][0] = s[0];
	matrixout[3][1] = s[1];
	matrixout[3][2] = s[2];	
}

void PI3DOffset(const PI3DVector& p, PI3DMatrix & matrix)
{
	matrix[3][0] += p[0];
	matrix[3][1] += p[1];
	matrix[3][2] += p[2];
};

void PI3DTranslate(PI3DMatrix & matrixin, const PI3DVector& s, PI3DMatrix & matrixout)
{
	PI3DMatrix m;
	PI3DSetTranslate(s,m);
	PI3DTransform(matrixin, m, matrixout);
};

void PI3DVectorTransform(const PI3DMatrix & matrix , const PI3DVector & p, PI3DVector & dst)
{
	dst[0] = (nativeFloat)(p[0] * matrix[0][0] + p[1] * matrix[1][0] + p[2] * matrix[2][0]);
	dst[1] = (nativeFloat)(p[0] * matrix[0][1] + p[1] * matrix[1][1] + p[2] * matrix[2][1]);
	dst[2] = (nativeFloat)(p[0] * matrix[0][2] + p[1] * matrix[1][2] + p[2] * matrix[2][2]);
};

void PI3DPositionToMatrix(PI3DVector & position, PI3DVector & angle,PI3DMatrix &matrix)
{
	
    
    PI3DFloat cx = cos( angle[0] * D2R );
	PI3DFloat cy = cos( angle[1] * D2R );
	PI3DFloat cz = cos( angle[2] * D2R );
	PI3DFloat sx = sin( angle[0] * D2R );
	PI3DFloat sy = sin( angle[1] * D2R );
	PI3DFloat sz = sin( angle[2] * D2R );
	
	matrix[0][0] = cy * cz;
	matrix[0][1] = -cy * sz;
	matrix[0][2] = -sy;
	matrix[0][3] = 0;
	
	matrix[1][0] = -sx * sy * cz + cx * sz;
	matrix[1][1] = sx * sy * sz + cx * cz;
	matrix[1][2] = -sx * cy;
	matrix[1][3] = 0;
	
	matrix[2][0] = cx * sy * cz + sx * sz;
	matrix[2][1] = -cx * sy * sz + sx * cz;
	matrix[2][2] = cx * cy;
	matrix[2][3] = 0;
	
	matrix[3][0] = position[0];
	matrix[3][1] = position[1];
	matrix[3][2] = position[2];
	matrix[3][3] = 1;
}

void PI3DMatrixToPosition(PI3DMatrix & matrix,PI3DVector & position, PI3DVector & angle)
{
	angle[1] = -asin( matrix[0][2]);        /* Calculate Y-axis angle */
	double C           =  cos( angle[1] );
	angle[1]    *= R2D;
	double trx,trya;

	if ( fabs( C ) > 0.005 )             /* Gimball lock? */
	{
		trx      =  matrix[2][2] / C;           /* No, so get X-axis angle */
		trya      = -matrix[1][2]  / C;

		angle[0]  = atan2( trya, trx ) * R2D;

		trx      =  matrix[0][0] / C;            /* Get Z-axis angle */
		trya      = -matrix[0][1] / C;

		angle[2]  = atan2( trya, trx ) * R2D;
	}
	else                                 /* Gimball lock has occurred */
	{
		angle[0]  = 0;                      /* Set X-axis angle to zero */

		trx      = matrix[1][1];                 /* And calculate Z-axis angle */
		trya      = matrix[1][0];

		angle[2]  = atan2( trya, trx ) * R2D;
	}

	position[0]=matrix[3][0];
	position[1]=matrix[3][1];
	position[2]=matrix[3][2];
}

nativeFloat PI3DDotProduct(const PI3DVector& v1, const PI3DVector& v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

void PI3DNormalize(PI3DVector & v)
{
	nativeFloat r = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	if ( r == 1.0f )
		return;

	r = sqrtf((float)r);

	if ( r )
	{
		v[0] /= r;
		v[1] /= r;
		v[2] /= r;
	}
	else
	{
		v[0] = v[1] = 0;
		v[2] = 1.0;
	}
};

/*  This code taken from Graphics Gems
 *  http://tog.acm.org/GraphicsGems/gemsiv/polar_decomp/
 *  Ken Shoemake, 1993
 *
 */

/**** Decompose.h - Basic declarations ****/
typedef struct {float x, y, z, w;} Quat; /* Quaternion */
enum QuatPart {X, Y, Z, W};
typedef Quat HVect; /* Homogeneous 3D vector */
typedef float HMatrix[4][4]; /* Right-handed, for column vectors */
typedef struct {
    HVect t;	/* Translation components */
    Quat  q;	/* Essential rotation	  */
    Quat  u;	/* Stretch rotation	  */
    HVect k;	/* Stretch factors	  */
    float f;	/* Sign of determinant	  */
} AffineParts;
float polar_decomp(HMatrix M, HMatrix Q, HMatrix S);
HVect spect_decomp(HMatrix S, HMatrix U);
Quat snuggle(Quat q, HVect *k);
void decomp_affine(HMatrix A, AffineParts *parts);
void invert_affine(AffineParts *parts, AffineParts *inverse);


/******* Matrix Preliminaries *******/

/** Fill out 3x3 matrix to 4x4 **/
#define mat_pad(A) (A[W][X]=A[X][W]=A[W][Y]=A[Y][W]=A[W][Z]=A[Z][W]=0,A[W][W]=1)

/** Copy nxn matrix A to C using "gets" for assignment **/
#define mat_copy(C,gets,A,n) {int i,j; for(i=0;i<n;i++) for(j=0;j<n;j++)\
    C[i][j] gets (A[i][j]);}

/** Copy transpose of nxn matrix A to C using "gets" for assignment **/
#define mat_tpose(AT,gets,A,n) {int i,j; for(i=0;i<n;i++) for(j=0;j<n;j++)\
    AT[i][j] gets (A[j][i]);}

/** Assign nxn matrix C the element-wise combination of A and B using "op" **/
#define mat_binop(C,gets,A,op,B,n) {int i,j; for(i=0;i<n;i++) for(j=0;j<n;j++)\
    C[i][j] gets (A[i][j]) op (B[i][j]);}

/** Multiply the upper left 3x3 parts of A and B to get AB **/
void mat_mult(HMatrix A, HMatrix B, HMatrix AB)
{
    int i, j;
    for (i=0; i<3; i++) for (j=0; j<3; j++)
	AB[i][j] = A[i][0]*B[0][j] + A[i][1]*B[1][j] + A[i][2]*B[2][j];
}

/** Return dot product of length 3 vectors va and vb **/
float vdot(float *va, float *vb)
{
    return (va[0]*vb[0] + va[1]*vb[1] + va[2]*vb[2]);
}

/** Set v to cross product of length 3 vectors va and vb **/
void vcross(float *va, float *vb, float *v)
{
    v[0] = va[1]*vb[2] - va[2]*vb[1];
    v[1] = va[2]*vb[0] - va[0]*vb[2];
    v[2] = va[0]*vb[1] - va[1]*vb[0];
}

/** Set MadjT to transpose of inverse of M times determinant of M **/
void adjoint_transpose(HMatrix M, HMatrix MadjT)
{
    vcross(M[1], M[2], MadjT[0]);
    vcross(M[2], M[0], MadjT[1]);
    vcross(M[0], M[1], MadjT[2]);
}

/******* Quaternion Preliminaries *******/

/* Construct a (possibly non-unit) quaternion from real components. */
Quat Qt_(float x, float y, float z, float w)
{
    Quat qq;
    qq.x = x; qq.y = y; qq.z = z; qq.w = w;
    return (qq);
}

/* Return conjugate of quaternion. */
Quat Qt_Conj(Quat q)
{
    Quat qq;
    qq.x = -q.x; qq.y = -q.y; qq.z = -q.z; qq.w = q.w;
    return (qq);
}

/* Return quaternion product qL * qR.  Note: order is important!
 * To combine rotations, use the product Mul(qSecond, qFirst),
 * which gives the effect of rotating by qFirst then qSecond. */
Quat Qt_Mul(Quat qL, Quat qR)
{
    Quat qq;
    qq.w = qL.w*qR.w - qL.x*qR.x - qL.y*qR.y - qL.z*qR.z;
    qq.x = qL.w*qR.x + qL.x*qR.w + qL.y*qR.z - qL.z*qR.y;
    qq.y = qL.w*qR.y + qL.y*qR.w + qL.z*qR.x - qL.x*qR.z;
    qq.z = qL.w*qR.z + qL.z*qR.w + qL.x*qR.y - qL.y*qR.x;
    return (qq);
}

/* Return product of quaternion q by scalar w. */
Quat Qt_Scale(Quat q, float w)
{
    Quat qq;
    qq.w = q.w*w; qq.x = q.x*w; qq.y = q.y*w; qq.z = q.z*w;
    return (qq);
}

/* Construct a unit quaternion from rotation matrix.  Assumes matrix is
 * used to multiply column vector on the left: vnew = mat vold.	 Works
 * correctly for right-handed coordinate system and right-handed rotations.
 * Translation and perspective components ignored. */
Quat Qt_FromMatrix(HMatrix mat)
{
    /* This algorithm avoids near-zero divides by looking for a large component
     * - first w, then x, y, or z.  When the trace is greater than zero,
     * |w| is greater than 1/2, which is as small as a largest component can be.
     * Otherwise, the largest diagonal entry corresponds to the largest of |x|,
     * |y|, or |z|, one of which must be larger than |w|, and at least 1/2. */
    Quat qu;
    register double tr, s;

    tr = mat[X][X] + mat[Y][Y]+ mat[Z][Z];
    if (tr >= 0.0) {
	    s = sqrt(tr + mat[W][W]);
	    qu.w = (float)(s*0.5);
	    s = 0.5 / s;
	    qu.x = (float)((mat[Z][Y] - mat[Y][Z]) * s);
	    qu.y = (float)((mat[X][Z] - mat[Z][X]) * s);
	    qu.z = (float)((mat[Y][X] - mat[X][Y]) * s);
	} else {
	    int h = X;
	    if (mat[Y][Y] > mat[X][X]) h = Y;
	    if (mat[Z][Z] > mat[h][h]) h = Z;
	    switch (h) {
#define caseMacro(i,j,k,I,J,K) \
	    case I:\
		s = sqrt( (mat[I][I] - (mat[J][J]+mat[K][K])) + mat[W][W] );\
		qu.i = (float)(s*0.5);\
		s = 0.5 / s;\
		qu.j = (float)((mat[I][J] + mat[J][I]) * s);\
		qu.k = (float)((mat[K][I] + mat[I][K]) * s);\
		qu.w = (float)((mat[K][J] - mat[J][K]) * s);\
		break
	    caseMacro(x,y,z,X,Y,Z);
	    caseMacro(y,z,x,Y,Z,X);
	    caseMacro(z,x,y,Z,X,Y);
	    }
	}
    if (mat[W][W] != 1.0) qu = Qt_Scale(qu, 1/sqrt(mat[W][W]));
    return (qu);
}
/******* Decomp Auxiliaries *******/

static HMatrix mat_id = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

/** Compute either the 1 or infinity norm of M, depending on tpose **/
float mat_norm(HMatrix M, int tpose)
{
    int i;
    float sum, max;
    max = 0.0;
    for (i=0; i<3; i++) {
	if (tpose) sum = fabs(M[0][i])+fabs(M[1][i])+fabs(M[2][i]);
	else	   sum = fabs(M[i][0])+fabs(M[i][1])+fabs(M[i][2]);
	if (max<sum) max = sum;
    }
    return max;
}

float norm_inf(HMatrix M) {return mat_norm(M, 0);}
float norm_one(HMatrix M) {return mat_norm(M, 1);}

/** Return index of column of M containing maximum abs entry, or -1 if M=0 **/
int find_max_col(HMatrix M)
{
    float abs, max;
    int i, j, col;
    max = 0.0; col = -1;
    for (i=0; i<3; i++) for (j=0; j<3; j++) {
	abs = M[i][j]; if (abs<0.0) abs = -abs;
	if (abs>max) {max = abs; col = j;}
    }
    return col;
}

/** Setup u for Household reflection to zero all v components but first **/
void make_reflector(float *v, float *u)
{
    float s = sqrt(vdot(v, v));
    u[0] = v[0]; u[1] = v[1];
    u[2] = v[2] + ((v[2]<0.0) ? -s : s);
    s = (float)sqrt(2.0/vdot(u, u));
    u[0] = u[0]*s; u[1] = u[1]*s; u[2] = u[2]*s;
}

/** Apply Householder reflection represented by u to column vectors of M **/
void reflect_cols(HMatrix M, float *u)
{
    int i, j;
    for (i=0; i<3; i++) {
	float s = u[0]*M[0][i] + u[1]*M[1][i] + u[2]*M[2][i];
	for (j=0; j<3; j++) M[j][i] -= u[j]*s;
    }
}
/** Apply Householder reflection represented by u to row vectors of M **/
void reflect_rows(HMatrix M, float *u)
{
    int i, j;
    for (i=0; i<3; i++) {
	float s = vdot(u, M[i]);
	for (j=0; j<3; j++) M[i][j] -= u[j]*s;
    }
}

/** Find orthogonal factor Q of rank 1 (or less) M **/
void do_rank1(HMatrix M, HMatrix Q)
{
    float v1[3], v2[3], s;
    int col;
    mat_copy(Q,=,mat_id,4);
    /* If rank(M) is 1, we should find a non-zero column in M */
    col = find_max_col(M);
    if (col<0) return; /* Rank is 0 */
    v1[0] = M[0][col]; v1[1] = M[1][col]; v1[2] = M[2][col];
    make_reflector(v1, v1); reflect_cols(M, v1);
    v2[0] = M[2][0]; v2[1] = M[2][1]; v2[2] = M[2][2];
    make_reflector(v2, v2); reflect_rows(M, v2);
    s = M[2][2];
    if (s<0.0) Q[2][2] = -1.0;
    reflect_cols(Q, v1); reflect_rows(Q, v2);
}

/** Find orthogonal factor Q of rank 2 (or less) M using adjoint transpose **/
void do_rank2(HMatrix M, HMatrix MadjT, HMatrix Q)
{
    float v1[3], v2[3];
    float w, x, y, z, c, s, d;
    int col;
    /* If rank(M) is 2, we should find a non-zero column in MadjT */
    col = find_max_col(MadjT);
    if (col<0) {do_rank1(M, Q); return;} /* Rank<2 */
    v1[0] = MadjT[0][col]; v1[1] = MadjT[1][col]; v1[2] = MadjT[2][col];
    make_reflector(v1, v1); reflect_cols(M, v1);
    vcross(M[0], M[1], v2);
    make_reflector(v2, v2); reflect_rows(M, v2);
    w = M[0][0]; x = M[0][1]; y = M[1][0]; z = M[1][1];
    if (w*z>x*y) {
	c = z+w; s = y-x; d = sqrt(c*c+s*s); c = c/d; s = s/d;
	Q[0][0] = Q[1][1] = c; Q[0][1] = -(Q[1][0] = s);
    } else {
	c = z-w; s = y+x; d = sqrt(c*c+s*s); c = c/d; s = s/d;
	Q[0][0] = -(Q[1][1] = c); Q[0][1] = Q[1][0] = s;
    }
    Q[0][2] = Q[2][0] = Q[1][2] = Q[2][1] = 0.0; Q[2][2] = 1.0;
    reflect_cols(Q, v1); reflect_rows(Q, v2);
}


/******* Polar Decomposition *******/

/* Polar Decomposition of 3x3 matrix in 4x4,
 * M = QS.  See Nicholas Higham and Robert S. Schreiber,
 * Fast Polar Decomposition of An Arbitrary Matrix,
 * Technical Report 88-942, October 1988,
 * Department of Computer Science, Cornell University.
 */
float polar_decomp(HMatrix M, HMatrix Q, HMatrix S)
{
#define TOL 1.0e-6
    HMatrix Mk, MadjTk, Ek;
    float det, M_one, M_inf, MadjT_one, MadjT_inf, E_one, gamma, g1, g2;
    int i, j;
    mat_tpose(Mk,=,M,3);
    M_one = norm_one(Mk);  M_inf = norm_inf(Mk);
    do {
	adjoint_transpose(Mk, MadjTk);
	det = vdot(Mk[0], MadjTk[0]);
	if (det==0.0) {do_rank2(Mk, MadjTk, Mk); break;}
	MadjT_one = norm_one(MadjTk); MadjT_inf = norm_inf(MadjTk);
	gamma = sqrt(sqrt((MadjT_one*MadjT_inf)/(M_one*M_inf))/fabs(det));
	g1 = (float)(gamma*0.5);
	g2 = (float)(0.5/(gamma*det));
	mat_copy(Ek,=,Mk,3);
	mat_binop(Mk,=,g1*Mk,+,g2*MadjTk,3);
	mat_copy(Ek,-=,Mk,3);
	E_one = norm_one(Ek);
	M_one = norm_one(Mk);  M_inf = norm_inf(Mk);
    } while (E_one>(M_one*TOL));
    mat_tpose(Q,=,Mk,3); mat_pad(Q);
    mat_mult(Mk, M, S);	 mat_pad(S);
    for (i=0; i<3; i++) for (j=i; j<3; j++)
	S[i][j] = S[j][i] = (float)0.5*(S[i][j]+S[j][i]);
    return (det);
}


/******* Spectral Decomposition *******/

/* Compute the spectral decomposition of symmetric positive semi-definite S.
 * Returns rotation in U and scale factors in result, so that if K is a diagonal
 * matrix of the scale factors, then S = U K (U transpose). Uses Jacobi method.
 * See Gene H. Golub and Charles F. Van Loan. Matrix Computations. Hopkins 1983.
 */
HVect spect_decomp(HMatrix S, HMatrix U)
{
    HVect kv;
    double Diag[3],OffD[3]; /* OffD is off-diag (by omitted index) */
    double g,h,fabsh,fabsOffDi,t,theta,c,s,tau,ta,OffDq,a,b;
    static char nxt[] = {Y,Z,X};
    int sweep, i, j;
    mat_copy(U,=,mat_id,4);
    Diag[X] = S[X][X]; Diag[Y] = S[Y][Y]; Diag[Z] = S[Z][Z];
    OffD[X] = S[Y][Z]; OffD[Y] = S[Z][X]; OffD[Z] = S[X][Y];
    for (sweep=20; sweep>0; sweep--) {
	float sm = (float)(fabs(OffD[X])+fabs(OffD[Y])+fabs(OffD[Z]));
	if (sm==0.0) break;
	for (i=Z; i>=X; i--) {
	    int p = nxt[i]; int q = nxt[p];
	    fabsOffDi = fabs(OffD[i]);
	    g = 100.0*fabsOffDi;
	    if (fabsOffDi>0.0) {
		h = Diag[q] - Diag[p];
		fabsh = fabs(h);
		if (fabsh+g==fabsh) {
		    t = OffD[i]/h;
		} else {
		    theta = 0.5*h/OffD[i];
		    t = 1.0/(fabs(theta)+sqrt(theta*theta+1.0));
		    if (theta<0.0) t = -t;
		}
		c = 1.0/sqrt(t*t+1.0); s = t*c;
		tau = s/(c+1.0);
		ta = t*OffD[i]; OffD[i] = 0.0;
		Diag[p] -= ta; Diag[q] += ta;
		OffDq = OffD[q];
		OffD[q] -= s*(OffD[p] + tau*OffD[q]);
		OffD[p] += s*(OffDq   - tau*OffD[p]);
		for (j=Z; j>=X; j--) {
		    a = U[j][p]; b = U[j][q];
		    U[j][p] -= (float)(s*(b + tau*a));
		    U[j][q] += (float)(s*(a - tau*b));
		}
	    }
	}
    }
    kv.x = (float)Diag[X]; kv.y = (float)Diag[Y]; kv.z = (float)Diag[Z]; kv.w = 1.0;
    return (kv);
}

/******* Spectral Axis Adjustment *******/

/* Given a unit quaternion, q, and a scale vector, k, find a unit quaternion, p,
 * which permutes the axes and turns freely in the plane of duplicate scale
 * factors, such that q p has the largest possible w component, i.e. the
 * smallest possible angle. Permutes k's components to go with q p instead of q.
 * See Ken Shoemake and Tom Duff. Matrix Animation and Polar Decomposition.
 * Proceedings of Graphics Interface 1992. Details on p. 262-263.
 */
Quat snuggle(Quat q, HVect *k)
{
#define SQRTHALF (0.7071067811865475244f)
#define sgn(n,v)    ((n)?-(v):(v))
#define swap(a,i,j) {a[3]=a[i]; a[i]=a[j]; a[j]=a[3];}
#define cycle(a,p)  if (p) {a[3]=a[0]; a[0]=a[1]; a[1]=a[2]; a[2]=a[3];}\
		    else   {a[3]=a[2]; a[2]=a[1]; a[1]=a[0]; a[0]=a[3];}
    Quat p;
    float ka[4];
    int i, turn = -1;
    ka[X] = k->x; ka[Y] = k->y; ka[Z] = k->z;
    if (ka[X]==ka[Y]) {if (ka[X]==ka[Z]) turn = W; else turn = Z;}
    else {if (ka[X]==ka[Z]) turn = Y; else if (ka[Y]==ka[Z]) turn = X;}
    if (turn>=0) {
	Quat qtoz, qp;
	unsigned neg[3], win;
	double mag[3], t;
	static Quat qxtoz = {0,SQRTHALF,0,SQRTHALF};
	static Quat qytoz = {SQRTHALF,0,0,SQRTHALF};
	static Quat qppmm = { 0.5, 0.5,-0.5,-0.5};
	static Quat qpppp = { 0.5, 0.5, 0.5, 0.5};
	static Quat qmpmm = {-0.5, 0.5,-0.5,-0.5};
	static Quat qpppm = { 0.5, 0.5, 0.5,-0.5};
	static Quat q0001 = { 0.0, 0.0, 0.0, 1.0};
	static Quat q1000 = { 1.0, 0.0, 0.0, 0.0};
	switch (turn) {
	default: return (Qt_Conj(q));
	case X: q = Qt_Mul(q, qtoz = qxtoz); swap(ka,X,Z) break;
	case Y: q = Qt_Mul(q, qtoz = qytoz); swap(ka,Y,Z) break;
	case Z: qtoz = q0001; break;
	}
	q = Qt_Conj(q);
	mag[0] = (double)q.z*q.z+(double)q.w*q.w-0.5;
	mag[1] = (double)q.x*q.z-(double)q.y*q.w;
	mag[2] = (double)q.y*q.z+(double)q.x*q.w;
	for (i=0; i<3; i++) if (neg[i] = (mag[i]<0.0)) mag[i] = -mag[i];
	if (mag[0]>mag[1]) {if (mag[0]>mag[2]) win = 0; else win = 2;}
	else		   {if (mag[1]>mag[2]) win = 1; else win = 2;}
	switch (win) {
	case 0: if (neg[0]) p = q1000; else p = q0001; break;
	case 1: if (neg[1]) p = qppmm; else p = qpppp; cycle(ka,0) break;
	case 2: if (neg[2]) p = qmpmm; else p = qpppm; cycle(ka,1) break;
	}
	qp = Qt_Mul(q, p);
	t = sqrt(mag[win]+0.5);
	p = Qt_Mul(p, Qt_((float)0.0,(float)0.0,(float)(-qp.z/t),(float)(qp.w/t)));
	p = Qt_Mul(qtoz, Qt_Conj(p));
    } else {
	float qa[4], pa[4];
	unsigned lo, hi, neg[4], par = 0;
	double all, big, two;
	qa[0] = q.x; qa[1] = q.y; qa[2] = q.z; qa[3] = q.w;
	for (i=0; i<4; i++) {
	    pa[i] = 0.0;
	    if (neg[i] = (qa[i]<0.0)) qa[i] = -qa[i];
	    par ^= neg[i];
	}
	/* Find two largest components, indices in hi and lo */
	if (qa[0]>qa[1]) lo = 0; else lo = 1;
	if (qa[2]>qa[3]) hi = 2; else hi = 3;
	if (qa[lo]>qa[hi]) {
	    if (qa[lo^1]>qa[hi]) {hi = lo; lo ^= 1;}
	    else {hi ^= lo; lo ^= hi; hi ^= lo;}
	} else {if (qa[hi^1]>qa[lo]) lo = hi^1;}
	all = (qa[0]+qa[1]+qa[2]+qa[3])*0.5;
	two = (qa[hi]+qa[lo])*SQRTHALF;
	big = qa[hi];
	if (all>two) {
	    if (all>big) {/*all*/
		{int i; for (i=0; i<4; i++) pa[i] = (float)sgn(neg[i], 0.5);}
		cycle(ka,par)
	    } else {/*big*/ pa[hi] = (float)sgn(neg[hi],1.0);}
	} else {
	    if (two>big) {/*two*/
		pa[hi] = sgn(neg[hi],SQRTHALF); pa[lo] = sgn(neg[lo], SQRTHALF);
		if (lo>hi) {hi ^= lo; lo ^= hi; hi ^= lo;}
		if (hi==W) {hi = "\001\002\000"[lo]; lo = 3-hi-lo;}
		swap(ka,hi,lo)
	    } else {/*big*/ pa[hi] = (float)sgn(neg[hi],1.0);}
	}
	p.x = -pa[0]; p.y = -pa[1]; p.z = -pa[2]; p.w = pa[3];
    }
    k->x = ka[X]; k->y = ka[Y]; k->z = ka[Z];
    return (p);
}


/******* Decompose Affine Matrix *******/

/* Decompose 4x4 affine matrix A as TFRUK(U transpose), where t contains the
 * translation components, q contains the rotation R, u contains U, k contains
 * scale factors, and f contains the sign of the determinant.
 * Assumes A transforms column vectors in right-handed coordinates.
 * See Ken Shoemake and Tom Duff. Matrix Animation and Polar Decomposition.
 * Proceedings of Graphics Interface 1992.
 */
void decomp_affine(HMatrix A, AffineParts *parts)
{
    HMatrix Q, S, U;
    Quat p;
    float det;
    parts->t = Qt_(A[X][W], A[Y][W], A[Z][W], 0);
    det = polar_decomp(A, Q, S);
    if (det<0.0) {
	mat_copy(Q,=,-Q,3);
	parts->f = -1;
    } else parts->f = 1;
    parts->q = Qt_FromMatrix(Q);
    parts->k = spect_decomp(S, U);
    parts->u = Qt_FromMatrix(U);
    p = snuggle(parts->u, &parts->k);
    parts->u = Qt_Mul(parts->u, p);
}

/******* Invert Affine Decomposition *******/

/* Compute inverse of affine decomposition.
 */
void invert_affine(AffineParts *parts, AffineParts *inverse)
{
    Quat t, p;
    inverse->f = parts->f;
    inverse->q = Qt_Conj(parts->q);
    inverse->u = Qt_Mul(parts->q, parts->u);
    inverse->k.x = (float)((parts->k.x==0.0) ? 0.0 : 1.0/parts->k.x);
    inverse->k.y = (float)((parts->k.y==0.0) ? 0.0 : 1.0/parts->k.y);
    inverse->k.z = (float)((parts->k.z==0.0) ? 0.0 : 1.0/parts->k.z);
    inverse->k.w = parts->k.w;
    t = Qt_(-parts->t.x, -parts->t.y, -parts->t.z, 0);
    t = Qt_Mul(Qt_Conj(inverse->u), Qt_Mul(t, inverse->u));
    t = Qt_(inverse->k.x*t.x, inverse->k.y*t.y, inverse->k.z*t.z, 0);
    p = Qt_Mul(inverse->q, inverse->u);
    t = Qt_Mul(p, Qt_Mul(t, Qt_Conj(p)));
    inverse->t = (inverse->f>0.0) ? t : Qt_(-t.x, -t.y, -t.z, 0);
}

void PI3DMatrixToVectorAngle(PI3DMatrix & matrix,PI3DVector & position, PI3DVector & angle)
{
	angle[1] = -asin( matrix[0][2]);        /* Calculate Y-axis angle */
	PI3DFloat C           =  cos( angle[1] );
	angle[1]    *= R2D;
	PI3DFloat trx,trya;

	if ( fabs( C ) > 0.005 )             /* Gimball lock? */
	{
		trx      =  matrix[2][2] / C;           /* No, so get X-axis angle */
		trya      = -matrix[1][2]  / C;

		angle[0]  = atan2( trya, trx ) * R2D;

		trx      =  matrix[0][0] / C;            /* Get Z-axis angle */
		trya      = -matrix[0][1] / C;

		angle[2]  = atan2( trya, trx ) * R2D;
	}
	else                                 /* Gimball lock has occurred */
	{
		angle[0]  = 0;                      /* Set X-axis angle to zero */

		trx      = matrix[1][1];                 /* And calculate Z-axis angle */
		trya      = matrix[1][0];

		angle[2]  = atan2( trya, trx ) * R2D;
	}

	position[0]=matrix[3][0];
	position[1]=matrix[3][1];
	position[2]=matrix[3][2];
}

#define FLOAT_EQUALS(A, B) (fabs( (float) (A) - (float) (B) ) < 1e-15f)

bool PI3DIsIdentityMatrix(PI3DMatrix & matrix)
{
	if( FLOAT_EQUALS(matrix[0][0],1.0f) && FLOAT_EQUALS(matrix[0][1],0.0f) && FLOAT_EQUALS(matrix[0][2],0.0f) && FLOAT_EQUALS(matrix[0][3],0.0f) &&
		FLOAT_EQUALS(matrix[1][0],0.0f) && FLOAT_EQUALS(matrix[1][1],1.0f) && FLOAT_EQUALS(matrix[1][2],0.0f) && FLOAT_EQUALS(matrix[1][3],0.0f) &&
		FLOAT_EQUALS(matrix[2][0],0.0f) && FLOAT_EQUALS(matrix[2][1],0.0f) && FLOAT_EQUALS(matrix[2][2],1.0f) && FLOAT_EQUALS(matrix[2][3],0.0f) &&
		FLOAT_EQUALS(matrix[3][0],0.0f) && FLOAT_EQUALS(matrix[3][1],0.0f) && FLOAT_EQUALS(matrix[3][2],0.0f) && FLOAT_EQUALS(matrix[3][3],1.0f)
		)
		return true;
	return false;
}

void PI3DMatrixToPosition(const PI3DMatrix &matrix,PI3DPosition *position)
	{
	
	position->yAngle = -asin( matrix[0][2]);        /* Calculate Y-axis angle */
    PI3DFloat C           =  cos( position->yAngle );
    position->yAngle    *= R2D;
	PI3DFloat trx,trya;
	
    if ( fabs( C ) > 0.005 )             /* Gimball lock? */
      {
      trx      =  matrix[2][2] / C;           /* No, so get X-axis angle */
      trya      = -matrix[1][2]  / C;

      position->xAngle  = atan2( trya, trx ) * R2D;

      trx      =  matrix[0][0] / C;            /* Get Z-axis angle */
      trya      = -matrix[0][1] / C;

      position->zAngle  = atan2( trya, trx ) * R2D;
      }
    else                                 /* Gimball lock has occurred */
      {
      position->xAngle  = 0;                      /* Set X-axis angle to zero */

      trx      = matrix[1][1];                 /* And calculate Z-axis angle */
      trya      = matrix[1][0];

      position->zAngle  = atan2( trya, trx ) * R2D;
      }
		
	position->x=matrix[3][0];
	position->y=matrix[3][1];
	position->z=matrix[3][2];
	
	}

/******************************************************************************/
void	SetMatrixToIdentity(PI3DMatrix &matrix)
	{
	memset(&matrix, 0, sizeof(PI3DMatrix));
	matrix[0][0] = matrix[1][1] = matrix[2][2] = matrix[3][3] = 1.0;
	}

/******************************************************************************/
void	SetMatrixToRotateZ(PI3DMatrix &matrix, PI3DFloat angleZRad)
	{
	SetMatrixToIdentity(matrix);
	PI3DFloat Cosine = cos(angleZRad);
	PI3DFloat Sine = sin(angleZRad);
	matrix[0][0] = Cosine;
	matrix[0][1] = Sine;
	matrix[1][0] = -Sine;
	matrix[1][1] = Cosine;	
	}

bool PI3DDecompose(PI3DMatrix & min, PI3DVector & position, PI3DQuat & rotation, PI3DVector & scale)
{
	HMatrix hm;
	//convert to HMatrix
	for(uint32 i=0;i<4;i++)
		for(uint32 j=0;j<4;j++)
			hm[i][j] = min[i][j];
	AffineParts _parts;
	//decompose
	decomp_affine(hm, &_parts);
	PI3DQuat in,out;
	in.w = _parts.q.w;
	in.x = _parts.q.x;
	in.y = _parts.q.y;
	in.z = _parts.q.z;
	PI3DInvertQuat(in,out);

	//set our outputs
	position[0] = _parts.t.x;
	position[1] = _parts.t.y;
	position[2] = _parts.t.z;
	rotation.w = out.w;
	rotation.x = out.x;
	rotation.y = out.y;
	rotation.z = out.z;
	scale[0] = _parts.k.x;
	scale[1] = _parts.k.y;
	scale[2] = _parts.k.z;

	return true;
}



/******************************************************************************/

void PI3DCleanupName (char *name)
{
	char *tmp = (char *) PI3DMemoryAlloc (strlen(name)+2);
	if (!tmp)
		return;

	int  i;

	/* Remove any leading blanks or quotes */
	i = 0;
	while ((name[i] == ' ' || name[i] == '"') && name[i] != '\0')
	{
		i++;
	}
	strcpy (tmp, &name[i]);

	/* Remove any trailing blanks or quotes */
	for (i = static_cast<int>(strlen(tmp))-1; i >= 0; i--)
	{
		if (isprint(tmp[i]) && !isspace(tmp[i]) && tmp[i] != '"')
		{
			break;
		}
		else
		{
			tmp[i] = '\0';
		}
	}

	strcpy (name, tmp);

	/* Prefix the letter 'N' to materials that begin with a digit */
	if (!isdigit (name[0]))
	{
		strcpy (tmp, name);
	}
	else 
	{
		tmp[0] = 'N';
		strcpy (&tmp[1], name);
	}

	/* Replace all illegal charaters in name with underscores */
	for (i = 0; tmp[i] != '\0'; i++)
	{
		if (!isalnum(tmp[i]))
		{
			tmp[i] = '_';
		}
	}

	strcpy (name, tmp);

	PI3DMemoryFree (tmp);
}

PI3DScene *PI3DCreateScene(void *userData)
{
	PI3DImport *theImporter=(PI3DImport*)PI3DMemoryAlloc(sizeof(PI3DImport));
	if (!theImporter)
		return NULL;

	theImporter->userData=userData;
	PI3DScene *scene = (PI3DScene*)PI3DMemoryAlloc(sizeof(PI3DScene));
	if (!scene)
	{
		PI3DMemoryFree(theImporter);
		return NULL;
	}

	memset(scene,0,sizeof(PI3DScene));
	PI3DSceneInitGlobals(scene);
	theImporter->scene=scene;
	if (PI3DParseFile (theImporter) == 0)
	{
/*#ifdef DEBUG
		wchar_t message[256];
		size_t copiedsize = mbstowcs(message, chunkErrorMsg, 256);
		pContext->Message(0,message);
#endif*/
		PI3DMemoryFree(scene);
		PI3DMemoryFree(theImporter);
		return 0;
	}
	PI3DMemoryFree(theImporter);
	return scene;

}

void PI3DKillPreservation(PI3DPreservation *preservation)
{
	if(!preservation)
		return;
	
	if(preservation->Id)
		PI3DMemoryFree(preservation->Id);
	if(preservation->SubId)
		PI3DMemoryFree(preservation->SubId);
	if(preservation->Name)
		PI3DMemoryFree(preservation->Name);
	if(preservation->Extra)
		PI3DMemoryFree(preservation->Extra);
	if(preservation->Controller)
		PI3DMemoryFree(preservation->Controller);
	if(preservation->Animations)
		PI3DMemoryFree(preservation->Animations);
	if(preservation->EffectProfiles)
		PI3DMemoryFree(preservation->EffectProfiles);
	
	//memset(preservation, 0, sizeof(PI3DPreservation));
	PI3DMemoryFree(preservation);
	preservation = NULL;
}
void PI3DKillMesh(PI3DMesh *mesh)
{
	if (mesh->vertex)
	{
		PI3DMemoryFree (mesh->vertex);
	}

	if (mesh->normal)
	{
		PI3DMemoryFree (mesh->normal);
	}
	
	if (mesh->face)
	{
		for(int i=0;i<mesh->faces;i++)
		{
			if (mesh->face[i].normals && (mesh->face[i].normals != mesh->face[i].points)) PI3DMemoryFree(mesh->face[i].normals);
			if (mesh->face[i].textures && (mesh->face[i].textures != mesh->face[i].points)) PI3DMemoryFree(mesh->face[i].textures);
			if (mesh->face[i].colors && (mesh->face[i].colors != mesh->face[i].points)) PI3DMemoryFree(mesh->face[i].colors);
			if (mesh->face[i].points) PI3DMemoryFree(mesh->face[i].points);
		}
		
		PI3DMemoryFree (mesh->face);
	}
	if (mesh->texture)
	{
		PI3DMemoryFree (mesh->texture);
	}
	if (mesh->color)
	{
		PI3DMemoryFree (mesh->color);
	}
	if (mesh->linecolors)
	{
		PI3DMemoryFree (mesh->linecolors);
	}
	if (mesh->line)
	{
		PI3DMemoryFree (mesh->line);
	}
	if (mesh->linematerial)
	{
		PI3DMemoryFree (mesh->linematerial);
	}
	
	if(mesh->preservation)
	{
		PI3DKillPreservation(mesh->preservation);
		mesh->preservation = NULL;
	}
}

void PI3DKillScene(PI3DScene *scene)
{
	if(!scene)
		return;
	
	//Taking this out since the data is now being passed in
	// walk the light list and delete PI3D objects
	PI3DLight *light;
	for (light = scene->lightList; light != (PI3DLight *) NULL; light = (PI3DLight *) light->next)
		if(light->preservation)
		{
			PI3DKillPreservation(light->preservation);
			light->preservation = NULL;
		}
	
	PI3DCamera *camera;
	for (camera = scene->cameraList; camera != (PI3DCamera *) NULL; camera = (PI3DCamera *) camera->next)
		if(camera->preservation)
		{
			PI3DKillPreservation(camera->preservation);
			camera->preservation = NULL;
		}
	
	PI3DMaterial *material;
	for (material = scene->matPropList; material != (PI3DMaterial *) NULL; material = (PI3DMaterial *) material->next)
		if(material->preservation)
		{
			PI3DKillPreservation(material->preservation);
			material->preservation = NULL;
		}
	
	if(scene->lightList)
		PI3DListDelete((PI3DList **)&scene->lightList);
	scene->lightList = NULL;
	
	// walk the camera list and delete PI3D objects
	if(scene->cameraList)
		PI3DListDelete((PI3DList **)&scene->cameraList);
	scene->cameraList = NULL;
	
	// walk the mesh list and delete malloced datra and PI3D objects
	PI3DMesh *mesh;
	for (mesh = scene->meshList; mesh != (PI3DMesh *) NULL; 
		 mesh = (PI3DMesh *) mesh->next)
	{
		
		PI3DKillMesh(mesh);
		
	}
	if(scene->meshList)
		PI3DListDelete((PI3DList **)&scene->meshList);
	scene->meshList = NULL;
	
	// walk the mesh list and delete malloced datra and PI3D objects
	PI3DKeyFrame* keyframe;
	for (keyframe = scene->keyFrameList; keyframe != (PI3DKeyFrame *) NULL; 
		 keyframe = (PI3DKeyFrame *) keyframe->next)
	{
		if (keyframe->positionData)
		{
			if(keyframe->positionData->aFrameData)
				PI3DMemoryFree (keyframe->positionData->aFrameData);
			
			PI3DListDelete((PI3DList **)&keyframe->positionData);
		}
		if (keyframe->rotationData)
		{
			if(keyframe->rotationData->aFrameData)
				PI3DMemoryFree (keyframe->rotationData->aFrameData);
			PI3DListDelete((PI3DList **)&keyframe->rotationData);
		}
		if (keyframe->scaleData)
		{
			if(keyframe->scaleData->aFrameData)
				PI3DMemoryFree (keyframe->scaleData->aFrameData);
			PI3DListDelete((PI3DList **)&keyframe->scaleData);
		}
		if (keyframe->rollData)
		{
			if(keyframe->rollData->aFrameData)
				PI3DMemoryFree (keyframe->rollData->aFrameData);
			PI3DListDelete((PI3DList **)&keyframe->rollData);
		}
		if (keyframe->fOVData)
		{
			if(keyframe->fOVData->aFrameData)
				PI3DMemoryFree (keyframe->fOVData->aFrameData);
			PI3DListDelete((PI3DList **)&keyframe->fOVData);
		}
		if (keyframe->hotData)
		{
			if(keyframe->hotData->aFrameData)
				PI3DMemoryFree (keyframe->hotData->aFrameData);
			PI3DListDelete((PI3DList **)&keyframe->hotData);
		}
		if (keyframe->colData)
		{
			if(keyframe->colData->aFrameData)
				PI3DMemoryFree (keyframe->colData->aFrameData);
			PI3DListDelete((PI3DList **)&keyframe->colData);
		}
		if (keyframe->fallData)
		{
			if(keyframe->fallData->aFrameData)
				PI3DMemoryFree (keyframe->fallData->aFrameData);
			PI3DListDelete((PI3DList **)&keyframe->fallData);
		}
		if (keyframe->hideData)
		{
			if(keyframe->hideData->aFrameData)
				PI3DMemoryFree (keyframe->hideData->aFrameData);
			PI3DListDelete((PI3DList **)&keyframe->hideData);
		}
		if (keyframe->morphData)
		{
			if(keyframe->morphData->aFrameData)
				PI3DMemoryFree (keyframe->morphData->aFrameData);
			PI3DListDelete((PI3DList **)&keyframe->morphData);
		}
		
		if(keyframe->preservation)
		{
			PI3DKillPreservation(keyframe->preservation);
			keyframe->preservation = NULL;
		}
	}
	if(scene->keyFrameList)
		PI3DListDelete((PI3DList **)&scene->keyFrameList);
	scene->keyFrameList = NULL;
	
	if(scene->matPropList)
		PI3DListDelete((PI3DList **)&scene->matPropList);
	scene->matPropList = NULL;
	
	if (scene->geoReference)
		PI3DMemoryFree(scene->geoReference);
	
	PI3DMemoryFree( scene);
	
}
short	PI3DParsePreservationIntoDescriptor(PI3DDescriptorProcs *procs, PI3DPreservation	*preservation, PIActionDescriptor* pReturnDescriptor)
{
	PSActionDescriptorProcs		*actionDescriptorProcs=procs->actionDescriptorProcs;
	PSBasicActionControlProcs	*actionControlProcs=procs->actionControlProcs;
	
	DescriptorTypeID typeID;
	char		emptyString[1]={0};
	
	PIActionDescriptor preservationDescriptor;
	actionDescriptorProcs->Make(&preservationDescriptor);
	
	//Id
	actionControlProcs->StringIDToTypeID(keyPreservationId,&typeID);
	if (preservation->Id)
		actionDescriptorProcs->PutString(preservationDescriptor,typeID,preservation->Id);
	else
		actionDescriptorProcs->PutString(preservationDescriptor,typeID,emptyString);
	
	//Id
	actionControlProcs->StringIDToTypeID(keyPreservationSubId,&typeID);
	if (preservation->SubId)
		actionDescriptorProcs->PutString(preservationDescriptor,typeID,preservation->SubId);
	else
		actionDescriptorProcs->PutString(preservationDescriptor,typeID,emptyString);

	//Id
	actionControlProcs->StringIDToTypeID(keyPreservationName,&typeID);
	if (preservation->Name)
		actionDescriptorProcs->PutString(preservationDescriptor,typeID,preservation->Name);
	else
		actionDescriptorProcs->PutString(preservationDescriptor,typeID,emptyString);

		
	//Extra
	actionControlProcs->StringIDToTypeID(keyPreservationExtra,&typeID);
	if (preservation->Extra)
		actionDescriptorProcs->PutData(preservationDescriptor,typeID,strlen((char*)preservation->Extra) * sizeof(uint8),preservation->Extra);
	
	//Flags
	actionControlProcs->StringIDToTypeID(keyPreservationFlags,&typeID);
	actionDescriptorProcs->PutInteger(preservationDescriptor,typeID, preservation->Flags);
	
	//Controller
	actionControlProcs->StringIDToTypeID(keyPreservationController,&typeID);
	if (preservation->Controller)
		actionDescriptorProcs->PutData(preservationDescriptor,typeID,strlen((char*)preservation->Controller) * sizeof(uint8),preservation->Controller);
	
	//Animations
	actionControlProcs->StringIDToTypeID(keyPreservationAnimations,&typeID);
	if (preservation->Animations)
		actionDescriptorProcs->PutData(preservationDescriptor,typeID,strlen((char*)preservation->Animations) * sizeof(uint8),preservation->Animations);
	
	//EffectProfiles
	actionControlProcs->StringIDToTypeID(keyPreservationEffectProfiles,&typeID);
	if (preservation->EffectProfiles)
		actionDescriptorProcs->PutData(preservationDescriptor,typeID,strlen((char*)preservation->EffectProfiles) * sizeof(uint8),preservation->EffectProfiles);

	*pReturnDescriptor=preservationDescriptor;

	return 1;
}

short	PI3DParseDescriptorIntoPreservation(PI3DDescriptorProcs *procs, PIActionDescriptor actionDescriptor, PI3DPreservation	**preservation)
{
	PSActionDescriptorProcs		*actionDescriptorProcs=procs->actionDescriptorProcs;
	PSBasicActionControlProcs	*actionControlProcs=procs->actionControlProcs;
	
	DescriptorTypeID typeID;
	Boolean hasKey;
	uint32 len;

	PI3DPreservation *Preservation = (PI3DPreservation*)PI3DMemoryAlloc (sizeof (PI3DPreservation));
	
	if(!Preservation)
		return 0;
	
	memset(Preservation,0,sizeof(PI3DPreservation));
	*preservation=Preservation;
	//Id
	actionControlProcs->StringIDToTypeID(keyPreservationId,&typeID);
	actionDescriptorProcs->HasKey(actionDescriptor, typeID, &hasKey);
	if(hasKey)
	{
		actionDescriptorProcs->GetStringLength(actionDescriptor, typeID, &len);
		if (len)
		{
			 Preservation->Id = (char*)PI3DMemoryAlloc((len+1) * sizeof(char));
			if(!Preservation->Id) return 0;
			actionDescriptorProcs->GetString(actionDescriptor,typeID,Preservation->Id, len+1);
			Preservation->Id[len] = '\0';
		}
		else
			Preservation->Id = NULL;
	}
	
	//SubId
	actionControlProcs->StringIDToTypeID(keyPreservationSubId,&typeID);
	actionDescriptorProcs->HasKey(actionDescriptor, typeID, &hasKey);
	if(hasKey)
	{
		actionDescriptorProcs->GetStringLength(actionDescriptor, typeID, &len);
		if (len)
		{
			Preservation->SubId = (char*)PI3DMemoryAlloc((len+1) * sizeof(char));
			if(!Preservation->SubId) return 0;
			actionDescriptorProcs->GetString(actionDescriptor,typeID,Preservation->SubId, len+1);
			Preservation->SubId[len] = '\0';
		}
		else
			Preservation->SubId = NULL;
	}
	
	//Name
	actionControlProcs->StringIDToTypeID(keyPreservationName,&typeID);
	actionDescriptorProcs->HasKey(actionDescriptor, typeID, &hasKey);
	if(hasKey)
	{
		actionDescriptorProcs->GetStringLength(actionDescriptor, typeID, &len);
		if (len)
		{
			Preservation->Name = (char*)PI3DMemoryAlloc((len+1) * sizeof(uint16));
			if(!Preservation->Name) return 0;
			actionDescriptorProcs->GetString(actionDescriptor,typeID,Preservation->Name, len+1);
			Preservation->Name[len] = '\0';
		}
		else
			Preservation->Name = NULL;
	}
	
	//Extra
	actionControlProcs->StringIDToTypeID(keyPreservationExtra,&typeID);
	actionDescriptorProcs->HasKey(actionDescriptor, typeID, &hasKey);
	if(hasKey)
	{
		actionDescriptorProcs->GetDataLength(actionDescriptor,typeID, (int32*)&len);
		if(len > 0)
		{
			Preservation->Extra = (uint8*)PI3DMemoryAlloc((len + 1) * sizeof(uint8));
			if(!Preservation->Extra) return 0;
			actionDescriptorProcs->GetData(actionDescriptor, typeID, Preservation->Extra);
			Preservation->Extra[len] = 0;
		}
	}
	
	//Flags
	actionControlProcs->StringIDToTypeID(keyPreservationFlags,&typeID);
	actionDescriptorProcs->HasKey(actionDescriptor, typeID, &hasKey);
	if(hasKey)
		actionDescriptorProcs->GetInteger(actionDescriptor, typeID, (int32*)&Preservation->Flags);
	
	//Controller
	actionControlProcs->StringIDToTypeID(keyPreservationController,&typeID);
	actionDescriptorProcs->HasKey(actionDescriptor, typeID, &hasKey);
	if(hasKey)
	{
		actionDescriptorProcs->GetDataLength(actionDescriptor,typeID, (int32*)&len);
		if(len > 0)
		{
			Preservation->Controller = (uint8*)PI3DMemoryAlloc((len + 1) * sizeof(uint8));
			if(!Preservation->Controller) return 0;
			actionDescriptorProcs->GetData(actionDescriptor, typeID, Preservation->Controller);
			Preservation->Controller[len] = 0;
		}
	}
	
	//Animations
	actionControlProcs->StringIDToTypeID(keyPreservationAnimations,&typeID);
	actionDescriptorProcs->HasKey(actionDescriptor, typeID, &hasKey);
	if(hasKey)
	{
		actionDescriptorProcs->GetDataLength(actionDescriptor,typeID, (int32*)&len);
		if(len > 0)
		{
			Preservation->Animations = (uint8*)PI3DMemoryAlloc((len + 1) * sizeof(uint8));
			if(!Preservation->Animations) return 0;
			actionDescriptorProcs->GetData(actionDescriptor, typeID, Preservation->Animations);
			Preservation->Animations[len] = 0;
		}
	}
	
	//Effect Profiles
	actionControlProcs->StringIDToTypeID(keyPreservationEffectProfiles,&typeID);
	actionDescriptorProcs->HasKey(actionDescriptor, typeID, &hasKey);
	if(hasKey)
	{
		actionDescriptorProcs->GetDataLength(actionDescriptor,typeID, (int32*)&len);
		if(len > 0)
		{
			Preservation->EffectProfiles = (uint8*)PI3DMemoryAlloc((len + 1) * sizeof(uint8));
			if(!Preservation->EffectProfiles) return 0;
			actionDescriptorProcs->GetData(actionDescriptor, typeID, Preservation->EffectProfiles);
			Preservation->EffectProfiles[len] = 0;
		}
	}
	
	*preservation=Preservation;
	return 1;
}


short PI3DParseAnimDataIntoDescriptor(PSActionDescriptorProcs *actionDescriptorProcs,PSActionListProcs *actionListProcs, PI3DAnimationData* pAnimData, PIActionDescriptor* pReturnDescriptor)
{
	actionDescriptorProcs->Make(pReturnDescriptor);

	// name
	actionDescriptorProcs->PutString(*pReturnDescriptor,keyNameAnim, pAnimData->name);
	//flags
	actionDescriptorProcs->PutInteger(*pReturnDescriptor, keyFlags, pAnimData->flags);
	//keys
	actionDescriptorProcs->PutInteger(*pReturnDescriptor, keyKeys, pAnimData->keys);

	//animation frame data
	PIActionList animframeActionList;
	actionListProcs->Make(&animframeActionList);
	for(int32 index=0;index<pAnimData->keys;index++)
	{
		PIActionDescriptor animframeDescriptor;
		actionDescriptorProcs->Make(&animframeDescriptor);

		actionDescriptorProcs->PutFloat(animframeDescriptor, keyAnimFrameNumber, pAnimData->aFrameData[index].framenumber);
		actionDescriptorProcs->PutInteger(animframeDescriptor, keyAnimFrameFlags, pAnimData->aFrameData[index].flags);
		actionDescriptorProcs->PutFloat(animframeDescriptor, keyAnimFrameTension, pAnimData->aFrameData[index].tension);
		actionDescriptorProcs->PutFloat(animframeDescriptor, keyAnimFrameContinuity, pAnimData->aFrameData[index].continuity);
		actionDescriptorProcs->PutFloat(animframeDescriptor, keyAnimFrameBias, pAnimData->aFrameData[index].bias);
		actionDescriptorProcs->PutFloat(animframeDescriptor, keyAnimFrameEaseTo, pAnimData->aFrameData[index].easeto);
		actionDescriptorProcs->PutFloat(animframeDescriptor, keyAnimFrameEaseFrom, pAnimData->aFrameData[index].easefrom);
		actionDescriptorProcs->PutFloat(animframeDescriptor, keyAnimFrameRotation, pAnimData->aFrameData[index].rotation);
		actionDescriptorProcs->PutFloat(animframeDescriptor, keyAnimFrameValue, pAnimData->aFrameData[index].value);
		actionDescriptorProcs->PutFloat(animframeDescriptor, keyAnimFrameVectorX, pAnimData->aFrameData[index].v[0]);
		actionDescriptorProcs->PutFloat(animframeDescriptor, keyAnimFrameVectorY, pAnimData->aFrameData[index].v[1]);
		actionDescriptorProcs->PutFloat(animframeDescriptor, keyAnimFrameVectorZ, pAnimData->aFrameData[index].v[2]);
		actionDescriptorProcs->PutBoolean(animframeDescriptor, keyAnimFrameUseQuat, pAnimData->aFrameData[index].usequat);
		actionDescriptorProcs->PutFloat(animframeDescriptor, keyAnimFrameQuatW, pAnimData->aFrameData[index].q.w);
		actionDescriptorProcs->PutFloat(animframeDescriptor, keyAnimFrameQuatX, pAnimData->aFrameData[index].q.x);
		actionDescriptorProcs->PutFloat(animframeDescriptor, keyAnimFrameQuatY, pAnimData->aFrameData[index].q.y);
		actionDescriptorProcs->PutFloat(animframeDescriptor, keyAnimFrameQuatZ, pAnimData->aFrameData[index].q.z);

		actionListProcs->PutObject(animframeActionList,keyAnimFrameClass,animframeDescriptor);
	}
	actionDescriptorProcs->PutList(*pReturnDescriptor,keyAnimFrameList,animframeActionList);		


	return 1;
}

bool	PI3DParseFacesIntoDescriptor(PI3DDescriptorProcs *procs,PI3DScene *scene, PI3DMesh *MeshList, int32 numPoints, PIActionDescriptor &meshDescriptor)
{	
	
	if(MeshList->faces)
		{
		PI3DFaceForReading *packedFaces=(PI3DFaceForReading*)PI3DMemoryAlloc (MeshList->faces*sizeof(PI3DFaceForReading));
		if(!packedFaces)
			return FALSE;
		
		for(int32 index=0;index<MeshList->faces;index++)
			{
			packedFaces[index].flags=MeshList->face[index].flags;
			packedFaces[index].smoothing=MeshList->face[index].smoothing;
			packedFaces[index].numPoints=MeshList->face[index].numPoints;
			packedFaces[index].material=MeshList->face[index].material;
			}

		procs->actionDescriptorProcs->PutData(meshDescriptor,keyFaceList,MeshList->faces*sizeof(PI3DFaceForReading),packedFaces);
		PI3DMemoryFree(packedFaces);
		}

	
				
	uint32 *faceDataBlob=NULL;
	int32 currentPoint=0;
	if(numPoints)
	{
		faceDataBlob=(uint32*)PI3DMemoryAlloc (numPoints * sizeof (int32) * 4);
		if(!faceDataBlob)
			return false;
	}
	else
		return false;

	
	for(int32 index=0;index<MeshList->faces;index++)
	{
		
		for(int32 index2=0;index2<MeshList->face[index].numPoints;index2++)
		{
			faceDataBlob[currentPoint]=MeshList->face[index].points[index2];
			currentPoint++;

			faceDataBlob[currentPoint]=MeshList->face[index].normals[index2];
			currentPoint++;

			faceDataBlob[currentPoint]=MeshList->face[index].textures[index2];
			currentPoint++;

			faceDataBlob[currentPoint]=MeshList->face[index].colors[index2];
			currentPoint++;
		}
	}
	
	
	if(numPoints)
		procs->actionDescriptorProcs->PutData(meshDescriptor, keyFaceIndexes,numPoints * sizeof (int32) * 4,faceDataBlob);
	
	if(faceDataBlob)
		PI3DMemoryFree (faceDataBlob);
	
	return true;
}

void	PI3DParseMeshFloatDataIntoDescriptor(PI3DDescriptorProcs *procs,PI3DMesh *MeshList,PIActionDescriptor &meshDescriptor)
{
	typedef double			PI3DSmallVector[3];
	typedef double			PI3DSmallPoint[2];
	typedef double			PI3DSmallMatrix[4][4];
	PI3DSmallVector				*vertex;					/* List of object vertices */
	PI3DSmallPoint				*texture;					/* List of object textures */
	PI3DSmallVector				*linecolors;				/* per line color - rgb*/
	PI3DSmallVector				*line;						/* line pts */
	PI3DSmallVector				*normal;					/* List of object normals */
	PI3DSmallVector				*color;	 					/* List of vertex colors */
	
	
	int32 index;
	//Vertices
	if(MeshList->vertices)
	{
		vertex=(PI3DSmallVector*)PI3DMemoryAlloc(MeshList->vertices*sizeof(PI3DSmallVector));
		if(!vertex)
			return;
		
		for(index=0;index<MeshList->vertices;index++)
		{
			vertex[index][0]=MeshList->vertex[index][0];
			vertex[index][1]=MeshList->vertex[index][1];
			vertex[index][2]=MeshList->vertex[index][2];
		}
		if(MeshList->vertices)
			procs->actionDescriptorProcs->PutData(meshDescriptor, keyVertexList,MeshList->vertices*sizeof(PI3DSmallVector),vertex);
		
		PI3DMemoryFree(vertex);
	}
	
	if(MeshList->normals)
	{
		normal=(PI3DSmallVector*)PI3DMemoryAlloc(MeshList->normals*sizeof(PI3DSmallVector));
		if(!normal)
			return;
		
		//Normals
		for(index=0;index<MeshList->normals;index++)
		{
			normal[index][0]=MeshList->normal[index][0];
			normal[index][1]=MeshList->normal[index][1];
			normal[index][2]=MeshList->normal[index][2];
		}
		if(MeshList->normals)
			procs->actionDescriptorProcs->PutData(meshDescriptor, keyNormalList,MeshList->normals*sizeof(PI3DSmallVector),normal);
		
		PI3DMemoryFree(normal);
	}
	
	//lines
	
	if(MeshList->lines)
	{
		line=(PI3DSmallVector*)PI3DMemoryAlloc(MeshList->lines*sizeof(PI3DSmallVector));
		if(!line)
			return;
		for(index=0;index<MeshList->lines;index++)
		{
			line[index][0]=MeshList->line[index][0];
			line[index][1]=MeshList->line[index][1];
			line[index][2]=MeshList->line[index][2];
		}
		
		if(MeshList->lines)
			procs->actionDescriptorProcs->PutData(meshDescriptor, keyLineList,MeshList->lines*sizeof(PI3DSmallVector),line);
		
		PI3DMemoryFree(line);
		
	}
	
	if(MeshList->linecolors)
	{
		linecolors=(PI3DSmallVector*)PI3DMemoryAlloc(MeshList->lines*sizeof(PI3DSmallVector));
		if(!linecolors)
			return;
		
		//linecolors
		for(index=0;index<MeshList->lines;index++)
		{
			linecolors[index][0]=MeshList->linecolors[index][0];
			linecolors[index][1]=MeshList->linecolors[index][1];
			linecolors[index][2]=MeshList->linecolors[index][2];
		}
		if(MeshList->linecolors)
			procs->actionDescriptorProcs->PutData(meshDescriptor, keylineColorList,MeshList->lines*sizeof(PI3DSmallVector),linecolors);
		
		PI3DMemoryFree(linecolors);
	}
	
	//Colors
	
	if(MeshList->colors)
	{
		color=(PI3DSmallVector*)PI3DMemoryAlloc(MeshList->colors*sizeof(PI3DSmallVector));
		if(!color)
			return;
		
		//colors
		for(index=0;index<MeshList->colors;index++)
		{
			color[index][0]=MeshList->color[index][0];
			color[index][1]=MeshList->color[index][1];
			color[index][2]=MeshList->color[index][2];
		}
		if(MeshList->colors)
			procs->actionDescriptorProcs->PutData(meshDescriptor, keyVertexColorList,MeshList->colors*sizeof(PI3DSmallVector),color);
		
		PI3DMemoryFree(color);
	}
	
	//UVs
	if(MeshList->textures)
	{
		texture=(PI3DSmallPoint*)PI3DMemoryAlloc(MeshList->textures*sizeof(PI3DSmallPoint));
		if(!texture)
			return;
		
		//textures
		for(index=0;index<MeshList->textures;index++)
		{
			texture[index][0]=MeshList->texture[index][0];
			texture[index][1]=MeshList->texture[index][1];
		}
		if(MeshList->textures)
			procs->actionDescriptorProcs->PutData(meshDescriptor, keyUVList,MeshList->textures*sizeof(PI3DSmallPoint),texture);
		
		PI3DMemoryFree(texture);
	}
	
}

short PI3DParseSceneIntoDescriptor(PI3DDescriptorProcs *procs,PI3DScene	*scene,PIDescriptorHandle *sceneDescriptor)
{
	PSActionDescriptorProcs *actionDescriptorProcs=procs->actionDescriptorProcs;
	PSActionListProcs *actionListProcs=procs->actionListProcs;
	PSBasicActionControlProcs	*actionControlProcs=procs->actionControlProcs;
	if(!sceneDescriptor || !scene)
		return FALSE;	
	
	int32		polyCount=0;
	PI3DList		*theList=NULL;
	long		index,i,j;
	DescriptorTypeID typeID;

	//Traverse the scene and build the descriptor

	PIActionDescriptor actionDescriptor=NULL;
	actionDescriptorProcs->Make(&actionDescriptor);
	if(!actionDescriptor)
		return FALSE;

	int32 tempInt;

	tempInt=kCurrenr3DMajorVersion;
	OSErr err = actionDescriptorProcs->PutInteger(actionDescriptor, keyVersionMajor, tempInt);
	tempInt=kCurrenr3DMinorVersion;
	err = actionDescriptorProcs->PutInteger(actionDescriptor, keyVersionMinor, tempInt);

	//globals
	actionDescriptorProcs->PutFloat(actionDescriptor,keyGlobalAmbientRed,scene->globalAmbientColor.red);
	actionDescriptorProcs->PutFloat(actionDescriptor,keyGlobalAmbientGreen,scene->globalAmbientColor.green);
	actionDescriptorProcs->PutFloat(actionDescriptor,keyGlobalAmbientBlue,scene->globalAmbientColor.blue);

	actionDescriptorProcs->PutFloat(actionDescriptor,keyGlobalUnits,scene->globalUnits);
	actionDescriptorProcs->PutInteger(actionDescriptor,keyGlobalAnimStart,scene->globalAnimationStartTime);
	actionDescriptorProcs->PutInteger(actionDescriptor,keyGlobalFrames,scene->globalNumFrames);
	actionDescriptorProcs->PutFloat(actionDescriptor,keyGlobalFPS,scene->globalFPS);
	actionDescriptorProcs->PutString(actionDescriptor,keyCustomData,scene->customData);
	actionDescriptorProcs->PutString(actionDescriptor,keyName,scene->name);
	
	
	actionControlProcs->StringIDToTypeID(kflipStr,&typeID);
	actionDescriptorProcs->PutBoolean(actionDescriptor,typeID,scene->texturesAreFlipped);

	// dump geo data
	if (scene->geoReference)
	{
		PIActionList geoActionList;
		actionListProcs->Make(&geoActionList);
		PIActionDescriptor geoDescriptor;
		actionDescriptorProcs->Make(&geoDescriptor);
		//Lat Long data
		actionDescriptorProcs->PutFloat(geoDescriptor,keyGlobalGeoLat,scene->geoReference->Latitude);
		actionDescriptorProcs->PutFloat(geoDescriptor,keyGlobalGeoLon,scene->geoReference->Longitude);
		actionDescriptorProcs->PutFloat(geoDescriptor,keyGlobalGeoAlt,scene->geoReference->Altitude);
		actionDescriptorProcs->PutInteger(geoDescriptor,keyGlobalGeoAltM,scene->geoReference->AltitudeMode);
		
		actionDescriptorProcs->PutFloat(geoDescriptor,keyGlobalGeoHead,scene->geoReference->Heading);
		actionDescriptorProcs->PutFloat(geoDescriptor,keyGlobalGeoTilt,scene->geoReference->Tilt);
		actionDescriptorProcs->PutFloat(geoDescriptor,keyGlobalGeoRoll,scene->geoReference->Roll);

		
		actionDescriptorProcs->PutFloat(geoDescriptor,keyGlobalGeoSclX,scene->geoReference->ScaleX);
		actionDescriptorProcs->PutFloat(geoDescriptor,keyGlobalGeoSclY,scene->geoReference->ScaleY);
		actionDescriptorProcs->PutFloat(geoDescriptor,keyGlobalGeoSclZ,scene->geoReference->ScaleZ);

		actionListProcs->PutObject(geoActionList, keyGlobalGeoRef,geoDescriptor);
		actionDescriptorProcs->PutList(actionDescriptor, keyGlobalGeoList,geoActionList);
	}


	
	//Dump the spot lights	
	PIActionList lightActionList;
	actionListProcs->Make(&lightActionList);
	theList=(PI3DList*)scene->lightList;
	while(theList)
		{
		PI3DLight *lightList = (PI3DLight*)theList;
		PIActionDescriptor lightDescriptor;
		actionDescriptorProcs->Make(&lightDescriptor);
		actionDescriptorProcs->PutString(lightDescriptor,keyName,lightList->name);

		//position
		actionDescriptorProcs->PutFloat(lightDescriptor,keyX,lightList->pos[0]);
		actionDescriptorProcs->PutFloat(lightDescriptor,keyY,lightList->pos[1]);
		actionDescriptorProcs->PutFloat(lightDescriptor,keyZ,lightList->pos[2]);

		//target
		actionDescriptorProcs->PutFloat(lightDescriptor,keyTargetX,lightList->target[0]);
		actionDescriptorProcs->PutFloat(lightDescriptor,keyTargetY,lightList->target[1]);
		actionDescriptorProcs->PutFloat(lightDescriptor,keyTargetZ,lightList->target[2]);

		//color
		actionDescriptorProcs->PutFloat(lightDescriptor,keyRed,lightList->col.red);
		actionDescriptorProcs->PutFloat(lightDescriptor,keyGreen,lightList->col.green);
		actionDescriptorProcs->PutFloat(lightDescriptor,keyBlue,lightList->col.blue);

		//hotspot
		actionDescriptorProcs->PutFloat(lightDescriptor,keyHotspot,lightList->hotspot);

		//falloff
		actionDescriptorProcs->PutFloat(lightDescriptor,keyFalloff,lightList->falloff);

		//shadowFlag
		actionDescriptorProcs->PutInteger(lightDescriptor,keyShadow,lightList->shadowFlag);

		//attenuation
		actionDescriptorProcs->PutBoolean(lightDescriptor,keyAttenuation,lightList->attenuation);

		//attenuation type
		actionDescriptorProcs->PutInteger(lightDescriptor,keyAttenType, lightList->attenuationType);

		//attenuation ABC
		actionDescriptorProcs->PutFloat(lightDescriptor,keyAttenA,lightList->attenuationAbc.a);
		actionDescriptorProcs->PutFloat(lightDescriptor,keyAttenB,lightList->attenuationAbc.b);
		actionDescriptorProcs->PutFloat(lightDescriptor,keyAttenC,lightList->attenuationAbc.c);

		//outer_radius
		actionDescriptorProcs->PutFloat(lightDescriptor,keyOuterRadius,lightList->outerRadius);

		//inner_radius
		actionDescriptorProcs->PutFloat(lightDescriptor,keyInnerRadius,lightList->innerRadius);

		//multiple
		actionDescriptorProcs->PutFloat(lightDescriptor,keyMultiple,lightList->multiple);

		//type
		actionDescriptorProcs->PutInteger(lightDescriptor,keyType, lightList->type);
			
		// light switch
		actionDescriptorProcs->PutInteger(lightDescriptor,keyIsOn, lightList->isOn);

		//soft shadow multiple
		actionDescriptorProcs->PutFloat(lightDescriptor,keySSMultiple,lightList->ssmultiple);

		//Preservation
		if (lightList->preservation)
			{
			PIActionList PreservationActionList;
			actionListProcs->Make(&PreservationActionList);
			PIActionDescriptor preservationDescriptor;
			PI3DParsePreservationIntoDescriptor(procs, lightList->preservation,&preservationDescriptor);
			actionListProcs->PutObject(PreservationActionList,keyPreservationObject,preservationDescriptor);		
			actionDescriptorProcs->PutList(lightDescriptor, keyPreservationClass,PreservationActionList);
			}
		actionListProcs->PutObject(lightActionList,keyLightClass,lightDescriptor);
		theList=(PI3DList*)theList->next;
		}
	actionDescriptorProcs->PutList(actionDescriptor,keyLightList,lightActionList);

	//Dump the cameras
	PIActionList cameraActionList;
	actionListProcs->Make(&cameraActionList);
	theList=(PI3DList*)scene->cameraList;
	while(theList)
		{	
		PI3DCamera    *CameraList=(PI3DCamera*)theList;
		PIActionDescriptor cameraDescriptor;
		actionDescriptorProcs->Make(&cameraDescriptor);
		actionDescriptorProcs->PutString(cameraDescriptor,keyName,CameraList->name);

		//position
		actionDescriptorProcs->PutFloat(cameraDescriptor,keyX,CameraList->pos[0]);
		actionDescriptorProcs->PutFloat(cameraDescriptor,keyY,CameraList->pos[1]);
		actionDescriptorProcs->PutFloat(cameraDescriptor,keyZ,CameraList->pos[2]);

		//target
		actionDescriptorProcs->PutFloat(cameraDescriptor,keyTargetX,CameraList->target[0]);
		actionDescriptorProcs->PutFloat(cameraDescriptor,keyTargetY,CameraList->target[1]);
		actionDescriptorProcs->PutFloat(cameraDescriptor,keyTargetZ,CameraList->target[2]);

		//bank
		actionDescriptorProcs->PutFloat(cameraDescriptor,keyBank,CameraList->bank);

		//lens
		actionDescriptorProcs->PutFloat(cameraDescriptor,keyLens,CameraList->lens);

		//ortho
		actionDescriptorProcs->PutBoolean(cameraDescriptor,keyOrtho,CameraList->ortho);

		//aspect ratio
		actionDescriptorProcs->PutFloat(cameraDescriptor,keyApsect,CameraList->aspectratio);

		//zoom factor
		actionDescriptorProcs->PutFloat(cameraDescriptor,keyZoomFactor,CameraList->zoomfactor);

		//Preservation
		if (CameraList->preservation)
			{
			PIActionList PreservationActionList;
			actionListProcs->Make(&PreservationActionList);
			PIActionDescriptor preservationDescriptor;
			PI3DParsePreservationIntoDescriptor(procs, CameraList->preservation,&preservationDescriptor);
			actionListProcs->PutObject(PreservationActionList,keyPreservationObject,preservationDescriptor);		
			actionDescriptorProcs->PutList(cameraDescriptor, keyPreservationClass,PreservationActionList);
			}
			
		actionListProcs->PutObject(cameraActionList,keyCameraClass,cameraDescriptor);
		theList=(PI3DList*)theList->next;
		}
	actionDescriptorProcs->PutList(actionDescriptor,keyCameraList,cameraActionList);
	

	//Dump the meshes
	PIActionList meshActionList;
	actionListProcs->Make(&meshActionList);
	theList=(PI3DList*)scene->meshList;
	while(theList)
		{	
		
		PI3DMesh    *MeshList=(PI3DMesh*)theList;
		PIActionDescriptor meshDescriptor;
		actionDescriptorProcs->Make(&meshDescriptor);
		actionDescriptorProcs->PutString(meshDescriptor,keyName,MeshList->name);

		PI3DParseMeshFloatDataIntoDescriptor(procs, MeshList,meshDescriptor);
			
		int32 numPoints=0;
		for(index=0;index<MeshList->faces;index++)
			numPoints+=MeshList->face[index].numPoints;

		if(numPoints)
			if(!PI3DParseFacesIntoDescriptor(procs,scene, MeshList, numPoints, meshDescriptor))
				return false;
	
		//matrix
		PIActionList matrixList;
		actionListProcs->Make(&matrixList);
		for( i=0;i<4;i++)
			for( j=0;j<4;j++)
				actionListProcs->PutFloat(matrixList,MeshList->matrix[i][j]);	
		actionDescriptorProcs->PutList(meshDescriptor,keyMatrix,matrixList);

		//flags
		actionDescriptorProcs->PutInteger(meshDescriptor,keyFlags,MeshList->flags);

		//hidden
		actionDescriptorProcs->PutInteger(meshDescriptor,keyHidden,MeshList->hidden);

		//shadow
		actionDescriptorProcs->PutInteger(meshDescriptor,keyShadow,MeshList->shadow);

		//matrixPresent
		actionDescriptorProcs->PutBoolean(meshDescriptor,keyHasMatrix,MeshList->matrixPresent);

		//smoothingGroupPresent
		actionDescriptorProcs->PutBoolean(meshDescriptor,keyHasSmoothing,MeshList->smoothingGroupPresent);
		
		//Is this mesh a volume
		actionDescriptorProcs->PutBoolean(meshDescriptor,keyMeshIsVolume,MeshList->isVolume);
			
		//Shadow catching
		actionDescriptorProcs->PutBoolean(meshDescriptor,keyMeshIsShadCtch,MeshList->isShadowCatching);
		actionDescriptorProcs->PutBoolean(meshDescriptor,keyMeshIsShadCast,MeshList->isShadowCasting);
		actionDescriptorProcs->PutBoolean(meshDescriptor,keyMeshIsShadInv,MeshList->isShadowInvisible);

			
		//Preservation
		if (MeshList->preservation)
			{
			PIActionList PreservationActionList;
			actionListProcs->Make(&PreservationActionList);
			PIActionDescriptor preservationDescriptor;
			PI3DParsePreservationIntoDescriptor(procs, MeshList->preservation,&preservationDescriptor);
			actionListProcs->PutObject(PreservationActionList,keyPreservationObject,preservationDescriptor);		
			actionDescriptorProcs->PutList(meshDescriptor, keyPreservationClass,PreservationActionList);
			}

		actionListProcs->PutObject(meshActionList,keyMeshClass,meshDescriptor);
		theList=(PI3DList*)theList->next;
		}
	actionDescriptorProcs->PutList(actionDescriptor,keyMeshList,meshActionList);

	//Dump the material properties
	PIActionList materialActionList;
	actionListProcs->Make(&materialActionList);
	theList=(PI3DList*)scene->matPropList;
	while(theList)
	{	
		PI3DMaterial    *MatPropList=(PI3DMaterial*)theList;
		PIActionDescriptor materialDescriptor;
		actionDescriptorProcs->Make(&materialDescriptor);
		actionDescriptorProcs->PutString(materialDescriptor,keyName,MatPropList->name);

		//ambient
		actionDescriptorProcs->PutFloat(materialDescriptor,keyAmbientRed, MatPropList->ambient.red);
		actionDescriptorProcs->PutFloat(materialDescriptor,keyAmbientGreen, MatPropList->ambient.green);
		actionDescriptorProcs->PutFloat(materialDescriptor,keyAmbientBlue, MatPropList->ambient.blue);

		//diffuse
		actionDescriptorProcs->PutFloat(materialDescriptor,keyDiffuseRed, MatPropList->diffuse.red);
		actionDescriptorProcs->PutFloat(materialDescriptor,keyDiffuseGreen, MatPropList->diffuse.green);
		actionDescriptorProcs->PutFloat(materialDescriptor,keyDiffuseBlue, MatPropList->diffuse.blue);

		//specular
		actionDescriptorProcs->PutFloat(materialDescriptor,keySpecularRed, MatPropList->specular.red);
		actionDescriptorProcs->PutFloat(materialDescriptor,keySpecularGreen, MatPropList->specular.green);
		actionDescriptorProcs->PutFloat(materialDescriptor,keySpecularBlue, MatPropList->specular.blue);

		//emissive
		actionDescriptorProcs->PutFloat(materialDescriptor,keyEmissiveRed, MatPropList->emissive.red);
		actionDescriptorProcs->PutFloat(materialDescriptor,keyEmissiveGreen, MatPropList->emissive.green);
		actionDescriptorProcs->PutFloat(materialDescriptor,keyEmissiveBlue, MatPropList->emissive.blue);

		//shininess
		actionDescriptorProcs->PutFloat(materialDescriptor,keyShininess, MatPropList->shininess);

		//glossiness
		actionDescriptorProcs->PutFloat(materialDescriptor,keyShininess2, MatPropList->glossiness);

		//transparency
		actionDescriptorProcs->PutFloat(materialDescriptor,keyTransparency, MatPropList->transparency);

		//reflection
		actionDescriptorProcs->PutFloat(materialDescriptor,keyReflection, MatPropList->reflection);

		//selfIllumination
		actionDescriptorProcs->PutInteger(materialDescriptor,keySelfIllumination, MatPropList->selfIllumination);

		//shading
		actionDescriptorProcs->PutInteger(materialDescriptor,keyShading, MatPropList->shading);

		//twoSided
		actionDescriptorProcs->PutBoolean(materialDescriptor,keyTwoSide, MatPropList->twoSided);

		//wireframe
		actionDescriptorProcs->PutBoolean(materialDescriptor,keyWireframe, MatPropList->wireframe);

		//decal
		actionDescriptorProcs->PutBoolean(materialDescriptor,keyDecal, MatPropList->decal);

		//wireframesize
		actionDescriptorProcs->PutFloat(materialDescriptor,keyWireframeSize, MatPropList->wireframesize);
		
		//wireframesize
		actionDescriptorProcs->PutFloat(materialDescriptor,keyRefraction, MatPropList->indexofrefraction);

		//maps
		PIActionList mapActionList;
		actionListProcs->Make(&mapActionList);
		for(index=0;index<kMax3DMapTypes;index++)
		{
			PIActionDescriptor mapDescriptor;
			actionDescriptorProcs->Make(&mapDescriptor);

			//map type
			actionDescriptorProcs->PutInteger(mapDescriptor, keyType, index);

			//map name
			actionDescriptorProcs->PutString(mapDescriptor, keyName,MatPropList->maps[index].map);
						
			//map strength
			actionDescriptorProcs->PutFloat(mapDescriptor, keyStrength, MatPropList->maps[index].strength);

			//map uscale
			actionDescriptorProcs->PutFloat(mapDescriptor, keyUScale, MatPropList->maps[index].uscale);

			//map vscale
			actionDescriptorProcs->PutFloat(mapDescriptor, keyVScale, MatPropList->maps[index].vscale);

			//map uoffset
			actionDescriptorProcs->PutFloat(mapDescriptor, keyUOffset, MatPropList->maps[index].uoffset);

			//map voffset
			actionDescriptorProcs->PutFloat(mapDescriptor, keyVOffset, MatPropList->maps[index].voffset);

			//min
			actionControlProcs->StringIDToTypeID(kminimumStr,&typeID);
			actionDescriptorProcs->PutFloat(mapDescriptor, typeID, MatPropList->maps[index].valueMin);

			//max
			actionControlProcs->StringIDToTypeID(kmaximumStr,&typeID);
			actionDescriptorProcs->PutFloat(mapDescriptor, typeID, MatPropList->maps[index].valueMax);

			//map angle
			actionDescriptorProcs->PutFloat(mapDescriptor, keyAngle, MatPropList->maps[index].angle);

			//map flags
			actionDescriptorProcs->PutInteger(mapDescriptor, keyFlags, MatPropList->maps[index].flags);

			// map 3ds style
			actionDescriptorProcs->PutBoolean(mapDescriptor, key3DSStyle, MatPropList->maps[index].mapIs3DSStyle);
			
			//type name
			actionControlProcs->StringIDToTypeID(key3DPaintTypeKey,&typeID);
			if(strlen(MatPropList->maps[index].textureTypeName))
				actionDescriptorProcs->PutData(mapDescriptor, typeID,(int32)strlen(MatPropList->maps[index].textureTypeName),MatPropList->maps[index].textureTypeName);
			
			//layer name
			if(strlen(MatPropList->maps[index].layerName))
				actionDescriptorProcs->PutData(mapDescriptor, keyLayerName,(int32)strlen(MatPropList->maps[index].layerName),MatPropList->maps[index].layerName);
			
			//Preservation
			if (MatPropList->preservation)
			{
				PIActionList PreservationActionList;
				actionListProcs->Make(&PreservationActionList);
				PIActionDescriptor preservationDescriptor;
				PI3DParsePreservationIntoDescriptor(procs, MatPropList->preservation,&preservationDescriptor);
				actionListProcs->PutObject(PreservationActionList,keyPreservationObject,preservationDescriptor);		
				actionDescriptorProcs->PutList(materialDescriptor, keyPreservationClass,PreservationActionList);
			}
			actionListProcs->PutObject(mapActionList,keyMapClass,mapDescriptor);
		}
		actionDescriptorProcs->PutList(materialDescriptor,keyMapList,mapActionList);		

		actionListProcs->PutObject(materialActionList,keyMaterialClass,materialDescriptor);
		theList=(PI3DList*)theList->next;
	}
	actionDescriptorProcs->PutList(actionDescriptor,keyMaterialList,materialActionList);

	//Dump the key frames
	PIActionList keyframeActionList;
	actionListProcs->Make(&keyframeActionList);
	theList=(PI3DList*)scene->keyFrameList;
	while(theList)
	{	
		PI3DKeyFrame    *KeyFrameList=(PI3DKeyFrame*)theList;
		PIActionDescriptor keyframeDescriptor;
		actionDescriptorProcs->Make(&keyframeDescriptor);

		//keyframe props
		actionDescriptorProcs->PutString(keyframeDescriptor,keyName,KeyFrameList->name);
		actionDescriptorProcs->PutString(keyframeDescriptor,keyInstanceName,KeyFrameList->instancename);
		actionDescriptorProcs->PutInteger(keyframeDescriptor,keyFlags1, KeyFrameList->flags1);
		actionDescriptorProcs->PutInteger(keyframeDescriptor,keyFlags2, KeyFrameList->flags2);
		actionDescriptorProcs->PutInteger(keyframeDescriptor,keyU3DNodeID, KeyFrameList->nodeID);
		actionDescriptorProcs->PutInteger(keyframeDescriptor,keyU3DParentID, KeyFrameList->parentNodeID);
		actionDescriptorProcs->PutInteger(keyframeDescriptor,keyU3DType, KeyFrameList->nodeType);
		actionDescriptorProcs->PutInteger(keyframeDescriptor,keyFlags2, KeyFrameList->flags2);

		//pivot pt
		PIActionList PivotActionList;
		actionListProcs->Make(&PivotActionList);
		PIActionDescriptor pivotDescriptor;
		actionDescriptorProcs->Make(&pivotDescriptor);
		actionDescriptorProcs->PutFloat(pivotDescriptor,keyPivotX, KeyFrameList->pivot[0]);
		actionDescriptorProcs->PutFloat(pivotDescriptor,keyPivotY, KeyFrameList->pivot[1]);
		actionDescriptorProcs->PutFloat(pivotDescriptor,keyPivotZ, KeyFrameList->pivot[2]);
		actionListProcs->PutObject(PivotActionList,keyPivotObject,pivotDescriptor);
		actionDescriptorProcs->PutList(keyframeDescriptor,keyPivotList,PivotActionList);

		//matrix
		PIActionList matrixList;
		actionListProcs->Make(&matrixList);
		for( i=0;i<4;i++)
			for( j=0;j<4;j++)
				actionListProcs->PutFloat(matrixList,KeyFrameList->localmatrix[i][j]);	
		actionDescriptorProcs->PutList(keyframeDescriptor,keyMatrix,matrixList);

		//pivot present
		actionDescriptorProcs->PutBoolean(keyframeDescriptor,keyHasPivot,KeyFrameList->usepivot);

		//matrix present
		actionDescriptorProcs->PutBoolean(keyframeDescriptor,keyHasLocalMatrix,KeyFrameList->matrixPresent);

		//animation range
		actionDescriptorProcs->PutBoolean(keyframeDescriptor,keyHasRange,KeyFrameList->userange);

		//position data
		int32 bDidit = 0;
		if (KeyFrameList->positionData)
		{
			PIActionList PositionActionList;
			actionListProcs->Make(&PositionActionList);
			PI3DList *theAList=(PI3DList*)KeyFrameList->positionData;
			while(theAList)
			{	
				PI3DAnimationData  *AnimationList=(PI3DAnimationData*)theAList;
				PIActionDescriptor PositionDescriptor;
				bDidit = PI3DParseAnimDataIntoDescriptor(actionDescriptorProcs, actionListProcs, AnimationList, &PositionDescriptor);
				actionListProcs->PutObject(PositionActionList,keyframePositionActionList,PositionDescriptor);
				theAList=(PI3DList*)theAList->next;
			}
			actionDescriptorProcs->PutList(keyframeDescriptor, keyframePositionList, PositionActionList);		
		}
		//scale data
		if (KeyFrameList->scaleData)
		{
			PIActionList ScaleActionList;
			actionListProcs->Make(&ScaleActionList);
			PI3DList *theAList=(PI3DList*)KeyFrameList->scaleData;
			while(theAList)
			{	
				PI3DAnimationData  *AnimationList=(PI3DAnimationData*)theAList;
				PIActionDescriptor ScaleDescriptor;
				bDidit = PI3DParseAnimDataIntoDescriptor(actionDescriptorProcs, actionListProcs, AnimationList, &ScaleDescriptor);
				actionListProcs->PutObject(ScaleActionList,keyframeScaleActionList,ScaleDescriptor);
				theAList=(PI3DList*)theAList->next;
			}
			actionDescriptorProcs->PutList(keyframeDescriptor, keyframeScaleList, ScaleActionList);		
		}
		//rotation data
		if (KeyFrameList->rotationData)		
		{
			PIActionList RotationActionList;
			actionListProcs->Make(&RotationActionList);
			PI3DList *theAList=(PI3DList*)KeyFrameList->rotationData;
			while(theAList)
			{	
				PI3DAnimationData  *AnimationList=(PI3DAnimationData*)theAList;
				PIActionDescriptor RotationDescriptor;
				bDidit = PI3DParseAnimDataIntoDescriptor(actionDescriptorProcs, actionListProcs, AnimationList, &RotationDescriptor);
				actionListProcs->PutObject(RotationActionList,keyframeRotationActionList,RotationDescriptor);
				theAList=(PI3DList*)theAList->next;
			}
			actionDescriptorProcs->PutList(keyframeDescriptor, keyframeRotationList, RotationActionList);
		}
		//FOV data
		if (KeyFrameList->fOVData)		
		{
			PIActionList FOVActionList;
			actionListProcs->Make(&FOVActionList);
			PI3DList *theAList=(PI3DList*)KeyFrameList->fOVData;
			while(theAList)
			{	
				PI3DAnimationData  *AnimationList=(PI3DAnimationData*)theAList;
				PIActionDescriptor FOVDescriptor;
				bDidit = PI3DParseAnimDataIntoDescriptor(actionDescriptorProcs, actionListProcs, AnimationList, &FOVDescriptor);
				actionListProcs->PutObject(FOVActionList,keyframeFOVActionList,FOVDescriptor);
				theAList=(PI3DList*)theAList->next;
			}
			actionDescriptorProcs->PutList(keyframeDescriptor, keyframeFOVList, FOVActionList);		
		}
		//Roll data
		if (KeyFrameList->rollData)
		{
			PIActionList RollActionList;
			actionListProcs->Make(&RollActionList);
			PI3DList *theAList=(PI3DList*)KeyFrameList->rollData;
			while(theAList)
			{	
				PI3DAnimationData  *AnimationList=(PI3DAnimationData*)theAList;
				PIActionDescriptor RollDescriptor;
				bDidit = PI3DParseAnimDataIntoDescriptor(actionDescriptorProcs, actionListProcs, AnimationList, &RollDescriptor);
				actionListProcs->PutObject(RollActionList,keyframeRollActionList,RollDescriptor);
				theAList=(PI3DList*)theAList->next;
			}
			actionDescriptorProcs->PutList(keyframeDescriptor, keyframeRollList, RollActionList);					
		}
		//Col data
		if (KeyFrameList->colData)
		{
			PIActionList ColorActionList;
			actionListProcs->Make(&ColorActionList);
			PI3DList *theAList=(PI3DList*)KeyFrameList->colData;
			while(theAList)
			{	
				PI3DAnimationData  *AnimationList=(PI3DAnimationData*)theAList;
				PIActionDescriptor ColorDescriptor;
				bDidit = PI3DParseAnimDataIntoDescriptor(actionDescriptorProcs, actionListProcs, AnimationList, &ColorDescriptor);
				actionListProcs->PutObject(ColorActionList,keyframeColorActionList,ColorDescriptor);
				theAList=(PI3DList*)theAList->next;
			}
			actionDescriptorProcs->PutList(keyframeDescriptor, keyframeColorList, ColorActionList);					
		}
		//Hot data
		if (KeyFrameList->hotData)
		{
			PIActionList hotActionList;
			actionListProcs->Make(&hotActionList);
			PI3DList *theAList=(PI3DList*)KeyFrameList->hotData;
			while(theAList)
			{	
				PI3DAnimationData  *AnimationList=(PI3DAnimationData*)theAList;
				PIActionDescriptor HotDescriptor;
				bDidit = PI3DParseAnimDataIntoDescriptor(actionDescriptorProcs, actionListProcs, AnimationList, &HotDescriptor);
				actionListProcs->PutObject(hotActionList,keyframeHotActionList,HotDescriptor);
				theAList=(PI3DList*)theAList->next;
			}
			actionDescriptorProcs->PutList(keyframeDescriptor, keyframeHotList, hotActionList);		
		}
		//Falloff data
		if (KeyFrameList->fallData)
		{
			PIActionList FallActionList;
			actionListProcs->Make(&FallActionList);
			PI3DList *theAList=(PI3DList*)KeyFrameList->fallData;
			while(theAList)
			{	
				PI3DAnimationData  *AnimationList=(PI3DAnimationData*)theAList;
				PIActionDescriptor FallDescriptor;
				bDidit = PI3DParseAnimDataIntoDescriptor(actionDescriptorProcs, actionListProcs, AnimationList, &FallDescriptor);
				actionListProcs->PutObject(FallActionList,keyframeFalloffActionList,FallDescriptor);
				theAList=(PI3DList*)theAList->next;
			}
			actionDescriptorProcs->PutList(keyframeDescriptor, keyframeFalloffList, FallActionList);		
		}
		//Hide data
		if (KeyFrameList->hideData)
		{
			PIActionList HideActionList;
			actionListProcs->Make(&HideActionList);
			PI3DList *theAList=(PI3DList*)KeyFrameList->hideData;
			while(theAList)
			{	
				PI3DAnimationData  *AnimationList=(PI3DAnimationData*)theAList;
				PIActionDescriptor HideDescriptor;
				bDidit = PI3DParseAnimDataIntoDescriptor(actionDescriptorProcs, actionListProcs, AnimationList, &HideDescriptor);
				actionListProcs->PutObject(HideActionList,keyframeHideActionList,HideDescriptor);
				theAList=(PI3DList*)theAList->next;
			}
			actionDescriptorProcs->PutList(keyframeDescriptor, keyframeHideList, HideActionList);		
		}
		//Morph data
		if (KeyFrameList->morphData)
		{
			PIActionList MorphActionList;
			actionListProcs->Make(&MorphActionList);
			PI3DList *theAList=(PI3DList*)KeyFrameList->morphData;
			while(theAList)
			{	
				PI3DAnimationData  *AnimationList=(PI3DAnimationData*)theAList;
				PIActionDescriptor MorphDescriptor;
				bDidit = PI3DParseAnimDataIntoDescriptor(actionDescriptorProcs, actionListProcs, AnimationList, &MorphDescriptor);
				actionListProcs->PutObject(MorphActionList,keyframeMorphActionList,MorphDescriptor);
				theAList=(PI3DList*)theAList->next;
			}
			actionDescriptorProcs->PutList(keyframeDescriptor, keyframeMorphList, MorphActionList);		
		}

		//Preservation
		if (KeyFrameList->preservation)
			{
			PIActionList PreservationActionList;
			actionListProcs->Make(&PreservationActionList);
			PIActionDescriptor preservationDescriptor;
			PI3DParsePreservationIntoDescriptor(procs, KeyFrameList->preservation,&preservationDescriptor);
			actionListProcs->PutObject(PreservationActionList,keyPreservationObject,preservationDescriptor);		
			actionDescriptorProcs->PutList(keyframeDescriptor, keyPreservationClass,PreservationActionList);
			}

		actionListProcs->PutObject(keyframeActionList,keyKeyFrameClass,keyframeDescriptor);
		theList=(PI3DList*)theList->next;
	}
	actionDescriptorProcs->PutList(actionDescriptor,keyKeyFrameList,keyframeActionList);
	actionDescriptorProcs->PutInteger(actionDescriptor,keyPolyCount,polyCount);
	
	*sceneDescriptor = NULL;
	actionDescriptorProcs->AsHandle(actionDescriptor, sceneDescriptor);
	actionDescriptorProcs->Free(actionDescriptor);
	
	if(!*sceneDescriptor)
		return false;

	return TRUE;

}

short PI3DParseAnimDataIntoScene(PSActionDescriptorProcs *actionDescriptorProcs,PSActionListProcs *actionListProcs, PI3DAnimationData* pAnimData, PIActionDescriptor* pDescriptor)
	{

	// name
	actionDescriptorProcs->GetString(*pDescriptor,keyNameAnim,pAnimData->name,80);
	//flags
	actionDescriptorProcs->GetInteger(*pDescriptor, keyFlags, &pAnimData->flags);
	//keys
	actionDescriptorProcs->GetInteger(*pDescriptor, keyKeys, &pAnimData->keys);

	//animation frame data
	pAnimData->aFrameData = (PI3DFrameVector*)PI3DMemoryAlloc (pAnimData->keys * sizeof (PI3DFrameVector));
	if (!pAnimData->aFrameData )
		return 0;

	memset(pAnimData->aFrameData,0,sizeof(*pAnimData->aFrameData));

	Boolean hasKey = false;
	actionDescriptorProcs->HasKey(*pDescriptor, keyAnimFrameList, &hasKey);
	if(hasKey)
		{
		DescriptorClassID tempType;
		PIActionList animframeActionList;
		actionDescriptorProcs->GetList(*pDescriptor,keyAnimFrameList,&animframeActionList);
		if(animframeActionList)
			{
			for(int32 index=0;index<pAnimData->keys;index++)
				{
				PIActionDescriptor animframeDescriptor;
				tempType = keyAnimFrameClass;
				actionListProcs->GetObject(animframeActionList,index,&tempType,&animframeDescriptor);
				if(animframeDescriptor)
					{
					double tempFloat;
					actionDescriptorProcs->GetFloat(animframeDescriptor, keyAnimFrameNumber, &tempFloat);
					pAnimData->aFrameData[index].framenumber = tempFloat;
					
					actionDescriptorProcs->GetInteger(animframeDescriptor, keyAnimFrameFlags, &pAnimData->aFrameData[index].flags);
					actionDescriptorProcs->GetBoolean(animframeDescriptor, keyAnimFrameUseQuat, &pAnimData->aFrameData[index].usequat);

					actionDescriptorProcs->GetFloat(animframeDescriptor, keyAnimFrameTension, &tempFloat);
					pAnimData->aFrameData[index].tension=tempFloat;
					actionDescriptorProcs->GetFloat(animframeDescriptor, keyAnimFrameContinuity, &tempFloat);
					pAnimData->aFrameData[index].continuity=tempFloat;
					actionDescriptorProcs->GetFloat(animframeDescriptor, keyAnimFrameBias, &tempFloat);
					pAnimData->aFrameData[index].bias=tempFloat;
					actionDescriptorProcs->GetFloat(animframeDescriptor, keyAnimFrameEaseTo, &tempFloat);
					pAnimData->aFrameData[index].easeto=tempFloat;
					actionDescriptorProcs->GetFloat(animframeDescriptor, keyAnimFrameEaseFrom, &tempFloat);
					pAnimData->aFrameData[index].easefrom=tempFloat;
					actionDescriptorProcs->GetFloat(animframeDescriptor, keyAnimFrameRotation, &tempFloat);
					pAnimData->aFrameData[index].rotation=tempFloat;
					actionDescriptorProcs->GetFloat(animframeDescriptor, keyAnimFrameValue, &tempFloat);
					pAnimData->aFrameData[index].value=tempFloat;
					actionDescriptorProcs->GetFloat(animframeDescriptor, keyAnimFrameVectorX, &tempFloat);
					pAnimData->aFrameData[index].v[0]=tempFloat;
					actionDescriptorProcs->GetFloat(animframeDescriptor, keyAnimFrameVectorY, &tempFloat);
					pAnimData->aFrameData[index].v[1]=tempFloat;
					actionDescriptorProcs->GetFloat(animframeDescriptor, keyAnimFrameVectorZ, &tempFloat);
					pAnimData->aFrameData[index].v[2]=tempFloat;
					actionDescriptorProcs->GetFloat(animframeDescriptor, keyAnimFrameQuatW, &tempFloat);
					pAnimData->aFrameData[index].q.w=tempFloat;
					actionDescriptorProcs->GetFloat(animframeDescriptor, keyAnimFrameQuatX, &tempFloat);
					pAnimData->aFrameData[index].q.x=tempFloat;
					actionDescriptorProcs->GetFloat(animframeDescriptor, keyAnimFrameQuatY, &tempFloat);
					pAnimData->aFrameData[index].q.y=tempFloat;
					actionDescriptorProcs->GetFloat(animframeDescriptor, keyAnimFrameQuatZ, &tempFloat);
					pAnimData->aFrameData[index].q.z=tempFloat;

					
					actionDescriptorProcs->Free(animframeDescriptor);
					}
				}
			actionListProcs->Free(animframeActionList);
			}
		}
	

	return 1;
	}

short PI3DParseDescriptorIntoScene(PI3DDescriptorProcs *procs,PIDescriptorHandle sceneDescriptor,PI3DScene **inScene)
{
	PSActionDescriptorProcs *actionDescriptorProcs=procs->actionDescriptorProcs;
	PSActionListProcs *actionListProcs=procs->actionListProcs;
	PSBasicActionControlProcs	*actionControlProcs=procs->actionControlProcs;
	
	DescriptorTypeID typeID;
	Boolean hasKey = false;
	uint32 listSize=0;
	uint32 index,i,j;
	int32 index2,index3;
	int32 tempInt;
	DescriptorClassID tempType;
	double	tFloat;
	PI3DScene *scene = (PI3DScene*)PI3DMemoryAlloc(sizeof(PI3DScene));
	if(!scene)
		return 0;
		
	memset(scene,0,sizeof(PI3DScene));

	PIActionDescriptor actionDescriptor = NULL;

	actionDescriptorProcs->HandleToDescriptor(sceneDescriptor,&actionDescriptor);

	//globals
	actionDescriptorProcs->GetFloat(actionDescriptor,keyGlobalAmbientRed,&tFloat);
	scene->globalAmbientColor.red = tFloat;
	actionDescriptorProcs->GetFloat(actionDescriptor,keyGlobalAmbientGreen,&tFloat);
	scene->globalAmbientColor.green = tFloat;
	actionDescriptorProcs->GetFloat(actionDescriptor,keyGlobalAmbientBlue,&tFloat);
	scene->globalAmbientColor.blue = tFloat;

	actionDescriptorProcs->GetFloat(actionDescriptor,keyGlobalUnits,&tFloat);
	scene->globalUnits = tFloat;
	actionDescriptorProcs->GetInteger(actionDescriptor,keyGlobalAnimStart,&scene->globalAnimationStartTime);
	actionDescriptorProcs->GetInteger(actionDescriptor,keyGlobalFrames,&scene->globalNumFrames);
	actionDescriptorProcs->GetFloat(actionDescriptor,keyGlobalFPS,&tFloat);
	scene->globalFPS = tFloat;

	actionDescriptorProcs->HasKey(actionDescriptor, keyCustomData, &hasKey);
	if(hasKey)
		actionDescriptorProcs->GetString(actionDescriptor,keyCustomData,scene->customData,256);

	actionDescriptorProcs->HasKey(actionDescriptor, keyName, &hasKey);
	if(hasKey)
		actionDescriptorProcs->GetString(actionDescriptor,keyName,scene->name,256);

	actionControlProcs->StringIDToTypeID(kflipStr,&typeID);
	actionDescriptorProcs->HasKey(actionDescriptor, typeID, &hasKey);
	if(hasKey)
		actionDescriptorProcs->GetBoolean(actionDescriptor, hasKey, &scene->texturesAreFlipped);
	
	actionDescriptorProcs->HasKey(actionDescriptor, keyGlobalGeoList, &hasKey);
	if(hasKey)
	{
		//Get the geo data	
		PIActionList geoActionList;
		actionDescriptorProcs->GetList(actionDescriptor, keyGlobalGeoList, &geoActionList);
		scene->geoReference = (PI3DGeoReference*)PI3DMemoryAlloc(sizeof(PI3DGeoReference));
		if(!scene->geoReference) return 0;
		PIActionDescriptor geoDescriptor;
		tempType = keyGlobalGeoList;
		actionListProcs->GetObject(geoActionList,0, &tempType, &geoDescriptor);
		//Lat Long data
		actionDescriptorProcs->GetFloat(geoDescriptor, keyGlobalGeoLat,&tFloat);
		scene->geoReference->Latitude = tFloat;
		actionDescriptorProcs->GetFloat(geoDescriptor, keyGlobalGeoLon, &tFloat);
		scene->geoReference->Longitude = tFloat;
		actionDescriptorProcs->GetFloat(geoDescriptor, keyGlobalGeoAlt, &tFloat);
		scene->geoReference->Altitude = tFloat;
		actionDescriptorProcs->GetInteger(geoDescriptor, keyGlobalGeoAltM, (int32*)&scene->geoReference->AltitudeMode);
		
		actionDescriptorProcs->GetFloat(geoDescriptor, keyGlobalGeoHead, &tFloat);
		scene->geoReference->Heading = tFloat;
		actionDescriptorProcs->GetFloat(geoDescriptor, keyGlobalGeoTilt, &tFloat);
		scene->geoReference->Tilt = tFloat;
		actionDescriptorProcs->GetFloat(geoDescriptor, keyGlobalGeoRoll, &tFloat);
		scene->geoReference->Roll = tFloat;
		
		actionDescriptorProcs->GetFloat(geoDescriptor, keyGlobalGeoSclX, &tFloat);
		scene->geoReference->ScaleX = tFloat;
		actionDescriptorProcs->GetFloat(geoDescriptor, keyGlobalGeoSclY, &tFloat);
		scene->geoReference->ScaleY = tFloat;
		actionDescriptorProcs->GetFloat(geoDescriptor, keyGlobalGeoSclZ, &tFloat);
		scene->geoReference->ScaleZ = tFloat;
	}

	
	actionDescriptorProcs->HasKey(actionDescriptor, keyLightList, &hasKey);
	if(hasKey)
	{
		//Get the lights	
		PIActionList lightActionList;
		actionDescriptorProcs->GetList(actionDescriptor,keyLightList,&lightActionList);
		if(lightActionList)
		{
			actionListProcs->GetCount(lightActionList,&listSize);
			for(index=0;index<listSize;index++)
			{
				PI3DLight *Light = (PI3DLight*)PI3DMemoryAlloc (sizeof (PI3DLight));
				if(!Light)
					return 0;
					
				PIActionDescriptor lightDescriptor;
				tempType = keyLightClass;
				actionListProcs->GetObject(lightActionList,index,&tempType,&lightDescriptor);
				if(lightDescriptor)
				{
					//name
					actionDescriptorProcs->GetString(lightDescriptor,keyName,Light->name,40);

					//position
					actionDescriptorProcs->GetFloat(lightDescriptor,keyX,&tFloat);
					Light->pos[0] = tFloat;
					actionDescriptorProcs->GetFloat(lightDescriptor,keyY,&tFloat);
					Light->pos[1] = tFloat;
					actionDescriptorProcs->GetFloat(lightDescriptor,keyZ,&tFloat);
					Light->pos[2] = tFloat;

					//target
					actionDescriptorProcs->GetFloat(lightDescriptor,keyTargetX,&tFloat);
					Light->target[0] = tFloat;
					actionDescriptorProcs->GetFloat(lightDescriptor,keyTargetY,&tFloat);
					Light->target[1] = tFloat;
					actionDescriptorProcs->GetFloat(lightDescriptor,keyTargetZ,&tFloat);
					Light->target[2] = tFloat;

					//color
					actionDescriptorProcs->GetFloat(lightDescriptor,keyRed,&tFloat);
					Light->col.red = tFloat;
					actionDescriptorProcs->GetFloat(lightDescriptor,keyGreen,&tFloat);
					Light->col.green = tFloat;
					actionDescriptorProcs->GetFloat(lightDescriptor,keyBlue,&tFloat);
					Light->col.blue = tFloat;

					//hotspot
					actionDescriptorProcs->GetFloat(lightDescriptor,keyHotspot,&tFloat);
					Light->hotspot = tFloat;

					//falloff
					actionDescriptorProcs->GetFloat(lightDescriptor,keyFalloff,&tFloat);
					Light->falloff = tFloat;

					//shadowFlag
					actionDescriptorProcs->GetInteger(lightDescriptor,keyShadow,&Light->shadowFlag);

					//attenuation
					Boolean bAtt;
					actionDescriptorProcs->GetBoolean(lightDescriptor,keyAttenuation,&bAtt);
					Light->attenuation = (bool)bAtt;

					//attenuation type
					int32 e;
					actionDescriptorProcs->GetInteger(lightDescriptor,keyAttenType, &e);
					Light->attenuationType = (PI3DAttenuationEnum)e;

					//attenuation ABC
					actionDescriptorProcs->GetFloat(lightDescriptor,keyAttenA,&tFloat);
					Light->attenuationAbc.a = tFloat;
					actionDescriptorProcs->GetFloat(lightDescriptor,keyAttenB,&tFloat);
					Light->attenuationAbc.b = tFloat;
					actionDescriptorProcs->GetFloat(lightDescriptor,keyAttenC,&tFloat);
					Light->attenuationAbc.c = tFloat;

					//outer_radius
					actionDescriptorProcs->GetFloat(lightDescriptor,keyOuterRadius,&tFloat);
					Light->outerRadius = tFloat;

					//inner_radius
					actionDescriptorProcs->GetFloat(lightDescriptor,keyInnerRadius,&tFloat);
					Light->innerRadius = tFloat;
					
					//multiple
					actionDescriptorProcs->GetFloat(lightDescriptor,keyMultiple,&tFloat);
					Light->multiple = tFloat;
					
					//light switch
					actionDescriptorProcs->HasKey(actionDescriptor, keyIsOn, &hasKey);
					if(hasKey)
						actionDescriptorProcs->GetBoolean(lightDescriptor,keyIsOn,&Light->isOn);

					//ssmultiple
					actionDescriptorProcs->HasKey(actionDescriptor, keySSMultiple, &hasKey);
					if(hasKey)
						{
						actionDescriptorProcs->GetFloat(lightDescriptor,keySSMultiple,&tFloat);
						Light->ssmultiple = tFloat;
						}

					//shading
					actionDescriptorProcs->GetInteger(lightDescriptor,keyType, &tempInt);
					Light->type = (PI3DLightEnum)tempInt;

					
					// Preservation
					Light->preservation = NULL;
					PIActionList PreservationActionList;
					actionDescriptorProcs->HasKey(lightDescriptor, keyPreservationClass, &hasKey);
					if(hasKey)
						{
						actionDescriptorProcs->GetList(lightDescriptor, keyPreservationClass, &PreservationActionList);
						PIActionDescriptor preservationDescriptor;
						tempType = keyPreservationObject;
						actionListProcs->GetObject(PreservationActionList, 0,&tempType,&preservationDescriptor);
						PI3DParseDescriptorIntoPreservation(procs, preservationDescriptor, &Light->preservation);		
						}
					
					actionDescriptorProcs->Free(lightDescriptor);
					PI3DListAdd((PI3DList **)&scene->lightList, reinterpret_cast<PI3DList *>(Light));
				}
			}
			actionListProcs->Free(lightActionList);
		}
	}

	actionDescriptorProcs->HasKey(actionDescriptor, keyCameraList, &hasKey);
	if(hasKey)
	{
		//Get the cameras
		PIActionList cameraActionList;
		actionDescriptorProcs->GetList(actionDescriptor,keyCameraList,&cameraActionList);
		if(cameraActionList)
			{
			actionListProcs->GetCount(cameraActionList,&listSize);
			for(index=0;index<listSize;index++)
			{
				PI3DCamera *Camera = (PI3DCamera*)PI3DMemoryAlloc (sizeof (PI3DCamera));
				if(!Camera)
					return 0;
					
				PIActionDescriptor cameraDescriptor;
				tempType = keyCameraClass;
				actionListProcs->GetObject(cameraActionList,index,&tempType,&cameraDescriptor);
				if(cameraDescriptor)
				{
					//name
					actionDescriptorProcs->GetString(cameraDescriptor,keyName,Camera->name, 40);

					//position
					actionDescriptorProcs->GetFloat(cameraDescriptor,keyX,&tFloat);
					Camera->pos[0] = tFloat;
					actionDescriptorProcs->GetFloat(cameraDescriptor,keyY,&tFloat);
					Camera->pos[1] = tFloat;
					actionDescriptorProcs->GetFloat(cameraDescriptor,keyZ,&tFloat);
					Camera->pos[2] = tFloat;

					//target
					actionDescriptorProcs->GetFloat(cameraDescriptor,keyTargetX,&tFloat);
					Camera->target[0] = tFloat;
					actionDescriptorProcs->GetFloat(cameraDescriptor,keyTargetY,&tFloat);
					Camera->target[1] = tFloat;
					actionDescriptorProcs->GetFloat(cameraDescriptor,keyTargetZ,&tFloat);
					Camera->target[2] = tFloat;

					//bank
					actionDescriptorProcs->GetFloat(cameraDescriptor,keyBank,&tFloat);
					Camera->bank = tFloat;

					//lens
					actionDescriptorProcs->GetFloat(cameraDescriptor,keyLens,&tFloat);
					Camera->lens = tFloat;

					//ortho
					Boolean bOrtho;
					actionDescriptorProcs->GetBoolean(cameraDescriptor,keyOrtho,&bOrtho);
					Camera->ortho = (bool)bOrtho;

					//aspect ratio
					actionDescriptorProcs->GetFloat(cameraDescriptor,keyApsect,&tFloat);
					Camera->aspectratio = tFloat;

					//zoom factor
					actionDescriptorProcs->GetFloat(cameraDescriptor,keyZoomFactor,&tFloat);
					Camera->zoomfactor = tFloat;
						

					// Preservation
					Camera->preservation = NULL;
					PIActionList PreservationActionList;
					actionDescriptorProcs->HasKey(cameraDescriptor, keyPreservationClass, &hasKey);
					if(hasKey)
						{
						actionDescriptorProcs->GetList(cameraDescriptor, keyPreservationClass, &PreservationActionList);
						PIActionDescriptor preservationDescriptor;
						tempType = keyPreservationObject;
						actionListProcs->GetObject(PreservationActionList, 0,&tempType,&preservationDescriptor);
						PI3DParseDescriptorIntoPreservation(procs, preservationDescriptor, &Camera->preservation);		
						}

					actionDescriptorProcs->Free(cameraDescriptor);
					
					PI3DListAdd((PI3DList **)&scene->cameraList, reinterpret_cast<PI3DList *>(Camera));
				}
			}
			actionListProcs->Free(cameraActionList);
		}
	}


	actionDescriptorProcs->HasKey(actionDescriptor, keyMeshList, &hasKey);
	if(hasKey)
	{
		//Get the meshes
		PIActionList meshActionList;
		actionDescriptorProcs->GetList(actionDescriptor,keyMeshList,&meshActionList);
		if(meshActionList)
		{
			actionListProcs->GetCount(meshActionList,&listSize);
			for(index=0;index<listSize;index++)
			{
				PI3DMesh *Mesh = (PI3DMesh*)PI3DMemoryAlloc (sizeof (PI3DMesh));
				if(!Mesh)
					return 0;
					
				memset(Mesh,0,sizeof(PI3DMesh));
				PIActionDescriptor meshDescriptor;
				tempType = keyMeshClass;
				actionListProcs->GetObject(meshActionList,index,&tempType,&meshDescriptor);
				if(meshDescriptor)
				{
					//name	
					actionDescriptorProcs->GetString(meshDescriptor,keyName,Mesh->name, 40);
					
					//vertices
					actionDescriptorProcs->HasKey(meshDescriptor, keyVertexList, &hasKey);
					if (hasKey)
					{
						actionDescriptorProcs->GetDataLength(meshDescriptor,keyVertexList,&Mesh->vertices);
						Mesh->vertices /= sizeof (PI3DVector);
						Mesh->vertex = (PI3DVector*)PI3DMemoryAlloc (Mesh->vertices * sizeof (PI3DVector));
						if(!Mesh->vertex)
							return 0;
							
						actionDescriptorProcs->GetData(meshDescriptor,keyVertexList,Mesh->vertex);
		#ifdef __ppc__
						for(index2=0;index2<Mesh->vertices;index2++)
							{
							PI3DByteSwapDouble(&Mesh->vertex[index2][0]);
							PI3DByteSwapDouble(&Mesh->vertex[index2][1]);
							PI3DByteSwapDouble(&Mesh->vertex[index2][2]);
							}
		#endif
					}
					
					//normals
					actionDescriptorProcs->HasKey(meshDescriptor, keyNormalList, &hasKey);
					if (hasKey)
					{
						actionDescriptorProcs->GetDataLength(meshDescriptor,keyNormalList,&Mesh->normals);
						Mesh->normals /= sizeof (PI3DVector);
						Mesh->normal = (PI3DVector*)PI3DMemoryAlloc (Mesh->normals * sizeof (PI3DVector));
						if(!Mesh->normal)
							return 0;
							
						actionDescriptorProcs->GetData(meshDescriptor,keyNormalList,Mesh->normal);
		#ifdef __ppc__
						for(index2=0;index2<Mesh->normals;index2++)
							{
							PI3DByteSwapDouble(&Mesh->normal[index2][0]);
							PI3DByteSwapDouble(&Mesh->normal[index2][1]);
							PI3DByteSwapDouble(&Mesh->normal[index2][2]);
							}
		#endif
					}

					//linecolors
					actionDescriptorProcs->HasKey(meshDescriptor, keylineColorList, &hasKey);
					if (hasKey)
					{
						actionDescriptorProcs->GetDataLength(meshDescriptor,keylineColorList,&Mesh->lines);
						Mesh->lines /= sizeof (PI3DVector);
						Mesh->linecolors = (PI3DVector*)PI3DMemoryAlloc (Mesh->lines * sizeof (PI3DVector));
						if(!Mesh->linecolors)
							return 0;
							
						actionDescriptorProcs->GetData(meshDescriptor,keylineColorList,Mesh->linecolors);
		#ifdef __ppc__
						for(index2=0;index2<Mesh->lines;index2++)
						{
							PI3DByteSwapDouble(&Mesh->linecolors[index2][0]);
							PI3DByteSwapDouble(&Mesh->linecolors[index2][1]);
							PI3DByteSwapDouble(&Mesh->linecolors[index2][2]);
						}
		#endif
					}

					//lines
					actionDescriptorProcs->HasKey(meshDescriptor, keyLineList, &hasKey);
					if (hasKey)
					{
						actionDescriptorProcs->GetDataLength(meshDescriptor,keyLineList,&Mesh->lines);
						Mesh->lines /= sizeof (PI3DVector);
						Mesh->line = (PI3DVector*)PI3DMemoryAlloc (Mesh->lines * sizeof (PI3DVector));
						if(!Mesh->line)
							return 0;
							
						actionDescriptorProcs->GetData(meshDescriptor,keyLineList,Mesh->line);
		#ifdef __ppc__
						for(index2=0;index2<Mesh->lines;index2++)
						{
							PI3DByteSwapDouble(&Mesh->line[index2][0]);
							PI3DByteSwapDouble(&Mesh->line[index2][1]);
							PI3DByteSwapDouble(&Mesh->line[index2][2]);
						}
		#endif
					}

					//linematerials
					actionDescriptorProcs->HasKey(meshDescriptor, keylineMaterialList, &hasKey);
					if (hasKey)
					{
						actionDescriptorProcs->GetDataLength(meshDescriptor,keylineMaterialList,&Mesh->lines);
						Mesh->lines /= sizeof (int32);
						Mesh->linematerial = (int32*)PI3DMemoryAlloc (Mesh->lines * sizeof (int32));
						if(!Mesh->linematerial)
							return 0;
							
						actionDescriptorProcs->GetData(meshDescriptor,keylineMaterialList,Mesh->linematerial);
		#ifdef __ppc__
						for(index2=0;index2<Mesh->lines;index2++)
						{
							SWAP32_IF_BIGENDIAN(Mesh->linematerial[index2]);
						}
		#endif
					}

					//vertex colors
					actionDescriptorProcs->HasKey(meshDescriptor, keyVertexColorList, &hasKey);
					if (hasKey)
					{
						actionDescriptorProcs->GetDataLength(meshDescriptor,keyVertexColorList,&Mesh->colors);
						Mesh->colors /= sizeof (PI3DVector);
						Mesh->color = (PI3DVector*)PI3DMemoryAlloc (Mesh->colors * sizeof (PI3DVector));
						if(!Mesh->color)
							return 0;
							
						actionDescriptorProcs->GetData(meshDescriptor,keyVertexColorList,Mesh->color);
		#ifdef __ppc__
						for(index2=0;index2<Mesh->colors;index2++)
						{
							PI3DByteSwapDouble(&Mesh->color[index2][0]);
							PI3DByteSwapDouble(&Mesh->color[index2][1]);
							PI3DByteSwapDouble(&Mesh->color[index2][2]);
						}
		#endif
					}
					//UVs
					actionDescriptorProcs->HasKey(meshDescriptor, keyUVList, &hasKey);
					if (hasKey)
					{
						actionDescriptorProcs->GetDataLength(meshDescriptor,keyUVList,&Mesh->textures);
						Mesh->textures /= sizeof (PI3DPoint);
						Mesh->texture = (PI3DPoint*)PI3DMemoryAlloc (Mesh->textures * sizeof (PI3DPoint));
						if(!Mesh->texture)
							return 0;
							
						actionDescriptorProcs->GetData(meshDescriptor,keyUVList,Mesh->texture);
		#ifdef __ppc__
						for(index2=0;index2<Mesh->textures;index2++)
							{
							PI3DByteSwapDouble(&Mesh->texture[index2][0]);
							PI3DByteSwapDouble(&Mesh->texture[index2][1]);
							}
		#endif
					}

					//faces
					actionDescriptorProcs->HasKey(meshDescriptor, keyFaceList, &hasKey);
					if (hasKey)
					{
						actionDescriptorProcs->GetDataLength(meshDescriptor,keyFaceList,&Mesh->faces);
						Mesh->faces /= sizeof (PI3DFaceForReading);
						Mesh->face = (PI3DFace*)PI3DMemoryAlloc (Mesh->faces * sizeof (PI3DFace));
						if(!Mesh->face)
							return 0;
							
						actionDescriptorProcs->GetData(meshDescriptor,keyFaceList,Mesh->face);
						if(sizeof(void*) == 8)
							{
							PI3DFaceForReading *origianlFaceArray=(PI3DFaceForReading*)Mesh->face;
							for(index2=Mesh->faces-1;index2>0;index2--)
								{
								Mesh->face[index2].material=origianlFaceArray[index2].material;
								Mesh->face[index2].smoothing=origianlFaceArray[index2].smoothing;
								Mesh->face[index2].flags=origianlFaceArray[index2].flags;
								Mesh->face[index2].numPoints=origianlFaceArray[index2].numPoints;
								}	
							}	

						int32 numPoints=0;
						actionDescriptorProcs->GetDataLength(meshDescriptor,keyFaceIndexes,&numPoints);
						numPoints /= sizeof(int32);
						uint32 *faceDataBlob=(uint32*)PI3DMemoryAlloc (numPoints * sizeof (int32));
						if(!faceDataBlob)
							return 0;
							
						actionDescriptorProcs->GetData(meshDescriptor,keyFaceIndexes,faceDataBlob);

						uint32 currentPoint=0;
						for(index2=0;index2<Mesh->faces;index2++)
							{
		#ifdef __ppc__
							SWAP32_IF_BIGENDIAN(Mesh->face[index2].flags);
							SWAP32_IF_BIGENDIAN(Mesh->face[index2].smoothing);
							SWAP32_IF_BIGENDIAN(Mesh->face[index2].numPoints);
							SWAP32_IF_BIGENDIAN(Mesh->face[index2].material);
		#endif
							Mesh->face[index2].points=(int32*)PI3DMemoryAlloc (Mesh->face[index2].numPoints * sizeof (int32));
							if(!Mesh->face[index2].points)
								return 0;
								
							Mesh->face[index2].normals=(int32*)PI3DMemoryAlloc (Mesh->face[index2].numPoints * sizeof (int32));
							if(!Mesh->face[index2].normals)
								return 0;
							Mesh->face[index2].textures=(int32*)PI3DMemoryAlloc (Mesh->face[index2].numPoints * sizeof (int32));
							if(!Mesh->face[index2].textures)
								return 0;
							Mesh->face[index2].colors=(int32*)PI3DMemoryAlloc (Mesh->face[index2].numPoints * sizeof (int32));
							if(!Mesh->face[index2].colors)
								return 0;
							for(index3=0;index3<Mesh->face[index2].numPoints;index3++)
								{
		#ifdef __ppc__
								SWAP32_IF_BIGENDIAN(faceDataBlob[currentPoint]);
		#endif
								Mesh->face[index2].points[index3]=faceDataBlob[currentPoint];
								currentPoint++;
		#ifdef __ppc__
								SWAP32_IF_BIGENDIAN(faceDataBlob[currentPoint]);
		#endif
								Mesh->face[index2].normals[index3]=faceDataBlob[currentPoint];
								currentPoint++;
		#ifdef __ppc__
								SWAP32_IF_BIGENDIAN(faceDataBlob[currentPoint]);
		#endif
								Mesh->face[index2].textures[index3]=faceDataBlob[currentPoint];
								currentPoint++;
		#ifdef __ppc__
								SWAP32_IF_BIGENDIAN(faceDataBlob[currentPoint]);
		#endif
								Mesh->face[index2].colors[index3]=faceDataBlob[currentPoint];
								currentPoint++;
								}
							}
						
						PI3DMemoryFree (faceDataBlob);
					}
					
					//matrix
					actionDescriptorProcs->HasKey(meshDescriptor, keyMatrix, &hasKey);
					if (hasKey)
					{
						PIActionList matrixList;
						actionDescriptorProcs->GetList(meshDescriptor,keyMatrix,&matrixList);
						for( i=0;i<4;i++)
							{
							for( j=0;j<4;j++)
								
								{
								actionListProcs->GetFloat(matrixList,j+i*4,&tFloat);
								Mesh->matrix[i][j] = tFloat;
								}
							}
					}

					//flags
					actionDescriptorProcs->GetInteger(meshDescriptor,keyFlags,&Mesh->flags);

					//hidden
					actionDescriptorProcs->GetInteger(meshDescriptor,keyHidden,&Mesh->hidden);

					//shadow
					actionDescriptorProcs->GetInteger(meshDescriptor,keyShadow,&Mesh->shadow);

					//matrixPresent
					Boolean bPresent;
					actionDescriptorProcs->GetBoolean(meshDescriptor,keyHasMatrix,&bPresent);
					Mesh->matrixPresent = (bool)bPresent;

					//smoothingGroupPresent
					actionDescriptorProcs->GetBoolean(meshDescriptor,keyHasSmoothing,&bPresent);
					Mesh->smoothingGroupPresent = (bool)bPresent;
					
					//Is this mesh a volume
					actionDescriptorProcs->HasKey(meshDescriptor, keyMeshIsVolume, &hasKey);
					if (hasKey)
						{
						actionDescriptorProcs->GetBoolean(meshDescriptor,keyMeshIsVolume,&bPresent);
						Mesh->isVolume = (bool)bPresent;
						}
					//Is this mesh a shadow catcher
					actionDescriptorProcs->HasKey(meshDescriptor, keyMeshIsShadCtch, &hasKey);
					if (hasKey)
						{
						actionDescriptorProcs->GetBoolean(meshDescriptor,keyMeshIsShadCtch,&bPresent);
						Mesh->isShadowCatching = (bool)bPresent;
						}
					//Is this mesh a shadow caster
					actionDescriptorProcs->HasKey(meshDescriptor, keyMeshIsShadCast, &hasKey);
					if (hasKey)
						{
						actionDescriptorProcs->GetBoolean(meshDescriptor,keyMeshIsShadCast,&bPresent);
						Mesh->isShadowCasting = (bool)bPresent;
						}
					//Is this mesh a visible shadow catcher
					actionDescriptorProcs->HasKey(meshDescriptor, keyMeshIsShadInv, &hasKey);
					if (hasKey)
						{
						actionDescriptorProcs->GetBoolean(meshDescriptor,keyMeshIsShadInv,&bPresent);
						Mesh->isShadowInvisible = (bool)bPresent;
						}
					// Preservation
					Mesh->preservation = NULL;
					PIActionList PreservationActionList;
					actionDescriptorProcs->HasKey(meshDescriptor, keyPreservationClass, &hasKey);
					if(hasKey)
						{
						actionDescriptorProcs->GetList(meshDescriptor, keyPreservationClass, &PreservationActionList);
						PIActionDescriptor preservationDescriptor;
						tempType = keyPreservationObject;
						actionListProcs->GetObject(PreservationActionList, 0,&tempType,&preservationDescriptor);
						PI3DParseDescriptorIntoPreservation(procs, preservationDescriptor, &Mesh->preservation);		
						}
					actionDescriptorProcs->Free(meshDescriptor);
					
					PI3DListAdd((PI3DList **)&scene->meshList, reinterpret_cast<PI3DList *>(Mesh));
				}
			}
		actionListProcs->Free(meshActionList);
		}
	}

	//Dump the material properties
	actionDescriptorProcs->HasKey(actionDescriptor, keyMaterialList, &hasKey);
	if(hasKey)
	{
		//Get the materials
		PIActionList materialActionList;
		actionDescriptorProcs->GetList(actionDescriptor,keyMaterialList,&materialActionList);
		if(materialActionList)
		{
			actionListProcs->GetCount(materialActionList,&listSize);
			for(index=0;index<listSize;index++)
			{
				PI3DMaterial *MatProp = PI3DCreateMaterial();
				if(!MatProp)
					return 0;
					
				PIActionDescriptor materialDescriptor;
				tempType = keyMaterialClass;
				actionListProcs->GetObject(materialActionList,index,&tempType,&materialDescriptor);

				//name
				actionDescriptorProcs->GetString(materialDescriptor,keyName,MatProp->name,80);

				//ambient
				actionDescriptorProcs->GetFloat(materialDescriptor,keyAmbientRed, &tFloat);
				MatProp->ambient.red = tFloat;
				actionDescriptorProcs->GetFloat(materialDescriptor,keyAmbientGreen, &tFloat);
				MatProp->ambient.green = tFloat;
				actionDescriptorProcs->GetFloat(materialDescriptor,keyAmbientBlue, &tFloat);
				MatProp->ambient.blue = tFloat;

				//diffuse
				actionDescriptorProcs->GetFloat(materialDescriptor,keyDiffuseRed, &tFloat);
				MatProp->diffuse.red = tFloat;
				actionDescriptorProcs->GetFloat(materialDescriptor,keyDiffuseGreen, &tFloat);
				MatProp->diffuse.green = tFloat;
				actionDescriptorProcs->GetFloat(materialDescriptor,keyDiffuseBlue, &tFloat);
				MatProp->diffuse.blue = tFloat;

				//specular
				actionDescriptorProcs->GetFloat(materialDescriptor,keySpecularRed, &tFloat);
				MatProp->specular.red = tFloat;
				actionDescriptorProcs->GetFloat(materialDescriptor,keySpecularGreen, &tFloat);
				MatProp->specular.green = tFloat;
				actionDescriptorProcs->GetFloat(materialDescriptor,keySpecularBlue, &tFloat);
				MatProp->specular.blue = tFloat;

				//emissive
				actionDescriptorProcs->GetFloat(materialDescriptor,keyEmissiveRed, &tFloat);
				MatProp->emissive.red = tFloat;
				actionDescriptorProcs->GetFloat(materialDescriptor,keyEmissiveGreen, &tFloat);
				MatProp->emissive.green = tFloat;
				actionDescriptorProcs->GetFloat(materialDescriptor,keyEmissiveBlue, &tFloat);
				MatProp->emissive.blue = tFloat;

				//shininess
				actionDescriptorProcs->GetFloat(materialDescriptor,keyShininess, &tFloat);
				MatProp->shininess = tFloat;

				//glossiness
				actionDescriptorProcs->GetFloat(materialDescriptor,keyShininess2, &tFloat);
				MatProp->glossiness = tFloat;

				//transparency
				actionDescriptorProcs->GetFloat(materialDescriptor,keyTransparency, &tFloat);
				MatProp->transparency = tFloat;

				//reflection
				actionDescriptorProcs->GetFloat(materialDescriptor,keyReflection, &tFloat);
				MatProp->reflection = tFloat;

				//selfIllumination
				actionDescriptorProcs->GetInteger(materialDescriptor,keySelfIllumination, &MatProp->selfIllumination);

				//shading
				actionDescriptorProcs->GetInteger(materialDescriptor,keyShading, &tempInt);
				MatProp->shading = (PI3DShadingEnum)tempInt;

				//twoSided
				Boolean bProp;
				actionDescriptorProcs->GetBoolean(materialDescriptor,keyTwoSide, &bProp);
				MatProp->twoSided = (bool)bProp;
				//wireframe
				actionDescriptorProcs->GetBoolean(materialDescriptor,keyWireframe, &bProp);
				MatProp->wireframe = (bool)bProp;
				//decal
				actionDescriptorProcs->GetBoolean(materialDescriptor,keyDecal, &bProp);
				MatProp->decal = (bool)bProp;
				//wireframesize
				actionDescriptorProcs->GetFloat(materialDescriptor,keyWireframeSize, &tFloat);
				MatProp->wireframesize = tFloat;

				
				//indexofrefraction
				actionDescriptorProcs->HasKey(materialDescriptor, keyRefraction, &hasKey);
				if(hasKey)
					{
					actionDescriptorProcs->GetFloat(materialDescriptor,keyRefraction, &tFloat);
					MatProp->indexofrefraction = tFloat;
					}
				//maps
				PIActionList mapActionList;
				actionDescriptorProcs->GetList(materialDescriptor,keyMapList,&mapActionList);
				if(mapActionList)
					{
					for(index2=0;index2<kMax3DMapTypes;index2++)
					{
						PIActionDescriptor mapDescriptor;
						tempType = keyMapClass;
						actionListProcs->GetObject(mapActionList,index2,&tempType,&mapDescriptor);
						
						if(mapDescriptor)
						{
							//map type - not needed
							actionDescriptorProcs->GetInteger(mapDescriptor, keyType, &tempInt);

							//map name
							uint32 stringdatalength = 0;
							actionDescriptorProcs->GetStringLength(mapDescriptor,keyName,&stringdatalength);
							if (stringdatalength > 0)
								actionDescriptorProcs->GetString(mapDescriptor, keyName, MatProp->maps[index2].map, 256);
							else
							{
								OSErr errordata = noErr;
								int32 mapstrlen = 0;
								actionDescriptorProcs->GetDataLength(mapDescriptor,keyName,&mapstrlen);
								errordata = actionDescriptorProcs->GetData(mapDescriptor, keyName, MatProp->maps[index2].map);
#if MSWindows
								if (errordata == noErr)
									MatProp->maps[index2].map[mapstrlen] = '\0';
								else if(errordata != noErr)
									MatProp->maps[index2].map[0] = '\0';
#endif
							}
							
							//map strength
							actionDescriptorProcs->GetFloat(mapDescriptor, keyStrength, &tFloat);
							MatProp->maps[index2].strength = tFloat;

							//map uscale
							actionDescriptorProcs->GetFloat(mapDescriptor, keyUScale, &tFloat);
							MatProp->maps[index2].uscale = tFloat;

							//map vscale
							actionDescriptorProcs->GetFloat(mapDescriptor, keyVScale, &tFloat);
							MatProp->maps[index2].vscale = tFloat;

							//map uoffset
							actionDescriptorProcs->GetFloat(mapDescriptor, keyUOffset, &tFloat);
							MatProp->maps[index2].uoffset = tFloat;

							//map voffset
							actionDescriptorProcs->GetFloat(mapDescriptor, keyVOffset, &tFloat);
							MatProp->maps[index2].voffset = tFloat;

							//map angle
							actionDescriptorProcs->GetFloat(mapDescriptor, keyAngle, &tFloat);
							MatProp->maps[index2].angle = tFloat;

							//map flags
							int32 temp2;
							actionDescriptorProcs->GetInteger(mapDescriptor, keyFlags, &temp2);
							MatProp->maps[index2].flags = (PI3DTextureMapFlags)temp2;

							// map 3ds style
							Boolean bProp;
							actionDescriptorProcs->GetBoolean(mapDescriptor, key3DSStyle, &bProp);
							MatProp->maps[index2].mapIs3DSStyle = (bool)bProp;

							// custom texture type
							actionControlProcs->StringIDToTypeID(kmaximumStr,&typeID);
							actionDescriptorProcs->HasKey(mapDescriptor, typeID, &hasKey);
							
							if(hasKey)
							{
								actionControlProcs->StringIDToTypeID(kmaximumStr,&typeID);
								actionDescriptorProcs->GetFloat(mapDescriptor, typeID, &tFloat);
								MatProp->maps[index2].valueMax = tFloat;

								actionControlProcs->StringIDToTypeID(kminimumStr,&typeID);
								actionDescriptorProcs->GetFloat(mapDescriptor, typeID, &tFloat);
								MatProp->maps[index2].valueMin = tFloat;
							}
							else
							{
								MatProp->maps[index2].valueMax = 1;
								MatProp->maps[index2].valueMin = 0;
							}
							// custom texture type
							actionControlProcs->StringIDToTypeID(key3DPaintTypeKey,&typeID);
							actionDescriptorProcs->HasKey(mapDescriptor, typeID, &hasKey);
							if(hasKey)
							{
								OSErr errordata = noErr;
								int32 mapstrlen = 0;
								actionDescriptorProcs->GetDataLength(mapDescriptor,typeID,&mapstrlen);
								errordata = actionDescriptorProcs->GetData(mapDescriptor, typeID, MatProp->maps[index2].textureTypeName);
#if MSWindows
								if (errordata == noErr)
									MatProp->maps[index2].textureTypeName[mapstrlen] = '\0';
								else if(errordata != noErr)
									MatProp->maps[index2].textureTypeName[0] = '\0';
#endif
							}	
							//Texture layer name
							actionDescriptorProcs->HasKey(mapDescriptor, keyLayerName, &hasKey);
							if(hasKey)
							{
								OSErr errordata = noErr;
								int32 mapstrlen = 0;
								actionDescriptorProcs->GetDataLength(mapDescriptor,keyLayerName,&mapstrlen);
								errordata = actionDescriptorProcs->GetData(mapDescriptor, typeID, MatProp->maps[index2].layerName);
#if MSWindows
								if (errordata == noErr)
									MatProp->maps[index2].layerName[mapstrlen] = '\0';
								else if(errordata != noErr)
									MatProp->maps[index2].layerName[0] = '\0';
#endif
							}	
							actionDescriptorProcs->Free(mapDescriptor);
						}
					}	

						
						
				actionListProcs->Free(mapActionList);
				}

				// Preservation
				MatProp->preservation = NULL;
				PIActionList PreservationActionList;
				actionDescriptorProcs->HasKey(materialDescriptor, keyPreservationClass, &hasKey);
				if(hasKey)
					{
					actionDescriptorProcs->GetList(materialDescriptor, keyPreservationClass, &PreservationActionList);
					PIActionDescriptor preservationDescriptor;
					tempType = keyPreservationObject;
					actionListProcs->GetObject(PreservationActionList, 0,&tempType,&preservationDescriptor);
					PI3DParseDescriptorIntoPreservation(procs, preservationDescriptor, &MatProp->preservation);		
					}
				actionDescriptorProcs->Free(materialDescriptor);
				PI3DListAdd((PI3DList **)&scene->matPropList, reinterpret_cast<PI3DList *>(MatProp));
			}
		actionListProcs->Free(materialActionList);
		}
	}

	//Get the key frames
	actionDescriptorProcs->HasKey(actionDescriptor, keyKeyFrameList, &hasKey);
	if(hasKey)
	{
		//Get the materials
		PIActionList KeyFrameActionList;
		actionDescriptorProcs->GetList(actionDescriptor,keyKeyFrameList,&KeyFrameActionList);
		if(KeyFrameActionList)
		{
			actionListProcs->GetCount(KeyFrameActionList,&listSize);
			for(index=0;index<listSize;index++)
			{
				PI3DKeyFrame *KeyframeProp = (PI3DKeyFrame*)PI3DMemoryAlloc (sizeof (PI3DKeyFrame));
				if(!KeyframeProp)
					return 0;
					
				KeyframeProp->positionData = NULL;
				KeyframeProp->scaleData = NULL;
				KeyframeProp->rotationData = NULL;
				KeyframeProp->fOVData = NULL;
				KeyframeProp->rollData = NULL;
				KeyframeProp->hideData = NULL;
				KeyframeProp->hotData = NULL;
				KeyframeProp->colData = NULL;
				KeyframeProp->fallData =  NULL;
				KeyframeProp->morphData =  NULL;
				PIActionDescriptor KeyframeDescriptor;
				tempType = keyKeyFrameClass;
				actionListProcs->GetObject(KeyFrameActionList,index,&tempType,&KeyframeDescriptor);
				
				if(KeyframeDescriptor)
				{
					//keyframe props
					actionDescriptorProcs->GetString(KeyframeDescriptor,keyName,KeyframeProp->name,80);
					actionDescriptorProcs->GetString(KeyframeDescriptor,keyInstanceName,KeyframeProp->instancename,80);
					actionDescriptorProcs->GetInteger(KeyframeDescriptor,keyFlags1, &KeyframeProp->flags1);
					actionDescriptorProcs->GetInteger(KeyframeDescriptor,keyFlags2, &KeyframeProp->flags2);
					actionDescriptorProcs->GetInteger(KeyframeDescriptor,keyU3DNodeID, &KeyframeProp->nodeID);
					actionDescriptorProcs->GetInteger(KeyframeDescriptor,keyU3DParentID, &KeyframeProp->parentNodeID);
					int32 dmmy = 0;
					actionDescriptorProcs->GetInteger(KeyframeDescriptor,keyU3DType, &dmmy);
					KeyframeProp->nodeType = (PI3DNodeTypes)dmmy;
					actionDescriptorProcs->GetInteger(KeyframeDescriptor,keyFlags2, &KeyframeProp->flags2);

					//pivot pt
					PIActionList PivotActionList;
					actionDescriptorProcs->GetList(KeyframeDescriptor,keyPivotList,&PivotActionList);
					if(PivotActionList)
					{
						PIActionDescriptor pivotDescriptor;
						tempType = keyPivotObject;
						actionListProcs->GetObject(PivotActionList,0,&tempType,&pivotDescriptor);
						if(pivotDescriptor)
						{
							actionDescriptorProcs->GetFloat(pivotDescriptor,keyPivotX, &tFloat);
							KeyframeProp->pivot[0] = tFloat;
							actionDescriptorProcs->GetFloat(pivotDescriptor,keyPivotY, &tFloat);
							KeyframeProp->pivot[1] = tFloat;
							actionDescriptorProcs->GetFloat(pivotDescriptor,keyPivotZ, &tFloat);
							KeyframeProp->pivot[2] = tFloat;
							actionDescriptorProcs->Free(pivotDescriptor);
						}
						actionListProcs->Free(PivotActionList);
					}
					//matrix
					actionDescriptorProcs->HasKey(KeyframeDescriptor, keyMatrix, &hasKey);
					if (hasKey)
					{
					PIActionList matrixList;
					actionDescriptorProcs->GetList(KeyframeDescriptor,keyMatrix,&matrixList);
					for( i=0;i<4;i++)
						{
						for( j=0;j<4;j++)
							{
							actionListProcs->GetFloat(matrixList,j+i*4,&tFloat);
							KeyframeProp->localmatrix[i][j] = tFloat;
							}
						}
					}

					//pivot_present
					Boolean bPresent;
					actionDescriptorProcs->GetBoolean(KeyframeDescriptor,keyHasPivot,&bPresent);
					KeyframeProp->usepivot = (bool)bPresent;

					//matrix present
					bPresent = FALSE;
					actionDescriptorProcs->GetBoolean(KeyframeDescriptor,keyHasLocalMatrix,&bPresent);
					KeyframeProp->matrixPresent = (bool)bPresent;

					//animation range
					actionDescriptorProcs->GetBoolean(KeyframeDescriptor,keyHasRange,&bPresent);
					KeyframeProp->userange = (bool)bPresent;

					//position data
					OSErr err = noErr;
					int32 bDidit = 0;
					actionDescriptorProcs->HasKey(KeyframeDescriptor, keyframePositionList, &hasKey);
					if(hasKey)
					{
						uint32 animlistSize=0;
						uint32 animindex=0;
						PIActionList PositionActionList;
						actionDescriptorProcs->GetList(KeyframeDescriptor,keyframePositionList,&PositionActionList);
						if(PositionActionList)
						{
							actionListProcs->GetCount(PositionActionList,&animlistSize);
							for(animindex=0;animindex<animlistSize;animindex++)
							{
								PI3DAnimationData *Animation = (PI3DAnimationData*)PI3DMemoryAlloc (sizeof (PI3DAnimationData));
								if(!Animation)
									return 0;
								PIActionDescriptor PositionDescriptor;
								tempType = keyframePositionActionList;
								actionListProcs->GetObject(PositionActionList,animindex,&tempType,&PositionDescriptor);
								if(PositionDescriptor)
									{
									bDidit = PI3DParseAnimDataIntoScene(actionDescriptorProcs, actionListProcs, Animation, &PositionDescriptor);
								
									actionDescriptorProcs->Free(PositionDescriptor);
									PI3DListAdd((PI3DList **)&KeyframeProp->positionData, reinterpret_cast<PI3DList *>(Animation));
									}
							}
							actionListProcs->Free(PositionActionList);
						}
					}
					//scale data
					actionDescriptorProcs->HasKey(KeyframeDescriptor, keyframeScaleList, &hasKey);
					if(hasKey)
					{
						uint32 animlistSize=0;
						uint32 animindex=0;
						PIActionList ScaleActionList;
						err = actionDescriptorProcs->GetList(KeyframeDescriptor,keyframeScaleList,&ScaleActionList);
						if(ScaleActionList)
						{
							actionListProcs->GetCount(ScaleActionList,&animlistSize);
							for(animindex=0;animindex<animlistSize;animindex++)
							{
								PI3DAnimationData *Animation = (PI3DAnimationData*)PI3DMemoryAlloc (sizeof (PI3DAnimationData));
								if(!Animation)
									return 0;
								PIActionDescriptor ScaleDescriptor;
								tempType = keyframeScaleActionList;
								actionListProcs->GetObject(ScaleActionList,animindex,&tempType,&ScaleDescriptor);
								if(ScaleDescriptor)
								{
									bDidit = PI3DParseAnimDataIntoScene(actionDescriptorProcs, actionListProcs, Animation, &ScaleDescriptor);
									actionDescriptorProcs->Free(ScaleDescriptor);
									PI3DListAdd((PI3DList **)&KeyframeProp->scaleData, reinterpret_cast<PI3DList *>(Animation));
								}
							}
							actionListProcs->Free(ScaleActionList);
						}
					}
					//rotation data
					actionDescriptorProcs->HasKey(KeyframeDescriptor, keyframeRotationList, &hasKey);
					if(hasKey)
					{
						uint32 animlistSize=0;
						uint32 animindex=0;
						PIActionList RotationActionList;
						err = actionDescriptorProcs->GetList(KeyframeDescriptor,keyframeRotationList,&RotationActionList);
						if(RotationActionList)
						{
							actionListProcs->GetCount(RotationActionList,&animlistSize);
							for(animindex=0;animindex<animlistSize;animindex++)
							{
								PI3DAnimationData *Animation = (PI3DAnimationData*)PI3DMemoryAlloc (sizeof (PI3DAnimationData));
								if(!Animation)
									return 0;
								PIActionDescriptor RotationDescriptor;
								tempType = keyframeRotationActionList;
								actionListProcs->GetObject(RotationActionList,animindex,&tempType,&RotationDescriptor);
								if(RotationDescriptor)
								{
									bDidit = PI3DParseAnimDataIntoScene(actionDescriptorProcs, actionListProcs, Animation, &RotationDescriptor);
									actionDescriptorProcs->Free(RotationDescriptor);
									PI3DListAdd((PI3DList **)&KeyframeProp->rotationData, reinterpret_cast<PI3DList *>(Animation));
								}
							}
							actionListProcs->Free(RotationActionList);
						}
					}
					//FOV data
					actionDescriptorProcs->HasKey(KeyframeDescriptor, keyframeFOVList, &hasKey);
					if(hasKey)
					{
						uint32 animlistSize=0;
						uint32 animindex=0;
						PIActionList FOVActionList;
						err = actionDescriptorProcs->GetList(KeyframeDescriptor,keyframeFOVList,&FOVActionList);
						if(FOVActionList)
						{
							actionListProcs->GetCount(FOVActionList,&animlistSize);
							for(animindex=0;animindex<animlistSize;animindex++)
							{
								PI3DAnimationData *Animation = (PI3DAnimationData*)PI3DMemoryAlloc (sizeof (PI3DAnimationData));
								if(!Animation)
									return 0;
								PIActionDescriptor FOVDescriptor;
								tempType = keyframeFOVActionList;
								actionListProcs->GetObject(FOVActionList,animindex,&tempType,&FOVDescriptor);
								if(FOVDescriptor)
								{
									bDidit = PI3DParseAnimDataIntoScene(actionDescriptorProcs, actionListProcs, Animation, &FOVDescriptor);
									actionDescriptorProcs->Free(FOVDescriptor);
									PI3DListAdd((PI3DList **)&KeyframeProp->fOVData, reinterpret_cast<PI3DList *>(Animation));
								}
							}
							actionListProcs->Free(FOVActionList);
						}
					}
					//Roll data
					actionDescriptorProcs->HasKey(KeyframeDescriptor, keyframeRollList, &hasKey);
					if(hasKey)
					{
						uint32 animlistSize=0;
						uint32 animindex=0;
						PIActionList RollActionList;
						err = actionDescriptorProcs->GetList(KeyframeDescriptor,keyframeRollList,&RollActionList);
						if(RollActionList)
						{
							actionListProcs->GetCount(RollActionList,&animlistSize);
							for(animindex=0;animindex<animlistSize;animindex++)
							{
								PI3DAnimationData *Animation = (PI3DAnimationData*)PI3DMemoryAlloc (sizeof (PI3DAnimationData));
								if(!Animation)
									return 0;
								PIActionDescriptor RollDescriptor;
								tempType = keyframeRollActionList;
								actionListProcs->GetObject(RollActionList,animindex,&tempType,&RollDescriptor);
								if(RollDescriptor)
								{
									bDidit = PI3DParseAnimDataIntoScene(actionDescriptorProcs, actionListProcs, Animation, &RollDescriptor);
									actionDescriptorProcs->Free(RollDescriptor);
									PI3DListAdd((PI3DList **)&KeyframeProp->rollData, reinterpret_cast<PI3DList *>(Animation));
								}
							}
							actionListProcs->Free(RollActionList);
						}
					}
					//Col data
					actionDescriptorProcs->HasKey(KeyframeDescriptor, keyframeColorList, &hasKey);
					if(hasKey)
					{
						uint32 animlistSize=0;
						uint32 animindex=0;
						PIActionList ColorActionList;
						err = actionDescriptorProcs->GetList(KeyframeDescriptor,keyframeColorList,&ColorActionList);
						if(ColorActionList)
						{
							actionListProcs->GetCount(ColorActionList,&animlistSize);
							for(animindex=0;animindex<animlistSize;animindex++)
							{
								PI3DAnimationData *Animation = (PI3DAnimationData*)PI3DMemoryAlloc (sizeof (PI3DAnimationData));
								if(!Animation)
									return 0;
								PIActionDescriptor ColorDescriptor;
								tempType = keyframeColorActionList;
								actionListProcs->GetObject(ColorActionList,animindex,&tempType,&ColorDescriptor);
								if(ColorDescriptor)
								{
									bDidit = PI3DParseAnimDataIntoScene(actionDescriptorProcs, actionListProcs, Animation, &ColorDescriptor);
									actionDescriptorProcs->Free(ColorDescriptor);
									PI3DListAdd((PI3DList **)&KeyframeProp->colData, reinterpret_cast<PI3DList *>(Animation));
								}
							}
							actionListProcs->Free(ColorActionList);
						}
					}
					//Hot data
					actionDescriptorProcs->HasKey(KeyframeDescriptor, keyframeHotList, &hasKey);
					if(hasKey)
					{
						uint32 animlistSize=0;
						uint32 animindex=0;
						PIActionList HotActionList;
						err = actionDescriptorProcs->GetList(KeyframeDescriptor,keyframeHotList,&HotActionList);
						if(HotActionList)
						{
							actionListProcs->GetCount(HotActionList,&animlistSize);
							for(animindex=0;animindex<animlistSize;animindex++)
							{
								PI3DAnimationData *Animation = (PI3DAnimationData*)PI3DMemoryAlloc (sizeof (PI3DAnimationData));
								if(!Animation)
									return 0;
								PIActionDescriptor HotDescriptor;
								tempType = keyframeHotActionList;
								actionListProcs->GetObject(HotActionList,animindex,&tempType,&HotDescriptor);
								if(HotDescriptor)
								{
									bDidit = PI3DParseAnimDataIntoScene(actionDescriptorProcs, actionListProcs, Animation, &HotDescriptor);
									actionDescriptorProcs->Free(HotDescriptor);
									PI3DListAdd((PI3DList **)&KeyframeProp->hotData, reinterpret_cast<PI3DList *>(Animation));
								}
							}
							actionListProcs->Free(HotActionList);
						}
					}
					//Fall data
					actionDescriptorProcs->HasKey(KeyframeDescriptor, keyframeFalloffList, &hasKey);
					if(hasKey)
					{
						uint32 animlistSize=0;
						uint32 animindex=0;
						PIActionList FallActionList;
						err = actionDescriptorProcs->GetList(KeyframeDescriptor,keyframeFalloffList,&FallActionList);
						if(FallActionList)
						{
							actionListProcs->GetCount(FallActionList,&animlistSize);
							for(animindex=0;animindex<animlistSize;animindex++)
							{
								PI3DAnimationData *Animation = (PI3DAnimationData*)PI3DMemoryAlloc (sizeof (PI3DAnimationData));
								if(!Animation)
									return 0;
								PIActionDescriptor FallDescriptor;
								tempType = keyframeFalloffActionList;
								actionListProcs->GetObject(FallActionList,animindex,&tempType,&FallDescriptor);
								if(FallDescriptor)
								{
									bDidit = PI3DParseAnimDataIntoScene(actionDescriptorProcs, actionListProcs, Animation, &FallDescriptor);
									actionDescriptorProcs->Free(FallDescriptor);
									PI3DListAdd((PI3DList **)&KeyframeProp->fallData, reinterpret_cast<PI3DList *>(Animation));
								}
							}
							actionListProcs->Free(FallActionList);
						}
					}
					//Hide data
					actionDescriptorProcs->HasKey(KeyframeDescriptor, keyframeHideList, &hasKey);
					if(hasKey)
					{
						uint32 animlistSize=0;
						uint32 animindex=0;
						PIActionList HideActionList;
						err = actionDescriptorProcs->GetList(KeyframeDescriptor,keyframeHideList,&HideActionList);
						if(HideActionList)
						{
							actionListProcs->GetCount(HideActionList,&animlistSize);
							for(animindex=0;animindex<animlistSize;animindex++)
							{
								PI3DAnimationData *Animation = (PI3DAnimationData*)PI3DMemoryAlloc (sizeof (PI3DAnimationData));
								if(!Animation)
									return 0;
								PIActionDescriptor HideDescriptor;
								tempType = keyframeHideActionList;
								actionListProcs->GetObject(HideActionList,animindex,&tempType,&HideDescriptor);
								if(HideDescriptor)
								{
									bDidit = PI3DParseAnimDataIntoScene(actionDescriptorProcs, actionListProcs, Animation, &HideDescriptor);
									actionDescriptorProcs->Free(HideDescriptor);
									PI3DListAdd((PI3DList **)&KeyframeProp->hideData, reinterpret_cast<PI3DList *>(Animation));
								}
							}
							actionListProcs->Free(HideActionList);
						}
					}
					//morph data
					actionDescriptorProcs->HasKey(KeyframeDescriptor, keyframeMorphList, &hasKey);
					if(hasKey)
					{
						uint32 animlistSize=0;
						uint32 animindex=0;
						PIActionList MorphActionList;
						err = actionDescriptorProcs->GetList(KeyframeDescriptor,keyframeMorphList,&MorphActionList);
						if(MorphActionList)
						{
							actionListProcs->GetCount(MorphActionList,&animlistSize);
							for(animindex=0;animindex<animlistSize;animindex++)
							{
								PI3DAnimationData *Animation = (PI3DAnimationData*)PI3DMemoryAlloc (sizeof (PI3DAnimationData));
								if(!Animation)
									return 0;
								PIActionDescriptor MorphDescriptor;
								tempType = keyframeMorphActionList;
								actionListProcs->GetObject(MorphActionList,animindex,&tempType,&MorphDescriptor);
								if(MorphDescriptor)
								{
									bDidit = PI3DParseAnimDataIntoScene(actionDescriptorProcs, actionListProcs, Animation, &MorphDescriptor);
									actionDescriptorProcs->Free(MorphDescriptor);
									PI3DListAdd((PI3DList **)&KeyframeProp->morphData, reinterpret_cast<PI3DList *>(Animation));
								}
							}
							actionListProcs->Free(MorphActionList);
						}
					}
					

					// Preservation
					KeyframeProp->preservation = NULL;
					PIActionList PreservationActionList;
					actionDescriptorProcs->HasKey(KeyframeDescriptor, keyPreservationClass, &hasKey);
					if(hasKey)
						{
						actionDescriptorProcs->GetList(KeyframeDescriptor, keyPreservationClass, &PreservationActionList);
						PIActionDescriptor preservationDescriptor;
						tempType = keyPreservationObject;
						actionListProcs->GetObject(PreservationActionList, 0,&tempType,&preservationDescriptor);
						PI3DParseDescriptorIntoPreservation(procs, preservationDescriptor, &KeyframeProp->preservation);		
						}

					actionDescriptorProcs->Free(KeyframeDescriptor);
					PI3DListAdd((PI3DList **)&scene->keyFrameList, reinterpret_cast<PI3DList *>(KeyframeProp));
				}
			}
		
		actionListProcs->Free(KeyFrameActionList);
		}
	}

	*inScene = scene;
	return 1;
}

/******************************************************************************/
#if Macintosh
#pragma mark -
#endif
/******************************************************************************/

	
uint32 PI3DStringLengthW(const wchar_t *string)
	{
	if(!string)
		return 0;
		
	uint32 strLen=0;
	
	while(string[strLen] != 0) 
		strLen++;
	
	return strLen;
	}
uint32 PI3DStringLength16(const uint16 *string)
	{
	if(!string)
		return 0;
		
	uint32 strLen=0;
	
	while(string[strLen] != 0) 
		strLen++;
	
	return strLen;
	}	

void PI3DStringCopy16C(char *destString,const uint16 *sourceString)
	{
	if(!destString || !sourceString)
		return;
		
	//••• We hope dest has enough memory
	uint32 strLen=PI3DStringLength16(sourceString);
	for(uint16 i=0;i<=strLen;i++)
		destString[i]=(char)sourceString[i];
	
	destString[strLen]=0;
	}
void PI3DStringCopyW16(uint16 *destString,const wchar_t *sourceString)
{
	if(!destString || !sourceString)
		return;
	
	//••• We hope dest has enough memory
	uint32 strLen=PI3DStringLengthW(sourceString);
	for(uint16 i=0;i<=strLen;i++)
		destString[i]=(char)sourceString[i];
	
	destString[strLen]=0;
}
void PI3DStringCopyWC(char *destString,const wchar_t *sourceString)
	{
	if(!destString || !sourceString)
		return;
		
	//••• We hope dest has enough memory
	uint32 strLen=PI3DStringLengthW(sourceString);
	for(uint16 i=0;i<=strLen;i++)
		destString[i]=(char)sourceString[i];
	
	destString[strLen]=0;
	}

void PI3DStringCopyC16(uint16 *destString,const char *sourceString)
	{
	if(!destString || !sourceString)
		return;
		
	//••• We hope dest has enough memory
	size_t strLen=strlen(sourceString);
	for(uint16 i=0;i<=strLen;i++)
		destString[i]=(uint16)sourceString[i];
	
	destString[strLen]=0;
	}
	
void PI3DStringCopy16W(wchar_t *destString,const uint16 *sourceString)
	{
	if(!destString || !sourceString)
		return;
		
	//••• We hope dest has enough memory
	uint32 strLen=PI3DStringLength16(sourceString);
	for(uint16 i=0;i<=strLen;i++)
		destString[i]=(wchar_t)sourceString[i];
	
	destString[strLen]=0;
	}	

void PI3DStringAppend16(uint16 *destString, uint32 destSize, uint16 *sourceString)
	{
	if(!destString || !sourceString)
		return;
	
	//¥¥¥ÊWe hope dest has enough memory
	uint32 strLenToAppend=PI3DStringLength16(sourceString);
	uint32 strLenInitial=PI3DStringLength16(destString);
	
	for(uint32 i=0;i<=strLenToAppend;i++)
	{
		if (strLenInitial+i < destSize)
			destString[strLenInitial+i]=sourceString[i];
	}
	
	if (strLenToAppend+strLenInitial < destSize)
		destString[strLenToAppend+strLenInitial]=0;
	destString[destSize - 1]=0;
	}	
void PI3DStringAppend16C(uint16 *destString, uint32 destSize, char *sourceString)
	{
	if(!destString || !sourceString)
		return;
	
	//¥¥¥ÊWe hope dest has enough memory
	uint32 strLenToAppend=strlen(sourceString);
	uint32 strLenInitial=PI3DStringLength16(destString);
	
	for(uint32 i=0;i<=strLenToAppend;i++)
	{
		if (strLenInitial+i < destSize)
			destString[strLenInitial+i]=sourceString[i];
	}
	
	if (strLenToAppend+strLenInitial < destSize)
		destString[strLenToAppend+strLenInitial]=0;
	destString[destSize - 1]=0;
	}	
//•••Remember to get rid of this stuff when we get rid of data and dataSize
void PI3DGetPathFromSpec(SPPlatformFileSpecificationW *fileSpec,uint16 *path)
	{
	if(!fileSpec || !path)
		return;
		
	#if MSWindows
	 uint32 strLen=PI3DStringLength16(fileSpec->mReference);
	 for(uint16 i=0;i<=strLen;i++)
		path[i]=fileSpec->mReference[i];
		
	 path[strLen]=0;
	#else
	 CFURLRef url = CFURLCreateFromFSRef( kCFAllocatorDefault, &fileSpec->mReference );
	 CFStringRef cfString = NULL;
	 char tempPath[2048];
	 if ( url != NULL )
		{
		cfString = CFURLCopyFileSystemPath( url, kCFURLPOSIXPathStyle );
		CFStringGetCString(cfString,(char*)tempPath,2048,kCFStringEncodingUTF8);
		int32 len=CFStringGetLength(cfString);
		CFRelease( url );
		path[len]=0;
		
		//This function wants us to fill in a 2 byte path, so we'll copy our utf8 string into it
		PI3DStringCopyC16(path,tempPath);
		}
		
	#endif
	}

/* string splitter */
Boolean PI3DSplitString(char *leftStr, char *srcStr, bool splitOnWhitespace, char splitChar)
{ // returns true if a split occurred
	int32	i;
	int32	leftI = 0;
	int32	srcI = 0;

	leftStr[leftI] = '\0';
	while (true)
	{
		if (srcStr[srcI] == '\0')
		{
			srcStr[0] = '\0';
			break;
		}
		if (srcStr[srcI] == splitChar || (splitOnWhitespace && isspace(srcStr[srcI])))
		{
			while (srcStr[srcI] == splitChar || (splitOnWhitespace && isspace(srcStr[srcI])))
				srcI++;
			i = 0;
			while (srcStr[i + srcI] != '\0')
			{
				srcStr[i] = srcStr[i + srcI];
				i++;
			}
			srcStr[i] = '\0';
			break;
		}
		leftStr[leftI++] = srcStr[srcI++];
	}
	leftStr[leftI] = '\0';
	if (leftStr[0] == '\0')
		return false;
	else
		return true;
}

/* Trim whitespace */
void PI3DTrimLeft(char *str)
{
	size_t		i, copyFrom;
	size_t		len = strlen(str);

	copyFrom = len;
	for (i = 0; i < len; i++)
	{
		if (!isspace(str[i]))
		{
			copyFrom = i;
			break;
		}
	}

	if (copyFrom < len)
	{
		i = 0;
		while (copyFrom < len)
		{
			str[i] = str[copyFrom];
			i++;
			copyFrom++;
		}
		str[i] = '\0';
	}
	else
		str[0] = '\0';
}

void PI3DTrimRight(char *str)
{
	size_t		i;
	size_t		len = strlen(str);

	i = len;
	while (i)
	{
		if (!isspace(str[i - 1]))
		{
			str[i] = '\0';
			len = i;
			break;
		}
		i--;
	}
	if (i == 0)
		str[0] = '\0';
}

/* Adding lights to light list */
void PI3DAddInfiniteLight(PI3DLight **lightList,float dirx, float diry, float dirz, float red, float green, float blue, bool cameraBound)
{
	PI3DLight	*theNewLight=PI3DCreateLight ();
	if(!theNewLight)
		return;
	int32 lightCount=1;
	PI3DLight *tempLight= *lightList;
	while(tempLight)
		{
		if(tempLight->type == PI3DInfiniteLight)
			lightCount++;
		tempLight = (PI3DLight*)tempLight->next;
		}

#ifdef __PSDefines__
	ZString lightName("$$$/3D/DefaultLightNames/InfiniteLight=Infinite Light ^0");
	lightName.Replace(0,ZString::RomanizationOf(lightCount));
	lightName.AsUTF8String((unsigned char*)theNewLight->name, 256);
#else		
	char lightname[256];
	sprintf(lightname,"Infinite Light %d",lightCount);
	strcpy (theNewLight->name, lightname);
#endif
	theNewLight->attenuation = false;
	theNewLight->attenuationAbc.a = 0.0f;
	theNewLight->attenuationAbc.b = 0.0f;
	theNewLight->attenuationAbc.c = 0.0f;
	theNewLight->attenuationType = PI3DLinearAttenuation;
	theNewLight->col.red = red;
	theNewLight->col.green = green;
	theNewLight->col.blue = blue;
	theNewLight->falloff = 45.0f;
	theNewLight->hotspot = 0.7f*theNewLight->falloff;
	theNewLight->innerRadius = 0;
	theNewLight->multiple = 1.0f;
	theNewLight->outerRadius = 1.0;
	theNewLight->pos[0] = 0;
	theNewLight->pos[1] = 0;
	theNewLight->pos[2] = 0;
	theNewLight->target[0] = dirx;
	theNewLight->target[1] = diry;
	theNewLight->target[2] = dirz;
	theNewLight->shadowFlag = true;
	theNewLight->ssmultiple = 0.0f;
	theNewLight->type = PI3DInfiniteLight;
	theNewLight->cameraBound = cameraBound;
	PI3DListAdd ((PI3DList **)lightList, reinterpret_cast<PI3DList *>(theNewLight));
}

void PI3DAddPointLight(PI3DLight **lightList,float cx, float cy, float cz, float red, float green, float blue, bool cameraBound)
{
	PI3DLight	*theNewLight=PI3DCreateLight ();
	if(!theNewLight)
		return;
	PI3DLight *tempLight= *lightList;
	int32 lightCount=1;
	while(tempLight)
		{
		if(tempLight->type == PI3DPointLight)
			lightCount++;
		tempLight = (PI3DLight*)tempLight->next;
		}
		
#ifdef __PSDefines__
	ZString lightName("$$$/3D/DefaultLightNames/PointLight=Point Light ^0");
	lightName.Replace(0,ZString::RomanizationOf(lightCount));
	lightName.AsUTF8String((unsigned char*)theNewLight->name, 256);
#else		
	char lightname[256];
	sprintf(lightname,"Point Light %d",lightCount);
	strcpy (theNewLight->name, lightname);
#endif
	theNewLight->attenuation = false;
	theNewLight->attenuationAbc.a = 0.0f;
	theNewLight->attenuationAbc.b = 0.0f;
	theNewLight->attenuationAbc.c = 0.0f;
	theNewLight->attenuationType = PI3DLinearAttenuation;
	theNewLight->col.red = red;
	theNewLight->col.green = green;
	theNewLight->col.blue = blue;
	theNewLight->falloff = 45.0f;
	theNewLight->hotspot = 0.7f*theNewLight->falloff;
	theNewLight->innerRadius = 0;
	theNewLight->multiple = 1.0f;
	theNewLight->outerRadius = 1.0;
	theNewLight->pos[0] = cx;
	theNewLight->pos[1] = cy;
	theNewLight->pos[2] = cz;
	theNewLight->target[0] = 0;
	theNewLight->target[1] = 0;
	theNewLight->target[2] = 0;
	theNewLight->shadowFlag = true;
	theNewLight->ssmultiple = 0.0f;
	theNewLight->type = PI3DPointLight;
	theNewLight->cameraBound = cameraBound;
	PI3DListAdd ((PI3DList **)lightList, reinterpret_cast<PI3DList *>(theNewLight));
}

void PI3DAddSpotLight(PI3DLight **lightList,float cx, float cy, float cz, float red, float green, float blue, bool cameraBound)
{
	PI3DLight	*theNewLight=PI3DCreateLight ();
	if(!theNewLight)
		return;
	PI3DLight *tempLight= *lightList;
	int32 lightCount=1;
	while(tempLight)
		{
		if(tempLight->type == PI3DSpotLight)
			lightCount++;
		tempLight = (PI3DLight*)tempLight->next;
		}
		
#ifdef __PSDefines__
	ZString lightName("$$$/3D/DefaultLightNames/SpotLight=Spot Light ^0");
	lightName.Replace(0,ZString::RomanizationOf(lightCount));
	lightName.AsUTF8String((unsigned char*)theNewLight->name, 256);
#else		
	char lightname[256];
	sprintf(lightname,"Spot Light %d",lightCount);
	strcpy (theNewLight->name, lightname);
#endif
	theNewLight->attenuation = false;
	theNewLight->attenuationAbc.a = 0.0f;
	theNewLight->attenuationAbc.b = 0.0f;
	theNewLight->attenuationAbc.c = 0.0f;
	theNewLight->attenuationType = PI3DLinearAttenuation;
	theNewLight->col.red = red;
	theNewLight->col.green = green;
	theNewLight->col.blue = blue;
	theNewLight->falloff = 45.0f;
	theNewLight->hotspot = 0.7f*theNewLight->falloff;
	theNewLight->innerRadius = 0;
	theNewLight->multiple = 1.0f;
	theNewLight->outerRadius = 1.0;
	theNewLight->pos[0] = cx;
	theNewLight->pos[1] = cy;
	theNewLight->pos[2] = cz;
	theNewLight->target[0] = 0;
	theNewLight->target[1] = 0;
	theNewLight->target[2] = 0;
	theNewLight->shadowFlag = true;
	theNewLight->ssmultiple = 0.0f;
	theNewLight->type = PI3DSpotLight;
	theNewLight->cameraBound = cameraBound;
	PI3DListAdd ((PI3DList **)lightList, reinterpret_cast<PI3DList *>(theNewLight));
}

/* Adding lights to light list */
PI3DLight* PI3DCreateInfiniteLight(float dirx, float diry, float dirz, float red, float green, float blue, bool cameraBound, int lightCount)
{
	PI3DLight	*theNewLight=PI3DCreateLight ();
	if(!theNewLight)
		return NULL;
#ifdef __PSDefines__
	ZString lightName("$$$/3D/DefaultLightNames/InfiniteLight=Infinite Light ^0");
	lightName.Replace(0,ZString::RomanizationOf(lightCount));
	lightName.AsUTF8String((unsigned char*)theNewLight->name, 256);
#else		
	char lightname[256];
	sprintf(lightname,"Infinite Light %d",lightCount);
	strcpy (theNewLight->name, lightname);
#endif
	theNewLight->attenuation = false;
	theNewLight->attenuationAbc.a = 0.0f;
	theNewLight->attenuationAbc.b = 0.0f;
	theNewLight->attenuationAbc.c = 0.0f;
	theNewLight->attenuationType = PI3DLinearAttenuation;
	theNewLight->col.red = red;
	theNewLight->col.green = green;
	theNewLight->col.blue = blue;
	theNewLight->falloff = 45.0f;
	theNewLight->hotspot = 0.7f*theNewLight->falloff;
	theNewLight->innerRadius = 0;
	theNewLight->multiple = 1.0f;
	theNewLight->outerRadius = 1.0;
	theNewLight->pos[0] = 0;
	theNewLight->pos[1] = 0;
	theNewLight->pos[2] = 0;
	theNewLight->target[0] = dirx;
	theNewLight->target[1] = diry;
	theNewLight->target[2] = dirz;
	theNewLight->shadowFlag = true;
	theNewLight->ssmultiple = 0.0f;
	theNewLight->type = PI3DInfiniteLight;
	theNewLight->cameraBound = cameraBound;
	return theNewLight;
}

PI3DLight* PI3DCreatePointLight(float cx, float cy, float cz, float red, float green, float blue, bool cameraBound, int lightCount)
{
	PI3DLight	*theNewLight=PI3DCreateLight ();
	
	if(!theNewLight)
		return NULL;
		
#ifdef __PSDefines__
	ZString lightName("$$$/3D/DefaultLightNames/PointLight=Point Light ^0");
	lightName.Replace(0,ZString::RomanizationOf(lightCount));
	lightName.AsUTF8String((unsigned char*)theNewLight->name, 256);
#else		
	char lightname[256];
	sprintf(lightname,"Point Light %d",lightCount);
	strcpy (theNewLight->name, lightname);
#endif
	theNewLight->attenuation = false;
	theNewLight->attenuationAbc.a = 0.0f;
	theNewLight->attenuationAbc.b = 0.0f;
	theNewLight->attenuationAbc.c = 0.0f;
	theNewLight->attenuationType = PI3DLinearAttenuation;
	theNewLight->col.red = red;
	theNewLight->col.green = green;
	theNewLight->col.blue = blue;
	theNewLight->falloff = 45.0f;
	theNewLight->hotspot = 0.7f*theNewLight->falloff;
	theNewLight->innerRadius = 0;
	theNewLight->multiple = 1.0f;
	theNewLight->outerRadius = 1.0;
	theNewLight->pos[0] = cx;
	theNewLight->pos[1] = cy;
	theNewLight->pos[2] = cz;
	theNewLight->target[0] = 0;
	theNewLight->target[1] = 0;
	theNewLight->target[2] = 0;
	theNewLight->shadowFlag = true;
	theNewLight->ssmultiple = 0.0f;
	theNewLight->type = PI3DPointLight;
	theNewLight->cameraBound = cameraBound;
	return theNewLight;
}

PI3DLight* PI3DCreateSpotLight(float cx, float cy, float cz, float red, float green, float blue, bool cameraBound, int lightCount)
{
	PI3DLight	*theNewLight=PI3DCreateLight ();
	if(!theNewLight)
		return NULL;
#ifdef __PSDefines__
	ZString lightName("$$$/3D/DefaultLightNames/SpotLight=Spot Light ^0");
	lightName.Replace(0,ZString::RomanizationOf(lightCount));
	lightName.AsUTF8String((unsigned char*)theNewLight->name, 256);
#else		
	char lightname[256];
	sprintf(lightname,"Spot Light %d",lightCount);
	strcpy (theNewLight->name, lightname);
#endif
	theNewLight->attenuation = false;
	theNewLight->attenuationAbc.a = 0.0f;
	theNewLight->attenuationAbc.b = 0.0f;
	theNewLight->attenuationAbc.c = 0.0f;
	theNewLight->attenuationType = PI3DLinearAttenuation;
	theNewLight->col.red = red;
	theNewLight->col.green = green;
	theNewLight->col.blue = blue;
	theNewLight->falloff = 45.0f;
	theNewLight->hotspot = 0.7f*theNewLight->falloff;
	theNewLight->innerRadius = 0;
	theNewLight->multiple = 1.0f;
	theNewLight->outerRadius = 1.0;
	theNewLight->pos[0] = cx;
	theNewLight->pos[1] = cy;
	theNewLight->pos[2] = cz;
	theNewLight->target[0] = 0;
	theNewLight->target[1] = 0;
	theNewLight->target[2] = 0;
	theNewLight->shadowFlag = true;
	theNewLight->ssmultiple = 0.0f;
	theNewLight->type = PI3DSpotLight;
	theNewLight->cameraBound = cameraBound;
	return theNewLight;
}