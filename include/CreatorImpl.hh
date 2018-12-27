#pragma once
#include "Base.hh"
#include <string>
#include "Creator.hh"
namespace CJP
{
    template <class T>
    class CreatorImpl : public Creator
    {
    public:
        CreatorImpl(const std::string& classname) : Creator(classname) {}

        virtual Base* create() { return new T; }
    };
}