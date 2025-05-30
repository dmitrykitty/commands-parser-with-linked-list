#include "stringObject.h"
using namespace std;

bool StringObject::operator==(const Object& obj) const {
    if (const auto strObj = dynamic_cast<const StringObject *>(&obj))
        return strObj->text == text;
    return false;
}

std::ostream& StringObject::print(std::ostream& os) const {
    os << "StringObject(\"" << text << "\")";
    return os;
}

void StringObject::multiply(int m) {
    std::string orig = text;
    text.clear();
    for (int i = 0; i < m; ++i) text += orig;
}
