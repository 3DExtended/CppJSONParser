#include "Creator.hh"
#include "Factory.hh"

using namespace CJP;
inline Creator::Creator(const std::string &classname)
{
    Factory::registerit(classname, this);
}

inline Base *Factory::create(const std::string &classname)
{
    std::map<std::string, Creator *>::iterator i;
    i = get_table().find(classname);

    if (i != get_table().end())
        return i->second->create();
    else
        return (Base *)NULL;
}

inline void Factory::registerit(const std::string &classname, Creator *creator)
{
    get_table()[classname] = creator;
}

#define REGISTER(classname) \
  private:                  \
    static const CreatorImpl<classname> creator;

#define REGISTERIMPL(classname) \
    const CJP::CreatorImpl<classname> classname::creator(#classname);

#define REGISTERTYPE(classname, ...)
