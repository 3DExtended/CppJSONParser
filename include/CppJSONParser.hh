#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <type_traits>
#include <functional>


namespace CJP
{
enum SpecialFields {
    Name = 1,
};

class Base { 
public:
    int a = 11;
    Base() = default;
    virtual ~Base() = default;

    //static std::shared_ptr<Base> create() { return std::make_shared<Base>(); };
    //static std::shared_ptr<Base<T>> create() { return std::make_shared<T>(); };
};

class JSONParser
{
public:
      JSONParser() {
          idMap["SpecialFields>Name"] = 0;
      }
      
      // recursion end
      void registerType(std::string structName) {}

      template <class StructT,  class DataT, class... Rest>
      void registerType(std::string structName, std::string fieldName, DataT StructT::*member, Rest... rest) {
          static_assert(std::is_base_of<Base, StructT>::value, "type parameter of this class must derive from Base");

          auto offset = reinterpret_cast<long>(
              &(reinterpret_cast<StructT const volatile *>(0)->*member));

          if (idMap.find(structName) == idMap.end()) {
              idMap[structName] = idMap.size();
              std::cout << "Adding (" << structName << ") with ID " << idMap[structName] << std::endl;
              //CJP::Base(*f)(void) = &StructT::create;
              //auto huhu = f();
              //typeList[idMap[structName]] = f;

              auto t = new StructT();
              std::cout << t->a;
          }

          idMap[structName + ">" + fieldName] = idMap.size();
          std::cout << "Adding (" << structName + ">" + fieldName << ") with ID " << idMap[structName + ">" + fieldName] << std::endl;
          offsetMap[idMap[structName + ">" + fieldName]] = offset;

          std::cout << offset << std::endl;
          registerType(structName, rest...);
      }

      template <class StructT, class DataT, class... Rest>
      void registerType(std::string structName, SpecialFields field, DataT StructT::*member, Rest... rest) {
          static_assert(std::is_base_of<Base, StructT>::value, "type parameter of this class must derive from Base");

          auto offset = reinterpret_cast<long>(
              &(reinterpret_cast<StructT const volatile *>(0)->*member));
          
          if (idMap.find(structName) == idMap.end()) {
              idMap[structName] = idMap.size();
              std::cout << "Adding (" << structName << ") with ID " << idMap[structName] << std::endl;
              //CJP::Base*(*f)(void) = StructT::create;
              //std::shared_ptr<StructT>(*f)(void) = StructT::create;
              /*std::shared_ptr<Base<StructT>>(*f)(void) = StructT::create;
              auto huhu = f();
              std::cout << huhu->a;*/
              auto t = new StructT();
              t->name = "asdf";
              Base* asdf = t;
              std::cout << asdf->a;
              //auto huhu = (*f)(); }
              //typeList[idMap[structName]] = f;
          }

          idMap[structName + ">SpecialFields>" + std::to_string(field)] = idMap.size();
          std::cout << "Adding (" << structName + ">SpecialFields>" + std::to_string(field) << ") with ID " << idMap[structName + ">SpecialFields>" + std::to_string(field)] << std::endl;

          offsetMap[idMap[structName + ">SpecialFields>" + std::to_string(field)]] = offset;

          std::cout << offset << std::endl;

          registerType(structName, rest...);
      }

      void add(std::string type) {
          //auto temp = (*typeList[idMap[type]])();
          //list.push_back();
          std::cout << "huhu" << std::endl;
      }

private:
    std::map<std::string, int> idMap;
    std::map<int, long> offsetMap;
    //std::map<int, CJP::Base(*)(void)> typeList;
    


    std::vector<void*> list;
};
} // namespace CJP
