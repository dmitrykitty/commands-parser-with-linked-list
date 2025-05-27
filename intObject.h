//
// Created by Lenovo on 27/05/2025.
//
#pragma once
#include "object.h"


class IntObject: public Object {
public:
    int num;


    IntObject(int n): num(n){}
    bool operator==(const Object& obj) const override;

    [[nodiscard]] Object* clone() const override { return new IntObject(*this); }
    std::ostream& print(std::ostream& os) const override;
    void multiply(int m) override { num *= m; }
};


