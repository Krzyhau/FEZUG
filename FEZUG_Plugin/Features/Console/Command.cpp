#include "Command.hpp"

#include "Console.hpp"

std::vector<Command*>& Command::GetList() {
    static std::vector<Command*> list;
    return list;
}


Command::Command(const char* name, string helpStr, _CommandCallback callback, int minParams, int maxParams) 
    : name(name)
    , helpStr(helpStr)
    , callback(callback)
    , minParams(minParams)
    , maxParams(maxParams)
{
    Command::GetList().push_back(this);
}

Command::Command(const char* name, string helpStr, _CommandCallback callback, int params)
    : Command(name, helpStr, callback, params, params)
{
}

Command::Command(const char* name, string helpStr, _CommandCallback callback)
    : Command(name, helpStr, callback, 0) {
}

void Command::Execute(vector<string> params) {
    if (callback != nullptr) {
        callback(params);
    }
}

bool Command::IsParamCountValid(int count) {
    return (count >= minParams && count <= maxParams) || maxParams<0;
}