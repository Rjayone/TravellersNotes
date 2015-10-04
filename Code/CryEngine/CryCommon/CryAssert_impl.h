// CryEngine Source File.
// Copyright (C), Crytek, 1999-2015.
// CRY_ASSERT implementation.

#include "CryAssert.h"
#if defined(USE_CRY_ASSERT)



#if CRY_PLATFORM_DURANGO
	#include "CryAssert_Durango.h"
#elif CRY_PLATFORM_MAC
	#include "CryAssert_Mac.h"
#elif CRY_PLATFORM_IOS
	#include "CryAssert_iOS.h"
#elif CRY_PLATFORM_ANDROID
	#include "CryAssert_Android.h"
#elif CRY_PLATFORM_LINUX
	#include "CryAssert_Linux.h"
#elif CRY_PLATFORM_WINDOWS
	#include "CryAssert_Windows.h"
#elif CRY_PLATFORM_ORBIS
	#include "CryAssert_Orbis.h"
#else

// Pull in system assert macro
#include <assert.h> 

void CryAssertTrace(const char *,...)
{
	// Empty on purpose
}

bool CryAssert(const char *,const char *,unsigned int,bool *)
{
	assert(false && "CryAssert not implemented");
	return true;
}

// Restore previous assert definition
#include "CryAssert.h" 

#endif

// Check if assert is enabled (the same on every platform)
bool CryAssertIsEnabled()
{
	return !gEnv || !gEnv->pSystem || gEnv->pSystem->IsAssertEnabled();
}

#endif // defined(USE_CRY_ASSERT)
