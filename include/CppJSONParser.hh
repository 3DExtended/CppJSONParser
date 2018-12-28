#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <type_traits>
#include <functional>

#include "CreatorImpl.hh"
#include "FactoryPattern.hh"

#define customoffsetof(s, m) (reinterpret_cast<long>(&(reinterpret_cast<s const volatile *>(0)->*m)))

namespace CJP
{

enum SpecialFields
{
    Name = 1,
};

Factory factory;

class JSONParser
{
  public:
    JSONParser()
    {
        idMapping["SpecialFields>Name"] = 0;
    }

    // recursion end
    void registerType(std::string structName) {}

    template <class StructT, class DataT, class... Rest>
    void registerType(std::string structName, std::string fieldName, DataT StructT::*member, Rest... rest)
    {
        static_assert(std::is_base_of<Base, StructT>::value, "type parameter of this class must derive from Base");
        registerStruct(structName);
        registerType(structName + ">" + fieldName, customoffsetof(StructT, member));
        registerType(structName, rest...);
    }

    template <class StructT, class DataT, class... Rest>
    void registerType(std::string structName, SpecialFields field, DataT StructT::*member, Rest... rest)
    {
        static_assert(std::is_base_of<Base, StructT>::value, "type parameter of this class must derive from Base");
        registerStruct(structName);
        registerType(structName + ">SpecialFields>" + std::to_string(field), customoffsetof(StructT, member));
        registerType(structName, rest...);
    }

    void registerType(std::string typeName, long offset)
    {
        idMapping[typeName] = (int)idMapping.size();
        offsetMapping[idMapping[typeName]] = offset;
    }

    void registerStruct(std::string structName)
    {
        if (idMapping.find(structName) == idMapping.end())
        {
            idMapping[structName] = (int)idMapping.size();
        }
    }

    void add(std::string name) {
        Base *p;
        p = Factory::create("Object");

        // TODO figure out!!!

        // Pointer magic to replace a pointer in the created class
        long long *address = (long long*)((long long)p + offsetMapping[idMapping["Object>SpecialFields>1"]]);
        
        // Overwriting the std::string * 
        auto test = new std::string("HUHU");
        *address = (long long)test;

    }

  private:
    std::map<std::string, int> idMapping;
    std::map<int, long> offsetMapping;
};
} // namespace CJP
