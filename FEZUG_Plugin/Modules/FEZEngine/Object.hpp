#pragma once

#include "Utils/Common.hpp"

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


// wrappers for functions from game object's VTables
#define FEZOBJ_VTABLE_FUNC_0(offset, returnType, name) \
    returnType name##() { \
        (returnType(__thiscall*)(void*))(*((*(void***)thisPtr) + offset))(thisPtr); \
    }
#define FEZOBJ_VTABLE_FUNC_1(offset, returnType, name, param1) \
    returnType name##(param1 a) { \
        ((returnType(__thiscall*)(void*, param1))(*((*(void***)thisPtr) + offset)))(thisPtr, a); \
    }
#define FEZOBJ_VTABLE_FUNC_2(offset, returnType, name, param1, param2) \
    returnType name##(param1 a, param2 b) { \
        ((returnType(__thiscall*)(void*, param1, param2))(*((*(void***)thisPtr) + offset)))(thisPtr, a, b); \
    }
#define FEZOBJ_VTABLE_FUNC_3(offset, returnType, name, param1, param2, param3) \
    returnType name##(param1 a, param2 b, param3 c) { \
        ((returnType(__thiscall*)(void*, param1, param2, param3))(*((*(void***)thisPtr) + offset)))(thisPtr, a, b, c); \
    }

namespace FEZEngine {

    class Object {
    protected:
        void* thisPtr;
    public:
        FEZOBJ_PROPERTY_GETTER(int, VTable, 0x00)
    public:
        void* ThisPtr() { return thisPtr; }
        bool IsValid() { return this != nullptr && thisPtr != nullptr && thisPtr != (void*)0xFFFFFFFF; }

        Object(void* ptr) { thisPtr = ptr; }
    };

};
