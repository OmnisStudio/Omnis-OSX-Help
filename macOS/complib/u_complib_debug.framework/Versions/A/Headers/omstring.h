/* $Header: svn://svn.omnis.net/trunk/Studio/O7EXE/INDY/omstring.h 18211 2017-10-11 07:10:55Z bmitchell $ */

/* Q4 omstring.h
Header file for string and memory runtime functions
Copyright (C) Raining Data 2002

Changes
Date			Edit				Bug					Description
05-Aug-16	rmm8993									Made OMstrtok thread-safe by adding an extra argument.
24-Dec-13	rmm8189									Removed gra0948 as the Mac core build was broken by this.
23-Dec-13	gra0948									pkiphone breaks JDBC DAM
17-Apr-12	rmm64bit1								First set of changes for 64 bit support: 64 bit Omnis data type.
14-Aug-08	pkiphone								iphone pass 1
28-Feb-06	rmm5654									MachO Unicode compile.
08-Dec-05	rmm_macho								Unicode functions implemented (because qoschar and wchar_t are 2 and 4 bytes respectively in Unicode builds)
13-Feb-04	rmmuni_osx6							Mac OSX Unicode port.
22-May-02	rmm											New file.
*/
#ifndef _OMSTRING_H_
#define _OMSTRING_H_

#ifndef _basics_
	// For the core and components, this file is included by chrbasic.he, which in turn is included by basics.h
	// If basics.h has not been used, this file is being used in an old Omnis external - in this case, we need
	// some extra definitions here.
	#define GDIAPI
	#define OMNISAPI
	typedef int32_t qlong;		// rmm64bit1
	typedef uint32_t qulong;	// rmm64bit1
	typedef unsigned char qbyte;
	typedef qbyte qbool;
	typedef uint32_t qchar;	// rmm64bit1
	#ifdef isunix
		typedef char qoschar;		// rmmuni_osx6
	#else
		typedef uint16_t qoschar;	// rmm64bit1
	#endif
	#if defined(ismacosx) && !defined(_TEXT)
		#define _TEXT(_a) ((qoschar *) (L ## _a))	// rmmuni_osx6
	#endif
	#define QTEXT(_a) _TEXT(_a)
	#define	QBYTELEN(_a) ((_a)*sizeof(qchar))
	#define QCHARLEN(_a) ((_a)/sizeof(qchar))
	#define	QOSBYTELEN(_a) ((_a)*sizeof(qoschar))
	#define QOSCHARLEN(_a) ((_a)/sizeof(qoschar))

	#define OM_OLD_EXTERNAL
#endif

#ifdef __cplusplus
	#define OM_EXTERN extern "C"
#else
	#define OM_EXTERN extern
#endif

// rmm8993: Context argument for OMstrtok
struct OMstrtokContextStruct
{
	void *mToken;
};
typedef struct OMstrtokContextStruct OMstrtokContext;

#if defined(ismach_o)	// pkiphone
	// rmm5654: sprintf for OSX Unicode - note that the variable arguments cannot contain any strings
	OM_EXTERN GDIAPI qlong OMNISAPI OMsprintf(qoschar *pBuffer, qoschar *pFormat,...); 
#endif 
// Functions for dealing with memory buffers of qbytes.
// rmm5654: moved these declarations to here, and modified their extern/export macros
OM_EXTERN GDIAPI void			OMNISAPI OMmemmove(void *pDest, const void *pSrc, qlong pCount);
OM_EXTERN GDIAPI void			OMNISAPI OMmemset(qbyte *pBuf, qbyte pByte, qlong pCount);
OM_EXTERN GDIAPI qlong		OMNISAPI OMmemcmp(const void *pBuf1, const void *pBuf2, qlong pCount);
OM_EXTERN GDIAPI qbyte *	OMNISAPI OMmemchr(const void *pBuf, qbyte pByte, qlong pCount);

// Start rmm_macho: also updated file, to use CHR everywhere instead of qchar, and to use the USE_TEMPLATE macro
#if defined(ismach_o) && defined(__cplusplus)	// pkiphone
	#define USE_CHAR_TEMPLATE template <class CHR>
	#define GDIAPI_OS
	#define OMNISAPI_OS
	#undef OM_EXTERN
	#define OM_EXTERN
	#define USE_MACHO_TEMPLATE
#else
	#define USE_CHAR_TEMPLATE
	#define CHR qchar
	#define GDIAPI_OS GDIAPI
	#define OMNISAPI_OS OMNISAPI
#endif
// End rmm_macho

#if !defined(USE_MACHO_TEMPLATE)	// rmm_macho
	// Functions for dealing with null terminated strings of qchar - just like the standard runtime functions.
	USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS qlong	OMNISAPI_OS OMstrlen(const CHR *pString);
	USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS CHR *	OMNISAPI_OS OMstrcpy(CHR *pDest, const CHR *pSource);
	USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS CHR *	OMNISAPI_OS OMstrncpy(CHR *pDest, const CHR *pSource, qlong pCount);
	USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS CHR *	OMNISAPI_OS OMstrcat(CHR *pDest, const CHR *pSource);
	USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS CHR *	OMNISAPI_OS OMstrncat(CHR *pDest, const CHR *pSource, qlong pCount);
	USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS qbool		OMNISAPI_OS OMstrequal(const CHR *pString1, const CHR *pString2);
	USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS CHR *	OMNISAPI_OS OMstrstr(const CHR *pString, const CHR *pStrCharSet);
	USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS CHR *	OMNISAPI_OS OMstrchr(const CHR *pString, CHR pChar);
	USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS CHR *	OMNISAPI_OS OMstrrchr(const CHR *pString, CHR pChar);
	USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS qlong		OMNISAPI_OS OMstrcspn(const CHR *pString, const CHR *pStrCharSet );
	USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS qlong		OMNISAPI_OS OMstrcmp(const CHR *pString1, const CHR *pString2);
	USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS qlong		OMNISAPI_OS OMstrncmp(const CHR *pString1, const CHR *pString2, qlong pCount);
	USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS qlong		OMNISAPI_OS OMstrspn(const CHR *pString, const CHR *pStrCharSet );
	USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS CHR *	OMNISAPI_OS OMstrpbrk(const CHR *pString, const CHR *pStrCharSet);
	USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS CHR *	OMNISAPI_OS OMstrtok(OMstrtokContext *pContext, CHR *pStrToken, const CHR *pStrDelimit); // rmm8993
	USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS CHR *	OMNISAPI_OS OMlongToString(CHR *pDest, qlong pLong);
	USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS qulong OMNISAPI_OS OMstrtoul(CHR *pText, CHR **pTextEnd, qlong pBase);	// rmm_macho
#endif	// rmm_macho: !defined(USE_MACHO_TEMPLATE)

#endif
// End of file
