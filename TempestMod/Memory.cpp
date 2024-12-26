#include "Memory.h"
#include <Windows.h>
#include <vector>

std::vector<uint8_t> pattern2bytes(const char* pattern)
{
    auto bytes = std::vector<uint8_t> {};
    const auto start = const_cast<char*>(pattern);
    const auto end = const_cast<char*>(pattern) + strlen(pattern);

    for (auto current = start; current < end; ++current)
    {
        if (*current == '?')
        {
            ++current;
            if (*current == '?')
                ++current;
            bytes.push_back(0xFF);
        }
        else
        {
            bytes.push_back(static_cast<uint8_t>(strtoul(current, &current, 16)));
        }
    }
    return bytes;
}

size_t Memory::GetSizeOfImage()
{
	uintptr_t ModuleBase = GetModuleBase();
	PIMAGE_DOS_HEADER DosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(GetModuleBase());
	PIMAGE_NT_HEADERS NtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(ModuleBase + DosHeader->e_lfanew);

	return NtHeaders->OptionalHeader.SizeOfImage;
}

uintptr_t Memory::GetModuleBase()
{
	return reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
}

Memory::Address Memory::FindPattern(const char* signature)
{
    const auto sizeOfImage = GetSizeOfImage();
    auto patternBytes = pattern2bytes(signature);
    const auto scanBytes = reinterpret_cast<const uint8_t*>(GetModuleBase());

    const auto patternSize = patternBytes.size();
    const auto* patternData = patternBytes.data();

    for (size_t i = 0; i <= sizeOfImage - patternSize; ++i)
    {
        bool found = true;

        for (size_t j = 0; j < patternSize; ++j)
        {
            if (scanBytes[i + j] != patternData[j] && patternData[j] != 0xFF)
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            return { reinterpret_cast<uintptr_t>(&scanBytes[i]) };
        }
    }

    return { nullptr };
}
