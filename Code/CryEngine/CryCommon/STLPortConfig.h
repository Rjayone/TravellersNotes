///////////////////////////////////////////////////////////////////////////
//
//  CryEngine Source File.
//  Copyright (C), Crytek Studios, 2009.
// -------------------------------------------------------------------------
//  File name:   STLPortConfig.h
//  Created:     14/7/2009 by Timur.
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __STLPortConfig_h__
#define __STLPortConfig_h__
#pragma once

// Temporary here
#ifndef _CRT_SECURE_NO_DEPRECATE
# define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _CRT_NONSTDC_NO_DEPRECATE
# define _CRT_NONSTDC_NO_DEPRECATE
#endif

#if defined(CHANGE_STL_DEBUG_SETTINGS)
// Microsoft Debug STL turned off so we can use intermixed debug/release versions of DLL.
#undef _HAS_ITERATOR_DEBUGGING
#define _HAS_ITERATOR_DEBUGGING 0
#undef _SECURE_SCL
#define _SECURE_SCL 0
#undef _SECURE_SCL_THROWS
#define _SECURE_SCL_THROWS 0
#endif

//////////////////////////////////////////////////////////////////////////
// STL Port User config settings.
//////////////////////////////////////////////////////////////////////////

// disable exception handling code
#define _STLP_DONT_USE_EXCEPTIONS 1

// Needed for STLPort so we use our own terminate function in platform_impl.h
#define _STLP_DEBUG_TERMINATE 1
// Needed for STLPort so we use our own debug message function in platform_impl.h
#define _STLP_DEBUG_MESSAGE 1

/*
* Set _STLP_DEBUG to turn the "Debug Mode" on.
* That gets you checked iterators/ranges in the manner
* of "Safe STL". Very useful for debugging. Thread-safe.
* Please do not forget to link proper STLport library flavor
* (e.g libstlportstlg.so or libstlportstlg.a) when you set this flag
* in STLport iostreams mode, namespace customization guaranty that you
* link to the right library.
*/
#if defined(_DEBUG)
	#if !defined(_STLP_DEBUG)
		#define _STLP_DEBUG 1
	#endif
	#define _STLP_THREADS
	#define _STLP_WIN32THREADS
#endif

/*
* To reduce the famous code bloat trouble due to the use of templates STLport grant
* a specialization of some containers for pointer types. So all instantiations
* of those containers with a pointer type will use the same implementation based on
* a container of void*. This feature has shown very good result on object files size
* but after link phase and optimization you will only experiment benefit if you use
* many container with pointer types.
* There are however a number of limitation to use this option:
*   - with compilers not supporting partial template specialization feature, you won't
*     be able to access some nested container types like iterator as long as the
*     definition of the type used to instanciate the container will be incomplete
*     (see IncompleteClass definition in test/unit/vector_test.cpp).
*   - you won't be able to use complex Standard allocator implementations which are
*     allocators having pointer nested type not being a real C pointer.
*/
#define _STLP_USE_PTR_SPECIALIZATIONS 1

#endif //__STLPortConfig_h__
