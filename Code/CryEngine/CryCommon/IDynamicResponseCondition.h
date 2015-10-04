// Copyright 2001-2015 Crytek GmbH. All rights reserved.

/************************************************************************

An Interface for Conditions that can be added to Response Segments

/************************************************************************/

#ifndef _DYNAMICRESPONSECONDITION_H_
#define _DYNAMICRESPONSECONDITION_H_

#include <IXml.h>
#include <Serialization/Forward.h>

namespace DRS
{
	struct IResponseActor;
	struct IVariableCollection;		

	struct IResponseCondition
	{
		virtual ~IResponseCondition() {}

		typedef uint32 ConditionIdentifierHash;
		// <title IsMet>
		// Syntax: its called by the DRS system, to evaluate if this condition is fulfilled
		// Description:
		//		the condition is required to check all needed data as fast as possible and return if the response can therefore be executed
		// Arguments:
		//		pObject - a pointer to the current active Actor for this response, can be used to get local variable collections from or use as execution object
		//		pContextVariableCollection - a pointer to the context variable collection for the current event.
		// Returns:
		//		needs to return, if the response is allowed to execute
		virtual bool IsMet(const IResponseActor* pObject, const IVariableCollection* pContextVariableCollection) = 0;

		// <title Serialize>
		// Syntax: Called by the serialization library to serialize to disk or to the UI
		// Description:
		//		Serializes all the member variables of this class
		// Arguments:
		// Returns:
		// See Also:
		virtual void Serialize(Serialization::IArchive& ar) = 0;
		
		// <title GetVerboseInfo>
		// Syntax: Will be called from the DRS for debug output
		// Description:
		//      Just some debug information, that describe the condition and its checked values. So that a history-log with checked conditions can be generated.
		// Arguments:
		// Returns:
		//		a descriptive string containing the condition type and its values
		// See Also:
		virtual const string GetVerboseInfo() const = 0;
	};
}  //namespace DRS

#endif  //_DYNAMICRESPONSECONDITION_H_
