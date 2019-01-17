#pragma once
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <CppJSONParser.hh>

/*class Object : public CJP::Base
{
    REGISTER(Object);

public:
    Object() {};
    virtual ~Object() {};
    std::string *name;
    std::string *objPath;
    std::string *material;
    int *integerValue;
};
REGISTERIMPL(Object);//*/

DEF_TYPE(Object, 
    std::string *name;
    std::string *objPath;
    std::string *material;
    int *integerValue;
);

int main(int argc, char const *argv[])
{
    auto parser = new CJP::JSONParser();
    parser->registerStructure(
        "Object",
        "name", &Object::name,
        "objPath", &Object::objPath,
        "material", &Object::material,
        "integerValue", &Object::integerValue);

    parser->parse("./object.jsonx");
    auto temp = parser->getFromTopLevel<Object>();
    return 0;
}
