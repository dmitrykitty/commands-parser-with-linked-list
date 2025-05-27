#include "stringObject.h"
using namespace std;

bool StringObject::operator==(const Object& obj) const {
    if (const auto strObj = dynamic_cast<const StringObject*>(&obj))
        return strObj->text == text;
    return false;
}

std::ostream& StringObject::print(std::ostream& os) const {
    os << "StringObject(\"" << text << "\")";
    return os;
}