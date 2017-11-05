#ifndef __PropertyUtils_H__
#define __PropertyUtils_H__

#include "PIGeneral.h"
#include "PIProperties.h"
#include <string>

using namespace std;

enum InterpolationMethod { pointSample = 1, bilinear, bicubic };
enum PaintCursorKind { standard = 1, precise, brushSize };
enum InterfaceColor { white = 0, 
                      buttonUpFill, 
					  bevelShadow, 
					  iconFillActive, 
                      iconFillDimmed,
					  paletteFill,
					  iconFrameDimmed,
					  iconFrameActive,
					  bevelHighlight,
					  buttonDownFill,
					  iconFillSelected,
					  border,
					  buttonDarkShadow,
					  iconFrameSelected,
					  black,
					  red };

OSErr HandleToString(const Handle h, string & outString);
OSErr HandleToString(const Handle h, wstring & outString);
OSErr HandleToString(const Handle h, Str255 & outString);

OSErr StringToHandle(const string & inString, Handle & outHandle);
OSErr StringToHandle(const wstring & inString, Handle & outHandle);

OSErr PIGetNumberChannels(intptr_t & numChannels);
OSErr PIGetChannelName(const int32 index, string & name);
OSErr PIGetChannelName(const int32 index, wstring & name);
	
OSErr PIGetImageMode(intptr_t & mode);

OSErr PIGetTargetLayerLock(bool & transparency, bool & composite, bool & position);
	
OSErr PIGetNumberLayers(intptr_t & layers);
OSErr PIGetTargetLayerIndex(intptr_t & index);
OSErr PIGetLayerName(const int32 index, string & layerName);
OSErr PIGetLayerName(const int32 index, wstring & layerName);

OSErr PIGetNumberPaths(intptr_t & numPaths);
OSErr PIGetPathName(const int32 index, string & name);
OSErr PIGetPathName(const int32 index, wstring & name);
OSErr PIGetWorkPathIndex(intptr_t & index);
OSErr PIGetClippingPathIndex(intptr_t & index);
OSErr PIGetTargetPathIndex(intptr_t & index);
OSErr PIGetPathContents(const int32 index, Handle & contents);
OSErr PIGetPathContentsAI(const int32 index, Handle & contents);
OSErr PIGetCaption(Handle & caption);
OSErr PISetCaption(const Handle h);

OSErr PIGetBigNudge(double & horizontal, double & vertical);
OSErr PISetBigNudge(const double horizontal, const double vertical);

OSErr PIGetInterpolationMethod(InterpolationMethod & method);

OSErr PIGetRulerUnits(RulerUnits & rulerUnits);

OSErr PIGetRulerOrigin(double & horizontal, double & vertical);
OSErr PISetRulerOrigin(const double horizontal, const double vertical);

OSErr PIGetGrid(double & major, double & minor);
OSErr PISetGrid(const double major, const double minor);

OSErr PIGetSerialString(string & serial);
OSErr PIGetSerialStringOld(string & serial);

#if __PIWin__
	OSErr PIGetHardwareGammaTable(Handle & table);
#endif

OSErr PIGetInterfaceColor(const InterfaceColor index, PIInterfaceColor & color);

OSErr PIGetWatchSuspension(intptr_t & watch);
OSErr PISetWatchSuspension(const intptr_t inWatchSuspension);

OSErr PIGetCopyright(bool & copyright);
OSErr PISetCopyright(bool inCopyright);

OSErr PIGetURL(string & url);
OSErr PISetURL(const string & inURL);

OSErr PIGetDocumentName(string & name);
OSErr PIGetDocumentName(wstring & name, bool withExtension = true);
OSErr PIGetUnicodeStr255Name(Str255 & name);

OSErr PIGetWatermark(bool & watermark);
OSErr PISetWatermark(void);

OSErr PIGetDocumentHeight(intptr_t & height);
OSErr PIGetDocumentWidth(intptr_t & width);

OSErr PIGetToolTips(bool & toopTips);

OSErr PIGetPaintCursorKind(PaintCursorKind & cursorKind);

OSErr PIGetSlices(Handle & slices);
OSErr PISetSlices(const Handle inSlices);

OSErr PIGetSelectedSliceID(intptr_t & sliceID);
OSErr PISetSelectedSliceID(const intptr_t sliceID);

OSErr PIGetSelectedSliceIndex(intptr_t & sliceIndex);
OSErr PISetSelectedSliceIndex(const intptr_t sliceIndex);

OSErr PIGetShowSliceNumbers(bool & showSliceNumbers);

OSErr PIGetSliceLineColor(RGBColor & lineColor);

OSErr PIGetPlayInProgress(bool & inProgress);

OSErr PIGetEXIFData(Handle & exifData);
OSErr PISetEXIFData(const Handle inEXIFData);

OSErr PIGetHostVersion(int32 & major, int32 & minor, int32 & fix);

OSErr PIGetXMP(string & xmp);
OSErr PISetXMP(const string & inXMP);

#endif
// end PropertyUtils.h