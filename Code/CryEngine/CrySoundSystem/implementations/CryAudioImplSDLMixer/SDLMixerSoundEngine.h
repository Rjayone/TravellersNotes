// CryEngine Header File.
// Copyright (C), Crytek, 1999-2014.

#ifndef SDLMIXER_SOUND_ENGINE_H_INCLUDED
#define SDLMIXER_SOUND_ENGINE_H_INCLUDED

#include "SDLMixerSoundEngineTypes.h"

namespace SDLMixer
{
	namespace SoundEngine
	{
		typedef void (*TFnEventCallback)(TAudioEventID);

		// Global events
		bool Init();
		void Release();
		void Refresh();
		void Update();

		void Pause();
		void Resume();

		void Mute();
		void UnMute();
		void Stop();

		// Load / Unload samples
		const TSampleID LoadSample(const string& sSampleFilePath);
		const TSampleID LoadSample(void* pMemory, const size_t nSize, const string& sSamplePath);
		void UnloadSample(const TSampleID nID);

		// Events
		SSDLMixerEventStaticData* CreateEventData(const TEventID nEventID);
		bool ExecuteEvent(SSDLMixerAudioObjectData* const pAudioObject, SSDLMixerEventStaticData const* const pEventStaticData, SSDLMixerEventInstanceData* const pEventInstance);
		// stops an specific event instance
		bool StopEvent(SSDLMixerEventInstanceData const* const pEventInstance);
		// stops all the events of this type
		bool StopEvents(const SDLMixer::TEventID nEventID);

		// Listeners
		bool SetListenerPosition(const TEventID nListenerID, const CAudioObjectTransformation& position);

		// Audio Objects
		bool RegisterAudioObject(SSDLMixerAudioObjectData* pAudioObjectData);
		bool UnregisterAudioObject(SSDLMixerAudioObjectData* pAudioObjectData);
		bool SetAudioObjectPosition(SSDLMixerAudioObjectData* pAudioObjectData, const CAudioObjectTransformation& position);

		// Callbacks
		void RegisterEventFinishedCallback(TFnEventCallback pCallbackFunction);
	}
}

#endif // SDLMIXER_SOUND_ENGINE_H_INCLUDED
