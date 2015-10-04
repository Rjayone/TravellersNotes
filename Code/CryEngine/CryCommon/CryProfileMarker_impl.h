////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2002.
// -------------------------------------------------------------------------
//  File name:   CryProfileMarker_impl.pc.h
//  Version:     v1.00
//  Created:     11/12/2002 by Christopher Bolte.
//  Compilers:   Visual Studio.NET
//  Description: Platform Profiling Marker Implementation, dispatches to the correct header file
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#if !defined(CRY_PROFILE_MARKER_IMPL_H_)
#define CRY_PROFILE_MARKER_IMPL_H_

#if defined(ENABLE_PROFILING_CODE) && !((defined(SANDBOX_EXPORTS) || defined(PLUGIN_EXPORTS)))
	#if CRY_PLATFORM_WINDOWS || CRY_PLATFORM_LINUX || CRY_PLATFORM_ANDROID || CRY_PLATFORM_APPLE
		#include <CryProfileMarker_impl.pc.h>
	#elif CRY_PLATFORM_DURANGO
		#include <CryProfileMarker_impl.durango.h>
	#elif CRY_PLATFORM_ORBIS
		#include <CryProfileMarker_impl.orbis.h>
	#else
		#error No Platform support for Profile Marker
	#endif
#endif

#endif // CRY_PROFILE_MARKER_IMPL_H_
