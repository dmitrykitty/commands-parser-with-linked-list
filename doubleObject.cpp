#include "doubleObject.h"
#include <iomanip>

bool DoubleObject::operator==(const Object& obj) const {
    if (const auto intObj = dynamic_cast<const DoubleObject *>(&obj))
        return intObj->doubleNum == doubleNum;
    return false;
}

std::ostream& DoubleObject::print(std::ostream& os) const {
    os << "DoubleObject(\"" << std::fixed << std::setprecision(3) << doubleNum << "\")";
    return os;
}
