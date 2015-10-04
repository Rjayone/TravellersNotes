/////////////////////////////////////////////////////////////////////////////
//
// Crytek Source File
// Copyright (C), Crytek Studios, 2001-2006.
//
// History:
// Jun 20, 2006: Created by Sascha Demetrio
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CryThread_pthreads.h"

#if PLATFORM_SUPPORTS_THREADLOCAL
THREADLOCAL CrySimpleThreadSelf
	*CrySimpleThreadSelf::m_Self = NULL;
#else
TLS_DEFINE(CrySimpleThreadSelf*, g_CrySimpleThreadSelf)
#endif


//////////////////////////////////////////////////////////////////////////
// CryEvent(Timed) implementation
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CryEventTimed::Reset()
{
	m_lockNotify.Lock();
	m_flag = false;
	m_lockNotify.Unlock();
}

//////////////////////////////////////////////////////////////////////////
void CryEventTimed::Set()
{
	m_lockNotify.Lock();
	m_flag = true;
	m_cond.Notify();
	m_lockNotify.Unlock();
}

//////////////////////////////////////////////////////////////////////////
void CryEventTimed::Wait()
{
	m_lockNotify.Lock();
	if (!m_flag)
		m_cond.Wait(m_lockNotify);
	m_flag	=	false;
	m_lockNotify.Unlock();
}

//////////////////////////////////////////////////////////////////////////
bool CryEventTimed::Wait( const uint32 timeoutMillis )
{
	bool bResult = true;
	m_lockNotify.Lock();
	if (!m_flag)
		bResult = m_cond.TimedWait(m_lockNotify,timeoutMillis);
	m_flag	=	false;
	m_lockNotify.Unlock();
	return bResult;
}

///////////////////////////////////////////////////////////////////////////////
// CryCriticalSection implementation
///////////////////////////////////////////////////////////////////////////////
typedef CryLockT<CRYLOCK_RECURSIVE> TCritSecType;

void  CryDeleteCriticalSection( void *cs )
{
	delete ((TCritSecType *)cs);
}

void  CryEnterCriticalSection( void *cs )
{
	((TCritSecType*)cs)->Lock();
}

bool  CryTryCriticalSection( void *cs )
{
	return false;
}

void  CryLeaveCriticalSection( void *cs )
{
	((TCritSecType*)cs)->Unlock();
}

void  CryCreateCriticalSectionInplace(void* pCS)
{
	new (pCS) TCritSecType;
}

void CryDeleteCriticalSectionInplace( void *)
{
}

void* CryCreateCriticalSection()
{
	return (void*) new TCritSecType;
}

//////////////////////////////////////////////////////////////////////////
// Linux 64-bit implementation of lockless single-linked list
//////////////////////////////////////////////////////////////////////////
#if (CRY_PLATFORM_LINUX && CRY_PLATFORM_64BIT) || CRY_PLATFORM_MAC || defined(IOS_SIMULATOR) && !CRY_PLATFORM_ORBIS
	typedef __uint128_t uint128;

	//////////////////////////////////////////////////////////////////////////
	// Implementation for Linux64 with gcc using __int128_t
	//////////////////////////////////////////////////////////////////////////
	void CryInterlockedPushEntrySList( SLockFreeSingleLinkedListHeader& list,  SLockFreeSingleLinkedListEntry &element )
	{
		uint64 curSetting[2];
		uint64 newSetting[2];
		uint64 newPointer = (uint64)&element;
		do 
		{
			curSetting[0] = (uint64)list.pNext;
			curSetting[1] = list.salt;
			element.pNext = (SLockFreeSingleLinkedListEntry*)curSetting[0];
			newSetting[0] = newPointer; // new pointer
			newSetting[1] = curSetting[1]+1; // new salt
		}
		// while (false == __sync_bool_compare_and_swap( (volatile uint128*)&list.pNext,*(uint128*)&curSetting[0],*(uint128*)&newSetting[0] ));
	   while (0 == _InterlockedCompareExchange128((volatile int64*)&list.pNext, (int64)newSetting[1], (int64)newSetting[0], (int64*)&curSetting[0])); 
	}
	
	//////////////////////////////////////////////////////////////////////////
	void CryInterlockedPushListSList( SLockFreeSingleLinkedListHeader& list, SLockFreeSingleLinkedListEntry& first, SLockFreeSingleLinkedListEntry& last, uint32 count)
	{
		(void)count; // unused

		uint64 curSetting[2];
		uint64 newSetting[2];
		uint64 newPointer = (uint64)&first;
		do
		{
			curSetting[0] = (uint64)list.pNext;
			curSetting[1] = list.salt;
			last.pNext = (SLockFreeSingleLinkedListEntry*)curSetting[0];
			newSetting[0] = newPointer; // new pointer
			newSetting[1] = curSetting[1]+1; // new salt
		}
		while (0 == _InterlockedCompareExchange128((volatile int64*)&list.pNext, (int64)newSetting[1], (int64)newSetting[0], (int64*)&curSetting[0]));
	}

	//////////////////////////////////////////////////////////////////////////
	void* CryInterlockedPopEntrySList(  SLockFreeSingleLinkedListHeader& list )
	{
		uint64 curSetting[2];
		uint64 newSetting[2];
		do 
		{
			curSetting[1]=list.salt;
			curSetting[0]=(uint64)list.pNext;
			if (curSetting[0]==0)
				return NULL;
			newSetting[0] = *(uint64*)curSetting[0]; // new pointer
			newSetting[1] = curSetting[1]+1; // new salt
		}
		//while (false == __sync_bool_compare_and_swap( (volatile uint128*)&list.pNext,*(uint128*)&curSetting[0],*(uint128*)&newSetting[0] ));
	   while (0 == _InterlockedCompareExchange128((volatile int64*)&list.pNext, (int64)newSetting[1], (int64)newSetting[0], (int64*)&curSetting[0])); 
		return (void*)curSetting[0];
	}

	//////////////////////////////////////////////////////////////////////////
	void* CryRtlFirstEntrySList( SLockFreeSingleLinkedListHeader& list )
	{
		return list.pNext;
	}

	//////////////////////////////////////////////////////////////////////////
	void CryInitializeSListHead(SLockFreeSingleLinkedListHeader& list)
	{
		list.salt = 0;
		list.pNext = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	void* CryInterlockedFlushSList(SLockFreeSingleLinkedListHeader& list)
	{
		uint64 curSetting[2];
		uint64 newSetting[2];
		uint64 newSalt;
		uint64 newPointer;
		do 
		{
			curSetting[1]=list.salt;
			curSetting[0]=(uint64)list.pNext;
			if (curSetting[0]==0)
				return NULL;
			newSetting[0] = 0;
			newSetting[1] = curSetting[1]+1;
		}
	//	while (false == __sync_bool_compare_and_swap( (volatile uint128*)&list.pNext,*(uint128*)&curSetting[0],*(uint128*)&newSetting[0] ));
	   while (0 == _InterlockedCompareExchange128((volatile int64*)&list.pNext, (int64)newSetting[1], (int64)newSetting[0], (int64*)&curSetting[0])); 
		return (void*)curSetting[0];
	}
	//////////////////////////////////////////////////////////////////////////
#elif (CRY_PLATFORM_LINUX && CRY_PLATFORM_32BIT) || CRY_PLATFORM_ANDROID
	//////////////////////////////////////////////////////////////////////////
	// Implementation for Linux32 with gcc using uint64
	//////////////////////////////////////////////////////////////////////////
	void CryInterlockedPushEntrySList( SLockFreeSingleLinkedListHeader& list,  SLockFreeSingleLinkedListEntry &element )
	{
		uint32 curSetting[2];
		uint32 newSetting[2];
		uint32 newPointer = (uint32)&element;
		do 
		{
			curSetting[0] = (uint32)list.pNext;
			curSetting[1] = list.salt;
			element.pNext = (SLockFreeSingleLinkedListEntry*)curSetting[0];
			newSetting[0] = newPointer; // new pointer
			newSetting[1] = curSetting[1]+1; // new salt
		}
		while (false == __sync_bool_compare_and_swap( (volatile uint64*)&list.pNext,*(uint64*)&curSetting[0],*(uint64*)&newSetting[0] ));
	}

	//////////////////////////////////////////////////////////////////////////
	void CryInterlockedPushListSList( SLockFreeSingleLinkedListHeader& list, SLockFreeSingleLinkedListEntry& first, SLockFreeSingleLinkedListEntry& last, uint32 count )
	{
		(void)count; //unused

		uint32 curSetting[2];
		uint32 newSetting[2];
		uint32 newPointer = (uint32)&first;
		do 
		{
			curSetting[0] = (uint32)list.pNext;
			curSetting[1] = list.salt;
			last.pNext = (SLockFreeSingleLinkedListEntry*)curSetting[0];
			newSetting[0] = newPointer; // new pointer
			newSetting[1] = curSetting[1]+1; // new salt
		}
		while (false == __sync_bool_compare_and_swap( (volatile uint64*)&list.pNext,*(uint64*)&curSetting[0],*(uint64*)&newSetting[0] ));
	}

	//////////////////////////////////////////////////////////////////////////
	void* CryInterlockedPopEntrySList(  SLockFreeSingleLinkedListHeader& list )
	{
		uint32 curSetting[2];
		uint32 newSetting[2];
		do 
		{
			curSetting[1]=list.salt;
			curSetting[0]=(uint32)list.pNext;
			if (curSetting[0]==0)
				return NULL;
			newSetting[0] = *(uint32*)curSetting[0]; // new pointer
			newSetting[1] = curSetting[1]+1; // new salt
		}
		while (false == __sync_bool_compare_and_swap( (volatile uint64*)&list.pNext,*(uint64*)&curSetting[0],*(uint64*)&newSetting[0] ));
		return (void*)curSetting[0];
	}

	//////////////////////////////////////////////////////////////////////////
	void* CryRtlFirstEntrySList( SLockFreeSingleLinkedListHeader& list )
	{
		return list.pNext;
	}

	//////////////////////////////////////////////////////////////////////////
	void CryInitializeSListHead(SLockFreeSingleLinkedListHeader& list)
	{
		list.salt = 0;
		list.pNext = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	void* CryInterlockedFlushSList(SLockFreeSingleLinkedListHeader& list)
	{
		uint32 curSetting[2];
		uint32 newSetting[2];
		uint32 newSalt;
		uint32 newPointer;
		do 
		{
			curSetting[1]=list.salt;
			curSetting[0]=(uint32)list.pNext;
			if (curSetting[0]==0)
				return NULL;
			newSetting[0] = 0;
			newSetting[1] = curSetting[1]+1;
		}
		while (false == __sync_bool_compare_and_swap( (volatile uint64*)&list.pNext,*(uint64*)&curSetting[0],*(uint64*)&newSetting[0] ));
		return (void*)curSetting[0];
	}
#elif CRY_PLATFORM_ORBIS
#include <kernel.h>
#include <sce_atomic.h>

	//////////////////////////////////////////////////////////////////////////
	// Implementation for ORBIS using pointer-embedded salt
	// This works because we know the maximum virtual address that can be used
	//////////////////////////////////////////////////////////////////////////
	struct SSalt
	{
		// Find log2 of some address, rounded down
		// Note: Pass in X + X - 1 to round up instead
		template<uintptr_t X, uintptr_t R = 0>
		struct log2 : log2<X / 2, R + 1> {};
		template<uintptr_t R>
		struct log2<1, R> { static const uintptr_t value = R; };

		// Analyze memory range to find maximum possible salt values
		static const uintptr_t kMemStart = SCE_KERNEL_APP_MAP_AREA_START_ADDR;
		static const uintptr_t kMemEnd = SCE_KERNEL_APP_MAP_AREA_END_ADDR;
		static_assert(kMemStart < kMemEnd, "invalid memory range");
		static const uintptr_t kSaltOne = uintptr_t(1) << log2<kMemEnd + kMemEnd - 1>::value;
		static const uintptr_t kPointerMask = kSaltOne - 1;
		static const uintptr_t kSaltMask = ~kPointerMask;
	};

	//////////////////////////////////////////////////////////////////////////
	void CryInterlockedPushEntrySList( SLockFreeSingleLinkedListHeader& list, SLockFreeSingleLinkedListEntry &element )
	{
		uintptr_t head;
		uintptr_t newHead;
		do
		{
			head = (volatile uintptr_t)list.pNext;
			newHead = (head & SSalt::kSaltMask) + SSalt::kSaltOne + (uintptr_t)&element;
			element.pNext = (SLockFreeSingleLinkedListEntry*)(head & SSalt::kPointerMask);
		} while (sceAtomicCompareAndSwap64((volatile int64_t*)&list.pNext, (int64_t)head, (int64_t)newHead) != head);
	}

	//////////////////////////////////////////////////////////////////////////
	void CryInterlockedPushListSList( SLockFreeSingleLinkedListHeader& list, SLockFreeSingleLinkedListEntry& first, SLockFreeSingleLinkedListEntry& last, uint32 count )
	{
		(void)count; // not used

		uintptr_t head;
		uintptr_t newHead;
		do
		{
			head = (volatile uintptr_t)list.pNext;
			newHead = (head & SSalt::kSaltMask) + SSalt::kSaltOne + (uintptr_t)&first;
			last.pNext = (SLockFreeSingleLinkedListEntry*)(head & SSalt::kPointerMask);
		} while (sceAtomicCompareAndSwap64((volatile int64_t*)&list.pNext, (int64_t)head, (int64_t)newHead) != head);
	}

	//////////////////////////////////////////////////////////////////////////
	void* CryInterlockedPopEntrySList( SLockFreeSingleLinkedListHeader& list )
	{
		uintptr_t head;
		uintptr_t newHead;
		SLockFreeSingleLinkedListEntry* pResult;
		do 
		{
			head = (volatile uintptr_t)list.pNext;
			pResult = (SLockFreeSingleLinkedListEntry*)(head & SSalt::kPointerMask);
			if (!pResult) break;
			newHead = (head & SSalt::kSaltMask) + SSalt::kSaltOne + (uintptr_t)pResult->pNext;
		} while (sceAtomicCompareAndSwap64((volatile int64_t*)&list, (int64_t)head, (int64_t)newHead) != head);
		return pResult;
	}

	//////////////////////////////////////////////////////////////////////////
	void* CryRtlFirstEntrySList( SLockFreeSingleLinkedListHeader& list )
	{
		uintptr_t head = sceAtomicLoad64((volatile int64_t*)&list.pNext);
		return (SLockFreeSingleLinkedListEntry*)(head & SSalt::kPointerMask);
	}

	//////////////////////////////////////////////////////////////////////////
	void CryInitializeSListHead(SLockFreeSingleLinkedListHeader& list)
	{
		list.pNext = nullptr;
	}

	//////////////////////////////////////////////////////////////////////////
	void* CryInterlockedFlushSList(SLockFreeSingleLinkedListHeader& list)
	{
		uintptr_t head;
		uintptr_t newHead;
		SLockFreeSingleLinkedListEntry* pResult;
		do 
		{
			head = (volatile uintptr_t)list.pNext;
			pResult = (SLockFreeSingleLinkedListEntry*)(head & SSalt::kPointerMask);
			if (!pResult) break;
			newHead = (head & SSalt::kSaltMask) + SSalt::kSaltOne;
		} while (sceAtomicCompareAndSwap64((volatile int64_t*)&list, (int64_t)head, (int64_t)newHead) != head);
		return pResult;
	}
#endif
