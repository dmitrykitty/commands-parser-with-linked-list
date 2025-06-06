#pragma once

#include <cinttypes> // std::size_t
#include <memory>    // std::unique_ptr (dla chętnych)
#include <set>

//#define UNIMPLEMENTED_OBJECT_LIST_CONSTRUCTOR_SIZE_EMPTY_METHODS
//#define UNIMPLEMENTED_PUSH_FRONT
//#define UNIMPLEMENTED_POP_FRONT
//#define UNIMPLEMENTED_FRONT
//#define UNIMPLEMENTED_CLEAR
//#define UNIMPLEMENTED_REVERSE // rozmowa kwalifikacyjna
//#define UNIMPLEMENTED_ITERATOR_METHODS_AT_LEAST_EMPTY
//#define UNIMPLEMENTED_ITERATORS
//#define UNIMPLEMENTED_ERASE_AFTER
//#define UNIMPLEMENTED_ERASE


class Object;


class ObjectList {
public:
    using value_type = Object;

private:
    struct Node {
        Node* next{};
        Node* prev{};
        value_type* valueObject{};

        Node() = default;
        Node(Node* prev, Node* next): next(next), prev(prev){}
        explicit Node(value_type* obj): valueObject(obj) {}
    };

    Node fakeNode;
    std::size_t size_{};

public:
    struct iterator {
        using iterator_category = std::bidirectional_iterator_tag; //dla listy dwukierunkowej,
        //dla jednokierunkowej std::forward_iterator_tag

        using pointer = Object *;
        using reference = Object &;
        using value_type = Object;
        using difference_type = std::ptrdiff_t;

        Node* ptrNode{};
        Node* tailPtr{}; //aby --end() dawał tail

        iterator() = default;

        iterator(Node* ptr): ptrNode(ptr) {}

        reference operator*() const { return *ptrNode->valueObject; }
        pointer operator->() const { return ptrNode->valueObject; }

        iterator& operator++() {
            ptrNode = ptrNode->next;
            return *this;
        }

        iterator operator++(int) {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        iterator& operator--() {
            ptrNode = ptrNode->prev;
            return *this;
        }

        iterator operator--(int) {
            auto tmp = *this;
            --*this;
            return tmp;
        }

        bool operator==(const iterator& o) const { return ptrNode == o.ptrNode; }
        bool operator!=(const iterator& o) const { return ptrNode != o.ptrNode; }
    };

    [[nodiscard]] iterator begin() const { return {fakeNode.next}; }
    [[nodiscard]] iterator end() const { return {const_cast<Node *>(&fakeNode)}; }


    ObjectList();
    ObjectList(const ObjectList& other);
    ObjectList(ObjectList&& other) noexcept ;
    ~ObjectList();

    ObjectList& operator=(const ObjectList& other);
    ObjectList& operator=(ObjectList&& other) noexcept ;

    void clear();

    void reverse() noexcept;

    [[nodiscard]] value_type& front() const { return *fakeNode.next->valueObject; };


    /**
     * @brief push_front
     * @param newObject
     * Wpierw sprawdzamy czy obiekt już jest w liście
     */
    void push_front(value_type* newObject);
    void push_back(value_type* newObject);
    [[nodiscard]] std::size_t size() const { return size_; }
    [[nodiscard]] bool empty() const { return size_ == 0; }

    void pop_front();
    void pop_back();

    void erase_after(iterator position);

    /// tego std::forward_list nie ma
    void erase(iterator position);

    /// dla chętnych, tego testy nie sprawdzają:
    ObjectList reverse_copy() const;

    /// dla chętnych, tego testy nie sprawdzają:
    void sort() noexcept;

protected:
    [[nodiscard]] bool contains(const value_type& obj) const;
    void swap(ObjectList& other) noexcept;
};
