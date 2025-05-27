#pragma once

#include <iosfwd>
#include <string>
#include "object.h"


class StringObject : public Object
{
public:
    std::string text;


    StringObject(std::string txt): text(std::move(txt)){}
    bool operator==(const Object& obj) const override;

    [[nodiscard]] Object* clone() const override { return new StringObject(*this); }
    std::ostream& print(std::ostream& os) const override;
    void multiply(int) override {
        // no-op or error?
    }
};

