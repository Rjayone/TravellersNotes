// Copyright 2001-2015 Crytek GmbH. All rights reserved.

#pragma once

#include <ATLEntityData.h>
#include <IAudioInterfacesCommonData.h>
#include <AK/SoundEngine/Common/AkTypes.h>

typedef std::vector<AkUniqueID, STLSoundAllocator<AkUniqueID> > TAKUniqueIDVector;

struct SATLAudioObjectData_wwise : public IATLAudioObjectData
{
	typedef std::map<AkAuxBusID, float, std::less<AkAuxBusID>, 
		STLSoundAllocator<std::pair<AkAuxBusID, float> > > TEnvironmentImplMap;

	SATLAudioObjectData_wwise(AkGameObjectID const nPassedAKID, bool const bPassedHasPosition)
		: bNeedsToUpdateEnvironments(false)
		, bHasPosition(bPassedHasPosition)
		, nAKID(nPassedAKID)
	{}

	bool									bNeedsToUpdateEnvironments;
	bool const						bHasPosition;
	AkGameObjectID const	nAKID;
	TEnvironmentImplMap		cEnvironemntImplAmounts;
};

struct SATLListenerData_wwise : public IATLListenerData
{
	explicit SATLListenerData_wwise(AkUniqueID const nPassedAKID)
		: nAKID(nPassedAKID)
	{}

	AkUniqueID const nAKID;
};

struct SATLTriggerImplData_wwise : public IATLTriggerImplData
{
	explicit SATLTriggerImplData_wwise(AkUniqueID const nPassedAKID)
		: nAKID(nPassedAKID)
	{}

	AkUniqueID const nAKID;
};

struct SATLRtpcImplData_wwise : public IATLRtpcImplData
{
	SATLRtpcImplData_wwise(AkRtpcID const nPassedAKID, float const m_fPassedMult, float const m_fPassedShift)
		: m_fMult(m_fPassedMult)
		, m_fShift(m_fPassedShift)
		, nAKID(nPassedAKID)
	{}

	float const m_fMult;
	float const m_fShift;
	AkRtpcID const nAKID;
};

enum EWwiseSwitchType : TATLEnumFlagsType
{
	eWST_NONE		= 0,
	eWST_SWITCH	= 1,
	eWST_STATE	= 2,
	eWST_RTPC		= 3,
};

struct SATLSwitchStateImplData_wwise : public IATLSwitchStateImplData
{
	SATLSwitchStateImplData_wwise(
		EWwiseSwitchType const ePassedType,
		AkUInt32 const nPassedAKSwitchID,
		AkUInt32 const nPassedAKStateID,
		float const fPassedRtpcValue = 0.0f)
		: eType(ePassedType)
		, nAKSwitchlID(nPassedAKSwitchID)
		, nAKStateID(nPassedAKStateID)
		, fRtpcValue(fPassedRtpcValue)
	{}

	EWwiseSwitchType const	eType;
	AkUInt32 const					nAKSwitchlID;
	AkUInt32 const					nAKStateID;
	float const							fRtpcValue;
};

enum EWwiseAudioEnvironmentType : TATLEnumFlagsType
{
	eWAET_NONE		= 0,
	eWAET_AUX_BUS	= 1,
	eWAET_RTPC		= 2,
};

struct SATLEnvironmentImplData_wwise : public IATLEnvironmentImplData
{
	explicit SATLEnvironmentImplData_wwise(EWwiseAudioEnvironmentType const ePassedType)
		: eType(ePassedType)
	{}

	SATLEnvironmentImplData_wwise(EWwiseAudioEnvironmentType const ePassedType, AkAuxBusID const nPassedAKBusID)
		: eType(ePassedType)
		, nAKBusID(nPassedAKBusID)
	{
		assert(ePassedType == eWAET_AUX_BUS);
	}

	SATLEnvironmentImplData_wwise(
		EWwiseAudioEnvironmentType const ePassedType,
		AkRtpcID const nPassedAKRtpcID,
		float const fPassedMult,
		float const fPassedShift)
		: eType(ePassedType)
		, nAKRtpcID(nPassedAKRtpcID)
		, fMult(fPassedMult)
		, fShift(fPassedShift)
	{
		assert(ePassedType == eWAET_RTPC);
	}

	EWwiseAudioEnvironmentType const	eType;

	union
	{
		// Aux Bus implementation
		struct
		{
			AkAuxBusID nAKBusID;
		};

		// Rtpc implementation
		struct
		{
			AkRtpcID	nAKRtpcID;
			float			fMult;
			float			fShift;
		};
	};
};

struct SATLEventData_wwise : public IATLEventData
{
	explicit SATLEventData_wwise(TAudioEventID const nPassedID)
		: audioEventState(eAES_NONE)
		, nAKID(AK_INVALID_UNIQUE_ID)
		, nATLID(nPassedID)
	{}

	EAudioEventState		audioEventState;
	AkUniqueID					nAKID;
	TAudioEventID const	nATLID;
};

struct SATLAudioFileEntryData_wwise : public IATLAudioFileEntryData
{
	SATLAudioFileEntryData_wwise()
		: nAKBankID(AK_INVALID_BANK_ID)
	{}

	AkBankID nAKBankID;
};
