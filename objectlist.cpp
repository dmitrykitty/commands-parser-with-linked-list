#include <memory>
#include <stdexcept>
#include <utility>  // std::exchange()
#include "objectlist.h"
#include "object.h"


ObjectList::ObjectList() = default;

ObjectList::~ObjectList() = default;

bool ObjectList::contains(const value_type& obj) {
    for (const auto& o: *this)
        if (o == obj)
            return true;
    return false;
}

void ObjectList::push_front(value_type* newObject) {
    if (contains(*newObject))
        return;

    auto currentHead = head;
    head = new Node(newObject);
    head->next = currentHead;
    ++size_;
}

void ObjectList::pop_front() {
    if (!head)
        return;

    auto newHead = head->next;
    delete head;
    head = newHead;
    --size_;
}