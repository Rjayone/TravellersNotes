// Copyright 2001-2015 Crytek GmbH. All rights reserved.

#pragma once

#include "IAudioSystemEditor.h"
#include "IAudioConnection.h"
#include "IAudioSystemControl.h"
#include "AudioWwiseLoader.h"

namespace AudioControls
{
	class IAudioConnectionInspectorPanel;

	class CRtpcConnection : public IAudioConnection
	{
	public:
		explicit CRtpcConnection(CID nID)
			: IAudioConnection(nID)
			, fMult(1.0f)
			, fShift(0.0f)
		{ }

		virtual ~CRtpcConnection() {}

		virtual bool HasProperties() override { return true; }

		virtual void Serialize(Serialization::IArchive& ar) override
		{
			ar(fMult, "mult", "Multiply");
			ar(fShift, "shift", "Shift");
		}

		float fMult;
		float fShift;
	};
	typedef std::shared_ptr<CRtpcConnection> TRtpcConnectionPtr;


	class CStateToRtpcConnection : public IAudioConnection
	{
	public:
		explicit CStateToRtpcConnection(CID nID)
			: IAudioConnection(nID)
			, fValue(0.0f)
		{ }

		virtual ~CStateToRtpcConnection() {}

		virtual bool HasProperties() override { return true; }


		virtual void Serialize(Serialization::IArchive& ar) override
		{
			ar(fValue, "value", "Value");
		}

		float fValue;
	};
	typedef std::shared_ptr<CStateToRtpcConnection> TStateConnectionPtr;


	class CAudioSystemEditor_wwise : public IAudioSystemEditor
	{
		friend class CAudioWwiseLoader;

	public:
		CAudioSystemEditor_wwise();
		~CAudioSystemEditor_wwise();

		//////////////////////////////////////////////////////////
		// IAudioSystemEditor implementation
		/////////////////////////////////////////////////////////
		virtual void Reload(bool bPreserveConnectionStatus = true) override;
		virtual IAudioSystemControl* CreateControl(const SControlDef& controlDefinition) override;
		virtual IAudioSystemControl* GetRoot() override { return &m_rootControl; }
		virtual IAudioSystemControl* GetControl(CID id) const override;
		virtual EACEControlType ImplTypeToATLType(TImplControlType type) const override;
		virtual TImplControlTypeMask GetCompatibleTypes(EACEControlType eATLControlType) const override;
		virtual TConnectionPtr CreateConnectionToControl(EACEControlType eATLControlType, IAudioSystemControl* pMiddlewareControl) override;
		virtual TConnectionPtr CreateConnectionFromXMLNode(XmlNodeRef pNode, EACEControlType eATLControlType) override;
		virtual XmlNodeRef CreateXMLNodeFromConnection(const TConnectionPtr pConnection, const EACEControlType eATLControlType) override;
		virtual string GetTypeIcon(TImplControlType type) const override;
		virtual string GetName() const override;
		virtual string GetDataPath() const;
		virtual void DataSaved() override {}
		virtual void ConnectionRemoved(IAudioSystemControl* pControl);
		//////////////////////////////////////////////////////////

	private:
		IAudioSystemControl* GetControlByName(const string& sName, bool bIsLocalised = false, IAudioSystemControl* pParent = nullptr) const;

		// Gets the ID of the control given its name. As controls can have the same name
		// if they're under different parents, the name of the parent is also needed (if there is one)
		CID GetID(const string& sName) const;
		void UpdateConnectedStatus();

		IAudioSystemControl m_rootControl;

		typedef std::shared_ptr<IAudioSystemControl> TControlPtr;
		typedef std::map<CID, TControlPtr> TControlMap;
		TControlMap m_controls;

		typedef std::map<CID, int> TConnectionsMap;
		TConnectionsMap m_connectionsByID;
		AudioControls::CAudioWwiseLoader m_loader;
	};
}