#pragma once

#include "Common.hpp"

namespace Memory {
    template<typename T>
    T Read(DWORD64 addr) {
        return *((T*)addr);
    }

    template<typename T>
    void Write(DWORD64 addr, T value) {
        *((T*)addr) = value;
    }

    DWORD64 SetProtection(uintptr_t addr, int size, DWORD protection);

    template<typename T>
    void Patch(DWORD64 addr, T newValue) {
        auto oldProtection = Memory::SetProtection(addr, sizeof(T), PAGE_EXECUTE_READWRITE);

        Memory::Write<T>(addr, newValue);

        Memory::SetProtection<T>(addr, oldProtection);
    }

    uintptr_t HookFunction(uintptr_t hookAt, uintptr_t newFunc, unsigned int size = 5);
    uintptr_t HookVTable(void* pVTable, void* fnHookFunc, int nOffset);
    MODULEENTRY32 GetModuleBaseEntry(std::string modName);
    uintptr_t GetModuleBaseAddress(std::string modName);
    
    vector<MEMORY_BASIC_INFORMATION> GetMemoryPages(bool all = false);

    void* HookVTableFunction(void* pVTable, void* fnHookFunc, int nOffset);

    uintptr_t Scan(const uintptr_t start, const uintptr_t end, const char* target);
    uintptr_t ScanInMemoryPages(const char* target, bool all=false);
}