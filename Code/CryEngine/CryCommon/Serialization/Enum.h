#pragma once

#include <yasli/Enum.h>

namespace Serialization
{
	using yasli::EnumDescription;
	using yasli::getEnumDescription;
}

#define SERIALIZATION_ENUM_BEGIN(Type, label) YASLI_ENUM_BEGIN(Type, label)
#define SERIALIZATION_ENUM_BEGIN_NESTED(Class, Enum, label) YASLI_ENUM_BEGIN_NESTED(Class, Enum, label)
#define SERIALIZATION_ENUM_BEGIN_NESTED2(Class1, Class2, Enum, label) YASLI_ENUM_BEGIN_NESTED2(Class1, Class2, Enum, label)
#define SERIALIZATION_ENUM(value, name, label) YASLI_ENUM(value, name, label)
#define SERIALIZATION_ENUM_END() YASLI_ENUM_END()
