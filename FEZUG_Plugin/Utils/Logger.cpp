#include "Logger.hpp"

#include "Utils/Common.hpp"

#include <chrono>

bool Logger::Init()
{
    if(!AllocConsole())return false;
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();
    SetConsoleOutputCP(1250);

    return true;
}

void Logger::Log(string component, string message, ...)
{
    using namespace std::chrono;
    auto timepoint = system_clock::now();
    auto coarse = system_clock::to_time_t(timepoint);
    auto fine = time_point_cast<milliseconds>(timepoint);
    tm ltm;
    localtime_s(&ltm, &coarse);
    char timeStr[sizeof "00:00:00"];
    std::strftime(timeStr, sizeof timeStr, "%T", &ltm);
    int timeMs = (int)(fine.time_since_epoch().count() % 1000);

    va_list ap1, ap2;
    va_start(ap1, message);
    va_copy(ap2, ap1);
    size_t sz = vsnprintf(NULL, 0, message.c_str(), ap1) + 1;
    va_end(ap1);
    char* buf = (char*)malloc(sz);
    vsnprintf(buf, sz, message.c_str(), ap2);
    va_end(ap2);

    string outMsg = Utils::ssprintf("(%s.%03lu) [%s] %s\n", timeStr, timeMs, component.c_str(), buf);

    free(buf);

    std::cout << outMsg;
}
