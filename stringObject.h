#pragma once

#include <iosfwd>
#include <string>
#include "object.h"


class StringObject : public Object
{
    std::string text;

public:
    StringObject(std::string txt): text(std::move(txt)){}
    bool operator==(const Object& obj) const override;

    [[nodiscard]] Object* clone() const override { return new StringObject(*this); }
};

