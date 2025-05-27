#include <memory>
#include <stdexcept>
#include <utility>  // std::exchange()
#include "objectlist.h"
#include "object.h"


ObjectList::ObjectList(): fakeNode{&fakeNode, &fakeNode}{}

ObjectList::~ObjectList() = default;

bool ObjectList::contains(const value_type& obj) const {
    for (const auto& o: *this)
        if (o == obj)
            return true;
    return false;
}

void ObjectList::push_front(value_type* newObject) {
    if (contains(*newObject))
        return;

    if (!fakeNode.next) {
        fakeNode.next = fakeNode.prev = new Node(newObject);
    } else {
        auto oldHead = fakeNode.next;
        fakeNode.next = new Node(newObject);
        fakeNode.next->next = oldHead;
        oldHead->prev = fakeNode.next;
    }
    ++size_;
}

void ObjectList::pop_front() {
    if (!fakeNode.next)
        return;

    auto oldHead = fakeNode.next;
    if (fakeNode.next == fakeNode.prev) {
        fakeNode.next = fakeNode.prev = nullptr;
    } else {
        fakeNode.next = fakeNode.next->next;
        fakeNode.next->prev = nullptr;
    }
    delete oldHead;
    --size_;
}
