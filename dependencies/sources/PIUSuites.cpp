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
//-------------------------------------------------------------------------------
//
//	File:
//		PIUSuites.cpp
//
//	Description:
//		Utility routines to acquire and release suites for your plug in
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------
#include "PIUSuites.h"

// global suite pointers
AutoSuite<PSActionDescriptorProcs> sPSActionDescriptor(kPSActionDescriptorSuite, kPSActionDescriptorSuiteVersion);
AutoSuite<PSActionControlProcs> sPSActionControl(kPSActionControlSuite, kPSActionControlSuiteVersion);
AutoSuite<PSBasicActionControlProcs> sPSBasicActionControl(kPSBasicActionControlSuite, kPSBasicActionControlSuiteVersion);
AutoSuite<PSActionReferenceProcs> sPSActionReference(kPSActionReferenceSuite, kPSActionReferenceSuiteVersion);
AutoSuite<PSActionListProcs> sPSActionList(kPSActionListSuite, kPSActionListSuiteVersion);
AutoSuite<PSDescriptorRegistryProcs> sPSRegistry(kPSDescriptorRegistrySuite, kPSDescriptorRegistrySuiteVersion);
AutoSuite<PSUIHooksSuite1> sPSUIHooks(kPSUIHooksSuite, kPSUIHooksSuiteVersion1);
AutoSuite<PSHandleSuite2> sPSHandle(kPSHandleSuite, kPSHandleSuiteVersion2);
AutoSuite<ASZStringSuite1> sASZString(kASZStringSuite, kASZStringSuiteVersion1);
AutoSuite<SPAccessSuite> sSPAccess(kSPAccessSuite, kSPAccessSuiteVersion);
AutoSuite<PSGetFileListSuite4> sPSFileList(kPSGetFileListSuite, kPSGetFileListSuiteVersion4);
AutoSuite<PSBufferSuite1> sPSBuffer(kPSBufferSuite, kPSBufferSuiteVersion1);
AutoSuite<PSChannelPortsSuite1> sPSChannelProcs(kPSChannelPortsSuite, kPSChannelPortsSuiteVersion3);
AutoSuite<SPRuntimeSuite> sSPRuntime(kSPRuntimeSuite, kSPRuntimeSuiteVersion);
AutoSuite<PropertyProcs> sPSProperty(kPIPropertySuite, kPIPropertySuiteVersion);
AutoSuite<PSAliasSuite> sPSAlias(kPSAliasSuite, kPSAliasSuiteVersion1);
AutoSuite<ResourceProcs> sPSResource(kPIResourceSuite, kPIResourceSuiteVersion);
AutoSuite<JSScriptingSuite2> sScripting(kJSScriptingSuite, kJSScriptingSuiteVersion);


void PIUSuitesRelease(void)
{
	sPSActionDescriptor.Unload();
	sPSActionControl.Unload();
	sPSBasicActionControl.Unload();
	sPSActionReference.Unload();
	sPSActionList.Unload();
	sPSRegistry.Unload();
	sPSHandle.Unload();
	sPSUIHooks.Unload();
	sASZString.Unload();
	sSPAccess.Unload();
	sPSFileList.Unload();
	sPSBuffer.Unload();
	sPSChannelProcs.Unload();
	sSPRuntime.Unload();
	sPSProperty.Unload();
	sPSAlias.Unload();
	sPSResource.Unload();
	sScripting.Unload();
}
// end PIUSuites.cpp