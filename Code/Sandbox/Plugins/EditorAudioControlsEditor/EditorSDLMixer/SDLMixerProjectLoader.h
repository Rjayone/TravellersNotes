// CryEngine Header File.
// Copyright (C), Crytek, 1999-2014.

#pragma once

#include <IXml.h>
#include "ACETypes.h"

namespace AudioControls
{
	class IAudioSystemEditor;

	class CSDLMixerProjectLoader
	{
	public:
		CSDLMixerProjectLoader(const string& sAssetsPath, IAudioSystemEditor* pAudioSystemImpl);
		IAudioSystemEditor* m_pAudioSystemImpl;
	};
}