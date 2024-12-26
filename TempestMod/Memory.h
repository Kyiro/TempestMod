#pragma once

// Based on https://github.com/kem0x/Memcury/blob/master/memcury.h

#include <cstdint>

namespace Memory
{
    class Address
    {
        uintptr_t _address;

    public:
        Address() : _address(0)
        {
        }

        Address(uintptr_t address)
            : _address(address)
        {
        }

        Address(void* address)
            : _address(reinterpret_cast<uintptr_t>(address))
        {
        }

        Address operator=(uintptr_t address)
        {
            _address = address;
            return *this;
        }

        Address operator=(void* address)
        {
            _address = reinterpret_cast<uintptr_t>(address);
            return *this;
        }

        Address operator+(uintptr_t offset)
        {
            return Address(_address + offset);
        }

        bool operator>(uintptr_t offset)
        {
            return _address > offset;
        }

        bool operator>(Address address)
        {
            return _address > address._address;
        }

        bool operator<(uintptr_t offset)
        {
            return _address < offset;
        }

        bool operator<(Address address)
        {
            return _address < address._address;
        }

        bool operator>=(uintptr_t offset)
        {
            return _address >= offset;
        }

        bool operator>=(Address address)
        {
            return _address >= address._address;
        }

        bool operator<=(uintptr_t offset)
        {
            return _address <= offset;
        }

        bool operator<=(Address address)
        {
            return _address <= address._address;
        }

        bool operator==(uintptr_t offset)
        {
            return _address == offset;
        }

        bool operator==(Address address)
        {
            return _address == address._address;
        }

        bool operator!=(uintptr_t offset)
        {
            return _address != offset;
        }

        bool operator!=(Address address)
        {
            return _address != address._address;
        }

        Address RelativeOffset(uint32_t offset)
        {
            _address = ((_address + offset + 4) + *(int32_t*)(_address + offset));
            return *this;
        }

        Address AbsoluteOffset(uint32_t offset)
        {
            _address = _address + offset;
            return *this;
        }

        uintptr_t Get()
        {
            return _address;
        }

        template <typename T>
        T GetAs()
        {
            return reinterpret_cast<T>(_address);
        }

        bool IsValid()
        {
            return _address != 0;
        }
    };

    size_t GetSizeOfImage();
    uintptr_t GetModuleBase();

	Address FindPattern(const char* signature);
}