// CryEngine Header File.
// Copyright (C), Crytek, 1999-2014.

#include "SDLMixerSoundEngine.h"
#include <CryCrc32.h>
#include <CryFile.h>
#include <CryPath.h>

#ifndef SDLMIXER_SOUND_ENGINE_UTIL_H_INCLUDED
#define SDLMIXER_SOUND_ENGINE_UTIL_H_INCLUDED

namespace SDLMixer
{
	inline const TSampleID GetIDFromString(const string& sName)
	{
		string sLowerName = sName;
		sLowerName.MakeLower();
		return CCrc32::Compute(sLowerName.c_str());
	}

	inline const TSampleID GetIDFromFilePath(const string& sFilePath)
	{
		return GetIDFromString(PathUtil::GetFile(sFilePath));
	}

	inline void GetDistanceAngleToObject(const CAudioObjectTransformation& listener, const CAudioObjectTransformation& object, float& out_nDistance, float& out_nAngle)
	{
		const Vec3 listenerToObject = object.GetPosition() - listener.GetPosition();

		// Distance
		out_nDistance = listenerToObject.len();

		// Angle
		// Project point to plane formed by the listeners position/direction
		Vec3 n = listener.GetUp().GetNormalized();
		Vec3 objectDir = Vec3::CreateProjection(listenerToObject, n).normalized();

		// Get angle between listener position and projected point
		const Vec3 listenerDir = listener.GetForward().GetNormalizedFast();
		out_nAngle = RAD2DEG(asin(objectDir.Cross(listenerDir).Dot(n)));
	}
}

#endif // SDLMIXER_SOUND_ENGINE_UTIL_H_INCLUDED
