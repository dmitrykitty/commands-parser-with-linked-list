#pragma once
#include "object.h"

class DoubleObject: public Object {
public:
    double doubleNum;


    DoubleObject(double dn): doubleNum(dn){}
    bool operator==(const Object& obj) const override;

    [[nodiscard]] Object* clone() const override { return new DoubleObject(*this); }
    std::ostream& print(std::ostream& os) const override;
    void multiply(int m) override { doubleNum *= m; }
};




