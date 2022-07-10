#pragma once

#include "Utils/Common.hpp"

class Command {
private:
    string name, helpStr;
    int minParams;
    int maxParams;

    using _CommandCallback = void(*)(vector<string> params);
    _CommandCallback callback = nullptr;
public:
    Command(const char* name, string helpStr, _CommandCallback callback, int minParams, int maxParams);
    Command(const char* name, string helpStr, _CommandCallback callback, int params);
    Command(const char* name, string helpStr, _CommandCallback callback);

    void Execute(vector<string> params);
    bool IsParamCountValid(int count);
    inline string GetName() const { return name; }
    inline string GetHelp() const { return helpStr; }

    static std::vector<Command*>& GetList();
};


#define CREATE_COMMAND_SIMPLE(name, helpStr)                             \
    CREATE_COMMAND_PARAMS(name, helpStr, 0)
#define CREATE_COMMAND_PARAMS(name, helpStr, params)                     \
    CREATE_COMMAND_PARAMS_OPTIONALS(name, helpStr, params, params)
#define CREATE_COMMAND_PARAMS_OPTIONALS(name, helpStr, minParams, maxParams)       \
    void name##_callback(vector<string> params);                    \
    Command name = Command(#name, helpStr, name##_callback, minParams, maxParams);        \
    void name##_callback(vector<string> params)
