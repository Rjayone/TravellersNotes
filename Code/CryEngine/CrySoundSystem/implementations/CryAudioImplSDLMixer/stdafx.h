// CryEngine Header File.
// Copyright (C), Crytek, 1999-2014.

#if !defined(AFX_STDAFX_H__1E4C7FAC_937A_454F_BF0E_GRHBD056EEE5__INCLUDED_)
#define AFX_STDAFX_H__1E4C7FAC_937A_454F_BF0E_GRHBD056EEE5__INCLUDED_

#include <CryModuleDefs.h>
#include <platform.h>
#include <StlUtils.h>
#include <ProjectDefines.h>
#include <SoundAllocator.h>

#if !defined(_RELEASE)
// Define this to enable logging via CAudioLogger.
// We disable logging for Release builds
#define ENABLE_AUDIO_LOGGING
#endif // _RELEASE

#include <AudioLogger.h>

extern CSoundAllocator g_SDLMixerImplMemoryPool;
extern CAudioLogger g_SDLMixerImplLogger;

#define AUDIO_ALLOCATOR_MEMORY_POOL g_SDLMixerImplMemoryPool
#include <STLSoundAllocator.h>

#if !defined(_RELEASE)
#define INCLUDE_SDLMIXER_IMPL_PRODUCTION_CODE
#endif

// Windows (32 or 64)
//////////////////////////////////////////////////////////////////////////
#if CRY_PLATFORM_WINDOWS
#include <windows.h>
#endif

// Windows32
//////////////////////////////////////////////////////////////////////////
#if CRY_PLATFORM_WINDOWS && CRY_PLATFORM_32BIT
#endif

// Windows64
//////////////////////////////////////////////////////////////////////////
#if CRY_PLATFORM_WINDOWS && CRY_PLATFORM_64BIT
#endif

// Mac
//////////////////////////////////////////////////////////////////////////
#if CRY_PLATFORM_MAC
#endif

// Android
//////////////////////////////////////////////////////////////////////////
#if CRY_PLATFORM_ANDROID
#endif

// iOS
//////////////////////////////////////////////////////////////////////////
#if CRY_PLATFORM_IOS
#endif

// Linux
//////////////////////////////////////////////////////////////////////////
#if CRY_PLATFORM_LINUX
#endif

#endif // AFX_STDAFX_H__1E4C7FAC_937A_454F_BF0E_GRHBD056EEE5__INCLUDED_
