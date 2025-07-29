#include "Assembly.h"
#include "Platform.h"
#include <Windows.h>
#include <detours/detours.h>

inline int(THISCALL* oUnknownFunc)(void* This, void* a2);
inline int(THISCALL* oBotFunc)(void* This, int a2);
inline int(THISCALL* AssemblyLoad)(void* This, char a2);
inline void* AssemblyGlobal;
inline bool bLoadedAssembly;

int __fastcall hUnknownFunc(THIS(void* This), void* a2)
{
	if (!bLoadedAssembly && AssemblyLoad(AssemblyGlobal, 1))
	{
		bLoadedAssembly = true;
	}

	return oUnknownFunc(This, a2);
}

int __fastcall hBotFunc(THIS(void* This), int a2)
{
	return 1;
}

void Assembly::InitHook()
{
	auto ModuleBase = GetModuleHandle(nullptr);

	oBotFunc = reinterpret_cast<decltype(oBotFunc)>(ModuleBase + 0xBB3530);

	oUnknownFunc = reinterpret_cast<decltype(oUnknownFunc)>(ModuleBase + 0x1924C);
	AssemblyLoad = reinterpret_cast<decltype(AssemblyLoad)>(ModuleBase + 0x31CCB4);
	AssemblyGlobal = reinterpret_cast<decltype(AssemblyGlobal)>(ModuleBase + 0x86E3E6);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread()); 

	DetourAttach(&(PVOID&)oUnknownFunc, hUnknownFunc);
	DetourAttach(&(PVOID&)oBotFunc, hBotFunc);

	DetourTransactionCommit();
}