// CryEngine Header File.
// Copyright (C), Crytek, 1999-2014.

#pragma once

#include "platform.h"
#include "CryString.h"
#include "IAudioConnection.h"
#include "ACETypes.h"

namespace AudioControls
{
	class IAudioSystemControl
	{
	public:
		IAudioSystemControl::IAudioSystemControl()
			: m_name("")
			, m_id(ACE_INVALID_ID)
			, m_type(AUDIO_IMPL_INVALID_TYPE)
			, m_bPlaceholder(false)
			, m_bLocalised(false)
			, m_parent(nullptr)
			, m_bConnected(false)
		{
		}

		IAudioSystemControl::IAudioSystemControl(const string& name, CID id, TImplControlType type)
			: m_name(name)
			, m_id(id)
			, m_type(type)
			, m_bPlaceholder(false)
			, m_bLocalised(false)
			, m_parent(nullptr)
			, m_bConnected(false)
		{
		}

		virtual ~IAudioSystemControl() {}

		// unique id for this control
		CID GetId() const { return m_id; }
		void SetId(CID id) { m_id = id; }

		TImplControlType GetType() const { return m_type; }
		void SetType(TImplControlType type) { m_type = type; }

		string GetName() const { return m_name; }
		void SetName(const string& name)
		{
			if (name != m_name)
			{
				m_name = name;
			}
		}

		bool IsPlaceholder() const { return m_bPlaceholder; }
		void SetPlaceholder(bool bIsPlaceholder) { m_bPlaceholder = bIsPlaceholder; }

		bool IsLocalised() const { return m_bLocalised; }
		void SetLocalised(bool bIsLocalised) { m_bLocalised = bIsLocalised; }

		bool IsConnected() const { return m_bConnected; }
		void SetConnected(bool bConnected) { m_bConnected = bConnected; }

		size_t ChildCount() const { return m_children.size(); }
		void AddChild(IAudioSystemControl* pChild) { m_children.push_back(pChild); pChild->SetParent(this); }
		IAudioSystemControl* GetChildAt(uint index) const { return m_children[index]; }
		void SetParent(IAudioSystemControl* pParent) { m_parent = pParent; }
		IAudioSystemControl* GetParent() const { return m_parent; }

	private:
		CID m_id;
		TImplControlType m_type;
		string m_name;
		bool m_bPlaceholder;
		bool m_bLocalised;
		bool m_bConnected;
		std::vector<IAudioSystemControl*> m_children;
		IAudioSystemControl* m_parent;
	};
}