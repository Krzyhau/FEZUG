#pragma once

#include "Utils/Common.hpp"
#include "Modules/FEZEngine.hpp"

// getters and setters for standard properties
#define FEZOBJ_PROPERTY_GETTER(type, name, offset) type Get##name##() { type* val = (type*)(((char*)thisPtr) + offset); return *val; }
#define FEZOBJ_PROPERTY_SETTER(type, name, offset) void Set##name##(type val) { *(type*)(((char*)thisPtr) + offset) = val; }
#define FEZOBJ_PROPERTY(type, name, offset) \
    FEZOBJ_PROPERTY_GETTER(type, name, offset) \
    FEZOBJ_PROPERTY_SETTER(type, name, offset)

// getters and setters which wrap and unwrap game objects with our custom classes
#define FEZOBJ_OBJECT_GETTER(type, name, offset) type* Get##name##() { \
    return new type(*(void**)(((char*)thisPtr) + offset)); \
}
#define FEZOBJ_OBJECT_SETTER(type, name, offset) void Set##name##(type* val) { \
    *(void**)(((char*)thisPtr + offset)) = val->ThisPtr(); \
}
#define FEZOBJ_OBJECT(type, name, offset) \
    FEZOBJ_OBJECT_GETTER(type, name, offset) \
    FEZOBJ_OBJECT_SETTER(type, name, offset)


// wrappers for functions from game
#define FEZOBJ_FUNC_0(offset, returnType, name) \
    returnType name##() { \
        ((returnType(__stdcall*)(void*))(FEZEngine::GetFuncAddr(offset)))(thisPtr); \
    }
#define FEZOBJ_FUNC_1(offset, returnType, name, t1) \
    returnType name##(t1 v1) { \
        ((returnType(__stdcall*)(void*, t1))(FEZEngine::GetFuncAddr(offset)))(thisPtr, v1); \
    }
#define FEZOBJ_FUNC_2(offset, returnType, name, t1, t2) \
    returnType name##(t1 v1, t2 v2) { \
        ((returnType(__stdcall*)(void*, param1, param2))(FEZEngine::GetFuncAddr(offset)))(thisPtr, v1, v2); \
    }
#define FEZOBJ_FUNC_3(offset, returnType, name, param1, param2, param3) \
    returnType name##(t1 v1, t2 v2, t3 v3) { \
        ((returnType(__stdcall*)(void*, t1, t2, t3))(FEZEngine::GetFuncAddr(offset)))(thisPtr, v1, v2, v3); \
    }

namespace FEZEngine {
    // definition is in FEZEngine.cpp. Have to have it here, otherwise compiler goes mad
    uintptr_t GetFuncAddr(int offset);

    class Object {
    protected:
        void* thisPtr;
    public:
        FEZOBJ_PROPERTY_GETTER(int, IdentifierOrSth, 0x00)
    public:
        void* ThisPtr() { return thisPtr; }
        bool IsValid() { return this != nullptr && thisPtr != nullptr && thisPtr != (void*)0xFFFFFFFF; }

        Object(void* ptr) { thisPtr = ptr; }
    };

};
