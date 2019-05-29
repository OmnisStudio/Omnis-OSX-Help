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

#define VERSION_MAJOR			1				// Major Version number - Update on MAJOR release builds  
#define VERSION_MINOR			0				// Minor Version number - Update on release builds



//////////////////////// Object Definitions //////////////////////////////
// object id for each object class
// define the object constant, object name resource id, flags, exflags
ECOobject Objects[cObject_Count] = 
{
	cObject_OSXHelp,								2000, 0, 0
};


// parameter definitions for object methods
// define the parameter resource id, parameter type, flags, exflags
ECOparam ObjectParams[] =
{
	7000, fftInteger, 0, 0,												//menu item index
	7001, fftCharacter, 0, 0,											//menu item title
	7002, fftCharacter, EXTD_FLAG_PARAMOPT, 0,							//menu item shortcut key
	7003, fftBoolean, EXTD_FLAG_PARAMOPT, 0,							//menu item handled by Studio
	7004, fftCharacter, 0, 0,											//help book pathname
	7005, fftCharacter, 0, 0,											//help book name
	7006, fftCharacter, EXTD_FLAG_PARAMOPT, 0,							//help book page url
	7007, fftCharacter, EXTD_FLAG_PARAMOPT, 0,							//help book anchor name
	7008, fftCharacter, 0, 0,											//help book name
	7009, fftCharacter, 0, 0,											//help book anchor name
	7010, fftCharacter, 0, 0,											//help book name
	7011, fftCharacter, 0, 0,											//search string
	7012, fftInteger, 0, 0,												//get menu item index
	7013, fftCharacter, EXTD_FLAG_PARAMALTER, 0,						//get menu item title
	7014, fftCharacter, EXTD_FLAG_PARAMALTER, 0,						//get menu item shortcut key
	7015, fftBoolean, EXTD_FLAG_PARAMALTER, 0,							//get menu item handled by Studio
}; 


// object methods
// define the object method resource id, return type, parameter count, parameter def index, flags, exflags
ECOmethodEvent ObjectFuncs[cIMethod_Count] = 
{
	cIMethodId_LoadHandler			, 6000, fftBoolean, 0, 0, 0, 0,
	cIMethodId_UnloadHandler		, 6001, fftBoolean, 0, 0, 0, 0,
	cIMethodId_Help					, 6002, 0, 0, 0, 0, 0,
	cIMethodId_AddHelpItem			, 6003, fftInteger, 3, &ObjectParams[1], 0, 0,
	cIMethodId_DeleteHelpItem		, 6004, fftBoolean, 1, &ObjectParams[0], 0, 0,
	cIMethodId_EnableHelpItem		, 6005, fftBoolean, 1, &ObjectParams[0], 0, 0,
	cIMethodId_DisableHelpItem		, 6006, fftBoolean, 1, &ObjectParams[0], 0, 0,
	cIMethodId_SetHelpItem			, 6007, fftBoolean, 4, &ObjectParams[0], 0, 0,
	cIMethodId_GetHelpItem			, 6008, fftBoolean, 4, &ObjectParams[12], 0, 0,
	cIMethodId_CountHelpItems		, 6009, fftInteger, 0, 0, 0, 0,
	cIMethodId_ClearHelpMenu		, 6010, 0, 0, 0, 0, 0,
	cIMethodId_RegisterHelpBook		, 6011, fftInteger, 1, &ObjectParams[4], 0, 0,
	cIMethodId_UnRegisterHelpBook	, 6012, fftInteger, 1, &ObjectParams[4], 0, 0,
	cIMethodId_OpenHelp				, 6013, fftInteger, 0, 0, 0, 0,
	cIMethodId_GotoPage				, 6014, fftInteger, 3, &ObjectParams[5], 0, 0,
	cIMethodId_GotoAnchor			, 6015, fftInteger, 2, &ObjectParams[8], 0, 0,
	cIMethodId_Search				, 6016, fftInteger, 2, &ObjectParams[10], 0, 0,
	cIMethodId_EnablePreferences	, 6017, 0, 0, 0, 0, 0,
	cIMethodId_DisablePreferences	, 6018, 0, 0, 0, 0, 0,
	cIMethodId_AboutBox				, 6019, 0, 0, 0, 0, 0	// CR0584
};
//////////////////////// End Object Definitions //////////////////////////////

//////////////////////// Generic Object Container //////////////////////////////
tqfContainer::tqfContainer(qobjinst pObjPtr)
{
	mObjPtr = pObjPtr;
	mObject = new tqfObj();
}

tqfContainer::tqfContainer(qobjinst pObjPtr,tqfContainer* pObjectContainer)
{
	mObjPtr = pObjPtr;
	mObject = pObjectContainer->mObject;
	if ( mObject )
		mObject->addRef();
}

tqfContainer::~tqfContainer()
{
	if ( mObject )
	{
		mObject->releaseRef();
		mObject = NULL;
	}
}

void tqfContainer::setObject(qobjinst pObjPtr,tqfContainer* pSource)
{
	if ( pSource->mObject )
		pSource->mObject->addRef();
	if ( mObject )
	{
		mObject->releaseRef();
		mObject = NULL;
	}	
	mObjPtr = pObjPtr;
	mObject = pSource->mObject;
}
//////////////////////// End Generic Object Container //////////////////////////////
	
//ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ  tqfObj::propertySupport  ÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙÙ

qlong tqfObj::propertySupport(LPARAM pMessage, WPARAM wParam, LPARAM lParam, EXTCompInfo* eci)
{
	// we don't have any properties
	return qfalse;
}


//////////////////////// Static Methods //////////////////////////////
#define MAX_PARAMS 	 6
qbool tqfStaticObj::staticMethod(EXTCompInfo *pEci)
{
	qlong funcId = ECOgetId(pEci);
	EXTParamInfo *param[MAX_PARAMS];
	for (qshort n = 1; n <= MAX_PARAMS; n++)
	{
		param[n-1] = ECOfindParamNum(pEci, n);
	}
	ECOgetParamCount(pEci); 
	qbool methodFound = qtrue;
	qlong resultVal = 0;
	str255 resultString;
	qreal realResultVal = 0.0;
	qbool isReal = qfalse;
	qbool isString =qfalse;
	switch (funcId)
	{
		case 0:		// Remove this line when you add the first static function (prevents compiler warning)
			break;
		default:
			methodFound = qfalse;
			break;
	}
	if (methodFound)
	{ // Push error code
		EXTfldval result;

		if (isString)
			result.setChar(resultString);
			//SetCharU(&result, resultString, strlen(resultString));
		else if (isReal) result.setNum(realResultVal); 
		else result.setLong(resultVal);
    ECOaddParam(pEci, &result);
	}
	return methodFound;
}
//////////////////////// End Static Methods //////////////////////////////


void getstudioparam(EXTParamInfo **pParams, int whichparam, char* result)
{
	qlong	paramlen;
	str255 	param;

	EXTfldval paramString((qfldval) pParams[whichparam]->mData);
	paramString.getChar(param);
	
	paramlen = 1;
	while (paramlen <= param.length())
    {
    	result[paramlen-1] = param[paramlen];
        paramlen++;
     }
     result[paramlen-1] = '\0';
}


//////////////////////////////////////////////////////
// component library entry point (name as declared in resource 31000)
extern "C" LRESULT OMNISWNDPROC OSXHelpProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam, EXTCompInfo* eci)
{
	 // initialize callback tables - THIS MUST BE DONE 
	ECOsetupCallbacks(hwnd, eci);
	
	switch (Msg)
	{
		// ECM_OBJCONSTRUCT - this is a message to create a new object.
		case ECM_OBJCONSTRUCT:				
		{
			if (eci->mCompId == cObject_OSXHelp)
			{
				tqfContainer* object = (tqfContainer*)ECOfindNVObject( eci->mOmnisInstance, lParam );
				if (!object)
				{
					tqfContainer* obj = new tqfContainer((qobjinst)lParam);
					ECOinsertNVObject(eci->mOmnisInstance, lParam, (void*)obj);
				}
				return qtrue;
			}
			return qfalse;
		}    
			
		// ECM_OBJDESTRUCT - this is a message to inform you to delete the object
		case ECM_OBJDESTRUCT:					
		{
			if (wParam == ECM_WPARAM_OBJINFO)
			{	
				void* object = ECOremoveNVObject(eci->mOmnisInstance, lParam);
				// Object found now figure out which object type it is
				if (eci->mCompId==cObject_OSXHelp)
				{
					tqfContainer* fileOps = (tqfContainer*)object;
					delete fileOps;
				}
			}	
			return qtrue;
		}
		
		case ECM_OBJECT_COPY:
		{
			objCopyInfo* copyInfo = (objCopyInfo*)lParam;
			tqfContainer* srcobj = (tqfContainer*)ECOfindNVObject(eci->mOmnisInstance, copyInfo->mSourceObject );
			if ( srcobj )
			{
				tqfContainer* destObj = (tqfContainer*)ECOfindNVObject(eci->mOmnisInstance, copyInfo->mDestinationObject );
				if ( !destObj )
				{
					destObj = new tqfContainer((qobjinst)copyInfo->mDestinationObject,srcobj);
					ECOinsertNVObject(eci->mOmnisInstance, copyInfo->mDestinationObject, (void*)destObj);
				}
				else
					destObj->setObject( (qobjinst)copyInfo->mDestinationObject,srcobj );
			}			
			break;
		}
		
	 	case ECM_CONNECT:
	    {   
		    return EXT_FLAG_LOADED|EXT_FLAG_ALWAYS_USABLE|EXT_FLAG_REMAINLOADED|EXT_FLAG_NVOBJECTS; // return external flags
		} 
		case ECM_DISCONNECT:
		{ 
			return qtrue;
		}	
		// ECM_GETCOMPLIBINFO - this is sent by OMNIS to find out the name of the library, and
		// the number of components this library supports
		case ECM_GETCOMPLIBINFO:
		{
			return ECOreturnCompInfo(gInstLib, eci, RES_LIB_NAME, 0);
		}
		case ECM_GETSTATICOBJECT:
		{
			// return ECOreturnMethods(gInstLib, eci, &StaticFuncs[0], cStaticMethodCount);
			return ECOreturnMethods(gInstLib, eci, NULL, cStaticMethodCount);
		}
		case ECM_GETOBJECT:
		{
			return ECOreturnObjects(gInstLib,eci, &Objects[0], cObject_Count);
		}
		case ECM_GETMETHODNAME:
		{
			if (eci->mCompId==cObject_OSXHelp)
				return ECOreturnMethods(gInstLib, eci, &ObjectFuncs[0], cIMethod_Count );
			break;	
		}
		case ECM_PROPERTYCANASSIGN:  	/* Object: Is the property assignable (ie readonly?) */
		case ECM_SETPROPERTY: 				/* Object: Assignment to a property */
		case ECM_GETPROPERTY:					/* Object: Retrieve value from property */
		{
			void* object = (void*)ECOfindNVObject(eci->mOmnisInstance, lParam);
			if ( object )
			{ // Object found now figure out which object type it is
				if (eci->mCompId == cObject_OSXHelp)
				{
					tqfContainer* instanceObj = (tqfContainer*)object;
					if (instanceObj->mObject)
						return instanceObj->mObject->propertySupport(Msg, wParam, lParam, eci);
				}
			}
			return 0L;
		}
		case ECM_METHODCALL:
		{
			void* object = (void*)ECOfindNVObject(eci->mOmnisInstance, lParam);
			if (object)
			{ // Object found now figure out which object type it is
				if (eci->mCompId == cObject_OSXHelp)
				{
					tqfContainer* instanceObj = (tqfContainer*)object;
					if (instanceObj->mObject)
						return instanceObj->mObject->methodCall(instanceObj->mObjPtr,eci);
				}	
			}
			else
				return tqfStaticObj::staticMethod(eci);
			return qfalse;
		}
		
 		case ECM_CONSTPREFIX:
		{
			EXTfldval exfldval;
			ECOaddParam(eci, &exfldval);
			str80 conPrefix; RESloadString(gInstLib, RES_CONST_PREFIX, conPrefix);

			exfldval.setChar(conPrefix);
			return qtrue;
		}
		case ECM_GETCONSTNAME:
		{
			return ECOreturnConstants(gInstLib, eci, RES_CONST_START, RES_CONST_END);
		}
		case ECM_GETVERSION:
		{
			return ECOreturnVersion(VERSION_MAJOR, VERSION_MINOR);
		}                                                                 
	}
	// As a final result this must ALWAYS be called. It handles all other messages that this component
	// decides to ignore.
	return WNDdefWindowProc(hwnd, Msg, wParam, lParam, eci);
}



// End of file
