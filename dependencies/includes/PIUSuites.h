// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2002 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this 
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.

#ifndef __PIUSuites_H__		// Has this not been defined yet?
#define __PIUSuites_H__		// Only include this once by predefining it

#include "PIDefines.h"
#include "PITypes.h"
#include "PIGeneral.h"
#include "SPBasic.h"
#include <string.h>
#include "PIActions.h"
#include "PIHandleSuite.h"
#include "PIUIHooksSuite.h"
#include "ASZStringSuite.h"
#include "SPAccess.h"
#include "SPRuntme.h"
#include "PIGetFileListSuite.h"
#include "PIBufferSuite.h"
#include "PIChannelPortsSuite.h"
#include "JSScriptingSuite.h"

#if __PIWin__
	#ifndef AliasHandle
		typedef Handle					AliasHandle;
	#endif
#endif

#include "PIAliasSuite.h"

template<class T> class AutoSuite;

extern SPBasicSuite * sSPBasic;

extern AutoSuite<PSActionDescriptorProcs> sPSActionDescriptor;
extern AutoSuite<PSActionControlProcs> sPSActionControl;
extern AutoSuite<PSBasicActionControlProcs> sPSBasicActionControl;
extern AutoSuite<PSActionReferenceProcs> sPSActionReference;
extern AutoSuite<PSActionListProcs> sPSActionList;
extern AutoSuite<PSDescriptorRegistryProcs> sPSRegistry;
extern AutoSuite<PSHandleSuite2> sPSHandle;
extern AutoSuite<PSUIHooksSuite1> sPSUIHooks;
extern AutoSuite<ASZStringSuite1> sASZString;
extern AutoSuite<SPAccessSuite> sSPAccess;
extern AutoSuite<PSGetFileListSuite4> sPSFileList;
extern AutoSuite<PSBufferSuite1> sPSBuffer;
extern AutoSuite<PSChannelPortsSuite1> sPSChannelProcs;
extern AutoSuite<SPRuntimeSuite> sSPRuntime;
extern AutoSuite<PropertyProcs> sPSProperty;
extern AutoSuite<PSAliasSuite> sPSAlias;
extern AutoSuite<ResourceProcs> sPSResource;
extern AutoSuite<JSScriptingSuite2> sScripting;

// A better template for acquiring and release suites.
// Depends on a globally defined sSPBasic pointer.
// Illustrator example
// AutoSuite<AITextPathSuite> sAITextPath(kAITextPathSuite, kAITextPathSuiteVersion);
// AutoSuite will throw on bad access
// if you were using MySuite switch to AutoSuite
template<class T> class AutoSuite
{
private:
	T * suite;
	const long suiteVersion;
	const char * suiteName;
	SPErr error;
	// T& operator *(); // don't write this but maybe this would be it{ return  *suite; }

	// make sure the compiler doesn't create this
	AutoSuite();
	
	void AcquireSuite(void)
	{		
		if (sSPBasic != NULL)
		{
			error = sSPBasic->AcquireSuite(suiteName, 
				                           suiteVersion, 
				 						   (const void**)&suite);
			if (error) throw((OSType)error);
		}
		else
		{
			throw((OSType)kSPBadParameterError);
		}
	}

public:
	AutoSuite(const char * name, const long version) : 
	  suite(NULL), suiteName(name), suiteVersion(version), error(0)
	{ }

	~AutoSuite()
	{
		Unload();
	}

	const T * operator->()
	{ 
		if (suite == NULL)
			AcquireSuite();
		return suite; 
	}

	const T * operator *()
	{
		if (suite == NULL)
			AcquireSuite();
		return suite;
	}

	// use this to switch back and forth from AutoSuite to old way
	const bool operator==(const int rhs) 
	{
		rhs;
		if (suite == NULL)
			AcquireSuite();
		return suite == NULL;
	}
	
	// use this to switch back and forth from AutoSuite to old way
	const bool operator!=(const int rhs) 
	{
		rhs;
		if (suite == NULL)
			AcquireSuite();
		return suite != NULL;
	}
	
	void Unload(void)
	{
		if (suite != NULL && suiteName != NULL && sSPBasic != NULL)
		{
			sSPBasic->ReleaseSuite(suiteName, suiteVersion);
			suite = NULL;
		}
	}
	
	bool IsNull(){ return (suite == NULL); }
	SPErr GetError() { return error; }
};

// Instead of those nasty goto statements use these classes to auto Free action
// descriptors, references, and lists
// These require that you have global sPSActionDescriptor, sPSActionReference, and
// sPSActionList
class Auto_Desc {
public:
	Auto_Desc(bool make = true) : desc(0), owns(true) 
	{
		if (desc == NULL && make)
			if (sPSActionDescriptor->Make(&desc))
				throw;
	}
	~Auto_Desc()
	{
		if (desc != NULL && owns)
			sPSActionDescriptor->Free(desc);
	}

	PIActionDescriptor get() const
	{
		return (desc); 
	}

	PIActionDescriptor * operator & ()
	{
		return (&desc);
	}
	
	PIActionDescriptor release()
	{
		this->owns = false;
		return (desc); 
	}

private:
	bool owns;
	PIActionDescriptor desc;
};



class Auto_Ref {
public:
	Auto_Ref(bool make = true) : ref(0), owns(true) 
	{
		if (ref == NULL && make)
			if (sPSActionReference->Make(&ref))
				throw;
	}
	~Auto_Ref()
	{
		if (ref != NULL && owns)
			sPSActionReference->Free(ref);
	}

	PIActionReference get() const
	{
		return (ref); 
	}

	PIActionReference * operator & ()
	{
		return (&ref);
	}
	
	PIActionReference release()
	{
		this->owns = false;
		return (ref); 
	}

private:
	bool owns;
	PIActionReference ref;
};


class Auto_List {
public:
	Auto_List(bool make = true) : list(0), owns(true) 
	{
		if (list == NULL && make)
			if (sPSActionList->Make(&list))
				throw;
	}
	~Auto_List()
	{
		if (list != NULL && owns)
			sPSActionList->Free(list);
	}

	PIActionList get() const
	{
		return (list); 
	}

	PIActionList * operator & ()
	{
		return (&list);
	}
	
	PIActionList release()
	{
		this->owns = false;
		return (list); 
	}

private:
	bool owns;
	PIActionList list;
};

void PIUSuitesRelease(void);

#endif
// end PIUSuites.h