#pragma once

#if defined(EDITOR_COMMON_EXPORTS)
#define PROPERTY_TREE_API __declspec(dllexport)
#elif defined(EDITOR_COMMON_IMPORTS)
#define PROPERTY_TREE_API __declspec(dllimport)
#else
#define PROPERTY_TREE_API 
#endif

#define YASLI_SERIALIZE_METHOD Serialize
#define YASLI_SERIALIZE_OVERRIDE Serialize

#define YASLI_STRINGS_DEFINED
#include <platform.h>
namespace yasli {
#ifdef RESOURCE_COMPILER
typedef CryStringLocalT<char> string;
typedef CryStringLocalT<wchar_t> wstring;
#else
typedef CryStringT<char> string;
typedef CryStringT<wchar_t> wstring;
#endif
}
namespace Serialization
{
typedef yasli::string string;
typedef yasli::wstring wstring;
}

#define YASLI_STRING_NAMESPACE_BEGIN 
#define YASLI_STRING_NAMESPACE_END 

#define YASLI_ASSERT_DEFINED
#define YASLI_ASSERT(x) CRY_ASSERT(x)
#define YASLI_ASSERT_STR(x,str) CRY_ASSERT_MESSAGE(x,str)
#define YASLI_ESCAPE(x, action) if(!(x)) { YASLI_ASSERT(0 && #x); action; };
#define YASLI_CHECK(x) (x)

#define YASLI_INTS_DEFINED 1
namespace yasli
{
	typedef int8 i8;
	typedef int16 i16;
	typedef int32 i32;
	typedef int64 i64;
	typedef uint8 u8;
	typedef uint16 u16;
	typedef uint32 u32;
	typedef uint64 u64;
}

#define YASLI_INLINE_IMPLEMENTATION 1
#define YASLI_INLINE inline
