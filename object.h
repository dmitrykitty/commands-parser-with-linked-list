#pragma once

#include <iosfwd>
#include <string>
#include <iostream>

class Object
{
public:
    virtual ~Object() = default;
    virtual bool operator==(const Object& obj) const = 0;
    [[nodiscard]] virtual Object* clone() const = 0;

    virtual std::ostream& print(std::ostream& os) const = 0;
    virtual void multiply(int m) = 0;
};

inline std::ostream& operator<<(std::ostream& os, Object const& obj) {
    return obj.print(os);
}