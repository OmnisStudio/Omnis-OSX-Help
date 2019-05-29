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
#include <extfval.he>
#include <hwnd.he>
#include <gdi.he>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <Cocoa/Cocoa.h>
#define eventNotHandledErr -9874
#include "osxhelp.he"
#include "xosxhelp.he"


void SetCharU(EXTfldval* cvalp, void* buffer, long len);
CFStringRef GetCFStrRef(strxxx &strval);

@implementation cocoa_proxy

- (id)initWithObject:(void*)objectToCall
{
    self = [super init];
    if (self)   {
        mDispatch = objectToCall;
    }
    return self;
}



- (void)action:(id)sender
{
    tqfObj* dispatch = (tqfObj*)mDispatch;
		dispatch->MenuHandler((__bridge void*)sender);
}
@end

//////////////////////// Object Methods //////////////////////////////

EventHandlerRef loadmenuhandler(OSXHelpDataPtr mOSXHelpDataRef)
{
		
	return nil; // in Cocoa the equivalent MenuHandler is connected per NSMenuItem action, not like Carbon events
}

void unloadmenuhandler(EventHandlerRef eventHandlerRef)
{
}


OSStatus unregisterhelpbook(CFURLRef bundleURL)
{
	return noErr;
}

//ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  SetCharU  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ
//	extFldVal.setChar for unicode or not

void SetCharU(EXTfldval* cvalp, void* buffer, long len)
{
	CHRconvFromBytes ubuf((qbyte *)buffer,(qlong)len);
	cvalp->setChar (ubuf.dataPtr(),ubuf.len());
}

//	end of SetCharU
//ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  GetCFStrRef  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ
//	CFStringCreateWithPascalString for unicode or not

CFStringRef GetCFStrRef(strxxx &strval)
{
		CFStringRef strref;
		unsigned char pasStr[256];
		CHRconvToBytes abuf(strval.cString(), strval.length());
		memcpy(&pasStr[1], abuf.dataPtr(), abuf.len());
		pasStr[0] = abuf.len();
		strref = CFStringCreateWithPascalString(NULL, pasStr, kCFStringEncodingMacRoman);
		return strref;
}

//	end of GetCFStrRef
//ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  tqfObj  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ


// object initialisation
tqfObj::tqfObj()
{
	mRefCount = 1;	// One instance
	mOSXHelpDataRef = NULL;
	//mIsDemo = qtrue; // demo version indicator
	mDemoCount = 0;
	mIsLoaded = qfalse;
}

//object close
void tqfObj::close()
{
	if (mIsLoaded)
	{
		unloadmenuhandler(mOSXHelpDataRef->menuHandlerRef);
		MEMfree((qchar*)mOSXHelpDataRef);
		mIsLoaded = qfalse;
	}
}

// object destruction - just call the close method	
tqfObj::~tqfObj()
{
	close();
}

//ÙÙ__bridge ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  tqfObj::MenuHandler  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ
//  called by cocoa_proxy action delegate wired as NSMenuItem target
void tqfObj::MenuHandler(void* id)
{
    if (mIsLoaded)  {
				NSMenuItem *item = (__bridge NSMenuItem*)id;
        NSString *cmdStr = [item title];

        char* cString = (char*)MEMmalloc(256);
        if (cString) {
            if ([cmdStr getCString:cString maxLength:255 encoding:NSMacOSRomanStringEncoding]) {	// NO return if conversion not successful
                
                str255  methodName;
                methodName.setUtf8((qbyte*)"$help", 5);
                EXTfldval menuItemSelected;
                SetCharU(&menuItemSelected, cString, strlen(cString));

                ECOdoMethod(mOSXHelpDataRef->mObjInst, &methodName, &menuItemSelected, 1);
            }
            MEMfree((qchar*)cString);
        }
    }
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
            //ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  $loadhandler method  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ
#pragma mark $loadhandler
			case cIMethodId_LoadHandler:		//	$loadhandler() Loads the menu handler
		{
			if (! mIsLoaded)
			{
				mOSXHelpDataRef = (OSXHelpDataPtr) new OSXHelpData();//MEMmalloc(sizeof(OSXHelpData));

				//	KB added Nov 15, 2007 - set a flag based on Leopard or not
                //  and tweaked for 64bit in Nov 2016
				//	On OS X 10.5 Leopard with OSXHelp v1.0.3, we get a double set of menu items, and no Spotlight item at the top
				
				mOSXHelpDataRef->tenFiveOrLater = true;
				
				//	end of KB additions

				mOSXHelpDataRef->mObjInst = pObjPtr;

				NSMenu *helpMenu = [[NSMenu alloc] initWithTitle:@"Help"];
				NSMenu *mainMenu = [NSApp mainMenu];
				mOSXHelpDataRef->oldHelpMenu = [[mainMenu itemAtIndex:[mainMenu numberOfItems] - 1] submenu];
				/*
					What I do now is
						[NSApp setMainMenu:self.mainMenu];
						[NSApp setHelpMenu:[[self.mainMenu itemAtIndex:[self.mainMenu numberOfItems] - 1] submenu]];
					and it works without problems.
				 */
				[NSApp setHelpMenu:helpMenu];	// Spotlight for Help will be installed in it
				[NSApp setHelpMenu:[[mainMenu itemAtIndex:[mainMenu numberOfItems] - 1] submenu]];
				mOSXHelpDataRef->helpMenuRef = [NSApp helpMenu];
				mOSXHelpDataRef->proxy = [[cocoa_proxy alloc] initWithObject:(void*)this];

				

				mIsLoaded = qtrue;
			}

			rtnVal.setLong(qtrue);
			hasRtnVal = qtrue;
			rtnCode = qtrue;
			break;
		}
            //ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  $unloadhandler method  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ
#pragma mark $unloadhandler
		case cIMethodId_UnloadHandler:		//	$unloadhandler() Unloads the menu handler
		{
			if (mIsLoaded)
			{
				NSMenu *mainMenu = [NSApp mainMenu];
				[NSApp setHelpMenu:mOSXHelpDataRef->oldHelpMenu];   //  clear our helpMenu
				[NSApp setHelpMenu:[[mainMenu itemAtIndex:[mainMenu numberOfItems] - 1] submenu]]; //   helped with stick but not unstick
				mOSXHelpDataRef->helpMenuRef = nil;
				mOSXHelpDataRef->oldHelpMenu = nil;
				delete mOSXHelpDataRef; //MEMfree((qchar*)mOSXHelpDataRef);
				mIsLoaded = qfalse;
			}

			rtnVal.setLong(qtrue);
			hasRtnVal = qtrue;
			rtnCode = qtrue;
			break;
		}
            //ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  $help method  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ
#pragma mark $help
		case cIMethodId_Help:	//	$help(menuitemname) Method called when menu item selected
		{
			rtnCode = qtrue;
			break;
		}
            //ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  $addhelpitem method  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ
#pragma mark $addhelpitem
		case cIMethodId_AddHelpItem:	//	$addhelpitem(menuitemname,shortcutkey,handledbystudio) Adds the menu item to the Help menu and returns the item's index
		{
			UInt16		itemIndex = 0;
			UInt32		handledbyStudio = noErr;
			CFStringRef		menuItemRef;
			
			if (mIsLoaded)
			{
				EXTParamInfo* param1 = ECOfindParamNum(pEci, 1);
				EXTParamInfo* param2 = ECOfindParamNum(pEci, 2);
				EXTParamInfo* param3 = ECOfindParamNum(pEci, 3);
				if (param1)
				{
					EXTfldval fval1((qfldval)param1->mData);
					strxxx &itemname = fval1.getChar();
					if (itemname.length() == 0)
						itemname.insert('-',1);
					
					menuItemRef = GetCFStrRef(itemname);

					SEL actionSel = sel_registerName("MenuHandler");
					NSMenu *helpMenu = [NSApp helpMenu];
					NSMenuItem *newItem = [[NSMenuItem alloc] initWithTitle:(__bridge NSString *)menuItemRef
                                                                         action:actionSel keyEquivalent:@""];
					[helpMenu addItem:newItem];
					itemIndex	= [[helpMenu itemArray] count];
					/*
					 cocoa_proxy* proxy = [[cocoa_proxy alloc] initWithObject:(void*)this];

					 */
					[newItem setTarget:mOSXHelpDataRef->proxy];
					[newItem setAction:@selector(action:)];

					CFRelease(menuItemRef);
						
					if (itemIndex > 0)
					{
						if (param2)
						{
							EXTfldval fval2((qfldval)param2->mData);
							strxxx &shortcutkey = fval2.getChar();
					
							if (shortcutkey.length() > 0)
							{
									shortcutkey.upps();
									CFStringRef shortcutRef = GetCFStrRef(shortcutkey);
									NSMenuItem *item = [helpMenu itemAtIndex:(itemIndex-1)];
									[item setKeyEquivalent:(__bridge NSString *)shortcutRef];
									CFRelease(shortcutRef);
							}
						}
						if (param3)
						{
							EXTfldval fval3((qfldval)param3->mData);
							handledbyStudio = fval3.getLong();
							if (handledbyStudio != 0)
								handledbyStudio = eventNotHandledErr;
						}
					}
				}
			}
	
			rtnVal.setLong(itemIndex);
			hasRtnVal = qtrue;
			rtnCode = qtrue;
			break;
		}
            //ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  $deletehelpitem method  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ
#pragma mark $deletehelpitem
		case cIMethodId_DeleteHelpItem:		//	$deletehelpitem(menuitemindex) Removes the menu item from the Help menu
		{
			qbool	result = qfalse;
            UInt16  itemCount = 0;

			if (mIsLoaded)
			{
				EXTParamInfo* param1 = ECOfindParamNum(pEci, 1);
				if (param1)
				{
					EXTfldval fval1((qfldval)param1->mData);
					UInt16 itemIndex = fval1.getLong();
					NSMenu *helpMenu = [NSApp helpMenu];
					itemCount	= [helpMenu numberOfItems];
					if ((itemIndex > 0) && (itemIndex <= itemCount))
					{
													NSMenuItem *item = [helpMenu itemAtIndex:(itemIndex-1)];
							[helpMenu removeItem:item];
							result = qtrue;
					}
				}
			}
			
			rtnVal.setLong(result);
			hasRtnVal = qtrue;
			rtnCode = qtrue;
			break;
		}
            //ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  $enablehelpitem method  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ
#pragma mark $enablehelpitem
		case cIMethodId_EnableHelpItem:	//	$enablehelpitem(menuitemindex) Enables the menu item in the Help menu
		{
			qbool	result = qfalse;
            UInt16  itemCount = 0;
	
			if (mIsLoaded)
			{
				EXTParamInfo* param1 = ECOfindParamNum(pEci, 1);
				if (param1)
				{
					EXTfldval fval1((qfldval)param1->mData);
					UInt16 itemIndex = fval1.getLong();
					NSMenu *helpMenu = [NSApp helpMenu];
					itemCount	= [helpMenu numberOfItems];
					if ((itemIndex > 0) && (itemIndex <= itemCount))
					{
							NSMenuItem *item = [helpMenu itemAtIndex:(itemIndex-1)];
							[item setEnabled:true];
							result = qtrue;
					}
				}
			}
			
			rtnVal.setLong(result);
			hasRtnVal = qtrue;
			rtnCode = qtrue;
			break;
		}
            //ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  $disablehelpitem method  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ
#pragma mark $disablehelpitem
		case cIMethodId_DisableHelpItem:		//	$disablehelpitem(menuitemindex) Disables the menu item in the Help menu
		{
			qbool	result = qfalse;
			if (mIsLoaded)
			{
				EXTParamInfo* param1 = ECOfindParamNum(pEci, 1);
				if (param1)
				{
					EXTfldval fval1((qfldval)param1->mData);
					UInt16 itemIndex = fval1.getLong();
					NSMenu *helpMenu = [NSApp helpMenu];
					NSInteger itemCount	= [helpMenu numberOfItems];
					if ((itemIndex > 0) && (itemIndex <= itemCount))
					{
							NSMenuItem *item = [helpMenu itemAtIndex:(itemIndex-1)];
							[item setEnabled:false];
							result = qtrue;
					}
				}
			}
			rtnVal.setLong(result);
			hasRtnVal = qtrue;
			rtnCode = qtrue;
			break;
		}
            //ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  $sethelpitem method  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ
#pragma mark $sethelpitem
		case cIMethodId_SetHelpItem:		//	$sethelpitem(menuitemindex,menuitemname,shortcutkey,handledbystudio) Sets the Help menu item properties
		{
            UInt16	itemIndex = 0;
            UInt16	itemCount = 0;
			qbool   result = qfalse;
			
			if (mIsLoaded)
			{
				EXTParamInfo* param1 = ECOfindParamNum(pEci, 1);
				EXTParamInfo* param2 = ECOfindParamNum(pEci, 2);
				EXTParamInfo* param3 = ECOfindParamNum(pEci, 3);
				EXTParamInfo* param4 = ECOfindParamNum(pEci, 4);
				if ((param1) && (param2))
				{
					EXTfldval fval1((qfldval)param1->mData);
					itemIndex = fval1.getLong();
					NSMenu *helpMenu = [NSApp helpMenu];
					itemCount	= [helpMenu numberOfItems];
					if ((itemIndex > 0) && (itemIndex <= itemCount))
					{
							EXTfldval fval2((qfldval)param2->mData);
							strxxx &itemname = fval2.getChar();
							if (itemname.length() > 0)
							{
									CFStringRef titleRef = GetCFStrRef(itemname);
									NSMenuItem *item = [helpMenu itemAtIndex:(itemIndex-1)];
									[item setTitle:(__bridge NSString *)titleRef];
							}
						
						
							if (param3) {

									EXTfldval fval3((qfldval)param3->mData);
									strxxx &shortcutkey = fval3.getChar();
								
									if (shortcutkey.length() > 0)
									{
											shortcutkey.upps();
											CFStringRef shortcutRef = GetCFStrRef(shortcutkey);
											NSMenuItem *item = [helpMenu itemAtIndex:(itemIndex-1)];
											[item setKeyEquivalent:(__bridge NSString *)shortcutRef];
											CFRelease(shortcutRef);
									}
							}
							if (param4)
							{
									// haven't found an equivalent for RefCon in Cocoa menus yet
									//SetMenuItemRefCon(mOSXHelpDataRef->helpMenuRef, itemIndex, handledbyStudio);
							}
							result = qtrue;
					}
				}
			}
			
			rtnVal.setLong(result);
			hasRtnVal = qtrue;
			rtnCode = qtrue;
			break;
		}
            //ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  $gethelpitem method  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ
#pragma mark $gethelpitem
		case cIMethodId_GetHelpItem:		//	$gethelpitem(menuitemindex,menuitemname,shortcutkey,handledbystudio) Gets the Help menu item properties
		{
            UInt16	itemIndex = 0;
            UInt16	itemCount = 0;
			qbool   result = qfalse;
			char*	cString;
			
			if (mIsLoaded)
			{
				EXTParamInfo* param1 = ECOfindParamNum(pEci, 1);
				EXTParamInfo* param2 = ECOfindParamNum(pEci, 2);
				EXTParamInfo* param3 = ECOfindParamNum(pEci, 3);
				EXTParamInfo* param4 = ECOfindParamNum(pEci, 4);
				if ((param1) && (param2))
				{
					EXTfldval fval1((qfldval)param1->mData);
					itemIndex = fval1.getLong();
					
					NSMenu *helpMenu = [NSApp helpMenu];
					cString = (char*)MEMmalloc(256);
				 //NSInteger itemCount	= [[helpMenu itemArray] count];
					itemCount	= [helpMenu numberOfItems];
					if (cString &&  (itemIndex > 0) && (itemIndex <= itemCount))
					{
							NSMenuItem *item = [helpMenu itemAtIndex:(itemIndex-1)];
							NSString *title = [item title];
							if ([title getCString:cString maxLength:255 encoding:NSMacOSRomanStringEncoding]) {	// NO return if conversion not successful
								
									EXTfldval fval2((qfldval)param2->mData);
									SetCharU(&fval2, cString, strlen(cString));
								
									ECOsetParameterChanged(pEci,2);
							}
							if (param3)
							{
									NSString *cmdChar = [item keyEquivalent];
									if ([cmdChar getCString:cString maxLength:255 encoding:NSMacOSRomanStringEncoding]) {	// NO return if conversion not successful

											EXTfldval fval3((qfldval)param3->mData);
											SetCharU(&fval3, cString, 1);
										
											ECOsetParameterChanged(pEci,3);
									}
							}
							if (param4)
							{
									// haven't found an equivalent for RefCon in Cocoa menus yet
									//GetMenuItemRefCon(mOSXHelpDataRef->helpMenuRef, itemIndex, &handledbyStudio);
							}
							MEMfree((qchar*)cString);
					}
					
				}
			}
			
			rtnVal.setLong(result);
			hasRtnVal = qtrue;
			rtnCode = qtrue;
			break;
		}
            //ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  $counthelpitems method  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ
#pragma mark $counthelpitems
		case cIMethodId_CountHelpItems:		//	$counthelpitems() Returns the number of items in the Help menu
		{
			UInt16 	itemCount = 0;
            if (mIsLoaded) {
                NSMenu *helpMenu = [NSApp helpMenu];
                itemCount	= [helpMenu numberOfItems];
            }
			rtnVal.setLong(itemCount);
			hasRtnVal = qtrue;
			rtnCode = qtrue;
			break;
		}
            //ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  $clearhelpmenu method  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ
#pragma mark $clearhelpmenu
		case cIMethodId_ClearHelpMenu:	//	$clearhelpmenu() Clears the Help menu
		{
			qbool 		result = qfalse;
			SInt16 		itemCount;

			if (mIsLoaded)
			{
				NSMenu *helpMenu = [NSApp helpMenu];
				itemCount	= [helpMenu numberOfItems];
				for (SInt16 index=itemCount;index>0;index--)	{
						[helpMenu removeItemAtIndex:(index-1)];
				}
				result = qtrue;
			}

			rtnVal.setLong(result);
			hasRtnVal = qtrue;
			rtnCode = qtrue;
			break;
		}
// Start CR0584
#pragma mark $registerhelpbook
		case cIMethodId_RegisterHelpBook:		//	$registerhelpbook(helpbookpackagepath) Registers the Help book package
#pragma mark $unregisterhelpbook
		case cIMethodId_UnRegisterHelpBook:	//	$unregisterhelpbook(helpbookpackagepath) Unregisters the Help book package
#pragma mark $openhelp
		case cIMethodId_OpenHelp:		//	$openhelp() Opens the Apple Help viewer
#pragma mark $gotopage
		case cIMethodId_GotoPage:		//	$gotopage(bookname,pageurl,anchorname) Displays the specified page from the Help book
#pragma mark $gotoanchor
		case cIMethodId_GotoAnchor:	//	$gotoanchor(bookname,anchorname) Displays the Help book page containing the anchor
#pragma mark $search
		case cIMethodId_Search:		//	$search(bookname,searchstring) Searches the Help book page with the specified string
		{
			rtnVal.setLong(qfalse);
			hasRtnVal = qtrue;
			rtnCode = qtrue;
			break;
		}
#pragma mark $enablepreferences
		case cIMethodId_EnablePreferences:	//	$enablepreferences() Enables the Preferences menu item
#pragma mark $disablepreferences
		case cIMethodId_DisablePreferences:	//	$disablepreferences() Disables the Preferences menu item
#pragma mark $about
		case cIMethodId_AboutBox:	//	$about:$about() Opens the about box
		{
			rtnCode = qtrue;
			break;
		}
// End CR0584
	}
	
	if (hasRtnVal)
		ECOaddParam(pEci,&rtnVal);

	return rtnCode;
}
//////////////////////// End Object Methods //////////////////////////////
// End of file
