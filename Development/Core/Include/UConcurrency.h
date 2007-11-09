#ifndef _UCONCURRENCY_H_
#define _UCONCURRENCY_H_
#include "Core.h"
#include "UTypes.h"
#include "UPlatform.h"

// Interlocked data types.
class KIInt
{
private:
	volatile int Value;
public:
	void Set(int I)
	{
		appInterlockedExchange((volatile LONG*)&Value, I);
	}
	int Get() const
	{
		int retValue;
		appInterlockedExchange((volatile LONG*)&retValue, Value);
		return retValue;
	}
	KIInt( const KIInt& I)
	{
		Set(I.Get());
	}
	KIInt( int I )
	{
		Set(I);
	}
	KIInt()
	{
	}
	int Increment()
	{
		return appInterlockedIncrement((volatile LONG*)&Value);
	}
	int Decrement()
	{
		return appInterlockedDecrement((volatile LONG*)&Value);
	}
	void Add(int V)
	{
		appInterlockedExchangeAdd((volatile LONG*)&Value, V);
	}
	int operator++()
	{
		int i = Get();
		Increment();
		return i;
	}
	int operator--()
	{
		int i = Get();
		Decrement();
		return i;
	}
	int Or( int V )
	{
		return appInterlockedOr((volatile LONG*)&Value, V);
	}
};

template<typename T>
class KIPointer
{
private:
	volatile T* Ptr;
public:
	void Set(T* P)
	{
		appInterlockedExchangePointer((volatile void**)&Ptr, P);
	}
	T* Get() const
	{
		T* retP;
		appInterlockedExchangePointer((volatile void**)&retP, (void*)Ptr);
		return retP;
	}
	KIPointer(T* P)
	{
		Set(P);
	}
	KIPointer(const KIPointer<T>& P)
	{
		Set(P.Get);
	}
	T* CompareExchange( T* Exchange, T* Comparand )
	{
		return (T*)appInterlockedCompareExchangePointer((volatile void**)&Ptr, Exchange, Comparand);
	}
	KIPointer<T>& operator=(T* P)
	{
		Set(P);
		return *this;
	}
	KIPointer<T>& operator=(KIPointer<T>& P)
	{
		Set(P.Get());
		return *this;
	}
	operator T*()
	{
		return Get();
	}
};

//////////////////////////////////////////////////////////////////////////////
// KIAppendList
// Templated Type-Safe Singly linked list. Allows append to end, iteration
// from start to end, and deletion from anywhere.
// Delete will stall while an iteration is being run, and vice-versa.
//////////////////////////////////////////////////////////////////////////////
template<typename T, typename Alloc = NonManagedAllocator<T>, typename Verification = DebugAssertVerification>
class KIAppendList
{
	struct KIAppendLink
	{
		const T Value;
		KIPointer<KIAppendLink> Next;
		KIAppendLink(const T& Val) : Value(Val), Next(NULL){}
		~KIAppendLink()
		{
			if(T::NeedsDestructor)
			{
				Value.~T();
			}
		}
	};
	KIPointer<KIAppendLink> First;
	KIPointer<KIAppendLink>* Last;
	enum { WaitMode_Delete, WaitMode_Iterate } WaitMode;
public:
	KIAppendList() : First(NULL), Last(&First){}
	~KIAppendList(){}
	void Append(const T& Item)
	{
		KIPointer<KIAppendLink>* OldLast;
		KIAppendLink* Link = Alloc::Alloc(sizeof(KIAppendLink));
		new(Link) KIAppendLink(Item);

		do
		{
			OldLast = Last;
		}
		while(OldLast != Last.CompareExchange(&Link.Next, OldLast))
		*OldLast = Link;
	}
	void Remove(const T& Item)
	{
		//What happens if I try to delete two at a time??
		KIPointer<KIAppendLink>* OldLast = &First;
		while(*OldLast != NULL)
		{
			if((*OldLast)->Value == Item)
			{
				KIAppendLink* Link = *OldLast;
				if(OldLast->CompareExchange(&Link->Next, Link) == Link)
				{
					Alloc::Free(Link);
				}
			}
			else
				OldLast = &(*OldLast)->Next
		}
	}
	class Iterator
	{
		KIAppendLink* Current;
		friend class KIAppendList;
		Iterator(KIAppendLink* First) : Current(First){}
		~Iterator(){}
	public:
		Iterator& operator++()
		{
			Current = Current->Next;
		}
		operator T()
		{
			return Current->Value;
		}
	};
};


//////////////////////////////////////////////////////////////////////////////
// KIQueue
// Templated Type-Safe Queue.
//////////////////////////////////////////////////////////////////////////////
template<typename T, typename Alloc = NonManagedAllocator<void>, typename Verification = DebugAssertVerification>
class KIQueue
{
	struct KIQueueLink
	{
		const T Value;
		KIPointer<KIQueueLink> Next;
		KIQueueLink(const T& Val) : Value(Val), Next(NULL){}
		~KIQueueLink()
		{
			if(T::NeedsDestructor)
			{
				Value.~T();
			}
		}
	};
	KIPointer<KIQueueLink> DeQ;
	KIPointer<KIPointer<KIQueueLink>> EnQ;
public:
	KIQueue() : DeQ(NULL), EnQ(&DeQ){}
	~KIQueue(){}
	void Enqueue(const T& Item)
	{
		KIPointer<KIQueueLink>* OldEnQ;
		KIQueueLink* Link = (KIQueueLink*)Alloc::Alloc(sizeof(KIQueueLink));
		new(Link) KIQueueLink(Item);

		do
		{
			OldEnQ = EnQ;
		}
		while(OldEnQ != EnQ.CompareExchange(&Link->Next, OldEnQ));
		*OldEnQ = Link;
	}
	bool Dequeue(T& DeQValue)
	{		
		KIQueueLink* OldDeQ = DeQ;

		do
		{
			OldDeQ = DeQ;
			if(OldDeQ == NULL)
				return false;

		}
		while(OldDeQ != DeQ.CompareExchange(OldDeQ->Next, OldDeQ));

		DeQValue = OldDeQ->Value;
		Alloc::Free(OldDeQ);

		return true;

	}
};

#endif //_UCONCURRENCY_H_
