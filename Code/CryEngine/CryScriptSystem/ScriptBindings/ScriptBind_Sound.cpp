// CryEngine Source File.
// Copyright (C), Crytek, 1999-2014.

#include "StdAfx.h"
#include "ScriptBind_Sound.h"
#include <IAudioSystem.h>

CScriptBind_Sound::CScriptBind_Sound(IScriptSystem* pScriptSystem, ISystem* pSystem)
{
	CScriptableBase::Init(pScriptSystem, pSystem);
	SetGlobalName("Sound");

#undef SCRIPT_REG_CLASSNAME 
#define SCRIPT_REG_CLASSNAME &CScriptBind_Sound::

	// AudioSystem
	SCRIPT_REG_TEMPLFUNC(GetAudioTriggerID, "sTriggerName");
	SCRIPT_REG_TEMPLFUNC(GetAudioSwitchID, "sSwitchName");
	SCRIPT_REG_TEMPLFUNC(GetAudioSwitchStateID, "hSwitchID, sStateName");
	SCRIPT_REG_TEMPLFUNC(GetAudioRtpcID, "sRtpcName");
	SCRIPT_REG_TEMPLFUNC(GetAudioEnvironmentID, "sEnvironmentName");
	SCRIPT_REG_TEMPLFUNC(SetAudioRtpcValue, "hRtpcID, fValue");

}

//////////////////////////////////////////////////////////////////////////
CScriptBind_Sound::~CScriptBind_Sound()
{
}

///////////////////////////////////////////////////////////////////////////
int CScriptBind_Sound::GetAudioTriggerID(IFunctionHandler* pH, char const* const sTriggerName)
{
	if ((sTriggerName != nullptr) && (sTriggerName[0] != '\0'))
	{
		TAudioControlID nTriggerID = INVALID_AUDIO_CONTROL_ID;
		if (gEnv->pAudioSystem->GetAudioTriggerID(sTriggerName, nTriggerID))
		{
			// ID retrieved successfully
			return pH->EndFunction(IntToHandle(nTriggerID));
		}
		else
		{
			return pH->EndFunction();
		}
	}

	return pH->EndFunction();
}

///////////////////////////////////////////////////////////////////////////
int CScriptBind_Sound::GetAudioSwitchID(IFunctionHandler* pH, char const* const sSwitchName)
{
	if ((sSwitchName != nullptr) && (sSwitchName[0] != '\0'))
	{
		TAudioControlID nSwitchID = INVALID_AUDIO_CONTROL_ID;
		if (gEnv->pAudioSystem->GetAudioSwitchID(sSwitchName, nSwitchID))
		{
			// ID retrieved successfully
			return pH->EndFunction(IntToHandle(nSwitchID));
		}
		else
		{
			return pH->EndFunction();
		}
	}

	return pH->EndFunction();
}

///////////////////////////////////////////////////////////////////////////
int CScriptBind_Sound::GetAudioSwitchStateID(IFunctionHandler* pH, ScriptHandle const hSwitchID, char const* const sSwitchStateName)
{
	if ((sSwitchStateName != nullptr) && (sSwitchStateName[0] != '\0'))
	{
		TAudioSwitchStateID nSwitchStateID = INVALID_AUDIO_SWITCH_STATE_ID;
		TAudioControlID nSwitchID = HandleToInt<TAudioControlID>(hSwitchID);
		if (gEnv->pAudioSystem->GetAudioSwitchStateID(nSwitchID, sSwitchStateName, nSwitchStateID))
		{
			// ID retrieved successfully
			return pH->EndFunction(IntToHandle(nSwitchStateID));
		}
		else
		{
			return pH->EndFunction();
		}
	}

	return pH->EndFunction();
}

///////////////////////////////////////////////////////////////////////////
int CScriptBind_Sound::GetAudioRtpcID(IFunctionHandler* pH, char const* const sRtpcName)
{
	if ((sRtpcName != nullptr) && (sRtpcName[0] != '\0'))
	{
		TAudioControlID nRtpcID = INVALID_AUDIO_CONTROL_ID;
		if (gEnv->pAudioSystem->GetAudioRtpcID(sRtpcName, nRtpcID))
		{
			// ID retrieved successfully
			return pH->EndFunction(IntToHandle(nRtpcID));
		}
		else
		{
			return pH->EndFunction();
		}
	}

	return pH->EndFunction();
}

///////////////////////////////////////////////////////////////////////////
int CScriptBind_Sound::GetAudioEnvironmentID(IFunctionHandler* pH, char const* const sEnvironmentName)
{
	if ((sEnvironmentName != nullptr) && (sEnvironmentName[0] != '\0'))
	{
		TAudioEnvironmentID nEnvironmentID = INVALID_AUDIO_ENVIRONMENT_ID;
		if (gEnv->pAudioSystem->GetAudioEnvironmentID(sEnvironmentName, nEnvironmentID))
		{
			// ID retrieved successfully
			return pH->EndFunction(IntToHandle(nEnvironmentID));
		}
		else
		{
			return pH->EndFunction();
		}
	}

	return pH->EndFunction();
}

//////////////////////////////////////////////////////////////////////////
int CScriptBind_Sound::SetAudioRtpcValue(IFunctionHandler* pH, ScriptHandle const hRtpcID, float const fValue)
{
	SAudioRequest request;
	SAudioObjectRequestData<eAORT_SET_RTPC_VALUE> requestData(HandleToInt<TAudioControlID>(hRtpcID), static_cast<float>(fValue));

	request.nFlags = eARF_PRIORITY_NORMAL;
	request.pData = &requestData;

	gEnv->pAudioSystem->PushRequest(request);

	return pH->EndFunction();
}
