#pragma once
#include <string>
#include "Base.hh"

namespace CJP {
class Creator
{
public:
    Creator(const std::string& classname);;
    virtual Base* create() = 0;
};
}