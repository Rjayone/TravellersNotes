// Copyright 2001-2015 Crytek GmbH. All rights reserved.

#include "StdAfx.h"
#include "FlowBaseNode.h"

class CFlowNode_AudioRtpc : public CFlowBaseNode<eNCT_Instanced>
{
public:

	explicit CFlowNode_AudioRtpc(SActivationInfo* pActInfo)
		: m_fValue(0.0f)
		, m_nRtpcID(INVALID_AUDIO_CONTROL_ID)
	{}

	//////////////////////////////////////////////////////////////////////////
	~CFlowNode_AudioRtpc() {}

	//////////////////////////////////////////////////////////////////////////
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo)
	{
		return new CFlowNode_AudioRtpc(pActInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	enum INPUTS
	{
		eIn_RtpcName,
		eIn_RtpcValue,
	};

	enum OUTPUTS 
	{
	};

	virtual void GetConfiguration(SFlowNodeConfig& config)
	{
		static const SInputPortConfig inputs[] = 
		{
			InputPortConfig<string>("audioRTPC_Name", _HELP("RTPC name"), "Name"),
			InputPortConfig<float>("value", _HELP("RTPC value"), "Value"),
			{0}
		};

		static const SOutputPortConfig outputs[] = 
		{
			{0}
		};

		config.pInputPorts = inputs;
		config.pOutputPorts = outputs;
		config.sDescription = _HELP("This node sets RTPC values.");
		config.nFlags |= EFLN_TARGET_ENTITY;
		config.SetCategory(EFLN_APPROVED);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual void Serialize(SActivationInfo* pActInfo, TSerialize ser)
	{
		float fValue = m_fValue;
		ser.BeginGroup("FlowAudioRtpcNode");
		ser.Value("value", fValue);
		ser.EndGroup();

		if (ser.IsReading())
		{
			SetValue(pActInfo->pEntity, fValue);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
	{
		switch (event)
		{
		case eFE_Initialize:
			{
				Init(pActInfo);

				break;
			}
		case eFE_Activate:
			{
				if (IsPortActive(pActInfo, eIn_RtpcValue))
				{
					SetValue(pActInfo->pEntity, GetPortFloat(pActInfo, eIn_RtpcValue));
				}

				if (IsPortActive(pActInfo, eIn_RtpcName))
				{
					GetRtpcID(pActInfo);
				}

				break;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	virtual void GetMemoryUsage(ICrySizer * s) const
	{
		s->Add(*this);
	}

private:

	static float const EPSILON;

	//////////////////////////////////////////////////////////////////////////
	void GetRtpcID(SActivationInfo* const pActInfo)  
	{
		string const& rRtpcName = GetPortString(pActInfo, eIn_RtpcName);
		if (!rRtpcName.empty())
		{
			gEnv->pAudioSystem->GetAudioRtpcID(rRtpcName.c_str(), m_nRtpcID);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void SetValue(IEntity* const pEntity, float const fValue)
	{
		// Don't optimize here!
		// We always need to set the value as it could have been manipulated by another entity.
		m_fValue = fValue;

		if (pEntity != NULL)
		{
			SetOnProxy(pEntity, m_fValue);
		}
		else
		{
			SetOnGlobalObject(m_fValue);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void Init(SActivationInfo* const pActInfo)
	{
		if (gEnv->pAudioSystem != NULL)
		{
			GetRtpcID(pActInfo);

			m_oRequest.pData = &m_oRequestData;
			m_oRequestData.nControlID = m_nRtpcID;

			float fValue = GetPortFloat(pActInfo, eIn_RtpcValue);
			SetValue(pActInfo->pEntity, fValue);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void SetOnProxy(IEntity* const pEntity, float const fValue)
	{
		IEntityAudioProxyPtr const pIEntityAudioProxy = crycomponent_cast<IEntityAudioProxyPtr>(pEntity->CreateProxy(ENTITY_PROXY_AUDIO));
		
		if (pIEntityAudioProxy != nullptr)
		{
			pIEntityAudioProxy->SetRtpcValue(m_nRtpcID, fValue);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void SetOnGlobalObject(float const fValue)
	{
		m_oRequestData.fValue = fValue;
		gEnv->pAudioSystem->PushRequest(m_oRequest);
	}

	//////////////////////////////////////////////////////////////////////////
	float						m_fValue;
	TAudioControlID	m_nRtpcID;

	SAudioRequest m_oRequest;
	SAudioObjectRequestData<eAORT_SET_RTPC_VALUE> m_oRequestData;
};

float const CFlowNode_AudioRtpc::EPSILON = 1.0e-6;

REGISTER_FLOW_NODE("Audio:Rtpc", CFlowNode_AudioRtpc);
