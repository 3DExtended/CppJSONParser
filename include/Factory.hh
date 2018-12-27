#pragma once
#include "Base.hh"
#include <string>
#include <map>

namespace CJP
{
    class Creator;

    class Factory
    {
    public:
        static Base* create(const std::string& classname);;

        static void registerit(const std::string& classname, Creator* creator);;

    private:
        static std::map<std::string, Creator*>& get_table() {
            static std::map<std::string, Creator*> table;
            return table;
        };
    };
}