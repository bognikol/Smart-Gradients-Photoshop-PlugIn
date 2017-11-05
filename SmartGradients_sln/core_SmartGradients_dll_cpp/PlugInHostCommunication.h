#ifndef _PLUGINHOSTCOMMUNICATION_H
#define _PLUGINHOSTCOMMUNICATION_H

#include "PIDefines.h"
#include "PITypes.h"
#include "PIAbout.h"
#include "PIFilter.h"
#include "PIUtilities.h"
#include "PIExport.h"
#include "PIProperties.h"

#include "SmartGradients.h"

#define memoryCheckBufferSize 30000000
#define drawingSurfaceMaxSize 8000

typedef struct
{
	GradientState	grState;
	ProxyState		proxyState;
} 
Parameters;

typedef struct
{	
	BufferID				HelpBufferID;
	Ptr						HelpBuffer;
	PSChannelPortsSuite1*	sPSChannelPortsSuite;
	PSBufferSuite1*			sPSBufferSuite;
} 
Data;

extern FilterRecord* gFilterRecord;
extern int16*        gResult;
extern Data*         gData;
extern Parameters*   gParams;
extern SPBasicSuite* sSPBasic;

void DoParameters(void);
void DoPrepare(void);
void DoStart(void);
void DoContinue(void);
void DoFinish(void);

void LockHandles(void);
void UnlockHandles(void);

void CreateParametersHandle(void);
void InitParameters(void);

void CreateDataHandle(void);
void InitData(void);

void CreateBuffers(void);
void DeleteBuffers(void);

int16 NumberOfPlanesToProcess(void);

#endif