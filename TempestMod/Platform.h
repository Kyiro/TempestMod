#pragma once

#ifdef _WIN64
#define THISCALL __fastcall
#define THIS(type, name) type name
#else
#define THISCALL __thiscall
#define THIS(type, name) type name, int __edx
#endif