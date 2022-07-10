#pragma once

#include "Utils/Common.hpp"

class Feature {
public:
    Feature();
    virtual ~Feature() = default;

    virtual void Draw();
    virtual void Update();

    static std::vector<Feature*>& GetList();
};

