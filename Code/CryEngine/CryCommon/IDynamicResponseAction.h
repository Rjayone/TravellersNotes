// Copyright 2001-2015 Crytek GmbH. All rights reserved.

/************************************************************************

An Interface for actions that can be executed by Response Segments

/************************************************************************/

#ifndef _DYNAMICRESPONSEACTION_H_
#define _DYNAMICRESPONSEACTION_H_

#include <IXml.h>
#include <Serialization/Forward.h>

namespace DRS
{
	struct IResponseInstance;
	struct IResponseActionInstance;

	typedef std::unique_ptr<IResponseActionInstance> IResponseActionInstanceUniquePtr;

	struct IResponseAction
	{
		virtual ~IResponseAction() {}

		// <title Execute>
		// Syntax: its called by the DRS system, when a response segment is executed which contains a action of this type
		// Description:
		//      Will execute the action. If the action is not an instantaneous action, it can return an ActionInstance, which is then updated as long as needed by the DRS to finish the execution of the action
		// Arguments:
		//		pResponseInstance - the ResponseInstance that requested the execution of the action. From the ResponseInstance the active actor and the context variables of signal can be obtained, if needed for the execution
		// Returns:
		//		if needed an actionInstance is returned. If the action was already completed during the function 0 will be returned. (For instant-effect-actions like setVariable)
		// See Also:
		//		IResponseInstance
		virtual IResponseActionInstanceUniquePtr Execute(IResponseInstance* pResponseInstance) = 0;

		// <title GetVerboseInfo>
		// Syntax: Will be called from the DRS for debug output
		// Description:
		//      Just some debug information, that describe the action and its parameter. So that a history-log with executed actions can be generated.
		// Arguments:
		// Returns:
		//		a descriptive string containing the action name and its current parameter
		// See Also:
		virtual const string GetVerboseInfo() const = 0;

		// <title Serialize>
		// Syntax: Called by the serialization library to serialize to disk or to the UI
		// Description:
		//      Serializes all the member variables of this class
		// Arguments:
		// Returns:
		// See Also:
		virtual void Serialize(Serialization::IArchive& ar) = 0;
	};

	//one concrete running instance of the actions. (There might be actions that dont need instances, because their action is instantaneously.
	struct IResponseActionInstance
	{
		enum eCurrentState
		{
			CS_RUNNING,
			CS_FINISHED,
			CS_CANCELED,
			CS_ERROR
		};

		virtual ~IResponseActionInstance() {}

		// <title Update>
		// Syntax: its called by the DRS system, as long as the action instance is running
		// Description:
		//      This method continues the execution of an started action that takes some time to finish. Its called from the DRS System until it stops returning CS_RUNNING.		
		// Arguments:
		// Returns:
		//		returns the current state of the execution of the action. CS_RUNNING means still needs time to finish, CS_FINISHED means finished successful, 
		//		CS_CANCELED means the action was canceled from the outside before it was finished, CS_ERROR means an error happened during execution
		// See Also:
		virtual eCurrentState Update() = 0;

		// <title Cancel>
		// Syntax: its called by the DRS system, when someone requested a cancellation of a running response (segment)
		// Description:
		//      Will be called when someone requested a cancellation of a running response (segment). Its up to the action instance how to handle this request.
		//		There might be cases for example, where the action decides to first finish the current sentence before actually stop. So the DRS will continue updating the ActionInstance, 
		//		until the actionInstance decides to stop (by returning CS_CANCELED or CS_FINISHED in the Update method).
		// Arguments:
		// Returns:
		// See Also:
		//		Update()
		virtual void Cancel() = 0;

		// <title GetVerboseInfo>
		// Syntax: For debug only
		// Description:
		//      Just some debug information, that describe the actionInstance and its parameter and its current process. So that a list of running action instances can be generated
		// Arguments:
		// Returns:
		//		a descriptive string containing information about the current state of the actionInstance
		// See Also:
		virtual const string GetVerboseInfo() const = 0;  //For debug purposes only
	};
} 

#endif  //_DYNAMICRESPONSEACTION_H_
