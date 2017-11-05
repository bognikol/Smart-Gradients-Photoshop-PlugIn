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
//		PIMacUI.cpp
//
//	Description:
//		Macintosh only code for the UI elements
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------
#include "PIUI.h"

#if !__LP64__

// Include issues on the Mac this is taken from ConditionalHeaders.h
#if !OPAQUE_TOOLBOX_STRUCTS
struct PopupPrivateData {
  MenuRef             mHandle;
  SInt16              mID;
};
typedef struct PopupPrivateData         PopupPrivateData;
typedef PopupPrivateData *              PopupPrivateDataPtr;
typedef PopupPrivateDataPtr *           PopupPrivateDataHandle;
#endif  /* !OPAQUE_TOOLBOX_STRUCTS */

int PIDialog::Modal(SPPluginRef pluginRef, char *, int dialogID)
{
	short item = -1;

	ProcessEventProc processEvent = sPSUIHooks->processEvent;

	ResContext context(pluginRef);

	DialogTHndl dialogTemplate = (DialogTHndl) GetResource ('DLOG', dialogID);
	if (dialogTemplate == NULL) return item;

	HNoPurge ((Handle) dialogTemplate);
	
	CenterDialog (dialogTemplate);
	
	DialogPtr dialog = GetNewDialog( dialogID, nil, (WindowPtr)-1L );
	if (dialog == NULL) return item;

	(void) SetDialogDefaultItem(dialog, 1);
	(void) SetDialogCancelItem(dialog, 2);
	(void) SetDialogTracksCursor(dialog, TRUE);
	
	SetDialog(dialog);
	
	Init();

	Boolean doneWithDialog = false;
	
	while (doneWithDialog == false)
	{
		MoveableModalDialog(dialog,
						    processEvent,
						    nil,
						    &item);
		Notify(item);
		if (item == 1 || item == 2)
			doneWithDialog = true;
	}
	
	DisposeDialog(dialog);
	HPurge ((Handle) dialogTemplate);
	
	return item;

}

PIItem PIDialog::GetItem(int32 index)
{
	return PIGetDialogItem(dialog, index);
}
	
void PIComboBox::Clear(void)
{
	PIMenu menuItem = PIGetMenuHandleFromItemHandle(item);
	short menuCount = CountMenuItems(menuItem);
	
	while (menuCount > 0)
	{
		DeleteMenuItem(menuItem, menuCount);
		menuCount--;
	}
}

void PIComboBox::AppendItem(const string & in)
{
	PIMenu menuItem = PIGetMenuHandleFromItemHandle(item);
	Str255 pString;
	CToPStringCopy(pString, in);
	short menuCount = CountMenuItems(menuItem);
	InsertMenuItem(menuItem, "\p ", menuCount);
	menuCount++;
	SetMenuItemText(menuItem, menuCount, pString);	
	short controlCount = GetControlMaximum((ControlRef)item);
	if (menuCount != controlCount)
		SetControlMaximum((ControlRef)item, menuCount);
}

void PIComboBox::SetCurrentSelection(int16 index)
{
	SetControlValue((ControlRef)item, index);
}

void PIComboBox::GetCurrentSelection(string & out)
{
	int16 menuIndex = GetControlValue((ControlRef)item);
	PIMenu menuItem = PIGetMenuHandleFromItemHandle(item);
	Str255 pString = "\p";
	GetMenuItemText(menuItem, menuIndex, pString);
	PToCStringCopy(out, pString);
}

void PIText::SetText(const string & in)
{
	Str255 pString;
	CToPStringCopy(pString, in);
	SetDialogItemText(item, pString);
}

void PIText::GetText(string & out)
{
	Str255 pString;
	GetDialogItemText(item, pString);
	PToCStringCopy(out, pString);
}

void PIRadioGroup::SetGroupRange(int32 first, int32 last)
{
	firstItem = first;
	lastItem = last;
}

void PIRadioGroup::SetSelected(int32 s)
{
	if (firstItem == -1 || lastItem == -1 || dialog == NULL) return;
	if (s > lastItem || s < firstItem) return;
	SetRadioGroupState(dialog, firstItem, lastItem, s);
}

int32 PIRadioGroup::GetSelected(void)
{	
	return GetRadioGroupState(dialog, firstItem, lastItem);
}

PIListBox::PIListBox() : item(NULL), combo(NULL) {}

PIListBox::PIListBox(PIItem in) : item(in), combo(in) {}

PIListBox::~PIListBox() {}

int32 PIListBox::Init(void)
{
	combo.SetItem(item);
	return 0;
}

void PIListBox::Clear()
{
	Init();
	combo.Clear();
}
	
void PIListBox::GetCurrentSelectionText(string & out)
{
	Init();
 	combo.GetCurrentSelection(out);
}

int32 PIListBox::GetCurrentSelectionIndex(void)
{
	Init();
	return GetControlValue((ControlRef)combo.GetItem());
}

int32 PIListBox::GetUserData(int32 index)
{
	Init();
	return index + 1;
}

void PIListBox::SetUserData(int32 index, int32 data)
{
	Init();
	data = index;
}

int32 PIListBox::AppendItem(const string & in)
{
	Init();
	combo.AppendItem(in);
	return 0;
}

void PIListBox::SetCurrentSelection(int32 index)
{
	Init();
	combo.SetCurrentSelection(index);
}

void PICheckBox::SetText(const string & in)
{
	Str255 pString;
	CToPStringCopy(pString, in);
	SetDialogItemText(item, pString);
}

void PICheckBox::SetChecked(bool checked)
{
	SetControlValue((ControlRef)item, checked);
}

bool PICheckBox::GetChecked(void)
{
	return GetControlValue((ControlRef)item) != 0;
}

PIItem PIGetDialogItem(PIDialogPtr dialog, int16 itemNumber)
{
	Rect r;
	short itemType;		
	PIItem h = NULL;
	GetDialogItem(dialog, itemNumber, &itemType, &h, &r);
	return (h);
}

PIMenu PIGetMenuHandleFromItemHandle(PIItem itemHandle)
{
	PIMenu menuHandle = NULL;
	if (itemHandle == NULL)
	{
		DebugStr("\pitemHandle is NULL");
		return menuHandle;
	}

	menuHandle = GetControlPopupMenuHandle((ControlRef)itemHandle);
	
	return (menuHandle);		
}

void CToPStringCopy(StringPtr dst, const string & src)
{
	dst[0] = src.length();
	short index = 0;
	while (index < dst[0])
	{
		dst[index+1] = src[index];
		index++;
	}
}

void PToCStringCopy(string & dst, const StringPtr src)
{
	dst.clear();
	short length = src[0];
	short index = 0;
	while (index < length)
	{
		dst.push_back(src[index+1]);
		index++;
	}
}

ResContext::ResContext(SPPluginRef pluginRef) : oldContext(0)
{	
	SPPlatformAccessRef resources = NULL;
	oldContext = CurResFile();
	SPAccessRef access = NULL;

	SPErr error = sSPAccess->GetPluginAccess(pluginRef, &access);
	if (!error && access != NULL)
	{
		SPPlatformAccessInfo info;
		error = sSPAccess->GetAccessInfo(access, &info);
		
		if (error == 0)
			if (info.resourceAccess != NULL)
				resources = info.resourceAccess;
			else
				resources = info.defaultAccess;
	}
	
	if (resources != NULL)
		UseResFile(resources->fileRefNum);
}

ResContext::~ResContext()
{	
	if (oldContext)
		UseResFile(oldContext);	
	sSPAccess.Unload();
}

void DoAbout(SPPluginRef plugin, int dialogID)
{
	short 			item;	// variable to receive item pressed (ignored)
	DialogPtr 		dp;		// dialog pointer for newly created dialog
	DialogTHndl 	dt;		// dialog handle for dialog resource
	StringHandle	h;		// string resource for about text
	
	ResContext context(plugin);

	h = GetString(dialogID);
	
	// Now go get the dialog information:
	dt = (DialogTHndl) GetResource ('DLOG', dialogID);

	HNoPurge ((Handle) dt); // mark it unpurgeable

	CenterDialog (dt); // center fully on the screen

	dp = GetNewDialog (dialogID, nil, (WindowPtr) -1); // instantiate the dialog
	
	// Standard routine to replace ^0, ^1, ^2, ^3 with text:
	ParamText (*h, NULL, NULL, NULL);
	
	(void) SetDialogDefaultItem (dp, ok); // hidden
	(void) SetDialogCancelItem (dp, ok); // either
	
	ProcessEventProc processEvent = sPSUIHooks->processEvent;

	// This routine handles the moveable modal and returns an item hit:
	do 
	{
		MoveableModalDialog (dp, processEvent, NULL, &item);
		if (item == 4 || item == 5)
		{
			const char * message;// = "";
			if (item == 4)
				message = "http://www.adobe.com";
			else
				message = "http://partners.adobe.com";
			try 
			{
				sPSFileList->BrowseUrl(message);
			}
			catch(...)
			{
				DebugStr("\pthis is bad.");
			}
		}
	}
	while (item == 4 || item == 5);
		
	DisposeDialog (dp); // closes and disposes the dialog

	ReleaseResource((Handle)h); // Releases the text resource

	HPurge ((Handle) dt); // mark the dialog info as purgeable

	ReleaseResource((Handle) dt); // releases the dialog info resource
}

#else // !__LP64__

void DoAbout(SPPluginRef plugin, int dialogID)
{
}

#endif // #if !__LP64__

// end PIMacUI.cpp
