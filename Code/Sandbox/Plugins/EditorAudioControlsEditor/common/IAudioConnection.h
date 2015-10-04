// CryEngine Header File.
// Copyright (C), Crytek, 1999-2014.

#pragma once

#include "CryString.h"
#include "ACETypes.h"
#include <Serialization/IArchive.h>
#include <Serialization/STL.h>

namespace AudioControls
{
	class IAudioSystemControl;
	class IAudioConnection
	{
	public:
		IAudioConnection(CID nID) 
			: m_nID(nID)
			, m_sGroup("default")
		{}

		virtual ~IAudioConnection() {}

		CID GetID() const {return m_nID;}

		const string& GetGroup() const {return m_sGroup;}
		void SetGroup(const string& group) {m_sGroup =  group;}

		virtual bool HasProperties() { return false; }

		virtual void Serialize(Serialization::IArchive& ar) {};

	private:
		CID m_nID;
		string m_sGroup;
	};
}