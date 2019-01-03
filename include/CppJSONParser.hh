#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <type_traits>
#include <functional>

#include "CreatorImpl.hh"
#include "FactoryPattern.hh"

#define customoffsetof(s, m) (reinterpret_cast<long>(&(reinterpret_cast<s const volatile *>(0)->*m)))

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define addressType uint64_t
#else
#define addressType uint32_t
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define addressType uint64_t
#else
#define addressType uint32_t
#endif
#endif

namespace CJP
{

enum SpecialFields
{
    Name = 1,
};

enum Datatypes
{
    typeStdString = 1 << 0,
    typeInt = 1 << 1,
    typeDouble = 1 << 2,
    typeFloat = 1 << 3,

    typINVALID = 0xFFFFFFFFFFFF
};

Factory factory;

class JSONParser
{
  public:
    JSONParser()
    {
        idMapping["SpecialFields>Name"] = 0;

        // read string and produce datatype

        // std::string reading
        std::function<addressType(const std::string &)> lambdaStr = [](const std::string &s) {
            auto test = new std::string(s.c_str());
            return (addressType)test;
        };
        datatypeCreation[Datatypes::typeStdString] = lambdaStr;

        // int reading
        std::function<addressType(const std::string &)> lambdaInt = [](const std::string &s) {
            int temp = std::stoi(s);
            auto test = new int(temp);
            return (addressType)test;
        };
        datatypeCreation[Datatypes::typeInt] = lambdaInt;

        // double reading
        std::function<addressType(const std::string &)> lambdaDouble = [](const std::string &s) {
            double temp = std::stod(s);
            auto test = new double(temp);
            return (addressType)test;
        };
        datatypeCreation[Datatypes::typeDouble] = lambdaDouble;

        // float reading
        std::function<addressType(const std::string &)> lambdaFloat = [](const std::string &s) {
            float temp = std::stof(s);
            auto test = new float(temp);
            return (addressType)test;
        };
        datatypeCreation[Datatypes::typeFloat] = lambdaFloat;
    }

    // recursion end
    void registerType(std::string structName) {}

    template <class StructT, class DataT, class... Rest>
    void registerType(std::string structName, std::string fieldName, DataT StructT::*member, Rest... rest)
    {
        static_assert(std::is_base_of<Base, StructT>::value, "type parameter of this class must derive from Base");
        registerStruct(structName);
        int id = registerIDWithOffset(structName + ">" + fieldName, customoffsetof(StructT, member));
        datatypeMapping[id] = getDatatype(member);
        registerType(structName, rest...);
    }

    template <class StructT, class DataT, class... Rest>
    void registerType(std::string structName, SpecialFields field, DataT StructT::*member, Rest... rest)
    {
        static_assert(std::is_base_of<Base, StructT>::value, "type parameter of this class must derive from Base");
        registerStruct(structName);
        int id = registerIDWithOffset(structName + ">SpecialFields>" + std::to_string(field), customoffsetof(StructT, member));
        datatypeMapping[id] = Datatypes::typeStdString;
        registerType(structName, rest...);
    }

    int registerIDWithOffset(std::string typeName, long offset)
    {
        int id = (int)idMapping.size();
        idMapping[typeName] = id;
        offsetMapping[id] = offset;
        return id;
    }

    void registerStruct(std::string structName)
    {
        if (idMapping.find(structName) == idMapping.end())
        {
            idMapping[structName] = (int)idMapping.size();
        }
    }

    void add(std::string name)
    {
        Base *p;
        p = Factory::create(name);

        // TODO figure out!!!

        // Pointer magic to replace a pointer in the created class
        //addressType *address = (addressType *)((addressType)p + offsetMapping[idMapping["Object>SpecialFields>1"]]);
        addressType *address = (addressType *)((addressType)p + offsetMapping[idMapping["Object>integerValue"]]);

        //TODO std::functions which return a long long value which corresponds to the pointer to an object of form dataT.

        // Overwriting the std::string *
        //auto test = new std::string("HUHU");
        //*address = (long long)test;

        // Overwriting the std::string *
        auto test = datatypeCreation[Datatypes::typeInt]("5015");
        *address = (addressType)test;
    }

  private:
    std::map<std::string, int> idMapping;
    std::map<int, long> offsetMapping;
    std::map<int, Datatypes> datatypeMapping;
    std::map<Datatypes, std::function<addressType(const std::string &)>> datatypeCreation;

    template <class StructT, class DataT>
    Datatypes getDatatype(DataT StructT::*member)
    {
        if (std::is_same<std::string *, DataT>::value == true)
        {
            return Datatypes::typeStdString;
        }
        else if (std::is_same<int *, DataT>::value == true)
        {
            return Datatypes::typeInt;
        }
        else if (std::is_same<double *, DataT>::value == true)
        {
            return Datatypes::typeDouble;
        }
        else if (std::is_same<float *, DataT>::value == true)
        {
            return Datatypes::typeFloat;
        }

        return Datatypes::typINVALID;
    }
};
} // namespace CJP
