#include "intObject.h"

bool IntObject::operator==(const Object& obj) const {
    if (const auto intObj = dynamic_cast<const IntObject*>(&obj))
        return intObj->num == num;
    return false;
}

std::ostream& IntObject::print(std::ostream& os) const {
    os << "IntObject(\"" << num << "\")";
    return os;
}