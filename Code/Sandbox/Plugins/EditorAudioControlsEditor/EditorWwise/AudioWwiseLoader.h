// CryEngine Header File.
// Copyright (C), Crytek, 1999-2014.

#pragma once

#include <IXml.h>
#include "ACETypes.h"
#include "AudioSystemControl_wwise.h"

namespace AudioControls
{
	class CAudioSystemEditor_wwise;

	class CAudioWwiseLoader
	{

	public:
		CAudioWwiseLoader() : m_pAudioSystemImpl(nullptr) {}
		void Load(CAudioSystemEditor_wwise* pAudioSystemImpl);
		string GetLocalizationFolder() const;

	private:
		void LoadSoundBanks(const string& sRootFolder, const string& sSubPath, bool bLocalised);
		void LoadControlsInFolder(const string& sFolderPath);
		void LoadControl(XmlNodeRef root);
		void ExtractControlsFromXML(XmlNodeRef root, EWwiseControlTypes type, const string& controlTag, const string& controlNameAttribute);

	private:
		static const string ms_sWwiseProjectPath;
		static const string ms_sGameParametersFolder;
		static const string ms_sGameStatesPath;
		static const string ms_sSwitchesFolder;
		static const string ms_sEventsFolder;
		static const string ms_sEnvironmentsFolder;
		static const string ms_sSoundBanksPath;

		string m_sLocalizationFolder;

		CAudioSystemEditor_wwise* m_pAudioSystemImpl;
	};
}