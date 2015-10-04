// Copyright 2001-2015 Crytek GmbH. All rights reserved.

#pragma once

#include "Cry_Math.h"
#include "BaseTypes.h"
#include "smartptr.h"

typedef uint32 TATLIDType;

typedef TATLIDType TAudioObjectID;
#define INVALID_AUDIO_OBJECT_ID ((TAudioObjectID)(0))
#define GLOBAL_AUDIO_OBJECT_ID ((TAudioObjectID)(1))
typedef TATLIDType TAudioControlID;
#define INVALID_AUDIO_CONTROL_ID ((TAudioControlID)(0))
typedef TATLIDType TAudioSwitchStateID;
#define INVALID_AUDIO_SWITCH_STATE_ID ((TAudioSwitchStateID)(0))
typedef TATLIDType TAudioEnvironmentID;
#define INVALID_AUDIO_ENVIRONMENT_ID ((TAudioEnvironmentID)(0))
typedef TATLIDType TAudioPreloadRequestID;
#define INVALID_AUDIO_PRELOAD_REQUEST_ID ((TAudioPreloadRequestID)(0))
typedef TATLIDType TAudioEventID;
#define INVALID_AUDIO_EVENT_ID ((TAudioEventID)(0))
typedef TATLIDType TAudioFileEntryID;
#define INVALID_AUDIO_FILE_ENTRY_ID ((TAudioFileEntryID)(0))
typedef TATLIDType TAudioTriggerImplID;
#define INVALID_AUDIO_TRIGGER_IMPL_ID ((TAudioTriggerImplID)(0))
typedef TATLIDType TAudioTriggerInstanceID;
#define INVALID_AUDIO_TRIGGER_INSTANCE_ID ((TAudioTriggerInstanceID)(0))
typedef TATLIDType TATLEnumFlagsType;
#define INVALID_AUDIO_ENUM_FLAG_TYPE ((TATLEnumFlagsType)(0))
#define ALL_AUDIO_REQUEST_SPECIFIC_TYPE_FLAGS ((TATLEnumFlagsType)(0xFFFFFFFF))
typedef TATLIDType TAudioProxyID;
#define INVALID_AUDIO_PROXY_ID ((TAudioProxyID)(0))
#define DEFAULT_AUDIO_PROXY_ID ((TAudioProxyID)(1))

class CAudioObjectTransformation
{	
public:
	CAudioObjectTransformation()
		: up(Vec3Constants<float>::fVec3_OneZ)
		, forward(Vec3Constants<float>::fVec3_OneY)
		, position(ZERO)
	{}

	CAudioObjectTransformation(Vec3 const& rPos)
		: up(Vec3Constants<float>::fVec3_OneZ)
		, forward(Vec3Constants<float>::fVec3_OneY)
		, position(rPos)
	{
	}

	CAudioObjectTransformation(Matrix34 const& rTransformation)
		: forward(rTransformation.GetColumn1())  //assuming forward vector = (0,1,0), also assuming unscaled
		, up(rTransformation.GetColumn2())  //assuming up vector = (0,0,1)
		, position(rTransformation.GetColumn3())
	{
		forward.NormalizeFast();
		up.NormalizeFast();
	}

	ILINE Vec3 const& GetPosition() const { return position; }
	ILINE Vec3 const& GetUp() const { return up; }
	ILINE Vec3 const& GetForward() const { return forward; }

private:
	Vec3 position;
	Vec3 up;
	Vec3 forward;
};

#define AUDIO_TRIGGER_IMPL_ID_NUM_RESERVED 100// IDs below that value are used for the CATLTriggerImpl_Internal

#define	MAX_AUDIO_CONTROL_NAME_LENGTH	64
#define	MAX_AUDIO_FILE_NAME_LENGTH		128
#define	MAX_AUDIO_FILE_PATH_LENGTH		256
#define	MAX_AUDIO_OBJECT_NAME_LENGTH	256
#define	MAX_AUDIO_MISC_STRING_LENGTH	512

enum EAudioRequestFlags : TATLEnumFlagsType
{
	eARF_NONE										= 0,
	eARF_PRIORITY_NORMAL				= BIT(0),
	eARF_PRIORITY_HIGH					= BIT(1),
	eARF_EXECUTE_BLOCKING				= BIT(2),
	eARF_SYNC_CALLBACK					= BIT(3),
	eARF_SYNC_FINISHED_CALLBACK	= BIT(4),
	eARF_STAY_IN_MEMORY					= BIT(5),
	eARF_THREAD_SAFE_PUSH				= BIT(6),
};

enum EAudioRequestType : TATLEnumFlagsType
{
	eART_NONE														= 0,
	eART_AUDIO_MANAGER_REQUEST					= 1,
	eART_AUDIO_CALLBACK_MANAGER_REQUEST	= 2,
	eART_AUDIO_OBJECT_REQUEST						= 3,
	eART_AUDIO_LISTENER_REQUEST					= 4,
	eART_AUDIO_ALL_REQUESTS							= 0xFFFFFFFF,
};

enum EAudioRequestResult : TATLEnumFlagsType
{
	eARR_NONE			= 0,
	eARR_SUCCESS	= 1,
	eARR_FAILURE	= 2,
};

enum EAudioEventState : TATLEnumFlagsType
{
	eAES_NONE             = 0,
	eAES_PLAYING          = 1,
	eAES_PLAYING_DELAYED  = 2,
	eAES_LOADING          = 3,
	eAES_UNLOADING        = 4,
	eAES_VIRTUAL          = 5,
};

struct SAudioRequestDataBase
{
	explicit SAudioRequestDataBase(EAudioRequestType const eType = eART_NONE)
		: eRequestType(eType)
	{}

	virtual ~SAudioRequestDataBase() {}

	EAudioRequestType const	eRequestType;
};

struct SAudioCallBackInfos
{
	SAudioCallBackInfos(SAudioCallBackInfos const& rOther)
		: pObjectToNotify(rOther.pObjectToNotify)
		, pUserData(rOther.pUserData)
		, pUserDataOwner(rOther.pUserDataOwner)
		, nRequestFlags(rOther.nRequestFlags)
	{}

	explicit SAudioCallBackInfos(
		void* const pPassedObjectToNotify = nullptr,
		void* const pPassedUserData = nullptr,
		void* const pPassedUserDataOwner = nullptr,
		TATLEnumFlagsType const nPassedRequestFlags = eARF_PRIORITY_NORMAL)
		: pObjectToNotify(pPassedObjectToNotify)
		, pUserData(pPassedUserData)
		, pUserDataOwner(pPassedUserDataOwner)
		, nRequestFlags(nPassedRequestFlags)
	{}

	static const SAudioCallBackInfos& GetEmptyObject() { static SAudioCallBackInfos emptyInstance; return emptyInstance; }

	void* const							pObjectToNotify;
	void* const							pUserData;
	void* const							pUserDataOwner;
	TATLEnumFlagsType const	nRequestFlags;
};

struct SAudioRequest
{
	SAudioRequest()
		: nFlags(eARF_NONE)
		, nAudioObjectID(INVALID_AUDIO_OBJECT_ID)
		, pOwner(nullptr)
		, pUserData(nullptr)
		, pUserDataOwner(nullptr)
		, pData(nullptr)
	{}

	~SAudioRequest() {}

	TATLEnumFlagsType				nFlags;
	TAudioObjectID					nAudioObjectID;
	void*										pOwner;
	void*										pUserData;
	void*										pUserDataOwner;
	SAudioRequestDataBase*	pData;

private:

	SAudioRequest(SAudioRequest const& other);
	SAudioRequest& operator=(SAudioRequest const& other);
};

struct SAudioRequestInfo
{
	explicit SAudioRequestInfo(
		EAudioRequestResult const _requestResult,
		void* const	_pOwner,
		void* const _pUserData,
		void* const _pUserDataOwner,
		char const* const _szValue,
		EAudioRequestType const _audioRequestType,
		TATLEnumFlagsType const _specificAudioRequest,
		TAudioControlID const _audioControlID,
		TAudioObjectID const _audioObjectID,
		TAudioObjectID const _reportingAudioObjectID,
		TAudioEventID const _audioEventID)
		: requestResult(_requestResult)
		, pOwner(_pOwner)
		, pUserData(_pUserData)
		, pUserDataOwner(_pUserDataOwner)
		, szValue(_szValue)
		, audioRequestType(_audioRequestType)
		, specificAudioRequest(_specificAudioRequest)
		, audioControlID(_audioControlID)
		, audioObjectID(_audioObjectID)
		, reportingAudioObjectID(_reportingAudioObjectID)
		, audioEventID(_audioEventID)
	{}

	EAudioRequestResult const	requestResult;
	void* const								pOwner;
	void* const								pUserData;
	void* const								pUserDataOwner;
	char const* const					szValue;
	EAudioRequestType const		audioRequestType;
	TATLEnumFlagsType	const		specificAudioRequest;
	TAudioControlID const			audioControlID;
	TAudioObjectID const			audioObjectID;
	TAudioObjectID const			reportingAudioObjectID;
	TAudioEventID const				audioEventID;
};
