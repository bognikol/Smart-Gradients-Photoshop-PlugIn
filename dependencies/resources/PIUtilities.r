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
//-------------------------------------------------------------------------------
//
//	File:
//		PIUtilities.r
//
//
//	Description:
//		This rez file contains standard resources
//		to simplify the use of suites and also some
//		helpful common plug-in resources. 
//
//	Use:
//		PIUtilities is intended to group common resources
//		and functions into a simple library that provides
//		most of the basic plug-in functionality, including
//		error reporting and About boxes.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Definitions -- Plug-in supplies.
//	Create generic definitions if not supplied by plug-in.
//-------------------------------------------------------------------------------

#ifndef plugInName
	#define plugInName	"Generic Plug-in"
#endif

#ifndef plugInCopyrightYear
	#define plugInCopyrightYear		"2010"
#endif

#ifndef plugInDescription
	#define plugInDescription		"An example plug-in module for Adobe Photoshop¨."
#endif

//-------------------------------------------------------------------------------
//	Defines -- Version information for SDK releases.
//-------------------------------------------------------------------------------

#ifndef VersionNumber
	#define VersionNumber	12
#endif
	
#ifndef VersionString
	#define VersionString 	"CS5"
#endif

#ifndef ReleaseString
	#define ReleaseString	"1 (02/05/2010)"
#endif

#ifndef CurrentYear
	#define CurrentYear		"2010"
#endif

//-------------------------------------------------------------------------------
//	Defines -- Generic resources, such as about boxes and resource IDs.
//	Redefine these if you need to.
//-------------------------------------------------------------------------------

#ifndef StringResource
	#define StringResource	'STR '
#endif

#ifndef ResourceID
	#define ResourceID		16000
#endif

#ifndef AboutID
	#define AboutID			ResourceID
#endif

#ifndef uiID
	#define uiID			ResourceID+1
#endif

#ifndef ADMAboutID
	#define ADMAboutID		uiID+1
#endif

#ifndef AlertID
	#define AlertID			16990
#endif

#ifndef kBadNumberID
	#define kBadNumberID	AlertID
#endif

#ifndef kBadDoubleID
	#define kBadDoubleID	kBadNumberID+1
#endif

#ifndef kNeedVers
	#define kNeedVers		kBadDoubleID+1
#endif

#ifndef kWrongHost
	#define kWrongHost		kNeedVers+1
#endif

#ifndef kSuiteMissing
	#define kSuiteMissing	kWrongHost+1
#endif

//-------------------------------------------------------------------------------
//	Alert strings.
//-------------------------------------------------------------------------------

resource StringResource (kBadNumberID, "Bad number", purgeable)
{
	"An integer is required between ^1 and ^2."
};

resource StringResource (kBadDoubleID, "Bad double", purgeable)
{
	"A decimal number is required between ^1 and ^2."
};

resource StringResource (kNeedVers, "Need vers", purgeable)
{
	"This plug-in requires Adobe Photoshop¨ ^1 or later functionality."
};

resource StringResource (kWrongHost, "Wrong host", purgeable)
{
	"This plug-in is incompatible with this version of the host program."
};

resource StringResource (kSuiteMissing, "Suite missing", purgeable)
{
	/* The host cannot complete this command because... */
	"the required suite \"^0\" is unavailable" /* period is added by host */
};

//-------------------------------------------------------------------------------
//	About string resource.  This is stored as text, instead of explicitly in
//	the About box definition, so that it can be used cross-platform.  Resources
//	of type 'STR ' (StringResource) are converted automatically by CNVTPIPL.
//-------------------------------------------------------------------------------

resource StringResource (AboutID, plugInName " About Text", purgeable)
{
	plugInName "\n\n"
	"Version " VersionString " "
	"Release " ReleaseString "\n"
	"Copyright " plugInCopyrightYear "-" CurrentYear ", Adobe Systems Incorporated.\n"
	"All Rights Reserved.\n\n"
	plugInDescription
};

// The ADM provided about box doesn't understand literals, but does understand
// slash characters.  So we'll provide it:
resource StringResource (ADMAboutID, plugInName "About Text", purgeable)
{
	plugInName "\\n\\n"
	"Version " VersionString " "
	"Release " ReleaseString "\\n"
	"Copyright " plugInCopyrightYear "-" CurrentYear " "
	"Adobe Systems Incorporated. "
	"All Rights Reserved.\\n\\n"
	plugInDescription "\\n\\n\\n\\n" // Must end with \\n \\n due to resizing error.
};
	
//-------------------------------------------------------------------------------
//	About dialog box.
//-------------------------------------------------------------------------------

resource 'DLOG' (AboutID, plugInName " Generic About Box", purgeable)
{
	{20, 0, 214, 390},
	movableDBoxProc,
	visible,
	noGoAway,
	0x0,
	ResourceID,
	"About " plugInName "...",
	alertPositionMainScreen	// Universal 3.0 headers.
};

resource 'dlgx' (AboutID) {
	versionZero {
		kDialogFlagsHandleMovableModal + kDialogFlagsUseThemeControls + kDialogFlagsUseThemeBackground
	}
};

resource 'DITL' (AboutID, plugInName " Generic About Box", purgeable)
{
	{
		{-80, 0, -60, 60}, 		Button { enabled, "Hidden" },
		{5, 5, 155, 380}, 		UserItem { enabled },
		{5, 5, 155, 380},		StaticText { disabled, "^0\n^1\n^2\n" },
		{160, 10, 180, 190}, 	Button { enabled, "www.adobe.com" },
		{160, 200, 180, 380},	Button { enabled, "partners.adobe.com" } 
	}
};

//-------------------------------------------------------------------------------
//	Alert dialog box.
//-------------------------------------------------------------------------------

resource 'DITL' (AlertID, plugInName " Generic Alert", purgeable)
{
	{	/* array DITLarray: 2 elements */
		/* [1] */
		{87, 230, 107, 298},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{12, 60, 76, 300},
		StaticText {
			disabled,
			"^0"
		}
	}
};

resource 'ALRT' (AlertID, plugInName " Generic Alert", purgeable)
{
	{0, 0, 120, 310},
	AlertID,
	{	/* array: 4 elements */
		/* [1] */
		OK, visible, sound1,
		/* [2] */
		OK, visible, sound1,
		/* [3] */
		OK, visible, sound1,
		/* [4] */
		OK, visible, sound1
	},
	alertPositionParentWindowScreen	// Universal 3.0 headers.
};

//-------------------------------------------------------------------------------
//	Version 'vers' resource.
//-------------------------------------------------------------------------------

resource 'vers' (1, plugInName " Version", purgeable)
{
	VersionNumber, 0x00, final, 0, verUs,
	VersionString,
	VersionString " (c) " plugInCopyrightYear "-" CurrentYear ", Adobe Systems Incorporated. All Rights Reserved."
};

//-------------------------------------------------------------------------------
//
//	'FltD' resource.  Dynamic resource for Adobe Premiere.
//
//	Descriptor for allowing filters to animate over time. A
//	structure of this type can be added to a 'VFlt', an 'AFlt',
//	or a Photoshop filter to describe the data structure of its
//	data blob. Specify pdOpaque for any non-scalar data in the
//	record, or data that you don't want Premiere to interpolate
//	for you. Make the FltD describe all the bytes in the data
//	blob. Use ID 1000D
//
//-------------------------------------------------------------------------------

type 'FltD' {
	array {
		integer					// Specifies the type of the data
			pdOpaque = 0,			// Opaque - don't interpolate this
			pdChar = 1,				// Interpolate as signed byte
			pdShort = 2,			// Interpolate as signed short
			pdLong = 3,				// Interpolate as signed long
			pdUnsignedChar = 4,		// Interpolate as unsigned byte
			pdUnsignedShort = 5,	// Interpolate as unsigned short
			pdUnsignedLong = 6,		// Interpolate as unsigned long
			pdExtended = 7,			// Interpolate as an extended
			pdDouble = 8,			// Interpolate as a double
			pdFloat = 9;			// Interpolate as a float
		integer;				// Count of bytes to skip with pdOpaque, 0 otherwise.
	};
};

//-------------------------------------------------------------------------------

// end PIUtilities.r
