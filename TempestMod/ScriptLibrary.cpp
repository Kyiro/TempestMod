#include "ScriptLibrary.h"
#include "Platform.h"
#include "Unreal.h"
#include <Windows.h>
#include <detours/detours.h>
#include <print>

bool bLazyLoaded = false;
UFunction* HookFunction = nullptr;

struct HookFunction_Args
{
	UClass* TargetClass;
	FString TargetFunction;
	UClass* DetourClass;
};

bool LazyInit()
{
	auto TempestModLibrary = UObject::FindObject(L"TgMod.TempestModLibrary");

	if (TempestModLibrary == nullptr) return false;

	HookFunction = UObject::FindObject<UFunction>(L"TgMod.TempestModLibrary.HookFunction");

	return true;
}

void __fastcall hProcessEvent(THIS(UObject*, This), UFunction* Function, void* Parms, int32_t Flags)
{
	if (bLazyLoaded)
	{
		std::string FunctionName = Function->GetName();

		std::println("func name: {}", FunctionName);

		if (FunctionName == "HookFunction")
		{
			std::println("HookFunction called! Function: {:x}", (uintptr_t)Function);

			auto Args = (HookFunction_Args*)Parms;

			std::println("Target: {}", Args->TargetFunction.ToString());

			return;
		}
	}
	else if (LazyInit())
	{
		bLazyLoaded = true;
	}

	return Natives::ProcessEvent(This, Function, Parms, Flags);
}

void ScriptLibrary::Init()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach(&(PVOID&)Natives::ProcessEvent, hProcessEvent);

	DetourTransactionCommit();
}
