// Copyright 2001-2015 Crytek GmbH. All rights reserved.

#pragma once

#include "FileIOHandler_wwise.h"
#include "ATLEntities_wwise.h"
#include <IAudioSystemImplementation.h>

#if (CRY_PLATFORM_WINDOWS && CRY_PLATFORM_64BIT) && !defined(IS_EAAS)
#define WWISE_USE_OCULUS
#endif // (CRY_PLATFORM_WINDOWS && CRY_PLATFORM_64BIT) && !defined(IS_EAAS)

class CAudioSystemImpl_wwise : public IAudioSystemImplementation
{
public:
	
	CAudioSystemImpl_wwise();
	virtual ~CAudioSystemImpl_wwise();

	// IAudioSystemImplementation
	virtual void																	Update(float const fUpdateIntervalMS) override;

	virtual EAudioRequestStatus										Init() override;
	virtual EAudioRequestStatus										ShutDown() override;
	virtual EAudioRequestStatus										Release() override;
	virtual EAudioRequestStatus										OnLoseFocus() override;
	virtual EAudioRequestStatus										OnGetFocus() override;
	virtual EAudioRequestStatus										MuteAll() override;
	virtual EAudioRequestStatus										UnmuteAll() override;
	virtual EAudioRequestStatus										StopAllSounds() override;

	virtual EAudioRequestStatus										RegisterAudioObject(
																									IATLAudioObjectData* const pObjectData, 
																									char const* const sObjectName) override;
	virtual EAudioRequestStatus										RegisterAudioObject(IATLAudioObjectData* const pObjectData) override;
	virtual EAudioRequestStatus										UnregisterAudioObject(IATLAudioObjectData* const pObjectData) override;
	virtual EAudioRequestStatus										ResetAudioObject(IATLAudioObjectData* const pObjectData) override;
	virtual EAudioRequestStatus										UpdateAudioObject(IATLAudioObjectData* const pObjectData) override;

	virtual EAudioRequestStatus										PlayFile(
																									IATLAudioObjectData* const pAudioObjectData,
																									char const* const szFile) override;
	virtual EAudioRequestStatus										StopFile(
																									IATLAudioObjectData* const pAudioObjectData,
																									char const* const szFile) override;
	virtual EAudioRequestStatus										PrepareTriggerSync(
																									IATLAudioObjectData* const pAudioObjectData,
																									IATLTriggerImplData const* const pTriggerDataData) override;
	virtual EAudioRequestStatus										UnprepareTriggerSync(
																									IATLAudioObjectData* const pAudioObjectData,
																									IATLTriggerImplData const* const pTriggerData) override;
	virtual EAudioRequestStatus										PrepareTriggerAsync(
																									IATLAudioObjectData* const pAudioObjectData,
																									IATLTriggerImplData const* const pTriggerData,
																									IATLEventData* const pEventData) override;
	virtual EAudioRequestStatus										UnprepareTriggerAsync(
																									IATLAudioObjectData* const pAudioObjectData,
																									IATLTriggerImplData const* const pTriggerData,
																									IATLEventData* const pEventData) override;
	virtual EAudioRequestStatus										ActivateTrigger(
																									IATLAudioObjectData* const pAudioObjectData,
																									IATLTriggerImplData const* const pTriggerData,
																									IATLEventData* const pEventData) override;
	virtual EAudioRequestStatus										StopEvent(
																									IATLAudioObjectData* const pAudioObjectData,
																									IATLEventData const* const pEventData) override;
	virtual EAudioRequestStatus										StopAllEvents(
																									IATLAudioObjectData* const pAudioObjectData) override;
	virtual EAudioRequestStatus										SetPosition(
																									IATLAudioObjectData* const pAudioObjectData,
																									CAudioObjectTransformation const& sWorldPosition) override;
	virtual EAudioRequestStatus										SetEnvironment(
																									IATLAudioObjectData* const pAudioObjectData,
																									IATLEnvironmentImplData const* const pEnvironmentImplData,
																									float const fAmount) override;
	virtual EAudioRequestStatus										SetRtpc(
																									IATLAudioObjectData* const pAudioObjectData,
																									IATLRtpcImplData const* const pRtpcData,
																									float const fValue) override;
	virtual EAudioRequestStatus										SetSwitchState(
																									IATLAudioObjectData* const pAudioObjectData,
																									IATLSwitchStateImplData const* const pSwitchStateData) override;
	virtual EAudioRequestStatus										SetObstructionOcclusion(
																									IATLAudioObjectData* const pAudioObjectData,
																									float const fObstruction,
																									float const fOcclusion) override;
	virtual EAudioRequestStatus										SetListenerPosition(
																									IATLListenerData* const pListenerData,
																									CAudioObjectTransformation const& oNewPosition) override;

	virtual EAudioRequestStatus										RegisterInMemoryFile(SATLAudioFileEntryInfo* const pAudioFileEntry) override;
	virtual EAudioRequestStatus										UnregisterInMemoryFile(SATLAudioFileEntryInfo* const pAudioFileEntry) override;

	virtual EAudioRequestStatus										ParseAudioFileEntry(XmlNodeRef const pAudioFileEntryNode, SATLAudioFileEntryInfo* const pFileEntryInfo) override;
	virtual void																	DeleteAudioFileEntryData(IATLAudioFileEntryData* const pOldAudioFileEntryData) override;
	virtual char const* const											GetAudioFileLocation(SATLAudioFileEntryInfo* const pFileEntryInfo) override;

	virtual SATLTriggerImplData_wwise const*			NewAudioTriggerImplData(XmlNodeRef const pAudioTriggerNode) override;
	virtual void																	DeleteAudioTriggerImplData(IATLTriggerImplData const* const pOldTriggerImplData) override;

	virtual SATLRtpcImplData_wwise const*					NewAudioRtpcImplData(XmlNodeRef const pAudioRtpcNode) override;
	virtual void																	DeleteAudioRtpcImplData(IATLRtpcImplData const* const pOldRtpcImplData) override;

	virtual SATLSwitchStateImplData_wwise const*	NewAudioSwitchStateImplData(XmlNodeRef const pAudioSwitchStateImplNode) override;
	virtual void																	DeleteAudioSwitchStateImplData(IATLSwitchStateImplData const* const pOldAudioSwitchStateImplNode) override;

	virtual SATLEnvironmentImplData_wwise const*	NewAudioEnvironmentImplData(XmlNodeRef const pAudioEnvironmentNode) override;
	virtual void																	DeleteAudioEnvironmentImplData(IATLEnvironmentImplData const* const pOldEnvironmentImplData) override;

	virtual SATLAudioObjectData_wwise*						NewGlobalAudioObjectData(TAudioObjectID const nObjectID) override;
	virtual SATLAudioObjectData_wwise*						NewAudioObjectData(TAudioObjectID const nObjectID) override;
	virtual void																	DeleteAudioObjectData(IATLAudioObjectData* const pOldObjectData) override;

	virtual SATLListenerData_wwise*								NewDefaultAudioListenerObjectData() override;
	virtual SATLListenerData_wwise*								NewAudioListenerObjectData(uint const nIndex) override;
	virtual void																	DeleteAudioListenerObjectData(IATLListenerData* const pOldListenerData) override;

	virtual SATLEventData_wwise*									NewAudioEventData(TAudioEventID const nEventID) override;
	virtual void																	DeleteAudioEventData(IATLEventData* const pOldEventData) override;
	virtual void																	ResetAudioEventData(IATLEventData* const pEventData) override;

	virtual void																	GamepadConnected(TAudioGamepadUniqueID const deviceUniqueID) override;
	virtual void																	GamepadDisconnected(TAudioGamepadUniqueID const deviceUniqueID) override;

	virtual void																	OnAudioSystemRefresh() override;
	virtual void																	SetLanguage(char const* const sLanguage) override;

	// Below data is only used when INCLUDE_WWISE_IMPL_PRODUCTION_CODE is defined!
	virtual char const* const											GetImplementationNameString() const override;
	virtual void																	GetMemoryInfo(SAudioImplMemoryInfo& oMemoryInfo) const override;
	// ~IAudioSystemImplementation

private:

	static char const* const	sWwiseEventTag;
	static char const* const	sWwiseRtpcTag;
	static char const* const	sWwiseSwitchTag;
	static char const* const	sWwiseStateTag;
	static char const* const	sWwiseRtpcSwitchTag;
	static char const* const	sWwiseFileTag;
	static char const* const	sWwiseAuxBusTag;
	static char const* const	sWwiseValueTag;

	static char const* const	sWwiseNameAttribute;
	static char const* const	sWwiseValueAttribute;
	static char const* const	sWwiseMutiplierAttribute;
	static char const* const	sWwiseShiftAttribute;
	static char const* const	sWwiseLocalisedAttribute;

	struct SEnvPairCompare
	{
		bool operator() (std::pair<AkAuxBusID, float> const& oPair1, std::pair<AkAuxBusID, float> const& oPair2) const;
	};

	SATLSwitchStateImplData_wwise const*	ParseWwiseSwitchOrState(XmlNodeRef pNode,	EWwiseSwitchType eType);
	SATLSwitchStateImplData_wwise const*	ParseWwiseRtpcSwitch(XmlNodeRef pNode);
	void																	ParseRtpcImpl(XmlNodeRef pNode, AkRtpcID& rAkRtpcID, float& rMult, float& rShift);
	EAudioRequestStatus										PrepUnprepTriggerSync(
																					IATLTriggerImplData const* const pTriggerData,
																					bool bPrepare);
	EAudioRequestStatus										PrepUnprepTriggerAsync(
																					IATLTriggerImplData const* const pTriggerData,
																					IATLEventData* const pEventData,
																					bool bPrepare);

	EAudioRequestStatus										PostEnvironmentAmounts(IATLAudioObjectData* const pAudioObjectData);

	AkGameObjectID												m_nDummyGameObjectID;
	AkBankID															m_nInitBankID;
	CFileIOHandler_wwise									m_oFileIOHandler;

	CryFixedStringT<MAX_AUDIO_FILE_PATH_LENGTH>	m_sRegularSoundBankFolder;
	CryFixedStringT<MAX_AUDIO_FILE_PATH_LENGTH>	m_sLocalizedSoundBankFolder;

	typedef std::map<TAudioGamepadUniqueID, AkUInt8, std::less<TAudioGamepadUniqueID>,
		STLSoundAllocator<std::pair<TAudioGamepadUniqueID, AkUInt8> > > TAudioInputDevices;

	TAudioInputDevices m_mapInputDevices;

#if !defined(WWISE_FOR_RELEASE)
	bool m_bCommSystemInitialized;
#endif // !WWISE_FOR_RELEASE

#if defined(INCLUDE_WWISE_IMPL_PRODUCTION_CODE)
	CryFixedStringT<MAX_AUDIO_FILE_PATH_LENGTH> m_sFullImplString;
#endif // INCLUDE_WWISE_IMPL_PRODUCTION_CODE

#if defined(WWISE_USE_OCULUS)
	void* m_pOculusSpatializerLibrary;
#endif // WWISE_USE_OCULUS
};
