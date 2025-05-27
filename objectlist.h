#pragma once

#include <cinttypes> // std::size_t
#include <memory>    // std::unique_ptr (dla chętnych)
#include <set>

//#define UNIMPLEMENTED_OBJECT_LIST_CONSTRUCTOR_SIZE_EMPTY_METHODS
//#define UNIMPLEMENTED_PUSH_FRONT
//#define UNIMPLEMENTED_POP_FRONT
//#define UNIMPLEMENTED_FRONT
#define UNIMPLEMENTED_CLEAR
#define UNIMPLEMENTED_REVERSE // rozmowa kwalifikacyjna
#define UNIMPLEMENTED_ITERATOR_METHODS_AT_LEAST_EMPTY
#define UNIMPLEMENTED_ITERATORS
#define UNIMPLEMENTED_ERASE_AFTER
#define UNIMPLEMENTED_ERASE


class Object;


class ObjectList {
public:
    using value_type = Object;

private:
    struct Node {
        Node* next{};
        value_type* valueObject;

        Node() = default;
        explicit Node(value_type* obj): valueObject(obj){}
    };

    Node* head{};
    std::size_t size_{};

public:
    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using pointer = Object *;
        using reference = Object &;
        using value_type = Object;
        using difference_type = std::ptrdiff_t;

        Node* ptrNode{};

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

        bool operator==(const iterator& o) const { return ptrNode == o.ptrNode; }
        bool operator!=(const iterator& o) const { return ptrNode != o.ptrNode; }
    };

    iterator begin() { return {head}; };
    iterator end() { return {nullptr}; }


    ObjectList();
    ~ObjectList();

    void clear();

    void reverse() noexcept;

    [[nodiscard]] value_type& front() const { return *head->valueObject; };


    /**
     * @brief push_front
     * @param newObject
     * Wpierw sprawdzamy czy obiekt już jest w liście
     */
    void push_front(value_type* newObject);

    [[nodiscard]] std::size_t size() const { return size_; }
    [[nodiscard]] bool empty() const { return size_ == 0; }

    void pop_front();

    void erase_after(iterator position);

    /// tego std::forward_list nie ma
    void erase(iterator position);

    /// dla chętnych, tego testy nie sprawdzają:
    ObjectList reverse_copy() const;

    /// dla chętnych, tego testy nie sprawdzają:
    void sort() noexcept;

protected:
    bool contains(const value_type& obj);
};
