// CryEngine Source File.
// Copyright (C), Crytek, 1999-2014.

#pragma once

#include "StdAfx.h"
#include "AudioSystemControl_wwise.h"

namespace AudioControls
{
	IAudioSystemControl_wwise::IAudioSystemControl_wwise(const string& name, CID id, TImplControlType type)
		: IAudioSystemControl(name, id, type)
	{
	}
}