// Copyright 2001-2015 Crytek GmbH. All rights reserved.

#pragma once

#include "AK/SoundEngine/Common/AkTypes.h"
#include "AK/AkWwiseSDKVersion.h"
#include <IAudioSystem.h>

#define WWISE_IMPL_BASE_PATH "/sounds/wwise/"
#define WWISE_IMPL_BANK_PATH "" // No further sub folders necessary.
#define WWISE_IMPL_BANK_FULL_PATH WWISE_IMPL_BASE_PATH WWISE_IMPL_BANK_PATH
#define WWISE_IMPL_INFO_STRING "Wwise " AK_WWISESDK_VERSIONNAME

#define ASSERT_WWISE_OK(x) (assert(x == AK_Success))
#define IS_WWISE_OK(x) (x == AK_Success)

// several wwise-specific helper functions
//////////////////////////////////////////////////////////////////////////
inline void FillAKVector(Vec3 const& vCryVector, AkVector& vAKVector)
{
	vAKVector.X = vCryVector.x;
	vAKVector.Y = vCryVector.z;
	vAKVector.Z = vCryVector.y;
}

///////////////////////////////////////////////////////////////////////////
inline void FillAKObjectPosition(CAudioObjectTransformation const& transformation, AkSoundPosition& outTransformation)
{
	FillAKVector(transformation.GetPosition(), outTransformation.Position);
	FillAKVector(transformation.GetForward(), outTransformation.Orientation);
}

///////////////////////////////////////////////////////////////////////////
inline void FillAKListenerPosition(CAudioObjectTransformation const& transformation, AkListenerPosition& outTransformation)
{
	FillAKVector(transformation.GetPosition(), outTransformation.Position);
	FillAKVector(transformation.GetForward(), outTransformation.OrientationFront);
	FillAKVector(transformation.GetUp(), outTransformation.OrientationTop);
}
