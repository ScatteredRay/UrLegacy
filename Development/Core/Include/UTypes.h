#ifndef _UTYPES_H_
#define _UTYPES_H_

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int int16;
typedef unsigned short int uint16;
//typedef signed int int;
typedef unsigned int uint;
typedef signed __int64 int64;
typedef unsigned __int64 uint64;
typedef size_t psize;
typedef unsigned char bitfield;

//Temp defines

typedef long LONG;


template <typename T>
class KTypeInfoPrimitiveBase
{
public:
	enum{NeedsConstructor = 0};
	enum{NeedsDestructor = 0};
	enum{Size = sizeof(T)};
};

template <typename T>
class KTypeInfoObjectBase
{
public:
	enum{NeedsConstructor = 1};
	enum{NeedsDestructor = 1};
	enum{Size = sizeof(T)};
};

template<typename T>
class KTypeInfo : public KTypeInfoObjectBase<T> {};

template<typename T>
class KTypeInfo<T*> : public KTypeInfoPrimitiveBase<T*> {};

template<> class KTypeInfo<int8> : public KTypeInfoPrimitiveBase<int8> {};
template<> class KTypeInfo<uint8> : public KTypeInfoPrimitiveBase<uint8> {};
template<> class KTypeInfo<int16> : public KTypeInfoPrimitiveBase<int16> {};
template<> class KTypeInfo<uint16> : public KTypeInfoPrimitiveBase<uint16> {};
template<> class KTypeInfo<int> : public KTypeInfoPrimitiveBase<int> {};
template<> class KTypeInfo<uint> : public KTypeInfoPrimitiveBase<uint> {};
template<> class KTypeInfo<int64> : public KTypeInfoPrimitiveBase<int64> {};
template<> class KTypeInfo<uint64> : public KTypeInfoPrimitiveBase<uint64> {};
template<> class KTypeInfo<float> : public KTypeInfoPrimitiveBase<float> {};
template<> class KTypeInfo<double> : public KTypeInfoPrimitiveBase<double> {};
template<> class KTypeInfo<bool> : public KTypeInfoPrimitiveBase<bool> {};


#include "UPlatform.h" //TODO: what am I doing here?? Split what needs this into file other then types.
#include <assert.h> //TODO: Same here!

template<typename T = void>
class ZeroedAllocator
{
public:
	enum{NeedsMemoryManagement = true};
	static T* Alloc(psize size)
	{
		void* Buffer = sysMalloc(size);
		appMemzero(Buffer, size);
		return (T*)Buffer;
	}
	static T* Realloc(T* P, psize size)
	{
		assert(false); //TODO: implement a zero'd version
		return (T*)sysRealloc(P, size);
	}
	static void Free(T* P)
	{
		sysFree(P);
	}
};

template<typename T = void>
class NonManagedAllocator
{
public:
	enum{NeedsMemoryManagement = true};
	static T* Alloc(psize size)
	{
		return (T*)sysMalloc(size);
	}
	static T* Realloc(T* P, psize size)
	{
		return (T*)sysRealloc(P, size);
	}
	static void Free(T* P)
	{
		sysFree(P);
	}
};

class DebugAssertVerification
{
public:
	static void Check(bool Condition)
	{
		assert(Condition);
	}
};

//////////////////////////////////////////////////////////////////////////////
// KArray
// Templated Array
// Not subclassing a generic buffer class because not only does that
// complicate things, yet we also gain little to none, because a buffer
// is more of an array of bytes, then a array of bytes being a buffer
// and we get that at little to no loss.
//////////////////////////////////////////////////////////////////////////////
template<typename T, typename Alloc = NonManagedAllocator<T>, typename Verification = DebugAssertVerification>
class KArray
{
private:
	T* First;
	uint Count;
	uint Allocated;
	uint GetAllocationSize(uint Num)
	{
		return (Count+Num)*2;
	}
public:
	KArray()
	{
		First = 0;
		Count = 0;
		Allocated = 0;
	}

	~KArray()
	{
		if(!First)
			return;

		if(KTypeInfo<T>::NeedsDestructor)
		{
			for(uint i=0; i<Count; i++)
			{
				First[i].~T();
			}
		}
		Alloc::Free(First);
	}

	void Add(uint Num)
	{
		if(Count+Num > Allocated)
		{
			uint NewAllocatedSize = GetAllocationSize(Num);
			if(Allocated == 0)
				First = Alloc::Alloc(sizeof(T) * NewAllocatedSize);
			else
				First = Alloc::Realloc(First, sizeof(T) * NewAllocatedSize);
			Allocated = NewAllocatedSize;
		}

		// Going to initialize and then increase so we don't need any extra local variables laying around.

		if(KTypeInfo<T>::NeedsConstructor)
		{
			for(uint i=Count; i<Count+Num; i++)
			{
				new(&First[i]) T();
			}
		}

		Count += Num;
	}

	void Remove(uint index)
	{
		for(uint i = index; i < Count-1; i++)
		{
			First[i] = First[i+1];
		}
		Count--;
	}

	void AddItem(const T& Item)
	{
		Add(1);
		First[Count-1] = Item; //TODO: Not working!?!?!?
	}

	bool RemoveItem(const T& Item)
	{
		for(uint i=0; i<Count; i++)
		{
			if(First[i] == Item)
			{
				Remove(i);
				return true;
			}
		}
		return false;
	}

	bool RemoveAllItem(const T& Item)
	{
		bool bFoundItem = false;
		for(uint i=0; i<Count; i++)
		{
			if(First[i] == Item)
			{
				Remove(i--);
				bFoundItem = true;
			}
		}
		return bFoundItem;
	}

	uint Num()
	{
		return Count;
	}

	T& operator[](uint Index)
	{
		Verification::Check(Index<Count);
		Verification::Check(First != NULL);

		return First[Index];
	}
};

#endif //_UTYPES_H_