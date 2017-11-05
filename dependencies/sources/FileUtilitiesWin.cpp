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
//		FileUtilitiesWin.cpp
//
//	Description:
//		This library contains sources and functions
//		to simplify the use of extremely common file
//		functions for plug-ins. Windows Only
//
//	Use:
//		Mac toolbox routines for the Windows API.
//
//-------------------------------------------------------------------------------

#include "FileUtilities.h"

OSErr PSSDKWrite(int32 refNum, int32 * count, void * buffPtr)
{
	if (NULL == count || NULL == buffPtr)
		return writErr;
		
	int32 bytes = (WORD)*count;
	
	if (!WriteFile((HANDLE)refNum, buffPtr, bytes, (unsigned long *)count, NULL)) 
		return writErr;
	
	if (bytes != *count)
		return writErr;

	return noErr;
}

OSErr PSSDKSetFPos(int32 refNum, short posMode, long posOff)
{
	long moved = SetFilePointer((HANDLE)refNum, posOff, NULL, posMode);

	if (moved != posOff)
		return writErr;
	
	return noErr;
}

OSErr PSSDKRead(int32 refNum, int32 * count, void * buffPtr)
{
	if (NULL == count || NULL == buffPtr)
		return readErr;

	int32 bytes = *count;

	if (!ReadFile((HANDLE)refNum, buffPtr, bytes, (unsigned long *)count, NULL))
		return readErr;

	if (bytes != *count)
		return readErr;

	return noErr;
}

/*****************************************************************************/

// end FileUtilitiesWin.cpp
