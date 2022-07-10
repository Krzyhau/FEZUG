#include "Memory.hpp"


DWORD64 Memory::SetProtection(uintptr_t addr, int size, DWORD protection)
{
    DWORD oldProtection;
    VirtualProtect((LPVOID)addr, size, protection, &oldProtection);

    return oldProtection;
}

uintptr_t Memory::HookFunction(uintptr_t hookAt, uintptr_t newFunc, unsigned int size) {
    if (size < 5) size = 5;
    auto oldProtection = Memory::SetProtection(hookAt, size, PAGE_EXECUTE_READWRITE);

    Memory::Write<uint8_t>(hookAt, 0xE9); // JMP opcode    
    uintptr_t newOffset = newFunc - hookAt - 5; // -5 since the jump is relative to the next instruction
    Memory::Write<uintptr_t>(hookAt + 1, newOffset);

    // nop extra bytes to avoid corrupting the overwritten opcode
    for (unsigned int i = 5; i < size; i++) {
        Memory::Write<uint8_t>(hookAt + i, 0x90);
    }

    Memory::SetProtection(hookAt, size, oldProtection);

    return hookAt + 5;
}

uintptr_t Memory::HookVTable(void* pVTable, void* fnHookFunc, int nOffset)
{
    uintptr_t ptrVtable = *((uintptr_t*)pVTable); // Pointer to our chosen vtable
    uintptr_t ptrFunction = ptrVtable + sizeof(uintptr_t) * nOffset; // The offset to the function (remember it's a zero indexed array with a size of four bytes)
    uintptr_t ptrOriginal = *((uintptr_t*)ptrFunction); // Save original address

    // Edit the memory protection so we can modify it
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery((LPCVOID)ptrFunction, &mbi, sizeof(mbi));
    VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect);

    // Overwrite the old function with our new one
    *((uintptr_t*)ptrFunction) = (uintptr_t)fnHookFunc;

    // Restore the protection
    VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect);

    // Return the original function address incase we want to call it
    return ptrOriginal;
}


MODULEENTRY32 Memory::GetModuleBaseEntry(std::string modName) {
    MODULEENTRY32 modEntry;
    modEntry.dwSize = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetCurrentProcessId());
    if (hSnap != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 modEntrySearch;
        modEntrySearch.dwSize = sizeof(modEntrySearch);
        if (Module32First(hSnap, &modEntrySearch)) {
            do {
                std::string name(modEntrySearch.szModule);
                if (name.compare(modName) == 0) {
                    modEntry = modEntrySearch;
                    break;
                }
            } while (Module32Next(hSnap, &modEntrySearch));
        }
    }
    CloseHandle(hSnap);
    return modEntry;
}

uintptr_t Memory::GetModuleBaseAddress(std::string modName) {
    MODULEENTRY32 modEntry = Memory::GetModuleBaseEntry(modName);
    if (modEntry.dwSize == 0) {
        return 0;
    }
    return (uintptr_t)modEntry.modBaseAddr;
}


vector<MEMORY_BASIC_INFORMATION> Memory::GetMemoryPages(bool all) {
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);

    uintptr_t min = 0x10000;
    uintptr_t max = 0x7FFEFFFF;

    uintptr_t addr = min;

    vector<MEMORY_BASIC_INFORMATION> pages;

    do
    {
        MEMORY_BASIC_INFORMATION meminfo;
        VirtualQuery((LPCVOID)addr, &meminfo, sizeof(meminfo));

        addr += meminfo.RegionSize;

        // don't care about reserved/free pages
        if (meminfo.State != MEM_COMMIT) continue;

        // probably don't care about guarded pages
        if (!all && (meminfo.Protect & PAGE_GUARD) != 0) continue;

        // probably don't care about image/file maps
        if (!all && meminfo.Type != MEM_PRIVATE) continue;

        pages.push_back(meminfo);

    } while (addr < max);

    return pages;
}


void* Memory::HookVTableFunction(void* pVTable, void* fnHookFunc, int nOffset) {
    intptr_t ptrVtable = *((intptr_t*)pVTable); // Pointer to our chosen vtable
    intptr_t ptrFunction = ptrVtable + sizeof(intptr_t) * nOffset; // The offset to the function (remember it's a zero indexed array with a size of four bytes)
    intptr_t ptrOriginal = *((intptr_t*)ptrFunction); // Save original address

    // Edit the memory protection so we can modify it
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery((LPCVOID)ptrFunction, &mbi, sizeof(mbi));
    VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect);

    // Overwrite the old function with our new one
    *((intptr_t*)ptrFunction) = (intptr_t)fnHookFunc;

    // Restore the protection
    VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect);

    // Return the original function address incase we want to call it
    return (void*)ptrOriginal;
}



#define INRANGE(x, a, b) (x >= a && x <= b)
#define getBits(x) (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA) : (INRANGE(x, '0', '9') ? x - '0' : 0))
#define getByte(x) (getBits(x[0]) << 4 | getBits(x[1]))

uintptr_t Memory::Scan(const uintptr_t start, const uintptr_t end, const char* target) {
    const char* pattern = target;
    uintptr_t result = 0;

    for (auto position = start; position < end; ++position) {
        if (!*pattern)
            return result;

        auto match = *reinterpret_cast<const uint8_t*>(pattern);
        auto byte = *reinterpret_cast<const uint8_t*>(position);

        if (match == '\?' || byte == getByte(pattern)) {
            if (!result)
                result = position;

            if (!pattern[2])
                return result;

            pattern += (match != '\?') ? 3 : 2;
        }
        else {
            pattern = target;
            result = 0;
        }
    }
    return 0;
}

uintptr_t Memory::ScanInMemoryPages(const char* target, bool all) {
    auto pages = GetMemoryPages(all);

    for (auto const& page : pages) {

        uintptr_t start = (uintptr_t)page.BaseAddress;
        uintptr_t end = start + page.RegionSize;

        auto result = Scan(start, end, target);
        if (result != 0) return result;
    }

    return 0;
}