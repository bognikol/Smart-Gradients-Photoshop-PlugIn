#include "FilterBigDocument.h"

#include "PlugInHostCommunication.h"
#include "SmartGradients.h"

FilterRecord* gFilterRecord = nullptr;
intptr_t*     gDataHandle   = nullptr;
int16*        gResult       = nullptr;
SPBasicSuite* sSPBasic      = nullptr;

Data*         gData         = nullptr;
Parameters*   gParams       = nullptr;

int32 __cdecl DoUI();
void  __cdecl msgBox(double message);
void  __cdecl msgBox(char*  message);

DLLExport MACPASCAL void PluginMain
    (
    const int16     selector,
    FilterRecordPtr filterRecord,
    intptr_t*       data,
    int16*          result
    )
{
    gFilterRecord = filterRecord;
    gDataHandle = data;
    gResult = result;

    if (selector == filterSelectorAbout)
    {
        sSPBasic = ((AboutRecord*)gFilterRecord)->sSPBasic;
    }
    else
    {
        sSPBasic = gFilterRecord->sSPBasic;

        if (gFilterRecord->bigDocumentData)
            gFilterRecord->bigDocumentData->PluginUsing32BitCoordinates = true;
    }

    switch (selector)
    {
    case filterSelectorAbout:
        msgBox(3);
        break;
    case filterSelectorParameters:
        DoParameters();
        break;
    case filterSelectorPrepare:
        DoPrepare();
        break;
    case filterSelectorStart:
        DoStart();
        break;
    case filterSelectorContinue:
        DoContinue();
        break;
    case filterSelectorFinish:
        DoFinish();
        break;
    default:
        break;
    }

    if (selector != filterSelectorAbout) UnlockHandles();
}

void DoParameters(void)
{
    if (gFilterRecord->parameters == nullptr) CreateParametersHandle();
    if ((*gDataHandle) == 0) CreateDataHandle();
    if (*gResult == noErr)
    {
        LockHandles();
        InitParameters();
        InitData();
    }
}

void DoPrepare(void)
{
    if (gFilterRecord->parameters == nullptr || (*gDataHandle) == 0)
    {
        if (gFilterRecord->parameters == nullptr) CreateParametersHandle();
        if ((*gDataHandle) == 0) CreateDataHandle();
        if (*gResult == noErr)
        {
            LockHandles();
            InitParameters();
            InitData();
        }
    }

    gFilterRecord->autoMask = true;
    gFilterRecord->bufferSpace = 2 * memoryCheckBufferSize;
}



void DoStart(void)
{
    LockHandles();

    CreateBuffers();

    int32 res = 0;

    BufferID memoryCheckBuffer, memoryCheckResult;
    gFilterRecord->bufferProcs->allocateProc(memoryCheckBufferSize, &memoryCheckBuffer);
    memoryCheckResult = memoryCheckBuffer;
    gFilterRecord->bufferProcs->freeProc(memoryCheckBuffer);
      
    if (memoryCheckResult)
    {
        if (gFilterRecord->bigDocumentData->wholeSize32.v > drawingSurfaceMaxSize || gFilterRecord->bigDocumentData->wholeSize32.h > drawingSurfaceMaxSize)
        {
            msgBox("Document is too large. Size of document must not exceed 8000 x 8000 px.");
            return;
        }
        if (gFilterRecord->bigDocumentData->filterRect32.bottom - gFilterRecord->bigDocumentData->filterRect32.top > drawingSurfaceMaxSize || gFilterRecord->bigDocumentData->filterRect32.right - gFilterRecord->bigDocumentData->filterRect32.left > drawingSurfaceMaxSize)
        {
            msgBox("Selected area is too large. Size of selected area must not exceed 8000 x 8000 px. If you have nothing selected, check if the size of active layer is appropriate.");
            return;
        }

        res = DoUI();
    } 
    else
    {
        res = -1;
    }

    if (res == -1)
    {
        *gResult = memFullErr;
        return;
    }
    else if (res == 0)
    {
        *gResult = userCanceledErr;
        return;
    }
    else if (res == 1)
    {
        if (ValidateGradientState(&(gParams->grState)) == GradientStateNoErr)
        {

            gFilterRecord->wantsAbsolute = true;
            SetOutRect(GetFilterRect());

            for (int16 plane = 0; plane < NumberOfPlanesToProcess(); plane++)
            {
                gFilterRecord->outLoPlane = plane;
                gFilterRecord->outHiPlane = plane;
                *gResult = gFilterRecord->advanceState();
                if (*gResult != noErr) return;
                DrawGradient(&(gParams->grState), GetOutRect(), gFilterRecord->outRowBytes, plane, (uint8*)gData->HelpBuffer, (uint8*)gFilterRecord->outData);
            }
        }
        else
        {
            *gResult = (int16)kBadParameterErr;
        }
    }

    DeleteBuffers();
}


void DoContinue(void)
{
    LockHandles();

    VRect zeroRect = { 0, 0, 0, 0 };
    SetInRect  (zeroRect);
    SetOutRect (zeroRect);
    SetMaskRect(zeroRect);
}

void DoFinish(void)
{
}



void LockHandles(void)
{
    if (gFilterRecord->parameters == nullptr || *gDataHandle == 0)
    {
        *gResult = filterBadParameters; return;
    }
    gParams = (Parameters*)gFilterRecord->handleProcs->lockProc(gFilterRecord->parameters, TRUE);
    gData = (Data*)gFilterRecord->handleProcs->lockProc((Handle)*gDataHandle, TRUE);
    if (gParams == nullptr || gData == nullptr)
    {
        *gResult = memFullErr;
        return;
    }
}

void UnlockHandles(void)
{
    if ((*gDataHandle) != 0) gFilterRecord->handleProcs->unlockProc((Handle)*gDataHandle);
    if (gFilterRecord->parameters != nullptr) gFilterRecord->handleProcs->unlockProc(gFilterRecord->parameters);
}

void CreateParametersHandle(void)
{
    gFilterRecord->parameters = gFilterRecord->handleProcs->newProc(sizeof(Parameters));
    if (gFilterRecord->parameters == nullptr) *gResult = memFullErr;
}

void InitParameters(void)
{
    gParams->grState.Type = COMPLEX;
    gParams->grState.ActiveAnchPixs = 3;
    gParams->grState.Px = 1;
    gParams->grState.Py = 1;
    gParams->grState.Angle = 1;
    gParams->grState.DiffusionIndex = DiffusionIndexNotUsed;
    gParams->grState.Parameter = ParameterNotUsed;
    gParams->grState.CentralPix = 0;
    gParams->grState.CenterX = 250;
    gParams->grState.CenterY = 250;
}

void CreateDataHandle(void)
{
    Handle h = gFilterRecord->handleProcs->newProc(sizeof(Data));
    if (h != nullptr) *gDataHandle = (intptr_t)h;
    else *gResult = memFullErr;
}

void InitData(void)
{
    gData->HelpBufferID = nullptr;
    gData->HelpBuffer = nullptr;
    gFilterRecord->sSPBasic->AcquireSuite(kPSChannelPortsSuite,
        kPSChannelPortsSuiteVersion3,
        (const void **)&gData->sPSChannelPortsSuite);
    gFilterRecord->sSPBasic->AcquireSuite(kPSBufferSuite,
        kPSBufferSuiteVersion1,
        (const void **)&gData->sPSBufferSuite);
}

void CreateBuffers(void)
{
    gFilterRecord->bufferProcs->allocateProc(HelperBufferSize, &gData->HelpBufferID);
    gData->HelpBuffer = gFilterRecord->bufferProcs->lockProc(gData->HelpBufferID, true);
    InitializeArcTanLookupTable();
}

void DeleteBuffers(void)
{
    gFilterRecord->bufferProcs->unlockProc(gData->HelpBufferID);
    gFilterRecord->bufferProcs->freeProc(gData->HelpBufferID);
    gData->HelpBufferID = nullptr;
    gData->HelpBuffer = nullptr;
    DestroyArcTanLookupTable();
}

int16 NumberOfPlanesToProcess()
{
    if (gFilterRecord->planes < 3)
    {
        return 0;
    }
    else
    {
        if (gFilterRecord->imageMode == plugInModeRGBColor)
        {
            if (gFilterRecord->absLayerPlanes == 0 && gFilterRecord->absTransparencyMask == 0) return 3;
            return 4;
        }
        else if (gFilterRecord->imageMode == plugInModeCMYKColor)
        {
            if (gFilterRecord->absLayerPlanes == 0 && gFilterRecord->absTransparencyMask == 0) return 4;
            return 5;
        }
    }
    return 0;
}

