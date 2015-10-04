// Copyright 2001-2015 Crytek GmbH. All rights reserved.

#pragma once

#include "StdAfx.h"
#include "AudioSystemEditor_sdlmixer.h"
#include "SDLMixerProjectLoader.h"
#include "AudioSystemControl_sdlmixer.h"
#include <CryFile.h>
#include <ISystem.h>
#include "CryCrc32.h"
#include <CryPath.h>
#include <StlUtils.h>
#include <IAudioSystem.h>

namespace AudioControls
{
	const string CAudioSystemEditor_sdlmixer::ms_sProjectFilePath = "sounds/sdlmixer";
	const string CAudioSystemEditor_sdlmixer::ms_sEventConnectionTag = "SDLMixerEvent";
	const string CAudioSystemEditor_sdlmixer::ms_sSampleConnectionTag = "SDLMixerSample";
	const string CAudioSystemEditor_sdlmixer::ms_sControlNameTag = "sdl_name";
	const string CAudioSystemEditor_sdlmixer::ms_sPanningEnabledTag = "enable_panning";
	const string CAudioSystemEditor_sdlmixer::ms_sAttenuationEnabledTag = "enable_distance_attenuation";
	const string CAudioSystemEditor_sdlmixer::ms_sAttenuationDistMin = "attenuation_dist_min";
	const string CAudioSystemEditor_sdlmixer::ms_sAttenuationDistMax = "attenuation_dist_max";
	const string CAudioSystemEditor_sdlmixer::ms_sVolumeTag = "volume";
	const string CAudioSystemEditor_sdlmixer::ms_sLoopCountTag = "loop_count";
	const string CAudioSystemEditor_sdlmixer::ms_sConnectionTypeTag = "event_type";
	const string CAudioSystemEditor_sdlmixer::ms_sEventIdTag = "event_id";
	uint CAudioSystemEditor_sdlmixer::m_nextEventID = 1;

	CAudioSystemEditor_sdlmixer::CAudioSystemEditor_sdlmixer()
	{
		Reload();
	}

	CAudioSystemEditor_sdlmixer::~CAudioSystemEditor_sdlmixer()
	{
	}

	void CAudioSystemEditor_sdlmixer::Reload(bool bPreserveConnectionStatus)
	{
		m_controls.clear();
		m_root = IAudioSystemControl();
		AudioControls::CSDLMixerProjectLoader(ms_sProjectFilePath, this);

		TSDLMixerConnections::const_iterator it = m_connectionsByID.begin();
		TSDLMixerConnections::const_iterator end = m_connectionsByID.end();
		for (; it != end; ++it)
		{
			if (it->second.size() > 0)
			{
				IAudioSystemControl* pControl = GetControl(it->first);
				if (pControl)
				{
					pControl->SetConnected(true);
				}
			}
		}
	}

	IAudioSystemControl* CAudioSystemEditor_sdlmixer::CreateControl(const SControlDef& controlDefinition)
	{
		const CID nID = GetId(controlDefinition.sName);
		IAudioSystemControl_sdlmixer* pControl = new IAudioSystemControl_sdlmixer(controlDefinition.sName, nID, controlDefinition.eType);
		if (pControl)
		{
			IAudioSystemControl* pParent = controlDefinition.pParent;
			if (pParent == nullptr)
			{
				pParent = &m_root;
			}

			pParent->AddChild(pControl);
			m_controls.push_back(pControl);
		}
		return pControl;
	}

	IAudioSystemControl* CAudioSystemEditor_sdlmixer::GetControl(CID id) const
	{
		if (id >= 0)
		{
			size_t size = m_controls.size();
			for (size_t i = 0; i < size; ++i)
			{
				if (m_controls[i]->GetId() == id)
				{
					return m_controls[i];
				}
			}
		}
		return nullptr;
	}

	TConnectionPtr CAudioSystemEditor_sdlmixer::CreateConnectionToControl(EACEControlType eATLControlType, IAudioSystemControl* pMiddlewareControl)
	{
		std::shared_ptr<CSDLMixerConnection> pConnection = std::make_shared<CSDLMixerConnection>(pMiddlewareControl->GetId());
		pConnection->nEventID = m_nextEventID++;
		pMiddlewareControl->SetConnected(true);
		m_connectionsByID[pMiddlewareControl->GetId()].push_back(pConnection);
		return pConnection;
	}

	TConnectionPtr CAudioSystemEditor_sdlmixer::CreateConnectionFromXMLNode(XmlNodeRef pNode, EACEControlType eATLControlType)
	{
		if (pNode)
		{
			const string sTag = pNode->getTag();
			if (sTag == ms_sEventConnectionTag || sTag == ms_sSampleConnectionTag)
			{
				const string sName = pNode->getAttr(ms_sControlNameTag);
				CID id = GetId(sName);
				if (id != ACE_INVALID_ID)
				{
					IAudioSystemControl* pControl = GetControl(id);
					if (pControl == nullptr)
					{
						pControl = CreateControl(SControlDef(sName, eSDLMT_EVENT));
						if (pControl)
						{
							pControl->SetPlaceholder(true);
						}
					}

					if (pControl)
					{
						TSDLConnectionPtr pConnection = std::make_shared<CSDLMixerConnection>(pControl->GetId());
						pControl->SetConnected(true);
						m_connectionsByID[pControl->GetId()].push_back(pConnection);

						uint eventID = m_nextEventID++;
						if (pNode->haveAttr(ms_sEventIdTag))
						{
							pNode->getAttr(ms_sEventIdTag, eventID);
							if (eventID >= m_nextEventID)
							{
								m_nextEventID = eventID + 1;
							}
						}
						pConnection->nEventID = eventID;

						const string sConnectionType = pNode->getAttr(ms_sConnectionTypeTag);
						pConnection->eType = sConnectionType == "stop" ? eSDLMCT_STOP : eSDLMCT_START;

						const string sEnablePanning = pNode->getAttr(ms_sPanningEnabledTag);
						pConnection->bPanningEnabled = sEnablePanning == "true" ? true : false;

						const string sEnableDistAttenuation = pNode->getAttr(ms_sAttenuationEnabledTag);
						pConnection->bAttenuationEnabled  = sEnableDistAttenuation == "true" ? true : false;

						const string sAttenuationMin = pNode->getAttr(ms_sAttenuationDistMin);
						pConnection->fMinAttenuation = (float)std::atof(sAttenuationMin.c_str());

						const string sAttenuationMax = pNode->getAttr(ms_sAttenuationDistMax);
						pConnection->fMaxAttenuation = (float)std::atof(sAttenuationMax.c_str());

						const string sVolume = pNode->getAttr(ms_sVolumeTag);
						pConnection->fVolume = (float)std::atof(sVolume.c_str());

						const string sLoopCount = pNode->getAttr(ms_sLoopCountTag);
						pConnection->nLoopCount = std::atoi(sLoopCount.c_str());
						if (pConnection->nLoopCount == -1)
						{
							pConnection->bInfiniteLoop = true;
						}

						return pConnection;
					}
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_EDITOR, VALIDATOR_ERROR, "Audio Controls Editor (SDL Mixer): Error reading connection to %s", sName);
				}
			}
		}
		return nullptr;
	}

	XmlNodeRef CAudioSystemEditor_sdlmixer::CreateXMLNodeFromConnection(const TConnectionPtr pConnection, const EACEControlType eATLControlType)
	{
		std::shared_ptr<const CSDLMixerConnection> pSDLMixerConnection = std::static_pointer_cast<const CSDLMixerConnection>(pConnection);
		const IAudioSystemControl* pControl = GetControl(pConnection->GetID());
		if (pControl && pSDLMixerConnection && eATLControlType == eACET_TRIGGER)
		{
			XmlNodeRef pConnectionNode = GetISystem()->CreateXmlNode(ms_sEventConnectionTag);
			pConnectionNode->setAttr(ms_sEventIdTag, pSDLMixerConnection->nEventID);
			pConnectionNode->setAttr(ms_sControlNameTag, pControl->GetName());

			if (pSDLMixerConnection->eType == eSDLMCT_START)
			{
				pConnectionNode->setAttr(ms_sConnectionTypeTag, "start");
				pConnectionNode->setAttr(ms_sPanningEnabledTag, pSDLMixerConnection->bPanningEnabled ? "true" : "false");
				pConnectionNode->setAttr(ms_sAttenuationEnabledTag, pSDLMixerConnection->bAttenuationEnabled ? "true" : "false");
				pConnectionNode->setAttr(ms_sAttenuationDistMin, pSDLMixerConnection->fMinAttenuation);
				pConnectionNode->setAttr(ms_sAttenuationDistMax, pSDLMixerConnection->fMaxAttenuation);
				pConnectionNode->setAttr(ms_sVolumeTag, pSDLMixerConnection->fVolume);
				if (pSDLMixerConnection->bInfiniteLoop)
				{
					pConnectionNode->setAttr(ms_sLoopCountTag, -1);
				}
				else
				{
					pConnectionNode->setAttr(ms_sLoopCountTag, pSDLMixerConnection->nLoopCount);
				}
			}
			else 
			{
				pConnectionNode->setAttr(ms_sConnectionTypeTag, "stop");
			}

			return pConnectionNode;
		}
		return nullptr;
	}

	AudioControls::CID CAudioSystemEditor_sdlmixer::GetId(const string& sName) const
	{
		return CCrc32::Compute(sName);
	}

	string CAudioSystemEditor_sdlmixer::GetTypeIcon(TImplControlType type) const
	{
		return "Editor/Icons/audio/sdl_mixer/Audio_Event.png";
	}

	AudioControls::EACEControlType CAudioSystemEditor_sdlmixer::ImplTypeToATLType(TImplControlType type) const
	{
		return eACET_TRIGGER;
	}

	AudioControls::TImplControlTypeMask CAudioSystemEditor_sdlmixer::GetCompatibleTypes(EACEControlType eATLControlType) const
	{
		switch (eATLControlType)
		{
		case eACET_TRIGGER:
			return eSDLMT_EVENT;
		}
		return AUDIO_IMPL_INVALID_TYPE;
	}

	string CAudioSystemEditor_sdlmixer::GetName() const
	{
		return "SDL Mixer";
	}

	string CAudioSystemEditor_sdlmixer::GetDataPath() const
	{
		return PathUtil::GetGameFolder() + "/" + ms_sProjectFilePath;
	}

	void CAudioSystemEditor_sdlmixer::DataSaved()
	{
		SAudioRequest oConfigDataRequest;
		oConfigDataRequest.nFlags = eARF_PRIORITY_HIGH;
		SAudioManagerRequestData<eAMRT_RETRIGGER_AUDIO_CONTROLS> oAMRetriggerData;
		oConfigDataRequest.pData = &oAMRetriggerData;
		gEnv->pAudioSystem->PushRequest(oConfigDataRequest);
	}

	SERIALIZATION_ENUM_BEGIN(ESDLMixerConnectionType, "Event Type")
	SERIALIZATION_ENUM(eSDLMCT_START, "start", "Start")
	SERIALIZATION_ENUM(eSDLMCT_STOP, "stop", "Stop")
	SERIALIZATION_ENUM_END()

}