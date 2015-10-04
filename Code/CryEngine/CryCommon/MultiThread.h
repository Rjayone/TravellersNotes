////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2005.
// -------------------------------------------------------------------------
//  File name:   MultiThread.h
//  Version:     v1.00
//  Compilers:   Visual Studio.NET 2003
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __MultiThread_h__
#define __MultiThread_h__
#pragma once

#if CRY_PLATFORM_LINUX || CRY_PLATFORM_ANDROID || CRY_PLATFORM_APPLE
#include <sched.h>
#endif

#include "CryAssert.h"

#define WRITE_LOCK_VAL (1<<16)

//as PowerPC operates via cache line reservation, lock variables should reside ion their own cache line
template <class T>
struct SAtomicVar
{
	T val;

	inline operator T()const{return val;}
	inline operator T()volatile const{return val;}
	inline void operator =(const T& rV){val = rV;return *this;}
	inline void Assign(const T& rV){val = rV;}
	inline void Assign(const T& rV)volatile{val = rV;}
	inline T* Addr() {return &val;}
	inline volatile T* Addr() volatile {return &val;}

	inline bool operator<(const T& v)const{return val < v;}
	inline bool operator<(const SAtomicVar<T>& v)const{return val < v.val;}
	inline bool operator>(const T& v)const{return val > v;}
	inline bool operator>(const SAtomicVar<T>& v)const{return val > v.val;}
	inline bool operator<=(const T& v)const{return val <= v;}
	inline bool operator<=(const SAtomicVar<T>& v)const{return val <= v.val;}
	inline bool operator>=(const T& v)const{return val >= v;}
	inline bool operator>=(const SAtomicVar<T>& v)const{return val >= v.val;}
	inline bool operator==(const T& v)const{return val == v;}
	inline bool operator==(const SAtomicVar<T>& v)const{return val == v.val;}
	inline bool operator!=(const T& v)const{return val != v;}
	inline bool operator!=(const SAtomicVar<T>& v)const{return val != v.val;}
	inline T operator*(const T& v)const{return val * v;}
	inline T operator/(const T& v)const{return val / v;}
	inline T operator+(const T& v)const{return val + v;}
	inline T operator-(const T& v)const{return val - v;}

	inline bool operator<(const T& v)volatile const{return val < v;}
	inline bool operator<(const SAtomicVar<T>& v)volatile const{return val < v.val;}
	inline bool operator>(const T& v)volatile const{return val > v;}
	inline bool operator>(const SAtomicVar<T>& v)volatile const{return val > v.val;}
	inline bool operator<=(const T& v)volatile const{return val <= v;}
	inline bool operator<=(const SAtomicVar<T>& v)volatile const{return val <= v.val;}
	inline bool operator>=(const T& v)volatile const{return val >= v;}
	inline bool operator>=(const SAtomicVar<T>& v)volatile const{return val >= v.val;}
	inline bool operator==(const T& v)volatile const{return val == v;}
	inline bool operator==(const SAtomicVar<T>& v)volatile const{return val == v.val;}
	inline bool operator!=(const T& v)volatile const{return val != v;}
	inline bool operator!=(const SAtomicVar<T>& v)volatile const{return val != v.val;}
	inline T operator*(const T& v)volatile const{return val * v;}
	inline T operator/(const T& v)volatile const{return val / v;}
	inline T operator+(const T& v)volatile const{return val + v;}
	inline T operator-(const T& v)volatile const{return val - v;}
};

typedef SAtomicVar<int> TIntAtomic;
typedef SAtomicVar<unsigned int> TUIntAtomic;
typedef SAtomicVar<float> TFloatAtomic;

#define __add_db16cycl__ NIntrinsics::YieldFor16Cycles();

	void CrySpinLock(volatile int *pLock,int checkVal,int setVal);
  void CryReleaseSpinLock	(volatile int*, int);

	LONG   CryInterlockedIncrement( int volatile *lpAddend );
	LONG   CryInterlockedDecrement( int volatile *lpAddend );
	LONG   CryInterlockedExchangeAdd(LONG volatile * lpAddend, LONG Value);
	LONG	 CryInterlockedCompareExchange(LONG volatile * dst, LONG exchange, LONG comperand);
	void*	 CryInterlockedCompareExchangePointer(void* volatile * dst, void* exchange, void* comperand);
  void*	 CryInterlockedExchangePointer       (void* volatile * dst, void* exchange);

	void*  CryCreateCriticalSection();
  void   CryCreateCriticalSectionInplace(void*);
	void   CryDeleteCriticalSection( void *cs );
  void   CryDeleteCriticalSectionInplace( void *cs );
	void   CryEnterCriticalSection( void *cs );
	bool   CryTryCriticalSection( void *cs );
	void   CryLeaveCriticalSection( void *cs );

#if CRY_PLATFORM_ORBIS
	LONG   CryInterlockedExchange(LONG volatile *addr, LONG exchange);
	#define InterlockedExchange CryInterlockedExchange
#endif

// Yield intervals when spinning, need to be power-of-two minus one
static const uint32 kSoftYieldInterval = 0x3FF;
static const uint32 kHardYieldInterval = 0x1FFF;

ILINE void CrySpinLock(volatile int *pLock,int checkVal,int setVal)
{ 
#ifdef _CPU_X86
# ifdef __GNUC__
	int val;
	__asm__ __volatile__ (
		"0:     mov %[checkVal], %%eax\n"
		"       lock cmpxchg %[setVal], (%[pLock])\n"
		"       jnz 0b"
		: "=m" (*pLock)
		: [pLock] "r" (pLock), "m" (*pLock),
		  [checkVal] "m" (checkVal),
		  [setVal] "r" (setVal)
		: "eax", "cc", "memory"
		);
# else //!__GNUC__
	__asm
	{
		mov edx, setVal
		mov ecx, pLock
Spin:
		// Trick from Intel Optimizations guide
#ifdef _CPU_SSE
		pause
#endif 
		mov eax, checkVal
		lock cmpxchg [ecx], edx
		jnz Spin
	}
# endif //!__GNUC__
#else // !_CPU_X86
#if CRY_PLATFORM_ORBIS
	uint32 loops = 0;
	while (sceAtomicCompareAndSwap32((volatile int32_t*)pLock, (int32_t)checkVal, (int32_t)setVal)!=checkVal)
	{
		_mm_pause();

		if (!(++loops & kHardYieldInterval))
		{
			sceKernelUsleep(1); // give threads with other prio chance to run
		}
		else if(!(loops & kSoftYieldInterval))
		{
			scePthreadYield(); // give threads with same prio chance to run
		}
	}

# elif CRY_PLATFORM_LINUX || CRY_PLATFORM_ANDROID || CRY_PLATFORM_APPLE
//    register int val;
//	__asm__ __volatile__ (
//		"0:     mov %[checkVal], %%eax\n"
//		"       lock cmpxchg %[setVal], (%[pLock])\n"
//		"       jnz 0b"
//		: "=m" (*pLock)
//		: [pLock] "r" (pLock), "m" (*pLock),
//		  [checkVal] "m" (checkVal),
//		  [setVal] "r" (setVal)
//		: "eax", "cc", "memory"
//		);
	//while(CryInterlockedCompareExchange((volatile long*)pLock,setVal,checkVal)!=checkVal) ;
	uint32 loops = 0;
	while ( __sync_val_compare_and_swap((volatile int32_t*)pLock, (int32_t)checkVal, (int32_t)setVal) != checkVal)
	{
#	if !CRY_PLATFORM_ANDROID
		_mm_pause();
#	endif 

		if(!(++loops & kHardYieldInterval))
		{
			usleep(1); // give threads with other prio chance to run
		}
		else if (!(loops & kSoftYieldInterval))
		{
			sched_yield(); // give threads with same prio chance to run
		}		
	}
# else
	// NOTE: The code below will fail on 64bit architectures!
	while(_InterlockedCompareExchange((volatile LONG*)pLock,setVal,checkVal)!=checkVal) 
	{
		_mm_pause();
	}
# endif
#endif
}

ILINE void CryReleaseSpinLock(volatile int *pLock,int setVal)
{ 
  *pLock = setVal;
}

//////////////////////////////////////////////////////////////////////////
#if CRY_PLATFORM_APPLE || (CRY_PLATFORM_LINUX && CRY_PLATFORM_64BIT)
// Fixes undefined reference to CryInterlockedAdd(unsigned long volatile*, long) on
// Mac and linux.
ILINE void CryInterLockedAdd(volatile LONG *pVal, LONG iAdd)
{
    (void) CryInterlockedExchangeAdd(pVal,iAdd); 
}


/*
ILINE void CryInterLockedAdd(volatile unsigned long *pVal, long iAdd)
{
    long r;
	__asm__ __volatile__ (
	#if (CRY_PLATFORM_LINUX && CRY_PLATFORM_64BIT) || CRY_PLATFORM_MAC  // long is 64 bits on amd64.
		"lock ; xaddq %0, (%1) \n\t"
	#else
		"lock ; xaddl %0, (%1) \n\t"
	#endif
		: "=r" (r)
		: "r" (pVal), "0" (iAdd)
		: "memory"
	);
    (void) r;
}*/
/*ILINE void CryInterlockedAdd(volatile size_t *pVal, ptrdiff_t iAdd) {
   //(void)CryInterlockedExchangeAdd((volatile long*)pVal,(long)iAdd);
    (void) __sync_fetch_and_add(pVal,iAdd);
}*/

#endif
ILINE void CryInterlockedAdd(volatile int *pVal, int iAdd)
{
#ifdef _CPU_X86
# ifdef __GNUC__
	__asm__ __volatile__ (
		"        lock add %[iAdd], (%[pVal])\n"
		: "=m" (*pVal)
		: [pVal] "r" (pVal), "m" (*pVal), [iAdd] "r" (iAdd)
		);
# else
	__asm
	{
		mov edx, pVal
		mov eax, iAdd
		lock add [edx], eax
	}
# endif
#else
	// NOTE: The code below will fail on 64bit architectures!
#if CRY_PLATFORM_DURANGO || (CRY_PLATFORM_WINDOWS && CRY_PLATFORM_64BIT)
  _InterlockedExchangeAdd((volatile LONG*)pVal,iAdd);
# elif CRY_PLATFORM_ORBIS
	sceAtomicAdd32((int32_t*)pVal, (int32_t)iAdd);
#elif CRY_PLATFORM_LINUX || CRY_PLATFORM_ANDROID || CRY_PLATFORM_APPLE
  CryInterlockedExchangeAdd((volatile LONG*)pVal,iAdd);
#elif CRY_PLATFORM_APPLE
	OSAtomicAdd32(iAdd, (volatile LONG*)pVal);
#else
  InterlockedExchangeAdd((volatile LONG*)pVal,iAdd);
#endif

#endif
}

ILINE void CryInterlockedAddSize(volatile size_t* pVal, ptrdiff_t iAdd)
{
#if CRY_PLATFORM_64BIT
	#if CRY_PLATFORM_DURANGO || CRY_PLATFORM_WINDOWS
	  _InterlockedExchangeAdd64((volatile __int64*)pVal,iAdd);
	#elif CRY_PLATFORM_ORBIS
		sceAtomicAdd64((int64_t*)pVal, (int64_t)iAdd);
	#elif CRY_PLATFORM_LINUX || CRY_PLATFORM_ANDROID || CRY_PLATFORM_APPLE
	  (void)__sync_fetch_and_add((int64_t*)pVal, (int64_t)iAdd);
	#else
		int64 x, n;
		do
		{
			x = (int64)*pVal;
			n = x + iAdd;
		}
		while (CryInterlockedCompareExchange64((volatile int64*)pVal, n, x) != x);
	#endif
#else
	CryInterlockedAdd((volatile int*)pVal, (int)iAdd);
#endif
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// CryInterlocked*SList Function, these are specialized C-A-S
// functions for single-linked lists which prevent the A-B-A problem there
// there are implemented in the platform specific CryThread_*.h files
// TODO clean up the interlocked function the same was the CryThread_* header are

//TODO somehow get their real size on WIN (without including windows.h...)
//NOTE: The sizes are verifyed at compile-time in the implementation functions, but this is still ugly
#if (CRY_PLATFORM_WINDOWS && CRY_PLATFORM_64BIT) || CRY_PLATFORM_DURANGO 
_MS_ALIGN(16)
#elif CRY_PLATFORM_WINDOWS
_MS_ALIGN(8)
#endif
struct SLockFreeSingleLinkedListEntry
{
	SLockFreeSingleLinkedListEntry * volatile pNext;
}
#if (CRY_PLATFORM_LINUX && CRY_PLATFORM_32BIT) || CRY_PLATFORM_ANDROID
_ALIGN(8)
#elif CRY_PLATFORM_APPLE || (CRY_PLATFORM_LINUX && CRY_PLATFORM_64BIT)
_ALIGN(16)
#endif
;

#if (CRY_PLATFORM_WINDOWS && CRY_PLATFORM_64BIT) || CRY_PLATFORM_DURANGO
_MS_ALIGN(16)
#elif CRY_PLATFORM_WINDOWS
_MS_ALIGN(8)
#endif
struct SLockFreeSingleLinkedListHeader
{
	// Initializes the single-linked list.
	friend void CryInitializeSListHead(SLockFreeSingleLinkedListHeader& list);

	// Push one element atomically onto the front of a single-linked list.
	friend void CryInterlockedPushEntrySList(SLockFreeSingleLinkedListHeader& list, SLockFreeSingleLinkedListEntry& element);

	// Push a list of elements atomically onto the front of a single-linked list.
	// Note: The entries must already be linked (ie, last must be reachable by moving through pNext starting at first).
	friend void CryInterlockedPushListSList(SLockFreeSingleLinkedListHeader& list, SLockFreeSingleLinkedListEntry& first, SLockFreeSingleLinkedListEntry& last, uint32 count);

	// Retrieves a pointer to the first item on a single-linked list.
	// Note: This does not remove the item from the list, and it's unsafe to inspect anything but the returned address.
	friend void* CryRtlFirstEntrySList(SLockFreeSingleLinkedListHeader& list);

	// Pops one element atomically from the front of a single-linked list, and returns a pointer to the item.
	// Note: If the list was empty, nullptr is returned instead.
	friend void* CryInterlockedPopEntrySList(SLockFreeSingleLinkedListHeader& list);

	// Flushes the entire single-linked list, and returns a pointer to the first item that was on the list.
	// Note: If the list was empty, nullptr is returned instead.
	friend void* CryInterlockedFlushSList(SLockFreeSingleLinkedListHeader& list);

private:
	SLockFreeSingleLinkedListEntry* volatile pNext;
#if /*defined(_CPU_ARM) ||*/ defined(_CPU_MIPS) || CRY_PLATFORM_ORBIS
	// Only need "salt" on platforms using CAS (and ORBIS uses embedded salt)
#elif CRY_PLATFORM_APPLE || CRY_PLATFORM_LINUX || CRY_PLATFORM_ANDROID
	// If pointers 32bit, salt should be as well.  Otherwise we get 4 bytes of padding between pNext and salt and CAS operations fail
#if CRY_PLATFORM_64BIT
	volatile uint64 salt;
#else
	volatile uint32 salt;
#endif
#endif
}
#if (CRY_PLATFORM_LINUX && CRY_PLATFORM_32BIT) || CRY_PLATFORM_ANDROID
_ALIGN(8)
#elif CRY_PLATFORM_APPLE || (CRY_PLATFORM_LINUX && CRY_PLATFORM_64BIT)
_ALIGN(16)
#endif
;

ILINE void CryReadLock(volatile int *rw)
{
	CryInterlockedAdd(rw,1);
#ifdef NEED_ENDIAN_SWAP
	volatile char *pw=(volatile char*)rw+1;
#else
	volatile char *pw=(volatile char*)rw+2;
#endif

	uint32 loops = 0;
	for(;*pw;) 
	{
#if !CRY_PLATFORM_ANDROID
		_mm_pause();
#endif

		if(!(++loops & kHardYieldInterval))
		{
			// give other threads with other prio right to run
#if CRY_PLATFORM_ORBIS
			sceKernelUsleep(1);
#elif CRY_PLATFORM_LINUX || CRY_PLATFORM_ANDROID
			usleep(1);
#endif
		}
		else if (!(loops & kSoftYieldInterval))
		{
			// give threads with same prio chance to run
#if CRY_PLATFORM_ORBIS
			scePthreadYield();
#elif CRY_PLATFORM_LINUX || CRY_PLATFORM_ANDROID
			sched_yield();
#endif
		}
	}
}

ILINE void CryReleaseReadLock(volatile int* rw)
{
	CryInterlockedAdd(rw,-1);
}

ILINE void CryWriteLock(volatile int* rw)
{
	CrySpinLock(rw,0,WRITE_LOCK_VAL);
}

ILINE void CryReleaseWriteLock(volatile int* rw)
{
	CryInterlockedAdd(rw,-WRITE_LOCK_VAL);
}

//////////////////////////////////////////////////////////////////////////
struct ReadLock
{
	ReadLock(volatile int &rw) : prw(&rw)
	{
		CryReadLock(prw);
	}

	~ReadLock()
	{
		CryReleaseReadLock(prw);
	}

private:
	volatile int* const prw;
};

struct ReadLockCond
{
	ReadLockCond(volatile int &rw,int bActive) : prw(&rw), iActive(0)
	{
		if (bActive)
		{
			iActive = 1;
			CryReadLock(prw);
		}
	}
	
	void SetActive(int bActive=1)
	{
		iActive = bActive;
	}
	
	void Release()
	{
		CryInterlockedAdd(prw, -iActive);
	}
	
	~ReadLockCond()
	{
		CryInterlockedAdd(prw, -iActive);
	}

private:
	int iActive;
	volatile int* const prw;
};

//////////////////////////////////////////////////////////////////////////
struct WriteLock
{
	WriteLock(volatile int &rw) : prw(&rw)
	{
		CryWriteLock(&rw);
	}
	
	~WriteLock()
	{
		CryReleaseWriteLock(prw);
	}

private:
	volatile int* const prw;
};

//////////////////////////////////////////////////////////////////////////
struct WriteAfterReadLock
{
	WriteAfterReadLock(volatile int &rw) : prw(&rw)
	{
		CrySpinLock(&rw,1,WRITE_LOCK_VAL+1);
	}

	~WriteAfterReadLock()
	{
		CryInterlockedAdd(prw,-WRITE_LOCK_VAL);
	}

private:
	volatile int* const prw;
};

//////////////////////////////////////////////////////////////////////////
struct WriteLockCond
{
	WriteLockCond(volatile int &rw,int bActive=1) : prw(&rw), iActive(0)
	{
		if (bActive)
		{
			iActive = WRITE_LOCK_VAL;
			CrySpinLock(prw, 0, iActive);
		}
	}

	WriteLockCond() : iActive(0), prw(&iActive) {}

	~WriteLockCond()
	{ 
		CryInterlockedAdd(prw,-iActive); 
	}

	void SetActive(int bActive=1)
	{
		iActive = -bActive & WRITE_LOCK_VAL;
	}
	
	void Release()
	{
		CryInterlockedAdd(prw,-iActive);
	}
	
	// Not private because used directly in Physics RWI
	int iActive;
	volatile int* prw;
};


#if CRY_PLATFORM_LINUX || CRY_PLATFORM_ANDROID || CRY_PLATFORM_APPLE
ILINE int64 CryInterlockedCompareExchange64( volatile int64 *addr, int64 exchange, int64 comperand )
{
    return __sync_val_compare_and_swap(addr,comperand,exchange);
    // This is OK, because long is signed int64 on Linux x86_64
	//return CryInterlockedCompareExchange((volatile long*)addr, (long)exchange, (long)comperand);
}

ILINE int64 CryInterlockedExchange64( volatile int64 *addr, int64 exchange)
{
	__sync_synchronize();
	return __sync_lock_test_and_set(addr, exchange);
}
#else
ILINE int64 CryInterlockedCompareExchange64( volatile int64 *addr, int64 exchange, int64 compare )
{
	// forward to system call
#if CRY_PLATFORM_ORBIS
	return sceAtomicCompareAndSwap64((volatile int64_t*)addr, (int64_t)compare, (int64_t)exchange);
#else
	return _InterlockedCompareExchange64( (volatile int64*)addr, exchange,compare);
#endif
}
#endif

//////////////////////////////////////////////////////////////////////////
#if defined(EXCLUDE_PHYSICS_THREAD) 
	 ILINE void SpinLock(volatile int *pLock,int checkVal,int setVal) { *(int*)pLock=setVal; } 
	 ILINE void AtomicAdd(volatile int *pVal, int iAdd) {	*(int*)pVal+=iAdd; }
	 ILINE void AtomicAdd(volatile unsigned int *pVal, int iAdd) { *(unsigned int*)pVal+=iAdd; }

   ILINE void JobSpinLock(volatile int *pLock,int checkVal,int setVal) { CrySpinLock(pLock,checkVal,setVal); } 
#else
	ILINE void SpinLock(volatile int *pLock,int checkVal,int setVal) { CrySpinLock(pLock,checkVal,setVal); } 
	ILINE void AtomicAdd(volatile int *pVal, int iAdd) {	CryInterlockedAdd(pVal,iAdd); }
	ILINE void AtomicAdd(volatile unsigned int *pVal, int iAdd) { CryInterlockedAdd((volatile int*)pVal,iAdd); }

  ILINE void JobSpinLock(volatile int *pLock,int checkVal,int setVal) { SpinLock(pLock,checkVal,setVal); } 
#endif

ILINE void JobAtomicAdd(volatile int *pVal, int iAdd) {	CryInterlockedAdd(pVal,iAdd); }
ILINE void JobAtomicAdd(volatile unsigned int *pVal, int iAdd) { CryInterlockedAdd((volatile int*)pVal,iAdd); }


#endif // __MultiThread_h__
