#pragma once

#include <iosfwd>
#include <string>
#include "object.h"


class StringObject : public Object
{
    std::string text;

public:
    // TODO: ...
    bool operator==(const Object& obj) const override;

    Object* clone() const override
    {
       return new StringObject(*this);
    }
};

