#include "Assembly.h"
#include "Memory.h"
#include "Platform.h"
#include <Windows.h>
#include <detours/detours.h>

inline int(THISCALL* oUnknownFunc)(void* This, void* a2);
inline int(THISCALL* AssemblyLoad)(void* This, char a2);
inline void* AssemblyGlobal;
inline bool bLoadedAssembly;

int __fastcall hUnknownFunc(THIS(void*, This), void* a2)
{
	if (!bLoadedAssembly && AssemblyLoad(AssemblyGlobal, 1))
	{
		bLoadedAssembly = true;
	}

	return oUnknownFunc(This, a2);
}

void Assembly::InitHook()
{
	auto ModuleBase = Memory::GetModuleBase();

	oUnknownFunc = reinterpret_cast<decltype(oUnknownFunc)>(ModuleBase + 0x63EB0);
	AssemblyLoad = reinterpret_cast<decltype(AssemblyLoad)>(ModuleBase + 0xC46F60);
	AssemblyGlobal = reinterpret_cast<decltype(AssemblyGlobal)>(ModuleBase + 0x1EF9970);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread()); 

	DetourAttach(&(PVOID&)oUnknownFunc, hUnknownFunc);

	DetourTransactionCommit();
}