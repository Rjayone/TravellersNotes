// CryEngine Source File.
// Copyright (C), Crytek, 1999-2014.

#pragma once

#include "StdAfx.h"
#include "AudioSystemControl_sdlmixer.h"

namespace AudioControls
{
	IAudioSystemControl_sdlmixer::IAudioSystemControl_sdlmixer(const string& name, CID id, TImplControlType type)
		: IAudioSystemControl(name, id, type)
	{
	}
}