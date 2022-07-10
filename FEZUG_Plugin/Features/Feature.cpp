#include "Feature.hpp"

std::vector<Feature*>& Feature::GetList() {
    static std::vector<Feature*> list;
    return list;
}


Feature::Feature() {
    Feature::GetList().push_back(this);
}

void Feature::Update() {}

void Feature::Draw() {}