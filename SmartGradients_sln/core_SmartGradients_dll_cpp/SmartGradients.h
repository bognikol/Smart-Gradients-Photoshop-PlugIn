#ifndef _SMARTGRADIENTS_H
#define _SMARTGRADIENTS_H

#include "PIDefines.h"
#include "PITypes.h"
#include "PIAbout.h"
#include "PIFilter.h"
#include "PIUtilities.h"

#include <cmath>

#include "Dense"

using Eigen::MatrixXd;
using Eigen::VectorXd;

#define AnchorPixelMax 25
#define HelperBufferSize 80000
#define WorkingSurfaceSize 10000

extern int32* ArcTanLookUp;

#pragma region GRADIENTSTATE

enum GradientType
{
	COMPLEX = 0,
	LINEAR  = 1,
	RADIAL  = 2,
	ANGLED  = 3,
};

typedef struct AnchorPixel
{
	int32 X;
	int32 Y;
	uint8 CH[5];
} 
AnchorPixel;

typedef struct GradientState
{
	//GENERAL MEMBERS
	bool OpacityApplied;
	GradientType Type;
	AnchorPixel AnchPixs[AnchorPixelMax];
	int32 ActiveAnchPixs;

	//COMPLEX SPECIFIC MEMBERS
	uint8 Px;
	uint8 Py;
	double DiffusionIndex;
	int32 Parameter;

	//LEGACY SPECIFIC MEMBERS
	double Grad;

	//LINEAR SPECIFIC MEMBERS
	double Angle;

	//RADIAL SPECIFIC MEMBERS
	uint8 CentralPix;

	//ANGLED SPECIFIC MEMEBERS
	int32 CenterX;
	int32 CenterY;
} 
GradientState;

typedef struct ProxyState
{
	VPoint ViewportLocation;
	VPoint ViewportSize;
	double ZoomFactor;
} 
ProxyState;

typedef struct RectangleRecord
{
	VRect ReadRectangle;
	VRect SelectionRectangle;
	VRect WriteRectangle;
	VRect RenderingRectangle;
	VRect WriteRenderingRectangle;
} 
RectangleRecord;

#define ParameterNotUsed -100
#define DiffusionIndexNotUsed 1

void InitializeAnchorPixel(GradientState* state,
	int32 num, int32 x, int32 y, uint8 ch0, uint8 ch1, uint8 ch2, uint8 ch3, uint8 ch4);

void ResampleGradientState(GradientState* state, VRect renderingRect);
void ResampleGradientState(GradientState* state, VRect renderingRect, double zoom);

int32 ValidateGradientState(GradientState* state);

enum GradientStateError
{
    GradientStateNoErr = 0,                       
    AnchorPixNumUnderMinimumForComplex,      
    AnchorPixNumAboveMaxForComplexWithParam, 
    AnchorPixNumAboveMaxForComplexNoParam,   
    PxPyUnderMinimumForComplex,              
    PxPyMismatchForComplexWithParam,         
    PxPyMismatchForComplexNoParam,           
    AnchorPixNumUnderMinimumForElse,         
    AnchorPixNumAboveMaximumForElse,        
    CentralPixelAboveMaxForRadial,           
    DiffusionIndexNegative,                  
    ParameterOutOfRange,                     
    UnsepecifiedError                       
};

#pragma endregion

#pragma region HELPERS

uint8 ToByte(double input);
int32 Signum(double x);
bool  CheckIfArrayContains(int32 i, int32* r, int32 num);
int32 GetArrayMin(int32* L, int32 num);
int32 GetArrayMinLocationButIgnoreValues(int32* L, int32* r, int32 num, int32 ignoreNum);
void  GetLinearRank(int32* L, int32* rank, int32 activeAnchPix);

#pragma endregion

#pragma region COMPLEX

double Transpose(uint8 C, double k);
double UnTranspose(double xC, double k);
int32 BinomialQuotient(int32 above, int32 under);
double Polynomial(VectorXd* q, int32 x, int32 pow);

MatrixXd GetMainMatrix(GradientState* state);
VectorXd GetChannelVector(GradientState* state, int16 channel);
VectorXd GetGradientQuotients(GradientState* state, MatrixXd* m, VectorXd* v);
void GetRecurrentQuotients(GradientState* state, VectorXd* q, VectorXd & rqX, VectorXd & rqY);

void DrawComplex(GradientState* state, VRect renderingRect, int32 stride, int32 channel, uint8* helpBuffer, uint8* outputBuffer);

#pragma endregion

#pragma region LEGACY

#define LinearDistanceGranularityQuotient 1
#define RadialDistanceGranularityQuotient 3
#define RadialDistanceGranularityQuotientSqr 9
#define AngleDistanceGranularity 10000
#define AngleDistanceGranularityQuotient 2546 

#define PI2546 7998
#define x2PI2546 15997
#define x05PI2546 3999

void GetLinearQuotients(double L1, double L2, double c1, double c2, double & a0, double & a1);

double LinearDistance   (double x,  double y,  double Ax, double Ay);

double RadialDistance   (double x1, double y1, double x2, double y2);
double RadialDistanceSqr(double x1, double y1, double x2, double y2);

void InitializeArcTanLookupTable(void);
void DestroyArcTanLookupTable(void);
double AngleDistance (double x1, double y1, double x2, double y2);

void PopulateLinearGradientLookup(GradientState* state, int32 width, int32 height, int32 channel, uint8* lookupBuffer, int32* offset);
void PopulateRadialGradientLookup(GradientState* state, int32 width, int32 height, int32 channel, uint8* lookupBuffer);
void PopulateAngledGradientLookup(GradientState* state, int32 width, int32 height, int32 channel, uint8* lookupBuffer);

void DrawRadial(GradientState* state, VRect renderingRect, int32 stride, int32 channel, uint8* helpBuffer, uint8* outputBuffer);
void DrawLinear(GradientState* state, VRect renderingRect, int32 stride, int32 channel, uint8* helpBuffer, uint8* outputBuffer);
void DrawAngled(GradientState* state, VRect renderingRect, int32 stride, int32 channel, uint8* helpBuffer, uint8* outputBuffer);

#pragma endregion

void DrawGradient(GradientState* state, VRect renderingRect, int32 stride, int32 channel, uint8* helpBuffer, uint8* outputBuffer);

#pragma region PROXY

#define transparencyChannel 3

void GetRectangles(ProxyState* proxyState, RectangleRecord* rects);
void WriteRectangle(uint8* destination, VRect writeRectangle, int32 pixelSize, int32 offset, int32 stride, uint8 color);

void WriteProxyMergedChannels(RectangleRecord* rects, int32 proxyPixelSize, int32 stride, uint8* destination);

typedef void(*WriteProxyChannelGradientFunction)
                             (GradientState* grState, RectangleRecord* rects, uint8* helpBuffer, bool superimpose, int32 channel, int32 proxyPixelSize, int32 bufferOffset, int32 stride, uint8* destination);
void WriteProxyChannelComplex(GradientState* grState, RectangleRecord* rects, uint8* helpBuffer, bool superimpose, int32 channel, int32 proxyPixelSize, int32 bufferOffset, int32 stride, uint8* destination);
void WriteProxyChannelLinear (GradientState* grState, RectangleRecord* rects, uint8* helpBuffer, bool superimpose, int32 channel, int32 proxyPixelSize, int32 bufferOffset, int32 stride, uint8* destination);
void WriteProxyChannelRadial (GradientState* grState, RectangleRecord* rects, uint8* helpBuffer, bool superimpose, int32 channel, int32 proxyPixelSize, int32 bufferOffset, int32 stride, uint8* destination);
void WriteProxyChannelAngled (GradientState* grState, RectangleRecord* rects, uint8* helpBuffer, bool superimpose, int32 channel, int32 proxyPixelSize, int32 bufferOffset, int32 stride, uint8* destination);

void WriteProxyGradient(GradientState* grState, ProxyState* pState, bool pStateChanged, RectangleRecord* rects, uint8* helpBuffer, int32 proxyPixelSize, int32 stride, uint8* destination);
void WriteProxy        (GradientState* grState, ProxyState* pState, bool pStateChanged, uint8* helpBuff, int32 proxyPixelSize, int32 stride, uint8* mergedBuffer, uint8* gradientBuffer, int32 & writeWidth, int32 & writeHeight);

#pragma endregion


#endif