// Copyright 2001-2015 Crytek GmbH. All rights reserved.

#pragma once

#include "IAudioInterfacesCommonData.h"
#include "ILipSyncProvider.h"

// General macros.
//#define ENABLE_AUDIO_PORT_MESSAGES

#if defined(ENABLE_AUDIO_PORT_MESSAGES) && defined(_MSC_VER)
#define AUDIO_STRINGANIZE2(x) #x
#define AUDIO_STRINGANIZE1(x) AUDIO_STRINGANIZE2(x)
#define TODO(y) __pragma(message(__FILE__ "(" AUDIO_STRINGANIZE1(__LINE__) ") : " "[AUDIO] TODO >>> " AUDIO_STRINGANIZE2(y)))
#define REINST_FULL(y) __pragma(message(__FILE__ "(" AUDIO_STRINGANIZE1(__LINE__) ") : " "[AUDIO] REINST " __FUNCSIG__ " >>> " AUDIO_STRINGANIZE2(y)))
#define REINST(y)
#else
#define TODO(y)
#define REINST_FULL(y)
#define REINST(y)
#endif

// Note:
//	 Need this explicit here to prevent circular includes to IEntity.
// Summary:
//	 Unique identifier for each entity instance.
typedef unsigned int EntityId;

// Forward declarations.
struct	IVisArea;
struct	SAudioRequest;
struct	ICVar;
struct	IAudioSystemImplementation;

enum EATLDataScope : TATLEnumFlagsType
{
	eADS_NONE						= 0,
	eADS_GLOBAL					= 1,
	eADS_LEVEL_SPECIFIC	= 2,
	eADS_ALL						= 3,
};

enum EAudioManagerRequestType : TATLEnumFlagsType
{
	eAMRT_NONE											= 0,
	eAMRT_SET_AUDIO_IMPL						= BIT(0),	
	eAMRT_REFRESH_AUDIO_SYSTEM			= BIT(1),
	eAMRT_RESERVE_AUDIO_OBJECT_ID		= BIT(2),
	eAMRT_LOSE_FOCUS								= BIT(3),
	eAMRT_GET_FOCUS									= BIT(4),
	eAMRT_MUTE_ALL									= BIT(5),
	eAMRT_UNMUTE_ALL								= BIT(6),
	eAMRT_STOP_ALL_SOUNDS						= BIT(7),
	eAMRT_PARSE_CONTROLS_DATA				= BIT(8),
	eAMRT_PARSE_PRELOADS_DATA				= BIT(9),
	eAMRT_CLEAR_CONTROLS_DATA				= BIT(10),
	eAMRT_CLEAR_PRELOADS_DATA				= BIT(11),
	eAMRT_PRELOAD_SINGLE_REQUEST		= BIT(12),
	eAMRT_UNLOAD_SINGLE_REQUEST			= BIT(13),
	eAMRT_UNLOAD_AFCM_DATA_BY_SCOPE	= BIT(14),
	eAMRT_DRAW_DEBUG_INFO						= BIT(15),	// Only used internally!
	eAMRT_ADD_REQUEST_LISTENER			= BIT(16),
	eAMRT_REMOVE_REQUEST_LISTENER		= BIT(17),
	eAMRT_CHANGE_LANGUAGE						= BIT(18),
	eAMRT_RETRIGGER_AUDIO_CONTROLS	= BIT(19),
};

enum EAudioCallbackManagerRequestType : TATLEnumFlagsType
{
	eACMRT_NONE                             = 0,
	eACMRT_REPORT_STARTED_EVENT             = BIT(0), // Only relevant for delayed playback.
	eACMRT_REPORT_FINISHED_EVENT            = BIT(1), // Only used internally!
	eACMRT_REPORT_FINISHED_TRIGGER_INSTANCE = BIT(2), // Only used internally!
	eACMRT_REPORT_STARTED_FILE							= BIT(3), // Only used internally!
	eACMRT_REPORT_STOPPED_FILE							= BIT(4), // Only used internally!
	eACMRT_REPORT_PROCESSED_OBSTRUCTION_RAY = BIT(5), // Only used internally!
	eACMRT_REPORT_VIRTUALIZED_EVENT         = BIT(6), // Only used internally!
	eACMRT_REPORT_PHYSICALIZED_EVENT        = BIT(7), // Only used internally!
};

enum EAudioListenerRequestType : TATLEnumFlagsType
{
	eALRT_NONE = 0,
	eALRT_SET_POSITION = BIT(0),
};

enum EAudioObjectRequestType : TATLEnumFlagsType
{
	eAORT_NONE										= 0,
	eAORT_PREPARE_TRIGGER					= BIT(0),
	eAORT_UNPREPARE_TRIGGER				= BIT(1),
	eAORT_PLAY_FILE								= BIT(2),
	eAORT_STOP_FILE								= BIT(3),
	eAORT_EXECUTE_TRIGGER					= BIT(4),
	eAORT_STOP_TRIGGER						= BIT(5),
	eAORT_STOP_ALL_TRIGGERS				= BIT(6),
	eAORT_SET_POSITION						= BIT(7),
	eAORT_SET_RTPC_VALUE					= BIT(8),
	eAORT_SET_SWITCH_STATE				= BIT(9),
	eAORT_SET_VOLUME							= BIT(10),
	eAORT_SET_ENVIRONMENT_AMOUNT	= BIT(11),
	eAORT_RESET_ENVIRONMENTS			= BIT(12),
	eAORT_RELEASE_OBJECT					= BIT(13),
};

enum EAudioObjectObstructionCalcType : TATLEnumFlagsType
{
	eAOOCT_None				= 0,
	eAOOCT_Ignore			= 1,
	eAOOCT_SingleRay	= 2,
	eAOOCT_MultiRay		= 3,

	eAOOCT_Count,
};
AUTO_TYPE_INFO(EAudioObjectObstructionCalcType)

enum EAudioControlType : TATLEnumFlagsType
{
	eACT_NONE					= 0,
	eACT_AUDIO_OBJECT	= 1,
	eACT_TRIGGER			= 2,
	eACT_RTPC					= 3,
	eACT_SWITCH				= 4,
	eACT_SWITCH_STATE	= 5,
	eACT_PRELOAD			= 6,
	eACT_ENVIRONMENT	= 7,
};

//////////////////////////////////////////////////////////////////////////
struct SAudioManagerRequestDataBase : public SAudioRequestDataBase
{
	explicit SAudioManagerRequestDataBase(EAudioManagerRequestType const ePassedType = eAMRT_NONE)
		: SAudioRequestDataBase(eART_AUDIO_MANAGER_REQUEST)
		, eType(ePassedType)
	{}

	EAudioManagerRequestType const eType;
};

//////////////////////////////////////////////////////////////////////////
template <EAudioManagerRequestType T>
struct SAudioManagerRequestData : public SAudioManagerRequestDataBase
{
	SAudioManagerRequestData()
		: SAudioManagerRequestDataBase(T)
	{}
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioManagerRequestData<eAMRT_SET_AUDIO_IMPL> : public SAudioManagerRequestDataBase
{
	SAudioManagerRequestData(IAudioSystemImplementation* const pPassedImpl)
		: SAudioManagerRequestDataBase(eAMRT_SET_AUDIO_IMPL)
		, pImpl(pPassedImpl)
	{}

	IAudioSystemImplementation* const pImpl;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioManagerRequestData<eAMRT_RESERVE_AUDIO_OBJECT_ID> : public SAudioManagerRequestDataBase
{
	SAudioManagerRequestData(TAudioObjectID* const pPassedObjectID, char const* const sPassedObjectName = nullptr)
		: SAudioManagerRequestDataBase(eAMRT_RESERVE_AUDIO_OBJECT_ID)
		, pObjectID(pPassedObjectID)
		, sObjectName(sPassedObjectName)
	{}

	TAudioObjectID* const pObjectID;
	char const* const			sObjectName;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioManagerRequestData<eAMRT_ADD_REQUEST_LISTENER> : public SAudioManagerRequestDataBase
{
	SAudioManagerRequestData(void const* const pPassedObjectToListenTo, void (*passedfunc)(SAudioRequestInfo const* const), EAudioRequestType passedRequestType, TATLEnumFlagsType passedSpecificRequestMask)
		: SAudioManagerRequestDataBase(eAMRT_ADD_REQUEST_LISTENER)
		, pObjectToListenTo(pPassedObjectToListenTo)
		, func(passedfunc)
		, requestType(passedRequestType)
		, specificRequestMask(passedSpecificRequestMask)
	{}

	void const* const	pObjectToListenTo;
	void							(*func)(SAudioRequestInfo const* const);
	EAudioRequestType const requestType;
	TATLEnumFlagsType const specificRequestMask;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioManagerRequestData<eAMRT_REMOVE_REQUEST_LISTENER> : public SAudioManagerRequestDataBase
{
	SAudioManagerRequestData(void const* const pPassedObjectToListenTo, void (*passedfunc)(SAudioRequestInfo const* const))
		: SAudioManagerRequestDataBase(eAMRT_REMOVE_REQUEST_LISTENER)
		, pObjectToListenTo(pPassedObjectToListenTo)
		, func(passedfunc)
	{}

	void const* const	pObjectToListenTo;
	void							(*func)(SAudioRequestInfo const* const);
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioManagerRequestData<eAMRT_PARSE_CONTROLS_DATA> : public SAudioManagerRequestDataBase
{
	SAudioManagerRequestData(char const* const sConfigFolderPath, EATLDataScope const ePassedDataScope)
		: SAudioManagerRequestDataBase(eAMRT_PARSE_CONTROLS_DATA)
		, sFolderPath(sConfigFolderPath)
		, eDataScope(ePassedDataScope)
	{}

	char const* const		sFolderPath;
	EATLDataScope const	eDataScope;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioManagerRequestData<eAMRT_PARSE_PRELOADS_DATA> : public SAudioManagerRequestDataBase
{
	SAudioManagerRequestData(char const* const sConfigFolderPath, EATLDataScope const ePassedDataScope)
		: SAudioManagerRequestDataBase(eAMRT_PARSE_PRELOADS_DATA)
		, sFolderPath(sConfigFolderPath)
		, eDataScope(ePassedDataScope)
	{}

	char const* const		sFolderPath;
	EATLDataScope const	eDataScope;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioManagerRequestData<eAMRT_CLEAR_CONTROLS_DATA> : public SAudioManagerRequestDataBase
{
	SAudioManagerRequestData(EATLDataScope const ePassedDataScope = eADS_NONE)
		: SAudioManagerRequestDataBase(eAMRT_CLEAR_CONTROLS_DATA)
		, eDataScope(ePassedDataScope)
	{}

	EATLDataScope	const eDataScope;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioManagerRequestData<eAMRT_CLEAR_PRELOADS_DATA> : public SAudioManagerRequestDataBase
{
	SAudioManagerRequestData(EATLDataScope const ePassedDataScope = eADS_NONE)
		: SAudioManagerRequestDataBase(eAMRT_CLEAR_PRELOADS_DATA)
		, eDataScope(ePassedDataScope)
	{}

	EATLDataScope	const eDataScope;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioManagerRequestData<eAMRT_PRELOAD_SINGLE_REQUEST> : public SAudioManagerRequestDataBase
{
	explicit SAudioManagerRequestData(TAudioPreloadRequestID const nRequestID = INVALID_AUDIO_PRELOAD_REQUEST_ID, bool const bPassedAutoLoadOnly = false)
		: SAudioManagerRequestDataBase(eAMRT_PRELOAD_SINGLE_REQUEST)
		, nPreloadRequestID(nRequestID)
		, bAutoLoadOnly(bPassedAutoLoadOnly)
	{}

	TAudioPreloadRequestID const nPreloadRequestID;
	bool const bAutoLoadOnly;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioManagerRequestData<eAMRT_UNLOAD_SINGLE_REQUEST> : public SAudioManagerRequestDataBase
{
	explicit SAudioManagerRequestData(TAudioPreloadRequestID const nRequestID = INVALID_AUDIO_PRELOAD_REQUEST_ID)
		: SAudioManagerRequestDataBase(eAMRT_UNLOAD_SINGLE_REQUEST)
		, nPreloadRequestID(nRequestID)
	{}

	TAudioPreloadRequestID const nPreloadRequestID;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioManagerRequestData<eAMRT_UNLOAD_AFCM_DATA_BY_SCOPE> : public SAudioManagerRequestDataBase
{
	SAudioManagerRequestData(EATLDataScope const eScope = eADS_NONE)
		: SAudioManagerRequestDataBase(eAMRT_UNLOAD_AFCM_DATA_BY_SCOPE)
		, eDataScope(eScope)
	{}

	EATLDataScope	const eDataScope;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioManagerRequestData<eAMRT_REFRESH_AUDIO_SYSTEM> : public SAudioManagerRequestDataBase
{
	SAudioManagerRequestData(char const* const sPassedLevelName)
		: SAudioManagerRequestDataBase(eAMRT_REFRESH_AUDIO_SYSTEM)
		, sLevelName(sPassedLevelName)
	{}

	char const* const sLevelName;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioManagerRequestData<eAMRT_CHANGE_LANGUAGE> : public SAudioManagerRequestDataBase
{
	SAudioManagerRequestData()
		: SAudioManagerRequestDataBase(eAMRT_CHANGE_LANGUAGE)
	{}
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioManagerRequestData<eAMRT_RETRIGGER_AUDIO_CONTROLS> : public SAudioManagerRequestDataBase
{
	SAudioManagerRequestData()
		: SAudioManagerRequestDataBase(eAMRT_RETRIGGER_AUDIO_CONTROLS)
	{}
};

//////////////////////////////////////////////////////////////////////////
struct SAudioCallbackManagerRequestDataBase : public SAudioRequestDataBase
{
	explicit SAudioCallbackManagerRequestDataBase(EAudioCallbackManagerRequestType const ePassedType = eACMRT_NONE)
		: SAudioRequestDataBase(eART_AUDIO_CALLBACK_MANAGER_REQUEST)
		, eType(ePassedType)
	{}

	EAudioCallbackManagerRequestType const eType;
};

//////////////////////////////////////////////////////////////////////////
template <EAudioCallbackManagerRequestType T>
struct SAudioCallbackManagerRequestData : public SAudioCallbackManagerRequestDataBase
{
	SAudioCallbackManagerRequestData()
		: SAudioCallbackManagerRequestDataBase(T)
	{}
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioCallbackManagerRequestData<eACMRT_REPORT_STARTED_EVENT> : public SAudioCallbackManagerRequestDataBase
{
	SAudioCallbackManagerRequestData(TAudioEventID const nPassedEventID)
		: SAudioCallbackManagerRequestDataBase(eACMRT_REPORT_STARTED_EVENT)
		, nEventID(nPassedEventID)
	{}

	TAudioEventID	const	nEventID;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioCallbackManagerRequestData<eACMRT_REPORT_FINISHED_EVENT> : public SAudioCallbackManagerRequestDataBase
{
	SAudioCallbackManagerRequestData(TAudioEventID const nPassedEventID, bool const bPassedSuccess)
		: SAudioCallbackManagerRequestDataBase(eACMRT_REPORT_FINISHED_EVENT)
		, nEventID(nPassedEventID)
		, bSuccess(bPassedSuccess)
	{}

	TAudioEventID	const	nEventID;
	bool const					bSuccess;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioCallbackManagerRequestData<eACMRT_REPORT_VIRTUALIZED_EVENT> : public SAudioCallbackManagerRequestDataBase
{
	SAudioCallbackManagerRequestData(TAudioEventID const nPassedEventID)
		: SAudioCallbackManagerRequestDataBase(eACMRT_REPORT_VIRTUALIZED_EVENT)
		, nEventID(nPassedEventID)
	{}

	TAudioEventID const nEventID;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioCallbackManagerRequestData<eACMRT_REPORT_PHYSICALIZED_EVENT> : public SAudioCallbackManagerRequestDataBase
{
	SAudioCallbackManagerRequestData(TAudioEventID const nPassedEventID)
		: SAudioCallbackManagerRequestDataBase(eACMRT_REPORT_PHYSICALIZED_EVENT)
		, nEventID(nPassedEventID)
	{}

	TAudioEventID const nEventID;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioCallbackManagerRequestData<eACMRT_REPORT_FINISHED_TRIGGER_INSTANCE> : public SAudioCallbackManagerRequestDataBase
{
	SAudioCallbackManagerRequestData(TAudioControlID const nPassedControlID)
		: SAudioCallbackManagerRequestDataBase(eACMRT_REPORT_FINISHED_TRIGGER_INSTANCE)
		, nAudioTriggerID(nPassedControlID)
	{}

	TAudioControlID	const nAudioTriggerID;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioCallbackManagerRequestData<eACMRT_REPORT_STARTED_FILE> : public SAudioCallbackManagerRequestDataBase
{
	SAudioCallbackManagerRequestData(TAudioObjectID const _audioObjectID, char const* const _szFile)
		: SAudioCallbackManagerRequestDataBase(eACMRT_REPORT_STARTED_FILE)
		, audioObjectID(_audioObjectID)
		, szFile(_szFile)
	{}

	TAudioObjectID const	audioObjectID;
	char const* const			szFile;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioCallbackManagerRequestData<eACMRT_REPORT_STOPPED_FILE> : public SAudioCallbackManagerRequestDataBase
{
	SAudioCallbackManagerRequestData(TAudioObjectID const _audioObjectID, char const* const _szFile)
		: SAudioCallbackManagerRequestDataBase(eACMRT_REPORT_STOPPED_FILE)
		, audioObjectID(_audioObjectID)
		, szFile(_szFile)
	{}

	TAudioObjectID const	audioObjectID;
	char const* const			szFile;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioCallbackManagerRequestData<eACMRT_REPORT_PROCESSED_OBSTRUCTION_RAY> : public SAudioCallbackManagerRequestDataBase
{
	explicit SAudioCallbackManagerRequestData(TAudioObjectID const nPassedObjectID, size_t const nPassedRayID = (size_t)-1)
		: SAudioCallbackManagerRequestDataBase(eACMRT_REPORT_PROCESSED_OBSTRUCTION_RAY)
		, nObjectID(nPassedObjectID)
		, nRayID(nPassedRayID)
	{}

	TAudioObjectID const	nObjectID;
	size_t const					nRayID;
};

//////////////////////////////////////////////////////////////////////////
struct SAudioObjectRequestDataBase : public SAudioRequestDataBase
{
	explicit SAudioObjectRequestDataBase(EAudioObjectRequestType const ePassedType = eAORT_NONE)
		: SAudioRequestDataBase(eART_AUDIO_OBJECT_REQUEST)
		, eType(ePassedType)
	{}

	EAudioObjectRequestType const eType;
};

//////////////////////////////////////////////////////////////////////////
template <EAudioObjectRequestType T>
struct SAudioObjectRequestData : public SAudioObjectRequestDataBase
{
	SAudioObjectRequestData()
		: SAudioObjectRequestDataBase(T)
	{}
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioObjectRequestData<eAORT_PREPARE_TRIGGER> : public SAudioObjectRequestDataBase
{
	SAudioObjectRequestData()
		: SAudioObjectRequestDataBase(eAORT_PREPARE_TRIGGER)
		, nTriggerID(INVALID_AUDIO_CONTROL_ID)
	{}

	explicit SAudioObjectRequestData(TAudioControlID const nPassedTriggerID)
		: SAudioObjectRequestDataBase(eAORT_PREPARE_TRIGGER)
		, nTriggerID(nPassedTriggerID)
	{}

	TAudioControlID nTriggerID;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioObjectRequestData<eAORT_UNPREPARE_TRIGGER> : public SAudioObjectRequestDataBase
{
	SAudioObjectRequestData()
		: SAudioObjectRequestDataBase(eAORT_UNPREPARE_TRIGGER)
		, nTriggerID(INVALID_AUDIO_CONTROL_ID)
	{}

	explicit SAudioObjectRequestData(TAudioControlID const nPassedTriggerID)
		: SAudioObjectRequestDataBase(eAORT_UNPREPARE_TRIGGER)
		, nTriggerID(nPassedTriggerID)
	{}

	TAudioControlID	nTriggerID;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioObjectRequestData<eAORT_PLAY_FILE> : public SAudioObjectRequestDataBase
{
	SAudioObjectRequestData()
		: SAudioObjectRequestDataBase(eAORT_PLAY_FILE)
		, szFile(nullptr)
	{}

	explicit SAudioObjectRequestData(char const* const _szFile)
		: SAudioObjectRequestDataBase(eAORT_PLAY_FILE)
		, szFile(_szFile)
	{}

	char const* szFile;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioObjectRequestData<eAORT_STOP_FILE> : public SAudioObjectRequestDataBase
{
	SAudioObjectRequestData()
		: SAudioObjectRequestDataBase(eAORT_STOP_FILE)
		, szFile(nullptr)
	{}

	explicit SAudioObjectRequestData(char const* const _szFile)
		: SAudioObjectRequestDataBase(eAORT_STOP_FILE)
		, szFile(_szFile)
	{}

	char const* szFile;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioObjectRequestData<eAORT_EXECUTE_TRIGGER> : public SAudioObjectRequestDataBase
{
	SAudioObjectRequestData()
		: SAudioObjectRequestDataBase(eAORT_EXECUTE_TRIGGER)
		, nTriggerID(INVALID_AUDIO_CONTROL_ID)
		, fTimeUntilRemovalInMS(0.0f)
		, eLipSyncMethod(eLSM_None)
	{}

	SAudioObjectRequestData(TAudioControlID const nPassedTriggerID, float const fPassedTimeUntilRemovalInMS)
		: SAudioObjectRequestDataBase(eAORT_EXECUTE_TRIGGER)
		, nTriggerID(nPassedTriggerID)
		, fTimeUntilRemovalInMS(fPassedTimeUntilRemovalInMS)
		, eLipSyncMethod(eLSM_None)
	{}

	TAudioControlID	nTriggerID;
	float						fTimeUntilRemovalInMS;
	ELipSyncMethod	eLipSyncMethod;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioObjectRequestData<eAORT_STOP_TRIGGER> : public SAudioObjectRequestDataBase
{
	SAudioObjectRequestData()
		: SAudioObjectRequestDataBase(eAORT_STOP_TRIGGER)
		, nTriggerID(INVALID_AUDIO_CONTROL_ID)
	{}

	explicit SAudioObjectRequestData(TAudioControlID const nPassedTriggerID)
		: SAudioObjectRequestDataBase(eAORT_STOP_TRIGGER)
		, nTriggerID(nPassedTriggerID)
	{}

	TAudioControlID nTriggerID;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioObjectRequestData<eAORT_STOP_ALL_TRIGGERS> : public SAudioObjectRequestDataBase
{
	SAudioObjectRequestData()
		: SAudioObjectRequestDataBase(eAORT_STOP_ALL_TRIGGERS)
	{}
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioObjectRequestData<eAORT_SET_POSITION> : public SAudioObjectRequestDataBase
{
	SAudioObjectRequestData()
		: SAudioObjectRequestDataBase(eAORT_SET_POSITION)
		, oPosition() 
	{}

	explicit SAudioObjectRequestData(CAudioObjectTransformation const& oPassedPosition)
		: SAudioObjectRequestDataBase(eAORT_SET_POSITION)
		, oPosition(oPassedPosition) 
	{}

	CAudioObjectTransformation oPosition;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioObjectRequestData<eAORT_SET_RTPC_VALUE> : public SAudioObjectRequestDataBase
{
	SAudioObjectRequestData()
		: SAudioObjectRequestDataBase(eAORT_SET_RTPC_VALUE)
		, nControlID(INVALID_AUDIO_CONTROL_ID)
		, fValue(0.0f) 
	{}

	SAudioObjectRequestData(TAudioControlID const nPassedControlID, float const fPassedValue)
		: SAudioObjectRequestDataBase(eAORT_SET_RTPC_VALUE)
		, nControlID(nPassedControlID)
		, fValue(fPassedValue) 
	{}

	TAudioControlID	nControlID;
	float						fValue;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioObjectRequestData<eAORT_SET_SWITCH_STATE> : public SAudioObjectRequestDataBase
{
	SAudioObjectRequestData()
		: SAudioObjectRequestDataBase(eAORT_SET_SWITCH_STATE)
		, nSwitchID(INVALID_AUDIO_CONTROL_ID)
		, nStateID(INVALID_AUDIO_SWITCH_STATE_ID) 
	{}

	SAudioObjectRequestData(TAudioControlID const nPassedControlID, TAudioSwitchStateID const nPassedStateID)
		: SAudioObjectRequestDataBase(eAORT_SET_SWITCH_STATE)
		, nSwitchID(nPassedControlID)
		, nStateID(nPassedStateID) 
	{}

	TAudioControlID			nSwitchID;
	TAudioSwitchStateID	nStateID;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioObjectRequestData<eAORT_SET_VOLUME> : public SAudioObjectRequestDataBase
{
	explicit SAudioObjectRequestData(float const fPassedVolume = 1.0f)
		: SAudioObjectRequestDataBase(eAORT_SET_VOLUME)
		, fVolume(fPassedVolume)
	{}

	float const fVolume;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioObjectRequestData<eAORT_SET_ENVIRONMENT_AMOUNT> : public SAudioObjectRequestDataBase
{
	SAudioObjectRequestData()
		: SAudioObjectRequestDataBase(eAORT_SET_ENVIRONMENT_AMOUNT)
		, nEnvironmentID(INVALID_AUDIO_ENVIRONMENT_ID)
		, fAmount(1.0f)
	{}

	SAudioObjectRequestData(TAudioEnvironmentID const nPassedEnvironmentID, float const fPassedAmount)
		: SAudioObjectRequestDataBase(eAORT_SET_ENVIRONMENT_AMOUNT)
		, nEnvironmentID(nPassedEnvironmentID)
		, fAmount(fPassedAmount)
	{}

	TAudioEnvironmentID	nEnvironmentID;
	float								fAmount;
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioObjectRequestData<eAORT_RESET_ENVIRONMENTS> : public SAudioObjectRequestDataBase
{
	SAudioObjectRequestData()
		: SAudioObjectRequestDataBase(eAORT_RESET_ENVIRONMENTS)
	{}
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioObjectRequestData<eAORT_RELEASE_OBJECT> : public SAudioObjectRequestDataBase
{
	SAudioObjectRequestData()
		: SAudioObjectRequestDataBase(eAORT_RELEASE_OBJECT)
	{}
};

//////////////////////////////////////////////////////////////////////////
struct SAudioListenerRequestDataBase : public SAudioRequestDataBase
{
	explicit SAudioListenerRequestDataBase(EAudioListenerRequestType const ePassedType = eALRT_NONE)
		: SAudioRequestDataBase(eART_AUDIO_LISTENER_REQUEST)
		, eType(ePassedType)
	{}

	EAudioListenerRequestType const eType;
};

//////////////////////////////////////////////////////////////////////////
template<EAudioListenerRequestType T>
struct SAudioListenerRequestData : public SAudioListenerRequestDataBase
{
	SAudioListenerRequestData()
		: SAudioListenerRequestDataBase(T)
	{}
};

//////////////////////////////////////////////////////////////////////////
template<>
struct SAudioListenerRequestData<eALRT_SET_POSITION> : public SAudioListenerRequestDataBase
{
	SAudioListenerRequestData()
		: SAudioListenerRequestDataBase(eALRT_SET_POSITION)
	{}

	explicit SAudioListenerRequestData(CAudioObjectTransformation const& oWorldPosition)
		: SAudioListenerRequestDataBase(eALRT_SET_POSITION)
		, oNewPosition(oWorldPosition)
	{}

	CAudioObjectTransformation oNewPosition;
};

//////////////////////////////////////////////////////////////////////////
struct SAudioSystemInfo
{
	SAudioSystemInfo()
	: nCountUsedAudioTriggers(0),
		nCountUnusedAudioTriggers(0),
		nCountUsedAudioEvents(0),
		nCountUnusedAudioEvents(0)
	{}

	size_t	nCountUsedAudioTriggers;
	size_t	nCountUnusedAudioTriggers;
	size_t	nCountUsedAudioEvents;
	size_t	nCountUnusedAudioEvents;

	Vec3		oListenerPos;
};

//////////////////////////////////////////////////////////////////////////
struct IAudioProxy
{
	// <interfuscator:shuffle>
	virtual ~IAudioProxy() {}

	virtual void						Initialize(char const* const sObjectName, bool const bInitAsync = true) = 0;
	virtual void						Release() = 0;
	virtual void						Reset() = 0;
	virtual void						PlayFile(char const* const szFile, SAudioCallBackInfos const& callBackInfos = SAudioCallBackInfos::GetEmptyObject()) = 0;
	virtual void						StopFile(char const* const szFile, SAudioCallBackInfos const& callBackInfos = SAudioCallBackInfos::GetEmptyObject()) = 0;
	virtual void						ExecuteTrigger(TAudioControlID const nTriggerID, ELipSyncMethod const eLipSyncMethod, SAudioCallBackInfos const& callBackInfos = SAudioCallBackInfos::GetEmptyObject()) = 0;
	virtual void						StopTrigger(TAudioControlID const nTriggerID) = 0;
	virtual void						SetSwitchState(TAudioControlID const nSwitchID, TAudioSwitchStateID const nStateID) = 0;
	virtual void						SetRtpcValue(TAudioControlID const nRtpcID, float const fValue) = 0;
	virtual void						SetObstructionCalcType(EAudioObjectObstructionCalcType const eObstructionType) = 0;
	virtual void						SetPosition(Matrix34 const& worldTransformation) = 0;
	virtual void						SetPosition(Vec3 const& worldPos) = 0;
	virtual void						SetEnvironmentAmount(TAudioEnvironmentID const nEnvironmentID, float const fAmount) = 0;
	virtual void						SetCurrentEnvironments(EntityId const nEntityToIgnore = 0) = 0;
	virtual void						SetLipSyncProvider(ILipSyncProvider* const pILipSyncProvider) = 0;
	virtual TAudioObjectID	GetAudioObjectID() const = 0;
	// </interfuscator:shuffle>
};

//////////////////////////////////////////////////////////////////////////
struct IAudioSystem
{
	// <interfuscator:shuffle>
	virtual ~IAudioSystem() {}

	virtual bool					Initialize() = 0;
	virtual void					Release() = 0;
	virtual void					PushRequest(SAudioRequest const& rAudioRequestData) = 0;
	virtual void					AddRequestListener(void (*func)(SAudioRequestInfo const* const), void* const pObjectToListenTo, EAudioRequestType const requestType = eART_AUDIO_ALL_REQUESTS, TATLEnumFlagsType const specificRequestMask = ALL_AUDIO_REQUEST_SPECIFIC_TYPE_FLAGS) = 0;
	virtual void					RemoveRequestListener(void (*func)(SAudioRequestInfo const* const), void* const pObjectToListenTo) = 0;
	virtual void					ExternalUpdate() = 0;
	virtual bool					GetAudioTriggerID(char const* const sAudioTriggerName, TAudioControlID& rAudioTriggerID) const = 0;
	virtual bool					GetAudioRtpcID(char const* const sAudioRtpcName, TAudioControlID& rAudioRtpcID) const = 0;
	virtual bool					GetAudioSwitchID(char const* const sAudioSwitchName, TAudioControlID& rAudioSwitchID) const = 0;
	virtual bool					GetAudioSwitchStateID(TAudioControlID const nSwitchID, char const* const sAudioTriggerName, TAudioSwitchStateID& rAudioStateID) const = 0;
	virtual bool					GetAudioPreloadRequestID(char const* const sAudioPreloadRequestName, TAudioPreloadRequestID& rAudioPreloadRequestID) const = 0;
	virtual bool					GetAudioEnvironmentID(char const* const sAudioEnvironmentName, TAudioEnvironmentID& rAudioEnvironmentID) const = 0;
	virtual bool					ReserveAudioListenerID(TAudioObjectID& rAudioObjectID) = 0;
	virtual bool					ReleaseAudioListenerID(TAudioObjectID const nAudioObjectID) = 0;
	virtual void					OnCVarChanged(ICVar* const pCvar) = 0;
	virtual void					GetInfo(SAudioSystemInfo& rAudioSystemInfo) = 0;
	virtual char const*		GetConfigPath() const = 0;
	virtual IAudioProxy*	GetFreeAudioProxy() = 0;
	virtual void					FreeAudioProxy(IAudioProxy* const pIAudioProxy) = 0;
	virtual char const*		GetAudioControlName(EAudioControlType const eAudioEntityType, TATLIDType const nAudioEntityID) = 0;
	virtual char const*		GetAudioControlName(EAudioControlType const eAudioEntityType, TATLIDType const nAudioEntityID1, TATLIDType const nAudioEntityID2) = 0;
	// </interfuscator:shuffle>
};

//////////////////////////////////////////////////////////////////////////
struct IAudioListener 
{
	// <interfuscator:shuffle>
	virtual ~IAudioListener() {}

	virtual void						Init() = 0;
	virtual EntityId				GetID() const = 0;
	virtual bool						GetActive() const = 0;
	virtual void						SetActive(bool const bActive) = 0;
	virtual bool						GetMoved() const = 0;
	virtual bool						GetInside() const = 0;
	virtual void						SetInside(bool const bInside) = 0;
	virtual void						SetRecordLevel(float const fRecordLevel) = 0;
	virtual float						GetRecordLevel() const = 0;
	virtual Vec3						GetPosition() const = 0;
	virtual void						SetPosition(Vec3 const& rPosition) = 0;
	virtual Vec3 const&			GetForward() const = 0;
	virtual Vec3						GetTop() const = 0;
	virtual Vec3 const&			GetVelocity() const = 0;
	virtual void						SetVelocity(Vec3 const& vVel) = 0;
	virtual void						SetMatrix(Matrix34 const& newTransformation) = 0;
	virtual Matrix34 const&	GetMatrix() const = 0;
	virtual float						GetUnderwater() const = 0;
	virtual void						SetUnderwater(const float fUnder) = 0;
	virtual IVisArea*				GetVisArea() const = 0;
	virtual void						SetVisArea(IVisArea* const pVArea) = 0;
	// </interfuscator:shuffle>
};
