/* $Header: */

/* Changes
Date			Edit				Fault					Description
14-Mar-19	CR0584										Clean up unsupported Apple Help book / viewer methods.
08-Mar-19	CR0583										Win/Linux stub changes and more tidy ( not marked ).
04-Mar-19	CR0580										Initial Omnis implementation ( tidy up not marked ).
13/11/2016		Kelly Burgess								Added 64bit Cocoa menu code for Studio 8.x (find 'is64bit')
15/11/2007		Kelly Burgess								Added suggested workaround for 2x sets of menu items shown (and no Spotlight menu) under OS X 10.5 (find 'KB added')
28/04/2006		Kevin Gale									First Universal Binary Build
04/08/2006		Kevin Gale			NCSHELP01				Fixed crash when selecting menu's with no titles (ie: in print dialog) and Preview button in print dialog
*/

#include <extcomp.he>
#include "osxhelp.he"
#include "xosxhelp.he"

//ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  tqfObj  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ


// object initialisation
tqfObj::tqfObj()
{
	mRefCount = 1;	// One instance
}

// object destruction - just call the close method	
tqfObj::~tqfObj()
{
}

//ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  tqfObj::methodCall  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ
// object method call
qbool tqfObj::methodCall(qobjinst pObjPtr, EXTCompInfo* pEci)
{
	qlong funcId = ECOgetId(pEci);
	qbool rtnCode = qfalse; 
	EXTfldval rtnVal; 
	qbool hasRtnVal=qfalse;

	switch (funcId)
	{
		case cIMethodId_LoadHandler:		//	$loadhandler() Loads the menu handler
		case cIMethodId_UnloadHandler:		//	$unloadhandler() Unloads the menu handler
		{
			rtnVal.setLong(qtrue);
			hasRtnVal = qtrue;
			rtnCode = qtrue;
			break;
		}
		case cIMethodId_Help:	//	$help(menuitemname) Method called when menu item selected
		case cIMethodId_EnablePreferences:	//	$enablepreferences() Enables the Preferences menu item
		case cIMethodId_DisablePreferences:	//	$disablepreferences() Disables the Preferences menu item
		case cIMethodId_AboutBox:	//	$about:$about() Opens the about box	// CR0584
		{
			rtnCode = qtrue;
			break;
		}
		case cIMethodId_AddHelpItem:	//	$addhelpitem(menuitemname,shortcutkey,handledbystudio) Adds the menu item to the Help menu and returns the item's index
		case cIMethodId_CountHelpItems:		//	$counthelpitems() Returns the number of items in the Help menu
		{
			rtnVal.setLong(0);
			hasRtnVal = qtrue;
			rtnCode = qtrue;
			break;
		}
            //ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  $deletehelpitem method  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ
		case cIMethodId_DeleteHelpItem:		//	$deletehelpitem(menuitemindex) Removes the menu item from the Help menu
		case cIMethodId_EnableHelpItem:	//	$enablehelpitem(menuitemindex) Enables the menu item in the Help menu
		case cIMethodId_DisableHelpItem:		//	$disablehelpitem(menuitemindex) Disables the menu item in the Help menu
		case cIMethodId_SetHelpItem:		//	$sethelpitem(menuitemindex,menuitemname,shortcutkey,handledbystudio) Sets the Help
		case cIMethodId_GetHelpItem:		//	$gethelpitem(menuitemindex,menuitemname,shortcutkey,handledbystudio) Gets the Help
		case cIMethodId_ClearHelpMenu:	//	$clearhelpmenu() Clears the Help menu
		case cIMethodId_RegisterHelpBook:		//	$registerhelpbook(helpbookpackagepath) Registers the Help book package
		case cIMethodId_UnRegisterHelpBook:	//	$unregisterhelpbook(helpbookpackagepath) Unregisters the Help book package
		case cIMethodId_OpenHelp:		//	$openhelp() Opens the Apple Help viewer
		case cIMethodId_GotoPage:		//	$gotopage(bookname,pageurl,anchorname) Displays the specified page from the Help book
		case cIMethodId_GotoAnchor:	//	$gotoanchor(bookname,anchorname) Displays the Help book page containing the anchor
		case cIMethodId_Search:		//	$search(bookname,searchstring) Searches the Help book page with the specified string
		{
			rtnVal.setLong(qfalse);
			hasRtnVal = qtrue;
			rtnCode = qtrue;
			break;
		}

            //ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  $about method  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ
	}
	
	if (hasRtnVal)
		ECOaddParam(pEci,&rtnVal);

	return rtnCode;
}
//////////////////////// End Object Methods //////////////////////////////
// End of file
