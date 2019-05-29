/* $Header: svn://svn.omnis.net/trunk/Studio/O7EXE/INDY/omstring.c 18211 2017-10-11 07:10:55Z bmitchell $ */

/* Q4 omstring.c
String and memory runtime functions
Copyright (C) Raining Data 2002

Changes
Date			Edit				Bug					Description
05-Aug-16	rmm8993									Made OMstrtok thread-safe by adding an extra argument.
02-Nov-12	rmm7705									Memory issues.
23-Jan-09	rmm_iphone2							Changes to iPhone controls.
05-Apr-06	gra0363									Damomsql unicode compile broken
28-Feb-06	rmm5654									MachO Unicode compile.
28-Feb-06	MHMACH15(rmm)						Unicode MachO compile.
08-Dec-05	rmm_macho								Unicode functions implemented (because qoschar and wchar_t are 2 and 4 bytes respectively in Unicode builds)
16-Sep-02	gra0092									Fix to OMstrtok
22-May-02	rmm											New file.
*/
#ifndef OMSTRING_C	// rmm_macho
#define OMSTRING_C	// rmm_macho

#if defined(isRCCLIB) || defined(isRCC) || defined (isEXECLIB) || defined(isOMNISCORE) || defined(isXCOMPLIB) || defined(isGDIPICTLIB) || defined(isGDIHWNDLIB)
	#define __CONNECT__ 
	#include "basics.h"
	#ifndef DAMOMSQL //gra0363
  		#undef __CONNECT__  // MHMACH15
  	#endif
#endif
#include "omstring.h"
// rmm_macho: updated file, to use CHR everywhere instead of qchar, and to use the USE_CHAR_TEMPLATE macro
#if (defined(ismacosx) && defined(ismobile)) // rmm_iphone2	
	#include <corefoundation/cfstring.h>
	#include <stdio.h>
#endif

// ---------------------------------------------------------------------------------------------------------------------------------------
// Functions for dealing with null terminated strings of qchar ---------------------------------------------------------------------------
USE_CHAR_TEMPLATE
static void sCopyString(CHR *pDest, const CHR *pSource)
{
	CHR c;
	CHR *p = pDest;
	do
	{
		c = *pSource++;
		*p++ = c;
	} while (c);
}

USE_CHAR_TEMPLATE
GDIAPI_OS qlong OMNISAPI_OS OMstrlen(const CHR *pString)
{
	qlong len;
	if (!pString) return 0;
	len = 0;
	while (*pString++) ++len;
	return len;
}

USE_CHAR_TEMPLATE
GDIAPI_OS CHR * OMNISAPI_OS OMstrcpy(CHR *pDest, const CHR *pSource)
{
	if (!pSource || !pDest) return pDest;
	sCopyString(pDest, pSource);
	return pDest;
}

USE_CHAR_TEMPLATE
GDIAPI_OS CHR * OMNISAPI_OS OMstrncpy(CHR *pDest, const CHR *pSource, qlong pCount)
{
	// The strncpy function copies the initial pCount characters of pSource to pDest and returns pDest. 
	// If count is less than or equal to the length of pSource, a null character is not appended automatically to the copied string. 
	// If count is greater than the length of pSource, the destination string is padded with null characters up to length count. 
	// The behavior of strncpy is undefined if the source and destination strings overlap.
	// Start rmm7705: Cannot read past the end of pSource (it may not be null terminated): qlong sourceLen = OMstrlen(pSource);
	CHR *destPtr;
	qlong i;
	qlong sourceLen = 0;
	if (pSource)
	{
		const CHR *src = pSource;
		while (*src++)
		{
			++sourceLen;
			if (sourceLen >= pCount)
				break;
		}
	}
	// End rmm7705
	destPtr = pDest;
	for (i = 0; i < pCount; ++i)
	{
		if (i < sourceLen)
			*destPtr++ = *pSource++;
		else 
			*destPtr++ = 0;
	}
	return pDest;
}

USE_CHAR_TEMPLATE
GDIAPI_OS CHR * OMNISAPI_OS OMstrcat(CHR *pDest, const CHR *pSource)
{
	if (!pSource || !pDest) return pDest;
	sCopyString(&pDest[OMstrlen(pDest)], pSource);
	return pDest;
}

USE_CHAR_TEMPLATE
GDIAPI_OS CHR * OMNISAPI_OS OMstrncat(CHR *pDest, const CHR *pSource, qlong pCount)
{
	// The strncat function appends, at most, the first count characters of strSource to strDest. 
	// The initial character of strSource overwrites the terminating null character of strDest. 
	// If a null character appears in strSource before count characters are appended, strncat appends all characters 
	// from strSource, up to the null character. 
	// If count is greater than the length of strSource, the length of strSource is used in place of count. 
	// The resulting string is terminated with a null character. 
	// If copying takes place between strings that overlap, the behavior is undefined.
	qlong sourceLen, destLen;
	if (!pSource || !pDest) return pDest;
	sourceLen = OMstrlen(pSource);
	destLen = OMstrlen(pDest);
	if (pCount > sourceLen) pCount = sourceLen;
	OMmemmove(pDest + destLen, pSource, QBYTELEN(pCount));
	pDest[destLen + pCount] = 0;
	return pDest;
}

// Returns qtrue if the strings are equal
USE_CHAR_TEMPLATE
GDIAPI_OS qbool OMNISAPI_OS OMstrequal(const CHR *pString1, const CHR *pString2)
{
	qlong len1, i;
	if (!pString1 && !pString2) return qtrue; 
	len1 = OMstrlen(pString1);
	if (len1 != OMstrlen(pString2)) return qfalse;

	for (i = 0; i < len1; ++i)
	{
		if (*pString1++ != *pString2++) return qfalse;
	}
	return qtrue;
}

USE_CHAR_TEMPLATE
GDIAPI_OS CHR * OMNISAPI_OS OMstrstr(const CHR *pString, const CHR *pStrCharSet)
{
	// Locate string pStrCharSet in pString
	// Returns a pointer to the first occurrence of strCharSet in string, or NULL if strCharSet does not appear in string. 
	// If strCharSet points to a string of zero length, the function returns string.
	qlong stringLen;
	qlong strCharSetLen = OMstrlen(pStrCharSet);
	if (!strCharSetLen) return (CHR *) pString;

	stringLen = OMstrlen(pString);
	while (strCharSetLen <= stringLen)
	{
		if (!OMmemcmp(pString, pStrCharSet, QBYTELEN(strCharSetLen))) return (CHR *) pString;
		pString++;
		stringLen--;
	}
	return 0;
}

USE_CHAR_TEMPLATE
GDIAPI_OS CHR * OMNISAPI_OS OMstrchr(const CHR *pString, CHR pChar)
{
	// Returns a pointer to the first occurrence of c in string, or NULL if c is not found.
	if (!pString) return 0;
	while (*pString)
	{
		if (*pString == pChar) return (CHR *) pString;
		pString++;
	}
	return 0;
}

USE_CHAR_TEMPLATE
GDIAPI_OS CHR * OMNISAPI_OS OMstrrchr(const CHR *pString, CHR pChar)
{
	// Returns a pointer to the last occurrence of c in string, or NULL if c is not found.
	CHR *p;
	qlong len = OMstrlen(pString);
	if (!len) return 0;
	p = (CHR *) pString + OMstrlen(pString) - 1;
	while (p >= pString)
	{
		if (*p == pChar) return p;
		--p;
	}
	return 0;
}

USE_CHAR_TEMPLATE
GDIAPI_OS qlong OMNISAPI_OS OMstrcspn(const CHR *pString, const CHR *pStrCharSet )
{
	// Returns an integer value specifying the length of the initial segment of string that consists entirely of 
	// characters not in strCharSet. 
	// If string begins with a character that is in strCharSet, the function returns 0. No return value is reserved to indicate an error.
	qlong charSetLen = OMstrlen(pStrCharSet);
	qlong retVal = 0;
	qlong i;
	while (*pString)
	{
		CHR c = *pString++;
		for (i = 0; i < charSetLen; ++i)
		{
			if (pStrCharSet[i] == c) return retVal;
		}
		++retVal;
	}
	return retVal;
}

USE_CHAR_TEMPLATE
GDIAPI_OS qlong OMNISAPI_OS OMstrcmp(const CHR *pString1, const CHR *pString2)
{
	while (1)
	{
		CHR c1 = *pString1++;
		CHR c2 = *pString2++;
		if (c1 && c2)
		{
			if (c1 < c2) return -1;
			else if (c1 > c2) return 1;
		}
		else if (c1) return 1;
		else if (c2) return -1;
		else break;	// Strings are equal, because both c1 and c2 are zero
	}
	return 0;
}

USE_CHAR_TEMPLATE
GDIAPI_OS qlong OMNISAPI_OS OMstrncmp(const CHR *pString1, const CHR *pString2, qlong pCount)
{
	qlong i;
	for (i = 0; i < pCount; ++i)
	{
		CHR c1 = *pString1++;
		CHR c2 = *pString2++;
		if (c1 && c2)
		{
			if (c1 < c2) return -1;
			else if (c1 > c2) return 1;
		}
		else if (c1) return 1;
		else if (c2) return -1;
		else return 0;
	}
	return 0;
}

USE_CHAR_TEMPLATE
GDIAPI_OS qlong OMNISAPI_OS OMstrspn(const CHR *pString, const CHR *pStrCharSet )
{
	// Returns an integer value specifying the length of the substring in string that consists entirely of characters in strCharSet. 
	// If string begins with a character not in strCharSet, the function returns 0. 
	// No return value is reserved to indicate an error.
	qlong charSetLen = OMstrlen(pStrCharSet);
	qlong retVal = 0;
	while (*pString)
	{
		CHR c = *pString++;
		qbool found = qfalse;
		qlong i;
		for (i = 0; i < charSetLen; ++i)
		{
			if (pStrCharSet[i] == c) 
			{
				found = qtrue;
				break;
			}
		}
		if (!found) return retVal;
		++retVal;
	}
	return retVal;	
}

USE_CHAR_TEMPLATE
GDIAPI_OS CHR * OMNISAPI_OS OMstrpbrk(const CHR *pString, const CHR *pStrCharSet)
{
	// Returns a pointer to the first occurrence of any character from pStrCharSet in pString, or a NULL pointer if the two string 
	// arguments have no characters in common.
	qlong len = OMstrlen(pString);
	qlong charsNotInStrCharSet = OMstrcspn(pString, pStrCharSet);
	if (charsNotInStrCharSet < len) return (CHR *) &pString[charsNotInStrCharSet];
	return 0;
}

USE_CHAR_TEMPLATE
GDIAPI_OS CHR * OMNISAPI_OS OMstrtok(OMstrtokContext *pContext, CHR *pStrToken, const CHR *pStrDelimit) // rmm8993
{
	// On the first call to strtok, the function skips leading delimiters and returns a pointer to the first token in pStrToken, 
	// terminating the token with a null character. 
	// More tokens can be broken out of the remainder of pStrToken by a series of calls to strtok. 
	// Each call to strtok modifies pStrToken by inserting a null character after the token returned by that call. 
	// To read the next token from pStrToken, call strtok with a NULL value for the pStrToken argument. 
	// The NULL pStrToken argument causes strtok to search for the next token in the modified pStrToken. 
	// The pStrDelimit argument can take any value from one call to the next so that the set of delimiters may vary.
	if (!pStrDelimit) return 0;
	if (pStrToken)
	{
		// An initial call, to find the first token.
		pContext->mToken = pStrToken; // rmm8993
	}
	else
	{
		// A call to get the next token - in this case, there must be a pointer to the string passed to the first call
		if (!pContext->mToken) return 0; // rmm8993
	}
	// sStrToken now points to the next section of the string we are to examine.
	// Skip delimiters
	{
		qlong delimiterLen = OMstrlen(pStrDelimit);
		CHR *thisToken = (CHR *)pContext->mToken; // rmm8993
		CHR *p;
		while (*thisToken)
		{
			qbool found = qfalse;
			qlong i;
			for (i = 0; i < delimiterLen; ++i)
			{
				if (*thisToken == pStrDelimit[i])
				{
					found = qtrue;
					break;
				}
			}
			if (found) ++thisToken;
			else break;
		}
		if (!*thisToken)
		{
			// No more tokens
			pContext->mToken = 0; // rmm8993
			return 0;
		}
		// Find the end of this token.
		p = thisToken + 1;
		while (*p)
		{
			qbool found = qfalse;
			qlong i;
			for (i = 0; i < delimiterLen; ++i)
			{
				if (*p == pStrDelimit[i])
				{
					found = qtrue;
					break;
				}
			}
			if (found) break;
			p++;	//gra0092
		}
		if (!*p)
		{
			// Reached the end of the string
			pContext->mToken = 0; // rmm8993
			return thisToken;
		}
		*p = 0;																// Null terminate thisToken
		pContext->mToken = (void *) (p + 1);	// Ready for the next token // rmm8993
		return thisToken;
	}
}

USE_CHAR_TEMPLATE
GDIAPI_OS CHR *	OMNISAPI_OS OMlongToString(CHR *pDest, qlong pLong)
{
  CHR a[16];
	CHR *p = pDest;
	short i, n;
  
  if (!pLong) 
	{ 
		*p++ = '0';	
	}
  else 
	{
    if (pLong < 0) 
		{ 
			*p++ = '-'; 
			pLong = -pLong; 
		}
    n = 0; 
		do 
		{ 
			a[++n] = (CHR) (pLong % 10); 
			pLong = pLong/10; 
		} while (pLong != 0);
    for (i = n; i > 0; i--) 
			*p++ = '0' + a[i];
  }
	*p++ = 0;	// Null terminator
	return pDest;
}

#if !defined(_CHRBASIC_HE_) // rmm_macho
// ---------------------------------------------------------------------------------------------------------------------------------------
// Functions for dealing with memory buffers of qbytes -----------------------------------------------------------------------------------
GDIAPI void OMNISAPI OMmemmove(void *pDest, const void *pSrc, qlong pCount)	// rmm5654
{
	qbyte *s = (qbyte *) pSrc;
	qbyte *d = (qbyte *) pDest;
	while (pCount--) *d++ = *s++;
}

GDIAPI void OMNISAPI OMmemset(qbyte *pBuf, qbyte pByte, qlong pCount)		// rmm5654
{
	while (pCount--) *pBuf++ = pByte;
}

GDIAPI qlong OMNISAPI OMmemcmp(const void *pBuf1, const void *pBuf2, qlong pCount)		// rmm5654
{
	qlong i;
	qbyte *b1Ptr = (qbyte *) pBuf1;
	qbyte *b2Ptr = (qbyte *) pBuf2;
	for (i = 0; i < pCount; ++i)
	{
		qbyte b1 = *b1Ptr++;
		qbyte b2 = *b2Ptr++;
		if (b1 < b2) return -1;
		else if (b1 > b2) return 1;
	}
	return 0;
}

GDIAPI qbyte * OMNISAPI OMmemchr(const void *pBuf, qbyte pByte, qlong pCount)		// rmm5654
{
	qbyte *buf = (qbyte *) pBuf;
	qlong i;
	for (i = 0; i < pCount; ++i)
	{
		if (*buf == pByte) return buf;
		++buf;
	}
	return 0;
}

// Start rmm5654: sprintf for OSX Unicode - note that the variable arguments cannot contain any strings
#if defined(ismach_o)
	#include <stdarg.h>
	GDIAPI qlong OMNISAPI OMsprintf(qoschar *pBuffer, qoschar *pFormat, ...)
	{
		char utf8Format[1024];
		// Get the length of the format (cannot use OMstrlen, as this is compiled as C code in omstring.c, and OMstrlen takes a qchar *)
		qlong utf8FormatLen = 0;
		qoschar *u = pFormat;
		while (*u++) ++utf8FormatLen;		
		// Convert the format to UTF-8, so we can use the char version of vsprintf
		CFStringRef fmtRef = CFStringCreateWithCharacters(kCFAllocatorDefault, pFormat, utf8FormatLen);
		// Get UTF8 format  
		qbool ok = (CFStringGetCString(fmtRef, utf8Format, (CFIndex) sizeof(utf8Format), kCFStringEncodingUTF8) != 0);
		CFRelease(fmtRef);
		if (!ok)
		{
			// This should never occur - it would occur if the utf8Format buffer was too small, but 1024 is more than enough for our requirements
			return 0;
		}
		char resultBuffer[1024];
		va_list args;
		va_start(args, pFormat);
		qlong resultLength = vsnprintf(resultBuffer, sizeof(resultBuffer), utf8Format, args);
		if (resultLength >= (qlong) sizeof(resultBuffer))
		{
			// Result was cut short due to buffer size (should never occur)
			resultLength = sizeof(resultBuffer) - 1;
		}
		// Now convert the result from UTF-8 to qoschar, and return the number of qoschars 
		CFStringRef resultRef = CFStringCreateWithBytes(NULL, (UInt8 *) resultBuffer, resultLength, kCFStringEncodingUTF8, false);
		CFIndex returnCharCount = CFStringGetLength(resultRef);
		CFStringGetCharacters(resultRef, CFRangeMake(0, returnCharCount), pBuffer);
		CFRelease(resultRef);
		// Null terminate returned string
		pBuffer[returnCharCount] = 0;
		return (qlong) returnCharCount;
	}
#endif
// End rmm5654

#endif // rmm_macho: #ifndef _CHRBASIC_HE_

// ---------------------------------------------------------------------------------------------------------------------------------------
// Start rmm_macho
USE_CHAR_TEMPLATE OM_EXTERN GDIAPI_OS qulong OMNISAPI_OS OMstrtoul(CHR *pText, CHR **pTextEnd, qlong pBase)
{
	qlong retVal = 0;
	qbool isNeg = qfalse;
	// Skip whitespace at start
	CHR c;
	while ((c = *pText) != 0 && (' ' == c || '\t' == c))
	{
		++pText;
	}
	if (!c) 
	{
		if (pTextEnd) *pTextEnd = pText;
		return 0;
	}
	// Check for sign character (even though this function returns an unsigned value, the definition of the 
	// standard runtime requires signs to be taken into account
	c = *pText++;
	if ('-' == c) 
	{
		isNeg = qtrue;
		c = *pText++;
	}
	else if ('+' == c)
	{
		c = *pText++;
	}
	if (!c) 
	{
		if (pTextEnd) *pTextEnd = --pText;
		return 0;
	}
	// Determine base from the data if necessary
	if (!pBase)
	{
		if (('x' == *pText || 'X' == *pText) && '0' == c)
		{
			pBase = 16;
			++pText;
			c = *pText++;
		}
		else if ('0' == c)
		{
			pBase = 8;
			// Get next character, as it will make no difference to the result
			c = *pText++;
		}
		else
		{
			pBase = 10;
		}
	}
	if (!c || pBase < 2 || pBase > 36)
	{
		if (pTextEnd) *pTextEnd = --pText;
		return 0;
	}
	// Perform conversion
	do
	{
		int nextDigit = -1;
		if (c >= '0' && c <= '9')
			nextDigit = c - '0';
		else if (c >= 'A' && c <= 'Z')
			nextDigit = c - 'A' + 10;
		else if (c >= 'a' && c <= 'z')
			nextDigit = c - 'a' + 10;
		if (-1 == nextDigit || nextDigit >= pBase)
		{
			// Found the end of the number
			if (pTextEnd) *pTextEnd = --pText;
			break;
		}
		// Apply to the result
		retVal *= pBase;
		retVal += nextDigit; 
		// Get next digit
		c = *pText++;
	} while (1);

	return (qulong) (retVal*(isNeg ? -1 : 1));
}
// End rmm_macho

#endif // rmm_macho: #ifndef OMSTRING_C
// End of file
