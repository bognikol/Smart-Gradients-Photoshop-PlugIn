/***********************************************************************/
/*                                                                     */
/* ASConfig.h                                                          */
/* Adobe Standard Environment Configuration                            */
/*                                                                     */
/* Copyright 1996-1999 Adobe Systems Incorporated.                     */
/* All Rights Reserved.                                                */
/*                                                                     */
/* Patents Pending                                                     */
/*                                                                     */
/* NOTICE: All information contained herein is the property of Adobe   */
/* Systems Incorporated. Many of the intellectual and technical        */
/* concepts contained herein are proprietary to Adobe, are protected   */
/* as trade secrets, and are made available only to Adobe licensees    */
/* for their internal use. Any reproduction or dissemination of this   */
/* software is strictly forbidden unless prior written permission is   */
/* obtained from Adobe.                                                */
/*                                                                     */
/* Started by Dave Lazarony, 01/26/1996                                */
/*                                                                     */
/***********************************************************************/

#ifndef __ASConfig__
#define __ASConfig__

/*
 * Defines for MPW and THINK C.
 */
#if defined(macintosh) || defined(THINK_C)
#ifndef MAC_ENV
#define MAC_ENV
#endif
#endif

/*
 * Defines for Metrowerks on Macintosh.
 */
#ifdef __MWERKS__

#if defined(__MC68K__) || defined(__POWERPC__)
#ifndef MAC_ENV
#define MAC_ENV
#endif
#endif

#ifdef __MC68K__
#error 68K plugins no longer supported!
#endif

#ifdef __POWERPC__
#ifndef MACPPC_ENV
#define MACPPC_ENV
#endif
#endif

#endif


/*
 * Defines for gcc4 on Mac OS X.
 */
#ifdef __APPLE_CC__ // DRSWAT: added to support Xcode build.
	#ifndef MAC_ENV
		#define MAC_ENV
	#endif

	#ifndef MAC_MACHO_ENV
		#define MAC_MACHO_ENV
	#endif

	#ifdef __POWERPC__
		#ifndef MACPPC_ENV
			#define MACPPC_ENV
		#endif
	#endif
#endif

/*
 * Defines for Metrowerks on Windows.
 */
#ifdef __MWERKS__
#ifdef __INTEL__
#ifndef WIN_ENV
#define WIN_ENV
#endif
#endif
#endif

/*
 * Defines for Visual C++ on Windows.
 */
#ifdef _WINDOWS
#ifndef WIN_ENV
#define WIN_ENV
#endif
#endif

#endif
