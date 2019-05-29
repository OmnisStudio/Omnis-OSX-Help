/* $Header: svn://svn.omnis.net/trunk/Studio/O7EXE/INDY/basics.h 21627 2018-11-23 14:10:11Z pashworth $ */

/* Q4 basics.h 																										*/
/* the general definitions of basic types available throughout q4 */
/* some of these are machine dependent														*/
/* Copyright (C) Blyth Holdings Inc 1990													*/
/**************************************** Changes History **************************
Date			Edit				Bug					Description
15-Oct-18	CR0553			ST/BE/1497	Added OMNIS_VERSION.
18-Dec-17	rmm9591									Tidied up Scanf and made a few performance improvements.
04-Sep-17	rmm9474									Removed csettype and hptrlock (many changes, not marked).
23-Mar-17	rmm9295									Enabled basic alpha support for 8.1.
14-Feb-17	rmmheadless							Support for headless Omnis server.
16-Jan-17	rmm9199									Studio 9.0 HWND alpha support - excluded via conditional compilation for Studio 8.1.x.
25-Oct-16	rmm9123									Implemented $exportjson() and $importjson().
10-Oct-16	rmm9092									Performance improvements. Includes renaming concatTokenizedChars to concatUtf8Chars,and setTokenizedChars to setUtf8Chars.
25-Aug-16	rmm9018									Miscellaneous tidying up - removed some non-Unicode code for example.
19-Aug-16	rmm9010			ST/EC/1416	Added support for large files (files with a size > maximum signed 32 bit integer).
18-Jul-16	rmm8982									Responsive design for JS client remote forms.
29-Apr-16	rmm8899									Removed old q q qs so we can see what needs to be tidied up.
07-Mar-16	CR0333			ST/BE/1236	Cocoa performance when loading large library is slow.
09-Feb-15	rmm8503			ST/HE/1254	Cocoa top-level window ordering issues.
27-Mar-14	rmm_rws									RESTful Web Services support: server
16-Sep-13	CRCOCOA									Cocoa target
19-Aug-13	rmm64bitux							Linux 64 bit port.
07-Nov-12	rmm_xc45								Changes to allow Xcode 4.5+ to be used for OSX build.
26-Apr-12	rmm64bit2								64 bit portability changes.
17-Apr-12	rmm64bit1								First set of changes for 64 bit support: 64 bit Omnis data type.
04-Jan-11	rmm7050									OLE/automation issues.
09-Dec-10	rmm7023									JavaScript client fonts and styles.
09-Sep-10	rmm_jsc									JavaScript client core changes.
26-Aug-09	gra0627									Linux compile issue
10-Feb-09	rmm_iphone6							Sorted out core support for iPhone font table etc.
30-Jan-09	rmm_iphone4							iPhone save and restore application state.
14-Aug-08	pkiphone								iphone pass 1
08-May-08	rmm_handle							Implemented platform independent handle classes as part of mobile device support.
18-Feb-08	rmm_mobile							Mobile device support.
31-Dec-07	rmm6290									Need to hide certain web client classes when building on Leopard, to avoid linker/loader issues.
22-May-06	rmm5786									Improved internal error reporting.
03-Mar-06 MHMACH17                Ensure that orfcexec symbols are hidden on Mach-O
18-Jan-06 MHMACH7                 orfcexec mach-o changes
07-Dec-05 MHMACH4                 Mach-O changes.
02-Dec-05	rmm5542									Problem converting a list from Unicode to non-Unicode.
27-Sep-05 MHMACH                  Mach-o changes
14-Apr-05	rmm5332									Import definitions for orfcexec were incorrect.
17-Feb-05	rmm5275									Added error diagnostic calls.
02-Jul-04 MHCW9										CW9 changes.
13-Feb-04	rmmuni_osx6							Mac OSX Unicode port.
24 JUN 02 MHJCORE									Java Support for Studio 4
27 JUN 01 mpm4733									moved qapp typedef here from extcrb.he
20 OCT 00 mpmCarbon								OSX Changes NOT MARKED
19-May-00	rmm3764									Implemented thread local storage for multi-threaded server.
15-Dec-99	rmm3568									Added meta-data methods to new ODBC DAM.
17 NOV 99 mpmCarbon5							More Carbon changes
07-Oct-99	DRS5100									Client method execution for ORFC
28-Jun-99 MHUX001                 Unix Changes.
17-Aug-98 PK4778									Added new defines for RCC
02 Apr 98 mt40449									use align power not powerc
26-Jan-98	mt40428									now have SET_CORE_PACKING && SET_BYTE_PACKING
05-Jan-98	AE4675									Win16: GPF when calling the device function	(added OMNISPRIPROC)
14-Oct-97	mt40360									Introduced qint data-type for general purpose use.
14-Oct-97	mt40358									Removed Temporay Objects constructions
11-Jul-96	mt40310									changes to mt40295 coz of MFC
12-Jun-96	mt40293									PPC now uses doubles instead of long doubles
09-Jun-96	mt40288									for 68000 use an unsigned short
27-Nov-96	DRS4216									Remove csetBad from non-debug versions
04-Nov-96	mt40046									68k code frag (#define for pragmas)
19-Jul-96	AE_STY									Cross platform styles for V4
07-Feb-96	rmm_cset								Added csettype and its values.
06-Mar-95	I3_CSB_0001 only include csbbasic.h if INCLUDE_CSB is defined
24-Feb-95	I3_CSB_0000 include csbbasic.h, added qcharw UnicChar
14-Dec-93	grcW32
25-11-93		ACJS309a more connects jigging.
08-11-93		ACJS309 Jiged for connects.
18/11/92 		pewl33 	rstrno changed to qlong from qshort for large error numbers
17-10-91		ACJ0010	Created from xgstuff.h
	
***********************************************************************************/
 
#ifndef _basics_
#define _basics_

// Start rmm64bit1
#if defined(iswin32) && !defined(isunix) && _MSC_VER < 1700
	#include "xstdint.h"
#else
	#include <stdint.h>
#endif
// End rmm64bit1
#include <string.h> // rmm64bitux

#define OMNIS_VERSION 10000	// CR0553

// Family Codes
// rmm_iphone6: Not safe to have these values the same as platform codes. Fortunately they are not used: #define OS_FAMILY_APPLE					'A'
// rmm_iphone6: #define OS_FAMILY_MICROSOFT			'M'
// rmm_iphone6: #define OS_FAMILY_IBM						'I'
#define OS_FAMILY_UNIX					'U'

// Platform Codes
#define PLATFORM_MACINTOSH			'M'
#define PLATFORM_POWERMACINTOSH	'P'
#define PLATFORM_MACOSX					'X'
#define PLATFORM_WINDOWS				'W'
#define PLATFORM_NT							'N'
#define PLATFORM_OS2						'S'
#define PLATFORM_HP							'H'
#define PLATFORM_AIX						'A'
#define PLATFORM_SOLARIS				'L'
#define PLATFORM_WINDOWSMOBILE	'C'	// rmm_mobile: Windows 'C'E
#define PLATFORM_IPHONE					'I'	// rmm_iphone6
#define PLATFORM_JAVASCRIPT			'J'	// rmm_jsc
#if !defined(OCX_BUILD) && !defined(isRCC) // rmm7050
	#define PLATFORM_UNKNOWN				' '	// rmm_mobile
	#define PLATFORM_RESTFUL				'R'	// rmm_rws
#endif

// rmm9474: Removed csettype and hptrlock (many changes, not marked)
  
/* basic types */
	// Start rmm64bit1
	typedef 					int16_t 		qshort;     /* Signed integer type, assumed to hold -32k to +32k at least																				*/
	typedef 					int32_t			qlong;      /* Signed long type																																									*/ 
	typedef						uint32_t 		qulong;   	/* Unsigned long, for use of datalen MPM convert																										*/
	typedef						int64_t			qlong64;		/* Signed 64 bit integer type */
	typedef						uint64_t		qulong64;		/* Unsigned 64 bit integer type */
	typedef						intptr_t		qlongptr;		// rmm64bit2: Signed integer data type suitable for casting to and from a pointer, and for use as a generic 32 or 64 bit data type as appropriate
	typedef						uintptr_t		qulongptr;	// rmm64bit2: Unsigned integer data type suitable for casting to and from a pointer, and for use as a generic 32 or 64 bit data type as appropriate
	typedef						qlongptr		dmrfn;			// rmm64bit2: Data manager file number - sometimes contains a pointer, hence the pointer type
	#ifdef is64bit
		typedef					int64_t			qlongarch;	// rmm64bit2: Integer data type that matches the current architecture (useful for C runtimes)
	#else
		typedef					int32_t			qlongarch;	// rmm64bit2: Integer data type that matches the current architecture (useful for C runtimes)
	#endif
	
	typedef						uint32_t		qchar;
	// End rmm64bit1
	#ifdef isunix
		#undef qoschar //gra0627
		typedef char qoschar;		// AESUNI
	#else
		typedef uint16_t qoschar;			// rmmuni: Operating System API character // rmm64bit1
	#endif
  #ifdef ismacosx
	  #define _TEXT(_a) ((qoschar *) (L ## _a))	// rmmuni_osx6
  #endif
  #if defined(ismobile) && defined(iswin32)
	  #define QTEXT(_a) _T(_a) // rmm_mobile
  #else
	  #define QTEXT(_a) _TEXT(_a) // AEUNI
  #endif
  #define QOSTEXT(_a) (_a)										// rmmuni: used for text which is already qoschar
  #define	QBYTELEN(_a) ((_a)*sizeof(qchar))
  #define QCHARLEN(_a) ((_a)/sizeof(qchar))
  #define	QOSBYTELEN(_a) ((_a)*sizeof(qoschar))
  #define QOSCHARLEN(_a) ((_a)/sizeof(qoschar))

	#define QOSCHAR(_a) ((qoschar *) (_a))			// used for static text to qoschar
	typedef						uint16_t 		qcharw;			/* Wide character I3_CSB_0000 */ // rmm64bit1
	typedef 					qcharw 		 	UnicChar;		/* Unicode character I3_CSB_0000 */
	typedef unsigned 	char	 			qbyte;      /* Unsigned single byte integer, assumed to hold 0..255 at least 																		*/
	typedef 					qbyte   		qbool;      /* Boolean, often used for function returns 																												*/
#if defined(__CONNECT__)
	#define 					qfalse			0						/* Defined FALSE																																										*/
	#define 					qtrue				1						/* Defined TRUE																																											*/
#else
	const   					qbool 			qfalse=0;		/* Defined FALSE																																										*/
	const           	qbool 			qtrue=1;		/* Defined TRUE																																											*/
#endif

//grcW32, mt40293
#if defined(iswin32) || defined(powerc)
	typedef 					double	qreal;
#else
	typedef 					long double	qreal;      /* Real type, used for all real arithmetic 																													*/
#endif

	typedef						uint16_t 		qushort; 		/* 2 byte unsigned integer																																					*/  // rmm64bit1
	typedef 					qshort  		qret;       /* Used for function returns, 0 usually means ok 																										*/
	enum            	qniltype 		{qnil=0};   /* Assigning qnil to something causes it to become empty 																						*/
	typedef						qlong64			qint8;			// rmm64bit2: 8 byte integer (as stored on disk)
	typedef 					qlong 			qint4;      /* 4 byte integer (as stored on disk) 																															*/	// rmm64bit1
	typedef 					qshort 			qint2;      /* 2 byte integer (as stored on disk)																																*/  // rmm64bit1
	typedef						qushort 		qword2;  		/* 2 byte unsigned integer (as stored on disk) 																											*/  // rmm64bit1
	typedef						qulong 			qword4;   	/* 4 byte unsigned integer (as stored on disk) MPM 1.11 																						*/	// rmm64bit1
	typedef 					qbyte 			qint1;      /* 1 byte unsigned integer (as stored on disk)																											*/
	typedef 					qlong 			rstrno;  		/* Used to hold resource string numbers, passed to rstr(), mess(), etc. Changed to qlong for pewl33 */
	typedef 					qshort 			attnumber;	/* An attribute  DRSyDAT 																																						*/
	typedef						qshort			preconst;		// rmm3568: identifies a predefined k... constant
 
#if defined(ismac) && !defined(powerc)
	// i.e. mt40288 68000
 	typedef unsigned	short				quint;			/* unsigned integer / main use is loop counts %%%% STE */
#else
 	typedef unsigned	int					quint;			/* unsigned integer / main use is loop counts %%%% STE */
#endif

#ifdef powerc
 	typedef 					int					qint;				/* mt40360 general purpose data type */
#else
 	typedef 					qshort			qint;				/* mt40360 general purpose data type */	
#endif
//  mt40046 - start
#if defined(iswin16) || defined(iswin32)
	#ifdef iswin16
		#define OMNISAPI _export _pascal
		#define OMNISPRIPROC _far _export _pascal
		// AEUNI Defines missing from VC1.52 required for cross character set dev.
		#define LPTSTR  LPSTR
		#define LPCTSTR LPCSTR
	#else
		#define OMNISAPI
		#define OMNISPRIPROC
	#endif

	#define SET_RETURN_REGISTER_ON
	#define SET_RETURN_REGISTER_OFF

	#define SET_BYTE_PACKING_ON						pack(1)
	#define SET_BYTE_PACKING_OFF					pack()
	#ifdef isunix
		//#define GCC_SET_BYTE_PACKING_ON _Pragma("pack(1)") // MHMACH
		//#define GCC_SET_BYTE_PACKING_OFF _Pragma("pack()") // MHMACH
		#define GCC_SET_BYTE_PACKING_ON 
		#define GCC_SET_BYTE_PACKING_OFF
	#else
		#define GCC_SET_BYTE_PACKING_ON  // MHMACH
		#define GCC_SET_BYTE_PACKING_OFF // MHMACH	
	#endif

	// mt40428
	#ifdef iswin16
		#define SET_CORE_PACKING_ON					pack(1)
	#else
		#define SET_CORE_PACKING_ON					pack(2)
		#ifdef isunix
			//#define GCC_SET_CORE_PACKING_ON _Pragma("pack(2)") // MHMACH
			#define GCC_SET_CORE_PACKING_ON  // MHMACH
		#else
			#define GCC_SET_CORE_PACKING_ON  // MHMACH		
		#endif
	#endif
	#define SET_CORE_PACKING_OFF					pack()
	#ifdef isunix
		//#define GCC_SET_CORE_PACKING_OFF _Pragma("pack()") // MHMACH
		#define GCC_SET_CORE_PACKING_OFF  // MHMACH
	#else
		#define GCC_SET_CORE_PACKING_OFF  // MHMACH	
	#endif
#else
	#define OMNISAPI
	#define OMNISPRIPROC

	// mt40428
	// MHMACH begins
	#ifdef ismach_o // mpm_cw
		// rmm_xc45: pack(2) is meant to be equivalent to align=mac68k (not 100% certain this does byte packing, but it works with our disk structures)
		#define GCC_SET_BYTE_PACKING_ON				_Pragma("pack(2)")
		#define GCC_SET_BYTE_PACKING_OFF			_Pragma("pack()")
		#define SET_BYTE_PACKING_ON						ignore
		#define SET_BYTE_PACKING_OFF					ignore
	#else
		#define GCC_SET_BYTE_PACKING_ON 
		#define GCC_SET_BYTE_PACKING_OFF 
	  #define SET_BYTE_PACKING_ON				 	options align=mac68k
	  #define SET_BYTE_PACKING_OFF				options align=reset
	#endif
	// MHMACH ends
	
// PKMACCLIENT
#if defined(isRCCLIB) || defined(isRCC) || defined (isEXECLIB)   //DRS5100
	#define HGLOBAL									qlongptr	//CRCOCOA
	#define DFAR
#endif

#ifndef isXCOMPLIB
	#ifdef powerc
		#define SET_RETURN_REGISTER_ON ignore // MHCW9
		
		#define SET_RETURN_REGISTER_OFF ignore // MHCW9

	// mt40428
		// MHMACH begins
		#ifdef ismach_o // mpm_cw
			#define GCC_SET_CORE_PACKING_ON _Pragma("options align=power")
			#define SET_CORE_PACKING_ON	ignore
			//#define GCC_SET_CORE_PACKING_ON _Pragma("pack(2)")
			//#define SET_CORE_PACKING_ON				pack(2)
		#else
			#define GCC_SET_CORE_PACKING_ON 
		  #define SET_CORE_PACKING_ON				options align=power
		#endif
		// MHMACH ends
	#else
		#define SET_RETURN_REGISTER_ON 	 	pointers_in_D0
		#define SET_RETURN_REGISTER_OFF  	pointers_in_A0

	// mt40428
		#define SET_CORE_PACKING_ON				options align=mac68k
	#endif		
#endif

	// MHMACH begins
	#ifdef ismach_o // mpm_cw
		#define GCC_SET_CORE_PACKING_OFF _Pragma("options align=reset")
		#define SET_CORE_PACKING_OFF ignore
		//#define GCC_SET_CORE_PACKING_OFF _Pragma("pack()")
		//#define SET_CORE_PACKING_OFF					pack()
	#else
		#define GCC_SET_CORE_PACKING_OFF 
	  #define SET_CORE_PACKING_OFF				options align=reset
	#endif
	// MHMACH ends

#ifdef isCOCOA	//CRCOCOA
	typedef qlongptr									( * CProcPtr )();
	#define FARPROC										CProcPtr
#elif defined(iscarbon)
	#define	FARPROC										ProcPtr
#else
	#define	FARPROC										UniversalProcPtr
#endif

	// PKMACCLIENT
#if defined(ismobile) && defined(ismacosx)	// pkiphone
#define HINSTANCE								int
#define HDC										int
#else
	struct	HINSTANCEstruct;
	#define HINSTANCE								HINSTANCEstruct*
#endif
/*
	#ifdef isRCCLIB
		struct	HINSTANCEstruct;
		#define HINSTANCE								HINSTANCEstruct*
	#else
	#ifndef isXCOMPLIB
		#define HINSTANCE									long				// mt403010
	#endif
	#endif
	*/

#endif
//  mt40046 - end



#if defined(iswin32)
	#define OMNIS_IMPORT dllimport
	#define OMNIS_EXPORT dllexport
#elif defined(ismac)
	#define OMNIS_IMPORT import
	#define OMNIS_EXPORT export
#endif
// PK4778

#ifdef isOMNISCORE
	#define RCCAPI
	#define GDIAPI
	#define HWNDAPI
	#define GDIPICTAPI
	#define CRBAPI
	#define PRIAPI
	#define ECOAPI
	#define BMPAPI
	// rmm3764: define OMTLS for a multi-threaded core which uses Thread Local Storage
	#if defined(iswin32) && !defined(OMODBC)
		#define OMTLS
	#endif

	// Start rmm5275:
	#ifdef __cplusplus
		class strxxx;	// rmm5786
		extern void	gReportInternalError(const char *pModule, qlong pLocationCode, qlong pExtraInfo = 0, const strxxx *pExtraString = 0);	// rmm5786
	#endif
	#define OM_INTERNAL_ERROR(_module, _code) gReportInternalError(_module, _code)
	#define OM_INTERNAL_ERROR2(_module, _code, _info) gReportInternalError(_module, _code, _info)
	#define OM_INTERNAL_ERROR3(_module, _code, _infostring) gReportInternalError(_module, _code, 0, _infostring) // rmm5786
	// To remove internal error reporting, use the following instead of the above definition of OM_INTERNAL_ERROR and OM_INTERNAL_ERROR2:
	// #define OM_INTERNAL_ERROR(_module, _code)
	// #define OM_INTERNAL_ERROR2(_module, _code, _info)
	// End rmm5275

#elif defined(isGDIPICTLIB)
	// MHUX001 begins
	#ifdef isunix
		#define RCCAPI
		#define GDIAPI
		#define HWNDAPI
		#define CRBAPI
		#define PRIAPI
		#define ECOAPI
		#define BMPAPI

		#define GDIPICTAPI __declspec(OMNIS_EXPORT)
	#else
		#define RCCAPI __declspec(OMNIS_IMPORT)
		#define GDIAPI __declspec(OMNIS_IMPORT)
		#define HWNDAPI __declspec(OMNIS_IMPORT)
		#define CRBAPI __declspec(OMNIS_IMPORT)
		#define PRIAPI __declspec(OMNIS_IMPORT)
		#define ECOAPI __declspec(OMNIS_IMPORT)
		#define BMPAPI __declspec(OMNIS_IMPORT)

		#define GDIPICTAPI __declspec(OMNIS_EXPORT)
	#endif
	// MHUX001 ends
#elif defined(isGDIHWNDLIB)
	// MHUX001 begins.
	#if defined(isunix) || defined(ismach_o) // MHMACH4
		// import
		#define RCCAPI
		#define CRBAPI
		#define PRIAPI
		#define ECOAPI
		// export
		// MHMACH4 begins
		#ifdef ismach_o
			#define GDIAPI __attribute__((visibility("default")))
			#define HWNDAPI __attribute__((visibility("default")))
			#define GDIPICTAPI __attribute__((visibility("default")))
			#define BMPAPI __attribute__((visibility("default"))) // MHMACH7
		#else
			#define GDIAPI
			#define HWNDAPI
			#define GDIPICTAPI
			#define BMPAPI // MHMACH7
		#endif
		// MHMACH4 ends
	#else
		// import
		#define RCCAPI __declspec(OMNIS_IMPORT)
		#define CRBAPI __declspec(OMNIS_IMPORT)
		#define PRIAPI __declspec(OMNIS_IMPORT)
		#define ECOAPI __declspec(OMNIS_IMPORT)
		#define BMPAPI __declspec(OMNIS_IMPORT)
		// export
		#define GDIAPI __declspec(OMNIS_EXPORT)
		#define HWNDAPI __declspec(OMNIS_EXPORT)
		#define GDIPICTAPI __declspec(OMNIS_EXPORT)
	#endif
	// MHUX001 ends.
#elif defined(isRCCLIB)
	// MHUX001 begins.
	#if defined(isunix) || defined(ismach_o) // MHMACH4
		#define GDIAPI
		#define HWNDAPI 
		#define GDIPICTAPI 
		#define PRIAPI 

		// MHMACH4 begins
		#ifdef ismach_o
			#define RCCAPI __attribute__((visibility("default"))) 
			#define CRBAPI __attribute__((visibility("default")))
			#define ECOAPI __attribute__((visibility("default")))
			#define BMPAPI __attribute__((visibility("default")))
      #define EXECAPI __attribute__((visibility("default"))) //MHMACH17
		#else
			#define RCCAPI 
			#define CRBAPI 
			#define ECOAPI 
			#define BMPAPI
  		#define EXECAPI  //DRS5100 // MHMACH17
		#endif
		// MHMACH4 ends
	#else
		#define GDIAPI __declspec(OMNIS_IMPORT)
		#define HWNDAPI __declspec(OMNIS_IMPORT)
		#define GDIPICTAPI __declspec(OMNIS_IMPORT)
		#define PRIAPI __declspec(OMNIS_IMPORT)
		#define EXECAPI __declspec(OMNIS_IMPORT)				//DRS5100

		#define RCCAPI __declspec(OMNIS_EXPORT)
		#define CRBAPI __declspec(OMNIS_EXPORT)
		#define ECOAPI __declspec(OMNIS_EXPORT)
		#define BMPAPI __declspec(OMNIS_EXPORT)
	#endif
	// MHUX001 ends.

	enum listtype {       //basic list types //DRS2102
      listFlen0=0,                  //fixed length data with a variable field position crb (isflen is false)
      listVlen=1,                   //variable length data
      listScol=2,                  //simple single text column
			listFlen=3,										//fixed length data with a fixed field position crb (isflen is true)
			listSmart=4										//smart list //DRS4051
    };  


#elif defined(isEXECLIB)							//DRS5100
	// MHUX001 begins.
	#if defined(isunix) || defined(ismach_o) // MHMACH4
		#define PRIAPI
		#define GDIAPI
		#define HWNDAPI
		#define GDIPICTAPI
		
		// MHMACH7 begins
		#ifdef ismach_o
			#define RCCAPI __attribute__((visibility("default"))) 
			#define CRBAPI __attribute__((visibility("default")))
			#define ECOAPI __attribute__((visibility("default")))
			#define BMPAPI __attribute__((visibility("default")))
      #define EXECAPI __attribute__((visibility("default"))) // MHMACH17
		#else
			#define RCCAPI 
			#define CRBAPI 
			#define ECOAPI 
			#define BMPAPI
      #define EXECAPI // MHMACH17
		#endif
		// MHMACH7 ends
		
#elif defined(ismobile) && defined(ismacosx)	// pkiphone
		#define PRIAPI
		#define GDIAPI
		#define HWNDAPI
		#define GDIPICTAPI
		#define RCCAPI 
		#define CRBAPI 
		#define ECOAPI 
		#define BMPAPI
		#define EXECAPI
		#define HWND int
		#define LPARAM long
		#define WPARAM long	// rmm_iphone4: was short
		#define UINT unsigned int

	#else
		// import
		#define RCCAPI __declspec(OMNIS_IMPORT)
		#define CRBAPI __declspec(OMNIS_IMPORT)
		#define PRIAPI __declspec(OMNIS_IMPORT)
		#define ECOAPI __declspec(OMNIS_IMPORT)
		#define BMPAPI __declspec(OMNIS_IMPORT)
		#define GDIAPI __declspec(OMNIS_IMPORT)					// rmm5332: these were all OMNIS_EXPORT
		#define HWNDAPI __declspec(OMNIS_IMPORT)				// rmm5332
		#define GDIPICTAPI __declspec(OMNIS_IMPORT)			// rmm5332
		// export
		#define EXECAPI __declspec(OMNIS_EXPORT)
	#endif

	enum listtype {       //basic list types //DRS2102 //DRS5100
      listFlen0=0,                  //fixed length data with a variable field position crb (isflen is false)
      listVlen=1,                   //variable length data
      listScol=2,                  //simple single text column
			listFlen=3,										//fixed length data with a fixed field position crb (isflen is true)
			listSmart=4										//smart list //DRS4051
    };  

#elif defined(isRCC)
	// MHUX001 begins.
	#if defined(isunix) || defined(ismach_o) // MHMACH4
		#define RCCAPI
		#define GDIAPI
		#define HWNDAPI
		#define GDIPICTAPI
		#define CRBAPI
		#define PRIAPI
		#define ECOAPI
		#define BMPAPI
		#define isRCCLIB
	#else
		#define RCCAPI __declspec(OMNIS_IMPORT)
		#define GDIAPI __declspec(OMNIS_IMPORT)
		#define HWNDAPI __declspec(OMNIS_IMPORT)
		#define GDIPICTAPI __declspec(OMNIS_IMPORT)
		#define CRBAPI __declspec(OMNIS_IMPORT)
		#define PRIAPI __declspec(OMNIS_IMPORT)
		#define ECOAPI __declspec(OMNIS_IMPORT)
		#define BMPAPI __declspec(OMNIS_IMPORT)
		#define isRCCLIB
	#endif
	// MHUX001 ends.

	enum listtype {       //basic list types //DRS2102
      listFlen0=0,                  //fixed length data with a variable field position crb (isflen is false)
      listVlen=1,                   //variable length data
      listScol=2,                   //simple single text column
			listFlen=3,										//fixed length data with a fixed field position crb (isflen is true)
			listSmart=4										//smart list //DRS4051
    };  
		
#elif defined(isXCOMPLIB)
	#define RCCAPI
	#define GDIAPI
	#define HWNDAPI
	#define GDIPICTAPI
	#define CRBAPI
	#define PRIAPI
	#define ECOAPI
	#define BMPAPI
	// MHJCORE begins
	#ifdef isunix
		#define JCAPI
	#else
		// MHMACH4 begins
		#ifdef ismach_o
			#define JCAPI __attribute__((visibility("default")))
		#else
		  #define JCAPI __declspec(OMNIS_EXPORT) 
	  #endif
		// MHMACH4 ends
	#endif
	// MHJCORE ends

	enum listtype {       //basic list types //DRS2102 //DRS5100
      	listFlen0=0,                  //fixed length data with a variable field position crb (isflen is false)
        listVlen=1,                   //variable length data
        listScol32=2,                   // pkcocoa_1050 simple single text column
				listFlen=3,										//fixed length data with a fixed field position crb (isflen is true)
				listSmart=4,										//smart list //DRS4051
				listScol64=5									//rmm64bit3: pkcocoa_1050 Single text column with 64 bit mark
  };


//pkcocoa_1050
#ifdef is64bit
#define listScol listScol64
#else
#define listScol listScol32
#endif

#endif


#if defined(isXCOMPLIB) || defined(isRCC)

	struct locptype;

	// Start rmm_handle
	typedef struct  
	{
		qulong  mLength;
		qulong  mDataLen;
		qbyte   *mData;
	} qHandleInfo;

	typedef	qHandleInfo *qHandle;
	// End rmm_handle

	#ifndef _xbasics_he_
		#include "xbasics.he"
	#endif

	// Start rmm9591
	inline void MEMmemFillByte(void *pDst, qint4 lNum, qbyte cCh)
	{
		memset(pDst, cCh, lNum);
	}
	#define fillc MEMmemFillByte

	inline void MEMmovel(const void *pSrc, void *pDst, qint4 lNum)
	{
		memcpy(pDst, pSrc, lNum);
	}
	#define movel MEMmovel

	inline void MEMmover(const void *pSrc, void *pDst, qint4 lNum)
	{
		memmove(pDst, pSrc, lNum);
	}

	inline int MEMmemcmp(const void * ptr1, const void * ptr2, size_t num)
	{
		return memcmp(ptr1, ptr2, num);
	}
	// End rmm9591
#else
	struct qHandleInfo;
	#ifndef __CONNECT__
		// rmm_handle: one typedef for all platforms
		typedef	struct qHandleInfo *qHandle; // MHMACH
	#endif
#endif

// rmm64bitux: Deal with issues caused by casting result of memcmp to qshort
#ifdef __cplusplus
  inline qshort smemcmp(const void *a1, const void *a2, qlong pLen)
  {
	  int cmp = memcmp(a1, a2, pLen);
	  if (cmp > 0) return 1;
	  else if (cmp < 0) return -1;
	  return 0;
  }
#endif

// Start rmmuni:
// MHCW9 begins
#ifdef ismach_o // MHMACH
		#define charEqual(_a,_b,_c) memcmp((_a),(_b),(unsigned long)QBYTELEN(_c))
#else
  #if (__MWERKS__ >= 0x3200) 
  	#define charEqual(_a,_b,_c) memcmp((_a),(_b),(unsigned long)QBYTELEN(_c))
  #else
  	#define charEqual(_a,_b,_c) memcmp((_a),(_b),QBYTELEN(_c))
  #endif
#endif // MHMACH
// MHCW9 ends
extern GDIAPI qshort charCompare(const qchar *add0, const qchar *add1, qlong len);
// End rmmuni

// mpm4733
#ifdef __cplusplus
class tqappfile;
typedef tqappfile*	qapp;				// pointer to OMNIS application

#include "chrbasic.he"	// rmmuni: only ever included here
#endif

// MHMACH4 begins
#ifdef ismach_o
	#define GCC_EXPORT_ON _Pragma("GCC visibility push(default)")
	#define GCC_EXPORT_OFF _Pragma("GCC visibility pop")
	#define GCC_HIDDEN_ON _Pragma("GCC visibility push(hidden)")	// rmm6290
	#define GCC_HIDDEN_OFF _Pragma("GCC visibility pop")					// rmm6290
#else
	#define GCC_EXPORT_ON 
	#define GCC_EXPORT_OFF 
	#define GCC_HIDDEN_ON			// rmm6290
	#define GCC_HIDDEN_OFF		// rmm6290
#endif
// MHMACH4 ends

typedef qshort qlayoutBreakpointValue;	// rmm8982
enum eWebClientType { eWebClientNone = -1, eWebClientPlugin = 0, eWebClientiPhone = 1, eWebClientJavaScript = 2 };	// rmm_jsc: Different types of client for remote form // rmm7023
enum eLayoutType { eLayoutTypeNone = -1, eLayoutTypeScreen = 0, eLayoutTypeSingle = 1, eLayoutTypeResponsive = 2};	// rmm8982: Layout types for JSC remote forms // rmm9123: added eLayoutTypeNone

// Start rmm8503
#ifdef isCOCOA
	#ifdef isdebug
		#define COCOALOG1(_a) NSLog((_a))
		#define COCOALOG2(_a,_b) NSLog((_a), (_b))
		#define COCOALOG3(_a,_b,_c) NSLog((_a), (_b), (_c))
		#define COCOALOG4(_a,_b,_c,_d) NSLog((_a), (_b), (_c), (_d))
		#define COCOALOG5(_a,_b,_c,_d,_e) NSLog((_a), (_b), (_c), (_d), (_e))
	#else
		#define COCOALOG1(_a)
		#define COCOALOG2(_a,_b)
		#define COCOALOG3(_a,_b,_c)
	#endif
#endif
// End rmm8503

// Start CR0333
#ifdef isCOCOA
typedef pthread_key_t threadStorageIndex;
#else
typedef qulong threadStorageIndex;
#endif
// end CR0333

// rmm8982: snprintf is not available for Windows until VS2015
#if defined(iswin32) && !defined(isunix) && _MSC_VER < 1900
	#define snprintf sprintf_s 
#endif

// Start rmm9010: Moved these to here, so they are defined in just one place
// Maximum number of characters in a character value
const qlong maxcharlen = 100000000;
// Maximum number of bytes in a binary value
const qlong maxbinlen = 0x7ffffff0; 
// End rmm9010

#define isstudio9	// rmm9295: For now, just define this - once we are complete happy with this feature, we can re-work the code to not require this symbol
// Start rmm9199
#if defined(isstudio9) && !defined(isunix) && !defined(isheadless) // rmmheadless
	#define isalphasystem
#endif
// End rmm9199
// Start rmmheadless
#if defined(isunix) && !defined(isheadless)
	#define isunixgui
#endif
// End rmmheadless

#endif
