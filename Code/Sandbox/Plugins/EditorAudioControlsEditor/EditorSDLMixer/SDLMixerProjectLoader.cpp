// CryEngine Source File.
// Copyright (C), Crytek, 1999-2014.

#include "StdAfx.h"
#include "SDLMixerProjectLoader.h"
#include "CryFile.h"
#include "ISystem.h"
#include "CryPath.h"
#include <IAudioSystemEditor.h>
#include <IAudioSystemControl.h>
#include "AudioSystemControl_sdlmixer.h"

using namespace PathUtil;

namespace AudioControls
{
	CSDLMixerProjectLoader::CSDLMixerProjectLoader(const string& sAssetsPath, IAudioSystemEditor* pAudioSystemImpl)
		: m_pAudioSystemImpl(pAudioSystemImpl)
	{
		_finddata_t fd;
		ICryPak* pCryPak = gEnv->pCryPak;
		intptr_t handle = pCryPak->FindFirst(sAssetsPath + "/*.*", &fd);
		if (handle != -1)
		{
			do
			{
				const string name = fd.name;
				if (name != "." && name != ".." && !name.empty())
				{
					if (name.find(".wav") != string::npos || name.find(".ogg") != string::npos /*|| name.find(".mp3") != string::npos*/)
					{
						// Create the event with the same name as the file
						m_pAudioSystemImpl->CreateControl(SControlDef(name, eSDLMT_EVENT));
					}
				}
			}
			while (pCryPak->FindNext(handle, &fd) >= 0);
			pCryPak->FindClose(handle);
		}
	}
}