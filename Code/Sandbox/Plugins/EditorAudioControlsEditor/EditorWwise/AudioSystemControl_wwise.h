// CryEngine Header File.
// Copyright (C), Crytek, 1999-2014.

#pragma once

#include <IAudioConnection.h>
#include <IAudioSystemControl.h>

namespace AudioControls
{
	enum EWwiseControlTypes
	{
		eWCT_INVALID = 0,
		eWCT_WWISE_EVENT = BIT(0),
		eWCT_WWISE_RTPC = BIT(1),
		eWCT_WWISE_SWITCH = BIT(2),
		eWCT_WWISE_AUX_BUS = BIT(3),
		eWCT_WWISE_SOUND_BANK = BIT(4),
		eWCT_WWISE_GAME_STATE = BIT(5),
		eWCT_WWISE_SWITCH_GROUP = BIT(6),
		eWCT_WWISE_GAME_STATE_GROUP = BIT(7),
	}; 

	class IAudioSystemControl_wwise : public IAudioSystemControl
	{
	public:
		IAudioSystemControl_wwise() {}
		IAudioSystemControl_wwise(const string& name, CID id, TImplControlType type);
		virtual ~IAudioSystemControl_wwise() {}
	};
}