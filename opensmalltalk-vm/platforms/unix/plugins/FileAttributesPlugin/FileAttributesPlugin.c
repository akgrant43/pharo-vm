/* Smalltalk from Squeak5.1 with VMMaker 4.16.3 translated as C source on 27 April 2017 9:47:16 am */
/* Automatically generated from Squeak on {27 April 2017 . 9:47:16 am} */

static char __buildInfo[] = "Generated on {27 April 2017 . 9:47:16 am}. Compiled on "__DATE__ ;




#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Default EXPORT macro that does nothing (see comment in sq.h): */
#define EXPORT(returnType) returnType

/* Do not include the entire sq.h file but just those parts needed. */
/*  The virtual machine proxy definition */
#include "sqVirtualMachine.h"
/* Configuration options */
#include "sqConfig.h"
/* Platform specific definitions */
#include "sqPlatformSpecific.h"

#define true 1
#define false 0
#define null 0  /* using 'null' because nil is predefined in Think C */
#ifdef SQUEAK_BUILTIN_PLUGIN
#undef EXPORT
// was #undef EXPORT(returnType) but screws NorCroft cc
#define EXPORT(returnType) static returnType
#endif
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
/* D T Lewis 2002-2013 - FileAttributesPlugin.c translated from class FileAttributesPlugin */

#include "sqMemoryAccess.h"


/*** Constants ***/

/*** Function Prototypes ***/
static sqInt accessAttributesForFilenameintostartingAt(char *cPathName, sqInt attributeArray, sqInt offset);
static sqInt canStatFilePathlength(char *aPathCString, sqInt length);
#pragma export on
EXPORT(sqInt) checkAccessmode(sqInt pathString, sqInt mode);
#pragma export off
static sqInt entryArrayForfromStatBufintostartingAt(sqInt fileNameOop, struct stat *statBuf, sqInt attributeArray, sqInt offset);
static sqInt fileOKFlag(void);
static int fileToAttributeArraymask(sqInt pathNameOop, sqInt attributeMask);
#pragma export on
EXPORT(const char*) getModuleName(void);
#pragma export off
static sqInt halt(void);
#pragma export on
EXPORT(sqInt) primitiveFileAttributes(void);
EXPORT(sqInt) primitiveFileExists(void);
EXPORT(sqInt) primitiveFileMasks(void);
#pragma export off
static struct stat * putStatForintoBuffer(char *nullTerminatedPathStringPointer, struct stat *statBufPointer);
static size_t readLinkintomaxLength(char *cPathName, char *cLinkPtr, size_t maxLength);
#pragma export on
EXPORT(sqInt) setInterpreter(struct VirtualMachine*anInterpreter);
#pragma export off
static sqInt smallOrLargeIntegerObjectFor(sqInt aPositiveInteger);
static time_t smalltalkSecondsFromPosixSeconds(time_t posixSeconds);
static sqInt stringFromCString(const char *aCString);
/*** Variables ***/
static int hasSecurityPlugin= 1;

#ifdef SQUEAK_BUILTIN_PLUGIN
extern
#endif
struct VirtualMachine* interpreterProxy;
static const char *moduleName =
#ifdef SQUEAK_BUILTIN_PLUGIN
	"FileAttributesPlugin 27 April 2017 (i)"
#else
	"FileAttributesPlugin 27 April 2017 (e)"
#endif
;
static int smallIntegerMaxVal= 1073741823;



/*	Call access() for each access type (R, W, X) on the c string cPathName, storing the results in the st array attributeArray. */

static sqInt accessAttributesForFilenameintostartingAt(char *cPathName, sqInt attributeArray, sqInt offset) {
    sqInt boolean;
    sqInt index;

	index = offset;
	if ((access(cPathName, R_OK)) == 0) {
		boolean = interpreterProxy->trueObject();
	} else {
		boolean = interpreterProxy->falseObject();
	}
	interpreterProxy->storePointerofObjectwithValue(index, attributeArray, boolean);
	index += 1;
	if ((access(cPathName, W_OK)) == 0) {
		boolean = interpreterProxy->trueObject();
	} else {
		boolean = interpreterProxy->falseObject();
	}
	interpreterProxy->storePointerofObjectwithValue(index, attributeArray, boolean);
	index += 1;
	if ((access(cPathName, X_OK)) == 0) {
		boolean = interpreterProxy->trueObject();
	} else {
		boolean = interpreterProxy->falseObject();
	}
	interpreterProxy->storePointerofObjectwithValue(index, attributeArray, boolean);
	return null;
}


/*	Answer non-zero if security permits the a stat() call on the file path. Allow a
	stat() call only on files which we are permitted to open. */
/*	FIXME: This function has not been tested. -dtl */

static sqInt canStatFilePathlength(char *aPathCString, sqInt length) {
    void *sCOFfn;


	/* If the security plugin can be loaded, use it to check . 
	If not, assume it's ok */
	/* The hasSecurityPlugin flag is set to 1 by default */

	if (hasSecurityPlugin == 0) {
		return 1;
	}
	sCOFfn = interpreterProxy->ioLoadFunctionFrom("secCanOpenFileOfSizeWritable", "SecurityPlugin");
	if (sCOFfn != 0) {
		return  ((int (*) (char *, int, int)) sCOFfn)(aPathCString, length, 0);
	} else {

		/* Reset the flag so we do not try again next time */

		hasSecurityPlugin = 0;
		return 1;
	}
	return null;
}


/*	Check access to pathString. */

EXPORT(sqInt) checkAccessmode(sqInt pathString, sqInt mode) {
    char cString[4096];
    sqInt idx;
    sqInt len;
    char *sPtr;


	/* Note: The static sized string buffer is faster than using a Smalltalk allocated
	string as the buffer, and is unlikely to fail in practice. In the event that
	the buffer turns out to be too small, write an error message to stdout before
	failing. */

	len = interpreterProxy->stSizeOf(pathString);
	if (len >= 4096) {
		fprintf(stderr, "PosixFileStatPlugin path name too long in checkAccessmode()\n");
		return 0;
	}
	sPtr = interpreterProxy->arrayValueOf(pathString);
	if ((canStatFilePathlength(sPtr, len)) == 0) {
		return 0;
	}
	idx = 0;
	while (idx < len) {
		cString[idx] = (sPtr[idx]);
		idx += 1;
	}
	cString[idx] = 0;
	return access(cString, mode);
}


/*	Answer a file entry array from pathName. */

static sqInt entryArrayForfromStatBufintostartingAt(sqInt fileNameOop, struct stat *statBuf, sqInt attributeArray, sqInt offset) {
    sqInt accessDate;
    sqInt creationDate;
    sqInt index;
    sqInt modificationDate;
    sqInt sizeIfFile;


	/* Use modification time instead (just like ls) */


	/* Use status change time instead */

	creationDate = smalltalkSecondsFromPosixSeconds(statBuf->st_mtime);
	modificationDate = smalltalkSecondsFromPosixSeconds(statBuf->st_ctime);
	accessDate = smalltalkSecondsFromPosixSeconds(statBuf->st_atime);
	if ((S_ISDIR(statBuf->st_mode)) == 0) {
		sizeIfFile = statBuf->st_size;
	} else {
		sizeIfFile = 0;
	}
	index = offset;
	interpreterProxy->storePointerofObjectwithValue(index, attributeArray, fileNameOop);
	index += 1;
	interpreterProxy->storePointerofObjectwithValue(index, attributeArray, smallOrLargeIntegerObjectFor(statBuf->st_mode));
	index += 1;
	interpreterProxy->storePointerofObjectwithValue(index, attributeArray, interpreterProxy->positive64BitIntegerFor(statBuf->st_ino));
	index += 1;
	interpreterProxy->storePointerofObjectwithValue(index, attributeArray, interpreterProxy->positive64BitIntegerFor(statBuf->st_dev));
	index += 1;
	interpreterProxy->storePointerofObjectwithValue(index, attributeArray, interpreterProxy->positive64BitIntegerFor(statBuf->st_nlink));
	index += 1;
	interpreterProxy->storePointerofObjectwithValue(index, attributeArray, smallOrLargeIntegerObjectFor(statBuf->st_uid));
	index += 1;
	interpreterProxy->storePointerofObjectwithValue(index, attributeArray, smallOrLargeIntegerObjectFor(statBuf->st_gid));
	index += 1;
	interpreterProxy->storePointerofObjectwithValue(index, attributeArray, smallOrLargeIntegerObjectFor(sizeIfFile));
	index += 1;
	interpreterProxy->storePointerofObjectwithValue(index, attributeArray, smallOrLargeIntegerObjectFor(accessDate));
	index += 1;
	interpreterProxy->storePointerofObjectwithValue(index, attributeArray, smallOrLargeIntegerObjectFor(modificationDate));
	index += 1;
	interpreterProxy->storePointerofObjectwithValue(index, attributeArray, smallOrLargeIntegerObjectFor(creationDate));
	return attributeArray;
}

static sqInt fileOKFlag(void) {
	return F_OK;
}


/*	Answer a file attribute array from pathNameOop. */

static int fileToAttributeArraymask(sqInt pathNameOop, sqInt attributeMask) {
    sqInt arraySize;
    sqInt attributeArray;
    char cLinkName[4096];
    char *cLinkPtr = (char *) &cLinkName;
    char cPathName[4096];
    sqInt fileNameOop;
    sqInt getAccess;
    sqInt getStats;
    sqInt idx;
    sqInt len;
    char *sPtr;
    struct stat statBuf;
    struct stat *statBufPointer= &statBuf;


	/* Note: The static sized string buffer is faster than using a Smalltalk allocated
	string as the buffer, and is unlikely to fail in practice as the normal PATH_MAX
	has been allocated. In the event that
	the buffer turns out to be too small, write an error message to stdout before
	failing. */

	len = interpreterProxy->stSizeOf(pathNameOop);
	if (len >= 4096) {
		fprintf(stderr, "PosixFileStatPlugin path name too long\n");
		return 0;
	}
	sPtr = interpreterProxy->arrayValueOf(pathNameOop);
	if ((canStatFilePathlength(sPtr, len)) == 0) {
		return 0;
	}
	idx = 0;
	while (idx < len) {
		cPathName[idx] = (sPtr[idx]);
		idx += 1;
	}
	cPathName[idx] = 0;
	getStats = (attributeMask & 1) == 1;
	getAccess = (attributeMask & 2) == 2;
	arraySize = 0;
	if (getStats) {
		arraySize += 11;
	}
	if (getAccess) {
		arraySize += 3;
	}
	if (arraySize == 0) {
		return 0;
	}

	/* Get attributes from stat() if requested */

	attributeArray = interpreterProxy->instantiateClassindexableSize(interpreterProxy->classArray(), arraySize);
	if (getStats) {
		statBufPointer = putStatForintoBuffer(cPathName, statBufPointer);
		if (statBufPointer == 0) {
			return 0;
		}
		if ((S_ISLNK(statBufPointer->st_mode)) == 0) {
			fileNameOop = interpreterProxy->nilObject();
		} else {
			len = readLinkintomaxLength(cPathName, cLinkPtr, 4096);
			if (len < 0) {
				return 0;
			}
			fileNameOop = stringFromCString(cLinkPtr);
		}
		entryArrayForfromStatBufintostartingAt(fileNameOop, statBufPointer, attributeArray, 0);
	}
	if (getAccess) {
		accessAttributesForFilenameintostartingAt(cPathName, attributeArray, 11);
	}
	return attributeArray;
}


/*	Note: This is hardcoded so it can be run from Squeak.
	The module name is used for validating a module *after*
	it is loaded to check if it does really contain the module
	we're thinking it contains. This is important! */

EXPORT(const char*) getModuleName(void) {
	return moduleName;
}

static sqInt halt(void) {
	;
	return null;
}


/*	Answer an array of file attributes.
	primFileAttributes: aString mask: attributeMask
	aString is the path to the file
	attributeMask indicates which attributes to retrieve:
	bit 0 - get stat() attributes
	bit 1 - get access() attributes */

EXPORT(sqInt) primitiveFileAttributes(void) {
    sqInt attributeArray;
    sqInt attributeMask;
    sqInt fileName;

	fileName = interpreterProxy->stackObjectValue(1);
	attributeMask = interpreterProxy->stackIntegerValue(0);
	if (!(interpreterProxy->isKindOf(fileName, "String"))) {
		fprintf(stderr, "primitiveFileAttributes: invalid argument, expected a String\n");
		return interpreterProxy->primitiveFail();
	}
	attributeArray = fileToAttributeArraymask(fileName, attributeMask);
	if (attributeArray == 0) {
		return interpreterProxy->primitiveFail();
	} else {
		interpreterProxy->pop(3);
		interpreterProxy->push(attributeArray);
	}
	return null;
}


/*	Check for existence of a file with a call to access(). */

EXPORT(sqInt) primitiveFileExists(void) {
    sqInt pathString;

	pathString = interpreterProxy->stackObjectValue(0);
	if (!(interpreterProxy->isKindOf(pathString, "String"))) {
		fprintf(stderr, "primitiveFileExists: invalid argument, expected a String\n");
		return interpreterProxy->primitiveFail();
	}
	if ((checkAccessmode(pathString, fileOKFlag())) == 0) {
		interpreterProxy->pop(2);
		interpreterProxy->push(interpreterProxy->trueObject());
	} else {
		interpreterProxy->pop(2);
		interpreterProxy->push(interpreterProxy->falseObject());
	}
	return null;
}


/*	Answer an array of well known file masks */

EXPORT(sqInt) primitiveFileMasks(void) {
    sqInt masks;

	masks = interpreterProxy->instantiateClassindexableSize(interpreterProxy->classArray(), 8);
	interpreterProxy->storePointerofObjectwithValue(0, masks, smallOrLargeIntegerObjectFor(S_IFMT));
	interpreterProxy->storePointerofObjectwithValue(1, masks, smallOrLargeIntegerObjectFor(S_IFSOCK));
	interpreterProxy->storePointerofObjectwithValue(2, masks, smallOrLargeIntegerObjectFor(S_IFLNK));
	interpreterProxy->storePointerofObjectwithValue(3, masks, smallOrLargeIntegerObjectFor(S_IFREG));
	interpreterProxy->storePointerofObjectwithValue(4, masks, smallOrLargeIntegerObjectFor(S_IFBLK));
	interpreterProxy->storePointerofObjectwithValue(5, masks, smallOrLargeIntegerObjectFor(S_IFDIR));
	interpreterProxy->storePointerofObjectwithValue(6, masks, smallOrLargeIntegerObjectFor(S_IFCHR));
	interpreterProxy->storePointerofObjectwithValue(7, masks, smallOrLargeIntegerObjectFor(S_IFIFO));
	interpreterProxy->popthenPush(1, masks);
	return null;
}


/*	Call stat() on nullTerminatedPathStringPointer, storing the results in
	the buffer at statBufPointer. */

static struct stat * putStatForintoBuffer(char *nullTerminatedPathStringPointer, struct stat *statBufPointer) {
    sqInt result;

	
# ifndef HAVE_LSTAT
	
# define HAVE_LSTAT 1
	
# endif
	
# if (HAVE_LSTAT == 1)
	result = lstat(nullTerminatedPathStringPointer, statBufPointer) && stat(nullTerminatedPathStringPointer, statBufPointer);
# else
	result = stat(nullTerminatedPathStringPointer, statBufPointer);
# endif  // HAVE_LSTAT == 1
	
	if (result != 0) {
		return 0;
	} else {
		return statBufPointer;
	}
	return null;
}


/*	Get the target filename of the supplied symbolic link. */

static size_t readLinkintomaxLength(char *cPathName, char *cLinkPtr, size_t maxLength) {
    size_t len;

	len = readlink(cPathName, cLinkPtr, maxLength);
	if (len < 0) {
		return len;
	}
	cLinkPtr[len] = 0;
	return len;
}


/*	Note: This is coded so that is can be run from Squeak. */

EXPORT(sqInt) setInterpreter(struct VirtualMachine*anInterpreter) {
    sqInt ok;

	interpreterProxy = anInterpreter;
	ok = interpreterProxy->majorVersion() == VM_PROXY_MAJOR;
	if (ok == 0) {
		return 0;
	}
	ok = interpreterProxy->minorVersion() >= VM_PROXY_MINOR;
	return ok;
}


/*	Answer an Integer object for aPositiveInteger. If the value is too large
	to fit in an SmallInteger, then answer a LargePositiveInteger. Note that
	smallIntegerMaxVal is a class variable with an initialization value determined
	when this class is translated to C code; see my #declareCVarsIn: class method. */

static sqInt smallOrLargeIntegerObjectFor(sqInt aPositiveInteger) {
	if (aPositiveInteger > smallIntegerMaxVal) {
		return ((aPositiveInteger << 1) | 1);
	} else {
		return interpreterProxy->positive32BitIntegerFor(aPositiveInteger);
	}
	return null;
}


/*	Adapted from Ian's convertToSqueakTime() function in sqXWindow.c. The Smalltalk
	epoch is Jan 1, 1901, and the Posix epoch is Jan 1, 1970. The difference is 17 leap
	years and 52 non-leap years. This method is not tested on Win32. */

static time_t smalltalkSecondsFromPosixSeconds(time_t posixSeconds) {
    time_t offset;
    time_t tzOffset;

	tzOffset = 0;
	
# ifdef HAVE_TM_GMTOFF  // POSIX.1-2001
	tzOffset = localtime(&posixSeconds)->tm_gmtoff;
# else
	
# ifdef HAVE_TIMEZONE  // note, may need something other than _timezone for Windows
	tzOffset = daylight * 3600 - timezone;
# else
	tzOffset = _daylight * 3600 - _timezone;
# endif  // HAVE_TIMEZONE
	
# endif  // HAVE_TM_GMTOFF
	

	/* ((52 * 365) + (17 * 366)) * 24 * 60 * 60 -> 2177452800 */

	offset = 2177452800U;
	return (posixSeconds + offset) + tzOffset;
}


/*	Answer a new String copied from a null-terminated C string.
	Caution: This may invoke the garbage collector. */

static sqInt stringFromCString(const char *aCString) {
    sqInt len;
    sqInt newString;

	len = strlen(aCString);
	newString = interpreterProxy->instantiateClassindexableSize(interpreterProxy->classString(), len);
	strncpy(interpreterProxy->arrayValueOf(newString), aCString, len);
	return newString;
}


#ifdef SQUEAK_BUILTIN_PLUGIN


void* FileAttributesPlugin_exports[][3] = {
	{"FileAttributesPlugin", "primitiveFileExists", (void*)primitiveFileExists},
	{"FileAttributesPlugin", "primitiveFileAttributes", (void*)primitiveFileAttributes},
	{"FileAttributesPlugin", "getModuleName", (void*)getModuleName},
	{"FileAttributesPlugin", "checkAccessmode", (void*)checkAccessmode},
	{"FileAttributesPlugin", "primitiveFileMasks", (void*)primitiveFileMasks},
	{"FileAttributesPlugin", "setInterpreter", (void*)setInterpreter},
	{NULL, NULL, NULL}
};


#endif /* ifdef SQ_BUILTIN_PLUGIN */

