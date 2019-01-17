#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <type_traits>
#include <functional>
#include <algorithm>

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

#define assert(expr) \
    if (!(expr)){ \
        std::cerr << #expr << std::endl; \
        exit(1); \
    }

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

        typINVALID = 0xFFFFFFFF
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
        void registerStructure(std::string structName) {}

        template <class StructT, class DataT, class... Rest>
        void registerStructure(std::string structName, std::string fieldName, DataT StructT::*member, Rest... rest)
        {
            static_assert(std::is_base_of<Base, StructT>::value, "type parameter of this class must derive from Base");
            registerStruct(structName);
            int id = registerIDWithOffset(structName + ">" + fieldName, customoffsetof(StructT, member));
            datatypeMapping[id] = getDatatype(member);
            registerStructure(structName, rest...);
        }

        template <class StructT, class DataT, class... Rest>
        void registerStructure(std::string structName, SpecialFields field, DataT StructT::*member, Rest... rest)
        {
            static_assert(std::is_base_of<Base, StructT>::value, "type parameter of this class must derive from Base");
            registerStruct(structName);
            int id = registerIDWithOffset(structName + ">SpecialFields>" + std::to_string(field), customoffsetof(StructT, member));
            datatypeMapping[id] = Datatypes::typeStdString;
            registerStructure(structName, rest...);
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

        /*void add(std::string name)
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
            auto test = datatypeCreation[Datatypes::typeStdString]("Huhu");
            //auto test = datatypeCreation[Datatypes::typeInt]("5015");
            *address = (addressType)test;
        }*/

        void parse(const std::string &path)
        {
            std::string fileContent = readFile(path);

            int openBrac = 0;
            int closeBrac = 0;
            //check if occurences of "{" matches "}":
            for (int i = 0; i < fileContent.size(); i++)
            {
                if (fileContent.at(i) == '{')
                    openBrac++;
                else if (fileContent.at(i) == '}')
                    closeBrac++;
            }
            if (closeBrac != openBrac)
            {
                //TODO fail
                assert(false);
            }

            std::vector<Base*> result;
            std::vector<std::string> resultObjects;
            std::vector<ParseStatus> status;
            status.push_back(SearchIdentifier);

            Base* tempStruct = nullptr;
            std::string temp = "";
            std::string currentStructureName = "";
            for (int i = 0; i < fileContent.size(); i++)
            {
                if (fileContent.at(i) == ':')
                {
                    if (status.back() == SearchIdentifier) {
                        if (idMapping.find(temp) != idMapping.end())
                        {
                            tempStruct = factory.create(temp);
                            currentStructureName = temp;

                            // progress in fileContent
                            temp = "";
                            assert(fileContent.at(i + 1) == '{');
                            i += 1;
                            continue;
                        }
                        else {
                            // check whether we have an Base in result where we can append this new information to.
                            if (tempStruct != nullptr) {
                                if (idMapping.find(currentStructureName + ">" + temp) != idMapping.end())
                                {
                                    int id = idMapping[currentStructureName + ">" + temp];
                                    long offset = offsetMapping[id];
                                    Datatypes datatype = datatypeMapping[id];
                                    i++;
                                    std::string datatypeValue = "";
                                    //read from i forward. CHeck for " and , and {
                                    for (; i < fileContent.size(); i++)
                                    {
                                        if (fileContent.at(i) == '"') {
                                            //ignore since we consider everything to be a string after a member identifier
                                            continue;
                                        }
                                        else if (fileContent.at(i) == ',' || fileContent.at(i) == '}') {
                                            // currently looking at comma behind datatypeValue. call continue after you used datatypeValue
                                            break;
                                        }
                                        else if (fileContent.at(i) == '{') {
                                            //TODO not implemented yet. you are trying to create a nested object which is currently not implemented
                                        }
                                        else {
                                            datatypeValue += fileContent.at(i);
                                        }
                                    }

                                    std::function<addressType(const std::string &)> creationFunction = datatypeCreation[datatype];
                                    
                                    // Pointer magic to replace the value behind the pointer to the member of the datastructure
                                    addressType *address = (addressType *)((addressType)tempStruct +offset);
                                    auto test = creationFunction(datatypeValue);
                                    *address = (addressType)test;

                                    temp = "";

                                    if (fileContent.at(i) == '}') {
                                        // close object
                                        result.push_back(tempStruct);
                                        tempStruct = nullptr;
                                        resultObjects.push_back(currentStructureName);
                                        currentStructureName = "";

                                        if (fileContent.size() > i+1 && fileContent.at(i + 1) == ',')
                                            i++;

                                    }
                                    
                                    continue;
                                }
                            }
                        }

                    }
                    else {
                        //TODO handle error.
                        // You were looking for something else than an identifier but found one..
                    }
                }
                else if (fileContent.at(i) == '}' && status.back() == SearchIdentifier) {
                    if (tempStruct != nullptr) {
                        result.push_back(tempStruct);
                        tempStruct = nullptr;
                        resultObjects.push_back(currentStructureName);
                        currentStructureName = "";
                    }
                    else {
                        //TODO warning: you closed a block without opening it with an identifier (whats that's supposed to mean)
                    }
                }
                temp += fileContent.at(i);
            }

            lastResults = result;
        }

        template <class T>
        std::vector<T*> getFromTopLevel() {
            std::vector<T*> res;
            for (int i = 0; i < lastResults.size(); i++) {
                if (typeid(T).hash_code() == typeid(*(lastResults[i])).hash_code()) {
                    res.push_back(reinterpret_cast<T*>( lastResults[i]));
                }
            }
            return res;
        }

    private:
        enum ParseStatus {
            SearchIdentifier,

        };

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

        std::string readFile(const std::string &path)
        {
            std::string fileContent = "";
            std::ifstream input(path);
            for (std::string line; getline(input, line);)
            {
                line.erase(remove(line.begin(), line.end(), ' '), line.end());
                line.erase(remove(line.begin(), line.end(), '\0'), line.end());
                line.erase(remove(line.begin(), line.end(), '\n'), line.end());
                line.erase(remove(line.begin(), line.end(), '\r'), line.end());
                fileContent += line;
            }
            return fileContent;
        }

        std::vector<Base*> lastResults;
    };
} // namespace CJP
