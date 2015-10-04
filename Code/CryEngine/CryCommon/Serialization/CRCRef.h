// Copyright (c) 2013 Crytek GmbH
#pragma once

template <uint32 StoreStrings, typename THash>
struct SCRCRef;

#include <Serialization/Forward.h>

template <uint32 StoreStrings, typename THash>
bool Serialize(Serialization::IArchive& ar, SCRCRef<StoreStrings, THash>& crcRef, const char* name, const char* label);

#include "CRCRefImpl.h"