#include "Unreal.h"
#include "Memory.h"
#include <intrin.h>
#include <windows.h>

void Unreal::Init()
{
	uintptr_t ModuleBase = Memory::GetModuleBase();

	Natives::StaticFindObject = reinterpret_cast<decltype(Natives::StaticFindObject)>(ModuleBase + 0xBDA70);
	Natives::ProcessEvent = reinterpret_cast<decltype(Natives::ProcessEvent)>(ModuleBase + 0x714D0);
	Natives::GNames = reinterpret_cast<decltype(Natives::GNames)>(ModuleBase + 0x1E8AAF8);
}

void** UObject::GetVTable()
{
	return VTable;
}

uint32_t UObject::GetInternalIndex()
{
	return *reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(this) + 0x20);
}

UObject* UObject::GetOuter()
{
	return reinterpret_cast<UObject*>(reinterpret_cast<uintptr_t>(this) + 0x28);
}

FName& UObject::GetName()
{
	return *reinterpret_cast<FName*>(reinterpret_cast<uintptr_t>(this) + 0x2C);
}

UClass* UObject::GetClass()
{
	return *reinterpret_cast<UClass**>(reinterpret_cast<uintptr_t>(this) + 0x34);
}

UField* UField::GetNext()
{
	return *reinterpret_cast<UClass**>(reinterpret_cast<uintptr_t>(this) + 0x38);
}

UStruct* UStruct::GetSuperStruct()
{
	return *reinterpret_cast<UStruct**>(reinterpret_cast<uintptr_t>(this) + 0x48);
}

UField* UStruct::GetField()
{
	return *reinterpret_cast<UStruct**>(reinterpret_cast<uintptr_t>(this) + 0x4C);
}

std::string FName::ToString() const
{
	return Natives::GNames->GetByIndex(Index)->GetName();
}

std::string FString::ToString() const
{
	int size = WideCharToMultiByte(CP_UTF8, 0, Data, Count - 1, nullptr, 0, nullptr, nullptr);
	std::string str(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, Data, Count - 1, &str[0], size, nullptr, nullptr);
	return str;
}


uintptr_t GetXorKey(size_t index)
{
	// TO-DO: cleanup
	static uintptr_t TEB = __readfsdword(0x2C);
	unsigned int TlsIndex = *(unsigned int*)(Memory::GetModuleBase() + 0x392F380);
	return *(uintptr_t*)(*((uint32_t*)TEB + TlsIndex) + 4 * (index & 0x7FF) + 40);
}

