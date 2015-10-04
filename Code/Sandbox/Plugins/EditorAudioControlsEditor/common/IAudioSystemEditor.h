// Copyright 2001-2015 Crytek GmbH. All rights reserved.

#pragma once

#include "ACETypes.h"
#include <IXml.h>

#ifdef PLUGIN_EXPORTS
#define ACE_API DLL_EXPORT
#else
#define ACE_API DLL_IMPORT
#endif

namespace AudioControls
{
	class IAudioSystemControl;
	class IAudioConnection;

	typedef uint TImplControlTypeMask;

	struct SControlDef
	{
		TImplControlType eType;           // middleware type of the control
		string sName;                     // name of the control
		string sPath;                     // subfolder/path of the control
		bool bLocalised;                  // true if the control is localised.
		IAudioSystemControl* pParent;     // pointer to the parent
		SControlDef(const string& name, TImplControlType type, bool localised = false, IAudioSystemControl* parent = nullptr, const string& path = "")
			: eType(type)
			, sName(name)
			, bLocalised(localised)
			, pParent(parent)
			, sPath(path)
		{}
	};

	class IAudioSystemEditor
	{
	public:
		IAudioSystemEditor() {}
		virtual ~IAudioSystemEditor() {}

		// <title Reload>
		// Description:
		//      Reloads all the middleware control data
		// Arguments:
		//      bPreserveConnectionStatus - Keep the connection status of the controls. If true, when
		//			reloading the controls it will try to map the connected status they had previously
		//			(if they existed before the reload).
		virtual void Reload(bool bPreserveConnectionStatus = true) = 0;

		// <title CreateControl>
		// Description:
		//      Creates a new middleware control given the specifications passed in as a parameter.
		//      The control is owned by this class.
		// Arguments:
		//      controlDefinition - Values to use to initialize the new control.
		// Returns:
		//      A pointer to the newly created control.
		// See Also:
		//      GetControl, GetRoot
		virtual IAudioSystemControl* CreateControl(const SControlDef& controlDefinition) = 0;

		// <title GetRoot>
		// Description:
		//      Middleware controls are organized in a tree structure.
		//      This function returns the root of the tree to allow for traversing the tree manually.
		// Returns:
		//      A pointer to the root of the control tree.
		// See Also:
		//      GetControl
		virtual IAudioSystemControl* GetRoot() = 0;

		// <title GetControl>
		// Description:
		//      Gets the middleware control given its unique id.
		// Arguments:
		//      id - Unique ID of the control.
		// Returns:
		//      A pointer to the control that corresponds to the passed id. If none is found NULL is returned.
		// See Also:
		//      GetRoot
		virtual IAudioSystemControl* GetControl(CID id) const  = 0;

		// <title ImplTypeToATLType>
		// Description:
		//      The ACE lets the user create an ATL control out of a middleware control, for this it needs to convert a middleware control type to an ATL control type.
		// Arguments:
		//      type - Middleware control type
		// Returns:
		//      An ATL control type that corresponds to the middleware control type passed as argument.
		// See Also:
		//      GetCompatibleTypes
		virtual EACEControlType ImplTypeToATLType(TImplControlType type) const = 0;
		
		// <title GetCompatibleTypes>
		// Description:
		//      Given an ATL control type this function returns all the middleware control types that can be connected to it.
		// Arguments:
		//      eATLControlType - An ATL control type.
		// Returns:
		//      A mask representing all the middleware control types that can be connected to the ATL control type passed as argument.
		// See Also:
		//      ImplTypeToATLType
		virtual TImplControlTypeMask GetCompatibleTypes(EACEControlType eATLControlType) const = 0;

		// <title CreateConnectionToControl>
		// Description:
		//      Creates and returns a connection to a middleware control. The connection object is owned by this class.
		// Arguments:
		//      eATLControlType - The type of the ATL control you are connecting to pMiddlewareControl.
		//      pMiddlewareControl - Middleware control for which to make the connection.
		// Returns:
		//      A pointer to the newly created connection
		// See Also:
		//      CreateConnectionFromXMLNode
		virtual TConnectionPtr CreateConnectionToControl(EACEControlType eATLControlType, IAudioSystemControl* pMiddlewareControl) = 0;
		
		// <title CreateConnectionFromXMLNode>
		// Description:
		//      Creates and returns a connection defined in an XML node. 
		//      The format of the XML node should be in sync with the WriteConnectionToXMLNode function which is in charge of writing the node during serialization.
		//      If the XML node is unknown to the system NULL should be returned.
		//      If the middleware control referenced in the XML node does not exist it should be created and marked as "placeholder".
		// Arguments:
		//      pNode - XML node where the connection is defined.
		//			eATLControlType - The type of the ATL control you are connecting to
		// Returns:
		//      A pointer to the newly created connection
		// See Also:
		//      WriteConnectionToXMLNode
		virtual TConnectionPtr CreateConnectionFromXMLNode(XmlNodeRef pNode, EACEControlType eATLControlType) = 0;
		
		// <title CreateXMLNodeFromConnection>
		// Description:
		//      When serializing connections between controls this function will be called once per connection to serialize its properties.
		//      This function should be in sync with CreateConnectionToControl as whatever it's written here will have to be read there.
		// Arguments:
		//      pConnection - Connection to serialize.
		//      eATLControlType - Type of the ATL control that has this connection.
		// Returns:
		//      XML node with the connection serialized
		// See Also:
		//      CreateConnectionFromXMLNode
		virtual XmlNodeRef CreateXMLNodeFromConnection(const TConnectionPtr pConnection, const EACEControlType eATLControlType) = 0;
		
		// <title ConnectionRemoved>
		// Description:
		//      Whenever a connection is removed from an ATL control this function should be called to keep the system informed of which controls have been connected and which ones haven't.
		// Arguments:
		//      pControl - Middleware control that was disconnected.   
		// See Also:
		//  
		virtual void ConnectionRemoved(IAudioSystemControl* pControl) {}
		
		// <title GetTypeIcon>
		// Description:
		//      Returns the icon corresponding to the middleware control type passed as argument.
		// Arguments:
		//      type - Middleware control type
		// Returns:
		//      A string with the path to the icon corresponding to the control type
		// See Also:
		//  
		virtual string GetTypeIcon(TImplControlType type) const = 0;
		
		// <title GetName>
		// Description:
		//      Gets the name of the implementation which might be used in the ACE UI.
		// Returns:
		//      String with the name of the implementation.
		virtual string GetName() const = 0;

		// <title GetDataPath>
		// Description:
		//      Gets the folder where the implementation specific controls data are stored.
		//			This is used by the ACE to update if controls are changed while the editor is open.
		// Returns:
		//      String with the path to the folder where the implementation specific controls are stored.
		virtual string GetDataPath() const = 0;

		// <title DataSaved>
		// Description:
		//      Informs the plugin that the ACE has saved the data in case it needs to do any clean up
		virtual void DataSaved() = 0;
	};
}