#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <string>
#include <iostream>

using namespace std;

std::uint32_t getPID(const wstring& processName) {
std:uint32_t pid = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        return 0;
    }
    PROCESSENTRY32 procEntry{};
    procEntry.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnap, &procEntry)) {
        do {
            if (wstring(procEntry.szExeFile).compare(processName) == 0) {
                pid = procEntry.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnap, &procEntry));
    }
    CloseHandle(hSnap);

    return pid;
}



// waits for a specific number of modules to be loaded so we're safe to use our custom one
int WaitForProcessToBeActive(HANDLE pHandle, DWORD pID) {
    uintptr_t fezModule = 0;

    int waitTime = 0;
    const int maxWaitTime = 1000;

    while (fezModule == 0 && waitTime < maxWaitTime) {
        int modulesCount = 0;

        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pID);
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry)) {
            do {
                modulesCount++;
            } while (Module32Next(hSnap, &modEntry));
        }
        CloseHandle(hSnap);

        Sleep(10);
        waitTime++;

        if (modulesCount >= 100)break;
    }
    //cout << waitTime << endl;

    if (waitTime == maxWaitTime) {
        return 0;
    }
    return 1;
}


HANDLE CreateGameProcess() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // launch the game
    auto result = CreateProcess(L"FEZ.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    if (!result) {
        cout << "Failed to create game's process. Make sure the launcher and the game's executable are in the same directory." << endl;
        return NULL;
    }
    CloseHandle(pi.hThread);

    if (!WaitForProcessToBeActive(pi.hProcess, pi.dwProcessId)) {
        cout << "Failed to detect the readiness state of the game. Closing..." << endl;
        return NULL;
    }

    return pi.hProcess;
}


bool InjectFEZUG(HANDLE hFEZProc) {
    // allocating memory in the process for the dll path
    void* fezugDllPathMemory = VirtualAllocEx(hFEZProc, 0x00, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (fezugDllPathMemory == nullptr) {
        cout << "Injection didn't succeed (allocation failed). Closing..." << endl;
        return false;
    }

    // write dll path to allocated memory
    constexpr const char* dllPath = "fezug.dll\0";
    WriteProcessMemory(hFEZProc, fezugDllPathMemory, dllPath, strlen(dllPath), 0);

    // run the library
    LPVOID LibAddr = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
    HANDLE hFEZThread = CreateRemoteThread(hFEZProc, nullptr, 0, (LPTHREAD_START_ROUTINE)LibAddr, fezugDllPathMemory, 0, nullptr);

    if (hFEZThread == nullptr || hFEZThread == INVALID_HANDLE_VALUE) {
        cout << "Injection didn't succeed (failed to run the library, code" << GetLastError() << "). Closing...";
        return false;
    }
    else {
        CloseHandle(hFEZThread);
    }

    cout << "Injection ended successfully! Have fun!" << endl;

    return true;
}

void DebugLoop(HANDLE hFEZProc) {

    while (WaitForSingleObject(hFEZProc, 0) == WAIT_TIMEOUT) {
        Sleep(10);
    }

    std::cout << "Game process lost. Exiting..." << std::endl;
}


int main() {

    cout << "FEZUG Launcher by Krzyhau" << endl << endl;
    cout << "Launching the game..." << endl;

    // prepare for FEZ.exe launch
    HANDLE hFEZProc = CreateGameProcess();
    if (!hFEZProc) {
        return 1;
    }
    cout << "Created the process. Injecting..." << endl;

    if (!InjectFEZUG(hFEZProc)) return 1;

    //DebugLoop(hFEZProc);

    return 0;
}