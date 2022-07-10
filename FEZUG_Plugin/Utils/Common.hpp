#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <list>
#include <math.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;

namespace Utils {
    string ssprintf(const char* str, ...);
    string StringReplace(std::string str, std::string from, std::string to);
}