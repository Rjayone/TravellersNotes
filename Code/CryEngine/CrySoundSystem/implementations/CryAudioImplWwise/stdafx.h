// CryEngine Header File.
// Copyright (C), Crytek, 1999-2014.

#if !defined(AFX_STDAFX_H__1E4C7FAC_937A_454F_BF0E_ABFBD056EEE5__INCLUDED_)
#define AFX_STDAFX_H__1E4C7FAC_937A_454F_BF0E_ABFBD056EEE5__INCLUDED_

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

extern CSoundAllocator g_AudioImplMemoryPool;
extern CAudioLogger g_AudioImplLogger;

#define AUDIO_ALLOCATOR_MEMORY_POOL g_AudioImplMemoryPool
#include <STLSoundAllocator.h>

#if !defined(_RELEASE)
#define INCLUDE_WWISE_IMPL_PRODUCTION_CODE
#endif // _RELEASE

#if CRY_PLATFORM_DURANGO
#define PROVIDE_WWISE_IMPL_SECONDARY_POOL
#endif

// Memory Allocation
#if defined(PROVIDE_WWISE_IMPL_SECONDARY_POOL)
#include <CryPool/PoolAlloc.h>

typedef NCryPoolAlloc::CThreadSafe< NCryPoolAlloc::CBestFit< NCryPoolAlloc::CReferenced< NCryPoolAlloc::CMemoryDynamic, 4*1024, true >, NCryPoolAlloc::CListItemReference > > tMemoryPoolReferenced;

extern tMemoryPoolReferenced g_AudioImplMemoryPoolSoundSecondary;

//////////////////////////////////////////////////////////////////////////
inline void* Secondary_Allocate(size_t const nSize)
{
	// Secondary Memory is Referenced. To not loose the handle, more memory is allocated
	// and at the beginning the handle is saved.

	/* Allocate in Referenced Secondary Pool */
	uint32 const nAllocHandle = g_AudioImplMemoryPoolSoundSecondary.Allocate<uint32>(nSize, AUDIO_MEMORY_ALIGNMENT);
	assert(nAllocHandle > 0);
	void* pAlloc = NULL;

	if (nAllocHandle > 0)
	{
		pAlloc = g_AudioImplMemoryPoolSoundSecondary.Resolve<void*>(nAllocHandle);
	}

	return pAlloc;
}

//////////////////////////////////////////////////////////////////////////
inline bool Secondary_Free(void* pFree)
{
	// Secondary Memory is Referenced. To not loose the handle, more memory is allocated
	// and at the beginning the handle is saved.

	// retrieve handle
	bool bFreed = (pFree == NULL);//true by default when passing NULL	
	uint32 const nAllocHandle = g_AudioImplMemoryPoolSoundSecondary.AddressToHandle(pFree);

	if (nAllocHandle > 0)
	{
		bFreed = g_AudioImplMemoryPoolSoundSecondary.Free(nAllocHandle);
	}

	return bFreed;
}
#endif // PROVIDE_AUDIO_IMPL_SECONDARY_POOL

// Windows or Durango
//////////////////////////////////////////////////////////////////////////
#if CRY_PLATFORM_WINDOWS || CRY_PLATFORM_DURANGO
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

// Durango
//////////////////////////////////////////////////////////////////////////
#if CRY_PLATFORM_DURANGO
//#include <xdk.h>
#endif

//////////////////////////////////////////////////////////////////////////
#if CRY_PLATFORM_ORBIS
#define AK_PS4
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

#endif // AFX_STDAFX_H__1E4C7FAC_937A_454F_BF0E_ABFBD056EEE5__INCLUDED_
