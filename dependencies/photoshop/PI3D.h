// ADOBE SYSTEMS INCORPORATED
// Copyright  2007 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this 
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.
//-------------------------------------------------------------------
//
//	File:
//		PI3D.h
//
//	Copyright 2007, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Distribution:
//		PUBLIC
//
//	Description:
//		This file describes version 1 of Photoshop's plug-in 3d interface.
//
//	Use:
//		Include in all plug-ins using 3D.
//		
//
#ifndef __PI3D__

#ifndef __PI3D__
#define __PI3D__

#if PRAGMA_ONCE
#pragma once
#endif
#include "PITypes.h"
#include "PIActions.h"
#include "PIGeneral.h"
#include "SPBasic.h"
#include "SPFiles.h"


/* Operation selectors */
#define formatSelectorPrepareRenderer		100
#define formatSelectorRender				101
#define formatSelectorFinishRenderer		102
#define formatSelectorSetObjectPosition		103
#define formatSelectorGoToCamera			104
#define formatSelectorSetLightMode			105
#define formatSelectorSetRenderMode			106
#define formatSelectorSetCrossSection		107
#define formatSelectorUpdateTexture			108
#define formatSelectorSetObjectScale		109
#define formatSelectorPreprocessU3D			110
#define formatSelectorGetDataFromEngine		111
#define formatSelectorReloadScene			112
#define formatSelectorGetTexturePointer		113
#define formatSelectorUpdateTextureInMem	114
#define formatSelectorSaveTexture			115
#define formatSelectorReleaseResources		116
#define formatSelectorSetTransferFunction	117

/*	We keep various pieces of information about the file format in the PiMI resource.
 Here is the structure of that resource. */

/******************************************************************************/
#define PIFmtSupports3DProperty				'fm3d'		/* 'fm3d' plug-in supports 3D */

#define key3DCurrentEngine			"key3DCurrentEngine"

#define k3DXAxis 0
#define k3DYAxis 1
#define k3DZAxis 2

#define kMax3DTextures				4096
#define kMax3DInterfaceStringLength 512
#define kMax3DURLStringLength		2048

#define kRenderQualityHigh 0
#define kRenderQualityLow 1

#define k3DToolOrbit 0
#define k3DToolRotate 1
#define k3DToolPan 2
#define k3DToolWalk 3
#define k3DToolZoom 4
#define k3DToolCrossSection 5

#define k3DDataTypeU3D 0
#define k3DDataType3DS 1
#define k3DDataTypeDAE 2
#define k3DDataTypeKMZ 3

#define k3DMapTypeTexture 0x0001
#define k3DMapTypeReflection 0x0002
#define k3DMapTypeBump 0x0004
#define k3DMapTypeSpecular 0x0008
#define k3DMapTypeOpacity 0x0010
#define k3DMapTypeShininess 0x0020
#define k3DMapTypeSelfIllumination 0x0040


/*	Keys in the 3D Scene Descriptor */

/******************************************************************************/
#define keyZ				'Z   '

#define keyGlobalAmbientRed  'GamR'
#define keyGlobalAmbientGreen	'GamG'
#define keyGlobalAmbientBlue	'GamB'

#define keyGlobalUnits		'Gunt'
#define keyGlobalAnimStart	'Gast'
#define keyGlobalFrames		'Gfrm'
#define keyGlobalFPS		'Gfps'
#define keyCustomData		'GRNm'

#define keyGlobalGeoRef		'GeoR'
#define keyGlobalGeoList	'GeoL'
#define keyGlobalGeoLat		'GeLa'
#define keyGlobalGeoLon		'GeLo'
#define keyGlobalGeoAlt		'GeAl'
#define keyGlobalGeoAltM	'GeAm'
#define keyGlobalGeoHead	'GeHd'
#define keyGlobalGeoTilt	'GeTt'
#define keyGlobalGeoRoll	'GeRl'
#define keyGlobalGeoSclX	'GeSx'
#define keyGlobalGeoSclY	'GeSy'
#define keyGlobalGeoSclZ	'GeSz'

#define keyLightList		'lite'
#define keyLightClass		'litc'
#define keyCameraList		'caml'
#define keyCameraClass		'camc'
#define keyMeshList			'mshl'
#define keyMeshClass		'msho'
#define keyMaterialList		'mtll'
#define keyMaterialClass	'mtlo'
#define keyMeshIsVolume		'misv'
#define keyMeshIsShadCtch	'misc'
#define keyMeshIsShadCast	'miss'
#define keyMeshIsShadInv	'misi'
#define keyRerenderPaint	'rrwp'
#define keyRerenderOnUp		'rrmu'
#define keyModelHadTrans	'mhtp'

#define keyVertexCount		'verc'
#define keyPolyCount		'plyc'

#define keyMultiple			'mult'
#define keySSMultiple		'ssml'
#define keyIsOn				'ison'
#define keyTargetX			'tarx'
#define keyTargetY			'tary'
#define keyTargetZ			'tarz'
#define keyHotspot			'hots'
#define keyShadow			'shdw'
#define keyAttenuation		'attn'
#define keyAttenType		'attt'
#define keyAttenA			'atta'
#define keyAttenB			'attb'
#define keyAttenC			'attc'
#define keyOuterRadius		'orad'
#define keyInnerRadius		'irad'
#define keyBank				'bank'
#define keyOrtho			'orth'
#define keyApsect			'aspr'
#define keyZoomFactor		'zmfc'
#define keyFlags			'flag'
#define keySmoothing		'smth'
#define key3DIndexList		'indl'
#define key3DIndex			'indx'
#define keyHidden			'hidn'
#define keyHasMatrix		'hmat'
#define keyHasSmoothing		'hsmt'
#define keyAmbientRed		'ared'
#define keyAmbientGreen		'agrn'
#define keyAmbientBlue		'ablu'
#define keyDiffuseRed		'dred'
#define keyDiffuseGreen		'dgrn'
#define keyDiffuseBlue		'dblu'
#define keySpecularRed		'sred'
#define keySpecularGreen	'sgrn'
#define keySpecularBlue		'sblu'
#define keyEmissiveRed		'ered'
#define keyEmissiveGreen	'egrn'
#define keyEmissiveBlue		'eblu'
#define keyShininess		'shin'
#define keyShininess2		'shi2'
#define keyReflection		'refl'
#define keySelfIllumination	'self'
#define keyShading			'shad'
#define keyTwoSide			'twos'
#define keyWireframe		'wire'
#define keyDecal			'decl'
#define key3DSStyle			'msty'
#define keyWireframeSize	'wfsz'
#define keyUScale			'uscl'
#define keyVScale			'vscl'
#define keyUOffset			'uoff'
#define keyVOffset			'voff'
#define keyFullMapName		'fMnM'
#define keyVertexList		'verl'
#define keyNormalList		'nrml'
#define keyUVList			'uvl '
#define keylineColorList	'lncl'
#define keylineMaterialList 'lnmt'
#define keyVertexColorList	'vnvl'
#define keyLineList			'lnfl'
#define keyFaceList			'facl'
#define keyFaceIndexes		'faci'
#define keyMapList			'mapl'
#define keyMapClass			'mapo'
#define keyMaterialIndList	'matl'
#define keyMaterialIndObj	'mato'
#define keyRefraction		'RfAc'

#define keyPreservationClass  'pRes'
#define keyPreservationObject 'pRob'

#define keyKeys				'KeyS'
#define keyFrameNumber		'FrNm'
#define keyFlagsAnim		'FlAn'
#define keyNameAnim			'FlNn'
#define keyTension			'KtEn'
#define keyBias				'KBia'
#define keyEasto			'Keto'
#define keyEasefrom			'Kfro'

#define keyRotationRad		  'RoRd'
#define keyAnimVectorX        'vctX'
#define keyAnimVectorY        'vctY'
#define keyAnimVectorZ        'vctZ'
#define keyAnimVectorObject   'vctO'
#define keyAnimationDataClass 'AniC'
#define keyAnimActionClass    'AnAc'
#define keyInstanceName		  'InsN'
#define keyFlags1			  'flgO'
#define keyFlags2			  'flgT'
#define keyU3DNodeID		  'NoID'
#define keyU3DParentID        'PrID'
#define keyU3DType			  'tYpE'
#define keyPivotX			  'PvtX'
#define keyPivotY			  'PvtY'
#define keyPivotZ			  'PvtZ'
#define keyPivotObject        'PvtO'
#define keyPivotList		  'PvLs'
#define keyHasPivot			  'PvPr'
#define keyHasRange			  'RgBl'
#define keyHasLocalMatrix	  'LcMt'
#define keyRangeName		  'RgNm'
#define keyframePositionList  'PosL'
#define keyframeScaleList     'SclL'
#define keyframeRotationList  'RotL'
#define keyframeFOVList		  'FOVl'
#define keyframeRollList	  'RolL'
#define keyKeyFrameList		  'KeFL'
#define keyKeyFrameClass      'KeCS'
#define keyAnimationList	  'AnLs'
#define keyframePositionActionList 'acPs'
#define keyframeScaleActionList    'acSc'
#define keyframeRotationActionList 'acRt'
#define keyframeFOVActionList      'acFV'
#define keyframeRollActionList     'acRl'
#define keyframeHotActionList		'acHt'
#define keyframeHotList				'HotL'
#define keyframeFalloffActionList	'acFo'
#define keyframeFalloffList			'FalL'
#define keyframeHideActionList		'acHd'
#define keyframeHideList			'HidL'
#define keyframeMorphActionList		'acMp'
#define keyframeMorphList			'MorL'
#define keyframeColorActionList		'acCl'
#define keyframeColorList			'ColL'

#define keyAnimFrameNumber			'KAfn'
#define keyAnimFrameFlags			'KAff'
#define keyAnimFrameTension			'KAft'
#define keyAnimFrameContinuity		'KAfc'
#define keyAnimFrameBias			'KAfb'
#define keyAnimFrameEaseTo			'KAet'
#define keyAnimFrameEaseFrom		'KAef'
#define keyAnimFrameRotation		'KAro'
#define keyAnimFrameValue			'KAvl'
#define keyAnimFrameVectorX			'KAvx'
#define keyAnimFrameVectorY			'KAvy'
#define keyAnimFrameVectorZ			'KAvz'
#define keyAnimFrameUseQuat			'KAuq'
#define keyAnimFrameQuatW			'KAqw'
#define keyAnimFrameQuatX			'KAqx'
#define keyAnimFrameQuatY			'KAqy'
#define keyAnimFrameQuatZ			'KAqz'
#define keyAnimFrameClass			'KAFC'
#define keyAnimFrameList			'KAFL'

#define keventToolStartStr "keventToolStart"
#define keventToolStopStr "keventToolStop"
#define krenderStateStr "krenderState"
#define krenderFunctionStr "krenderFunction"
#define ktool3DStr "tool3D"

#define key3DRenderFunctionPtr					"key3DRenderFunction"
#define key3DDirectScenePtr						"key3DDirectScenePtr"

#define kFrameReaderClass						"FrameReader"
#define kFrameReaderTypeKey						"frameReaderType"

#define kDescVersionKey							"descVersion"
#define kDocumentSizeKey						"documentSize"
#define key3DSceneKey							"key3DScene"
#define key3DDataKey							"key3DData" 
#define key3DFileNameKey						"key3DFileName"
#define key3DFileListKey						"fileList"
#define key3DMeshTexturePathKey					"key3DMeshTexturePath"
#define key3DTexturesExternalKey				"key3DTexturesExternal" 
#define key3DTexturesVisibleKey					"key3DTexturesVisible" 
#define key3DTextureListKey						"key3DTextureList"
#define key3DTextureObjectKey					"key3DTextureObject"
#define key3DTextureNameKey						"key3DTextureName"
#define key3DTexturePathKey						"key3DTexturePath"
#define key3DTextureDataKey						"key3DTextureData"
#define key3DTextureVisibleKey					"key3DTextureVisible"
#define key3DTextureTypeKey						"key3DTextureType"
#define key3DDurationKey						"key3DDuration"
#define key3DScriptKey							"key3DScript"
#define key3DState								"key3DState"
#define key3DPositionKey						"key3DPosition"
#define key3DStateListKey						"key3DStateList"
#define key3DStateNameKey						"key3DStateName"
#define key3DXPosKey							"key3DXPos"
#define key3DYPosKey							"key3DYPos"
#define key3DZPosKey							"key3DZPos"
#define key3DXAngleKey							"key3DXAngle"
#define key3DYAngleKey							"key3DYAngle"
#define key3DZAngleKey							"key3DZAngle"
#define key3DFOVKey								"key3DFOV"
#define key3DSpeedKey							"key3DSpeed"
#define key3DCameraDistanceKey					"key3DCameraDistance"
#define key3DCurrentCameraPositionKey			"key3DCurrentCameraPosition" 
#define key3DCurrentFOVKey						"key3DCurrentFOV" 
#define key3DCurrentPositionKey					"key3DCurrentPosition" 
#define key3DCurrentOrthographicKey				"key3DOrthographic" 
#define key3DCurrentOrthographicScaleKey		"key3DOrthographicScale" 
#define key3DCurrentRenderModeKey				"key3DRenderMode"
#define key3DCurrentLightModeKey				"key3DLightMode"
#define key3DCurrentTimeKey						"key3DTime"
#define key3DCurrentCrossSectionKey				"key3DCrossSection"
#define key3DCrossSectionPlaneColorKey			"key3DCrossSectionPlaneColor"
#define key3DCrossSectionIntersectionColorKey	"key3DCrossSectionIntersectionColor"
#define key3DCrossSectionOffsetKey				"key3DCrossSectionOffset"
#define key3DCrossSectionPlaneTilt1Key			"key3DCrossSectionPlaneTilt1"
#define key3DCrossSectionPlaneTilt2Key			"key3DCrossSectionPlaneTilt2"
#define key3DCrossSectionPlaneOpacityKey		"key3DCrossSectionPlaneOpacity"
#define key3DCrossSectionAlignmentKey			"key3DCrossSectionAlignment"
#define key3DCrossSectionEnabledKey				"key3DCrossSectionEnabled"
#define key3DCrossSectionPlaneFlipKey			"key3DCrossSectionPlaneFlip"
#define key3DCrossSectionPlaneVisibleKey		"key3DCrossSectionPlaneVisible"
#define key3DCrossSectionIntersectionVisibleKey	"key3DCrossSectionIntersectionVisible"
#define key3DCurrentObjectXScaleKey				"key3DCurrentObjectXScale"
#define key3DCurrentObjectYScaleKey				"key3DCurrentObjectYScale"
#define key3DCurrentObjectZScaleKey				"key3DCurrentObjectZScale"
#define key3DAuxilaryColorKey					"key3DAuxilaryColor"
#define key3DFaceColorKey						"key3DFaceColor"
#define key3DOpacityKey							"key3DOpacity"
#define key3DLineWidthKey						"key3DLineWidth"
#define key3DCreaseValueKey						"key3DCreaseValue"
#define key3DViewIndexKey						"key3DViewIndex"
#define key3DEngineIndexKey						"key3DEngineIndex"
#define key3DViewNameKey						"key3DViewName"
#define key3DPaintTypeKey						"key3DPaintType"
#define key3DStateTypeKey						"key3DStateType"
#define key3DTextureFunctionPtr					"key3DTextureFunction"
#define key3DTextureSizeFunctionPtr				"key3DTextureSizeFunction"

#define key3DKeepLayersSeparateKey				"key3DKeepLayersSeparate"

#define keyPolyCount							'plyc'

#define key3DCurrentRenderSettingsKey			"key3DRenderSettings"
#define key3DSecondaryRenderSettingsKey			"key3DRenderSettings2"
#define key3DLineColorKey						"key3DLineColor"
#define key3DVertexColorKey						"key3DVertexColor"
#define key3DRenderFacesKey						"key3DRenderFaces"
#define key3DRenderEdgesKey						"key3DRenderEdges"
#define key3DRenderVerticesKey					"key3DRenderVertices"
#define key3DRenderFaceStyleKey					"key3DRenderFaceStyle"
#define key3DRenderEdgeStyleKey					"key3DRenderEdgeStyle"
#define key3DRenderVerticesStyleKey				"key3DRenderVerticesStyle"
#define key3DRenderAntiAliasKey					"key3DRenderAntiAlias"
#define key3DRenderRayDepthKey					"key3DRenderRayDepth"
#define key3DRenderReflectionsKey				"key3DRenderReflections"
#define key3DRenderRefractionsKey				"key3DRenderRefractions"
#define key3DRenderShadowsKey					"key3DRenderShadows"
#define key3DRenderRemoveBackfacesKey			"key3DRenderRemoveBackfaces"
#define key3DRenderRemoveBackfaceLinesKey		"key3DRenderRemoveBackfaceLines"
#define key3DRenderRemoveBackfaceVerticesKey	"key3DRenderRemoveBackfaceVertices"
#define key3DRenderVolumesKey					"key3DRenderVolume"
#define key3DRenderSeteroKey					"key3DRenderStereo"
#define key3DRenderVolumeStyleKey				"key3DRenderVolumeStyle"
#define key3DRenderStereoStyleKey				"key3DRenderStereoStyle"
#define key3DRenderGradientEnhancedKey			"key3DRenderGradientEnhanced"
#define key3DStereoOffsetKey					"key3DStereoOffset"
#define key3DStereoSpacingKey					"key3DStereoLenticularSpacing"
#define key3DStereoFocalPlaneKey				"key3DStereoFocalPlane"
#define key3DVertexRadiusKey					"key3DVertexRadius"
#define key3DRenderAdvancedIllumKey				"key3DRenderAdvancedIllumVideo"
#define key3DRenderRemoveHiddenLinesKey			"key3DRenderRemoveHiddenLines"
#define key3DRenderRemoveHiddenVerticesKey		"key3DRenderRemoveHiddenVertices"
#define key3DRenderOpacityScaleKey				"key3DRenderOpacityScale"
#define key3DShowGroundPlaneKey					"key3DShowGroundPlane"
#define key3DShowLightsKey						"key3DShowLights"

#define keyBBoxCenterX							"keyBBoxCenterX"
#define keyBBoxCenterY							"keyBBoxCenterY"
#define keyBBoxCenterZ							"keyBBoxCenterZ"

#define keyBBoxMinX								"keyBBoxMinX"
#define keyBBoxMinY								"keyBBoxMinY"
#define keyBBoxMinZ								"keyBBoxMinZ"

#define keyBBoxMaxX								"keyBBoxMaxX"
#define keyBBoxMaxY								"keyBBoxMaxY"
#define keyBBoxMaxZ								"keyBBoxMaxZ"

#define keyPreservationId						"keyPreservationId"
#define keyPreservationSubId					"keyPreservationSubId"
#define keyPreservationName						"keyPreservationName"
#define keyPreservationExtra					"keyPreservationExtra"
#define keyPreservationFlags					"keyPreservationFlags"

#define keyPreservationController				"keyPreservationController"
#define keyPreservationAnimations				"keyPreservationAnimations"
#define keyPreservationEffectProfiles			"keyPreservationEffectProfiles"

#define toolBadMode -31000	/* the tool is not read for external commands */


#if WIN32
#pragma pack(push,4)
#endif

#if PRAGMA_STRUCT_ALIGN
#pragma options align=mac68k
#endif

#ifndef kPI3DMaxPathLength
#define kPI3DMaxPathLength 2048
#endif

#ifndef kMaxNodeNameLength
#define kMaxNodeNameLength 256
#endif

//  Callback from device plugin to ask 3D tool to update
//  idleProc is a RenderDeviceIdleFunction - but one has to be defined first
#define k3DRenderPluginUpdateTypeSetIdleFunc	0
#define k3DRenderPluginUpdateTypeRemoveIdleFunc	1
#define k3DRenderPluginUpdateTypeUpdateState	2
typedef MACPASCAL OSErr (*RenderUpdate) (int32 updateType, PIActionDescriptor stateDescriptor, void *idleProc);

/// Callback from app to plugin to give it time
//Plugins fill in the doInteraction flag to say if they need to be active
//We will sit in a loop until this is false
//Plugins should call RenderUpdate when idled to get the state changed
#define k3DRenderAppUpdateTypeCheckInteraction		0
#define k3DRenderAppUpdateTypeSetState				1
#define k3DRenderAppUpdateTypeDoInteraction			2

typedef MACPASCAL OSErr (*RenderDeviceIdleFunction) (int32 updateType, Boolean *doInteraction, PIActionDescriptor stateDescriptor, RenderUpdate updateProc);

typedef int16			U3DAlignment;
typedef int16			RenderQuality;
typedef int16			ToolID;
typedef uint16			U3DInterfaceString[512];

#define PI3D			3.1415926535897932384626433832795
#define D2R				( PI3D / 180 )
#define R2D				( 180 / PI3D )
#define PI_OVER_2		( PI3D / 2)

// API specific
#include <time.h>

#ifndef PI3DTimeStamp
#define PI3DTimeStamp time_t
#endif

#ifndef PI3DFloat
#define PI3DFloat double
#endif
#ifndef RenderModeBitfield
#define RenderModeBitfield	unsigned long long
#endif

#ifndef AxisWidgetVisibilityBitfield
#define AxisWidgetVisibilityBitfield unsigned long long
#endif

#ifndef RenderModeStyle
#define RenderModeStyle		unsigned int
#endif

#ifndef PI3DVisibilityBitfield
#define PI3DVisibilityBitfield		void *
#endif

typedef PI3DFloat		PI3DVector[3];
typedef PI3DFloat		PI3DPoint[2];
typedef PI3DFloat		PI3DMatrix[4][4];
typedef int32			PI3DTriangle[3];
typedef uint32			PI3DStateType;

#define kMaxTransferFunctionValue	32767
#define kTransferFunctionSize		4097
typedef uint16			PI3DTransferFunction[4][kTransferFunctionSize];	

#define kNum3DWidgetTypes			4
#define kInfiniteLightWidgetIndex	0
#define kSpotLightWidgetIndex		1
#define kPointLightWidgetIndex		2
#define	kAxisWidgetIndex			3

#define kCurrenr3DMajorVersion			1
#define kCurrenr3DMinorVersion			0


#define kMax3DMapTypes					32
#define kNumSupported3DMapTypes			9
#define PI3DDiffuseMap					0
#define PI3DSphereEnvironmentMap		1
#define PI3DBumpMap						2
#define PI3DSpecularIntensityMap		3
#define PI3DOpacityMap					4
#define PI3DSpecularExponentMap			5
#define PI3DSelfIlluminationMap			6
#define PI3DReflectivityMap				7
#define PI3DNormalMap					8

#define PI3DUnknownMap1					9
#define PI3DUnknownMap2					10
#define PI3DUnknownMap3					11
#define PI3DUnknownMap4					12
#define PI3DUnknownMap5					13
#define PI3DUnknownMap6					14
#define PI3DUnknownMap7					15
#define PI3DUnknownMap8					16
#define PI3DUnknownMap9					17
#define PI3DUnknownMap10				18
#define PI3DUnknownMap11				19
#define PI3DUnknownMap12				20
#define PI3DUnknownMap13				21
#define PI3DUnknownMap14				22
#define PI3DUnknownMap15				23
#define PI3DUnknownMap16				24
#define PI3DUnknownMap17				25
#define PI3DUnknownMap18				26
#define PI3DUnknownMap19				27
#define PI3DUnknownMap20				28
#define PI3DUnknownMap21				29
#define PI3DUnknownMap22				30
#define PI3DUnknownMap23				31



typedef enum {
	PI3DLinearAttenuation=0,
	PI3DABCAttenuation
} PI3DAttenuationEnum;

typedef struct
	{
		PI3DFloat a;
		PI3DFloat b;
		PI3DFloat c;
	} PI3DABCAtt;

typedef struct
	{
		PI3DFloat w;
		PI3DFloat x;
		PI3DFloat y;
		PI3DFloat z;
	} PI3DQuat;

typedef struct 
	{
		PI3DVector axis; // origin, direction and angle
		PI3DFloat angle;
	} PI3DAngleAxis;

typedef enum {
	PI3DWireFrame=0x0001,
	PI3DFlat,
	PI3DGouraud,
	PI3DPhong,
	PI3DMetal
} PI3DShadingEnum; // shading type, NOT USED EXCEPT FOR WIREFRAME RIGHT NOW

typedef enum {
	PI3DSpotLight=0x0001,
	PI3DPointLight,
	PI3DInfiniteLight, 
	PI3DGlobalAmbientLight // This type of light is only found in U3D files so far
	// It will be added to the global ambient (see T3DCurrentScene::PI3DParseDescriptorIntoScene())
	// and not used as separate light source
} PI3DLightEnum;

typedef enum {
	PI3DClampX = 0x0001,
	PI3DClampY = 0x0002,
	PI3DStyle_3DS = 0x0004, //3D Studio style map
	PI3DModulate = 0x0008,
	PI3DColorMap = 0x0010,
	PI3DBumpNormal= 0x0010,
	PI3DInvert = 0x0020
} PI3DTextureMapFlags;

typedef enum {
	PI3DAmbientNode,	
	PI3DObjectNode,
	PI3DCameraNode,
	PI3DTargetNode,
	PI3DLightNode,
	PI3DLtargetNode,
	PI3DSpotlightNode,
	PI3DGroupNode //used for PI3D and other formats that have parent nodes which are not meshes or some other object
} PI3DNodeTypes;

typedef struct {
    char				name[kMaxNodeNameLength];
    void				*next;
} PI3DList;

typedef struct {
    PI3DFloat			red;
	PI3DFloat			green;
	PI3DFloat			blue;
} PI3DColor;

typedef struct {
	int64				start;
	int64				end;
	int64				length;
	uint16				tag;
} PI3DChunk;

typedef struct {
	uint8				red;
	uint8				green;
	uint8				blue;
} PI3DColor24;

typedef enum {
	PI3DGeoReference_RelativeToGround,
	PI3DGeoReference_Absolute
} PI3DGeoReferenceAltitudeModeEnum;

typedef struct {
	// Positional information.
	PI3DFloat			Latitude;
	PI3DFloat			Longitude;
	PI3DFloat			Altitude;
	uint32				AltitudeMode; // See PI3DGeoReferenceAltitudeModeEnum
	
	// Rotational information.
	PI3DFloat			Heading; // where NORTH is zero.
	PI3DFloat			Tilt; // where looking towards the center of the earth is zero.
	PI3DFloat			Roll; // ?
	
	// Size information
	PI3DFloat			ScaleX;
	PI3DFloat			ScaleY;
	PI3DFloat			ScaleZ;
	
} PI3DGeoReference;

typedef struct {
	// Naming information.
	// Important to keep the animation bindings intact.
	char*				Id;
	char*				SubId;
	char*				Name;
	uint8*				Extra; // Non-processed <extra> at the <node> level.
	uint32				Flags;
	
	// Extra trees and non-processed entities.
	uint8*				Controller; // On PI3DMesh only.
	uint8*				Animations; // Any?
	uint8*				EffectProfiles; // On PI3DMaterial only.
	
} PI3DPreservation;

/* Face flags */
//Edges															0x007
//Double sided													0x010
//Triangle visible												0x40

typedef struct {
	int32				numPoints;		/* number of points in the face */
    int32				*points; 
	int32				flags;			/* Face flags - see above */
	int32				smoothing;		/* smoothing value */
	int32				material;		/* material index */
	int32				*normals;		/* normals if you have them */
	int32				*colors;		/* colors if you have them */
	int32				*textures; 
		
	//	Below here is calculated on load if it is missing
	int32				*padding;			/* edge indices list, number of edges would be numPoints */
	
} PI3DFace;

typedef int32			smallPointer;
typedef struct {
	int32				numPoints;		/* number of points in the face */
	smallPointer		points; 
	int32				flags;			/* Face flags - see above */
	int32				smoothing;		/* smoothing value */
	int32				material;		/* material index */
	smallPointer		normals;		/* normals if you have them */
	smallPointer		colors;		/* colors if you have them */
	smallPointer		textures; 
	
	smallPointer		padding;
} PI3DFaceForReading;

typedef struct {
    //Standard list fields
	char				name[kMaxNodeNameLength];
	void				*next;
	
	PI3DLightEnum		type;				/* light type */
    PI3DVector			pos;				/* light position */
    PI3DVector			target;				/* light target location */
    PI3DColor			col;				/* light colour */
    PI3DFloat			hotspot;			/* Hotspot angle (degrees) */
    PI3DFloat			falloff;			/* Falloff angle (degrees) */
    int32				shadowFlag;			/* Shadow flag (not used) */
	Boolean				attenuation;		/* Use attenuation */
	PI3DAttenuationEnum	attenuationType;	/* linear or ABC */
	PI3DABCAtt			attenuationAbc;		/* use if you have ABC attenuation */
	PI3DFloat			outerRadius;		/* outer range */
	PI3DFloat			innerRadius;		/* inner range */
	PI3DFloat			multiple;			/* multiplier */
	
	Boolean				cameraBound;		/* true if light shall be bound to camera instead of world */
	
	Boolean				isOn;				/* is the light turned on? */
	PI3DFloat			ssmultiple;			/* soft shadow multiplier */
	
	PI3DPreservation*	preservation;
	
	char				padding[374];
} PI3DLight;


/* Camera */
typedef struct {
    //Standard list fields
	char				name[kMaxNodeNameLength];
	void				*next;
	
    PI3DVector			pos;				/* Camera location */
    PI3DVector			target;				/* Camera target */
    PI3DFloat			bank;				/* Banking angle (degrees) */
    PI3DFloat			lens;				/* Camera lens size (mm) */
	Boolean				ortho;				/* use orthographic */
	PI3DFloat			aspectratio;		/* Aspect Ratio */
	PI3DFloat			zoomfactor;			/* zoom factor */
	
	PI3DPreservation*	preservation;
	
	char				padding[380];
} PI3DCamera;


typedef struct {
	char					map[2048];
    PI3DFloat				strength;		/* intensity of the map */
	PI3DFloat				uscale;  
	PI3DFloat				vscale;
	PI3DFloat				uoffset;
	PI3DFloat				voffset;
	PI3DFloat				angle;
	PI3DTextureMapFlags		flags;
	PI3DFloat				valueMin;								/* Minimum value mapping (to be calculated inside fragment shader after lookup) - default is 0.0 */
	PI3DFloat				valueMax;								/* Maximum value mapping (to be calculated inside fragment shader after lookup) - default is 1.0 */
	Boolean					mapIs3DSStyle;	/* Needed only by the 3ds importer */
	
	int32					photoshopTextureID; /* filled in by Photoshop on load */
	char					textureTypeName[kMaxNodeNameLength];	//Texture to show in UI for type of texture
	char					layerName[kMaxNodeNameLength];
	Boolean					wasAdded;
	
	char					padding[295];
} PI3DTextureMatProp;

typedef struct {
    //Standard list fields
	char				name[kMaxNodeNameLength];
	void				*next;
	
    PI3DColor			ambient;
    PI3DColor			diffuse;
    PI3DColor			specular;
	PI3DColor			emissive;
    PI3DFloat			shininess;
	PI3DFloat			glossiness;			/* specular level */
    PI3DFloat			transparency;
    PI3DFloat			reflection;
    int32				selfIllumination;	/* unused */
	PI3DShadingEnum		shading;
	PI3DTextureMatProp	maps[kMax3DMapTypes];
	Boolean				twoSided;
	Boolean				wireframe;
	Boolean				decal;
	PI3DFloat			wireframesize;
	uint32				importdata;		/* reserved, do not use */
	Boolean				hasRepeatingTextures;
	PI3DFloat			indexofrefraction;
	
	PI3DPreservation*	preservation;
	char				padding[503];
} PI3DMaterial;

typedef struct U3DBoundingBox
	{
		PI3DFloat				minX;	
		PI3DFloat				maxX;	
		PI3DFloat				centerX;	
		
		PI3DFloat				minY;	
		PI3DFloat				maxY;	
		PI3DFloat				centerY;	
		
		PI3DFloat				minZ;	
		PI3DFloat				maxZ;	
		PI3DFloat				centerZ;	
		
	}
	U3DBoundingBox, *U3DBoundingBoxPtr;


typedef struct {
    //Standard list fields
	char				name[kMaxNodeNameLength];
	void				*next;
	
    int32				vertices;					/* Number of vertices */
    PI3DVector			*vertex;					/* List of object vertices */
    int32				textures;					/* Number of textures */
    PI3DPoint			*texture;					/* List of object textures */
    int32				faces;						/* Number of faces */
    PI3DFace			*face;						/* List of object faces */
	int32				lines;						/* number of lines */
	PI3DVector			*linecolors;				/* per line color - rgb*/
	PI3DVector			*line;						/* line pts */
	int32				*linematerial;				/* line material index */
	int32				flags;						/* NOT USED */
    int32				hidden;						/* Hidden flag */
    int32				shadow;						/* Shadow flag */
	PI3DMatrix			matrix;						/* Mesh transformation matrix */
	Boolean				matrixPresent;				/* is there a transformation matrix */
	Boolean				smoothingGroupPresent;		/* is there a smoothing group */
    int32				normals;					/* Number of normals */
    PI3DVector			*normal;					/* List of object normals */
	int32				colors;						/* Number of colors */
	PI3DVector			*color;						/* List of vertex colors */

	int32				tangents;					/* Number of tangents */
	PI3DVector			*tangent;					/* List of tangent (dP/du) vectors*/
	
	int32				binormals;					/* Number of binormals */
	PI3DVector			*binormal;					/* List of binormal (dP/dv) vectors*/
	
	
	int32				triangles;					/* Number of triangles */
	int32				*triangle;					/* List of triangles.  Each triangle is 3 ints */
	
	U3DBoundingBox		bbox;						/* Per-mesh bounding box */
	
	int32				edges;						/* Number of edges in mesh  - calculated at run time*/
	void				*edge;						/* Edge list   - calculated at run time*/
	void				**vconnect;					/* Vertex connection data   - calculated at run time*/
	
	Boolean				isVolume;					/* Is this mesh a volume */
	Boolean				isShadowCatching;			/* Is this mesh catching shadows */
	Boolean				isShadowCasting;			/* Is this mesh casting shadows */
	Boolean				isShadowInvisible;			/* Is this mesh shadow catching invisible */

	int32				numUniqueMaterials;			/* Number of materials on this mesh */
	int32				*uniqueMaterialList;		/* Array of material ids contained on this mesh */

	PI3DPreservation*	preservation;
} PI3DMesh;

/* Frame vector flags */
//Start cyclic							0x010
//End cyclic							0x10
//Linear								0x00000
//Bezier								0x10000 NOT USED RIGHT NOW
//TCB(Use the tension continuity...)	0x20000 

typedef struct {
	PI3DFloat			framenumber;
	int32				flags;			/* Frame vector flags - see above*/
	PI3DFloat			tension;
	PI3DFloat			continuity;
	PI3DFloat			bias;
	PI3DFloat			easeto;
	PI3DFloat			easefrom;
	PI3DFloat			rotation;		//radians
	PI3DFloat			value;			//FOV, HOT, FALL, ROLL - in degrees
	PI3DVector			v;				//POS, SCL, ROT(angleaxis value)
	Boolean				usequat;		//if true using q if not using v
	PI3DQuat			q;				//ROT(quaternion)
	
	char				padding[512];
} PI3DFrameVector;

typedef struct {
	//Standard list fields
	char				name[kMaxNodeNameLength];				/* name of animation segment */
	void				*next;
	
	int32				flags;
	int32				keys;					/* number of keyframes */
	PI3DFrameVector*	aFrameData;
	
	char				padding[512];
} PI3DAnimationData;

/* A key frame object */
typedef struct {
    //Standard list fields
	char				name[kMaxNodeNameLength];
	void				*next;
	
	char				instancename[kMaxNodeNameLength];		/* instance name for this object */
	int32				nodeID;					/* node id */
	int32				parentNodeID;			/* parent node id - 65535 means scene is parent */
	int32				flags1;
	int32				flags2;
	
	PI3DVector			pivot;
	Boolean				usepivot;				/* if not using pivot, local matrix will be set for keyframes that do not have animation data */
	Boolean				matrixPresent;
	PI3DMatrix			localmatrix;
	
	Boolean				userange;
	PI3DNodeTypes		nodeType;
	
	PI3DVector			bboxmin;				/* bounding box */
	PI3DVector			bboxmax;
	
    PI3DAnimationData	*positionData;
    PI3DAnimationData	*rotationData;
    PI3DAnimationData	*scaleData;
	PI3DAnimationData	*colData;
	PI3DAnimationData	*fOVData;
	PI3DAnimationData	*hotData;				/* not in use */
	PI3DAnimationData	*fallData;				/* not in use */
	PI3DAnimationData	*rollData;
	PI3DAnimationData	*hideData;				/* not in use */
	PI3DAnimationData	*morphData;				/* not in use */
	
	PI3DPreservation*	preservation;

	char				padding[508];
} PI3DKeyFrame;


typedef struct {
	PI3DLight			*lightList;
	PI3DCamera			*cameraList;
	PI3DMesh			*meshList;
	PI3DMaterial		*matPropList;
	PI3DKeyFrame		*keyFrameList;
	PI3DColor			globalAmbientColor;
	PI3DFloat			globalUnits;					/* measurement units in inches */
	char				customData[kMaxNodeNameLength];				/* For 3rd party plugins */
	int32				globalAnimationStartTime;
	int32				globalNumFrames;
	PI3DFloat			globalFPS;						/* frames per second */
	
	PI3DColor			lightingSchemeGlobalAmbientColor; /* We set this color in our lighting scheme */
	/* e.g. white lights, hard lights, etc.     */
	
	char				name[kMaxNodeNameLength];
	U3DBoundingBox		globalBoundingBox;
	uint32				polyCount;
	PI3DGeoReference		*geoReference;
	
	Boolean				texturesAreFlipped;					//In U3D files, the textures come in flipped

} PI3DScene;

typedef enum {
	PI3DDLRootNode=0,
	PI3DDLTransformNode,
	PI3DDLGroupNode,
	PI3DDLGeometryNode,
	PI3DDLLightNode,
	PI3DDLCameraNode,
	PI3DDLCameraTargetNode,
	PI3DDLLightTargetNode,
	PI3DDLSwitchNode,
	PI3DDLVisibilityNode,
	PI3DDLRenderStateNode
} PI3DDisplayListNodeType;

typedef enum {
	PI3DAnimRotation=0,
	PI3DAnimTranslation,
	PI3DAnimScale,
	PI3DAnimFalloff,
	PI3DAnimHotspot,
	PI3DAnimFOV,
	PI3DAnimRoll,
	PI3DAnimColor
} PI3DDisplayListAnimType;

typedef enum
{	
	PI3DWidgetHandleNone = 0,
	PI3DWidgetHandleUniformScale,
	PI3DWidgetHandleXTranslate, 
	PI3DWidgetHandleXRotate, 
	PI3DWidgetHandleXScale, 
	PI3DWidgetHandleYTranslate, 
	PI3DWidgetHandleYRotate, 
	PI3DWidgetHandleYScale, 
	PI3DWidgetHandleZTranslate, 
	PI3DWidgetHandleZRotate, 
	PI3DWidgetHandleZScale, 
	PI3DWidgetHandleXZTranslate, 
	PI3DWidgetHandleYZTranslate, 
	PI3DWidgetHandleXYTranslate, 
	PI3DWidgetHandleXZTranslateHoverMask, 
	PI3DWidgetHandleYZTranslateHoverMask, 
	PI3DWidgetHandleXYTranslateHoverMask, 
	PI3DWidgetHandleXZTranslateBackup, 
	PI3DWidgetHandleYZTranslateBackup, 
	PI3DWidgetHandleXYTranslateBackup, 
	PI3DWidgetHandleXZTranslateHoverMaskBackup, 
	PI3DWidgetHandleYZTranslateHoverMaskBackup, 
	PI3DWidgetHandleXYTranslateHoverMaskBackup, 
	PI3DWidgetHandleWidgetMove,
	PI3DWidgetHandleWidgetScale,
	PI3DWidgetHandleWidgetMinimize,
	PI3DWidgetHandleWidgetRestore
} PI3DAxisWidgetHandleType;



#define k3DAxisWidgetVisible					0x00000001
#define k3DAxisWidgetAllControlsVisible			0x00000002
#define k3DAxisWidgetAllAxesVisible				0x00000004
#define k3DAxisWidgetSomeControlsVisible		0x00000008
#define k3DAxisWidgetSomeAxesVisible			0x00000010
#define k3DAxisWidgetHandleUniformScaleVisible  (0x1 << PI3DWidgetHandleUniformScale+4)
#define k3DAxisWidgetHandleXTranslateVisible	(0x1 << PI3DWidgetHandleXTranslate+4)
#define k3DAxisWidgetHandleXRotateVisible		(0x1 << PI3DWidgetHandleXRotate+4)
#define k3DAxisWidgetHandleXScaleVisible		(0x1 << PI3DWidgetHandleXScale+4)
#define k3DAxisWidgetHandleYTranslateVisible	(0x1 << PI3DWidgetHandleYTranslate+4)
#define k3DAxisWidgetHandleYRotateVisible		(0x1 << PI3DWidgetHandleYRotate+4)
#define k3DAxisWidgetHandleYScaleVisible		(0x1 << PI3DWidgetHandleYScale+4)
#define k3DAxisWidgetHandleZTranslateVisible	(0x1 << PI3DWidgetHandleZTranslate+4)
#define k3DAxisWidgetHandleZRotateVisible		(0x1 << PI3DWidgetHandleZRotate+4)
#define k3DAxisWidgetHandleZScaleVisible		(0x1 << PI3DWidgetHandleZScale+4)
#define k3DAxisWidgetHandleXAxisVisible			0x00008000
#define k3DAxisWidgetHandleYAxisVisible			0x00010000
#define k3DAxisWidgetHandleZAxisVisible			0x00020000
#define k3DAxisWidgetOutlineVisible				0x00040000
#define	k3DAxisWidgetAxesAsWireframe			0x00080000

// flags below
// 0x00 - no changes
// 0x01 - use isOn for mesh visibility
// 0x02 - use bitfield for visibility
typedef struct {
	uint32						flags;
	Boolean						isOn;				/* is the mesh visible? , one check instead of walking bitfield */
	PI3DVisibilityBitfield		visibilityBitfield;
} PI3DVisUpdate; 

typedef struct {
	Boolean						isShadowCatcher;
	Boolean						isShadowCasting;
	Boolean						isShadowInvisible;
} PI3DShadowUpdates; 

typedef struct {
	void						*next;
	uint32						numFaces;
	PI3DVisUpdate*				overlayVisUpdate;	// replace display list visibility
	PI3DMatrix*					overlaymatrix;	// replace display list node matrix	
	PI3DShadowUpdates*			overlayShadowUpdate; // tell mesh to be a shadow catcher
} PI3DDisplayListMeshFaceUpdate; 

typedef struct {
	void							*next;
	PI3DDisplayListAnimType			type; // Rotation, Scale and Translation will not be here, they are part of the rendermatrix
	PI3DFloat						value; // value of animation at scene time
	PI3DColor						color; // color value of animation at scene time
} PI3DDisplayListAnimation; 

struct PI3DDisplayListNode{
	PI3DDisplayListNodeType			nodetype; // type of node
	PI3DTimeStamp					timestamp; // time stamp so we know if it changed
	void*							pNodeTypePtr; // Mesh, Light, Camera, RenderState, etc...
	PI3DMatrix						rendermatrix;	// matrix at display time	
	U3DBoundingBox					bbox; // bounding box at display time
	PI3DDisplayListAnimation*		panimationvalues; // animation values at display time
	PI3DDisplayListMeshFaceUpdate*	pupdates; // mesh face updates, materials and visibility at display time
	struct PI3DDisplayListNode*		pLeft; 
	struct PI3DDisplayListNode*		pRight;
};

typedef struct {
	PI3DDisplayListNode			*renderstateroot;
	PI3DDisplayListNode			*root;
	PI3DMaterial					*displayListMaterials;
	int32						numberOfLights;
	int32						numberOfSpotlights;
	bool						forceShaderRecompile; // tell shaders to recompile even if number of lights did not change
} PI3DDisplayList;


typedef struct U3DPosition
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
	U3DPosition, *U3DPositionPtr;



#define kMaxTransferFunctionValue	32767
#define kTransferFunctionSize		4097
typedef uint16			U3DTransferFunction[4][kTransferFunctionSize];

typedef struct 
	{
		//Faces
		Boolean				renderFaces;				//Flag for controlling whether or not to render faces
		uint32				faceStyle;					//Constant, Flat, Smooth, Bounding Box, Ray Traced, Unlit Texture, Face IDs, Depth, Normals
		PI3DFloat			faceColor[3];				//Face colors in illustration modes
		uint32				textureType;				//When rendering diffuse only, what kind of texture do we display
		uint32				antiAliasQuality;			//1, 2, 4
		uint32				rayDepth;					//# of bounces
		Boolean				showReflections;			//Flag for controlling whether or not to render reflections
		Boolean				showRefractions;			//Flag for controlling whether or not to render refractions
		Boolean				showShadows;				//Flag for controlling whether or not to render shadows
		Boolean				useAdvancedIllumination;	//Flag for controlling whether or not to render using advanced illumination
		Boolean				removeBackfaces;			//Flag for controlling whether or not to remove back facing polygons
		
		
		//Edges
		Boolean				renderEdges;				//Flag for controlling whether or not to render edges
		uint32				lineStyle;					//Constant, Flat, Smooth, Bounding Box, Illustrative
		PI3DFloat			lineColor[3];				//Line colors in illustration modes
		PI3DFloat			lineWidth;					//Width of wireframe lines
		PI3DFloat			creaseValue;				//Threshold angle for illustration angle
		Boolean				removeBackfaceLines;		//Flag for controlling whether or not to remove back facing lines
		Boolean				removeHiddenLines;			//Flag for controlling visibility of lines thatw oudl not be seen in full face rendering
		
		//Vertices
		Boolean				renderVertices;				//Flag for controlling whether or not to render vertices
		uint32				vertexStyle;				//Constant, Flat, Smooth, Bounding Box
		PI3DFloat			vertexColor[3];				//Vertex colors in illustration modes	
		PI3DFloat			vertexRadius;				//Radius of drawn vertices
		Boolean				removeBackfaceVertices;		//Flag for controlling whether or not to remove back facing vertices
		Boolean				removeHiddenVertices;		//Flag for controlling visibility of vertices thatw oudl not be seen in full face rendering
		
		//Volumes	
		Boolean				renderVolume;				//Flag for controlling whether or not to render volumes
		uint32				volumeStyle;				//MIP, Alpha
		U3DTransferFunction	transferFunction;			//Volume lookup
		Boolean				gradientEnhancement;		//Enhance Gradients	
		PI3DFloat			opacityScale;				//Coefficient for scaling the opacity of each voxel
		
		//Stereo	
		Boolean				renderStereo;				//Flag for controlling whether or not to render in stereo
		uint32				stereoStyle;				//Red-Blue, Vertical Interlaced, Horizontal Interlaced
		PI3DFloat			stereoOffset;				//Eye separation in bbox coordinates
		PI3DFloat			stereoLineSpacing;			//Spacing to use for interlacing
		PI3DFloat			focalPlane;					//Plane that the stereo camera focus on. Zero is the cenetr of the bounding box, -100 is the back and 100 is the front.
		
		//For Photoshop use only
		int16				projection;					//Rendering into textures
		Boolean				blendUnlitWithBaseValue;	//Flag to determine how unlit texture mode gets drawn
		Boolean				drawGroundPlane;			//Include ground plane in rendering
		Boolean				drawLights;					//Include light widgets in rendering
		Boolean				videoAtHighestQuality;		//Turn on ray tracing with advanced illumination when rendering video 
	}
U3DRenderSettings;

typedef struct 
	{
		PI3DDisplayList					*widgetDisplayList[kNum3DWidgetTypes];  //array of display lists for widget drawing
		PI3DScene						*widgetScene[kNum3DWidgetTypes]; //array of scenes for widget drawing
		PI3DColor						guideColor;						 //Color that's used for the guides, for light widget drawing.
		Boolean							drawAxisWidgetOnly;				//Put the engine in the mode where it has to draw just the axis widget
		PI3DAxisWidgetHandleType			axisWidgetHandleHighlight; 		//Which part of axis widget to highlighted, during axis widget rendering.
		AxisWidgetVisibilityBitfield	axisWidgetVisibilityFlags;
		PI3DVector						axisWidgetAxisProjection[3];		//Projections of axes of the axis widget onto screen
		PI3DVector						axisWidgetRotationHandleProjection[3];		//Projections of axes of the axis widget onto screen
		uint8							planarSlideHandleOcclusion[3];		//Indicates if XZ, YZ, XY planar slide controls are occluded.
		U3DPosition						changeDuringInteraction;        //Relative change that happened during interaction.
	}
U3DWidgetRenderSettings;

typedef struct U3DCrossSection
	{
		PI3DFloat				planeColor[3];
		PI3DFloat				intersectionColor[3];
		PI3DFloat				offset;
		PI3DFloat				planeTilt1;
		PI3DFloat				planeTilt2;
		PI3DFloat				planeOpacity;
		U3DAlignment			alignmentMode;
		Boolean					crossSectionEnabled;
		Boolean					planeFlip;
		Boolean					planeVisible;
		Boolean					intersectionVisible;
		
		U3DRenderSettings		secondSideRenderSettings;

		/* Reserved for future expansion */
		char					reserved [512]; 				// Set to zero 
	}
	U3DCrossSection, *U3DCrossSectionPtr;


typedef struct 
	{
		uint32					length;							// number of textures
		uint16					**textureNames;					// texture names
		uint16					**texturePaths;					// texture paths
		uint32					*textureType;					// flags to indicate the type of texture
		/* Reserved for future expansion */
		char					reserved [508]; 				// Set to zero 
	} 
	U3DTextureArray;

typedef struct RenderDrawBuffer
	{
		void				*pixels;
		int32				rowBytes;
		VPoint				sceneSize;
		VRect				tileRect;
		int16				planeMap[16];							/* Maps plug-in plane numbers to host plane
		 numbers.  The host initializes this is a linear
		 mapping.  The plug-in may change this mapping if
		 it sees the data in a different order. */
		/* Reserved for future expansion */
		int32				bitsPerPixel;
		char				reserved [508]; 					// Set to zero 
		
	} 
	RenderDrawBuffer, *RenderDrawBufferPtr;


/// Callback from plugin to get texture data
typedef MACPASCAL OSErr (*GetTextureSizeFunction) (int32 index, RenderDrawBuffer *buffer);
typedef MACPASCAL OSErr (*GetTextureFunction) (int32 index, RenderDrawBuffer *buffer);
typedef MACPASCAL void  *(*Malloc3D) (uint32 size);
typedef MACPASCAL void (*Free3D) (void *mem);
typedef uint32			U3DStateType;
typedef MACPASCAL Boolean (*HostProgressFunction) (int32 position, int32 end);

typedef struct 
	{
		//State Data - set to defaults by plugin and saved in PSD
		U3DPosition			currentCameraPosition;				//Current camera position
		PI3DFloat			currentFieldOfView;					//Field of view of camera (degrees)
		PI3DFloat			currentObjectXScale;				//Scale applied to the object by the user
		PI3DFloat			currentObjectYScale;				//Scale applied to the object by the user
		PI3DFloat			currentObjectZScale;				//Scale applied to the object by the user
		U3DPosition			currentObjectPosition;				//Current object position
		Boolean				currentOrthographic;				//Is the camera currently in orthographic mode
		PI3DFloat			currentOrthoScale;					//Scale for orthorgraphic view 1/x
		uint32				currentLightMode;					//Current light mode
		PI3DFloat			currentTime;						//Current time of the play head in seconds
		U3DCrossSection		currentCrossSection;				//Data to define the current cross section	
		
	//added for CS4
		U3DRenderSettings	currentRenderSettings;				//Data to define the current render settings
		U3DWidgetRenderSettings	currentWidgetRenderSettings;		//Data to control widget rendering
		PI3DColor			currentGlobalAmbient;				//Global ambient color
		U3DStateType		stateType;

		//Interaction Data - set by Photoshop per render
		RenderQuality		quality;							//Quality level of final output
		uint32				currentTexture;						//Set before calling update texture
		char				currentTextureLayer[4096];			//Set before calling update texture
		
		
		RenderDrawBuffer	drawBuffer;							//Used for passing texture updates
		
		void						*unsused;						//Unused
		int32						currentToolMode;			//Kept current by PS.  Set by controller plugin when requesting tool mode change.
		int32						currentToolID;				//Kept current by PS.  Set by controller plugin when requesting tool id change.
		
		
		GetTextureSizeFunction		getTextureSizeFunction;
		GetTextureFunction			getTextureFunction;
		
		int32						faceCount;
		uint32						*faceList;
		int32						materialID;							// If >= 0, only render faces that use that material ID
		int32						textureID;							// Used if materialID >= 0 - tells the renderer which texture map is being rendered into
		PI3DFloat					paintFadeMinThreshold;				// Used to fade paint effects near object edges
		PI3DFloat					paintFadeMaxThreshold;
		Boolean						useSoftwarePaint;
		
		//App data
		void						*appData;							//Used in callbacks to PS
		Boolean						blendWithBufferContents;			//Set to true if the rendering should be alpha blended with the existing contents of the buffer
		
		VRect						viewportRect;  
		PI3DFloat					rotationAngle;
		PI3DFloat					rotationScaleX;
		PI3DFloat					rotationScaleY;
		
		VPoint						directToScreenBufferDimensions;

		void						*reserved;
		
		PI3DFloat					pixelAspectRatio;					// The PAR for the render target.
		PI3DFloat					documentResolution;					// The document's resolution- used for lenticulars
		Boolean						anaglyphAsGrayscale;				// Go to grayscale for anaglyphs before doing red/blue
		Boolean						engineMustClearGLColorBuffer;		// Used in direct-to-screen, if false, engine will not clear color buffer
		int8						externalResourceSharingGLContextPresent; //Indicates whether external context (GL canvas one) exists
		
		//Initial Bounding Box of Scene	
		U3DBoundingBox				initialBoundingBox;					//Bounding box on initial load
	}
	RenderState, *RenderStatePtr;


typedef MACPASCAL void (*RenderFrame) (RenderDrawBuffer *buffer, PI3DScene *scene, RenderState *state);

typedef struct 
	{
		uint32					length;							// number of cameras
		uint16					**positionNames;				// position names
		RenderState				*viewStates;					// states of current view
		
		/* Reserved for future expansion */
		char					reserved [512]; 				// Set to zero 
	} 
	U3DStateArray;

typedef struct 
	{
		void				*engine;							//Pointer to a 3D engine instance
		//This should be passed back from PrepareU3DRenderer
		//It will be provided in all render calls
		Boolean				supportsOrthographic;				//Can this renderer do orthographic?
		Boolean				supportsCrossSection;				//Can this renderer render a cross section?
		uint16				numRenderModes;
		uint16				**renderModeList;					//List of known render modes
		uint16				numLightModes;
		RenderModeBitfield	*renderModeBitfields;				//Bitfield for each mode enabling related interface items
		RenderModeStyle		*renderModeStyles;					//Style flag for each render mode
		uint16				**lightModeList;					//List of known light modes
		Boolean				useHardware;						//Flag to determine whether or not to use hardware acceleration
		
		PI3DLight			*lightList;							//Actual lights that map to the light mode names
		Boolean				supportsPainting;					//Can this engine support painting
		Boolean				supportsVolumeRendering;			//Can this engine support volume rendering
		char				*fGPUProfile;						//String returned from the engine, describing the profile of the GPU from OpenGL and Cg queries
		PI3DDisplayList		*displayList;						//Pointer to Display List, not allocated in RenderRecord, just passed through
		
		
		/* Reserved for future expansion */
		char				reserved [503]; 					// Set to zero 
		
	}
	RendererInfo, *RendererInfoPtr;


typedef struct RenderRecord
	{
		//File Data	- retrieved when the file is loaded and saved in PSD
		SPPlatformFileSpecificationW	*fileSpec2;							//Location of 3D data on disk
		PIDescriptorHandle				sceneDescriptor;					//This will contain the scene data
		SPBasicSuite					*sSPBasic;							//SuitePea basic suite
		uint8							*data;								//Pointer to File Data
		//This should be filled in on ReadContinue and will be provided on PrepareU3DRenderer and on all writing calls
		Boolean							mustAllocateData;					//If the host allocated the data, we need to allocate a copy of our own
		uint32							dataSize;							//Size of the U3D data
		uint16							dataType;							//Type of data
		PI3DFloat						duration;							//Duration of animation in the file in seconds
		U3DStateArray					stateList;							//List of known states
		U3DTextureArray					textureList;						//List of textures in the file
		Boolean							texturesExternal;					//Set to true if the textures were not contained inside the original 3D file
		uint16							*reserved;						
		
		RenderDrawBuffer				drawBuffer;
		//State Data - set to defaults by plugin and saved in PSD	
		RenderState						renderState;
		
		//Engine Data - not needed for file and not saved per instance - sent to PS in prepareRenderer
		RendererInfo					rendererInfo;
		
		//Initial Bounding Box of Scene	
		//U3DBoundingBox					initialBoundingBox;					//Bounding box on initial load
		
		uint16							*meshTexturePath;					//Path to texture to use for the mesh
		
		int32							reserved2;
		
		//Memory allocation that needs to be turned over to the app	
		Malloc3D						mallocFunc;
		Free3D							freeFunc;
		int32							version;							// Not the best place for this, but putting it at the top now would cause problems
		HostProgressFunction			progressCallback;					// Used by ray tracer to give host time in long renders

		
		/* Reserved for future expansion */
		char							reserved3 [500]; 					// Set to zero 
	}
	RenderRecord, *RenderRecordPtr;


#if WIN32
#pragma pack(pop)
#endif

#if PRAGMA_STRUCT_ALIGN
#pragma options align=reset
#endif

#endif  // __PI3D_h__

#endif //__PI3D__
