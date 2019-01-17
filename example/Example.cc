#pragma once
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <CppJSONParser.hh>

class Object : public CJP::Base
{
    REGISTER(Object);

public:
    Object() {};
    virtual ~Object() {};
    virtual void foo() { std::cout << "1"; };
    std::string *name;
    std::string *objPath;
    std::string *material;
    int *integerValue;
};
REGISTERIMPL(Object);

/*class Object2 : public CJP::Base
{
  private:
    static const CreatorImpl<Object2> creator;

  public:
    Object2(){};
    virtual ~Object2(){};
    virtual void foo() { std::cout << "2"; };

    std::string name = "Huhu";
    std::string objPath;
    std::string material;
};
const CJP::CreatorImpl<Object2> Object2::creator("Object2"); //*/

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
