#include <Windows.h>
#include "Assembly.h"
#include "ScriptLibrary.h"
#include "Unreal.h"

DWORD WINAPI Main(LPVOID lpParam)
{
    Unreal::Init();
	Assembly::InitHook();
    ScriptLibrary::Init();

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
        CreateThread(nullptr, 0, Main, hModule, 0, nullptr);

    return true;
}