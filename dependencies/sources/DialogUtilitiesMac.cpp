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
//		DialogUtilities.h
//
//
//	Description:
//		This library contains the sources and routines
//		related to Macintosh dialog manipulation. 
//
//	Use:
//		DialogUtilities is intended to make Macintosh dialog
//		manipulation a little simpler by providing a standard
//		suite of functions.  Use these routines for whatever you
//		need, and feel free to tweak them for your own work.  They
//		expect A4-globals to be set-up already, and for gStuff to
//		be a valid pointer to your global structure's parameter
//		block.
//
//-------------------------------------------------------------------------------

#include "DialogUtilities.h"

#if !__LP64__

//-------------------------------------------------------------------------------
//
//	ShowVersionAlert
//	
//	Pops a cross-platform alert box letting the user know that they're
//	running under a host that does not support a function the plug-in
//	needs.
//
//	Inputs:
//		DialogPtr dp			Dialog resource to use for alert.  Leave NULL.
//
//		const short alertID		Resource ID of 'ALRT' resource.
//
//		const short stringID	Resource ID of 'STR ' resource with text to
//								replace "^0" in 'ALRT' resource.
//
//		Str255 versText1		Pascal text to replace "^1" from 'STR ' resource.
//
//		Str255 versText2		Pascal text to replace "^2" from 'STR ' resource.
//
//	Outputs:
//		Displays alert ID.
//
//		returns short result	Whether ID was shown successfully (0 = no error)
//
//-------------------------------------------------------------------------------

short ShowVersionAlert (DialogPtr dp,
						const short alertID, 
						const short stringID,
						Str255 versText1,
						Str255 versText2)
{	
	#ifdef __MWERKS__
		#pragma unused (dp)
	#endif // I know we're not using these in the Mac version. Don't warn.
	
	short 			result = 0;			// start with no error.
	Handle			alertHandle = NULL;	// handle for alert resource
	Str255			ds = "";			// pascal string for text
	
	// we'll load the initial text resource into this string handle (Macintosh):
	StringHandle	h = 0;
	
	if (alertID)
	{ // if alertID isn't even valid, don't even bother doing this!
	
		if (stringID > 0)
		{ // we have a string to display.  Go get it.
		
			// Use Inside Macintosh library call GetString to automatically load
			// string resource stringID into a StringHandle:
			h = GetString(stringID);
			
			if (h != NULL)
			{ // successfully loaded string.  Use the PIUtilities routine to
			  // copy it to our Pascal string:
				AppendString(ds, *h, 1, *h[0]);
				
				ReleaseResource((Handle)h); // release this resource now that we're done
				h = NULL; // just to be safe
			} // h was NULL
		} // stringID was < 1
		
		// Load the resource from the currently open resource fork.  Should be
		// our plug-ins resource fork.  If more than one plug-in is open, you
		// might want to use Get1Resource instead.  See Inside Macintosh.
		alertHandle = GetResource('ALRT', alertID);
		
		HNoPurge (alertHandle); // mark as unpurgeable until we're done with it
	
		// DialogUtilities has a routine that will center this for us:
		CenterDialog ((DialogTHndl) alertHandle);
	
		// PIParamText from PIUtilities will replace any occurrences of
		// ^0, ^1, or ^2 in a string (ds) with the text passed in:
		PIParamText(ds, NULL, versText1, versText2);
		
		// This is the Macintosh call to replace the ^0 we've put in our
		// alert box with the string we've provided from the 'STR ' resource:
		ParamText (ds, NULL, NULL, NULL);
		
		// Now display a CautionAlert and return the result (0 = no error).
		result = CautionAlert (alertID, nil);
	
		// Mark the alertHandle as purgeable at next garbage collection:
		HPurge (alertHandle);
		
		ReleaseResource (alertHandle); // deallocates and sets to NULL
		
	} // alert ID wasn't even valid!

	return result;
	
} // end ShowVersionAlert

//-------------------------------------------------------------------------------
//
//	ShowAlertType
//
//	Pops a cross-platform alert box using strings with
//	minimum and maximum values, alerting a user to what
//	values are valid.
//
//	Inputs:
//		DialogPtr dp			Dialog resource to use for alert.  Leave NULL.
//
//		const short alertID		Resource ID of 'ALRT' resource.
//
//		const short stringID	Resource ID of 'STR ' resource with text to
//								replace "^0" in 'ALRT' resource.
//
//		Str255 minText			Pascal text to replace "^1" from 'STR ' resource.
//
//		Str255 maxText			Pascal text to replace "^2" from 'STR ' resource.
//
//		const short alertType	Either PIAlertStop or PIAlertCaution.
//
//	Outputs:
//		Displays alert ID of type Stop or Caution.
//
//		short result			Whether ID was shown successfully (0 = no error)
//
//-------------------------------------------------------------------------------

short ShowAlertType (DialogPtr dp,
				     short alertID, 
				     short stringID, 
				     Str255 minText, 
				     Str255 maxText,
				     short alertType)
{
	#ifdef __MWERKS__
		#pragma unused (dp)
	#endif // I know we're not using these in the Mac version. Don't warn.
	
	short 			result = 0;			// start with no error.
	Handle			alertHandle = NULL;	// handle for alert resource
	Str255			ds = "";			// pascal string for text
	
	// we'll load the initial text resource into this string handle (Macintosh):
	StringHandle	h = 0;
	
	if (alertID)
	{ // if alertID isn't even valid, don't even bother doing this!
	
		if (stringID > 0)
		{ // we have a string to display.  Go get it.
		
			// Use Inside Macintosh library call GetString to automatically load
			// string resource stringID into a StringHandle:
			h = GetString(stringID);
			
			if (h != NULL)
			{ // successfully loaded string.  Use the PIUtilities routine to
			  // copy it to our Pascal string:
				AppendString(ds, *h, 1, *h[0]);
				
				ReleaseResource((Handle)h); // release this resource now that we're done
				h = NULL; // just to be safe
			} // h was NULL
		} // stringID was < 1
		
		// Load the resource from the currently open resource fork.  Should be
		// our plug-ins resource fork.  If more than one plug-in is open, you
		// might want to use Get1Resource instead.  See Inside Macintosh.
		alertHandle = GetResource('ALRT', alertID);
		
		HNoPurge (alertHandle); // mark as unpurgeable until we're done with it
	
		// DialogUtilities has a routine that will center this for us:
		CenterDialog ((DialogTHndl) alertHandle);
	
		// PIParamText from PIUtilities will replace any occurrences of
		// ^0, ^1, or ^2 in a string (ds) with the text passed in:
		PIParamText(ds, NULL, minText, maxText);

		// This is the Macintosh call to replace the ^0 we've put in our
		// alert box with the string we've provided from the 'STR ' resource:
		ParamText (ds, NULL, NULL, NULL);
		
		switch (alertType)
		{ // will pop either stop or caution alert
		
			case PIAlertStop: // pop a stop alert, return any error (0 = no error)
				result = StopAlert (alertID, nil);
				break; // PIAlertStop
			case PIAlertCaution: // pop a caution alert, return any error (0 = no error)
			default:
				result = CautionAlert (alertID, nil);
				break;
				
		}
	
		// Mark the alertHandle as purgeable at next garbage collection:		
		HPurge (alertHandle);
		
		ReleaseResource (alertHandle); // deallocates and sets to NULL

	} // alertID wasn't valid
	
	return result;
	
} // end ShowAlertType

//-------------------------------------------------------------------------------
//
//	ShowAbout
//
//	Pops a simple dialog with text information about the plug-in.
//
//	Inputs:
//		const short dialogID	ID of dialog AND string resource with about text.
//
//	Outputs:
//		Displays About box.  Press Escape, Return, or Enter to dismiss.
//
//-------------------------------------------------------------------------------

void ShowAbout (short dialogID)
{
	short 			item;	// variable to receive item pressed (ignored)
	DialogPtr 		dp;		// dialog pointer for newly created dialog
	DialogTHndl 	dt;		// dialog handle for dialog resource
	StringHandle	h;		// string resource for about text
	
	// Inside Macintosh GetString() allocates then loads 'STR ' resource:
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
			sPSFileList->BrowseUrl(message);
		}
	}
	while (item == 4 || item == 5);
		
	DisposeDialog (dp); // closes and disposes the dialog

	ReleaseResource((Handle)h); // Releases the text resource

	HPurge ((Handle) dt); // mark the dialog info as purgeable

	ReleaseResource((Handle) dt); // releases the dialog info resource

} // end ShowAbout

//-------------------------------------------------------------------------------
//
//	CenterDialog
//
//	Centers a dialog template 1/3 of the way down the main screen.
//
//	Inputs:
//		DialogTHndl dt			Handle to dialog template.  Use GetResource.
//
//	Outputs:
//		Offsets
//			dialogBounds.top,
//			dialogBounds.left,
//			dialogBounds.bottom,
//			dialogBounds.right
//		By the amount appropriate to the screenBounds to center the dialog.
//		You must still display the dialog yourself.
//
//-------------------------------------------------------------------------------

void CenterDialog (DialogTHndl dt)
{
	// From QuickDraw.h, GetMainDevice provides a handle to what GDevice
	// is the startup screen, which includes the startup screen's
	// rect in gdRect:
	Rect screenBounds = (*GetMainDevice())->gdRect;

	// Calculate the dimensions of the screen:
	short width = screenBounds.right - screenBounds.left;
	short height = screenBounds.bottom - screenBounds.top;

	// Get our dialog's dimensions:
	Rect dialogBounds = (**dt).boundsRect;

	// From QuickDraw.h, OffsetRect will move a Rect by a constant value:
	OffsetRect (&dialogBounds, -dialogBounds.left, -dialogBounds.top);

	// Now move to top third, horizontally centered:
	OffsetRect (&dialogBounds, (width - dialogBounds.right) / 2,
					(height - dialogBounds.bottom - MenuHeight) / 3 + MenuHeight);

	// Store the new position:
	(**dt).boundsRect = dialogBounds;

} // end CenterDialog

//-------------------------------------------------------------------------------
//	Dialog handling -- Routine descriptors
//-------------------------------------------------------------------------------
ModalFilterUPP gFilterProc = NULL;

//-------------------------------------------------------------------------------
//
//	MoveableModalDialog
//
//	Dispatch and handle events for moveable modal dialogs.
//	DO NOT USE THIS WITH NON-MOVEABLE MODALS.  IT WILL CRASH.
//
//	Inputs:
//		DialogPtr dp					Pointer to instantiated dialog.
//										Use DisposeDialog when you're done.
//
//		ProcessEventproc processEvent	Pointer to the processEvent routine.
//
//		ModalFilterProcPtr filter		Pointer to routine to process special
//										(or all) events.
//
//		short *item						Item processed.
//
//	Outputs:
//		short *item						Item processed.
//
//-------------------------------------------------------------------------------

void MoveableModalDialog (DialogPtr dp,
						  ProcessEventProc processEvent,
						  ModalFilterProcPtr filter,
						  short *item)
{	
	ModalData data; // data structure with our custom data. See DialogUtilities.h

	data.oldRefCon = GetWRefCon (GetDialogWindow(dp)); // store current data in old reference
	data.filter = filter; // update filter proc to point at new filter
	data.processEvent = processEvent; // update process proc to point at new process
	
	SetWRefCon (GetDialogWindow(dp), (long) &data); // store new info
	
	// Process dialog using ModalDialog routine, pointing at Power Macintosh
	// routine or standard routine, depending on environment (see Routine
	// descriptors in DialogUtilities.h):
	gFilterProc = NewModalFilterUPP(DialogFilter);
	ModalDialog(gFilterProc, item);
	
	SetWRefCon (GetDialogWindow(dp), data.oldRefCon); // restore old info
	
} // end MoveableModalDialog

//-------------------------------------------------------------------------------
//
//	GetMoveableWRefCon
//
//	Retrieve the previous ModalData structure.
//
//	Inputs:
//		DialogPtr dp		Pointer to instantiated dialog.
//							Use DisposeDialog when you're done.
//
//	Outputs:
//		long				Pointer to ModalData structure.
//
//-------------------------------------------------------------------------------

long GetMoveableWRefCon (DialogPtr dp)
{
	
	// Grab the reference constant from Dialog dp and cast it to a
	// ModalData structure (it better be a pointer to a ModalData
	// structure, anyway, since we're working with it):
	ModalData *data = (ModalData *) GetWRefCon (GetDialogWindow(dp));
	
	// In that structure is a pointer to the last ModalData structure
	// that was processed, which was also stored in the reference
	// constant.  Return that pointer.
	return data->oldRefCon;
	
}

//-------------------------------------------------------------------------------
//
//	DialogFilter
//
//	Event filter for moveable modal dialogs.
//
//	Inputs:
//		DialogPtr dp			Pointer to instantiated dialog.
//								Use DisposeDialog when you're through with it.
//
//		EventRecord *event		What event occurred.  See Events.h.
//
//		short *item				Item processed.
//
//	Outputs:
//		returns FALSE			Didn't process anything.
//		returns TRUE			Got a valid event and dispatched it.
//
//		short *item				Item processed.
//
//-------------------------------------------------------------------------------

static pascal Boolean DialogFilter (DialogPtr dp,
									EventRecord *event,
									short *item)
{
	Boolean result = FALSE; // return value from filter events
	GrafPtr savePort; // from QuickDraw.h, variable for recent port
	
	// We use the reference constant to store a pointer to our
	// ModalData structure:
	ModalData *data = (ModalData *) GetWRefCon (GetDialogWindow(dp));
	
	if (data->filter)
	{ // hit a userItem that's been set to process via our routine:
		
		SetWRefCon (GetDialogWindow(dp), data->oldRefCon); // restore old reference value
		
		// call our routine, returning TRUE if successful:
		result = (*data->filter) (dp, event, item);
		
		// Store a pointer to the last reference constant in our structure:
		data->oldRefCon = GetWRefCon (GetDialogWindow(dp));
		
		// Update the current reference constant to our new ModalData:
		SetWRefCon (GetDialogWindow(dp), (long) data);
		
		// Return whether the filter process completed successfully:
		if (result)
			return TRUE;
		
	} // data->filter
	
	if (event->what == mouseDown)
	{ // we have to do window-dragging ourself

		Point pt = event->where; // the mouse location
		WindowPtr window; // we'll find the current window

		if (FindWindow (pt, &window) == inDrag && window == GetDialogWindow(dp))
		{ // we've found something

			// Get the current region (should be the whole dialog)
			// out to its bounds:
			Rect bounds;
			GetRegionBounds(GetGrayRgn(), &bounds);

			// Expand the bounds by 4 pixels (to cover shading, etc.):
			InsetRect (&bounds, 4, 4);

			// System routine to process dragging a Window:
			DragWindow (window, pt, &bounds);

			event->what = nullEvent; // Reset the event to nothing

		} // FindWindow
	} // mouseDown

	else if (event->what == keyDown && FALSE)
	{ // have put FALSE in here because the System generally
	  // handles these events better than we do.  Take out
	  // FALSE if you need the processing.

	  // We have to do button key-equivalents ourselves, too:

		// Get the character pressed:
		char ch = event->message & charCodeMask;

		if (ch == RETURN || ch == ENTER)
		{ // do default button on RETURN or ENTER

			*item = ok; // set item to ok button
			FlashDialogButton (dp, ok); // flash the button
			result = TRUE; // return that we processed something
		
		} // RETURN || ENTER

		else if (ch == ESCAPE || (ch == PERIOD && (event->modifiers & cmdKey)))
		{ // do cancel button on ESCAPE or Command-PERIOD

			*item = cancel; // set item to cancel button
			FlashDialogButton (dp, cancel); // flash the button
			result = TRUE; // return that we processed something
		
		} // ESCAPE || Cmd-Period
		
		else if (ch == TAB && (event->modifiers & shiftKey))
		{ // highlight previous edit text field on Shift-TAB
			
			// Put code here to
			// select previous item
			
		} // Shift-TAB
	
	} // keyDown
	
	else if (event->what == updateEvt || event->what == activateEvt)
	{ // Pass updates and activities out to the host:

		if (data->processEvent && (event->message != ((long) dp)))
			(*data->processEvent) (event);
	
	} // updateEvt

	else if (event->what == nullEvent)
	{ // Let the host idle:

		if (data->processEvent)
			(*data->processEvent) (event);

	} // nullEvent
		
	GetPort (&savePort); // store the current port
	SetPort ( GetDialogPort(dp) );
		
	// Use Standard Macintosh filter to process this event,
	// will return TRUE if successful:
	result = StdFilterProc (dp, event, item);
	
	SetPort (savePort); // restore the port

	return result; // TRUE if successful

} // end DialogFilter

//-------------------------------------------------------------------------------
//	Dialog outline -- Routine descriptors
//-------------------------------------------------------------------------------
UserItemUPP gUserItemOutlineOK = NULL;
UserItemUPP gUserItemOutlineGroup = NULL;	

//-------------------------------------------------------------------------------
//
//	SetOutlineOK
//
//	Routine to set button to be outlined like the default OK button.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		short item				Dialog item to outline.
//
//	Outputs:
//		Draws an outline around "item".
//
//-------------------------------------------------------------------------------

void SetOutlineOK (DialogPtr dp, const short item)
{
	
	// For GetDialogItem, these variables are passed to indicate the
	// item's type, it's local rect, and any extra data (Handle h):
	short 	itemType;
	Rect 	r;
	Handle 	h;

	// Get the info, then Set it, passing a handle to our
	// routine which will draw the outline (either on Power Mac or
	// on 68k):
	GetDialogItem (dp, item, &itemType, &h				   , &r);
	
	gUserItemOutlineOK = NewUserItemUPP(OutlineOK);
	SetDialogItem (dp, item,  itemType, (Handle) gUserItemOutlineOK, &r);
	
} // end SetOutlineOK

//-------------------------------------------------------------------------------
//
//	OutlineOK
//
//	Dialog proc to draw a heavy line around a "default" item.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		short item				Dialog item to outline.
//
//	Outputs:
//		Draws an outline around "item".
//
//-------------------------------------------------------------------------------

static pascal void OutlineOK (DialogPtr dp, const short item)
{
	
	PenState originalPenState; // from QuickDraw.h, will save the pen

	// For GetDialogItem, these variables are passed to indicate the
	// item's type, it's local rect, and any extra data (Handle h):
	Rect r;
	Handle h;
	short itemType;
	short myItem = item;
	
	// First, store the current pen state:
	GetPenState (&originalPenState);

	// Force the item to point to the "ok" button.  Take this out
	// if you want to outline some other button; specifically, make
	// sure you UN-outline the OK button before you outline
	// something else:
	myItem = ok;

	// Get the dialog information on the button:
	GetDialogItem (dp, myItem, &itemType, &h, &r);

	// Now change the pen to normal, size 3 points:
	PenNormal ();
	PenSize (3, 3);
	
	// Shrink the button rect by 4 points (to draw both on the
	// inside and outside of it) then draw a round rect:
	InsetRect (&r, -4, -4);
	FrameRoundRect (&r, 16, 16);

	SetPenState (&originalPenState); // restore the pen
	
} // end OutlineOK
	
//-------------------------------------------------------------------------------
//
//	SetOutlineGroup
//
//	Routine to set a user item to be a group box.  It expects
//	the next item to be the title for the group box.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		short item				Dialog item to be group box.
//
//	Outputs:
//		Draws an outline around "item".
//
//-------------------------------------------------------------------------------

void SetOutlineGroup (DialogPtr dp, short item)
{
	
	// For GetDialogItem, these variables are passed to indicate the
	// item's type, it's local rect, and any extra data (Handle h):
	short 	itemType;
	Rect 	r;
	Handle 	h;

	// Get the info, then Set it, passing a handle to our
	// routine which will draw the outline (either on Power Mac or
	// on 68k):
	GetDialogItem (dp, item, &itemType, &h				       , &r);
	gUserItemOutlineGroup = NewUserItemUPP(OutlineGroup);
	SetDialogItem (dp, item,  itemType, (Handle) gUserItemOutlineGroup, &r);
	
} // end SetOutlineGroup

//-------------------------------------------------------------------------------
//
//	OutlineGroup
//
//	Dialog proc to draw a group box.  It expects
//	the next item to be the title for the group box.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short item		Dialog item to be group box.
//
//	Outputs:
//		Draws an outline around "item".
//
//-------------------------------------------------------------------------------

static pascal void OutlineGroup (DialogPtr dp, const short item)
{

	PenState originalPenState; // from QuickDraw.h, will save the pen

	// For GetDialogItem, these variables are passed to indicate the
	// item's type, it's local rect, and any extra data (Handle h):
	Rect	 r, rText;
	Handle	 h;
	short	 itemType;

	// First, store the current pen state:
	GetPenState (&originalPenState);

	// Get the bounds of the title text (assumed to be next item):
	GetDialogItem (dp, item+1, &itemType, &h, &rText);

	// Get the bounds of the userItem to draw as a group box:
	GetDialogItem (dp, item,   &itemType, &h, &r);
	
	// Now, change the pen to normal, size 1 point:
	PenNormal ();
	PenSize (1, 1);
	
	// Move the pen to the top line of the group box,
	// to the right of the title text:
	MoveTo (rText.right, r.top);
	
	LineTo (r.right, r.top); // draw a line to the right box edge
	LineTo (r.right, r.bottom); // draw a line to the bottom right corner
	LineTo (r.left, r.bottom); // draw a line to the bottom left corner
	LineTo (r.left, r.top); // draw a line to the top left corner
	LineTo (rText.left, r.top); // draw a line to the left of the title text
	
	SetPenState (&originalPenState); // restore the pen

} // end OutlineGroup
	
//-------------------------------------------------------------------------------
//
//	SelectTextItem
//
//	Highlights the text in an edit text item.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short item		Edit text item to be selected.
//
//	Outputs:
//		Highlights the text.
//
//-------------------------------------------------------------------------------

void SelectTextItem (DialogPtr dp, const short item)
{
	
	SelectDialogItemText (dp, item, 0, 32767);
	
}

//-------------------------------------------------------------------------------
//
//	StuffText
//
//	Sets the text in a text item.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short item		Text item to be changed.
//
//		Str255 text				Pascal string to store in text item.
//
//	Outputs:
//		Changes the text of the dialog item to the Pascal string.
//
//-------------------------------------------------------------------------------

void StuffText (DialogPtr dp, const short item, Str255 text)
{
	
	// These items are passed to GetDialogItem to retrieve a
	// dialog's rect, type, and info:
	Rect 	r;
	short 	itemType;
	Handle 	h;
	
	GetDialogItem (dp, item, &itemType, &h, &r); // get item info
	SetDialogItemText (h, text); // sets new text
	
} // end StuffText

//-------------------------------------------------------------------------------
//
//	FetchText
//
//	Gets the text from a text item.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short item		Text item to be changed.
//
//	Outputs:
//		Str255 text				Pascal string to return text.
//
//-------------------------------------------------------------------------------

void FetchText (DialogPtr dp, const short item, Str255 text)
{
	
	// These items are passed to GetDialogItem to retrieve a
	// dialog's rect, type, and info:
	Rect 	r;
	short 	itemType;
	Handle 	h;
	
	GetDialogItem (dp, item, &itemType, &h, &r); // get item info
	GetDialogItemText (h, text); // gets new text
	
} // end FetchText
	
//-------------------------------------------------------------------------------
//
//	StuffNumber
//
//	Stuff a numeric (long) value into a text field.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short item		Text item to be changed.
//
//		const long value		Numeric value to put into item.
//
//	Outputs:
//		Puts number into text item.
//
//-------------------------------------------------------------------------------

void StuffNumber (DialogPtr dp, const short item, const long value)
{
	
	Str255 s; // pascal string to receive text version of value
		
	// System routine that converts a signed value to a pascal string:
	NumToString (value, s);
	
	// Use our routine to stuff text into an item:
	StuffText (dp, item, s);
	
} // end StuffNumber


//-------------------------------------------------------------------------------
//
//	FetchNumber
//
//	Retrieve a value from a text field.  FetchNumber will do range
//	checking and validation, but will NOT pin the number to the bounds.
//	Instead, the AlertNumber routine is used to pin the number.  This
//	way, the number can be returned as fast as possible and you can
//	do your own bounds checking, if you so desire.
//
//	The value will be set to 0 if no valid number is found.  This insures
//	you will always get returned a value.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short item		Text item to extract number from.
//
//		const long min			Minimum, low-bound value.
//
//		const long max			Maximum, high-bound value.
//
//	Outputs:
//		returns noErr			A valid value was retrieved from the field.
//		returns errOutOfRange	Value was beyond bounds and was pinned.
//		returns errNotANumber	Value was not valid. 
//
//		long *value				Value (0 if not valid).
//
//-------------------------------------------------------------------------------
   
short FetchNumber (DialogPtr dp,
					const short item,
					const long min,
					const long max,
					long *value)
{		
	Str255 	s = "";			// string to retrieve text from item
	long 	x = 0;			// initial number conversion
	short 	retn = noErr;	// return noErr if this works
	
	// Use our routine to retrieve a pascal string with the text
	// of the item:
	FetchText (dp, item, s);
	
	// Use the system routine StringToNumber to convert:
	if (StringToNumber (s, &x) == kStringToNumberInvalid)
	{ // StringToNumber returned false, meaning no valid number
	
		x = 0; // since no number was found, set value to 0
		retn = errNotANumber; // error to return
	}
	else if (x < min || x > max)
	{ // won't pin here, but will return error code
	
		retn = errOutOfRange;
	}
	
	*value = x; // assign value
	
	return retn; // return error code or noErr

} // end FetchNumber

//-------------------------------------------------------------------------------
//
//	AlertNumber
//
//	This is the accompanying alert mechanism to FetchNumber.
//	FetchNumber will return either noErr, errOutOfRange, or
//	errNotANumber.  AlertNumber pops error dialogs (the same, but
//	you can have them be different) for the two error conditions,
//	and pins the number between bounds.  It also stuffs the
//	newly pinned number into the text field and selects it.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short item		Text item to stuff number back into.
//
//		const long min			Minimum, low-bound value.
//
//		const long max			Maximum, high-bound value.
//
//		const short alertID		Resource ID of the alert box.
//
//		const short numberErr	Error enumeration.  See DialogUtilities.h.
//
//	Outputs:
//		Stuffs newly pinned value into text item.
//
//		long *value				Pinned value.
//
//-------------------------------------------------------------------------------

void AlertNumber (DialogPtr dp,
				  const short item,
				  const long min,
				  const long max,
				  long *value,
		 		  const short alertID,
		 		  const short numberErr)
{
	Str255 minText = "";	// convert min value to text string
	Str255 maxText = "";	// convert max value to text string
	
	long alertTextID;		// the alert text we'll display
	long x = *value;		// pin x then return value
	
	// Pin to bounds:
	if (x <= min) x = min;
	else x = max; // we wouldn't be here if we didn't have a problem,
				  // so assume if x isn't smaller than min, its
				  // too high.  And yes, we could do this as:
				  // x = (x < min ? min : max)
				  // but we won't save any cycles and it's not
				  // very readable
				  
	*value = x;	  // copy back to original
		
	StuffNumber (dp, item, x);	// replace dialog text item

	// convert min and max values to text equivalents:
	NumToString(min, minText);
	NumToString(max, maxText);
	
	switch (numberErr)
	{ // figure out what text to use
		case errOutOfRange:
		case errNotANumber:
		default:
			// we'll use the same dialog for all errors
			alertTextID = kBadNumberID;
			break;
	}
	
	// Now use our cross-platform dialog popper to display this.  You
	// could use a switch or other statement to pop different alerts,
	// if you wanted.  Once for errNotANumber to report "That's not
	// a number" and one for errOutOfRange to report "Valid range is
	// min to max." What I chose to do is report the same error in
	// each case "An integer is required between min and max.":
	(void) ShowCaution (dp,
						alertID,
				  	    alertTextID,
						minText,
						maxText);		

	// Now that we're done popping extra dialogs in front of our
	// target, go ahead and set the target to be the new text we
	// just shoved in the text field:
	SelectTextItem (dp, item);

} // end AlertNumber

//-------------------------------------------------------------------------------
//
//	StuffDouble
//
//	Stuff a floating point (double) value into a text field.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short item		Text item to be changed.
//
//		const double value		Numeric value to put into item.
//
//		const short precision	Decimal precision for conversion.
//
//	Outputs:
//		Puts floating point number into text item.
//
//-------------------------------------------------------------------------------

void StuffDouble (DialogPtr dp,
				  const short item,
				  const double value,
				  const short precision)
{
	Str255 	s; // pascal string to receive text version of value
	
	// Routine in PIUtilities that converts a double to a pascal string
	// with "precision" decimal points:
	DoubleToString(s, value, precision);
	
	// Use our routine to stuff text into an item:
	StuffText(dp, item, s);

} // end StuffDouble

//-------------------------------------------------------------------------------
//
//	FetchDouble
//
//	Retrieve a floating point (double) value from a text field.
//  FetchDouble will do range checking and validation,
//	but will NOT pin the number to the bounds.
//	Instead, the AlertDouble routine is used to pin the number.  This
//	way, the number can be returned as fast as possible and you can
//	do your own bounds checking, if you so desire.
//
//	The value will be set to 0.0 if no valid number is found.  This insures
//	you will always get returned a value.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short item		Text item to extract number from.
//
//		const double min		Minimum, low-bound value.
//
//		const double max		Maximum, high-bound value.
//
//	Outputs:
//		returns noErr			A valid value was retrieved from the field.
//		returns errOutOfRange	Value was beyond bounds and was pinned.
//		returns errNotANumber	Value was not valid. 
//
//		double *value			Value (0.0 if not valid).
//
//-------------------------------------------------------------------------------

short FetchDouble (DialogPtr dp,
				   const short item,
				   const double min,
				   const double max,
				   double *value)
{
	Str255 	s = "";			// string to receive whole number portion
	double 	x = 0;			// initial number conversion
	short 	retn = noErr;	// return noErr unless not a number of out of range
	
	// Use our routine to retrieve a pascal string with the text
	// of the item:
	FetchText (dp, item, s);

	// Use the PIUtilities routine StringToDouble to convert:
	if (StringToDouble (s, &x) == kStringToNumberInvalid)
	{ // StringToDouble returned false, meaning no valid number

		x = 0; // since no number was found, set value to 0
		retn = errNotANumber; // error to return
	}
	else if (x < min || x > max) 
	{ 	// won't pin here, but will return error code
	
		retn = errOutOfRange;
	}

	*value = x; // assign to value
	
	return retn; // return noErr or errOutOfRange/errNotANumber

} // end FetchDouble

//-------------------------------------------------------------------------------
//
//	AlertDouble
//
//	This is the accompanying alert mechanism to FetchDouble.
//	FetchDouble will return either noErr, errOutOfRange, or
//	errNotANumber.  AlertDouble pops error dialogs (the same, but
//	you can have them be different) for the two error conditions,
//	and pins the number between bounds.  It also stuffs the
//	newly pinned number into the text field and selects it.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short item		Text item to stuff number back into.
//
//		const double min		Minimum, low-bound value.
//
//		const double max		Maximum, high-bound value.
//
//		const short alertID		Resource ID of the alert box.
//
//		const short numberErr	Error enumeration.  See DialogUtilities.h.
//
//	Outputs:
//		Stuffs newly pinned value into text item.
//
//		double *value			Pinned value.
//
//-------------------------------------------------------------------------------
  
void AlertDouble (DialogPtr dp,
				  const short item,
				  const double min,
				  const double max,
				  double *value,
				  const short alertID,
				  const short numberErr)
{ 
	Str255 minText = "";	// convert min value to text string
	Str255 maxText = "";	// convert max value to text string
	
	short precision = 0;	// start with no decimal precision
	long alertTextID;		// alert text to use
	double x = *value;		// pin x then return value
	
	// We're going to figure out precision how we did before:
	// Grabbing the original text, dividing it at the decimal
	// point (period ".") and using the length of the string
	// portion of the decimal to be the precision:
	FetchText(dp, item, minText); // retrieve text from dialog
	
	// Our routine to truncate string1 (minText) at the decimal
	// point (period ".") and move anything after to
	// string2 (maxText):
	DivideAtDecimal(minText, maxText);
	
	// The precision for the conversion should be no more than
	// the length of the decimal portion string:
	precision = maxText[0]; // pascal string length is first byte
	
	// Pin to bounds:
	if (x <= min)
		x = min;
	else x = max; // we wouldn't be here if we didn't have a problem,
				  // so assume if x isn't smaller than min, its
				  // too high.  And yes, we could do this as:
				  // x = (x < min ? min : max)
				  // but we won't save any cycles and it's not
				  // very readable
	
	*value = x;	  // copy back to original
			
	StuffDouble (dp, item, x, precision); // replace dialog text item

	// convert min and max values to text equivalents:
	DoubleToString (minText, min, precision);
	DoubleToString (maxText, max, precision);
	
	switch (numberErr)
	{ // figure out what text to use
		case errOutOfRange:
		case errNotANumber:
		default:
			// we'll use the same dialog for all errors
			alertTextID = kBadNumberID;
			break;
	}
// Now use our cross-platform dialog popper to display this.  You
	// could use a switch or other statement to pop different alerts,
	// if you wanted.  Once for errNotANumber to report "That's not
	// a number" and one for errOutOfRange to report "Valid range is
	// min to max." What I chose to do is report the same error in
	// each case "An integer is required between min and max.":
	(void) ShowCaution (dp,
						alertID,
				  	    kBadDoubleID,
						minText,
						maxText);	

	// Now that we're done popping extra dialogs in front of our
	// target, go ahead and set the target to be the new text we
	// just shoved in the text field:
	SelectTextItem (dp, item);

} // end AlertDouble

//-------------------------------------------------------------------------------
//
//	ProcessChecksAndRadios
//
//	Perform standard handling for check boxes and radio buttons. For radio
//	buttons, we assume that the group for the radio button extends forward
//	and backward in the DITL as long as the item type is radio button.
//
//	Inputs:
//		DialogPtr dp					Pointer to instantiated dialog.
//										Use DisposeDialog when you're done.
//
//		short item						Item to process.
//
//	Outputs:
//		Toggles check boxes and radio button groups.
//
//		Edit text, OK, Cancel, and any other items you still must
//		handle yourself.
//
//-------------------------------------------------------------------------------

void ProcessChecksAndRadios (DialogPtr dp, short item)
{
	
	// For GetDialogItem, these variables are passed to indicate the
	// item's type, it's local rect, and any extra data (Handle h):
	Rect 	r;
	Handle 	h;
	short 	itemType;
	
	short first, last, count; // for radio groups
	
	// First, get the information about the item:
	GetDialogItem (dp, item, &itemType, &h, &r);
	
	switch (itemType)
	{ // see if we can handle this item:
		
		case ctrlItem+chkCtrl: // item is a Check Box, so toggle it
			(void) ToggleCheckBoxState (dp, item);
			break; // ctrlItem+chkCtrl
			
		case ctrlItem+radCtrl: // item is a radio button in a group
			first = last = item;
			while (first > 1)
			{ // work backwards until we find the first button in
			  // the group:
			  
				GetDialogItem (dp, first - 1, &itemType, &h, &r); // get item info
				if (itemType != ctrlItem+radCtrl)
					break;
					// we found a non-radio, so the group must
					// be done
					
				--first; // keep subtracting from first
			
			} // first
			
			// get the total number of dialog items:
			count = CountDITL (dp);
			
			while (last < count)
			{ // work forwards until we find the last button in
			  // the group:
			  
				GetDialogItem (dp, last + 1, &itemType, &h, &r); // get item info
				if (itemType != ctrlItem+radCtrl)
					break;
					// we found a non-radio, so the group must
					// be done
					
				++last; // keep adding to last
			
			} // last
			
			// Now use our routine to set all items from
			// "first" to "last" to off, then turn on "item":
			SetRadioGroupState (dp, first, last, item);
			
			break; // ctrlItem+radCtrl
		
	} // itemType
	
} // end ProcessChecksAndRadios

//-------------------------------------------------------------------------------
//
//	SetCheckBoxState
//
//	Sets the state of a check box (or radio button)
//	to either checked (on) or unchecked (off).
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short item		Dialog item to process.
//
//		const Boolean checkIt	TRUE = Check; FALSE = Uncheck.
//
//	Outputs:
//		Checks or unchecks "item".
//
//-------------------------------------------------------------------------------

void SetCheckBoxState (DialogPtr dp, const short item, const bool checkIt)
{

	// For GetDialogItem, these variables are passed to indicate the
	// item's type, it's local rect, and any extra data (Handle h):
	Rect	r;
	Handle	h;
	short	itemType;
	
	short	oldValue;
	short	newValue = 0; // initially be off
	
	GetDialogItem (dp, item, &itemType, &h, &r); // get item info

	oldValue =  GetControlValue ((ControlHandle) h); // get value (on/off)
	
	// Now make the value whatever checkIt is:
	if (checkIt) newValue = 1; // otherwise it's already one
	
	// There are plenty of other ways to do this.  I didn't do:
	// newValue = checkIt ? 1 : 0
	// because that doesn't save any code and it is far less
	// readable.  I also didn't do:
	// newValue = !checkIt
	// because, while FALSE is usually always defined as 0, some
	// compilers define TRUE as -1 (although most define it as 1).

	// If oldValue was not what was requested in checkIt, then
	// we've changed states, so go ahead and make the change:
	if (oldValue != newValue)
		SetControlValue ((ControlHandle) h, newValue);

} // end SetCheckBoxState

//-------------------------------------------------------------------------------
//
//	GetCheckBoxState
//
//	Returns the state of a check box or radio button.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short item		Dialog item to query.
//
//	Outputs:
//		returns Boolean	TRUE	If checked/on.
//		returns Boolean FALSE	If unchecked/off.
//
//-------------------------------------------------------------------------------

bool GetCheckBoxState (DialogPtr dp, const short item)
{
	
	// For GetDialogItem, these variables are passed to indicate the
	// item's type, it's local rect, and any extra data (Handle h):
	Rect	r;
	Handle	h;
	short	itemType;
	
	short	oldValue; // we'll put the value here
	
	GetDialogItem (dp, item, &itemType, &h, &r); // get item info

	oldValue = GetControlValue ((ControlHandle) h); // get value
	
	return (oldValue != 0); // non-zero is checked
	
} // end GetCheckBoxState

//-------------------------------------------------------------------------------
//
//	ToggleCheckBoxState
//
//	Toggles the state of a check box or radio button.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short item		Dialog item to process.
//
//	Outputs:
//		Swaps the state of the check box/radio button and
//		returns its current state.
//
//		returns Boolean	TRUE	If checked/on.
//		returns Boolean FALSE	If unchecked/off.
//
//-------------------------------------------------------------------------------

bool ToggleCheckBoxState (DialogPtr dp, const short inItem)
{
	
	// GetCheckBoxState returns the state (TRUE=on, FALSE=off)
	// so lets just use the opposite of its return:
	bool newState = !GetCheckBoxState (dp, inItem);
	
	SetCheckBoxState (dp, inItem, newState); // set the new state
	
	return newState; // return that new state
	
} // end ToggleCheckBoxState

//-------------------------------------------------------------------------------
//
//	SetRadioGroupState
//
//	Turns off all items of a radio group and turns on the one selected.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short first		First dialog item in group.
//
//		const short last		Last dialog item in group.
//
//		const short item		Dialog item that is selected.
//
//	Outputs:
//		Changes radio button group state to all off except
//		the one selected.
//
//-------------------------------------------------------------------------------

void SetRadioGroupState (DialogPtr dp,
						 const short first,
						 const short last,
						 const short item)
{

	short loop; // use this to loop
	
	// loop from first to last, turning everything off that is
	// not the selected item (ie, all but the item):
	for (loop = first; loop <= last; ++loop)
		SetCheckBoxState (dp, loop, loop == item);

} // end SetRadioGroupState

//-------------------------------------------------------------------------------
//
//	GetRadioGroupState
//
//	Returns the selected button from a group of buttons.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short first		First dialog item in group.
//
//		const short last		Last dialog item in group.
//
//	Outputs:
//		returns short item		Dialog item that is selected,
//								0 if nothing selected. If you
//								are returned 0, and you're
//								working with a radio group, then
//								you've just broken human interface
//								guidelines, which state that
//								radio buttons are mutually exclusive
//								groups with always one selected.
//
//-------------------------------------------------------------------------------

short GetRadioGroupState (DialogPtr dp, 
						  const short first, 
						  const short last)
{
	
	short loop = first;
	short item = 0; // return 0 if we can't find it
	
	// now loop through the group, and if we find one that's on
	// return its item number in "item":
	do
	{ // start at the end and work to the first item
	
		if (GetCheckBoxState (dp, loop))
		{ // found one!
		
			item = loop; // we're return this
			loop = last; // end our loop gracefully
	
		} // CheckBoxState returned FALSE
	
	} while (loop++ < last);
		
	return item; // 0 if we couldn't find it or item number
	
	}

//-------------------------------------------------------------------------------
//
//	SetPopUpMenuValue
//
//	Sets the menu item of a dialog pop-up menu.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short first		First dialog item in group.
//
//		const short last		Last dialog item in group.
//
//		const short item		Dialog item that is selected.
//
//	Outputs:
//		Changes radio button group state to all off except
//		the one selected.
//
//-------------------------------------------------------------------------------

void SetPopUpMenuValue (DialogPtr dp,
						const short item,
						const short newValue)
	{

	// For GetDialogItem, these variables are passed to indicate the
	// item's type, it's local rect, and any extra data (Handle h):
	Rect	r;
	Handle	h;
	short	itemType;

	short	oldValue; // we'll put the value here
	
	GetDialogItem (dp, item, &itemType, &h, &r); // get item info

	oldValue =  GetControlValue ((ControlHandle) h); // get value
	
	if (oldValue != newValue)
	{ // we want to store something different, so check
	  // it against the bounds for the control, and if the
	  // bounds are less, then grow them to be at least as
	  // large as the new value:
	
		// Check to see if we need to grow the Maximum:
		if (GetControlMaximum ((ControlHandle) h) < newValue)
			SetControlMaximum ((ControlHandle) h, newValue);
		
		// Now set the control's value to our newValue:
		SetControlValue ((ControlHandle) h, newValue);
	
	} // oldValue == newValue

} // end SetPopUpmenuValue

//-------------------------------------------------------------------------------
//
//	GetPopUpMenuValue
//
//	Return the selected item of a dialog pop-up menu.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short item		Pop-up menu dialog item.
//
//	Outputs:
//		returns short			Value of the control.  For a pop-up menu,
//								the current line selected/displayed.
//
//-------------------------------------------------------------------------------

short GetPopUpMenuValue (DialogPtr dp, short item)
{
	
	// For GetDialogItem, these variables are passed to indicate the
	// item's type, it's local rect, and any extra data (Handle h):
	Rect	r;
	Handle	h;
	short	itemType;
	
	GetDialogItem (dp, item, &itemType, &h, &r); // get item info

	// Now get the value and return it:
	return GetControlValue ((ControlHandle) h);
	
} // end GetPopUpMenuValue

//-------------------------------------------------------------------------------
//
//	ShowHideItem
//
//	Shows or hides a dialog item.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short inItem		Pop-up menu dialog item.
//
//		const Boolean inState	TRUE to show, FALSE to hide.
//
//	Outputs:
//		Shows (TRUE) or Hides (FALSE) control.
//
//-------------------------------------------------------------------------------

void ShowHideItem (DialogPtr dp, const short inItem, const bool inState)
{
	if (inState)
		ShowDialogItem(dp, inItem);
	else
		HideDialogItem(dp, inItem);

} // end ShowHideItem

//-------------------------------------------------------------------------------
//
//	PIDisableControl
//
//	Disables a control dialog item.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short inItem		Control dialog item.
//
//	Outputs:
//		Disables (greys) the control.
//
//-------------------------------------------------------------------------------

void PIDisableControl (DialogPtr dp, const short inItem)
{

	// For GetDialogItem, these variables are passed to indicate the
	// item's type, it's local rect, and any extra data (Handle h):
	Rect	r;
	Handle	h;
	short	itemType;
	
	GetDialogItem (dp, inItem, &itemType, &h, &r); // get item info

	itemType |= itemDisable; // flag disable handle

	// There's no reason for us to change the control's value, just
	// its state.  Uncomment this code if you're in old Mac OS's, which
	// don't do the right thing:
	/*
	// Now get the old value and see if we are changing the state:
	{
	short oldValue = GetControlValue ((ControlHandle) h);
	
	if (oldValue != 0)
		SetControlValue ((ControlHandle) h, 0); // change state to disabled
	}
	*/

	HiliteControl ((ControlHandle) h, 255);

	SetDialogItem (dp, inItem, itemType, h, &r); // store new info

} // end PIDisableControl

//-------------------------------------------------------------------------------
//
//	PIEnableControl
//
//	Enables a control dialog item.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short inItem		Control dialog item.
//
//	Outputs:
//		Enables the control.
//
//-------------------------------------------------------------------------------

void PIEnableControl (DialogPtr dp, short inItem)
{

	// For GetDialogItem, these variables are passed to indicate the
	// item's type, it's local rect, and any extra data (Handle h):
	Rect	r;
	Handle	h;
	short	itemType;
	
	GetDialogItem (dp, inItem, &itemType, &h, &r);

	itemType &= ~itemDisable; // bitwise AND with NOT (reverse) itemDisable bit

	HiliteControl ((ControlHandle) h, 0);

	SetDialogItem (dp, inItem, itemType, h, &r); // store new info

} // end PIEnableControl

//-------------------------------------------------------------------------------
//
//	EnableDisableControl
//
//	Enables or Disables a control dialog item.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short inItem		Control dialog item.
//
//		const Boolean inState	Enable if TRUE, disable if FALSE.
//
//	Outputs:
//		Enables (TRUE) or Disables/greys (FALSE) the control.
//
//-------------------------------------------------------------------------------

void EnableDisableControl (DialogPtr dp, const short inItem, const bool inState)
{
	
	if (inState)
		PIEnableControl (dp, inItem);
	else
		PIDisableControl (dp, inItem);
	
} // end EnableDisableControl

//-------------------------------------------------------------------------------
//
//	InvalItem
//
//	Invalidate a dialog item.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short inItem		Dialog item.
//
//	Outputs:
//		Invalidates an item so it gets redrawn at next redraw.
//
//-------------------------------------------------------------------------------

void InvalItem (DialogPtr dp, short inItem)
{
	
	// For GetDialogItem, these variables are passed to indicate the
	// item's type, it's local rect, and any extra data (Handle h):
	Rect	r;
	Handle	h;
	short	itemType;
	
	GrafPtr oldPort;
	
	GetDialogItem (dp, inItem, &itemType, &h, &r); // get item info
	
	GetPort (&oldPort); // store the old port
	
	SetPort (GetDialogPort(dp)); // point to our dialog
	
	InvalWindowRect (GetWindowFromPort(GetDialogPort(dp)), &r); // invalidate the rect of our item
	
	SetPort (oldPort); // restore the old port
	
} // end InvalItem

//-------------------------------------------------------------------------------
//
//	FlashDialogButton
//
//	Flashes a dialog button.  (Usually in response to a keystroke).
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		const short inItem		Dialog item.
//
//	Outputs:
//		Flashes the item in question if it is a button.
//
//-------------------------------------------------------------------------------

 void FlashDialogButton (DialogPtr dp, short inItem)
{

	// For GetDialogItem, these variables are passed to indicate the
	// item's type, it's local rect, and any extra data (Handle h):
	Rect r;
	Handle h;
	short itemType;

	GetDialogItem (dp, inItem, &itemType, &h, &r); // get item info
	
	if (itemType == (ctrlItem + btnCtrl))
	{ // is this item a button?

		unsigned long ignore;

		HiliteControl ((ControlHandle) h, 1); // inverted

		Delay (5, &ignore); // wait

		HiliteControl ((ControlHandle) h, 0); // normal

	} // item was not a button or done
	
} // end FlashDialogButton

#else // #if !__LP64__ 

//-------------------------------------------------------------------------------

// TODO add support here for generic alert on 64 bit

short ShowVersionAlert (void* dp,
						const short alertID, 
						const short stringID,
						unsigned char* versText1,
						unsigned char* versText2)
{
	return 0;
}

void ShowAbout (short dialogID)
{
}

#endif // #if !__LP64__ 

// end DialogUtilitiesMac.cpp

