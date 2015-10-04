// Copyright 2001-2015 Crytek GmbH. All rights reserved.


#pragma once

#include "IAudioSystemEditor.h"
#include "IAudioConnection.h"
#include "IAudioSystemControl.h"
#include <Serialization/Decorators/Range.h>
#include <Serialization/Enum.h>
#include <Serialization/ClassFactory.h>

namespace AudioControls
{

	enum ESDLMixerConnectionType
	{
		eSDLMCT_START = 0,
		eSDLMCT_STOP,
		eSDLMCT_NUM_TYPES
	};

	class CSDLMixerConnection : public IAudioConnection
	{
	public:
		explicit CSDLMixerConnection(CID nID)
			: IAudioConnection(nID)
			, eType(eSDLMCT_START)
			, bPanningEnabled(true)
			, bAttenuationEnabled(true)
			, fMinAttenuation(0.0f)
			, fMaxAttenuation(100.0f)
			, fVolume(-14.0f)
			, nLoopCount(1)
			, nEventID(0)
			, bInfiniteLoop(false)
		{ }

		virtual ~CSDLMixerConnection() {}

		virtual bool HasProperties() { return true; }

		virtual void Serialize(Serialization::IArchive& ar) override
		{
			ar(nEventID, "event_id", "");
			ar(eType, "action", "Action");
			ar(bPanningEnabled, "panning", "Enable Panning");

			if (ar.openBlock("DistanceAttenuation", "+Distance Attenuation"))
			{
				ar(bAttenuationEnabled, "attenuation", "Enable");
				if (bAttenuationEnabled)
				{
					if (ar.isInput())
					{
						float minAtt = fMinAttenuation;
						float maxAtt = fMaxAttenuation;
						ar(minAtt, "min_att", "Min Distance");
						ar(maxAtt, "max_att", "Max Distance");

						if (minAtt > maxAtt)
						{
							if (minAtt != fMinAttenuation)
							{
								maxAtt = minAtt;
							}
							else
							{
								minAtt = maxAtt;
							}
						}
						fMinAttenuation = minAtt;
						fMaxAttenuation = maxAtt;
					}
					else
					{
						ar(fMinAttenuation, "min_att", "Min Distance");
						ar(fMaxAttenuation, "max_att", "Max Distance");
					}
				}
				else
				{
					ar(fMinAttenuation, "min_att", "!Min Distance");
					ar(fMaxAttenuation, "max_att", "!Max Distance");
				}
				ar.closeBlock();
			}

			ar(Serialization::Range(fVolume, -96.0f, 0.0f), "vol", "Volume (dB)");

			if (ar.openBlock("Looping", "+Looping"))
			{
				ar(bInfiniteLoop, "infinite", "Infinite");
				if (bInfiniteLoop)
				{
					ar(nLoopCount, "loop_count", "!Count");
				}
				else
				{
					ar(nLoopCount, "loop_count", "Count");
				}
				ar.closeBlock();
			}
		}

		uint nEventID;
		ESDLMixerConnectionType eType;
		float fMinAttenuation;
		float fMaxAttenuation;
		float fVolume;
		uint nLoopCount;
		bool bPanningEnabled;
		bool bAttenuationEnabled;
		bool bInfiniteLoop;
	};

	typedef std::shared_ptr<CSDLMixerConnection> TSDLConnectionPtr;

	class CAudioConnectionInspectorPanel_sdlmixer;

	class CAudioSystemEditor_sdlmixer : public IAudioSystemEditor
	{
	public:
		CAudioSystemEditor_sdlmixer();
		virtual ~CAudioSystemEditor_sdlmixer();

		CID GetId(const string& sName) const;

		//////////////////////////////////////////////////////////
		// IAudioSystemEditor implementation
		/////////////////////////////////////////////////////////
		virtual void Reload(bool bPreserveConnectionStatus = true) override;
		virtual IAudioSystemControl* CreateControl(const SControlDef& controlDefinition) override;
		virtual IAudioSystemControl* GetRoot() override { return &m_root; }
		virtual IAudioSystemControl* GetControl(CID id) const override;
		virtual EACEControlType ImplTypeToATLType(TImplControlType type) const override;
		virtual TImplControlTypeMask GetCompatibleTypes(EACEControlType eATLControlType) const override;
		virtual TConnectionPtr CreateConnectionToControl(EACEControlType eATLControlType, IAudioSystemControl* pMiddlewareControl) override;
		virtual TConnectionPtr CreateConnectionFromXMLNode(XmlNodeRef pNode, EACEControlType eATLControlType) override;
		virtual XmlNodeRef CreateXMLNodeFromConnection(const TConnectionPtr pConnection, const EACEControlType eATLControlType) override;
		virtual string GetTypeIcon(TImplControlType type) const override;
		virtual string GetName() const override;
		virtual string GetDataPath() const;
		virtual void DataSaved() override;
		//////////////////////////////////////////////////////////

	private:

		static const string ms_sProjectFilePath;
		static const string ms_sControlNameTag;
		static const string ms_sEventIdTag;
		static const string ms_sEventConnectionTag;
		static const string ms_sSampleConnectionTag;
		static const string ms_sConnectionTypeTag;
		static const string ms_sPanningEnabledTag;
		static const string ms_sAttenuationEnabledTag;
		static const string ms_sAttenuationDistMin;
		static const string ms_sAttenuationDistMax;
		static const string ms_sVolumeTag;
		static const string ms_sLoopCountTag;

		static uint m_nextEventID;

		IAudioSystemControl m_root;
		typedef std::map<CID, std::vector<TSDLConnectionPtr> > TSDLMixerConnections;
		TSDLMixerConnections m_connectionsByID;
		std::vector<IAudioSystemControl*> m_controls;
	};
}
