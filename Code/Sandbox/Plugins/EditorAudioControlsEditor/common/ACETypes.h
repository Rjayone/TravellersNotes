// Copyright 2001-2015 Crytek GmbH. All rights reserved.

#pragma once

#include <vector>

namespace AudioControls
{
	enum EACEControlType
	{
		eACET_TRIGGER = 0,
		eACET_RTPC,
		eACET_SWITCH,
		eACET_SWITCH_STATE,
		eACET_ENVIRONMENT,
		eACET_PRELOAD,
		eACET_NUM_TYPES
	};

	typedef unsigned int TImplControlType;
	static const TImplControlType AUDIO_IMPL_INVALID_TYPE = 0;

	typedef unsigned int CID;
	static const CID ACE_INVALID_ID = 0;

	typedef std::vector<CID> ControlList;

	class IAudioConnection;
	typedef std::shared_ptr<IAudioConnection> TConnectionPtr;
}