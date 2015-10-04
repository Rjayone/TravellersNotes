// Copyright 2001-2015 Crytek GmbH. All rights reserved.

#pragma once

#include <IScriptSystem.h>

struct IAudioSystem;

/*! <remarks>These function will never be called from C-Code. They're script-exclusive.</remarks>*/
class CScriptBind_Sound : public CScriptableBase
{
public:

	CScriptBind_Sound(IScriptSystem* pScriptSystem, ISystem* pSystem);
	virtual ~CScriptBind_Sound();

	virtual void GetMemoryUsage(ICrySizer *pSizer) const
	{
		pSizer->AddObject(this, sizeof(*this));
	}

	//! <code>Sound.GetAudioTriggerID(char const* const sTriggerName)</code>
	//! <description>Get the trigger TAudioControlID (wrapped into a ScriptHandle).</description>
	//!		<param name="sTriggerName">unique name of an audio trigger</param>
	//! <returns>ScriptHandle with the TAudioControlID value, or nil if the sTriggerName is not found.</returns>
	int GetAudioTriggerID(IFunctionHandler* pH, char const* const sTriggerName);

	//! <code>Sound.GetAudioSwitchID(char const* const sSwitchName)</code>
	//! <description>Get the switch TAudioControlID (wrapped into a ScriptHandle).</description>
	//!		<param name="sSwitchName">unique name of an audio switch</param>
	//! <returns>ScriptHandle with the TAudioControlID value, or nil if the sSwitchName is not found.</returns>
	int GetAudioSwitchID(IFunctionHandler* pH, char const* const sSwitchName);

	//! <code>Sound.GetAudioSwitchStateID(ScriptHandle const hSwitchID, char const* const sSwitchStateName)</code>
	//! <description>Get the SwitchState TAudioSwitchStatelID (wrapped into a ScriptHandle).</description>
	//!		<param name="sSwitchStateName">unique name of an audio switch state</param>
	//! <returns>ScriptHandle with the TAudioSwitchStateID value, or nil if the sSwitchStateName is not found.</returns>
	int GetAudioSwitchStateID(IFunctionHandler* pH, ScriptHandle const hSwitchID, char const* const sSwitchStateName);

	//! <code>Sound.GetAudioRtpcID(char const* const sRtpcName)</code>
	//! <description>Get the RTPC TAudioControlID (wrapped into a ScriptHandle).</description>
	//!		<param name="sRtpcName">unique name of an audio RTPC</param>
	//! <returns>ScriptHandle with the TAudioControlID value, or nil if the sRtpcName is not found.</returns>
	int GetAudioRtpcID(IFunctionHandler* pH, char const* const sRtpcName);

	//! <code>Sound.GetAudioEnvironmentID(char const* const sEnvironmentName)</code>
	//! <description>Get the Audio Environment TAudioEnvironmentID (wrapped into a ScriptHandle).</description>
	//!		<param name="sEnvironmentName">unique name of an Audio Environment</param>
	//! <returns>ScriptHandle with the TAudioEnvironmentID value, or nil if the sEnvironmentName is not found.</returns>
	int GetAudioEnvironmentID(IFunctionHandler* pH, char const* const sEnvironmentName);

	//! <code>Sound.SetAudioRtpcValue( hRtpcID, fValue )</code>
	//! <description>Globally sets the specified audio RTPC to the specified value</description>
	//!		<param name="hRtpcID">the audio RTPC ID handle</param>
	//!		<param name="fValue">the RTPC value</param>
	//! <returns>nil</returns>
	int SetAudioRtpcValue(IFunctionHandler* pH, ScriptHandle const hRtpcID, float const fValue);
	
};
