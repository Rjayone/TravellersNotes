// Copyright (c) 2012 Crytek GmbH
#pragma once

#include <Serialization/Forward.h>

template< class T, class I, class STORE>
struct DynArray;

template<class T, class I, class S>
bool Serialize(Serialization::IArchive& ar, DynArray<T, I, S>& container, const char* name, const char* label);

#include "DynArrayImpl.h"
