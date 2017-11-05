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
//		PIDLLInstance.cpp
//
//	Description:
//		Windows only file for centralized placed to get the instance
//		of this plugin.
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------
#include "PIDLLInstance.h"

namespace PIDLLInstance 
{
	HINSTANCE dllInstance = NULL;
}

using namespace PIDLLInstance;

HINSTANCE GetDLLInstance(SPPluginRef plugin)
{
	if (plugin == NULL) return dllInstance;

	SPAccessRef access = NULL;
	SPErr error = sSPAccess->GetPluginAccess(plugin, &access);
	
	if (error == 0 && access != NULL)
	{
		SPPlatformAccessInfo info;
		error = sSPAccess->GetAccessInfo(access, &info);
			
		if (error == 0)
			if (info.resourceAccess != NULL)
				dllInstance = reinterpret_cast<HINSTANCE>(info.resourceAccess);
			else
				dllInstance = reinterpret_cast<HINSTANCE>(info.defaultAccess);
		else
			dllInstance = NULL;
	}

	return dllInstance;
}

//-------------------------------------------------------------------------------
//
// DllMain
//
// Initialization and termination code for Windows DLLs
//
//-------------------------------------------------------------------------------


extern "C" BOOL APIENTRY DllMain(HANDLE hModule,
					             DWORD ul_reason_for_call,
					             LPVOID /*lpReserved*/);

BOOL APIENTRY DllMain(HANDLE hModule,
					  DWORD ul_reason_for_call,
					  LPVOID /*lpReserved*/)
{
	// very odd, when running SimpleFormat and selecting multiple images
	// in the open dialog I get dettach messages at about the 7th image ?????
	// the detach messages come with a valid hModule so no if ?????
	// if (ul_reason_for_call == DLL_PROCESS_ATTACH ||
	// 	   ul_reason_for_call == DLL_THREAD_ATTACH)
		dllInstance = static_cast<HINSTANCE>(hModule);
	// else
	// dllInstance = NULL;
	return  true;
}
// end PIDLLInstance.cpp