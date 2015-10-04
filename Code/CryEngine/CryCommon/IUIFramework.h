/*************************************************************************
  Crytek Source File.
  Copyright (C), Crytek Studios, 2001-2004.
 -------------------------------------------------------------------------
  $Id$
  $DateTime$
  Description: 
  
 -------------------------------------------------------------------------
  History:
  - 10:12:2014 : Created by Jan Neugebauer

*************************************************************************/
#ifndef __IUIFRAMEWORK_H__
#define __IUIFRAMEWORK_H__

#if _MSC_VER > 1000
# pragma once
#endif

#include <IGameFramework.h>

struct IFlashPlayer;
struct IFlashVariableObject;
struct SUILayoutEvent;



typedef CCryName CUIName;
typedef std::vector< CUIName > UINameVec;
typedef std::vector< SUILayoutEvent > UIEventVec;



struct SUILayoutEvent
{
	SUILayoutEvent()
		: commandName("")
		, signalName("")
		, guid("")
		, object("")
	{};

	~SUILayoutEvent(){};

	CUIName commandName;
	CUIName signalName;
	CUIName guid;
	CUIName object;
};



struct IUILayoutListener
{
	virtual void OnUIObjectEvent( const SUILayoutEvent& event ) = 0;
};



struct IUIObject
{
	virtual ~IUIObject() {}
	virtual void Unload() = 0;
	virtual IUIObject* GetChild( const char* objectName ) = 0;
	virtual void SetParam( const char* paramName, const char* paramValue ) = 0;
};



struct IUILayout
{
	virtual bool Load() = 0;
	virtual void Unload() = 0;
	virtual bool IsLoaded() const = 0;
	virtual void SetVisible( const bool visible ) = 0;
	virtual bool IsVisible() const = 0;
	virtual IUIObject* GetObject( const char* objectName ) = 0;
	virtual	IFlashPlayer* GetPlayer() = 0;
	virtual void GetUIEvents( UIEventVec& uiEvents ) const = 0;
	virtual void RegisterListener( IUILayoutListener* pListener, const char* eventName ) = 0;
	virtual void RegisterListenerWithAllEvents( IUILayoutListener* pListener ) = 0;
	virtual void UnregisterListener( IUILayoutListener* pListener, const char* eventId) = 0;
	virtual void UnregisterListenerAll( IUILayoutListener* pListener) = 0;
};



struct IInworldUI
{
	virtual ~IInworldUI(){}	
	virtual void CreatePanel(const EntityId entityId, const char* material) = 0;
	virtual void SetPanelMaterial(const EntityId entityId, const char* material) = 0;
	virtual void SetPanelPositionOffset(const EntityId entityId, const Vec3 position) = 0;
	virtual void SetPanelRotationOffset(const EntityId entityId, const Quat rotation) = 0;
};



namespace UIFramework
{

	struct IUIFramework : public ICryUnknown
	{

		CRYINTERFACE_DECLARE(IUIFramework, 0x89F04B15741A40DE, 0x94AD79A8AC3B7419)

		virtual IUILayout* GetLayout( const char* layoutName ) = 0;
		virtual void GetAllLayoutNames(UINameVec& layoutNames) const = 0;
		virtual bool LoadLayout( const char* layoutName ) = 0;
		virtual void UnloadLayout( const char* layoutName ) = 0;
		virtual IInworldUI* GetInworldUI() = 0;
		virtual void Init() = 0;
		virtual void Clear() = 0;
	};

	IUIFramework* CreateFramework(IGameFramework* pGameFramework);
}


#endif //__IUIFRAMEWORK_H__