// CryEngine Header File.
// Copyright (C), Crytek, 1999-2014.

#pragma once

#include <IAudioConnection.h>
#include <IAudioSystemControl.h>

namespace AudioControls
{
	enum ESDLMixerTypes
	{
		eSDLMT_INVALID = 0,
		eSDLMT_EVENT = 1,
		eSDLMT_SAMPLE_FILE = 2,
	}; 

	class IAudioSystemControl_sdlmixer : public IAudioSystemControl
	{

	public:
		IAudioSystemControl_sdlmixer() {}
		IAudioSystemControl_sdlmixer(const string& name, CID id, TImplControlType type);
		virtual ~IAudioSystemControl_sdlmixer() {}
	};
}