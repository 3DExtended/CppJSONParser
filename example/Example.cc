#pragma once
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <CppJSONParser.hh>

class Object1 : public CJP::Base
{
    REGISTER(Object1);

  public:
    Object1(){};
    virtual ~Object1(){};
    virtual void foo() { std::cout << "1"; };
    std::string name;
    std::string objPath;
    std::string material;
};
REGISTERIMPL(Object1);

class Object2 : public CJP::Base
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
const CJP::CreatorImpl<Object2> Object2::creator("Object2");

int main(int argc, char const *argv[])
{
    Base *p;
    p = Factory::create("Object2");
    std::cout << p->name;
    /*auto parser = new CJP::JSONParser();
    parser->registerType(
        "Object",
        CJP::SpecialFields::Name, &Object::name,
        "objPath", &Object::objPath,
        "material", &Object::material
    );

    parser->add("Object");
    //parser->register("objects", std::vector<Object>);//*/
    return 0;
}
