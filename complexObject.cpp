//
// Created by Lenovo on 27/05/2025.
//

#include "complexObject.h"

bool ComplexObject::operator==(const Object& obj) const {
    if (auto other = dynamic_cast<const ComplexObject *>(&obj))
        return complex == other->complex;
    return false;
}

std::ostream& ComplexObject::print(std::ostream& os) const {
    int re = complex.real();
    int im = complex.imag();
    os << "ComplexObject(" << re;
    if (im != 0) {
        if (im > 0)
            os << '+'; // znak plus tylko gdy urojona ≥ 0
        os << im << "i";
    }
    os << ")";
    return os;
}
