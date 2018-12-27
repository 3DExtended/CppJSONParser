#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <type_traits>
#include <functional>

#include "CreatorImpl.hh"
#include "FactoryPattern.hh"

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
        //idMap["SpecialFields>Name"] = 0;
    }

    // recursion end
    void registerType(std::string structName) {}

    template <class StructT, class DataT, class... Rest>
    void registerType(std::string structName, std::string fieldName, DataT StructT::*member, Rest... rest)
    {
        static_assert(std::is_base_of<Base, StructT>::value, "type parameter of this class must derive from Base");
        // TODO
        registerType(structName, rest...);
    }

    template <class StructT, class DataT, class... Rest>
    void registerType(std::string structName, SpecialFields field, DataT StructT::*member, Rest... rest)
    {
        static_assert(std::is_base_of<Base, StructT>::value, "type parameter of this class must derive from Base");
        //TODO
        registerType(structName, rest...);
    }

  private:
};
} // namespace CJP
