//
// Created by Lenovo on 27/05/2025.
//

#pragma once
#include <complex>
#include "object.h"


class ComplexObject: public Object {
public:
    std::complex<int> complex;


    ComplexObject(int re, int im): complex(re, im){}
    bool operator==(const Object& obj) const override;

    [[nodiscard]] Object* clone() const override { return new ComplexObject(*this); }
    std::ostream& print(std::ostream& os) const override;
    void multiply(int m) override { complex *= m; }
};
