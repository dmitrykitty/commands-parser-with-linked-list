#include <memory>
#include <stdexcept>
#include <utility>  // std::exchange()
#include "objectlist.h"
#include "object.h"


ObjectList::ObjectList(): fakeNode{&fakeNode, &fakeNode} {}

ObjectList::ObjectList(const ObjectList& other): ObjectList() {
    for (auto& obj : other) {
        push_back(obj.clone());
    }
}

ObjectList::ObjectList(ObjectList&& other) noexcept {
    size_ = other.size_;
    fakeNode.next = other.fakeNode.next;
    fakeNode.prev = other.fakeNode.prev;

    if (fakeNode.next != &other.fakeNode)
        fakeNode.next->prev = &fakeNode;
    if (fakeNode.prev != &other.fakeNode)
        fakeNode.prev->next = &fakeNode;

    other.fakeNode.next = &other.fakeNode;
    other.fakeNode.prev = &other.fakeNode;
    other.size_ = 0;
}

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

    auto newNode = new Node(newObject);
    newNode->next = fakeNode.next;
    newNode->prev = &fakeNode;
    fakeNode.next->prev = newNode;
    fakeNode.next = newNode;

    ++size_;
}


void ObjectList::push_back(value_type* newObject) {
    if (contains(*newObject))
        return;
    auto newNode = new Node(newObject);
    newNode->prev = fakeNode.prev;
    newNode->next = &fakeNode;
    fakeNode.prev->next = newNode;
    fakeNode.prev = newNode;

    ++size_;
}

void ObjectList::pop_back() {
    if (fakeNode.next == &fakeNode)
        return;

    Node* last = fakeNode.prev;
    Node* beforeLast = last->prev;

    fakeNode.prev = beforeLast;
    beforeLast->next = &fakeNode;

    --size_;
}


void ObjectList::pop_front() {
    if (fakeNode.next == &fakeNode)
        return;

    Node* first = fakeNode.next;
    Node* second = first->next;

    fakeNode.next = second;
    second->prev = &fakeNode;

    delete first;
    --size_;
}

void ObjectList::clear() {
    if (!fakeNode.next)
        return;

    Node* current = fakeNode.next;
    while (current != &fakeNode) {
        Node* next = current->next;
        delete current;
        current = next;
    }

    fakeNode.next = fakeNode.prev = &fakeNode;
    size_ = 0;
}

void ObjectList::reverse() noexcept {
    auto p = &fakeNode;
    do {
        std::swap(p->next, p->prev);
        p = p->prev;
    } while (p != &fakeNode);
}

void ObjectList::erase(iterator position) {
    if (position == end())
        return;

    Node* node = position.ptrNode;

    if (node == fakeNode.next) {
        pop_front();
        return;
    }

    if (node == fakeNode.prev) {
        pop_back();
        return;
    }

    Node* before = node->prev;
    Node* after = node->next;
    before->next = after;
    after->prev = before;

    delete node;
    --size_;
}

void ObjectList::erase_after(iterator position) {
    auto nextPosition = position;
    ++nextPosition;

    if (position == end() || nextPosition == end())
        return;

    erase(nextPosition);
}

// void ObjectList::swap(ObjectList& other) noexcept {
//     std::swap(size_, other.size_);
//     std::swap(fakeNode.next, other.fakeNode.next);
//     std::swap(fakeNode.prev, other.fakeNode.prev);
//
// }

ObjectList& ObjectList::operator=(const ObjectList& other) {
    if (this == &other)
        return *this;
    clear();
    for (auto& obj : other) {
        push_back(obj.clone());
    }
    return *this;
}
ObjectList& ObjectList::operator=(ObjectList&& other)noexcept {
    if (this != &other) {
        clear();

        size_ = other.size_;
        fakeNode.next = other.fakeNode.next;
        fakeNode.prev = other.fakeNode.prev;

        if (fakeNode.next != &other.fakeNode)
            fakeNode.next->prev = &fakeNode;
        if (fakeNode.prev != &other.fakeNode)
            fakeNode.prev->next = &fakeNode;

        other.fakeNode.next = &other.fakeNode;
        other.fakeNode.prev = &other.fakeNode;
        other.size_ = 0;
    }
    return *this;
}

