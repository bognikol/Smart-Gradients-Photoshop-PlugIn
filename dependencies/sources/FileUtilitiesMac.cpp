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
//		FileUtilitiesMac.cpp
//
//	Description:
//		This library contains sources and functions
//		to simplify the use of extremely common file
//		functions for plug-ins. 
//
//	Use:
//		Macintosh routines with common names. 
//
//-------------------------------------------------------------------------------

#include "PIDefines.h"
#include "FileUtilities.h"

#if !__LP64__

Boolean PIOpenFile (FSSpec& fsSpec, FileHandle *fRefNum, short *result)
{
	*fRefNum = 0;
	
	if (!TestAndStoreResult (result, FSpOpenDF (&fsSpec, fsCurPerm, fRefNum)))
	{
		FSpDelete (&fsSpec);
		return FALSE;
	}
	else
	{
		return TRUE;
	}	
}

/*****************************************************************************/

Boolean PICloseFileAndMakeAlias (FSSpec& fsSpec,
					 FileHandle fRefNum, Boolean sameNames,
					 Boolean *dirty, AliasHandle *alias, short *result)
{
//	FSSpec			spec;
	OSErr			gotErr;
	
	// Close the file if it is open.
	if (fRefNum != 0)
		(void) TestAndStoreResult (result, FSClose (fRefNum));
		
	// Delete the file if we had an error.	
	 if (*result != noErr)
		 (void) FSpDelete (&fsSpec);
			
	// Flush the volume anyway.
	if (!TestAndStoreResult (result, FlushVol (NULL, fsSpec.vRefNum))) return FALSE;
	
	// Mark the file as clean.
	*dirty = FALSE;
	
	/* create alias for scripting system */
	
	if (sameNames)
	{ // didn't enter new filename, so set filename to nothing
		fsSpec.name[ (fsSpec.name[0] = 0)+1 ] = 0;
	} // otherwise use filename and store entire reference

	gotErr = NewAlias(nil, &fsSpec, alias);

	return (gotErr == noErr);
}

/*****************************************************************************/

#else //  !__LP64__

/*****************************************************************************/

Boolean PIOpenFile (FSSpec& fsSpec, FileHandle *fRefNum, short *result)
{
	return FALSE;
}

/*****************************************************************************/

Boolean PICloseFileAndMakeAlias (FSSpec& fsSpec,
					 FileHandle fRefNum, Boolean sameNames,
					 Boolean *dirty, AliasHandle *alias, short *result)
{
	return FALSE;
}

/*****************************************************************************/

#endif // !__LP64__

/*****************************************************************************/

OSErr PSSDKWrite(int32 refNum, int32 * count, void * buffPtr)
{
	if (NULL == count || NULL == buffPtr)
		return writErr;
		
	ByteCount bytes = *count;

	ByteCount bCount = *count;
	
	OSErr err = FSWriteFork(refNum, fsAtMark | noCacheMask, 0, bytes, buffPtr, &bCount);

	if (noErr != err)
		return err;
	
	if (bytes != bCount)
		return writErr;
		
	*count = bCount;

	return err;
}

/*****************************************************************************/

OSErr PSSDKSetFPos(int32 refNum, short posMode, long posOff)
{
	OSErr err = FSSetForkPosition(refNum, posMode, posOff);
	return err;
}

/*****************************************************************************/

OSErr PSSDKRead(int32 refNum, int32 * count, void * buffPtr)
{
	if (NULL == count || NULL == buffPtr)
		return writErr;

	ByteCount bytes = *count;

	ByteCount bCount = *count;

	OSErr err = FSReadFork(refNum, fsAtMark, 0, bytes, buffPtr, &bCount);

	if (bytes != bCount)
		return readErr;
		
	*count = bCount;

	return err;
}

// end FileUtilitiesMac.cpp
