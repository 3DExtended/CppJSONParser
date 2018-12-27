#pragma once
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <CppJSONParser.hh>

int main(int argc, char const *argv[])
{

    class Object : public CJP::Base{
    public:
        int a = 55;

        Object() {};
        virtual ~Object() {};

        std::string name;
        std::string objPath;
        std::string material;

    };

    auto parser = new CJP::JSONParser();
    parser->registerType(
        "Object", 
        CJP::SpecialFields::Name, &Object::name,
        "objPath", &Object::objPath,
        "material", &Object::material
    );

    parser->add("Object");
    //parser->register("objects", std::vector<Object>);
    return 0;
}