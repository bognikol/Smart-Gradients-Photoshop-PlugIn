// ADOBE SYSTEMS INCORPORATED
// (c) Copyright  2009 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this 
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.
//-------------------------------------------------------------------
/**
*
*	\file PSIntTypes.h
*
*	\brief 
*      	Fixed sized integer types used in Photoshop
*
*      Distribution:
*      	PUBLIC
*      
*/     	

#ifndef __PSIntTypes__
#define __PSIntTypes__

//-------------------------------------------------------------------
//REVISIT stdint

//Our supported compilers are ILP32 (Mac32/Win32) LP64 (Mac64) and LLP64 (Win64)
//We should eventually migrate everything over to use the __ADOBE_USE_STD_INT__ #define

#if defined(__ADOBE_USE_STD_INT__) || defined(qDNGUseStdInt)

	#ifdef _MSC_VER
		#include <stddef.h>
	#else
		#include <stdint.h>
	#endif

	typedef int8_t  int8;
	typedef int16_t int16;
	typedef int32_t int32;
	typedef int64_t int64;
	
	typedef uint8_t  uint8;
	typedef uint16_t uint16;
	typedef uint32_t uint32;
	typedef uint64_t uint64;

	typedef uint8_t  unsigned8;
	typedef uint16_t unsigned16;
	typedef uint32_t unsigned32;
	typedef uint64_t unsigned64;

#else
	//Old style:
	//use char for int8
	//use short for int16
	//use long for int32 on ILP32/LLP64, int on LP64
	//use long long for int64

	#ifndef _INT8
		#define _INT8
		typedef signed char  int8;
	#endif

	#ifndef _INT16
		#define _INT16
		typedef signed short int16;
	#endif

	#ifndef _INT32
		#define _INT32
		#if __LP64__
			typedef signed int  int32;
		#else
			typedef signed long  int32;
		#endif
	#endif

	#ifndef _INT64
		#define _INT64
		typedef long long int64;
	#endif

	//-------------------------------------------------------------------

	#ifndef _UINT8
		#define _UINT8
		typedef unsigned char  uint8;
	#endif

	#ifndef _UINT16
		#define _UINT16
		typedef unsigned short uint16;
	#endif

	#ifndef _UINT32
		#define _UINT32
		#if __LP64__
			typedef unsigned int uint32;
		#else
			typedef unsigned long uint32;
		#endif
	#endif

	#ifndef _UINT64
		#define _UINT64
		typedef unsigned long long uint64;
	#endif

	//-------------------------------------------------------------------

	#ifndef _UNSIGNED8
		#define _UNSIGNED8
		typedef unsigned char  unsigned8;
	#endif

	#ifndef _UNSIGNED16
		#define _UNSIGNED16
		typedef unsigned short unsigned16;
	#endif

	#ifndef _UNSIGNED32
		#define _UNSIGNED32
		#if __LP64__
			typedef unsigned int unsigned32;
		#else
			typedef unsigned long unsigned32;
		#endif
	#endif

	#ifndef _UNSIGNED64
		#define _UNSIGNED64
		typedef unsigned long long unsigned64;
	#endif

	//-------------------------------------------------------------------

	#ifndef _INT64_T
		#define _INT64_T
		typedef long long int64_t; 	// Apple defines this in types.h
	#endif

#endif //__ADOBE_USE_STD_INT__

//-------------------------------------------------------------------
//-------------------------------------------------------------------

#ifndef _BOOL8
    #define _BOOL8
	typedef unsigned char	Bool8;
#endif

#ifndef _BOOL32
	#define _BOOL32
	typedef uint32 Bool32;
#endif

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//REVISIT - PSFixedTypes.h would probably be a more appropriate name for this file

#ifndef _REAL32
    #define _REAL32
	typedef float	real32;
#endif

#ifndef _REAL64
    #define _REAL64
	typedef double	real64;
#endif

#endif /* __PSIntTypes.h__ */
