// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2002 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this 
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.
//-------------------------------------------------------------------
//----------------------------------------------------------------------------
//
//	Code File Name:			PhotoDeluxeWDEFs.c
//
//	Code File Purpose:		To create WDEFs for PhotoDeluxe
//
//----------------------------------------------------------------------------
//
//	Developer:				Paul C. Anderson
//
//	Date Written:			04-12-95
//
//----------------------------------------------------------------------------
//	
//	Special Notes and Information:
//	-----------------------------
//	
//		The various types of windows are produced by using the varCode as and
//			index into a table of supported window types.  To add another
//			window type, just add a new entry into the window type table.
//
//		----------------------------------------------------------------------
//
//		When it can be figured out how to make this a safe fat binary code
//			resource, it should be done.  At this moment in time, however,
//			there is no way to use the standard descriptor macros, in the
//			calling program for a WDEF, to specify a safe fat binary code
//			resource for a WDEF since the resource ID for the WDEF sits in
//			either a WIND resource or is merely put as a number in a NewWindow
//			or NewCWindow call.
//
//		----------------------------------------------------------------------
//
//		The MyDataRecord type below was created so other data besides just the
//			userState and stdState data could be stored in the window record's
//			dataHandle.  (Normally just a WStateData record is stored in the
//			dataHandle.  Since MyDataRecord begins with the same fields, it is
//			compatable with the WStateData record, yet other field can be
//			added to MyDataRecord and used internally by this WDEF.
//
//----------------------------------------------------------------------------
//
//	History of Change:
//
//		06-01-95	RTS		Extended window type capabilities.
//							Added color capabilities
//							Added 3D icons
//		04-12-95	PCA		Initial Design and Code.
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
//	INCLUDES
//
//----------------------------------------------------------------------------

#include "Traps.h"		// used for DebugStr

#include "A4Stuff.h"
#include "SetupA4.h"

//----------------------------------------------------------------------------
//
//	MACROS (DEFINES)
//
//----------------------------------------------------------------------------

#define LIGHTBOTTOM		1		// 3D Rect style:  light bottom
#define DARKBOTTOM		2		// 3D Rect style:  dark bottom

//----------------------------------------------------------------------------
//
//	TYPEDEFS
//
//----------------------------------------------------------------------------

//typedef struct
//{
//	Rect	userState,
//			stdState;
//}  MyDataRecord, *MyDataPointer, **MyDataHandle;

typedef WStateData MyDataRecord, *MyDataPointer, **MyDataHandle;

//----------------------------------------------------------------------------
//
//	GLOBAL VARIABLES
//
//----------------------------------------------------------------------------

// window types
struct
{
	short fFrameWidth;				// frame width
	short fTitleBarHeight;			// title bar height
	RGBColor fTitleBarColor;		// title bar color
	RGBColor fTitleBarLightColor;	// title bar color
	RGBColor fTitleBarDarkColor;	// title bar color
	RGBColor fTitleTextColor;		// title text color
	short fTitleFontID;				// title bar font id
	short fTitleFontSize;			// title bar font size
	short fTitleFontFace;			// title bar font face
	Point fCloseBoxSize;			// close box size
	Point fCloseBoxOffset;			// close box offset
	Point fZoomBoxSize;				// zoom box size
	Point fZoomBoxOffset;			// zoom box offset
	Point fScrollBarSize;			// scroll bar size
	Point fGrowBoxSize;				// grow box size
} const kWindowType[] =
{
	// varcode 0 -- a null wdef
	{
		0,							// frame width
		0,							// title bar height
		{ 0, 0, 0 },				// title bar color
		{ 0, 0, 0 },				// title bar light color
		{ 0, 0, 0 },				// title bar dark color
		{ 0, 0, 0 },				// title text color
		0,							// title bar font id
		0,							// title bar font size
		0,							// title bar font face
		{ 0, 0 },					// close box size
		{ 0, 0 },					// close box offset
		{ 0, 0 },					// zoom box size
		{ 0, 0 },					// zoom box offset
		{ 0, 0 },					// scroll bar size
		{ 0, 0 },					// grow box size
	},

	// varcode 1 -- 1 pixel frame, title, close, zoom, scroll, grow
	{
		1,							// frame width
		15,							// title bar height
		{ 13107, 0, 39321 },		// title bar color
		{ 65535, 65535, 65535 },	// title bar light color
		{ 32768, 32768, 32768 },	// title bar dark color
		{ 65535, 65535, 65535 },	// title text color
		1,							// title bar font id -- 1 stands for the default application font
		9,							// title bar font size
		bold,						// title bar font face
		{  9,  9 },					// close box size
		{  3,  3 },					// close box offset
		{  9,  9 },					// zoom box size
		{  3,  3 },					// zoom box offset
		{ 15, 15 },					// scroll bar size
		{ 15, 15 },					// grow box size
	},

	// varcode 2 -- 1 pixel frame, title, close, no zoom, no scroll, no grow
	{
		1,							// frame width
		15,							// title bar height
		{ 13107, 0, 39321 },		// title bar color
		{ 65535, 65535, 65535 },	// title bar light color
		{ 32768, 32768, 32768 },	// title bar dark color
		{ 65535, 65535, 65535 },	// title text color
		1,							// title bar font id -- 1 stands for the default application font
		9,							// title bar font size
		bold,						// title bar font face
		{  9,  9 },					// close box size
		{  3,  3 },					// close box offset
		{  0,  0 },					// zoom box size
		{  0,  0 },					// zoom box offset
		{  0,  0 },					// scroll bar size
		{  0,  0 },					// grow box size
	}
}, *gWindowType;

// usefull colors
RGBColor gWhiteColor	= { 65535, 65535, 65535 };
RGBColor gBlackColor	= { 0, 0, 0 };

//----------------------------------------------------------------------------
//
//	FUNCTIONS
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
//	void SetupPort(Boolean color)
//
//	This routine sets up the port for a drawing operation.  This may include setting the
//	port to the COLOR Window Manager port and copying fields from the old Window Manager
//	port.
//
//	Parameters:
//		None.
//
//	Returns:
//		Nothing.
//
//----------------------------------------------------------------------------
static void
SetupPort( Boolean color )
{
	GrafPtr		bwPort;
	CGrafPtr	colorPort;
	
	GetWMgrPort( &bwPort );
	if ( color )
	{
		GetCWMgrPort( &colorPort );
		SetPort( (GrafPtr) colorPort );
		colorPort->pnLoc = bwPort->pnLoc;
		colorPort->pnSize = bwPort->pnSize;
		colorPort->pnMode = bwPort->pnMode;
	
		// Skip pnPat, but copy remainder of fields
		// Note: The length 38 copies everything up to (but not including) grafProcs.
		BlockMoveData( &bwPort->pnVis, &colorPort->pnVis, 38 );
		PenPat( &bwPort->pnPat );
		BackPat( &bwPort->bkPat );
	}
	else
	{
		SetPort( bwPort );
	}
}

//----------------------------------------------------------------------------
//
//	void Draw3DRect(WCTabHandle wctb, Rect *box, short boxStyle, short fillColor)
//
//	This routine draws a three-dimensional-looking rectangle in one of two styles.  The
//	LIGHTBOTTOM style is used for close and zoom boxes.  The DARKBOTTOM style is used in
//	the grow box.  CHANGES COLOR STATE.
//
//	Parameters:
//		theBox		The rectangle we're drawing.
//		boxStyle	Either LIGHTBOTTOM or DARKBOTTOM.
//		fillColor	The shade to fill the center with.
//
//	Returns:
//		Nothing.
//
//----------------------------------------------------------------------------
static void
Draw3DRect( Rect *theBox, short boxStyle, RGBColor *fillColor )
{	
	Rect box = *theBox;
	
	RGBColor savedColor;
	GetForeColor( &savedColor );
	
	// draw top left highlight
	RGBForeColor( (boxStyle == LIGHTBOTTOM) ? &gWindowType->fTitleBarDarkColor : &gWindowType->fTitleBarLightColor );
	MoveTo( box.left, box.bottom - 1 );			// Left vertical
	LineTo( box.left, box.top );
	LineTo( box.right - 1, box.top );			// Top horizontal

	// draw bottom right highlight
	box.top += 1;
	box.left += 1;
	MoveTo( box.left + 1, box.bottom - 2  );	// Inset bottom horizontal
	LineTo( box.right - 2, box.bottom - 2 );
	LineTo( box.right - 2, box.top + 1 );		// Inset right vertical

	// frame box
	RGBForeColor( (boxStyle == LIGHTBOTTOM) ? &gWindowType->fTitleBarLightColor : &gWindowType->fTitleBarDarkColor );
	FrameRect( &box );							// Offset box
	
	// fill box
	RGBForeColor( fillColor );
	InsetRect( &box, 1, 1 );
	if ( boxStyle == LIGHTBOTTOM )
	{
		box.bottom -= 1;
		box.right -= 1;
	}
	PaintRect( &box );

	RGBForeColor( &savedColor );
}

//----------------------------------------------------------------------------
//
//	CalculateTitleBarRect
//
//		Compute title bar rect
//
//----------------------------------------------------------------------------
static void
CalculateTitleBarRect( const Rect *strucRect, Rect *titleBarRect )
{
	*titleBarRect			= *strucRect;
	titleBarRect->bottom	= titleBarRect->top + gWindowType->fTitleBarHeight;
}

//----------------------------------------------------------------------------
//
//	CalculateCloseBoxRect
//
//		Calculate close box rectangle in global coordinates.
//
//----------------------------------------------------------------------------
static void
CalculateCloseBoxRect( const Rect *structRect, Rect *closeBoxRect )
{
	*closeBoxRect = *structRect;
	
	OffsetRect( closeBoxRect, gWindowType->fCloseBoxOffset.h, gWindowType->fCloseBoxOffset.v );

	closeBoxRect->bottom	= closeBoxRect->top  + gWindowType->fCloseBoxSize.v;
	closeBoxRect->right		= closeBoxRect->left + gWindowType->fCloseBoxSize.h;
}

//----------------------------------------------------------------------------
//
//	CalculateZoomBoxRect
//
//		Calculate zoom box rectangle in global coordinates.
//
//----------------------------------------------------------------------------
static void
CalculateZoomBoxRect( const Rect *structRect, Rect *zoomBoxRect )
{
	short	titleBarWidth = structRect->right - structRect->left;
	
	*zoomBoxRect = *structRect;
	
	OffsetRect( zoomBoxRect, titleBarWidth - gWindowType->fZoomBoxOffset.h - gWindowType->fZoomBoxSize.h, gWindowType->fZoomBoxOffset.v );
	zoomBoxRect->bottom	= zoomBoxRect->top + gWindowType->fZoomBoxSize.v;
	zoomBoxRect->right	= zoomBoxRect->left + gWindowType->fZoomBoxSize.h;
}

//----------------------------------------------------------------------------
//
//	CalculateGrowBoxRect
//
//		Calculate grow box rectangle in global coordinates.
//
//----------------------------------------------------------------------------
static void
CalculateGrowBoxRect( const Rect *contentRect, Rect *growBoxRect )
{
	*growBoxRect = *contentRect;
	
	growBoxRect->top	= growBoxRect->bottom - gWindowType->fGrowBoxSize.v;
	growBoxRect->left	= growBoxRect->right  - gWindowType->fGrowBoxSize.h;
}

//----------------------------------------------------------------------------
//
//	CalculateContentRegion
//
//		Calculate content region in global coordinates.
//
//----------------------------------------------------------------------------
static void
CalculateContentRegion( WindowPtr theWindow )
{
	GrafPtr			savedPort;
	MyDataHandle	theData		= (MyDataHandle) ((CWindowPeek) theWindow)->dataHandle;
	Rect			tempRect	= ((CWindowPtr) theWindow)->portRect;
	
	
	// retrieve the portRect in global (from local) coordinates
	GetPort( &savedPort );
	SetPort( theWindow );
	
	LocalToGlobal( &topLeft( tempRect ) );
	LocalToGlobal( &botRight( tempRect ) );
	
	SetPort( savedPort );
	
	// set contRgn equal to  portRect
	RectRgn( ((CWindowPeek) theWindow)->contRgn, &tempRect );
	
	// update information in window dataHandle of window
	if ( ! EqualRect( &(*theData)->userState, &tempRect ) ) (*theData)->userState = tempRect;
}

//----------------------------------------------------------------------------
//
//	CalculateStructureRegion
//
//		Calculate structure region in global coordinates.
//
//----------------------------------------------------------------------------
static void
CalculateStructureRegion( WindowPtr theWindow )
{
	// start with content region bounding box
	Rect tempRect = (*((CWindowPeek) theWindow)->contRgn)->rgnBBox;
	
	// add frame width and title bar height to temporary rectangle
	InsetRect( &tempRect, -gWindowType->fFrameWidth, -gWindowType->fFrameWidth );
	tempRect.top -= gWindowType->fTitleBarHeight;
	
	// set structure region
	RectRgn( ((CWindowPeek) theWindow)->strucRgn, &tempRect );
}

//----------------------------------------------------------------------------
//
//	DrawTitleBarFrame
//
//		Draw title bar frame
//
//----------------------------------------------------------------------------
static void
DrawTitleBarFrame( WindowPtr theWindow, Rect *titleBarRect )
{
	RGBColor savedColor;

	// fill title bar
	GetBackColor( &savedColor );
	
	if ( ((CWindowPeek) theWindow)->hilited ) 
		RGBBackColor( &gWindowType->fTitleBarColor );
	else
	{
		RGBBackColor( &gWindowType->fTitleBarLightColor );
		RGBForeColor( &gWindowType->fTitleTextColor );
	}
	
	EraseRect( titleBarRect );
	RGBBackColor( &savedColor );

	// draw title bar frame
	titleBarRect->bottom += 1;
	FrameRect( titleBarRect );
	titleBarRect->bottom -= 1;
}

//----------------------------------------------------------------------------
//
//	DrawTitleBarTitleText
//
//		Draw title bar title text
//
//----------------------------------------------------------------------------
static void
DrawTitleBarTitleText( WindowPtr theWindow, Rect *titleBarRect )
{
	short			closeBoxSpace,
					spaceForTitle,
					titleOffset,
					titleBarWidth,
					titleWidth,
					xStartPos,
					yStartPos,
					zoomBoxSpace;

	RGBColor		savedForeColor, savedBackColor;

	StringHandle	titleHandle		= ((CWindowPeek) theWindow)->titleHandle;
					
	Str255			title;

	// compute space available on drag region for title
	titleBarWidth	= titleBarRect->right - titleBarRect->left;
	closeBoxSpace	= (2 * gWindowType->fCloseBoxOffset.h) + gWindowType->fCloseBoxSize.h;
	zoomBoxSpace	= (2 * gWindowType->fZoomBoxOffset.h) + gWindowType->fZoomBoxSize.h;
//	spaceForTitle	= titleBarWidth - closeBoxSpace - zoomBoxSpace;
	spaceForTitle	= titleBarWidth - closeBoxSpace - closeBoxSpace;	// temporary fix for centering the title in the drag region
	
	// if we have room for a title
	if ( spaceForTitle > 0 )
	{
		// copy title string into temporary storage for use
		BlockMove( *titleHandle, title, (*titleHandle)[0] + 1 );
		
		// shorten title string for placement into drag region -- in case it's too long
		titleWidth = StringWidth( title );
		while ( titleWidth > spaceForTitle )
		{
			title[ 0 ]--;
			titleWidth = StringWidth( title );
		}
		
		//	if we have a title
		if ( title[0] )
		{
			// set position of text
			titleOffset	= (spaceForTitle - titleWidth) / 2;
			xStartPos	= titleBarRect->left + closeBoxSpace + titleOffset;
			yStartPos	= titleBarRect->bottom - gWindowType->fTitleBarHeight / 4;
			MoveTo( xStartPos, yStartPos );
				
			// set color and draw text
			GetBackColor( &savedBackColor );
			GetForeColor( &savedBackColor );
			if ( ((CWindowPeek) theWindow)->hilited )
			{
				RGBBackColor( &gWindowType->fTitleBarColor );
				RGBForeColor( &gWindowType->fTitleTextColor );
			}
			else
			{
				RGBBackColor( &gWindowType->fTitleBarLightColor );
				RGBForeColor( &gWindowType->fTitleTextColor );
			}
			
			DrawString( title );
			RGBForeColor( &savedBackColor );
			RGBBackColor( &savedBackColor );
		}
	}
}

//----------------------------------------------------------------------------
//
//	DrawCloseBox
//
//		Draw close box on window title bar
//
//----------------------------------------------------------------------------
static void
DrawCloseBox( WindowPtr theWindow, Rect *closBoxRect, Boolean hilite )
{
	if ( ((CWindowPeek) theWindow)->hilited )
		Draw3DRect( closBoxRect, hilite ? LIGHTBOTTOM : DARKBOTTOM, &gWindowType->fTitleBarColor );
}

//----------------------------------------------------------------------------
//
//	DrawZoomBox
//
//		Draw window's zoom box
//
//----------------------------------------------------------------------------
static void
DrawZoomBox( WindowPtr theWindow, Rect *zoomBoxRect, Boolean hilite )
{
	if ( ((CWindowPeek) theWindow)->hilited )
	{
		RGBColor savedColor;
		GetForeColor( &savedColor );
	
		Draw3DRect( zoomBoxRect, hilite ? LIGHTBOTTOM : DARKBOTTOM, &gWindowType->fTitleBarColor );
	
		RGBForeColor( hilite ?  &gWindowType->fTitleBarLightColor : &gWindowType->fTitleBarDarkColor );
		MoveTo( zoomBoxRect->left + 3, zoomBoxRect->top + (zoomBoxRect->bottom - zoomBoxRect->top) / 2 );
		LineTo( zoomBoxRect->left + 1 + (zoomBoxRect->right - zoomBoxRect->left) / 2, zoomBoxRect->top + (zoomBoxRect->bottom - zoomBoxRect->top) / 2 );
		LineTo( zoomBoxRect->left + 1 + (zoomBoxRect->right - zoomBoxRect->left) / 2, zoomBoxRect->top + 3 );
	
		RGBForeColor( &savedColor );
	}
}

//----------------------------------------------------------------------------
//
//	HiliteCloseBox
//
//		Handle hiliting and unhiliting close box
//
//----------------------------------------------------------------------------
static void
HiliteCloseBox( WindowPtr theWindow, Rect *closeBoxRect )
{
	Point	localPoint;
	GetMouse( &localPoint );
	
	// hilite close box
	DrawCloseBox( theWindow, closeBoxRect, PtInRect( localPoint, closeBoxRect ) );
}

//----------------------------------------------------------------------------
//
//	HiliteZoomBox
//
//		Handle hiliting and unhiliting zoom box
//
//----------------------------------------------------------------------------
static void
HiliteZoomBox( WindowPtr theWindow, Rect *zoomBoxRect )
{
	Point localPoint;
	GetMouse( &localPoint );
	
	// hilite zoom box
	DrawZoomBox( theWindow, zoomBoxRect, PtInRect( localPoint, zoomBoxRect ) );
}

//----------------------------------------------------------------------------
//
//	InitializeWindow
//
//		window type specific extra initialization
//
//----------------------------------------------------------------------------
static void
InitializeWindow( WindowPtr theWindow )
{
	// initialize spare flag for resizeable window
	if ( gWindowType->fGrowBoxSize.h) ((CWindowPeek) theWindow)->spareFlag = TRUE;
		
	// allocate dataHandle information
	if ( ! ((CWindowPeek) theWindow)->dataHandle )
	{
		((CWindowPeek) theWindow)->dataHandle = NewHandle( sizeof( MyDataRecord ) );
		
		// set application window size
		if ( ((CWindowPeek) theWindow)->dataHandle )
		{
			SetRect( &(*((MyDataHandle) ((CWindowPeek) theWindow)->dataHandle))->userState, 0, 0, 0, 0 );
			SetRect( &(*((MyDataHandle) ((CWindowPeek) theWindow)->dataHandle))->stdState, 0, 0, 0, 0 );
		}
	}
}

//----------------------------------------------------------------------------
//
//	DisposeTheWindow
//
//		window type specific extra disposing
//
//----------------------------------------------------------------------------
static void
DisposeTheWindow( WindowPtr theWindow )
{
	//	dispose of dataHandle information
	if (((CWindowPeek) theWindow)->dataHandle) DisposHandle( ((CWindowPeek) theWindow)->dataHandle );
}

//----------------------------------------------------------------------------
//
//	CalculateRegions
//
//		Calculate regions of window structure from portRect.
//		These regions are in global coordinates.
//
//----------------------------------------------------------------------------
static void
CalculateRegions( WindowPtr theWindow )
{
	// calculate specific regions
	CalculateContentRegion( theWindow );
	CalculateStructureRegion( theWindow );
}

//----------------------------------------------------------------------------
//
//	DrawWindowFrame
//
//		draw window frame; only use low-order word of param
//		the upper word is garbage
//
//		The coordinate system is the current window's port
//
//----------------------------------------------------------------------------
static void
DrawWindowFrame( WindowPtr theWindow, long param )
{
	short		drawPart = (short) param;
	
	GrafPtr		savedPort;

	PenState	savedPenState;
	RGBColor	savedForeColor;
	short		txFont,
				txSize,
				txFace,
				txMode;

	Rect		strucRect;

	// draw nothing if window is invisible
	if ( ! ((CWindowPeek) theWindow)->visible ) return;
		
	// draw nothing if no frame
	if ( gWindowType->fFrameWidth == 0 ) return;
	
	// save port state
	GetPenState( &savedPenState );

	GetForeColor( &savedForeColor );
	
	GetPort( &savedPort );
	txFont = savedPort->txFont;
	txSize = savedPort->txSize;
	txFace = savedPort->txFace;
	txMode = savedPort->txMode;

	// set font
	TextFont( gWindowType->fTitleFontID );
	TextSize( gWindowType->fTitleFontSize );
	TextFace( gWindowType->fTitleFontFace );
	TextMode( srcCopy );

	// set current pen state and color
	PenNormal();
	RGBForeColor( &gBlackColor );
	
	strucRect = (*((CWindowPeek) theWindow)->strucRgn)->rgnBBox;

	// draw whole window frame
	if ( ! drawPart )
	{
		Rect frameRect = (*((CWindowPeek) theWindow)->contRgn)->rgnBBox;
		Rect titleBarRect;

		// draw window frame
		InsetRect( &frameRect, -1, -1 );
		FrameRect( &frameRect );
		
		// draw title bar
		CalculateTitleBarRect( &strucRect, &titleBarRect );
		DrawTitleBarFrame( theWindow, &titleBarRect );
		DrawTitleBarTitleText( theWindow, &titleBarRect );
		
		// draw close box
		if ( ((CWindowPeek) theWindow)->goAwayFlag && gWindowType->fCloseBoxSize.h  )
		{
			Rect closeBoxRect;
			CalculateCloseBoxRect( &strucRect, &closeBoxRect );
			DrawCloseBox( theWindow, &closeBoxRect, FALSE );
		}

		// draw zoom box
		if ( (((CWindowPeek) theWindow)->spareFlag) && gWindowType->fZoomBoxSize.h )
		{
			Rect zoomBoxRect;
			CalculateZoomBoxRect( &strucRect, &zoomBoxRect );
			DrawZoomBox( theWindow, &zoomBoxRect, FALSE );
		}
	}
	
	// draw close box hilighting
	else if ( drawPart == wInGoAway )
	{
		Rect closeBoxRect;
		CalculateCloseBoxRect( &strucRect, &closeBoxRect );
		HiliteCloseBox( theWindow, &closeBoxRect );
	}
	
	// draw zoom box hilighting for resizeable window
	else if ( (drawPart == wInZoomIn || drawPart == wInZoomOut) && gWindowType->fGrowBoxSize.h )
	{
		Rect zoomBoxRect;
		CalculateZoomBoxRect( &strucRect, &zoomBoxRect );
		HiliteZoomBox( theWindow, &zoomBoxRect );
	}
	
	// restore port state
	TextFont( txFont );
	TextSize( txSize );
	TextFace( txFace );
	TextMode( txMode );

	RGBForeColor( &savedForeColor );

	SetPenState( &savedPenState );
}

//----------------------------------------------------------------------------
//
//	DrawGrowBox
//
//		window grow box drawing
//
//----------------------------------------------------------------------------
static void
DrawGrowBox( WindowPtr theWindow )
{
	GrafPtr		savedPort;
	PenState	savedPenState;
	RGBColor	savedColor;
	Rect		growBoxRect;
	short		hLineOffset,
				vLineOffset;
	
	// draw scroll bars and resize box only for resizeable window
	if ( gWindowType->fGrowBoxSize.h )
	{
		// set up environment for drawing to correct port
		GetPort( &savedPort );
		SetPort( theWindow );
			
		// save current pen state and color
		GetPenState( &savedPenState );
		GetForeColor( &savedColor );
		
		// set current pen state and color
		PenNormal();
		RGBForeColor( &gBlackColor );
		
		// draw lines for scroll bar rectangles
		hLineOffset = theWindow->portRect.right - gWindowType->fScrollBarSize.h;
		vLineOffset = theWindow->portRect.bottom - gWindowType->fScrollBarSize.v;
		
		// draw lines delimiting size box and scroll bars
		MoveTo( theWindow->portRect.left, vLineOffset );
		LineTo( theWindow->portRect.right, vLineOffset );
		
		MoveTo( hLineOffset, theWindow->portRect.top );
		LineTo( hLineOffset, theWindow->portRect.bottom );
		
		// compute size box
		growBoxRect			= theWindow->portRect;
		growBoxRect.top		= growBoxRect.bottom - gWindowType->fScrollBarSize.v;
		growBoxRect.left	= growBoxRect.right - gWindowType->fScrollBarSize.h;
		
		// Frame
		FrameRect( &growBoxRect );
		InsetRect( &growBoxRect, 1, 1 );

		// draw size box in content region
		if ( ((CWindowPeek) theWindow)->hilited )
		{
			// Fill
			RGBForeColor( &gWindowType->fTitleBarColor );
			PaintRect( &growBoxRect );
			
			// Bigger 3D box
			growBoxRect.top += 3;
			growBoxRect.left += 3;
			growBoxRect.bottom -= 2;
			growBoxRect.right -= 2;
			Draw3DRect( &growBoxRect, DARKBOTTOM, &gWindowType->fTitleBarColor );
			
			// Smaller 3D box
			growBoxRect.top -= 1;
			growBoxRect.left -= 1;
			growBoxRect.bottom -= 4;
			growBoxRect.right -= 4;
			Draw3DRect( &growBoxRect, DARKBOTTOM, &gWindowType->fTitleBarColor );
		}

		// restore current color and pen state
		RGBForeColor( &savedColor );
		SetPenState( &savedPenState );
		
		// reset to saved port
		SetPort( savedPort );
	}
}

//----------------------------------------------------------------------------
//
//	DrawGrowWindow
//
//		Window resizing.  Received rectPtr is in global coordinates.  Its
//		upper left corner is aligned with portRect.
//
//----------------------------------------------------------------------------
static void
DrawGrowWindow( WindowPtr theWindow, long rectPtr )
{
	if ( gWindowType->fGrowBoxSize.h )
	{
		Rect	contentRect = *(Rect *)rectPtr;
		short	topLine;
		
		// set up lines for drawing
		InsetRect( &contentRect, -gWindowType->fFrameWidth, -gWindowType->fFrameWidth );
		topLine	= contentRect.top;
		contentRect.top -= gWindowType->fTitleBarHeight;
		
		// draw necessary lines
		FrameRect( &contentRect );
		
		MoveTo( contentRect.left, topLine );
		LineTo( contentRect.right, topLine );
		
		MoveTo( contentRect.right - gWindowType->fGrowBoxSize.h - gWindowType->fFrameWidth, contentRect.bottom - gWindowType->fGrowBoxSize.v - 1 - gWindowType->fFrameWidth);
		LineTo( contentRect.right, contentRect.bottom - gWindowType->fGrowBoxSize.v - 1 - gWindowType->fFrameWidth);
		
		MoveTo( contentRect.right - gWindowType->fGrowBoxSize.h - gWindowType->fFrameWidth, topLine );
		LineTo( contentRect.right - gWindowType->fGrowBoxSize.h - gWindowType->fFrameWidth, contentRect.bottom );
	}
}

//----------------------------------------------------------------------------
//
//	FindWindowPart
//
//		find window part that mouse is over
//
//----------------------------------------------------------------------------
static long
FindWindowPart( WindowPtr theWindow, long param )
{
	Boolean	windowIsActive	= ((CWindowPeek) theWindow)->hilited;

	Rect	closeBoxRect,
			contentRect		= (*((CWindowPeek) theWindow)->contRgn)->rgnBBox,
			growBoxRect,
			strucRect		= (*((CWindowPeek) theWindow)->strucRgn)->rgnBBox,
			titleBarRect,
			zoomBoxRect,
			stdStateRect	= (*((MyDataHandle) ((CWindowPeek) theWindow)->dataHandle))->stdState;
	
	long	hit				= wNoHit;

	// if point is in window
	if ( PtInRect( *(Point *)&param, &strucRect ) )
	{
		// if we have a title bar
		if ( gWindowType->fTitleBarHeight )
		{
			// calculate title bar rect
			CalculateTitleBarRect( &strucRect, &titleBarRect );

			// if in title bar
			if ( PtInRect( *(Point *)&param, &titleBarRect )	)
			{
				// if we have a close box	
				if  ( hit == wNoHit && gWindowType->fCloseBoxSize.h )
				{
					// calculate close box rect
					CalculateCloseBoxRect( &strucRect, &closeBoxRect );
	
					// if in close box
					if  ( PtInRect( *(Point *)&param, &closeBoxRect ) ) hit = windowIsActive ? wInGoAway : wInDrag;
				}
	
				// if we have a zoom box
				if ( hit == wNoHit && gWindowType->fZoomBoxSize.h )
				{
					// calculate zoom box rect
					CalculateZoomBoxRect( &strucRect, &zoomBoxRect );
			
					// if in zoom box
					if ( PtInRect( *(Point *)&param, &zoomBoxRect ) )
						hit = windowIsActive ? EqualRect( &contentRect, &stdStateRect ) ? wInZoomIn : wInZoomOut : wInDrag;
				}
	
				// if nothing hit then we're in drag
				if ( hit == wNoHit ) hit = wInDrag;
			}
		}

		// if we have a grow box
		if ( hit == wNoHit && gWindowType->fGrowBoxSize.h )
		{
			// calculate grow box rect
			CalculateGrowBoxRect( &contentRect, &growBoxRect );
			
			// in grow box
			if ( PtInRect( *(Point *)&param, &growBoxRect ) && windowIsActive ) hit = wInGrow;
		}

		// if no hit, point must be in content
		if ( hit == wNoHit ) hit = wInContent;
	}

	return hit;
}

//----------------------------------------------------------------------------
//
//	MAIN
//
//----------------------------------------------------------------------------
pascal long
main( short varCode, WindowPtr theWindow, short message, long param )
{
	//-------------------------------------------------------------------------------
	//	here A4 does not point to our data segment, so we cannot use any global data.
	//-------------------------------------------------------------------------------
	
	long		oldA4;				//	used to store old A4 value
	long		returnCode = 0L;

	GrafPtr		savedPort;
	oldA4 = SetCurrentA4();		//	setup A4 to point to global data segment
								//	before(!) using any global data or strings
								
	//-------------------------------------------------------------------------------
	//	now we can use global data as long as we stay within this code resource
	//-------------------------------------------------------------------------------
	
	// set window type
	gWindowType = &kWindowType[ (varCode >= 0 && varCode < sizeof( kWindowType ) / sizeof( kWindowType[ 0 ] )) ? varCode : 0 ];

	// save port
	GetPort( &savedPort );
	SetupPort( TRUE );

	// process message
	switch ( message )
	{
		case wNew:
			InitializeWindow( theWindow );
			break;
			
		case wDispose:
			DisposeTheWindow( theWindow );
			break;
			
		case wCalcRgns:
			CalculateRegions( theWindow );
			break;
			
		case wDraw:
			DrawWindowFrame( theWindow, param );
			break;
			
		case wDrawGIcon:
			DrawGrowBox( theWindow );
			break;

		case wGrow:
			DrawGrowWindow( theWindow, param );
			break;
			
		case wHit:
			returnCode = FindWindowPart( theWindow, param );
			break;
	}
	
	// restore port
	SetPort( savedPort );

	// restore a4
	SetA4( oldA4 );

	return returnCode;
}



