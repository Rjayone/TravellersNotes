#pragma once
// Means to serialize and edit CryExtension pointers.

#include <CryExtension/ICryFactory.h>

namespace Serialization
{

// Allows to have std::shared_ptr<TPointer> but serialize it by
// interface-casting to TSerializable, i.e. implementing Serialization through
// separate interface.
template<class TPointer, class TSerializable = TPointer>
struct CryExtensionPointer
{
	std::shared_ptr<TPointer>& ptr;

	CryExtensionPointer(std::shared_ptr<TPointer>& ptr) : ptr(ptr) {}
	void Serialize(Serialization::IArchive& ar);
};

}

// This function treats T as a type derived from CryUnknown type.
template<class T>
bool Serialize(Serialization::IArchive& ar, std::shared_ptr<T>& ptr, const char* name, const char* label);

#include "CryExtensionImpl.h"