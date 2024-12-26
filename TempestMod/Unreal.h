#pragma once

#include <chrono>
#include <cstdint>
#include "Platform.h"
#include "Unreal.h"

class UObject;
class UField;
class UStruct;
class UClass;
struct FName;
struct FNameEntry;
template <typename T> struct TAntiCheatArray;

namespace Natives
{
	inline TAntiCheatArray<FNameEntry*>* GNames;

	inline UObject* (*StaticFindObject)(UClass* Class, UObject* InOuter, const wchar_t* Name, int ExactClass);
	inline void(THISCALL* ProcessEvent)(UObject* This, UObject* Function, void* Parms, int32_t Flags);
}

namespace Unreal
{
	void Init();
}

class UObject
{
	void** VTable;

public:
	void** GetVTable();
	uint32_t GetInternalIndex();
	UObject* GetOuter();
	FName& GetName();
	UClass* GetClass();

	template <typename T = UObject>
	static T* FindObject(const wchar_t* Name, UClass* Class = nullptr)
	{
		return (T*)Natives::StaticFindObject(Class, nullptr, Name, false);
	}
};

class UField : public UObject
{
	UField* GetNext();
};

class UStruct : public UField
{
	UStruct* GetSuperStruct();
	UField* GetField();
};

class UClass : public UStruct
{
};

class UFunction : public UStruct
{
};

struct FName
{
	uint32_t Index;
	uint32_t Number;

	operator std::string() const {
		return ToString();
	}

	std::string ToString() const;
};

struct FNameEntry
{
	static const auto NAME_WIDE_MASK = 0x1;
	static const auto NAME_INDEX_SHIFT = 1;

	uint64_t Flags;
	uint32_t Index;
	char pad_000C[4];
	char Text[1024];

	int32_t GetIndex() const
	{
		return Index >> NAME_INDEX_SHIFT;
	}

	bool IsWide() const
	{
		return Index & NAME_WIDE_MASK;
	}

	const char* GetAnsiName() const
	{
		return Text;
	}

	std::string GetName() const
	{
		return Text;
	}
};

template<class T>
struct TArray
{
	friend struct FString;

public:
	TArray()
	{
		Data = nullptr;
		Count = Max = 0;
	};

	size_t Num() const
	{
		return Count;
	};

	T& operator[](size_t i)
	{
		return Data[i];
	};

	const T& operator[](size_t i) const
	{
		return Data[i];
	};

	bool IsValidIndex(size_t i) const
	{
		return i < Num();
	}

private:
	T* Data;
	int32_t Count;
	int32_t Max;
};

struct FString : public TArray<wchar_t>
{
	std::string ToString() const;
};

uintptr_t GetXorKey(size_t index);

template<class T>
struct TAntiCheatArray
{
	friend struct FString;
private:
	uintptr_t Decrypt(size_t i) {
		return reinterpret_cast<uintptr_t>(Data[i]) ^ GetXorKey(i);
	}

	T GetData(size_t i) {
		return reinterpret_cast<T>(Decrypt(i));
	}
public:
	TAntiCheatArray()
	{
		Data = nullptr;
		Count = Max = 0;
	};

	size_t Num() const
	{
		return Count;
	};

	T operator[](size_t i)
	{
		return GetData(i);
	};

	T GetByIndex(size_t i)
	{
		return GetData(i);
	}

	bool IsValidIndex(size_t i)
	{
		uintptr_t addr = Decrypt(i);

		if (addr == NULL)
			return false;

		return i < Num();
	}

private:
	T* Data;
	int32_t Max;
	int32_t Count;
};