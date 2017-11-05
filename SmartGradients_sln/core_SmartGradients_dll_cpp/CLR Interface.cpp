
#include "FilterBigDocument.h"
#include "PIUtilities.h"

#include "PlugInHostCommunication.h"
#include "SmartGradients.h"

#using <System.dll>
#using <PresentationCore.dll>
#using <PresentationFramework.dll>
#using <WindowsBase.dll>
#using <System.Xaml.dll>
#using <System.Windows.Forms.dll>
#using <WindowsFormsIntegration.dll>
#using <System.Drawing.dll>

#ifdef _M_IX86
#ifdef _DEBUG
#using <..\..\Output\Debug\x86_32\GradientControl.dll>
#else
#using <..\..\Output\Release\x86_32\GradientControl.dll>
#endif
#endif

#ifdef _M_X64
#ifdef _DEBUG
#using <..\..\Output\Debug\x86_64\GradientControl.dll>
#else
#using <..\..\Output\Release\x86_64\GradientControl.dll>
#endif
#endif

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Controls;
using namespace System::Windows::Interop;
using namespace System::Windows::Media;
using namespace System::Windows::Forms;
using namespace System::Windows::Forms::Integration;
using namespace System::Windows::Media::Imaging;
using namespace System::ComponentModel;
using namespace GradientControls;

void __cdecl msgBox(Double message)
{
    Forms::MessageBox::Show(message.ToString());
}

void __cdecl msgBox(char* message)
{
    Forms::MessageBox::Show(gcnew String(message));
}


#pragma region MAIN UI

Color LoadColorDialog(AnchorPixelControl^ apix)
{
    ColorServicesInfo service; CSInitInfo(&service);

    service.selector =    plugIncolorServicesChooseColor;
    service.sourceSpace = plugIncolorServicesRGBSpace;
    service.resultSpace = plugIncolorServicesRGBSpace;

    service.colorComponents[0] = apix->Color.R;
    service.colorComponents[1] = apix->Color.G;
    service.colorComponents[2] = apix->Color.B;

    gFilterRecord->colorServices(&service);

    return Color::FromArgb(255, 
        (uint8)service.colorComponents[0], 
        (uint8)service.colorComponents[1],
        (uint8)service.colorComponents[2]);
}



void CloneAnchorPixel(AnchorPixel* a, m_AnchorPixel^ am, int32 transH, int32 transV)
{
    a->X =     am->X - transH;
    a->Y =     am->Y - transV;
    a->CH[0] = am->CH0;
    a->CH[1] = am->CH1;
    a->CH[2] = am->CH2;
    a->CH[3] = am->CH3;
    a->CH[4] = am->CH4;

}

void CloneGradientState(GradientState* g, m_GradientState^ gm, bool isForProxy)
{
    int32 transH = 0;
    int32 transV = 0;

    if (!isForProxy)
    {
        transH = gFilterRecord->bigDocumentData->floatCoord32.h;
        transV = gFilterRecord->bigDocumentData->floatCoord32.v;
        if (transH > 0) transH = 0;
        if (transV > 0) transV = 0;
    }
    
    g->OpacityApplied =     gm->OpacityApplied;
    g->Type = (GradientType)gm->Type;
    g->Grad =               gm->Grad;
    g->ActiveAnchPixs =     gm->ActiveAnchPixs;
    g->Px =                 gm->Px;
    g->Py =                 gm->Py;
    g->DiffusionIndex =     gm->DiffusionIndex;

    if (gm->Parameter < 0 || gm->Parameter>400)
        g->Parameter = ParameterNotUsed;
    else
        g->Parameter = gm->Parameter;

    g->Angle =              gm->Angle;
    g->CentralPix =         gm->CentralPix;
    g->CenterX =            gm->CenterX - transH;
    g->CenterY =            gm->CenterY - transV;

    for (int32 i = 0; i <= g->ActiveAnchPixs - 1; i++)
        CloneAnchorPixel(&g->AnchPixs[i], gm->AnchPixs[i], transH, transV);
}

void CloneProxyState(ProxyState* p, m_ProxyState^ pm)
{
    p->ViewportLocation.h = pm->ViewportLocationH;
    p->ViewportLocation.v = pm->ViewportLocationV;
    p->ViewportSize.h =     pm->ViewportSizeH;
    p->ViewportSize.v =     pm->ViewportSizeV;
    p->ZoomFactor =         pm->ZoomFactor;
}



Form^ GetParentForm(Controls::UserControl^ mWpfCtrl)
{
    HwndSource^ wpfHandle = (HwndSource^)PresentationSource::FromVisual(mWpfCtrl);
    if (wpfHandle)
    {
        ElementHost^ host = (ElementHost^)Forms::Control::FromChildHandle(wpfHandle->Handle);
        return (Form^)host->Parent;
    }
    return nullptr;
}



void ContinuationRequestedHandler(GradinetControl^ sender, GradinetControl::GradientControlEventArgs^ e)
{
    CloneGradientState(&gParams->grState, sender->GradientState, false);
    sender->Result = 1;
    Form^ parentForm = GetParentForm(sender); if (parentForm) parentForm->Close();
}

void CancelationRequestedHandler(GradinetControl^ sender, GradinetControl::GradientControlEventArgs^ e)
{
    Form^ parentForm = GetParentForm(sender); if (parentForm) parentForm->Close();
}

void ColorDialogRequestedHandler(GradinetControl^ sender, GradinetControl::GradientControlEventArgs^ e)
{
    e->ReferenceAnchPixel->Color = LoadColorDialog(e->ReferenceAnchPixel);
}

void ProxyUpdateRequestedHandler(GradinetControl^ sender, GradinetControl::GradientControlEventArgs^ e)
{
    uint8* mergedBuffer   = nullptr;
    uint8* gradientBuffer = nullptr;
    Int32  stride = 0;
    int32  width  = 0;
    int32  height = 0;

    CloneProxyState     (&gParams->proxyState, sender->ProxyState);
    CloneGradientState  (&gParams->grState,    sender->GradientState, true);

    sender->LockProxyBuffers(mergedBuffer, gradientBuffer, stride);
    WriteProxy(&gParams->grState, &gParams->proxyState, e->ProxyStateChanged, (uint8*)gData->HelpBuffer, 4, stride, mergedBuffer, gradientBuffer, width, height);
    sender->UnlockProxyBuffers(width, height);
}



int32 __cdecl DoUI()
{
    int32 result = 0;

    Form^               dialogForm                  = nullptr;
    ElementHost^        gradientControlElementHost  = nullptr;
    GradinetControl^    gradientControl             = nullptr;

    bool opacityAllowed = true;
    if (gFilterRecord->absLayerPlanes == 0 && gFilterRecord->absTransparencyMask == 0) opacityAllowed = false;

    int imageHeight = gFilterRecord->documentInfo->bounds.bottom;
    int imageWidth  = gFilterRecord->documentInfo->bounds.right;

    try
    {
        dialogForm =                    gcnew Form();
        gradientControlElementHost =    gcnew ElementHost();
        gradientControl =               gcnew GradinetControl(imageWidth, imageHeight, opacityAllowed);
    }
    catch (Exception^)
    {
        return -1;
    }

    gradientControl->CancelationRequested  += gcnew GradinetControl::GradientControlDelegate(&CancelationRequestedHandler);
    gradientControl->ContinuationRequested += gcnew GradinetControl::GradientControlDelegate(&ContinuationRequestedHandler);
    gradientControl->ColorDialogRequested  += gcnew GradinetControl::GradientControlDelegate(&ColorDialogRequestedHandler);
    gradientControl->ProxyUpdateRequested  += gcnew GradinetControl::GradientControlDelegate(&ProxyUpdateRequestedHandler);

    gradientControlElementHost->Dock  = DockStyle::Fill;
    gradientControlElementHost->Child = gradientControl;

    dialogForm->Controls->Add(gradientControlElementHost);
    dialogForm->ShowInTaskbar = false;
    dialogForm->FormBorderStyle = FormBorderStyle::None;  
    dialogForm->Width  = (int32)SystemParameters::MaximizedPrimaryScreenWidth  - 40;
    dialogForm->Height = (int32)SystemParameters::MaximizedPrimaryScreenHeight - 40;
    dialogForm->StartPosition = FormStartPosition::CenterScreen;

    dialogForm->ShowDialog();

    result = gradientControl->Result;
    delete dialogForm;
    return result;
}

#pragma endregion
