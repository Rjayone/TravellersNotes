// Copyright 2001-2015 Crytek GmbH. All rights reserved.

/************************************************************************

This class initiates from a given string, and stores the hashed Representation of that string
Plus: It can store a copy of the original string for debug purposes

/************************************************************************/

#ifndef _HASHED_STRING_H_
#define _HASHED_STRING_H_

#include <CryCrc32.h>
#include <Serialization/STL.h>
#include <Serialization/IArchive.h>

#if !defined(_RELEASE)
	#define HASHEDSTRING_STORES_SOURCE_STRING  //for debug purposes, it is often interesting to see from which string the hash was created
	#define HASHEDSTRING_CHECKS_FOR_HASH_CONFLICTS  //will compare the strings if the hash-values are equal, and assert if the hash-values match but the strings dont.
#endif

class CHashedString
{
	static const uint32 INVALID_HASH = 0; //therefore empty strings will cause an invalid hashedString

public:
	static const CHashedString& GetEmptyHashedString() 
	{ 
		const static CHashedString emptyInstance;
		return emptyInstance;
	}

	CHashedString()
	{
		m_hash = INVALID_HASH;
	}

	explicit CHashedString(const uint32 hash)
	{
		m_hash = hash;
#if defined(HASHEDSTRING_STORES_SOURCE_STRING)
		m_textCopy = GetHashAsString();
#endif
	}

	CHashedString(const char* szText)
	{
#if defined(HASHEDSTRING_STORES_SOURCE_STRING)
		m_textCopy = szText;
#endif
		m_hash = CreateHash(szText);
	}

	CHashedString(const string& text) 
	{
#if defined(HASHEDSTRING_STORES_SOURCE_STRING)
		m_textCopy = text;
#endif
		m_hash = CreateHash(text.c_str());
	}

	void Clear()
	{
#if defined(HASHEDSTRING_STORES_SOURCE_STRING)
		m_textCopy.clear();
#endif
		m_hash = INVALID_HASH;
	}

	CHashedString& operator=(const char* szText)
	{
#if defined(HASHEDSTRING_STORES_SOURCE_STRING)
		m_textCopy = szText;
#endif
		m_hash = CreateHash(szText);
		return *this;
	}

	const char* GetSourceText() const
	{
#if defined(HASHEDSTRING_STORES_SOURCE_STRING)
		return m_textCopy.c_str();
#else
		return nullptr;
#endif
	}

	string GetText() const
	{
#if defined(HASHEDSTRING_STORES_SOURCE_STRING)
		return m_textCopy;
#else
		return GetHashAsString();
#endif
	}

	bool IsValid() const
	{
		return m_hash != INVALID_HASH;
	}
	
	uint32 GetHash() const
	{
		return m_hash;
	}

	string GetHashAsString() const
	{
		char buffer[16];
		sprintf(buffer, "0x%08x", m_hash);
		return buffer;
	}

	bool operator==(const char* szText) const
	{
#if defined(HASHEDSTRING_CHECKS_FOR_HASH_CONFLICTS)
		return *this == CHashedString(szText);
#endif
		return m_hash == CreateHash(szText);
	}

	bool operator==(const CHashedString& other) const
	{
#if defined(HASHEDSTRING_CHECKS_FOR_HASH_CONFLICTS)
		if (m_hash == other.m_hash)
		{
			assert(m_textCopy.compareNoCase(other.m_textCopy) == 0 || m_textCopy == other.GetHashAsString() || GetHashAsString() == other.m_textCopy);
			return true;
		}
		else
		{
			return false;
		}
#endif
		return m_hash == other.m_hash;
	}

	bool operator!=(const CHashedString& other) const
	{
		return m_hash != other.m_hash;
	}

	bool operator<(const CHashedString& other) const
	{
		return m_hash < other.m_hash;
	}  

	operator uint32() const
	{
		return m_hash;
	}

protected:

	static uint32 CreateHash(const char* szStringToHash)
	{
		if (szStringToHash == NULL)
		{
			return INVALID_HASH;
		}
		if (szStringToHash[0] == '0' && szStringToHash[1] == 'x')  //if the given string already starts with 0x..., we assume that it is already a hashCode, stored in a string. we therefore simply convert it back
		{
			return strtoul(szStringToHash+2, 0, 16);
		}
		return CCrc32::ComputeLowercase(szStringToHash);
	}

	uint32 m_hash;

#if defined(HASHEDSTRING_STORES_SOURCE_STRING)
	string m_textCopy;
#endif
	friend bool Serialize(Serialization::IArchive& ar, CHashedString& str, const char* name, const char* label);
};

namespace std 
{
	// Hash computation for std::unordered_map etc.
	template<> struct hash<CHashedString>
	{
		size_t operator ()(const CHashedString& s) const { return s.GetHash(); }
	};
}

inline bool Serialize(Serialization::IArchive& ar, CHashedString& str, const char* name, const char* label)
{
	if (ar.caps(Serialization::IArchive::BINARY))
	{
#if defined(HASHEDSTRING_STORES_SOURCE_STRING)
		if(!ar(str.m_hash, name, label))
			return false;
		if (ar.isInput())
			str.m_textCopy = str.GetHashAsString();
		return true;
#else
		return ar(str.m_hash, name, label);
#endif
	}

#if defined(HASHEDSTRING_STORES_SOURCE_STRING)
	if (!ar(str.m_textCopy, name, label))
		return false;
	if (ar.isInput())
		str.m_hash = CHashedString::CreateHash(str.m_textCopy.c_str());
#else
	string temp;
	if (ar.isOutput())
		temp = str.GetText();  //we serialize our hash code as a string
	if (!ar(temp, name, label))
		return false;
	if (ar.isInput())
		str.m_hash = CHashedString::CreateHash(temp.c_str());
#endif
	return true;
}

#endif  //_HASHED_STRING_H_
