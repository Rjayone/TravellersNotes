// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

// TODO investigate why this warning is thrown if ref new Platfrom::Array<...> is used
#pragma warning ( disable:4717 ) // stupid warning in vccorlib.h

#include <CryModuleDefs.h>
#define eCryModule eCryM_Input

#define CRYINPUT_EXPORTS

#include <platform.h>

#include <vector>
#include <list>

#if defined(_DEBUG) && CRY_PLATFORM_WINAPI
#include <crtdbg.h>
#endif

#include <ITimer.h>
#include <IInput.h>
#include <CryName.h>
#include <StlUtils.h>
#include "CryInput.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
