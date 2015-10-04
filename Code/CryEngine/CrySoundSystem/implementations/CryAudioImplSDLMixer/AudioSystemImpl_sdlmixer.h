// CryEngine Header File.
// Copyright (C), Crytek, 1999-2014.

#ifndef AUDIO_SYSTEM_IMPL_SDLMIXER_H_INCLUDED
#define AUDIO_SYSTEM_IMPL_SDLMIXER_H_INCLUDED

#include <IAudioSystemImplementation.h>
#include "SDLMixerSoundEngine.h"

class CAudioSystemImpl_sdlmixer : public IAudioSystemImplementation
{
public:

	CAudioSystemImpl_sdlmixer();
	virtual ~CAudioSystemImpl_sdlmixer();

	// IAudioSystemImplementation
	virtual void Update(float const fUpdateIntervalMS) override;

	virtual EAudioRequestStatus Init() override;
	virtual EAudioRequestStatus ShutDown() override;
	virtual EAudioRequestStatus Release() override;
	virtual void OnAudioSystemRefresh() override;
	virtual EAudioRequestStatus OnLoseFocus() override;
	virtual EAudioRequestStatus OnGetFocus() override;
	virtual EAudioRequestStatus MuteAll() override;
	virtual EAudioRequestStatus UnmuteAll() override;
	virtual EAudioRequestStatus StopAllSounds() override;

	virtual EAudioRequestStatus RegisterAudioObject(IATLAudioObjectData* const pObjectData, char const* const sObjectName) override;
	virtual EAudioRequestStatus	RegisterAudioObject(IATLAudioObjectData* const pObjectData) override;
	virtual EAudioRequestStatus	UnregisterAudioObject(IATLAudioObjectData* const pObjectData) override;
	virtual EAudioRequestStatus ResetAudioObject(IATLAudioObjectData* const pObjectData) override;
	virtual EAudioRequestStatus UpdateAudioObject(IATLAudioObjectData* const pObjectData) override;
	virtual EAudioRequestStatus PlayFile(IATLAudioObjectData* const pAudioObjectData, char const* const szFile) override;
	virtual EAudioRequestStatus StopFile(IATLAudioObjectData* const pAudioObjectData, char const* const szFile) override;
	virtual EAudioRequestStatus PrepareTriggerSync(IATLAudioObjectData* const pAudioObjectData, IATLTriggerImplData const* const pTriggerData) override;
	virtual EAudioRequestStatus UnprepareTriggerSync(IATLAudioObjectData* const pAudioObjectData, IATLTriggerImplData const* const pTriggerData) override;
	virtual EAudioRequestStatus PrepareTriggerAsync(IATLAudioObjectData* const pAudioObjectData, IATLTriggerImplData const* const pTriggerData, IATLEventData* const pEventData) override;
	virtual EAudioRequestStatus	UnprepareTriggerAsync(IATLAudioObjectData* const pAudioObjectData, IATLTriggerImplData const* const pTriggerData, IATLEventData* const pEventData) override;
	virtual EAudioRequestStatus ActivateTrigger(IATLAudioObjectData* const pAudioObjectData, IATLTriggerImplData const* const pTriggerData, IATLEventData* const pEventData) override;
	virtual EAudioRequestStatus StopEvent(IATLAudioObjectData* const pAudioObjectData, IATLEventData const* const pEventData) override;
	virtual EAudioRequestStatus StopAllEvents(IATLAudioObjectData* const pAudioObjectData) override;
	virtual EAudioRequestStatus SetPosition(IATLAudioObjectData* const pAudioObjectData, CAudioObjectTransformation const& oWorldPosition) override;
	virtual EAudioRequestStatus	SetRtpc(IATLAudioObjectData* const pAudioObjectData, IATLRtpcImplData const* const pRtpcData, float const fValue) override;
	virtual EAudioRequestStatus SetSwitchState(IATLAudioObjectData* const pAudioObjectData, IATLSwitchStateImplData const* const pSwitchStateData) override;
	virtual EAudioRequestStatus SetObstructionOcclusion(IATLAudioObjectData* const pAudioObjectData, float const fObstruction, float const fOcclusion) override;
	virtual EAudioRequestStatus SetEnvironment(IATLAudioObjectData* const pAudioObjectData, IATLEnvironmentImplData const* const pEnvironmentImplData, float const fAmount) override;
	virtual EAudioRequestStatus SetListenerPosition(IATLListenerData* const pListenerData, CAudioObjectTransformation const& oNewPosition) override;
	virtual EAudioRequestStatus RegisterInMemoryFile(SATLAudioFileEntryInfo* const pAudioFileEntry) override;
	virtual EAudioRequestStatus	UnregisterInMemoryFile(SATLAudioFileEntryInfo* const pAudioFileEntry) override;
	virtual EAudioRequestStatus ParseAudioFileEntry(XmlNodeRef const pAudioFileEntryNode, SATLAudioFileEntryInfo* const pFileEntryInfo) override;

	virtual void DeleteAudioFileEntryData(IATLAudioFileEntryData* const pOldAudioFileEntryData) override;
	virtual char const* const	GetAudioFileLocation(SATLAudioFileEntryInfo* const pFileEntryInfo) override;
	virtual IATLTriggerImplData const* NewAudioTriggerImplData(XmlNodeRef const pAudioTriggerNode) override;
	virtual void DeleteAudioTriggerImplData(IATLTriggerImplData const* const pOldTriggerImplData) override;
	virtual IATLRtpcImplData const* NewAudioRtpcImplData(XmlNodeRef const pAudioRtpcNode) override;
	virtual void DeleteAudioRtpcImplData(IATLRtpcImplData const* const pOldRtpcImplData) override;
	virtual IATLSwitchStateImplData const* NewAudioSwitchStateImplData(XmlNodeRef const pAudioSwitchStateImplNode) override;
	virtual void DeleteAudioSwitchStateImplData(IATLSwitchStateImplData const* const pOldAudioSwitchStateImplNode) override;
	virtual IATLEnvironmentImplData const* NewAudioEnvironmentImplData(XmlNodeRef const pAudioEnvironmentNode) override;
	virtual void DeleteAudioEnvironmentImplData(IATLEnvironmentImplData const* const pOldEnvironmentImplData) override;
	virtual IATLAudioObjectData* NewGlobalAudioObjectData(TAudioObjectID const nObjectID) override;
	virtual IATLAudioObjectData* NewAudioObjectData(TAudioObjectID const nObjectID) override;
	virtual void DeleteAudioObjectData(IATLAudioObjectData* const pOldObjectData) override;
	virtual IATLListenerData* NewDefaultAudioListenerObjectData() override;
	virtual IATLListenerData* NewAudioListenerObjectData(uint const nIndex) override;
	virtual void DeleteAudioListenerObjectData(IATLListenerData* const pOldListenerData) override;
	virtual IATLEventData* NewAudioEventData(TAudioEventID const nEventID) override;
	virtual void DeleteAudioEventData(IATLEventData* const pOldEventData) override;
	virtual void ResetAudioEventData(IATLEventData* const pEventData) override;
	virtual void GamepadConnected(TAudioGamepadUniqueID const deviceUniqueID) override;
	virtual void GamepadDisconnected(TAudioGamepadUniqueID const deviceUniqueID) override;
	virtual void SetLanguage(char const* const sLanguage) override;
	virtual char const* const GetImplementationNameString() const override;
	virtual void GetMemoryInfo(SAudioImplMemoryInfo& oMemoryInfo) const override;
	// ~IAudioSystemImplementation

private:

	static char const* const	ms_sSDLFileTag;
	static char const* const	ms_sSDLEventTag;
	static char const* const	ms_sSDLCommonAttribute;
	static char const* const	ms_sSDLSoundLibraryPath;
	static char const* const	ms_sSDLEventTypeTag;
	static char const* const	ms_sSDLEventPanningEnabledTag;
	static char const* const	ms_sSDLEventAttenuationEnabledTag;
	static char const* const	ms_sSDLEventAttenuationMinDistanceTag;
	static char const* const	ms_sSDLEventAttenuationMaxDistanceTag;
	static char const* const	ms_sSDLEventVolumeTag;
	static char const* const	ms_sSDLEventLoopCountTag;
	static char const* const	ms_sSDLEventIdTag;

	string m_sGameFolder;
	size_t m_nMemoryAlignment;

#if defined(INCLUDE_SDLMIXER_IMPL_PRODUCTION_CODE)
	std::map<TAudioObjectID, string> m_idToName;
	CryFixedStringT<MAX_AUDIO_FILE_PATH_LENGTH> m_sFullImplString;
#endif // INCLUDE_SDLMIXER_IMPL_PRODUCTION_CODE

};

#endif // AUDIO_SYSTEM_IMPL_SDLMIXER_H_INCLUDED
