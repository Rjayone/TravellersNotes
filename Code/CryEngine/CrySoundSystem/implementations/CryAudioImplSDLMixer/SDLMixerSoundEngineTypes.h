// CryEngine Header File.
// Copyright (C), Crytek, 1999-2014.

#ifndef SDLMIXER_SOUND_ENGINE_TYPES_H_INCLUDED
#define SDLMIXER_SOUND_ENGINE_TYPES_H_INCLUDED

#include <IAudioSystemImplementation.h>
#include <STLSoundAllocator.h>

namespace SDLMixer
{
	typedef uint TSampleID;
	typedef uint TEventID;
	typedef uint TListenerID;
}

typedef std::set<int, std::less<int>, STLSoundAllocator<int> > TChannelSet;

struct SSDLMixerEventStaticData : public IATLTriggerImplData
{
	explicit SSDLMixerEventStaticData(const SDLMixer::TEventID nID)
		: nSDLMixerEventID(nID)
		, nSampleID(0)
		, fAttenuationMinDistance(0)
		, fAttenuationMaxDistance(100)
		, nVolume(128)
		, nLoopCount(1)
		, bPanningEnabled(true)
		, bStartEvent(true)
	{}
	const SDLMixer::TEventID nSDLMixerEventID;
	SDLMixer::TSampleID nSampleID;
	float fAttenuationMinDistance;
	float fAttenuationMaxDistance;
	int nVolume;
	int nLoopCount;
	bool bPanningEnabled;
	bool bStartEvent;
};

struct SATLRtpcImplData_sdlmixer : public IATLRtpcImplData
{
	// Empty implementation so that the engine has something
	// to refer to since RTPCs are not currently supported by
	// the SDL Mixer implementation
};

struct SATLSwitchStateImplData_sdlmixer : public IATLSwitchStateImplData
{
	// Empty implementation so that the engine has something
	// to refer to since switches are not currently supported by
	// the SDL Mixer implementation
};

struct SATLEnvironmentImplData_sdlmixer : public IATLEnvironmentImplData
{
	// Empty implementation so that the engine has something
	// to refer to since environments are not currently supported by
	// the SDL Mixer implementation
};

struct SSDLMixerEventInstanceData : public IATLEventData
{
	explicit SSDLMixerEventInstanceData(TAudioEventID const nPassedID)
		: nEventID(nPassedID)
		, pStaticData(nullptr)
	{}

	void Reset()
	{
		channels.clear();
		pStaticData = nullptr;
	}
	const TAudioEventID nEventID;
	TChannelSet channels;
	const SSDLMixerEventStaticData* pStaticData;
};
typedef std::set<SSDLMixerEventInstanceData*, std::less<SSDLMixerEventInstanceData*>, STLSoundAllocator<SSDLMixerEventInstanceData*> > TEventInstanceSet;

struct SSDLMixerAudioObjectData : public IATLAudioObjectData
{
	SSDLMixerAudioObjectData(TAudioObjectID nID, bool bIsGlobal)
		: nObjectID(nID)
		, bGlobal(bIsGlobal)
		, bPositionChanged(false) {}

	const TAudioObjectID nObjectID;
	CAudioObjectTransformation position;
	TEventInstanceSet events;
	bool bGlobal;
	bool bPositionChanged;
};

struct SATLListenerData : public IATLListenerData
{
	explicit SATLListenerData(const SDLMixer::TListenerID nID)
		: nListenerID(nID)
	{}

	const SDLMixer::TListenerID nListenerID;
};

struct SSDLMixerAudioFileEntryData : public IATLAudioFileEntryData
{
	SSDLMixerAudioFileEntryData()
	{}

	SDLMixer::TSampleID nSampleID;
};

#endif // SDLMIXER_SOUND_ENGINE_TYPES_H_INCLUDED
