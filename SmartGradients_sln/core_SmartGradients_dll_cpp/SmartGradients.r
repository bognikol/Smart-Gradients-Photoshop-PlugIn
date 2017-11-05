// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2002 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this 
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.

//-------------------------------------------------------------------------------
#include "PIDefines.h"

#ifdef __PIMac__
	#include <Carbon.r>
	#include "PIGeneral.r"
	#include "DissolveScripting.h"
	#include "PIUtilities.r"
#elif defined(__PIWin__)
	#define Rez
	#include "PIGeneral.h"
	#include "PIUtilities.r"
#endif

#include "PIDefines.h"
#include "PIActions.h"
#include "PITerminology.h"

#ifndef Rez
#include "PlugInHostCommunication.h"
#endif

#define vendorName			"BogdanNikolic"
#define plugInName			"Smart Gradients"
#define plugInAETEComment	""
#define plugInSuiteID		'sdk1'
#define plugInClassID		plugInSuiteID
#define plugInEventID		plugInClassID
#define plugInUniqueID		"21ws5nm87y015s4n0d65m1we54hy065m4c"

#define plugInCopyrightYear  "2015"
#define plugInDescription \
	"Powerfull gradients in real-time."

#define keyDisposition 		'disP'
#define keyIgnoreSelection	'ignS'
#define typeMood			'mooD'
#define dispositionClear	'moD0'
#define dispositionCool		'moD1'
#define dispositionHot		'moD2'
#define dispositionSick		'moD3'

#include "PIActions.h"

resource 'PiPL' ( 16000, "Dissolve", purgeable )
{
	{
		Kind { Filter },
		Name { "Launch Smart Gradients" },
		Category { "Smart Gradients Beta" },
		Version { (latestFilterVersion << 16 ) | latestFilterSubVersion },

		#ifdef __PIMac__
			#if (defined(__i386__))
				CodeMacIntel32 { "PluginMain" },
			#endif
			#if (defined(__ppc__))
				CodeMachOPowerPC { 0, 0, "PluginMain" },
			#endif
		#else
			#if defined(_WIN64)
				CodeWin64X86 { "PluginMain" },
			#else
			CodeWin32X86 { "PluginMain" },
			#endif
		#endif

		SupportedModes
		{
			noBitmap, noGrayScale,
			noIndexedColor, doesSupportRGBColor,
			noCMYKColor, noHSLColor,
			noHSBColor, noMultichannel,
			noDuotone, noLABColor
		},

		HasTerminology
		{
			plugInClassID,
			plugInEventID,
			16000,
			plugInUniqueID
		},
		
		EnableInfo { "in (PSHOP_ImageMode, RGBMode)"},

		PlugInMaxSize { 2000000, 2000000 },
		
		FilterLayerSupport {doesSupportFilterLayers},
		
		FilterCaseInfo
		{
			{
				/* Flat data, no selection */
				inWhiteMat, outStraightData,
				doNotWriteOutsideSelection,
				doesNotFilterLayerMasks, worksWithBlankData,
				doNotCopySourceToDestination,
					
				/* Flat data with selection */
				inWhiteMat, outWhiteMat,
				doNotWriteOutsideSelection,
				doesNotFilterLayerMasks, worksWithBlankData,
				doNotCopySourceToDestination,
				
				/* Floating selection */
				inWhiteMat, outStraightData,
				doNotWriteOutsideSelection,
				doesNotFilterLayerMasks, worksWithBlankData,
				doNotCopySourceToDestination,
					
				/* Editable transparency, no selection */
				inWhiteMat, outStraightData,
				doNotWriteOutsideSelection,
				doesNotFilterLayerMasks, worksWithBlankData,
				doNotCopySourceToDestination,
					
				/* Editable transparency, with selection */
				inWhiteMat, outWhiteMat,
				doNotWriteOutsideSelection,
				doesNotFilterLayerMasks, worksWithBlankData,
				doNotCopySourceToDestination,
					
				/* Preserved transparency, no selection */
				inWhiteMat, outStraightData,
				doNotWriteOutsideSelection,
				doesNotFilterLayerMasks, worksWithBlankData,
				doNotCopySourceToDestination,
					
				/* Preserved transparency, with selection */
				inWhiteMat, outStraightData,
				doNotWriteOutsideSelection,
				doesNotFilterLayerMasks, worksWithBlankData,
				doNotCopySourceToDestination
			}
		}	
	}
};

resource 'aete' (16000, "Smart Gradients dictionary", purgeable)
{
	1, 0, english, roman,									/* aete version and language specifiers */
	{
		vendorName,											/* vendor suite name */
		"Powerfull gradients in real-time.",				/* optional description */
		plugInSuiteID,										/* suite ID */
		1,													/* suite code, must be 1 */
		1,													/* suite level, must be 1 */
		{													/* structure for filters */
			plugInName,										/* unique filter name */
			plugInAETEComment,								/* optional description */
			plugInClassID,									/* class ID, must be unique or Suite ID */
			plugInEventID,									/* event ID, must be unique to class ID */
			
			NO_REPLY,										/* never a reply */
			IMAGE_DIRECT_PARAMETER,							/* direct parameter, used by Photoshop */
			{												/* parameters here, if any */
			}
		},
		{													/* non-filter plug-in class here */
		},
		{													/* comparison ops (not supported) */
		},
		{													/* any enumerations */
		}
	}
};
