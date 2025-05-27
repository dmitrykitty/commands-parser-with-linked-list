#pragma once

#include <iosfwd>
#include <string>


class Object
{
public:
    virtual ~Object() = default;
    virtual bool operator==(const Object& obj) const = 0;
    virtual Object* clone() const = 0;
};
