#pragma once
namespace CJP
{
    class Base {
    public:
        Base() = default;
        virtual ~Base() = default;
        virtual void foo() = 0;
    };
}