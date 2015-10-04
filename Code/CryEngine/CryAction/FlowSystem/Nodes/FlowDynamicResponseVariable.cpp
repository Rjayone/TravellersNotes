// Copyright 2001-2015 Crytek GmbH. All rights reserved.

#include "StdAfx.h"
#include "FlowBaseNode.h"
#include <ISystem.h>
#include <IEntityProxy.h>
#include <IDynamicResponseSystem.h>

namespace
{
	DRS::IVariableCollection* GetVariableCollection(EntityId entityID, const string& collectionName)
	{
		if (entityID != INVALID_ENTITYID)  //entityID specified = use local collection of that entity
		{
			IActor* pActor01 = reinterpret_cast<IActor*>(gEnv->pGame->GetIGameFramework()->GetIActorSystem()->GetActor(entityID));
			IEntity* pVariableCollectionOwningEntity = gEnv->pEntitySystem->GetEntity(entityID);
			if (pVariableCollectionOwningEntity)
			{
				const IEntityDynamicResponseProxyPtr pIEntityDrsProxy = crycomponent_cast<IEntityDynamicResponseProxyPtr>(pVariableCollectionOwningEntity->CreateProxy(ENTITY_PROXY_DYNAMICRESPONSE));
				if (pIEntityDrsProxy)
				{
					return pIEntityDrsProxy->GetLocalVariableCollection();
				}
			}
			return NULL;
		}
		else  //fetch an hopefully existing Collection
		{
			DRS::IVariableCollection* variableCollection = gEnv->pDynamicResponseSystem->GetCollection(collectionName);
			if (!variableCollection)
			{
				CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "CAction:DRS_SetVariable: Could not find collection with name '%s'", collectionName.c_str());
			}

			return variableCollection;
		}
		return 0;
	}
}

class CFlowNode_SetFloatDynamicResponseVariable : public CFlowBaseNode<eNCT_Instanced>
{
public:
	enum EInputs
	{
		eIn_Set,
		eIn_EntityID,
		eIn_VariableCollectionName,
		eIn_VariableName,
		eIn_FloatValue,
		eIn_ResetTime,
	};

	enum EOutputs 
	{
		eOut_VariableCollectionName,
	};

	explicit CFlowNode_SetFloatDynamicResponseVariable(SActivationInfo* pActInfo) 
	{
	}

	//////////////////////////////////////////////////////////////////////////
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo)
	{
		return new CFlowNode_SetFloatDynamicResponseVariable(pActInfo);
	}

	//////////////////////////////////////////////////////////////////////////

	virtual void GetConfiguration(SFlowNodeConfig& config)
	{
		static const SInputPortConfig inputs[] = 
		{
			InputPortConfig_Void("Set", _HELP("Sets the given value to the specified variable. Will create a new variable collection or use existing")),
			InputPortConfig<EntityId>("EntityID", _HELP("The ID of the entity to fetch the collection from.")),
			InputPortConfig<string>("CollectionName", _HELP("The name of a collection. Only used if no entity-id is specified, otherwise the entity-local variable collection will be used"), "CollectionName"),
			InputPortConfig<string>("VariableName", _HELP("The name of the variable to set, will be created if not existing"), "VariableName"),
			InputPortConfig<float>("FloatValue", _HELP("A float value to set to the variable"), "FloatValue"),
			InputPortConfig<float>("ResetTime", _HELP("The time after which the variable is reset to its previous value"), "ResetTime"),
			{0}
		};

		static const SOutputPortConfig outputs[] = {
			OutputPortConfig<string> ("UsedCollectionName", _HELP("Outputs the name of the variable-collection created or used")),
			{0}
		};

		config.pInputPorts = inputs;
		config.pOutputPorts = outputs;
		config.sDescription = _HELP("This node is used to set a variable in a variable collection for the Dynamic Response System.");
		//config.nFlags |= EFLN_TARGET_ENTITY;
		config.SetCategory(EFLN_APPROVED);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual void Serialize(SActivationInfo* pActInfo, TSerialize ser)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
	{
		switch (event)
		{
		case eFE_Initialize:
			{
				break;
			}
		case eFE_Activate:
			{
				if (IsPortActive(pActInfo, eIn_Set))
				{
					const string& variableName = GetPortString(pActInfo, eIn_VariableName);

					if (variableName.empty())
					{
						CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "CAction:DRS_SetVariable: Cannot be executed without a Variable Name.");
						return;
					}

					const string& collectionName = GetPortString(pActInfo, eIn_VariableCollectionName);
					EntityId entityID = GetPortEntityId(pActInfo, eIn_EntityID);

					DRS::IVariableCollection* variableCollection = GetVariableCollection(entityID, collectionName);

					if (variableCollection)
					{
						float newVariableValue = GetPortFloat(pActInfo, eIn_FloatValue);
						variableCollection->SetVariableValue(variableName, newVariableValue, true, GetPortFloat(pActInfo, eIn_ResetTime));
						ActivateOutput(pActInfo, eOut_VariableCollectionName, variableCollection->GetName().GetText());
					}
				}
				break;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	virtual void GetMemoryUsage(ICrySizer * s) const
	{
		s->Add(*this);
	}
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class CFlowNode_SetIntegerDynamicResponseVariable : public CFlowBaseNode<eNCT_Instanced>
{
public:
	enum EInputs
	{
		eIn_Set,
		eIn_EntityID,
		eIn_VariableCollectionName,
		eIn_VariableName,
		eIn_IntegerValue,
		eIn_ResetTime,
	};

	enum EOutputs 
	{
		eOut_VariableCollectionName,
	};

	explicit CFlowNode_SetIntegerDynamicResponseVariable(SActivationInfo* pActInfo)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo)
	{
		return new CFlowNode_SetIntegerDynamicResponseVariable(pActInfo);
	}

	//////////////////////////////////////////////////////////////////////////

	virtual void GetConfiguration(SFlowNodeConfig& config)
	{
		static const SInputPortConfig inputs[] = 
		{
			InputPortConfig_Void("Set", _HELP("Sets the given value to the specified variable. Will create a new variable collection or use existing")),
			InputPortConfig<EntityId>("EntityID", _HELP("The ID of the entity to fetch the collection from.")),
			InputPortConfig<string>("CollectionName", _HELP("The name of a collection. Only used if no entity-id is specified, otherwise the entity-local variable collection will be used"), "CollectionName"),
			InputPortConfig<string>("VariableName", _HELP("The name of the variable to set, will be created if not existing"), "VariableName"),
			InputPortConfig<int>("IntegerValue", _HELP("A integer value to set to the variable"), "IntegerValue"),
			InputPortConfig<float>("ResetTime", _HELP("The time after which the variable is reset to its previous value"), "ResetTime"),
			{0}
		};

		static const SOutputPortConfig outputs[] = {
			OutputPortConfig<string> ("UsedCollectionName", _HELP("Outputs the name of the variable-collection created or used")),
			{0}
		};

		config.pInputPorts = inputs;
		config.pOutputPorts = outputs;
		config.sDescription = _HELP("This node is used to set a variable in a variable collection for the Dynamic Response System.");
		//config.nFlags |= EFLN_TARGET_ENTITY;
		config.SetCategory(EFLN_APPROVED);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual void Serialize(SActivationInfo* pActInfo, TSerialize ser)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
	{
		switch (event)
		{
		case eFE_Initialize:
			{
				break;
			}
		case eFE_Activate:
			{
				const string& variableName = GetPortString(pActInfo, eIn_VariableName);

				if (variableName.empty())
				{
					CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "CAction:DRS_SetVariable: Cannot be executed without a Variable Name.");
					return;
				}

				const string& collectionName = GetPortString(pActInfo, eIn_VariableCollectionName);
				EntityId entityID = GetPortEntityId(pActInfo, eIn_EntityID);

				DRS::IVariableCollection* variableCollection = GetVariableCollection(entityID, collectionName);

				if (variableCollection)
				{
					int newVariableValue = GetPortInt(pActInfo, eIn_IntegerValue);
					variableCollection->SetVariableValue(variableName, newVariableValue, true, GetPortFloat(pActInfo, eIn_ResetTime));
					ActivateOutput(pActInfo, eOut_VariableCollectionName, variableCollection->GetName().GetText());
				}

				break;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	virtual void GetMemoryUsage(ICrySizer * s) const
	{
		s->Add(*this);
	}
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class CFlowNode_SetStringDynamicResponseVariable : public CFlowBaseNode<eNCT_Instanced>
{
public:
	enum EInputs
	{
		eIn_Set,
		eIn_EntityID,
		eIn_VariableCollectionName,
		eIn_VariableName,
		eIn_StringValue,
		eIn_ResetTime,
	};

	enum EOutputs 
	{
		eOut_VariableCollectionName,
	};

	explicit CFlowNode_SetStringDynamicResponseVariable(SActivationInfo* pActInfo)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo)
	{
		return new CFlowNode_SetStringDynamicResponseVariable(pActInfo);
	}

	//////////////////////////////////////////////////////////////////////////

	virtual void GetConfiguration(SFlowNodeConfig& config)
	{
		static const SInputPortConfig inputs[] = 
		{
			InputPortConfig_Void("Set", _HELP("Sets the given value to the specified variable. Will create a new variable collection or use existing")),
			InputPortConfig<EntityId>("EntityID", _HELP("The ID of the entity to fetch the collection from.")),
			InputPortConfig<string>("CollectionName", _HELP("The name of a collection. Only used if no entity-id is specified, otherwise the entity-local variable collection will be used"), "CollectionName"),
			InputPortConfig<string>("VariableName", _HELP("The name of the variable to set, will be created if not existing"), "VariableName"),
			InputPortConfig<string>("StringValue", _HELP("A string value to set to the variable"), "StringValue"),
			InputPortConfig<float>("ResetTime", _HELP("The time after which the variable is reset to its previous value"), "ResetTime"),
			{0}
		};

		static const SOutputPortConfig outputs[] = {
			OutputPortConfig<string> ("UsedCollectionName", _HELP("Outputs the name of the variable-collection created or used")),
			{0}
		};

		config.pInputPorts = inputs;
		config.pOutputPorts = outputs;
		config.sDescription = _HELP("This node is used to set a variable in a variable collection for the Dynamic Response System.");
		//config.nFlags |= EFLN_TARGET_ENTITY;
		config.SetCategory(EFLN_APPROVED);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual void Serialize(SActivationInfo* pActInfo, TSerialize ser)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
	{
		switch (event)
		{
		case eFE_Initialize:
			{
				break;
			}
		case eFE_Activate:
			{
				const string& variableName = GetPortString(pActInfo, eIn_VariableName);

				if (variableName.empty())
				{
					CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "CAction:DRS_SetVariable: Cannot be executed without a Variable Name.");
					break;
				}

				const string& collectionName = GetPortString(pActInfo, eIn_VariableCollectionName);
				EntityId entityID = GetPortEntityId(pActInfo, eIn_EntityID);

				DRS::IVariableCollection* variableCollection = GetVariableCollection(entityID, collectionName);

				if (variableCollection)
				{					
					const string& newVariableValue = GetPortString(pActInfo, eIn_StringValue);
					variableCollection->SetVariableValue(variableName, newVariableValue.c_str(), true, GetPortFloat(pActInfo, eIn_ResetTime));
					ActivateOutput(pActInfo, eOut_VariableCollectionName, variableCollection->GetName().GetText());
				}

				break;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	virtual void GetMemoryUsage(ICrySizer * s) const
	{
		s->Add(*this);
	}
};


#if !defined(IS_EAAS)
REGISTER_FLOW_NODE("DynamicResponse:SetFloatVariable", CFlowNode_SetFloatDynamicResponseVariable);
REGISTER_FLOW_NODE("DynamicResponse:SetIntegerVariable", CFlowNode_SetIntegerDynamicResponseVariable);
REGISTER_FLOW_NODE("DynamicResponse:SetStringVariable", CFlowNode_SetStringDynamicResponseVariable);
#endif
